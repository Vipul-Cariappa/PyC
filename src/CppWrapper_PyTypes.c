#define PY_SSIZE_T_CLEAN
#include "CppTypeInfo.h"
#include "PyC.h"
#include "Py_C_Types.h"
#include "Python.h"
#include <dlfcn.h>
#include <errno.h>
#include <stdbool.h>
#include <stdlib.h>

CXString (*mangled_name_getter_fn)(CXCursor) = &clang_getCursorSpelling;

CXString GET_MANGLED_NAME(CXCursor cursor) {
  return mangled_name_getter_fn(cursor);
}

typedef struct PyC_CppModule {
  PyObject_HEAD const char *library_name;
  const char *header_name;
  void *so;
  Symbols *symbols;
  int loaded;
  PyObject *cache_dict; // cache created function wrappers and structs
} PyC_CppModule;

typedef struct PyC_CppFunction {
  PyObject_HEAD Function *funcType;
  void *so;
  PyObject *parentModule;
} PyC_CppFunction;

typedef struct PyC_CppStruct // FIXME: modify it with new design
{
  PyObject_HEAD Structure *structType;
  void *data;
} PyC_CppStruct;

PyTypeObject py_CppModuleType = {
    PyVarObject_HEAD_INIT(NULL, 0).tp_name = "PyCpp.CppModule",
    .tp_basicsize = sizeof(PyC_CppModule),
    .tp_itemsize = 0,
    .tp_getattr = &Cpp_ModuleGet,
    .tp_setattr = &Cpp_ModuleSet,
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_doc = CPP_MODULE_DOC_STRING,
    .tp_init = &Cpp_ModuleInit,
    .tp_new = PyType_GenericNew,
    .tp_finalize = &Cpp_ModuleGC,
};

PyTypeObject py_CppFunctionType = {
    PyVarObject_HEAD_INIT(NULL, 0).tp_name = "PyCpp.CppFunction",
    .tp_basicsize = sizeof(PyC_CppFunction),
    .tp_itemsize = 0,
    .tp_call = &Cpp_FunctionCall,
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_doc = CPP_FUNCTION_DOC_STRING,
    .tp_new = PyType_GenericNew,
    .tp_finalize = &Cpp_FunctionGC,
};

PyMethodDef PyC_Methods[] = {
    {"LoadCpp", (PyCFunction)load_cpp, METH_VARARGS | METH_KEYWORDS,
     LOAD_CPP_DOC_STRING},
    {"print_CppModule", (PyCFunction)print_PyC_CppModule, METH_VARARGS,
     PRINT_CPPMODULE_DOC_STRING},
    {NULL, NULL, 0, NULL}};

PyModuleDef PyC_Module = {PyModuleDef_HEAD_INIT, "PyCpp", "PyCpp", -1,
                          PyC_Methods};

// loads *.so file into the running program and parses the header file for
// symbols
static PyObject *load_cpp(PyObject *self, PyObject *args, PyObject *kwargs) {
  PyObject *obj = PyObject_GetAttrString(PyC, "CppModule");
  if (obj) {
    return PyObject_Call(obj, args, kwargs);
  }

  PyErr_SetString(py_BindingError, "Unable to access PyCpp.CppModule");
  return NULL;
}

// print symbols of given CppModule
static PyObject *print_PyC_CppModule(PyObject *self, PyObject *args,
                                     PyObject *kwargs) {
  PyObject *py_PyC_CppModule;

  if (!PyArg_ParseTuple(args, "O", &py_PyC_CppModule))
    return NULL;

  PyC_CppModule *type_PyC_CppModule = (PyC_CppModule *)py_PyC_CppModule;
  print_Symbols(type_PyC_CppModule->symbols);

  Py_RETURN_NONE;
}

static int Cpp_ModuleInit(PyObject *self, PyObject *args, PyObject *kwargs) {
  PyC_CppModule *selfType = (PyC_CppModule *)self;

  const char *library;
  const char *header;
  if (!PyArg_ParseTuple(args, "ss", &library, &header)) {
    return 0;
  }

  if (kwargs) {
    PyObject *cpp = PyUnicode_FromString("cpp");
    if (PyDict_Contains(kwargs, cpp)) {
      PyObject *mode = PyDict_GetItem(kwargs, cpp);

      if (mode == Py_True) {
        mangled_name_getter_fn = &clang_Cursor_getMangling;
      } else if (mode == Py_False) {
        mangled_name_getter_fn = &clang_getCursorSpelling;
      } else {
        PyErr_SetString(
            PyExc_TypeError,
            "Expected boolen value. Got non boolean value for mode");
        return -1;
      }
    }
    Py_DECREF(cpp);
  } else {
    mangled_name_getter_fn = &clang_getCursorSpelling;
  }

  selfType->header_name = header;
  selfType->library_name = library;
  selfType->symbols = create_Symbol(header);
  if (!Symbols_parse(selfType->symbols, header)) {
    free_Symbols(selfType->symbols);

    if (!PyErr_Occurred()) {
      PyErr_SetString(py_BindingError, "Unable to parse translation unit. "
                                       "Possible reasons: file does not exist");
    }
    return -1;
  }

  // print_Symbols(selfType->symbols);

  // opening the shared library
  void *so = dlopen(library, RTLD_NOW);
  if (!so) {
    PyErr_SetString(py_BindingError, "Unable to load the shared library. "
                                     "Possible reasons: file does not exist");
    return -1;
  }

  selfType->so = so;

  // create dict for caching
  selfType->cache_dict = PyDict_New();

  return 0;
}

// PyCpp.CppModule.__get__
static PyObject *Cpp_ModuleGet(PyObject *self, char *attr) {
  PyC_CppModule *selfType = (PyC_CppModule *)self;

  PyObject *py_attr_name =
      PyUnicode_FromString(attr); // TODO: decrement refrence count

  int cached = PyDict_Contains(selfType->cache_dict, py_attr_name);
  if (cached == 1) {
    PyObject *result = PyDict_GetItem(selfType->cache_dict, py_attr_name);
    Py_INCREF(result);
    Py_DECREF(py_attr_name);
    return result;
  } else if (cached == -1) {
    return NULL;
  }

  Function *funcType = Symbols_getFunction(selfType->symbols, attr);

  if (funcType) {
    PyObject *obj = PyObject_GetAttrString(PyC, "CppFunction");
    if (obj) {
      PyC_CppFunction *pyCppFunction =
          (PyC_CppFunction *)PyObject_CallObject(obj, NULL);
      pyCppFunction->funcType = funcType;
      pyCppFunction->so = selfType->so;
      pyCppFunction->parentModule = self;

      if (PyDict_SetItem(selfType->cache_dict, py_attr_name,
                         (PyObject *)pyCppFunction)) {
        return NULL;
      }

      return (PyObject *)pyCppFunction;
    }
  } else if (errno != 0)
    return NULL;

  Global *globalVar = Symbols_getGlobal(selfType->symbols, attr);

  if (globalVar) {
    void *var = dlsym(selfType->so, attr);

    if (!var) {
      PyErr_SetString(py_CppError, dlerror());
      return NULL;
    }

    return cppArg_to_pyArg(var, globalVar->type, globalVar->underlyingType,
                           NULL, NULL, self); // TODO: update NULL for global structs
  } else if (errno != 0)
    return NULL;

  Structure *structVar = Symbols_getStructure(selfType->symbols, attr);

  if (structVar) {
    PyObject *result = create_py_c_struct(structVar, self);
    if (PyDict_SetItem(selfType->cache_dict, py_attr_name, result)) {
      return NULL;
    }

    return result;
  }

  else if (errno != 0)
    return NULL;
  
  Union *unionVar = Symbols_getUnion(selfType->symbols, attr);

  if (unionVar) {
    PyObject *result = create_py_c_union(unionVar, self);
    if (PyDict_SetItem(selfType->cache_dict, py_attr_name, result)) {
      return NULL;
    }

    return result;
  }

  else if (errno != 0)
    return NULL;

  PyObject *value = PyObject_GenericGetAttr(self, PyUnicode_FromString(attr));
  if (value)
    return value;

  PyErr_SetString(py_CppError,
                  "Could not find any declaration with given name");
  return NULL;
}

static int Cpp_ModuleSet(PyObject *self, char *attr, PyObject *pValue) {
  PyErr_SetString(py_BindingError, "Cannot Assign Values to LuaModule Objects");
  return -1;
}

static void Cpp_ModuleGC(PyObject *self) {
  PyC_CppModule *selfType = (PyC_CppModule *)self;
  dlclose(selfType->so);
  free_Symbols(selfType->symbols);
}

// PyCpp.CppFunction.__call__
PyObject *Cpp_FunctionCall(PyObject *self, PyObject *args, PyObject *kwargs) {
  PyC_CppFunction *selfType = (PyC_CppFunction *)self;

  int funcNum = match_ffi_type_to_defination(selfType->funcType, args);

  if (funcNum == -1) {
    PyErr_SetString(
        py_CppError,
        "Could not find function with given declaration with same name");
    return NULL;
  } else if (funcNum == -2) {
    PyErr_SetString(py_BindingError, "Type Convertion of given function "
                                     "declaration is not implemented.");
    return NULL;
  }

  FunctionType *funcType =
      qvector_getat(selfType->funcType->functionTypes, funcNum, false);

  // getting function
  void *func =
      dlsym(selfType->so,
            qlist_getat(selfType->funcType->mangledNames, funcNum, NULL,
                        false)); // TODO: store the func* in FunctionType
  if (!func) {
    PyErr_SetString(py_CppError, dlerror());
    return NULL;
  }

  size_t args_count = funcType->argsCount;

  qvector_t *args_list = funcType->argsType;
  ffi_cif cif;
  ffi_type **ffi_args =
      (ffi_type **)malloc(sizeof(ffi_type *) * (args_count + 1));
  void *rc = malloc(sizeof(funcType->returnType.size));

  for (int i = 0; i < args_count; i++) {
    ffi_args[i] = qvector_getat(args_list, i, false);
  }
  ffi_args[args_count] = NULL;

  void **args_values = pyArgs_to_cppArgs(args, args_list);
  if (!args_values) {
    return NULL;
  }

  if (ffi_prep_cif(&cif, FFI_DEFAULT_ABI, args_count, &funcType->returnType,
                   ffi_args) == FFI_OK) {
    ffi_call(&cif, (void (*)())func, rc, args_values);
  }

  return cppArg_to_pyArg(
      rc, funcType->returnType, funcType->returnsUnderlyingType,
      funcType->returnUnderlyingStruct,
      funcType->returnUnderlyingUnion, selfType->parentModule);
}

// PyCpp.CppFunction.__del__
static void Cpp_FunctionGC(PyObject *self) {
  // TODO: implement
  PyC_CppFunction *selfType = (PyC_CppFunction *)self;
  return;
}
