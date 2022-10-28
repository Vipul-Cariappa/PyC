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
  PyObject ob_base;
  char *library_name;
  char *header_name;
  void *so;
  Symbols *symbols;
  int loaded;
  PyObject *cache_dict; // cache function wrappers and structs & union
} PyC_CppModule;

typedef struct PyC_CppFunction {
  PyObject ob_base;
  Function *funcType;
  void *so;
  PyObject *parentModule;
} PyC_CppFunction;

PyTypeObject py_CppModuleType = {
    PyVarObject_HEAD_INIT(NULL, 0).tp_name = "PyCpp.CppModule",
    .tp_basicsize                          = sizeof(PyC_CppModule),
    .tp_itemsize                           = 0,
    .tp_getattr                            = &Cpp_ModuleGet,
    .tp_setattr                            = &Cpp_ModuleSet,
    .tp_flags    = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_HAVE_GC,
    .tp_doc      = CPP_MODULE_DOC_STRING,
    .tp_init     = &Cpp_ModuleInit,
    .tp_new      = PyType_GenericNew,
    .tp_dealloc  = &Cpp_ModuleGC,
    .tp_traverse = &Cpp_ModuleTraverse,
    .tp_clear    = &Cpp_ModuleClear,
};

PyTypeObject py_CppFunctionType = {
    PyVarObject_HEAD_INIT(NULL, 0).tp_name = "PyCpp.CppFunction",
    .tp_basicsize                          = sizeof(PyC_CppFunction),
    .tp_itemsize                           = 0,
    .tp_call                               = &Cpp_FunctionCall,
    .tp_flags    = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_HAVE_GC,
    .tp_doc      = CPP_FUNCTION_DOC_STRING,
    .tp_new      = PyType_GenericNew,
    .tp_dealloc  = &Cpp_FunctionGC,
    .tp_traverse = &Cpp_FunctionTraverse,
    .tp_clear    = &Cpp_FunctionClear,
};

// loads *.so file into the running program and parses the header file for
// symbols
PyObject *load_cpp(PyObject *self, PyObject *args, PyObject *kwargs) {
  PyObject *obj = PyObject_GetAttrString(PyC, "CppModule");
  if (obj) {
    PyObject *result = PyObject_Call(obj, args, kwargs);
    Py_DECREF(obj);
    return result;
  }

  PyErr_SetString(py_BindingError, "Unable to access PyCpp.CppModule");
  return NULL;
}

// print symbols of given CppModule
PyObject *
print_PyC_CppModule(PyObject *self, PyObject *args, PyObject *kwargs) {
  PyObject *py_PyC_CppModule;

  if (!PyArg_ParseTuple(args, "O", &py_PyC_CppModule))
    return NULL;

  print_Symbols(((PyC_CppModule *)py_PyC_CppModule)->symbols);

  Py_RETURN_NONE;
}

static int Cpp_ModuleInit(PyObject *self, PyObject *args, PyObject *kwargs) {
  PyC_CppModule *selfType = (PyC_CppModule *)self;

  char *library;
  char *header;
  int is_cpp_file = false;

  // parsing arguments
  static char *kwlist[] = {"library", "header", "cpp", NULL};

  if (!PyArg_ParseTupleAndKeywords(args,
                                   kwargs,
                                   "ss|p",
                                   kwlist,
                                   &library,
                                   &header,
                                   &is_cpp_file)) {
    return -1;
  }

  library = strdup(library);
  if (library == NULL) {
    PyErr_NoMemory();
    return -1;
  }

  header = strdup(header);
  if (header == NULL) {
    PyErr_NoMemory();
    return -1;
  }

  if (is_cpp_file) {
    mangled_name_getter_fn = &clang_Cursor_getMangling;
  } else {
    mangled_name_getter_fn = &clang_getCursorSpelling;
  }

  selfType->header_name  = header;
  selfType->library_name = library;

  // parsing header file
  selfType->symbols = create_Symbol(header);
  if (selfType->symbols == NULL) {
    return -1;
  }

  if (Symbols_parse(selfType->symbols, header) == false) {
    if (PyErr_Occurred() == NULL) {
      PyErr_SetString(py_CppError,
                      "Unable to parse translation unit. "
                      "Possible reasons: file does not exist");
    }
    return -1;
  }

  // loading in the shared library
  void *so = dlopen(library, RTLD_NOW);
  if (!so) {
    PyErr_Format(py_CppError,
                 "Unable to load the shared library: %s",
                 dlerror());
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

  PyObject *py_attr_name = PyUnicode_FromString(attr);

  // checking if cached
  int cached = PyDict_Contains(selfType->cache_dict, py_attr_name);
  if (cached == 1) {
    PyObject *result = PyDict_GetItem(selfType->cache_dict, py_attr_name);
    Py_INCREF(result);
    Py_DECREF(py_attr_name);
    return result;
  } else if (cached == -1) {
    // error occured
    Py_DECREF(py_attr_name);
    return NULL;
  }

  // checking for function of same name
  Function *funcType = Symbols_getFunction(selfType->symbols, attr);
  if (funcType) {
    PyC_CppFunction *result =
        (PyC_CppFunction *)_PyObject_New(&py_CppFunctionType);
    if (!result) {
      return NULL;
    }

    result->funcType = funcType;
    result->so       = selfType->so;
    Py_INCREF(self);
    result->parentModule = self;

    if (PyDict_SetItem(selfType->cache_dict,
                       py_attr_name,
                       (PyObject *)result)) {
      Py_DECREF(result);
      Py_DECREF(py_attr_name);
      return NULL;
    }

    Py_DECREF(py_attr_name);
    return (PyObject *)result;
  }

  // checking for global decleration of same name
  Global *globalVar = Symbols_getGlobal(selfType->symbols, attr);
  if (globalVar) {
    void *var = dlsym(selfType->so, attr);

    if (var == NULL) {
      Py_DECREF(py_attr_name);
      PyErr_SetString(py_CppError, dlerror());
      return NULL;
    }

    return cppArg_to_pyArg(var, globalVar->typeCXX, globalVar->extraInfo, self);
  }

  // checking for struct of same name
  Structure *structVar = Symbols_getStructure(selfType->symbols, attr);
  if (structVar) {
    PyObject *result = create_py_c_struct(structVar, self);
    if (result == NULL) {
      return NULL;
    }

    if (PyDict_SetItem(selfType->cache_dict, py_attr_name, result)) {
      Py_DECREF(py_attr_name);
      return NULL;
    }

    return result;
  }

  // checking for union of same name
  Union *unionVar = Symbols_getUnion(selfType->symbols, attr);
  if (unionVar) {
    PyObject *result = create_py_c_union(unionVar, self);
    if (result == NULL) {
      return NULL;
    }

    if (PyDict_SetItem(selfType->cache_dict, py_attr_name, result)) {
      Py_DECREF(py_attr_name);
      return NULL;
    }
    Py_DECREF(py_attr_name);
    return result;
  }

  // checking for typedef of same name
  TypeDef *tdVar = Symbols_getTypeDef(selfType->symbols, attr);
  if (tdVar) {
    switch (tdVar->type) {
    case CXX_VoidPointer:
      Py_DECREF(py_attr_name);
      Py_INCREF((PyObject *)&py_c_void_type);
      return (PyObject *)&py_c_void_type;
    case CXX_Char:
    case CXX_CharPointer:
      Py_DECREF(py_attr_name);
      Py_INCREF((PyObject *)&py_c_char_type);
      return (PyObject *)&py_c_char_type;
    case CXX_Bool:
    case CXX_BoolPointer:
      Py_DECREF(py_attr_name);
      Py_INCREF((PyObject *)&py_c_bool_type);
      return (PyObject *)&py_c_bool_type;
    case CXX_Short:
    case CXX_ShortPointer:
      Py_DECREF(py_attr_name);
      Py_INCREF((PyObject *)&py_c_short_type);
      return (PyObject *)&py_c_short_type;
    case CXX_UShort:
    case CXX_UShortPointer:
      Py_DECREF(py_attr_name);
      Py_INCREF((PyObject *)&py_c_ushort_type);
      return (PyObject *)&py_c_ushort_type;
    case CXX_Int:
    case CXX_IntPointer:
      Py_DECREF(py_attr_name);
      Py_INCREF((PyObject *)&py_c_int_type);
      return (PyObject *)&py_c_int_type;
    case CXX_UInt:
    case CXX_UIntPointer:
      Py_DECREF(py_attr_name);
      Py_INCREF((PyObject *)&py_c_uint_type);
      return (PyObject *)&py_c_uint_type;
    case CXX_Long:
    case CXX_LongPointer:
      Py_DECREF(py_attr_name);
      Py_INCREF((PyObject *)&py_c_long_type);
      return (PyObject *)&py_c_long_type;
    case CXX_ULong:
    case CXX_ULongPointer:
      Py_DECREF(py_attr_name);
      Py_INCREF((PyObject *)&py_c_ulong_type);
      return (PyObject *)&py_c_ulong_type;
    case CXX_Float:
    case CXX_FloatPointer:
      Py_DECREF(py_attr_name);
      Py_INCREF((PyObject *)&py_c_float_type);
      return (PyObject *)&py_c_float_type;
    case CXX_Double:
    case CXX_DoublePointer:
      Py_DECREF(py_attr_name);
      Py_INCREF((PyObject *)&py_c_double_type);
      return (PyObject *)&py_c_double_type;
    case CXX_Struct:
    case CXX_StructPointer:
      Py_DECREF(py_attr_name);
      return Cpp_ModuleGet(self,
                           (char *)(tdVar->typedef_name) +
                               7); // +7 offset to skip "struct " at start
    case CXX_Union:
    case CXX_UnionPointer:
      Py_DECREF(py_attr_name);
      return Cpp_ModuleGet(self,
                           (char *)(tdVar->typedef_name) +
                               6); // +6 offset to skip "union " at start
    default:
      Py_DECREF(py_attr_name);
      return PyErr_Format(py_BindingError,
                          "Could not figure out typedef's underlying type. "
                          "type name: %s typedef name: %s",
                          tdVar->name,
                          tdVar->typedef_name);
    }
  }

  // checking for enum value with same name
  Enum *enumVar = Symbols_getEnum(selfType->symbols, attr);
  if (enumVar) {
    PyObject *result = PyLong_FromLong(enumVar->value);
    if (PyDict_SetItem(selfType->cache_dict, py_attr_name, result)) {
      Py_DECREF(py_attr_name);
      return NULL;
    }
    Py_DECREF(py_attr_name);
    return result;
  }

  Py_DECREF(py_attr_name);
  return PyObject_GenericGetAttr(self, PyUnicode_FromString(attr));
}

// PyCpp.CppModule.__set__
static int Cpp_ModuleSet(PyObject *self, char *attr, PyObject *pValue) {
  PyErr_SetString(PyExc_TypeError, "Cannot Assign Values to CppModule Objects");
  return -1;
}

// PyCpp.CppModule.__del__
static void Cpp_ModuleGC(PyObject *self) {
  PyC_CppModule *selfType = (PyC_CppModule *)self;
  PyObject_GC_UnTrack(self);

  if (selfType->so)
    dlclose(selfType->so);

  if (selfType->header_name)
    free(selfType->header_name);

  if (selfType->library_name)
    free(selfType->library_name);

  if (selfType->symbols)
    free_Symbols(selfType->symbols);

  Cpp_ModuleClear(self);
  Py_TYPE(self)->tp_free((PyObject *)self);
}

static int Cpp_ModuleTraverse(PyObject *self, visitproc visit, void *arg) {
  Py_VISIT(((PyC_CppModule *)self)->cache_dict);
  return 0;
}

static int Cpp_ModuleClear(PyObject *self) {
  Py_CLEAR(((PyC_CppModule *)self)->cache_dict);
  return 0;
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
  }

  FunctionType *funcType =
      FunctionType_array_get_ptr_at(selfType->funcType->functionTypes, funcNum);

  if (!funcType->func) {
    // getting function
    void *func = dlsym(selfType->so, funcType->mangledName);
    if (!func) {
      return PyErr_Format(py_CppError,
                          "Error while loading cpp function: %s",
                          dlerror());
    }

    funcType->func = func;
  }

  size_t args_count                 = funcType->argsCount;
  p_ffi_type_array_t *args_ffi_list = funcType->argsTypeFFI;

  ffi_cif cif;

  // preparing ffi_type array for args
  ffi_type **ffi_args = calloc(args_count + 1, sizeof(ffi_type *));
  if (ffi_args == NULL) {
    return PyErr_NoMemory();
  }

  void **args_values = calloc(args_count + 1, sizeof(void *));
  if (args_values == NULL) {
    free(ffi_args);
    return PyErr_NoMemory();
  }

  bool *locations_to_free = calloc(args_count + 1, sizeof(bool));
  if (ffi_args == NULL) {
    free(ffi_args);
    free(args_values);
    return PyErr_NoMemory();
  }

  for (int i = 0; i < args_count; i++) {
    ffi_args[i] = p_ffi_type_array_getat(args_ffi_list, i);

    PyObject *arg              = PyTuple_GetItem(args, i);
    enum CXX_Type arg_cxx_type = CXX_Type_array_getat(funcType->argsTypeCXX, i);

    args_values[i] = pyArg_to_cppArg(arg, arg_cxx_type, locations_to_free + i);
  }
  ffi_args[args_count]    = NULL;
  args_values[args_count] = NULL;

  // preparing ffi_type for return value
  void *rc = malloc(funcType->returnTypeFFI->size);
  if (ffi_args == NULL) {
    free(ffi_args);
    free(args_values);
    free(locations_to_free);
    return PyErr_NoMemory();
  }

  if (ffi_prep_cif(&cif,
                   FFI_DEFAULT_ABI,
                   args_count,
                   funcType->returnTypeFFI,
                   ffi_args) == FFI_OK) {
    ffi_call(&cif, funcType->func, rc, args_values);
  }

  // clear allocated memory
  for (int i = 0; i < args_count; i++) {
    if (locations_to_free[i] == true) {
      free(args_values[i]);
    }
  }
  free(ffi_args);
  free(args_values);
  free(locations_to_free);
  // TODO: free rc based of type

  return cppArg_to_pyArg(rc,
                         funcType->returnTypeCXX,
                         funcType->returnTypeInfo,
                         selfType->parentModule);
}

// PyCpp.CppFunction.__del__
static void Cpp_FunctionGC(PyObject *self) {
  PyObject_GC_UnTrack(self);
  Cpp_FunctionClear(self);
  Py_TYPE(self)->tp_free((PyObject *)self);
}

static int Cpp_FunctionTraverse(PyObject *self, visitproc visit, void *arg) {
  Py_VISIT(((PyC_CppFunction *)self)->parentModule);
  return 0;
}

static int Cpp_FunctionClear(PyObject *self) {
  Py_CLEAR(((PyC_CppFunction *)self)->parentModule);
  return 0;
}
