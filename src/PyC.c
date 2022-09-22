#define PY_SSIZE_T_CLEAN
#include "PyC.h"
#include "Py_C_Types.h"
#include "Python.h"

PyObject *PyC;
p_void_array_t *EXTRA_HEAP_MEMORY;

PyObject *py_CppError;
PyObject *py_BindingError;

PyMODINIT_FUNC PyInit_PyC(void) {
  PyObject *m;

  EXTRA_HEAP_MEMORY = p_void_array_new();
  if (!EXTRA_HEAP_MEMORY) {
    return PyErr_NoMemory();
  }

  // creating module
  m = PyModule_Create(&PyC_Module);
  if (m == NULL)
    return NULL;

  // creating CppModuleType
  if (PyType_Ready(&py_CppModuleType) < 0) {
    return NULL;
  }

  Py_INCREF(&py_CppModuleType);
  if (PyModule_AddObject(m, "CppModule", (PyObject *)&py_CppModuleType) < 0) {
    Py_DECREF(&py_CppModuleType);
    Py_DECREF(m);
    return NULL;
  }

  // creating CppFunctionType
  if (PyType_Ready(&py_CppFunctionType) < 0) {
    return NULL;
  }

  Py_INCREF(&py_CppFunctionType);
  if (PyModule_AddObject(m, "CppFunction", (PyObject *)&py_CppFunctionType) <
      0) {
    Py_DECREF(&py_CppFunctionType);
    Py_DECREF(m);
    return NULL;
  }

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

  // creating c_type: c_int
  if (PyType_Ready(&py_c_int_type) < 0) {
    return NULL;
  }

  Py_INCREF(&py_c_int_type);
  if (PyModule_AddObject(m, "c_int", (PyObject *)&py_c_int_type) < 0) {
    Py_DECREF(&py_c_int_type);
    Py_DECREF(m);
    return NULL;
  }

  // creating c_type: c_uint
  if (PyType_Ready(&py_c_uint_type) < 0) {
    return NULL;
  }

  Py_INCREF(&py_c_uint_type);
  if (PyModule_AddObject(m, "c_uint", (PyObject *)&py_c_uint_type) < 0) {
    Py_DECREF(&py_c_uint_type);
    Py_DECREF(m);
    return NULL;
  }

  // creating c_type: c_double
  if (PyType_Ready(&py_c_double_type) < 0) {
    return NULL;
  }

  Py_INCREF(&py_c_double_type);
  if (PyModule_AddObject(m, "c_double", (PyObject *)&py_c_double_type) < 0) {
    Py_DECREF(&py_c_double_type);
    Py_DECREF(m);
    return NULL;
  }

  // creating c_type: c_float
  if (PyType_Ready(&py_c_float_type) < 0) {
    return NULL;
  }

  Py_INCREF(&py_c_float_type);
  if (PyModule_AddObject(m, "c_float", (PyObject *)&py_c_float_type) < 0) {
    Py_DECREF(&py_c_float_type);
    Py_DECREF(m);
    return NULL;
  }

  // creating c_type: c_short
  if (PyType_Ready(&py_c_short_type) < 0) {
    return NULL;
  }

  Py_INCREF(&py_c_short_type);
  if (PyModule_AddObject(m, "c_short", (PyObject *)&py_c_short_type) < 0) {
    Py_DECREF(&py_c_short_type);
    Py_DECREF(m);
    return NULL;
  }

  // creating c_type: c_ushort
  if (PyType_Ready(&py_c_ushort_type) < 0) {
    return NULL;
  }

  Py_INCREF(&py_c_ushort_type);
  if (PyModule_AddObject(m, "c_ushort", (PyObject *)&py_c_ushort_type) < 0) {
    Py_DECREF(&py_c_ushort_type);
    Py_DECREF(m);
    return NULL;
  }

  // creating c_type: c_long
  if (PyType_Ready(&py_c_long_type) < 0) {
    return NULL;
  }

  Py_INCREF(&py_c_long_type);
  if (PyModule_AddObject(m, "c_long", (PyObject *)&py_c_long_type) < 0) {
    Py_DECREF(&py_c_long_type);
    Py_DECREF(m);
    return NULL;
  }

  // creating c_type: c_ulong
  if (PyType_Ready(&py_c_ulong_type) < 0) {
    return NULL;
  }

  Py_INCREF(&py_c_ulong_type);
  if (PyModule_AddObject(m, "c_ulong", (PyObject *)&py_c_ulong_type) < 0) {
    Py_DECREF(&py_c_ulong_type);
    Py_DECREF(m);
    return NULL;
  }

  // creating c_type: c_bool
  if (PyType_Ready(&py_c_bool_type) < 0) {
    return NULL;
  }

  Py_INCREF(&py_c_bool_type);
  if (PyModule_AddObject(m, "c_bool", (PyObject *)&py_c_bool_type) < 0) {
    Py_DECREF(&py_c_bool_type);
    Py_DECREF(m);
    return NULL;
  }

  // creating c_type: c_char
  if (PyType_Ready(&py_c_char_type) < 0) {
    return NULL;
  }

  Py_INCREF(&py_c_char_type);
  if (PyModule_AddObject(m, "c_char", (PyObject *)&py_c_char_type) < 0) {
    Py_DECREF(&py_c_char_type);
    Py_DECREF(m);
    return NULL;
  }

  // creating c_type: c_void
  if (PyType_Ready(&py_c_void_type) < 0) {
    return NULL;
  }

  Py_INCREF(&py_c_void_type);
  if (PyModule_AddObject(m, "c_void", (PyObject *)&py_c_void_type) < 0) {
    Py_DECREF(&py_c_void_type);
    Py_DECREF(m);
    return NULL;
  }

  // creating py_c_struct_type
  if (PyType_Ready(&py_c_struct_type) < 0) {
    return NULL;
  }

  Py_INCREF(&py_c_struct_type);
  if (PyModule_AddObject(m, "c_struct", (PyObject *)&py_c_struct_type) < 0) {
    Py_DECREF(&py_c_struct_type);
    Py_DECREF(m);
    return NULL;
  }

  // creating py_c_union_type
  if (PyType_Ready(&py_c_union_type) < 0) {
    return NULL;
  }

  Py_INCREF(&py_c_union_type);
  if (PyModule_AddObject(m, "c_union", (PyObject *)&py_c_union_type) < 0) {
    Py_DECREF(&py_c_union_type);
    Py_DECREF(m);
    return NULL;
  }

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