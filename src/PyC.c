#define PY_SSIZE_T_CLEAN
#include "Python.h"

#include "PyC.h"

PyObject* PyC;

PyObject *py_CppError;
PyObject *py_BindingError;


PyMODINIT_FUNC PyInit_PyC(void) {
  PyObject *m;

  m = PyModule_Create(&PyC_Module);
  if (m == NULL)
    return NULL;

  if (PyType_Ready(&py_CppModuleType) < 0) {
    return NULL;
  }

  Py_INCREF(&py_CppModuleType);
  if (PyModule_AddObject(m, "CppModule", (PyObject *)&py_CppModuleType) < 0) {
    Py_DECREF(&py_CppModuleType);
    Py_DECREF(m);
    return NULL;
  }

  if (PyType_Ready(&py_CppFunctionType) < 0) {
    return NULL;
  }

  Py_INCREF(&py_CppFunctionType);
  if (PyModule_AddObject(m, "CppFunction", (PyObject *)&py_CppFunctionType) < 0) {
    Py_DECREF(&py_CppFunctionType);
    Py_DECREF(m);
    return NULL;
  }

  if (PyType_Ready(&py_CppStructType) < 0) {
    return NULL;
  }

  Py_INCREF(&py_CppStructType);
  if (PyModule_AddObject(m, "CppStruct", (PyObject *)&py_CppStructType) < 0) {
    Py_DECREF(&py_CppStructType);
    Py_DECREF(m);
    return NULL;
  }

  py_CppError = PyErr_NewException("PyCpp.CppError", NULL, NULL);
  Py_XINCREF(py_CppError);
  if (PyModule_AddObject(m, "CppError", py_CppError) < 0) {
    Py_XDECREF(py_CppError);
    Py_CLEAR(py_CppError);
    Py_DECREF(m);
    return NULL;
  }

  py_BindingError = PyErr_NewException("PyCpp.BindingError", NULL, NULL);
  Py_XINCREF(py_BindingError);
  if (PyModule_AddObject(m, "BindingError", py_BindingError) < 0) {
    Py_XDECREF(py_BindingError);
    Py_CLEAR(py_BindingError);
    Py_DECREF(m);
    return NULL;
  }

  PyC = m;
  return m;
}
