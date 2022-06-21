#pragma once
#include "Python.h"
#include <stdbool.h>

// c_type: c_int
extern PyTypeObject py_c_int_type;

typedef struct PyC_c_int {
  PyObject_HEAD;
  int value;
  int *pointer;
  bool isPointer;
  bool isArray;
  size_t arraySize;
  size_t arrayCapacity;
} PyC_c_int;

static int c_int_init(PyObject *self, PyObject *args, PyObject *kwargs);
static PyObject *c_int_iter(PyObject *self);
static PyObject *c_int_getattr(PyObject *self, char *attr);
static void c_int_finalizer(PyObject *self);
static PyObject *c_int_append(PyObject *self, PyObject *args);
static PyObject *c_int_pop(PyObject *self);
static PyObject *c_int_value(PyObject *self);
static PyObject *c_int_donot_free(PyObject *self, PyObject *args,
                                  PyObject *kwargs);
static PyObject *c_int_to_pointer(PyObject *self);
static PyObject *c_int_to_int(PyObject *self);
static Py_ssize_t c_int_len(PyObject *self);
static PyObject *c_int_getitem(PyObject *self, PyObject *attr);
static int c_int_setitem(PyObject *self, PyObject *attr, PyObject *value);

// c_type: c_double
extern PyTypeObject py_c_double_type;

typedef struct PyC_c_double {
  PyObject_HEAD;
  double value;
  double *pointer;
  bool isPointer;
  bool isArray;
  size_t arraySize;
  size_t arrayCapacity;
} PyC_c_double;


static int c_double_init(PyObject *self, PyObject *args, PyObject *kwargs);
static PyObject *c_double_iter(PyObject *self);
static PyObject *c_double_getattr(PyObject *self, char *attr);
static void c_double_finalizer(PyObject *self);
static PyObject *c_double_append(PyObject *self, PyObject *args);
static PyObject *c_double_pop(PyObject *self);
static PyObject *c_double_value(PyObject *self);
static PyObject *c_double_donot_free(PyObject *self, PyObject *args,
                                    PyObject *kwargs);
static PyObject *c_double_to_pointer(PyObject *self);
static PyObject *c_double_to_float(PyObject *self);
static Py_ssize_t c_double_len(PyObject *self);
static PyObject *c_double_getitem(PyObject *self, PyObject *attr);
static int c_double_setitem(PyObject *self, PyObject *attr, PyObject *value);
