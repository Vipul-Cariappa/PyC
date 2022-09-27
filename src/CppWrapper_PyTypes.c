#define PY_SSIZE_T_CLEAN
#include "CppTypeInfo.h"
#include "PyC.h"
#include "Py_C_Types.h"
#include "Python.h"
#if defined(__linux__)
#include <dlfcn.h>
#endif
#if defined(_WIN32)
#include <Windows.h>
#endif
#include <errno.h>
#include <stdbool.h>
#include <stdlib.h>

#if defined(_WIN32)
void DisplayErrorText(DWORD dwLastError) {
  HMODULE hModule = NULL; // default to system source
  LPSTR MessageBuffer;
  DWORD dwBufferLength;

  DWORD dwFormatFlags = FORMAT_MESSAGE_ALLOCATE_BUFFER |
                        FORMAT_MESSAGE_IGNORE_INSERTS |
                        FORMAT_MESSAGE_FROM_SYSTEM;

  if (dwBufferLength = FormatMessageA(dwFormatFlags, hModule, dwLastError,
                                      MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                                      (LPSTR)&MessageBuffer, 0, NULL)) {
    DWORD dwBytesWritten;

    WriteFile(GetStdHandle(STD_ERROR_HANDLE), MessageBuffer, dwBufferLength,
              &dwBytesWritten, NULL);

    LocalFree(MessageBuffer);
  }
}
#endif

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
  PyObject *cache_dict; // cache function wrappers and structs & union
} PyC_CppModule;

typedef struct PyC_CppFunction {
  PyObject_HEAD Function *funcType;
  void *so;
  PyObject *parentModule;
} PyC_CppFunction;

PyTypeObject py_CppModuleType = {
    PyVarObject_HEAD_INIT(NULL, 0).tp_name = "PyCpp.CppModule",
    .tp_basicsize = sizeof(PyC_CppModule),
    .tp_itemsize = 0,
    .tp_getattr = &Cpp_ModuleGet,
    .tp_setattr = &Cpp_ModuleSet,
    .tp_flags = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_HAVE_GC,
    .tp_doc = CPP_MODULE_DOC_STRING,
    .tp_init = &Cpp_ModuleInit,
    .tp_new = PyType_GenericNew,
    .tp_dealloc = &Cpp_ModuleGC,
    .tp_traverse = &Cpp_ModuleTraverse,
    .tp_clear = &Cpp_ModuleClear,
};

PyTypeObject py_CppFunctionType = {
    PyVarObject_HEAD_INIT(NULL, 0).tp_name = "PyCpp.CppFunction",
    .tp_basicsize = sizeof(PyC_CppFunction),
    .tp_itemsize = 0,
    .tp_call = &Cpp_FunctionCall,
    .tp_flags = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_HAVE_GC,
    .tp_doc = CPP_FUNCTION_DOC_STRING,
    .tp_new = PyType_GenericNew,
    .tp_dealloc = &Cpp_FunctionGC,
    .tp_traverse = &Cpp_FunctionTraverse,
    .tp_clear = &Cpp_FunctionClear,
};

PyMethodDef PyC_Methods[] = {
    {"LoadCpp", (PyCFunction)load_cpp, METH_VARARGS | METH_KEYWORDS,
     LOAD_CPP_DOC_STRING},
    {"print_CppModule", (PyCFunction)print_PyC_CppModule, METH_VARARGS,
     PRINT_CPPMODULE_DOC_STRING},
    {NULL, NULL, 0, NULL}};

PyModuleDef PyC_Module = {PyModuleDef_HEAD_INIT, "PyCpp", "PyCpp", -1,
                          PyC_Methods,           NULL,    NULL,    NULL,
                          &PyClear_PyC};

// loads *.so file into the running program and parses the header file for
// symbols
static PyObject *load_cpp(PyObject *self, PyObject *args, PyObject *kwargs) {
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
    return -1;
  }

  library = strdup(library);
  header = strdup(header);
  if (!header && !library) {
    PyErr_NoMemory();
    return -1;
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

  // opening the shared library
#if defined(__linux__)
  void *so = dlopen(library, RTLD_NOW);
#endif

#if defined(_WIN32)
  printf("%s\n", library);

  const size_t cSize = strlen(library) + 1;
  wchar_t *wc = malloc(cSize * sizeof(wchar_t));
  mbstowcs(wc, library, cSize);

  HINSTANCE so = LoadLibrary(wc);

  free(wc);

  // if (!so) {
  // unsigned long error_code = GetLastError();
  // printf("ERRORNO: %d", error_code);
  // DisplayErrorText(error_code);
  // }
#endif

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

  PyObject *py_attr_name = PyUnicode_FromString(attr);

  int cached = PyDict_Contains(selfType->cache_dict, py_attr_name);
  if (cached == 1) {
    PyObject *result = PyDict_GetItem(selfType->cache_dict, py_attr_name);
    Py_INCREF(result);
    Py_DECREF(py_attr_name);
    return result;
  } else if (cached == -1) {
    Py_DECREF(py_attr_name);
    return NULL;
  }

  Function *funcType = Symbols_getFunction(selfType->symbols, attr);

  if (funcType) {
    PyObject *obj = PyObject_GetAttrString(PyC, "CppFunction");
    if (!obj) {
      return NULL;
    }

    PyC_CppFunction *pyCppFunction =
        (PyC_CppFunction *)PyObject_CallObject(obj, NULL);
    pyCppFunction->funcType = funcType;
    pyCppFunction->so = selfType->so;
    Py_INCREF(self);
    pyCppFunction->parentModule = self;

    if (PyDict_SetItem(selfType->cache_dict, py_attr_name,
                       (PyObject *)pyCppFunction)) {
      Py_DECREF(obj);
      Py_DECREF(py_attr_name);
      return NULL;
    }
    Py_DECREF(obj);
    Py_DECREF(py_attr_name);
    return (PyObject *)pyCppFunction;

  } else if (errno != 0) {
    Py_DECREF(py_attr_name);
    return NULL;
  }

  Global *globalVar = Symbols_getGlobal(selfType->symbols, attr);

  if (globalVar) {
#if defined(__linux__)
    void *var = dlsym(selfType->so, attr);

    if (!var) {
      Py_DECREF(py_attr_name);
      PyErr_SetString(py_CppError, dlerror());
      return NULL;
    }
#endif

#if defined(_WIN32)
    void *var = GetProcAddress(selfType->so, attr);

    if (!var) {
      PyErr_SetString(
          py_CppError,
          "Could not get the required symbol"); // TODO: better error message
      return NULL;
    }
#endif

    return cppArg_to_pyArg(var, globalVar->type, globalVar->underlyingType,
                           globalVar->underlyingStruct,
                           globalVar->underlyingUnion, self);
  } else if (errno != 0) {
    Py_DECREF(py_attr_name);
    return NULL;
  }

  Structure *structVar = Symbols_getStructure(selfType->symbols, attr);

  if (structVar) {
    PyObject *result = create_py_c_struct(structVar, self);
    if (PyDict_SetItem(selfType->cache_dict, py_attr_name, result)) {
      Py_DECREF(py_attr_name);
      return NULL;
    }

    return result;
  }

  else if (errno != 0) {
    Py_DECREF(py_attr_name);
    return NULL;
  }

  Union *unionVar = Symbols_getUnion(selfType->symbols, attr);

  if (unionVar) {
    PyObject *result = create_py_c_union(unionVar, self);
    if (PyDict_SetItem(selfType->cache_dict, py_attr_name, result)) {
      Py_DECREF(py_attr_name);
      return NULL;
    }
    Py_DECREF(py_attr_name);
    return result;
  } else if (errno != 0) {
    Py_DECREF(py_attr_name);
    return NULL;
  }

  Py_DECREF(py_attr_name);

  TypeDef *tdVar = Symbols_getTypeDef(selfType->symbols, attr);

  if (tdVar) {

    if (tdVar) {
      switch (tdVar->type) {
      case CXType_Elaborated:
      case CXType_Record: {
        if (strncmp(tdVar->type_name, "struct ", 7) == 0) {
          return Cpp_ModuleGet(self, (char *)(tdVar->type_name) + 7);
        }
        if (strncmp(tdVar->type_name, "union ", 6) == 0) {
          return Cpp_ModuleGet(self, (char *)(tdVar->type_name) + 6);
        }
      }
      default:
        PyErr_SetString(py_BindingError,
                        "Internal Error. "
                        "Could not figure out underlying type of typedef");
        return NULL;
      }
    }
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
  PyErr_SetString(py_BindingError, "Cannot Assign Values to CppModule Objects");
  return -1;
}

static void Cpp_ModuleGC(PyObject *self) {
  PyC_CppModule *selfType = (PyC_CppModule *)self;
#if defined(__linux__)
  dlclose(selfType->so);
#endif

#if defined(_WIN32)
  FreeLibrary(selfType->so);
#endif

  PyObject_GC_UnTrack(self);
  Cpp_ModuleClear(self);

  free((void *)selfType->header_name);
  free((void *)selfType->library_name);

  free_Symbols(selfType->symbols);

  Py_TYPE(self)->tp_free((PyObject *)self);
}

static int Cpp_ModuleTraverse(PyObject *self, visitproc visit, void *arg) {
  PyC_CppModule *selfType = (PyC_CppModule *)self;

  Py_VISIT(selfType->cache_dict);
  return 0;
}

static int Cpp_ModuleClear(PyObject *self) {
  PyC_CppModule *selfType = (PyC_CppModule *)self;

  Py_CLEAR(selfType->cache_dict);
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
  } else if (funcNum == -2) {
    PyErr_SetString(py_BindingError, "Type Convertion of given function "
                                     "declaration is not implemented.");
    return NULL;
  }

  FunctionType *funcType =
      FunctionType_array_get_ptr_at(selfType->funcType->functionTypes, funcNum);

  if (!funcType->func) {

    // getting function
#if defined(__linux__)
    void *func = dlsym(selfType->so, funcType->mangledName);
    if (!func) {
      PyErr_SetString(py_CppError, dlerror());
      return NULL;
    }
#endif

#if defined(_WIN32)
    void *func = GetProcAddress(selfType->so, funcType->mangledName);

    if (!func) {
      PyErr_SetString(
          py_CppError,
          "Could not get the required symbol"); // TODO: better error message
      return NULL;
    }
#endif

    funcType->func = func;
  }
  size_t args_count = funcType->argsCount;

  p_ffi_type_array_t *args_list = funcType->argsType;
  ffi_cif cif;
  ffi_type **ffi_args =
      (ffi_type **)malloc(sizeof(ffi_type *) * (args_count + 1));
  void *rc = malloc(sizeof(funcType->returnType.size));

  for (int i = 0; i < args_count; i++) {
    ffi_args[i] = p_ffi_type_array_getat(args_list, i);
  }
  ffi_args[args_count] = NULL;

  bool *where_to_free = malloc(args_count * sizeof(bool));
  memset(where_to_free, 0, args_count * sizeof(bool));
  void **extra_stuff_to_free = malloc(args_count * sizeof(void *));
  memset(extra_stuff_to_free, 0, args_count * sizeof(void *));

  void **args_values =
      pyArgs_to_cppArgs(args, args_list, where_to_free, extra_stuff_to_free);
  if (!args_values) {
    return NULL;
  }

  if (ffi_prep_cif(&cif, FFI_DEFAULT_ABI, args_count, &funcType->returnType,
                   ffi_args) == FFI_OK) {
    ffi_call(&cif, funcType->func, rc, args_values);
  }

  PyObject *result =
      cppArg_to_pyArg(rc, funcType->returnType, funcType->returnsUnderlyingType,
                      funcType->returnUnderlyingStruct,
                      funcType->returnUnderlyingUnion, selfType->parentModule);

  free(ffi_args);
  for (size_t i = 0; i < args_count; i++) {
    if (where_to_free[i]) {
      free(args_values[i]);
    }
    if (extra_stuff_to_free[i]) {
      free(extra_stuff_to_free[i]);
    }
  }

  free(where_to_free);
  free(extra_stuff_to_free);
  free(args_values);
  free(rc);

  return result;
}

// PyCpp.CppFunction.__del__
static void Cpp_FunctionGC(PyObject *self) {
  PyC_CppFunction *selfType = (PyC_CppFunction *)self;

  PyObject_GC_UnTrack(self);
  Cpp_FunctionClear(self);
  Py_TYPE(self)->tp_free((PyObject *)self);
}

static int Cpp_FunctionTraverse(PyObject *self, visitproc visit, void *arg) {
  PyC_CppFunction *selfType = (PyC_CppFunction *)self;

  Py_VISIT(selfType->parentModule);
  return 0;
}

static int Cpp_FunctionClear(PyObject *self) {
  PyC_CppFunction *selfType = (PyC_CppFunction *)self;

  Py_CLEAR(selfType->parentModule);
  return 0;
}
