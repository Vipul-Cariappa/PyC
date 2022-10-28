#define PY_SSIZE_T_CLEAN
#include "PyC.h"
#include "Py_C_Types.h"
#include "Python.h"

PyObject *PyC;
p_void_array_t *EXTRA_HEAP_MEMORY;

PyObject *py_CppError;
PyObject *py_BindingError;

PyMethodDef PyC_Methods[] = {{"LoadCpp",
                              (PyCFunction)load_cpp,
                              METH_VARARGS | METH_KEYWORDS,
                              LOAD_CPP_DOC_STRING},
                             {"print_CppModule",
                              (PyCFunction)print_PyC_CppModule,
                              METH_VARARGS,
                              PRINT_CPPMODULE_DOC_STRING},
                             {NULL, NULL, 0, NULL}};

PyModuleDef PyC_Module = {PyModuleDef_HEAD_INIT,
                          "PyCpp",
                          "PyCpp",
                          -1,
                          PyC_Methods,
                          NULL,
                          NULL,
                          NULL,
                          &PyClear_PyC};

PyMODINIT_FUNC PyInit_PyC(void) {
  PyObject *m;

  EXTRA_HEAP_MEMORY = p_void_array_new();
  if (!EXTRA_HEAP_MEMORY)
    return PyErr_NoMemory();

  // creating module
  m = PyModule_Create(&PyC_Module);
  if (m == NULL)
    return NULL;

  // creating Exception CppError
  py_CppError = PyErr_NewException("PyCpp.CppError", NULL, NULL);
  Py_XINCREF(py_CppError);
  if (PyModule_AddObject(m, "CppError", py_CppError) < 0) {
    Py_XDECREF(py_CppError);
    Py_CLEAR(py_CppError);
    Py_DECREF(m);
    return NULL;
  }

  // creating Exception BindingError
  py_BindingError = PyErr_NewException("PyCpp.BindingError", NULL, NULL);
  Py_XINCREF(py_BindingError);
  if (PyModule_AddObject(m, "BindingError", py_BindingError) < 0) {
    Py_XDECREF(py_BindingError);
    Py_CLEAR(py_BindingError);
    Py_DECREF(m);
    return NULL;
  }

#define ADD_OBJECT_TO_MODULE(module, object, name)                             \
  if (PyType_Ready(object) < 0) {                                              \
    return NULL;                                                               \
  }                                                                            \
                                                                               \
  Py_INCREF(object);                                                           \
  if (PyModule_AddObject(module, name, (PyObject *)object) < 0) {              \
    Py_DECREF(object);                                                         \
    Py_DECREF(m);                                                              \
  }

  ADD_OBJECT_TO_MODULE(m, &py_CppModuleType, "CppModule")
  ADD_OBJECT_TO_MODULE(m, &py_CppFunctionType, "CppFunction")

  ADD_OBJECT_TO_MODULE(m, &py_c_type_type, "c_type")
  ADD_OBJECT_TO_MODULE(m, &py_c_int_type, "c_int")
  ADD_OBJECT_TO_MODULE(m, &py_c_uint_type, "c_uint")
  ADD_OBJECT_TO_MODULE(m, &py_c_long_type, "c_long")
  ADD_OBJECT_TO_MODULE(m, &py_c_ulong_type, "c_ulong")
  ADD_OBJECT_TO_MODULE(m, &py_c_short_type, "c_short")
  ADD_OBJECT_TO_MODULE(m, &py_c_ushort_type, "c_ushort")
  ADD_OBJECT_TO_MODULE(m, &py_c_bool_type, "c_bool")
  ADD_OBJECT_TO_MODULE(m, &py_c_char_type, "c_char")
  ADD_OBJECT_TO_MODULE(m, &py_c_void_type, "c_void")
  ADD_OBJECT_TO_MODULE(m, &py_c_float_type, "c_float")
  ADD_OBJECT_TO_MODULE(m, &py_c_double_type, "c_double")
  ADD_OBJECT_TO_MODULE(m, &py_c_struct_type, "c_struct")
  ADD_OBJECT_TO_MODULE(m, &py_c_union_type, "c_union")

  PyC = m;
  return m;
}

void PyClear_PyC(void *self) {
  size_t len = p_void_array_size(EXTRA_HEAP_MEMORY);
  for (size_t i = 0; i < len; i++) {
    // free(array_p_void_getat(EXTRA_HEAP_MEMORY, i));
  }

  p_void_array_clear(EXTRA_HEAP_MEMORY);
}
