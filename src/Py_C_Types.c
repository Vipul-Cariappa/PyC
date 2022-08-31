#define PY_SSIZE_T_CLEAN
#include "Py_C_Types.h"
#include "DataStructures.h"
#include "PyC.h"
#include "Python.h"
#include "structmember.h"
#include <stddef.h>

// TODO: better error msg fro c_utypes (unsigned types)

// ----- c_void -----

PyMethodDef c_void_methods[] = {{"do_free", (PyCFunction)&c_void_do_free,
                                 METH_VARARGS | METH_KEYWORDS,
                                 "c_int.do_free()"},
                                {NULL, NULL, 0, NULL}};

PyTypeObject py_c_void_type = {
    PyVarObject_HEAD_INIT(NULL, 0).tp_name = "PyCpp.c_void",
    .tp_basicsize = sizeof(PyC_c_void),
    .tp_itemsize = 0,
    // .tp_as_mapping = &c_void_as_mapping,
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_doc = "PyCpp.c_void",
    // .tp_iter = &c_void_iter,
    // .tp_iternext = &c_void_next,
    .tp_methods = c_void_methods,
    // .tp_members = c_void_members,
    .tp_init = &c_void_init,
    .tp_new = PyType_GenericNew,
    .tp_dealloc = &c_void_finalizer,
};

// ----- c_void: functions and methods -----

// PyC.c_void.__init__
static int c_void_init(PyObject *self, PyObject *args, PyObject *kwargs) {
  PyC_c_void *selfType = (PyC_c_void *)self;
  selfType->pointer = NULL;
  selfType->freeOnDel = true;
  return 0;
}

// PyC.c_void.__del__
static void c_void_finalizer(PyObject *self) {
  PyC_c_void *selfType = (PyC_c_void *)self;

  if ((selfType->freeOnDel) && (selfType->pointer))
    free(selfType->pointer);

  Py_TYPE(self)->tp_free((PyObject *)self);
}

// PyC.c_void.donot_free
static PyObject *c_void_do_free(PyObject *self, PyObject *args,
                                PyObject *kwargs) {
  PyC_c_void *selfType = (PyC_c_void *)self;
  int value;

  if (args) {
    PyArg_ParseTuple(args, "p", &value);

    if (value)
      selfType->freeOnDel = true;
    else
      selfType->freeOnDel = false;

    Py_RETURN_NONE;
  }
  selfType->freeOnDel = true;
  Py_RETURN_NONE;
}

// ----- c_int -----
PyNumberMethods c_int_as_int = {
    .nb_int = &c_int_to_int,
};

PyMappingMethods c_int_as_mapping = {
    .mp_length = &c_int_len,
    .mp_subscript = &c_int_getitem,
    .mp_ass_subscript = &c_int_setitem,
};

PyMethodDef c_int_methods[] = {
    {"append", (PyCFunction)&c_int_append, METH_VARARGS, "c_int.append()"},
    {"pop", (PyCFunction)&c_int_pop, METH_NOARGS, "c_int.pop()"},
    {"value", (PyCFunction)&c_int_value, METH_NOARGS, "c_int.value()"},
    {"donot_free", (PyCFunction)&c_int_donot_free, METH_VARARGS | METH_KEYWORDS,
     "c_int.donot_free()"},
    {"to_pointer", (PyCFunction)&c_int_to_pointer, METH_NOARGS,
     "c_int.to_pointer()"},
    {NULL, NULL, 0, NULL}};

PyMemberDef c_int_members[] = {
    {"is_pointer", T_BOOL, offsetof(PyC_c_int, isPointer), READONLY,
     "PyC.c_int.is_pointer"},
    {"is_array", T_BOOL, offsetof(PyC_c_int, isArray), READONLY,
     "PyC.c_int.is_array"},
    {NULL, 0, 0, 0, NULL}};

PyTypeObject py_c_int_type = {
    PyVarObject_HEAD_INIT(NULL, 0).tp_name = "PyCpp.c_int",
    .tp_basicsize = sizeof(PyC_c_int),
    .tp_itemsize = 0,
    .tp_as_number = &c_int_as_int,
    .tp_as_mapping = &c_int_as_mapping,
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_doc = "PyCpp.c_int",
    .tp_iter = &c_int_iter,
    .tp_iternext = &c_int_next,
    .tp_methods = c_int_methods,
    .tp_members = c_int_members,
    .tp_init = &c_int_init,
    .tp_new = PyType_GenericNew,
    .tp_dealloc = &c_int_finalizer,
};

PyTypeObject py_c_uint_type = {
    PyVarObject_HEAD_INIT(NULL, 0).tp_name = "PyCpp.c_uint",
    .tp_basicsize = sizeof(PyC_c_int),
    .tp_itemsize = 0,
    .tp_as_number = &c_int_as_int,
    .tp_as_mapping = &c_int_as_mapping,
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_doc = "PyCpp.c_uint",
    .tp_iter = &c_int_iter,
    .tp_iternext = &c_int_next,
    .tp_methods = c_int_methods,
    .tp_members = c_int_members,
    .tp_init = &c_int_init,
    .tp_new = PyType_GenericNew,
    .tp_dealloc = &c_int_finalizer,
};

// ----- c_int: functions and methods -----

// PyC.c_int.__init__
static int c_int_init(PyObject *self, PyObject *args, PyObject *kwargs) {
  // TODO: implement init from c_pointer
  // TODO: implement keyword args: is_pointer, is_array

  PyC_c_int *selfType = (PyC_c_int *)self;

  if (kwargs) {
    PyObject *key = PyUnicode_FromFormat("pointer");
    if (PyDict_Contains(kwargs, key) == 1) {
      PyObject *pointer_value = PyDict_GetItem(kwargs, key);

      if (pointer_value == Py_None) {
        selfType->value = 0;
        selfType->pointer = NULL;
        selfType->isPointer = true;
        selfType->isArray = false;
        selfType->arraySize = 0;
        selfType->arrayCapacity = 0;
        selfType->_i = 0;
      }

      return 0;
    }
    Py_DECREF(key);
  }

  PyObject *arg_1 = PyTuple_GetItem(args, 0);

  if (PyNumber_Check(arg_1)) {
    int value = PyLong_AsLongLong(arg_1);

    selfType->value = value;
    selfType->pointer = &(selfType->value);
    selfType->isPointer = false;
    selfType->isArray = false;
    selfType->arraySize = 0;
    selfType->arrayCapacity = 0;
    selfType->_i = 0;

    return 0;

  } else if (PyTuple_Check(arg_1)) {
    size_t len = PyTuple_Size(arg_1);

    selfType->value = 0;
    selfType->pointer = calloc(len + 1, sizeof(int));
    selfType->isPointer = true;
    selfType->isArray = true;
    selfType->arraySize = len;
    selfType->arrayCapacity = len + 1;
    selfType->_i = 0;

    for (size_t i = 0; i < len; i++) {
      PyObject *element = PyTuple_GetItem(arg_1, i);

      if (!(PyNumber_Check(element))) {
        PyErr_SetString(PyExc_TypeError,
                        "Expected tuple of intergers or interger");
        return -1;
      }

      int value = PyLong_AsLongLong(element);
      selfType->pointer[i] = value;
    }

    selfType->pointer[len] = 0;

    return 0;
  }

  PyErr_SetString(PyExc_TypeError, "Expected tuple or interger");
  return -1;
}

// PyC.c_int.__iter__
static PyObject *c_int_iter(PyObject *self) {
  PyC_c_int *selfType = (PyC_c_int *)self;

  if (selfType->isArray) {
    selfType->_i = 0;
    Py_INCREF(self);
    return self;
  }

  PyErr_SetString(py_CppError,
                  "given c_int instance is not an array type instance");
  return NULL;
}

// PyC.c_int.__next__
static PyObject *c_int_next(PyObject *self) {
  PyC_c_int *selfType = (PyC_c_int *)self;
  PyObject *rvalue = NULL;

  size_t index = selfType->_i;

  if (selfType->arraySize > index) {
    if (PyObject_IsInstance(self, (PyObject *)&py_c_int_type)) {
      rvalue = PyLong_FromLongLong(selfType->value);
    }

    rvalue = PyLong_FromLongLong((unsigned int)selfType->value);
  }

  (selfType->_i)++;

  return rvalue;
}

// PyC.c_int.__del__
static void c_int_finalizer(PyObject *self) {
  PyC_c_int *selfType = (PyC_c_int *)self;

  if ((selfType->freeOnDel) && (selfType->freeOnDel))
    free(selfType->pointer);

  Py_TYPE(self)->tp_free((PyObject *)self);
}

// PyC.c_int.append
static PyObject *c_int_append(PyObject *self, PyObject *args) {
  PyC_c_int *selfType = (PyC_c_int *)self;

  if (!(selfType->isArray)) {
    PyErr_SetString(py_CppError,
                    "given instance of c_int is not an array type instance");
    return NULL;
  }

  PyObject *item = PyTuple_GetItem(args, 0);

  if (!(PyNumber_Check(item))) {
    PyErr_SetString(PyExc_TypeError,
                    "Expected interger type got some other type");
    return NULL;
  }

  int value = PyLong_AsLongLong(item);

  if (selfType->arrayCapacity > (selfType->arraySize + 1)) {
    selfType->pointer[selfType->arraySize] = value;
    (selfType->arraySize)++;

    selfType->pointer[selfType->arraySize] = 0;
  } else {
    int new_capacity = (selfType->arrayCapacity * 2) * sizeof(int);
    selfType->pointer = realloc(selfType->pointer, new_capacity);
    selfType->arrayCapacity = new_capacity / sizeof(int);

    selfType->pointer[selfType->arraySize] = value;
    (selfType->arraySize)++;

    selfType->pointer[selfType->arraySize] = 0;
  }

  Py_INCREF(item);
  return item;
}

// PyC.c_int.pop
static PyObject *c_int_pop(PyObject *self) {
  PyC_c_int *selfType = (PyC_c_int *)self;

  if (!(selfType->isArray)) {
    PyErr_SetString(py_CppError,
                    "given instance of c_int is not an array type instance");
    return NULL;
  }

  if (!(selfType->arraySize)) {
    PyErr_SetString(py_CppError, "no elements in the array to pop");
    return NULL;
  }

  PyObject *rvalue;
  if (PyObject_IsInstance(self, (PyObject *)&py_c_int_type)) {
    rvalue = PyLong_FromLongLong(
        (unsigned int)(selfType->pointer[(selfType->arraySize) - 1]));
  } else {
    rvalue = PyLong_FromLongLong(selfType->pointer[(selfType->arraySize) - 1]);
  }

  selfType->pointer[(selfType->arraySize) - 1] = 0;
  (selfType->arraySize)--;

  if ((selfType->arraySize * 2) < selfType->arrayCapacity) {
    selfType->pointer =
        realloc(selfType->pointer, (selfType->arraySize) * sizeof(int));
    selfType->arrayCapacity = selfType->arraySize;
  }

  return rvalue;
}

// PyC.c_int.value
static PyObject *c_int_value(PyObject *self) {
  PyC_c_int *selfType = (PyC_c_int *)self;

  if (PyObject_IsInstance(self, (PyObject *)&py_c_int_type)) {
    return PyLong_FromLongLong(selfType->value);
  }

  return PyLong_FromLongLong((unsigned int)selfType->value);
}

// PyC.c_int.donot_free
static PyObject *c_int_donot_free(PyObject *self, PyObject *args,
                                  PyObject *kwargs) {
  PyC_c_int *selfType = (PyC_c_int *)self;
  int value;

  if (args) {
    PyArg_ParseTuple(args, "p", &value);

    if (value)
      selfType->freeOnDel = true;
    else
      selfType->freeOnDel = false;

    Py_RETURN_NONE;
  }
  selfType->freeOnDel = false;
  Py_RETURN_NONE;
}

// PyC.c_int.to_pointer
static PyObject *c_int_to_pointer(PyObject *self) {
  // TODO: implement c_int_to_pointer

  PyC_c_int *selfType = (PyC_c_int *)self;
  Py_RETURN_NONE;
}

// PyC.c_int.__int__
static PyObject *c_int_to_int(PyObject *self) {
  PyC_c_int *selfType = (PyC_c_int *)self;

  if (PyObject_IsInstance(self, (PyObject *)&py_c_int_type)) {
    return PyLong_FromLongLong(selfType->value);
  }

  return PyLong_FromLongLong((unsigned int)selfType->value);
}

// PyC.c_int.__len__
static Py_ssize_t c_int_len(PyObject *self) {
  PyC_c_int *selfType = (PyC_c_int *)self;
  return selfType->arraySize;
}

// PyC.c_int.__getitem__
static PyObject *c_int_getitem(PyObject *self, PyObject *attr) {
  PyC_c_int *selfType = (PyC_c_int *)self;

  if (!(selfType->isArray)) {
    PyErr_SetString(py_CppError,
                    "given instance of c_int is not an array type instance");
    return NULL;
  }

  if (!(PyNumber_Check(attr))) {
    PyErr_SetString(PyExc_TypeError,
                    "Expected interger type got some other type");
    return NULL;
  }

  size_t index = PyLong_AsLongLong(attr);

  if (selfType->arraySize > index) {
    if (PyObject_IsInstance(self, (PyObject *)&py_c_int_type)) {
      return PyLong_FromLongLong((selfType->pointer)[index]);
    }

    return PyLong_FromLongLong((unsigned int)(selfType->pointer)[index]);
  }

  PyErr_SetString(py_CppError, "Index out of range");
  return NULL;
}

// PyC.c_int.__setitem__
static int c_int_setitem(PyObject *self, PyObject *attr, PyObject *value) {
  PyC_c_int *selfType = (PyC_c_int *)self;

  if (!(selfType->isArray)) {
    PyErr_SetString(py_CppError,
                    "given instance of c_int is not an array type instance");
    return -1;
  }

  if (!(PyNumber_Check(attr))) {
    PyErr_SetString(PyExc_TypeError,
                    "Expected interger type got some other type");
    return -1;
  }

  size_t index = PyLong_AsLongLong(attr);

  if (selfType->arraySize > index) {
    int int_value = PyLong_AsLongLong(value);

    (selfType->pointer)[index] = int_value;
    return 0;
  }

  PyErr_SetString(py_CppError, "Index out of range");
  return -1;
}

// ----- c_double -----
PyNumberMethods c_double_as_double = {
    .nb_float = &c_double_to_float,
};

PyMappingMethods c_double_as_mapping = {
    .mp_length = &c_double_len,
    .mp_subscript = &c_double_getitem,
    .mp_ass_subscript = &c_double_setitem,
};

PyMethodDef c_double_methods[] = {
    {"append", (PyCFunction)&c_double_append, METH_VARARGS,
     "c_double.append()"},
    {"pop", (PyCFunction)&c_double_pop, METH_NOARGS, "c_double.pop()"},
    {"value", (PyCFunction)&c_double_value, METH_NOARGS, "c_double.value()"},
    {"donot_free", (PyCFunction)&c_double_donot_free,
     METH_VARARGS | METH_KEYWORDS, "c_double.donot_free()"},
    {"to_pointer", (PyCFunction)&c_double_to_pointer, METH_NOARGS,
     "c_double.to_pointer()"},
    {NULL, NULL, 0, NULL}};

PyMemberDef c_double_members[] = {
    {"is_pointer", T_BOOL, offsetof(PyC_c_double, isPointer), READONLY,
     "PyC.c_double.is_pointer"},
    {"is_array", T_BOOL, offsetof(PyC_c_double, isArray), READONLY,
     "PyC.c_double.is_array"},
    {NULL, 0, 0, 0, NULL}};

PyTypeObject py_c_double_type = {
    PyVarObject_HEAD_INIT(NULL, 0).tp_name = "PyCpp.c_double",
    .tp_basicsize = sizeof(PyC_c_double),
    .tp_itemsize = 0,
    .tp_as_number = &c_double_as_double,
    .tp_as_mapping = &c_double_as_mapping,
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_doc = "PyCpp.c_double",
    .tp_iter = &c_double_iter,
    .tp_iternext = &c_double_next,
    .tp_methods = c_double_methods,
    .tp_members = c_double_members,
    .tp_init = &c_double_init,
    .tp_new = PyType_GenericNew,
    .tp_dealloc = &c_double_finalizer,
};

// ----- c_double: functions and methods -----

// PyC.c_double.__init__
static int c_double_init(PyObject *self, PyObject *args, PyObject *kwargs) {
  // TODO: implement init from c_pointer
  // TODO: implement keyword args: is_pointer, is_array

  PyC_c_double *selfType = (PyC_c_double *)self;

  if (kwargs) {
    PyObject *key = PyUnicode_FromFormat("pointer");
    if (PyDict_Contains(kwargs, key) == 1) {
      PyObject *pointer_value = PyDict_GetItem(kwargs, key);

      if (pointer_value == Py_None) {
        selfType->value = 0;
        selfType->pointer = NULL;
        selfType->isPointer = true;
        selfType->isArray = false;
        selfType->arraySize = 0;
        selfType->arrayCapacity = 0;
        selfType->_i = 0;
      }

      return 0;
    }
    Py_DECREF(key);
  }

  PyObject *arg_1 = PyTuple_GetItem(args, 0);

  if (PyNumber_Check(arg_1)) {
    double value = PyFloat_AsDouble(arg_1);

    selfType->value = value;
    selfType->pointer = &(selfType->value);
    selfType->isPointer = false;
    selfType->isArray = false;
    selfType->arraySize = 0;
    selfType->arrayCapacity = 0;
    selfType->_i = 0;

    return 0;

  } else if (PyTuple_Check(arg_1)) {
    size_t len = PyTuple_Size(arg_1);

    selfType->value = 0;
    selfType->pointer = calloc(len + 1, sizeof(int));
    selfType->isPointer = true;
    selfType->isArray = true;
    selfType->arraySize = len;
    selfType->arrayCapacity = len + 1;
    selfType->_i = 0;

    for (size_t i = 0; i < len; i++) {
      PyObject *element = PyTuple_GetItem(arg_1, i);

      if (!(PyNumber_Check(element))) {
        PyErr_SetString(PyExc_TypeError, "Expected tuple of floats or float");
        return -1;
      }

      double value = PyFloat_AsDouble(element);
      selfType->pointer[i] = value;
    }

    selfType->pointer[len] = 0;

    return 0;
  }

  PyErr_SetString(PyExc_TypeError, "Expected tuple or float");
  return -1;
}

// PyC.c_double.__iter__
static PyObject *c_double_iter(PyObject *self) {
  PyC_c_double *selfType = (PyC_c_double *)self;

  if (selfType->isArray) {
    selfType->_i = 0;
    Py_INCREF(self);
    return self;
  }

  PyErr_SetString(py_CppError,
                  "given c_double instance is not an array type instance");
  return NULL;
}

// PyC.c_double.__next__
static PyObject *c_double_next(PyObject *self) {
  PyC_c_int *selfType = (PyC_c_int *)self;
  PyObject *rvalue = NULL;

  size_t index = selfType->_i;

  if (selfType->arraySize > index) {
    rvalue = PyFloat_FromDouble((selfType->pointer)[index]);
  }

  (selfType->_i)++;

  return rvalue;
}

// PyC.c_double.__del__
static void c_double_finalizer(PyObject *self) {
  PyC_c_double *selfType = (PyC_c_double *)self;

  if ((selfType->freeOnDel) && (selfType->freeOnDel))
    free(selfType->pointer);

  Py_TYPE(self)->tp_free((PyObject *)self);
}

// PyC.c_double.append
static PyObject *c_double_append(PyObject *self, PyObject *args) {
  PyC_c_double *selfType = (PyC_c_double *)self;

  if (!(selfType->isArray)) {
    PyErr_SetString(py_CppError,
                    "given instance of c_double is not an array type instance");
    return NULL;
  }

  PyObject *item = PyTuple_GetItem(args, 0);

  if (!(PyNumber_Check(item))) {
    PyErr_SetString(PyExc_TypeError, "Expected float type got some other type");
    return NULL;
  }

  double value = PyFloat_AsDouble(item);

  if (selfType->arrayCapacity > (selfType->arraySize + 1)) {
    selfType->pointer[selfType->arraySize] = value;
    (selfType->arraySize)++;

    selfType->pointer[selfType->arraySize] = 0;
  } else {
    int new_capacity = (selfType->arrayCapacity * 2) * sizeof(double);
    selfType->pointer = realloc(selfType->pointer, new_capacity);
    selfType->arrayCapacity = new_capacity / sizeof(double);

    selfType->pointer[selfType->arraySize] = value;
    (selfType->arraySize)++;

    selfType->pointer[selfType->arraySize] = 0;
  }

  Py_INCREF(item);
  return item;
}

// PyC.c_double.pop
static PyObject *c_double_pop(PyObject *self) {
  PyC_c_double *selfType = (PyC_c_double *)self;

  if (!(selfType->isArray)) {
    PyErr_SetString(py_CppError,
                    "given instance of c_double is not an array type instance");
    return NULL;
  }

  if (!(selfType->arraySize)) {
    PyErr_SetString(py_CppError, "no elements in the array to pop");
    return NULL;
  }

  PyObject *rvalue =
      PyFloat_FromDouble(selfType->pointer[(selfType->arraySize) - 1]);
  selfType->pointer[(selfType->arraySize) - 1] = 0;
  (selfType->arraySize)--;

  if ((selfType->arraySize * 2) < selfType->arrayCapacity) {
    selfType->pointer =
        realloc(selfType->pointer, (selfType->arraySize) * sizeof(int));
    selfType->arrayCapacity = selfType->arraySize;
  }

  return rvalue;
}

// PyC.c_double.value
static PyObject *c_double_value(PyObject *self) {
  PyC_c_double *selfType = (PyC_c_double *)self;
  return PyFloat_FromDouble(selfType->value);
}

// PyC.c_double.donot_free
static PyObject *c_double_donot_free(PyObject *self, PyObject *args,
                                     PyObject *kwargs) {
  PyC_c_double *selfType = (PyC_c_double *)self;
  int value;

  if (args) {
    PyArg_ParseTuple(args, "p", &value);

    if (value)
      selfType->freeOnDel = true;
    else
      selfType->freeOnDel = false;

    Py_RETURN_NONE;
  }
  selfType->freeOnDel = false;
  Py_RETURN_NONE;
}

// PyC.c_double.to_pointer
static PyObject *c_double_to_pointer(PyObject *self) {
  // TODO: implement c_double_to_pointer

  PyC_c_double *selfType = (PyC_c_double *)self;
  Py_RETURN_NONE;
}

// PyC.c_double.__float__
static PyObject *c_double_to_float(PyObject *self) {
  PyC_c_double *selfType = (PyC_c_double *)self;
  return PyFloat_FromDouble(selfType->value);
}

// PyC.c_double.__len__
static Py_ssize_t c_double_len(PyObject *self) {
  PyC_c_double *selfType = (PyC_c_double *)self;
  return selfType->arraySize;
}

// PyC.c_double.__getitem__
static PyObject *c_double_getitem(PyObject *self, PyObject *attr) {
  PyC_c_double *selfType = (PyC_c_double *)self;

  if (!(selfType->isArray)) {
    PyErr_SetString(py_CppError,
                    "given instance of c_double is not an array type instance");
    return NULL;
  }

  if (!(PyNumber_Check(attr))) {
    PyErr_SetString(PyExc_TypeError, "Expected float type got some other type");
    return NULL;
  }

  size_t index = PyLong_AsLongLong(attr);

  if (selfType->arraySize > index) {
    return PyFloat_FromDouble((selfType->pointer)[index]);
  }

  PyErr_SetString(py_CppError, "Index out of range");
  return NULL;
}

// PyC.c_double.__setitem__
static int c_double_setitem(PyObject *self, PyObject *attr, PyObject *value) {
  PyC_c_double *selfType = (PyC_c_double *)self;

  if (!(selfType->isArray)) {
    PyErr_SetString(py_CppError,
                    "given instance of c_double is not an array type instance");
    return -1;
  }

  if (!(PyNumber_Check(attr))) {
    PyErr_SetString(PyExc_TypeError,
                    "Expected interger type got some other type");
    return -1;
  }

  size_t index = PyLong_AsLongLong(attr);

  if (selfType->arraySize > index) {
    double int_value = PyFloat_AsDouble(value);

    (selfType->pointer)[index] = int_value;
    return 0;
  }

  PyErr_SetString(py_CppError, "Index out of range");
  return -1;
}

// ----- c_float -----
PyNumberMethods c_float_as_float = {
    .nb_float = &c_float_to_float,
};

PyMappingMethods c_float_as_mapping = {
    .mp_length = &c_float_len,
    .mp_subscript = &c_float_getitem,
    .mp_ass_subscript = &c_float_setitem,
};

PyMethodDef c_float_methods[] = {
    {"append", (PyCFunction)&c_float_append, METH_VARARGS, "c_float.append()"},
    {"pop", (PyCFunction)&c_float_pop, METH_NOARGS, "c_float.pop()"},
    {"value", (PyCFunction)&c_float_value, METH_NOARGS, "c_float.value()"},
    {"donot_free", (PyCFunction)&c_float_donot_free,
     METH_VARARGS | METH_KEYWORDS, "c_float.donot_free()"},
    {"to_pointer", (PyCFunction)&c_float_to_pointer, METH_NOARGS,
     "c_float.to_pointer()"},
    {NULL, NULL, 0, NULL}};

PyMemberDef c_float_members[] = {
    {"is_pointer", T_BOOL, offsetof(PyC_c_float, isPointer), READONLY,
     "PyC.c_float.is_pointer"},
    {"is_array", T_BOOL, offsetof(PyC_c_float, isArray), READONLY,
     "PyC.c_float.is_array"},
    {NULL, 0, 0, 0, NULL}};

PyTypeObject py_c_float_type = {
    PyVarObject_HEAD_INIT(NULL, 0).tp_name = "PyCpp.c_float",
    .tp_basicsize = sizeof(PyC_c_float),
    .tp_itemsize = 0,
    .tp_as_number = &c_float_as_float,
    .tp_as_mapping = &c_float_as_mapping,
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_doc = "PyCpp.c_float",
    .tp_iter = &c_float_iter,
    .tp_iternext = &c_float_next,
    .tp_methods = c_float_methods,
    .tp_members = c_float_members,
    .tp_init = &c_float_init,
    .tp_new = PyType_GenericNew,
    .tp_dealloc = &c_float_finalizer,
};

// ----- c_float: functions and methods -----

// PyC.c_float.__init__
static int c_float_init(PyObject *self, PyObject *args, PyObject *kwargs) {
  // TODO: implement init from c_pointer
  // TODO: implement keyword args: is_pointer, is_array

  PyC_c_float *selfType = (PyC_c_float *)self;

  if (kwargs) {
    PyObject *key = PyUnicode_FromFormat("pointer");
    if (PyDict_Contains(kwargs, key) == 1) {
      PyObject *pointer_value = PyDict_GetItem(kwargs, key);

      if (pointer_value == Py_None) {
        selfType->value = 0;
        selfType->pointer = NULL;
        selfType->isPointer = true;
        selfType->isArray = false;
        selfType->arraySize = 0;
        selfType->arrayCapacity = 0;
        selfType->_i = 0;
      }

      return 0;
    }
    Py_DECREF(key);
  }

  PyObject *arg_1 = PyTuple_GetItem(args, 0);

  if (PyNumber_Check(arg_1)) {
    float value = PyFloat_AsDouble(arg_1);

    selfType->value = value;
    selfType->pointer = &(selfType->value);
    selfType->isPointer = false;
    selfType->isArray = false;
    selfType->arraySize = 0;
    selfType->arrayCapacity = 0;
    selfType->_i = 0;

    return 0;

  } else if (PyTuple_Check(arg_1)) {
    size_t len = PyTuple_Size(arg_1);

    selfType->value = 0;
    selfType->pointer = calloc(len + 1, sizeof(int));
    selfType->isPointer = true;
    selfType->isArray = true;
    selfType->arraySize = len;
    selfType->arrayCapacity = len + 1;
    selfType->_i = 0;

    for (size_t i = 0; i < len; i++) {
      PyObject *element = PyTuple_GetItem(arg_1, i);

      if (!(PyNumber_Check(element))) {
        PyErr_SetString(PyExc_TypeError, "Expected tuple of floats or float");
        return -1;
      }

      float value = PyFloat_AsDouble(element);
      selfType->pointer[i] = value;
    }

    selfType->pointer[len] = 0;

    return 0;
  }

  PyErr_SetString(PyExc_TypeError, "Expected tuple or float");
  return -1;
}

// PyC.c_float.__iter__
static PyObject *c_float_iter(PyObject *self) {
  PyC_c_float *selfType = (PyC_c_float *)self;

  if (selfType->isArray) {
    selfType->_i = 0;
    Py_INCREF(self);
    return self;
  }

  PyErr_SetString(py_CppError,
                  "given c_float instance is not an array type instance");
  return NULL;
}

// PyC.c_float.__next__
static PyObject *c_float_next(PyObject *self) {
  PyC_c_int *selfType = (PyC_c_int *)self;
  PyObject *rvalue = NULL;

  size_t index = selfType->_i;

  if (selfType->arraySize > index) {
    rvalue = PyFloat_FromDouble((selfType->pointer)[index]);
  }

  (selfType->_i)++;

  return rvalue;
}

// PyC.c_float.__del__
static void c_float_finalizer(PyObject *self) {
  PyC_c_float *selfType = (PyC_c_float *)self;

  if ((selfType->freeOnDel) && (selfType->freeOnDel))
    free(selfType->pointer);

  Py_TYPE(self)->tp_free((PyObject *)self);
}

// PyC.c_float.append
static PyObject *c_float_append(PyObject *self, PyObject *args) {
  PyC_c_float *selfType = (PyC_c_float *)self;

  if (!(selfType->isArray)) {
    PyErr_SetString(py_CppError,
                    "given instance of c_float is not an array type instance");
    return NULL;
  }

  PyObject *item = PyTuple_GetItem(args, 0);

  if (!(PyNumber_Check(item))) {
    PyErr_SetString(PyExc_TypeError, "Expected float type got some other type");
    return NULL;
  }

  float value = PyFloat_AsDouble(item);

  if (selfType->arrayCapacity > (selfType->arraySize + 1)) {
    selfType->pointer[selfType->arraySize] = value;
    (selfType->arraySize)++;

    selfType->pointer[selfType->arraySize] = 0;
  } else {
    int new_capacity = (selfType->arrayCapacity * 2) * sizeof(float);
    selfType->pointer = realloc(selfType->pointer, new_capacity);
    selfType->arrayCapacity = new_capacity / sizeof(float);

    selfType->pointer[selfType->arraySize] = value;
    (selfType->arraySize)++;

    selfType->pointer[selfType->arraySize] = 0;
  }

  Py_INCREF(item);
  return item;
}

// PyC.c_float.pop
static PyObject *c_float_pop(PyObject *self) {
  PyC_c_float *selfType = (PyC_c_float *)self;

  if (!(selfType->isArray)) {
    PyErr_SetString(py_CppError,
                    "given instance of c_float is not an array type instance");
    return NULL;
  }

  if (!(selfType->arraySize)) {
    PyErr_SetString(py_CppError, "no elements in the array to pop");
    return NULL;
  }

  PyObject *rvalue =
      PyFloat_FromDouble(selfType->pointer[(selfType->arraySize) - 1]);
  selfType->pointer[(selfType->arraySize) - 1] = 0;
  (selfType->arraySize)--;

  if ((selfType->arraySize * 2) < selfType->arrayCapacity) {
    selfType->pointer =
        realloc(selfType->pointer, (selfType->arraySize) * sizeof(int));
    selfType->arrayCapacity = selfType->arraySize;
  }

  return rvalue;
}

// PyC.c_float.value
static PyObject *c_float_value(PyObject *self) {
  PyC_c_float *selfType = (PyC_c_float *)self;
  return PyFloat_FromDouble(selfType->value);
}

// PyC.c_float.donot_free
static PyObject *c_float_donot_free(PyObject *self, PyObject *args,
                                    PyObject *kwargs) {
  PyC_c_float *selfType = (PyC_c_float *)self;
  int value;

  if (args) {
    PyArg_ParseTuple(args, "p", &value);

    if (value)
      selfType->freeOnDel = true;
    else
      selfType->freeOnDel = false;

    Py_RETURN_NONE;
  }
  selfType->freeOnDel = false;
  Py_RETURN_NONE;
}

// PyC.c_float.to_pointer
static PyObject *c_float_to_pointer(PyObject *self) {
  // TODO: implement c_float_to_pointer

  PyC_c_float *selfType = (PyC_c_float *)self;
  Py_RETURN_NONE;
}

// PyC.c_float.__float__
static PyObject *c_float_to_float(PyObject *self) {
  PyC_c_float *selfType = (PyC_c_float *)self;
  return PyFloat_FromDouble(selfType->value);
}

// PyC.c_float.__len__
static Py_ssize_t c_float_len(PyObject *self) {
  PyC_c_float *selfType = (PyC_c_float *)self;
  return selfType->arraySize;
}

// PyC.c_float.__getitem__
static PyObject *c_float_getitem(PyObject *self, PyObject *attr) {
  PyC_c_float *selfType = (PyC_c_float *)self;

  if (!(selfType->isArray)) {
    PyErr_SetString(py_CppError,
                    "given instance of c_float is not an array type instance");
    return NULL;
  }

  if (!(PyNumber_Check(attr))) {
    PyErr_SetString(PyExc_TypeError, "Expected float type got some other type");
    return NULL;
  }

  size_t index = PyLong_AsLongLong(attr);

  if (selfType->arraySize > index) {
    return PyFloat_FromDouble((selfType->pointer)[index]);
  }

  PyErr_SetString(py_CppError, "Index out of range");
  return NULL;
}

// PyC.c_float.__setitem__
static int c_float_setitem(PyObject *self, PyObject *attr, PyObject *value) {
  PyC_c_float *selfType = (PyC_c_float *)self;

  if (!(selfType->isArray)) {
    PyErr_SetString(py_CppError,
                    "given instance of c_float is not an array type instance");
    return -1;
  }

  if (!(PyNumber_Check(attr))) {
    PyErr_SetString(PyExc_TypeError,
                    "Expected interger type got some other type");
    return -1;
  }

  size_t index = PyLong_AsLongLong(attr);

  if (selfType->arraySize > index) {
    float int_value = PyFloat_AsDouble(value);

    (selfType->pointer)[index] = int_value;
    return 0;
  }

  PyErr_SetString(py_CppError, "Index out of range");
  return -1;
}

// ----- c_bool -----
// TODO: support array / pointer
PyNumberMethods c_bool_as_bool = {
    .nb_bool = &c_bool_to_bool,
};

PyMappingMethods c_bool_as_mapping = {
    .mp_length = &c_bool_len,
    .mp_subscript = &c_bool_getitem,
    .mp_ass_subscript = &c_bool_setitem,
};

PyMethodDef c_bool_methods[] = {
    {"append", (PyCFunction)&c_bool_append, METH_VARARGS, "c_bool.append()"},
    {"pop", (PyCFunction)&c_bool_pop, METH_NOARGS, "c_bool.pop()"},
    {"value", (PyCFunction)&c_bool_value, METH_NOARGS, "c_bool.value()"},
    {"donot_free", (PyCFunction)&c_bool_donot_free,
     METH_VARARGS | METH_KEYWORDS, "c_bool.donot_free()"},
    {"to_pointer", (PyCFunction)&c_bool_to_pointer, METH_NOARGS,
     "c_bool.to_pointer()"},
    {NULL, NULL, 0, NULL}};

PyTypeObject py_c_bool_type = {
    PyVarObject_HEAD_INIT(NULL, 0).tp_name = "PyCpp.c_bool",
    .tp_basicsize = sizeof(PyC_c_bool),
    .tp_itemsize = 0,
    .tp_as_number = &c_bool_as_bool,
    .tp_as_mapping = &c_bool_as_mapping,
    .tp_getattr = &c_bool_getattr,
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_doc = "PyCpp.c_bool",
    .tp_iter = &c_bool_iter,
    .tp_methods = c_bool_methods,
    .tp_init = &c_bool_init,
    .tp_new = PyType_GenericNew,
    .tp_dealloc = &c_bool_finalizer,
};

// ----- c_bool: functions and methods -----

// PyC.c_bool.__init__
static int c_bool_init(PyObject *self, PyObject *args, PyObject *kwargs) {
  // TODO: implement init from iter
  // TODO: implement init from c_pointer
  // TODO: implement keyword args: is_pointer, is_array

  PyC_c_bool *selfType = (PyC_c_bool *)self;

  int value;
  if (!PyArg_ParseTuple(args, "p", &value)) {
    return -1;
  }

  selfType->value = (bool)value;
  selfType->pointer = &(selfType->value);
  selfType->isPointer = false;
  selfType->isArray = false;
  selfType->arraySize = 0;
  selfType->arrayCapacity = 0;

  return 0;
}

// PyC.c_bool.__iter__
static PyObject *c_bool_iter(PyObject *self) {
  // TODO: implement c_bool_iter

  PyC_c_bool *selfType = (PyC_c_bool *)self;
  Py_RETURN_NONE;
}

// PyC.c_bool.__getattr__
static PyObject *c_bool_getattr(PyObject *self, char *attr) {
  // TODO: implement c_bool_getattr

  PyC_c_bool *selfType = (PyC_c_bool *)self;

  PyObject *value = PyObject_GenericGetAttr(self, PyUnicode_FromString(attr));
  if (value)
    return value;

  PyErr_Clear();
  Py_RETURN_NONE;
}

// PyC.c_bool.__del__
static void c_bool_finalizer(PyObject *self) {
  // TODO: implement c_bool_finalizer
  PyC_c_bool *selfType = (PyC_c_bool *)self;
  Py_TYPE(self)->tp_free((PyObject *)self);
}

// PyC.c_bool.append
static PyObject *c_bool_append(PyObject *self, PyObject *args) {
  // TODO: implement c_bool_append

  PyC_c_bool *selfType = (PyC_c_bool *)self;
  Py_RETURN_NONE;
}

// PyC.c_bool.pop
static PyObject *c_bool_pop(PyObject *self) {
  // TODO: implement c_bool_pop

  PyC_c_bool *selfType = (PyC_c_bool *)self;
  Py_RETURN_NONE;
}

// PyC.c_bool.value
static PyObject *c_bool_value(PyObject *self) {
  PyC_c_bool *selfType = (PyC_c_bool *)self;

  if (selfType->value) {
    Py_RETURN_TRUE;
  }
  Py_RETURN_FALSE;
}

// PyC.c_bool.donot_free
static PyObject *c_bool_donot_free(PyObject *self, PyObject *args,
                                   PyObject *kwargs) {
  // TODO: implement c_bool_donot_free

  PyC_c_bool *selfType = (PyC_c_bool *)self;
  Py_RETURN_NONE;
}

// PyC.c_bool.to_pointer
static PyObject *c_bool_to_pointer(PyObject *self) {
  // TODO: implement c_bool_to_pointer

  PyC_c_bool *selfType = (PyC_c_bool *)self;
  Py_RETURN_NONE;
}

// PyC.c_bool.__bool__
static int c_bool_to_bool(PyObject *self) {
  PyC_c_bool *selfType = (PyC_c_bool *)self;

  if (selfType->value) {
    return 1;
  }
  return 0;
}

// PyC.c_bool.__len__
static Py_ssize_t c_bool_len(PyObject *self) {
  // TODO: implement c_bool_len

  PyC_c_bool *selfType = (PyC_c_bool *)self;
  return 0;
}

// PyC.c_bool.__getitem__
static PyObject *c_bool_getitem(PyObject *self, PyObject *attr) {
  // TODO: implement c_bool_getitem

  PyC_c_bool *selfType = (PyC_c_bool *)self;
  Py_RETURN_NONE;
}

// PyC.c_bool.__setitem__
static int c_bool_setitem(PyObject *self, PyObject *attr, PyObject *value) {
  // TODO: implement c_bool_setitem

  PyC_c_bool *selfType = (PyC_c_bool *)self;
  return 0;
}

// ----- c_short -----
PyNumberMethods c_short_as_short = {
    .nb_int = &c_short_to_int,
};

PyMappingMethods c_short_as_mapping = {
    .mp_length = &c_short_len,
    .mp_subscript = &c_short_getitem,
    .mp_ass_subscript = &c_short_setitem,
};

PyMethodDef c_short_methods[] = {
    {"append", (PyCFunction)&c_short_append, METH_VARARGS, "c_short.append()"},
    {"pop", (PyCFunction)&c_short_pop, METH_NOARGS, "c_short.pop()"},
    {"value", (PyCFunction)&c_short_value, METH_NOARGS, "c_short.value()"},
    {"donot_free", (PyCFunction)&c_short_donot_free,
     METH_VARARGS | METH_KEYWORDS, "c_short.donot_free()"},
    {"to_pointer", (PyCFunction)&c_short_to_pointer, METH_NOARGS,
     "c_short.to_pointer()"},
    {NULL, NULL, 0, NULL}};

PyMemberDef c_short_members[] = {
    {"is_pointer", T_BOOL, offsetof(PyC_c_short, isPointer), READONLY,
     "PyC.c_short.is_pointer"},
    {"is_array", T_BOOL, offsetof(PyC_c_short, isArray), READONLY,
     "PyC.c_short.is_array"},
    {NULL, 0, 0, 0, NULL}};

PyTypeObject py_c_short_type = {
    PyVarObject_HEAD_INIT(NULL, 0).tp_name = "PyCpp.c_short",
    .tp_basicsize = sizeof(PyC_c_short),
    .tp_itemsize = 0,
    .tp_as_number = &c_short_as_short,
    .tp_as_mapping = &c_short_as_mapping,
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_doc = "PyCpp.c_short",
    .tp_iter = &c_short_iter,
    .tp_iternext = &c_short_next,
    .tp_methods = c_short_methods,
    .tp_members = c_short_members,
    .tp_init = &c_short_init,
    .tp_new = PyType_GenericNew,
    .tp_dealloc = &c_short_finalizer,
};

PyTypeObject py_c_ushort_type = {
    PyVarObject_HEAD_INIT(NULL, 0).tp_name = "PyCpp.c_ushort",
    .tp_basicsize = sizeof(PyC_c_short),
    .tp_itemsize = 0,
    .tp_as_number = &c_short_as_short,
    .tp_as_mapping = &c_short_as_mapping,
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_doc = "PyCpp.c_ushort",
    .tp_iter = &c_short_iter,
    .tp_iternext = &c_short_next,
    .tp_methods = c_short_methods,
    .tp_members = c_short_members,
    .tp_init = &c_short_init,
    .tp_new = PyType_GenericNew,
    .tp_dealloc = &c_short_finalizer,
};

// ----- c_short: functions and methods -----

// PyC.c_short.__init__
static int c_short_init(PyObject *self, PyObject *args, PyObject *kwargs) {
  // TODO: implement init from c_pointer
  // TODO: implement keyword args: is_pointer, is_array

  PyC_c_short *selfType = (PyC_c_short *)self;

  if (kwargs) {
    PyObject *key = PyUnicode_FromFormat("pointer");
    if (PyDict_Contains(kwargs, key) == 1) {
      PyObject *pointer_value = PyDict_GetItem(kwargs, key);

      if (pointer_value == Py_None) {
        selfType->value = 0;
        selfType->pointer = NULL;
        selfType->isPointer = true;
        selfType->isArray = false;
        selfType->arraySize = 0;
        selfType->arrayCapacity = 0;
        selfType->_i = 0;
      }

      return 0;
    }
    Py_DECREF(key);
  }

  PyObject *arg_1 = PyTuple_GetItem(args, 0);

  if (PyNumber_Check(arg_1)) {
    short value = PyLong_AsLong(arg_1);

    selfType->value = value;
    selfType->pointer = &(selfType->value);
    selfType->isPointer = false;
    selfType->isArray = false;
    selfType->arraySize = 0;
    selfType->arrayCapacity = 0;
    selfType->_i = 0;

    return 0;

  } else if (PyTuple_Check(arg_1)) {
    size_t len = PyTuple_Size(arg_1);

    selfType->value = 0;
    selfType->pointer = calloc(len + 1, sizeof(int));
    selfType->isPointer = true;
    selfType->isArray = true;
    selfType->arraySize = len;
    selfType->arrayCapacity = len + 1;
    selfType->_i = 0;

    for (size_t i = 0; i < len; i++) {
      PyObject *element = PyTuple_GetItem(arg_1, i);

      if (!(PyNumber_Check(element))) {
        PyErr_SetString(PyExc_TypeError, "Expected tuple of ints or int");
        return -1;
      }

      short value = PyLong_AsLong(element);
      selfType->pointer[i] = value;
    }

    selfType->pointer[len] = 0;

    return 0;
  }

  PyErr_SetString(PyExc_TypeError, "Expected tuple or int");
  return -1;
}

// PyC.c_short.__iter__
static PyObject *c_short_iter(PyObject *self) {
  PyC_c_short *selfType = (PyC_c_short *)self;

  if (selfType->isArray) {
    selfType->_i = 0;
    Py_INCREF(self);
    return self;
  }

  PyErr_SetString(py_CppError,
                  "given c_short instance is not an array type instance");
  return NULL;
}

// PyC.c_short.__next__
static PyObject *c_short_next(PyObject *self) {
  PyC_c_int *selfType = (PyC_c_int *)self;
  PyObject *rvalue = NULL;

  size_t index = selfType->_i;

  if (selfType->arraySize > index) {
    if (PyObject_IsInstance(self, (PyObject *)&py_c_ushort_type)) {
      rvalue =
          PyLong_FromLongLong((unsigned short)((selfType->pointer)[index]));
    } else {
      rvalue = PyLong_FromLongLong((selfType->pointer)[index]);
    }
  }

  (selfType->_i)++;

  return rvalue;
}

// PyC.c_short.__del__
static void c_short_finalizer(PyObject *self) {
  PyC_c_short *selfType = (PyC_c_short *)self;

  if ((selfType->freeOnDel) && (selfType->freeOnDel))
    free(selfType->pointer);

  Py_TYPE(self)->tp_free((PyObject *)self);
}

// PyC.c_short.append
static PyObject *c_short_append(PyObject *self, PyObject *args) {
  PyC_c_short *selfType = (PyC_c_short *)self;

  if (!(selfType->isArray)) {
    PyErr_SetString(py_CppError,
                    "given instance of c_short is not an array type instance");
    return NULL;
  }

  PyObject *item = PyTuple_GetItem(args, 0);

  if (!(PyNumber_Check(item))) {
    PyErr_SetString(PyExc_TypeError, "Expected short type got some other type");
    return NULL;
  }

  short value = PyLong_AsLong(item);

  if (selfType->arrayCapacity > (selfType->arraySize + 1)) {
    selfType->pointer[selfType->arraySize] = value;
    (selfType->arraySize)++;

    selfType->pointer[selfType->arraySize] = 0;
  } else {
    int new_capacity = (selfType->arrayCapacity * 2) * sizeof(short);
    selfType->pointer = realloc(selfType->pointer, new_capacity);
    selfType->arrayCapacity = new_capacity / sizeof(short);

    selfType->pointer[selfType->arraySize] = value;
    (selfType->arraySize)++;

    selfType->pointer[selfType->arraySize] = 0;
  }

  Py_INCREF(item);
  return item;
}

// PyC.c_short.pop
static PyObject *c_short_pop(PyObject *self) {
  PyC_c_short *selfType = (PyC_c_short *)self;

  if (!(selfType->isArray)) {
    PyErr_SetString(py_CppError,
                    "given instance of c_short is not an array type instance");
    return NULL;
  }

  if (!(selfType->arraySize)) {
    PyErr_SetString(py_CppError, "no elements in the array to pop");
    return NULL;
  }

  PyObject *rvalue;

  if (PyObject_IsInstance(self, (PyObject *)&py_c_ushort_type)) {
    rvalue = PyLong_FromLong(
        (unsigned short)(selfType->pointer[(selfType->arraySize) - 1]));
  } else {
    rvalue = PyLong_FromLong(selfType->pointer[(selfType->arraySize) - 1]);
  }

  selfType->pointer[(selfType->arraySize) - 1] = 0;
  (selfType->arraySize)--;

  if ((selfType->arraySize * 2) < selfType->arrayCapacity) {
    selfType->pointer =
        realloc(selfType->pointer, (selfType->arraySize) * sizeof(int));
    selfType->arrayCapacity = selfType->arraySize;
  }

  return rvalue;
}

// PyC.c_short.value
static PyObject *c_short_value(PyObject *self) {
  PyC_c_short *selfType = (PyC_c_short *)self;

  if (PyObject_IsInstance(self, (PyObject *)&py_c_ushort_type)) {
    return PyLong_FromLongLong((unsigned short)(selfType->value));
  }
  return PyLong_FromLongLong(selfType->value);
}

// PyC.c_short.donot_free
static PyObject *c_short_donot_free(PyObject *self, PyObject *args,
                                    PyObject *kwargs) {
  PyC_c_short *selfType = (PyC_c_short *)self;
  int value;

  if (args) {
    PyArg_ParseTuple(args, "p", &value);

    if (value)
      selfType->freeOnDel = true;
    else
      selfType->freeOnDel = false;

    Py_RETURN_NONE;
  }
  selfType->freeOnDel = false;
  Py_RETURN_NONE;
}

// PyC.c_short.to_pointer
static PyObject *c_short_to_pointer(PyObject *self) {
  // TODO: implement c_short_to_pointer

  PyC_c_short *selfType = (PyC_c_short *)self;
  Py_RETURN_NONE;
}

// PyC.c_short.__short__
static PyObject *c_short_to_int(PyObject *self) {
  PyC_c_short *selfType = (PyC_c_short *)self;

  if (PyObject_IsInstance(self, (PyObject *)&py_c_ushort_type)) {
    return PyLong_FromLongLong((unsigned short)(selfType->value));
  }
  return PyLong_FromLongLong(selfType->value);
}

// PyC.c_short.__len__
static Py_ssize_t c_short_len(PyObject *self) {
  PyC_c_short *selfType = (PyC_c_short *)self;
  return selfType->arraySize;
}

// PyC.c_short.__getitem__
static PyObject *c_short_getitem(PyObject *self, PyObject *attr) {
  PyC_c_short *selfType = (PyC_c_short *)self;

  if (!(selfType->isArray)) {
    PyErr_SetString(py_CppError,
                    "given instance of c_short is not an array type instance");
    return NULL;
  }

  if (!(PyNumber_Check(attr))) {
    PyErr_SetString(PyExc_TypeError, "Expected int type got some other type");
    return NULL;
  }

  size_t index = PyLong_AsLongLong(attr);

  if (selfType->arraySize > index) {
    if (PyObject_IsInstance(self, (PyObject *)&py_c_ushort_type)) {
      return PyLong_FromLongLong((unsigned short)((selfType->pointer)[index]));
    }
    return PyLong_FromLongLong((selfType->pointer)[index]);
  }

  PyErr_SetString(py_CppError, "Index out of range");
  return NULL;
}

// PyC.c_short.__setitem__
static int c_short_setitem(PyObject *self, PyObject *attr, PyObject *value) {
  PyC_c_short *selfType = (PyC_c_short *)self;

  if (!(selfType->isArray)) {
    PyErr_SetString(py_CppError,
                    "given instance of c_short is not an array type instance");
    return -1;
  }

  if (!(PyNumber_Check(attr))) {
    PyErr_SetString(PyExc_TypeError,
                    "Expected interger type got some other type");
    return -1;
  }

  size_t index = PyLong_AsLongLong(attr);

  if (selfType->arraySize > index) {
    short int_value = PyLong_AsLong(value);

    (selfType->pointer)[index] = int_value;
    return 0;
  }

  PyErr_SetString(py_CppError, "Index out of range");
  return -1;
}

// ----- c_long -----
PyNumberMethods c_long_as_long = {
    .nb_int = &c_long_to_int,
};

PyMappingMethods c_long_as_mapping = {
    .mp_length = &c_long_len,
    .mp_subscript = &c_long_getitem,
    .mp_ass_subscript = &c_long_setitem,
};

PyMethodDef c_long_methods[] = {
    {"append", (PyCFunction)&c_long_append, METH_VARARGS, "c_long.append()"},
    {"pop", (PyCFunction)&c_long_pop, METH_NOARGS, "c_long.pop()"},
    {"value", (PyCFunction)&c_long_value, METH_NOARGS, "c_long.value()"},
    {"donot_free", (PyCFunction)&c_long_donot_free,
     METH_VARARGS | METH_KEYWORDS, "c_long.donot_free()"},
    {"to_pointer", (PyCFunction)&c_long_to_pointer, METH_NOARGS,
     "c_long.to_pointer()"},
    {NULL, NULL, 0, NULL}};

PyMemberDef c_long_members[] = {
    {"is_pointer", T_BOOL, offsetof(PyC_c_long, isPointer), READONLY,
     "PyC.c_long.is_pointer"},
    {"is_array", T_BOOL, offsetof(PyC_c_long, isArray), READONLY,
     "PyC.c_long.is_array"},
    {NULL, 0, 0, 0, NULL}};

PyTypeObject py_c_long_type = {
    PyVarObject_HEAD_INIT(NULL, 0).tp_name = "PyCpp.c_long",
    .tp_basicsize = sizeof(PyC_c_long),
    .tp_itemsize = 0,
    .tp_as_number = &c_long_as_long,
    .tp_as_mapping = &c_long_as_mapping,
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_doc = "PyCpp.c_long",
    .tp_iter = &c_long_iter,
    .tp_iternext = &c_long_next,
    .tp_methods = c_long_methods,
    .tp_members = c_long_members,
    .tp_init = &c_long_init,
    .tp_new = PyType_GenericNew,
    .tp_dealloc = &c_long_finalizer,
};

PyTypeObject py_c_ulong_type = {
    PyVarObject_HEAD_INIT(NULL, 0).tp_name = "PyCpp.c_ulong",
    .tp_basicsize = sizeof(PyC_c_long),
    .tp_itemsize = 0,
    .tp_as_number = &c_long_as_long,
    .tp_as_mapping = &c_long_as_mapping,
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_doc = "PyCpp.c_ulong",
    .tp_iter = &c_long_iter,
    .tp_iternext = &c_long_next,
    .tp_methods = c_long_methods,
    .tp_members = c_long_members,
    .tp_init = &c_long_init,
    .tp_new = PyType_GenericNew,
    .tp_dealloc = &c_long_finalizer,
};

// ----- c_long: functions and methods -----

// PyC.c_long.__init__
static int c_long_init(PyObject *self, PyObject *args, PyObject *kwargs) {
  // TODO: implement init from c_pointer
  // TODO: implement keyword args: is_pointer, is_array

  PyC_c_long *selfType = (PyC_c_long *)self;

  PyObject *key = PyUnicode_FromFormat("pointer");

  if (kwargs) {
    PyObject *key = PyUnicode_FromFormat("pointer");
    if (PyDict_Contains(kwargs, key) == 1) {
      PyObject *pointer_value = PyDict_GetItem(kwargs, key);

      if (pointer_value == Py_None) {
        selfType->value = 0;
        selfType->pointer = NULL;
        selfType->isPointer = true;
        selfType->isArray = false;
        selfType->arraySize = 0;
        selfType->arrayCapacity = 0;
        selfType->_i = 0;
      }

      return 0;
    }
    Py_DECREF(key);
  }

  PyObject *arg_1 = PyTuple_GetItem(args, 0);

  if (PyNumber_Check(arg_1)) {
    long value = PyLong_AsLong(arg_1);

    selfType->value = value;
    selfType->pointer = &(selfType->value);
    selfType->isPointer = false;
    selfType->isArray = false;
    selfType->arraySize = 0;
    selfType->arrayCapacity = 0;
    selfType->_i = 0;

    return 0;

  } else if (PyTuple_Check(arg_1)) {
    size_t len = PyTuple_Size(arg_1);

    selfType->value = 0;
    selfType->pointer = calloc(len + 1, sizeof(int));
    selfType->isPointer = true;
    selfType->isArray = true;
    selfType->arraySize = len;
    selfType->arrayCapacity = len + 1;
    selfType->_i = 0;

    for (size_t i = 0; i < len; i++) {
      PyObject *element = PyTuple_GetItem(arg_1, i);

      if (!(PyNumber_Check(element))) {
        PyErr_SetString(PyExc_TypeError, "Expected tuple of ints or int");
        return -1;
      }

      long value = PyLong_AsLong(element);
      selfType->pointer[i] = value;
    }

    selfType->pointer[len] = 0;

    return 0;
  }

  PyErr_SetString(PyExc_TypeError, "Expected tuple or int");
  return -1;
}

// PyC.c_long.__iter__
static PyObject *c_long_iter(PyObject *self) {
  PyC_c_long *selfType = (PyC_c_long *)self;

  if (selfType->isArray) {
    selfType->_i = 0;
    Py_INCREF(self);
    return self;
  }

  PyErr_SetString(py_CppError,
                  "given c_long instance is not an array type instance");
  return NULL;
}

// PyC.c_long.__next__
static PyObject *c_long_next(PyObject *self) {
  PyC_c_int *selfType = (PyC_c_int *)self;
  PyObject *rvalue = NULL;

  size_t index = selfType->_i;

  if (selfType->arraySize > index) {

    if (PyObject_IsInstance(self, (PyObject *)&py_c_ulong_type)) {
      rvalue = PyLong_FromLong((unsigned long)((selfType->pointer)[index]));
    } else {
      rvalue = PyLong_FromLong((selfType->pointer)[index]);
    }
  }

  (selfType->_i)++;

  return rvalue;
}

// PyC.c_long.__del__
static void c_long_finalizer(PyObject *self) {
  PyC_c_long *selfType = (PyC_c_long *)self;

  if ((selfType->freeOnDel) && (selfType->freeOnDel))
    free(selfType->pointer);

  Py_TYPE(self)->tp_free((PyObject *)self);
}

// PyC.c_long.append
static PyObject *c_long_append(PyObject *self, PyObject *args) {
  PyC_c_long *selfType = (PyC_c_long *)self;

  if (!(selfType->isArray)) {
    PyErr_SetString(py_CppError,
                    "given instance of c_long is not an array type instance");
    return NULL;
  }

  PyObject *item = PyTuple_GetItem(args, 0);

  if (!(PyNumber_Check(item))) {
    PyErr_SetString(PyExc_TypeError, "Expected long type got some other type");
    return NULL;
  }

  long value = PyLong_AsLong(item);

  if (selfType->arrayCapacity > (selfType->arraySize + 1)) {
    selfType->pointer[selfType->arraySize] = value;
    (selfType->arraySize)++;

    selfType->pointer[selfType->arraySize] = 0;
  } else {
    int new_capacity = (selfType->arrayCapacity * 2) * sizeof(long);
    selfType->pointer = realloc(selfType->pointer, new_capacity);
    selfType->arrayCapacity = new_capacity / sizeof(long);

    selfType->pointer[selfType->arraySize] = value;
    (selfType->arraySize)++;

    selfType->pointer[selfType->arraySize] = 0;
  }

  Py_INCREF(item);
  return item;
}

// PyC.c_long.pop
static PyObject *c_long_pop(PyObject *self) {
  PyC_c_long *selfType = (PyC_c_long *)self;

  if (!(selfType->isArray)) {
    PyErr_SetString(py_CppError,
                    "given instance of c_long is not an array type instance");
    return NULL;
  }

  if (!(selfType->arraySize)) {
    PyErr_SetString(py_CppError, "no elements in the array to pop");
    return NULL;
  }

  PyObject *rvalue;

  if (PyObject_IsInstance(self, (PyObject *)&py_c_ulong_type)) {
    rvalue = PyLong_FromLong(
        (unsigned long)(selfType->pointer[(selfType->arraySize) - 1]));
  } else {
    rvalue = PyLong_FromLong(selfType->pointer[(selfType->arraySize) - 1]);
  }

  selfType->pointer[(selfType->arraySize) - 1] = 0;
  (selfType->arraySize)--;

  if ((selfType->arraySize * 2) < selfType->arrayCapacity) {
    selfType->pointer =
        realloc(selfType->pointer, (selfType->arraySize) * sizeof(int));
    selfType->arrayCapacity = selfType->arraySize;
  }

  return rvalue;
}

// PyC.c_long.value
static PyObject *c_long_value(PyObject *self) {
  PyC_c_long *selfType = (PyC_c_long *)self;

  if (PyObject_IsInstance(self, (PyObject *)&py_c_ulong_type)) {
    return PyLong_FromLongLong((unsigned long)(selfType->value));
  }
  return PyLong_FromLongLong(selfType->value);
}

// PyC.c_long.donot_free
static PyObject *c_long_donot_free(PyObject *self, PyObject *args,
                                   PyObject *kwargs) {
  PyC_c_long *selfType = (PyC_c_long *)self;
  int value;

  if (args) {
    PyArg_ParseTuple(args, "p", &value);

    if (value)
      selfType->freeOnDel = true;
    else
      selfType->freeOnDel = false;

    Py_RETURN_NONE;
  }
  selfType->freeOnDel = false;
  Py_RETURN_NONE;
}

// PyC.c_long.to_pointer
static PyObject *c_long_to_pointer(PyObject *self) {
  // TODO: implement c_long_to_pointer

  PyC_c_long *selfType = (PyC_c_long *)self;
  Py_RETURN_NONE;
}

// PyC.c_long.__int__
static PyObject *c_long_to_int(PyObject *self) {
  PyC_c_long *selfType = (PyC_c_long *)self;

  if (PyObject_IsInstance(self, (PyObject *)&py_c_ulong_type)) {
    return PyLong_FromLongLong((unsigned long)(selfType->value));
  }
  return PyLong_FromLongLong(selfType->value);
}

// PyC.c_long.__len__
static Py_ssize_t c_long_len(PyObject *self) {
  PyC_c_long *selfType = (PyC_c_long *)self;
  return selfType->arraySize;
}

// PyC.c_long.__getitem__
static PyObject *c_long_getitem(PyObject *self, PyObject *attr) {
  PyC_c_long *selfType = (PyC_c_long *)self;

  if (!(selfType->isArray)) {
    PyErr_SetString(py_CppError,
                    "given instance of c_long is not an array type instance");
    return NULL;
  }

  if (!(PyNumber_Check(attr))) {
    PyErr_SetString(PyExc_TypeError, "Expected int type got some other type");
    return NULL;
  }

  size_t index = PyLong_AsLongLong(attr);

  if (selfType->arraySize > index) {
    if (PyObject_IsInstance(self, (PyObject *)&py_c_ulong_type)) {
      return PyLong_FromLong((unsigned long)((selfType->pointer)[index]));
    }
    return PyLong_FromLong((selfType->pointer)[index]);
  }

  PyErr_SetString(py_CppError, "Index out of range");
  return NULL;
}

// PyC.c_long.__setitem__
static int c_long_setitem(PyObject *self, PyObject *attr, PyObject *value) {
  PyC_c_long *selfType = (PyC_c_long *)self;

  if (!(selfType->isArray)) {
    PyErr_SetString(py_CppError,
                    "given instance of c_long is not an array type instance");
    return -1;
  }

  if (!(PyNumber_Check(attr))) {
    PyErr_SetString(PyExc_TypeError,
                    "Expected interger type got some other type");
    return -1;
  }

  size_t index = PyLong_AsLongLong(attr);

  if (selfType->arraySize > index) {
    long int_value = PyLong_AsLong(value);

    (selfType->pointer)[index] = int_value;
    return 0;
  }

  PyErr_SetString(py_CppError, "Index out of range");
  return -1;
}

// ----- c_char -----
// TODO: support array / pointer
// TODO: support c_uchar

PyMappingMethods c_char_as_mapping = {
    .mp_length = &c_char_len,
    .mp_subscript = &c_char_getitem,
    .mp_ass_subscript = &c_char_setitem,
};

PyMethodDef c_char_methods[] = {
    {"append", (PyCFunction)&c_char_append, METH_VARARGS, "c_char.append()"},
    {"pop", (PyCFunction)&c_char_pop, METH_NOARGS, "c_char.pop()"},
    {"value", (PyCFunction)&c_char_value, METH_NOARGS, "c_char.value()"},
    {"donot_free", (PyCFunction)&c_char_donot_free,
     METH_VARARGS | METH_KEYWORDS, "c_char.donot_free()"},
    {"to_pointer", (PyCFunction)&c_char_to_pointer, METH_NOARGS,
     "c_char.to_pointer()"},
    {NULL, NULL, 0, NULL}};

PyTypeObject py_c_char_type = {
    PyVarObject_HEAD_INIT(NULL, 0).tp_name = "PyCpp.c_char",
    .tp_basicsize = sizeof(PyC_c_char),
    .tp_itemsize = 0,
    .tp_as_mapping = &c_char_as_mapping,
    .tp_str = &c_char_to_str,
    .tp_getattr = &c_char_getattr,
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_doc = "PyCpp.c_char",
    .tp_iter = &c_char_iter,
    .tp_methods = c_char_methods,
    .tp_init = &c_char_init,
    .tp_new = PyType_GenericNew,
    .tp_dealloc =
        &c_char_finalizer, // TODO: use .tp_getset for PyC_c_char's attributes
};

// ----- c_char: functions and methods -----

// PyC.c_char.__init__
static int c_char_init(PyObject *self, PyObject *args, PyObject *kwargs) {
  // TODO: implement init from c_pointer
  // TODO: implement keyword args: is_pointer, is_array
  // TODO: implement init from number / int

  PyC_c_char *selfType = (PyC_c_char *)self;

  char *value;
  if (!PyArg_ParseTuple(args, "s", &value)) {
    return -1;
  }

  size_t len = strlen(value);

  if (len > 1) {
    selfType->value = 0;
    selfType->isPointer = true;
    selfType->isArray = true;

    char *string = malloc(len + 1); // FIXME: ?
    strcpy(string, value);
    selfType->pointer = string;
    selfType->arraySize = len;
    selfType->arrayCapacity = len + 1;
  } else if (len == 0) {
    selfType->value = 0;
    selfType->pointer = &(selfType->value);
    selfType->isPointer = false;
    selfType->isArray = false;
    selfType->arraySize = 0;
    selfType->arrayCapacity = 0;
  } else {
    selfType->value = value[0];
    selfType->pointer = &(selfType->value);
    selfType->isPointer = false;
    selfType->isArray = false;
    selfType->arraySize = 1;
    selfType->arrayCapacity = 0;
  }

  return 0;
}

// PyC.c_char.__iter__
static PyObject *c_char_iter(PyObject *self) {
  // TODO: implement c_char_iter

  PyC_c_char *selfType = (PyC_c_char *)self;
  Py_RETURN_NONE;
}

// PyC.c_char.__getattr__
static PyObject *c_char_getattr(PyObject *self, char *attr) {
  // TODO: implement c_char_getattr

  PyC_c_char *selfType = (PyC_c_char *)self;

  PyObject *value = PyObject_GenericGetAttr(self, PyUnicode_FromString(attr));
  if (value)
    return value;

  PyErr_Clear();
  Py_RETURN_NONE;
}

// PyC.c_char.__del__
static void c_char_finalizer(PyObject *self) {
  // TODO: implement c_char_finalizer
  PyC_c_char *selfType = (PyC_c_char *)self;
  Py_TYPE(self)->tp_free((PyObject *)self);
}

// PyC.c_char.append
static PyObject *c_char_append(PyObject *self, PyObject *args) {
  // TODO: implement c_char_append

  PyC_c_char *selfType = (PyC_c_char *)self;
  Py_RETURN_NONE;
}

// PyC.c_char.pop
static PyObject *c_char_pop(PyObject *self) {
  // TODO: implement c_char_pop

  PyC_c_char *selfType = (PyC_c_char *)self;
  Py_RETURN_NONE;
}

// PyC.c_char.value
static PyObject *c_char_value(PyObject *self) {
  PyC_c_char *selfType = (PyC_c_char *)self;

  if (selfType->isPointer) {
    return PyUnicode_FromString(selfType->pointer);
  } else {
    char string[2];
    string[0] = selfType->value;
    string[1] = 0;

    return PyUnicode_FromString(string);
  }
}

// PyC.c_char.donot_free
static PyObject *c_char_donot_free(PyObject *self, PyObject *args,
                                   PyObject *kwargs) {
  // TODO: implement c_char_donot_free

  PyC_c_char *selfType = (PyC_c_char *)self;
  Py_RETURN_NONE;
}

// PyC.c_char.to_pointer
static PyObject *c_char_to_pointer(PyObject *self) {
  // TODO: implement c_char_to_pointer

  PyC_c_char *selfType = (PyC_c_char *)self;
  Py_RETURN_NONE;
}

// PyC.c_char.__char__
static PyObject *c_char_to_str(PyObject *self) {
  PyC_c_char *selfType = (PyC_c_char *)self;

  if (selfType->isPointer) {
    return PyUnicode_FromString(selfType->pointer);
  } else {
    char string[2];
    string[0] = selfType->value;
    string[1] = 0;

    return PyUnicode_FromString(string);
  }
}

// PyC.c_char.__len__
static Py_ssize_t c_char_len(PyObject *self) {
  // TODO: implement c_char_len

  PyC_c_char *selfType = (PyC_c_char *)self;

  return selfType->arraySize;
}

// PyC.c_char.__getitem__
static PyObject *c_char_getitem(PyObject *self, PyObject *attr) {
  // TODO: implement c_char_getitem

  PyC_c_char *selfType = (PyC_c_char *)self;
  Py_RETURN_NONE;
}

// PyC.c_char.__setitem__
static int c_char_setitem(PyObject *self, PyObject *attr, PyObject *value) {
  // TODO: implement c_char_setitem

  PyC_c_char *selfType = (PyC_c_char *)self;
  return 0;
}

// ----- c_struct -----

PyMappingMethods c_struct_as_mapping = {
    .mp_length = &c_struct_len,
    .mp_subscript = &c_struct_getitem,
    .mp_ass_subscript = &c_struct_setitem,
};

PyMemberDef c_struct_members[] = {
    {"is_array", T_BOOL, offsetof(PyC_c_struct, isArray), READONLY,
     "PyC.c_struct.is_array"},
    {"__python_representation", T_OBJECT, offsetof(PyC_c_struct, pyDictRepr),
     READONLY, "PyC.c_struct.__python_representation"},
    {NULL, 0, 0, 0, NULL}};

PyMethodDef c_struct_methods[] = {
    {"append", (PyCFunction)&c_struct_append, METH_VARARGS,
     "c_struct.append()"},
    {"pop", (PyCFunction)&c_struct_pop, METH_NOARGS, "c_struct.pop()"},
    {"donot_free", (PyCFunction)&c_struct_donot_free,
     METH_VARARGS | METH_KEYWORDS, "c_struct.donot_free()"},
    {"to_pointer", (PyCFunction)&c_struct_to_pointer, METH_NOARGS,
     "c_struct.to_pointer()"},
    {NULL, NULL, 0, NULL}};

PyTypeObject py_c_struct_type = {
    PyVarObject_HEAD_INIT(NULL, 0).tp_name = "PyCpp.c_struct",
    .tp_basicsize = sizeof(PyC_c_struct),
    .tp_itemsize = 0,
    .tp_getattr = &c_struct_getattr,
    .tp_setattr = &c_struct_setattr,
    .tp_flags = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,
    .tp_call = c_struct_call,
    .tp_doc = "PyCpp.c_struct",
    .tp_iter = &c_struct_iter,
    .tp_iternext = &c_struct_next,
    .tp_methods = c_struct_methods,
    .tp_members = c_struct_members,
    .tp_init = &c_struct_init,
    .tp_new = PyType_GenericNew,
    .tp_dealloc = &c_struct_finalizer,
    // TODO: use .tp_getset for PyC_c_int's attributes
};

// c_struct methods

// PyC.c_struct.__init__
static int c_struct_init(PyObject *self, PyObject *args, PyObject *kwargs) {
  // TODO: implement
  PyC_c_struct *selfType = (PyC_c_struct *)self;
  return 0;
}

// PyC.c_struct.__getattr__
static PyObject *c_struct_getattr(PyObject *self, char *attr) {
  // TODO: implement
  PyC_c_struct *selfType = (PyC_c_struct *)self;

  for (size_t i = 0; i < selfType->structure->attrCount; i++) {
    if (!(strcmp(attr, array_str_getat(selfType->structure->attrNames, i)))) {
      char *data = (char *)selfType->pointer;

      return cppArg_to_pyArg(
          data + (array_long_long_getat(selfType->structure->offsets, i) / 8),
          *array_p_ffi_type_getat(selfType->structure->attrTypes, i),
          array_CXTypeKind_getat(selfType->structure->attrUnderlyingType, i),
          array_p_Structure_getat(selfType->structure->attrUnderlyingStructs,
                                  i),
          array_p_Union_getat(selfType->structure->attrUnderlyingUnions, i),
          selfType->parentModule);
    }
  }

  Py_RETURN_NONE;
}

// PyC.c_struct.__setattr__
static int c_struct_setattr(PyObject *self, char *attr, PyObject *pValue) {
  // TODO: implement

  PyC_c_struct *selfType = (PyC_c_struct *)self;

  for (size_t i = 0; i < selfType->structure->attrCount; i++) {
    if (!(strcmp(attr, array_str_getat(selfType->structure->attrNames, i)))) {
      ffi_type *type =
          array_p_ffi_type_getat(selfType->structure->attrTypes, i);

      bool should_free = false;
      void *data = pyArg_to_cppArg(pValue, *type, &should_free);

      if (type->type == FFI_TYPE_POINTER) {
        memcpy((char *)(selfType->pointer) +
                   (array_long_long_getat(selfType->structure->offsets, i) / 8),
               data, type->size);
      } else if (PyObject_IsInstance(pValue, (PyObject *)&py_c_struct_type)) {
        memcpy((char *)(selfType->pointer) +
                   (array_long_long_getat(selfType->structure->offsets, i) / 8),
               data, ((PyC_c_struct *)pValue)->structure->structSize);
      } else {
        memcpy((char *)(selfType->pointer) +
                   (array_long_long_getat(selfType->structure->offsets, i) / 8),
               data, type->size);
      }

      if (should_free) {
        free(data);
      }

      return 0;
    }
  }

  PyErr_SetString(py_CppError, "Struct with given attribute not found");
  return -1;
}

// PyC.c_struct.__del__
static void c_struct_finalizer(PyObject *self) {
  // TODO: implement
  PyC_c_struct *selfType = (PyC_c_struct *)self;

  // free(selfType->pointer);
  Py_DECREF(selfType->parentModule);
  Py_TYPE(self)->tp_free((PyObject *)self);
}

// PyC.c_struct.__call__
static PyObject *c_struct_call(PyObject *self, PyObject *args,
                               PyObject *kwargs) {
  // TODO: implement
  PyC_c_struct *selfType = (PyC_c_struct *)self;

  PyObject *obj = PyObject_GetAttrString(PyC, selfType->structure->name);
  if (obj) {
    PyC_c_struct *result = (PyC_c_struct *)PyObject_CallObject(obj, NULL);
    result->structure = selfType->structure;
    result->pointer = malloc(selfType->structure->structSize); // FIXME: malloc
    Py_INCREF(selfType->parentModule);
    result->parentModule = selfType->parentModule;

    return (PyObject *)result;
  }
  PyErr_SetString(py_BindingError,
                  "Unable to access PyCpp.c_struct base class");
  return NULL;
}

// helper function
PyObject *create_py_c_struct(Structure *structure, PyObject *module) {
  // TODO: implement
  typedef struct PyC_c_new {
    PyC_c_struct super;
    // PyObject_HEAD;
  } PyC_c_new;

  PyTypeObject *py_c_new_type =
      malloc(sizeof(PyTypeObject)); // TODO: free this malloc

  if (py_c_new_type) {
    array_p_void_append(EXTRA_HEAP_MEMORY, py_c_new_type);
  } else {
    return PyErr_NoMemory();
  }

  char *struct_name =
      malloc(7 + strlen(structure->name)); // TODO: free this malloc
  strcpy(struct_name, "PyCpp.");
  strcat(struct_name, structure->name);

  if (struct_name) {
    array_p_void_append(EXTRA_HEAP_MEMORY, struct_name);
  } else {
    return PyErr_NoMemory();
  }

  *py_c_new_type = (PyTypeObject){
      PyVarObject_HEAD_INIT(NULL, 0).tp_name = struct_name,
      .tp_basicsize = sizeof(PyC_c_new),
      .tp_itemsize = 0,
      .tp_flags = Py_TPFLAGS_DEFAULT,
      .tp_doc = struct_name,
      .tp_new = PyType_GenericNew,
      .tp_base = &py_c_struct_type,
  };

  if (PyType_Ready(py_c_new_type) < 0) {
    return NULL;
  }

  Py_INCREF(py_c_new_type);
  if (PyModule_AddObject(PyC, structure->name, (PyObject *)py_c_new_type) < 0) {
    Py_DECREF(py_c_new_type);
    Py_DECREF(PyC);
    return NULL;
  }

  PyObject *obj = PyObject_GetAttrString(PyC, structure->name);
  if (obj) {
    PyC_c_struct *result = (PyC_c_struct *)PyObject_CallObject(obj, NULL);
    result->structure = structure;
    result->pointer = malloc(structure->structSize);  // FIXME ?
    result->parentModule = module;

    return (PyObject *)result;
  }
  PyErr_SetString(py_BindingError,
                  "Unable to access PyCpp.c_struct base class");
  return NULL;
}

static PyObject *c_struct_iter(PyObject *self) {
  // TODO: implement
  Py_RETURN_NONE;
}

static PyObject *c_struct_next(PyObject *self) {
  // TODO: implement
  Py_RETURN_NONE;
}

static PyObject *c_struct_append(PyObject *self, PyObject *args) {
  // TODO :implement
  Py_RETURN_NONE;
}

static PyObject *c_struct_pop(PyObject *self) {
  // TODO :implement
  Py_RETURN_NONE;
}

static PyObject *c_struct_donot_free(PyObject *self, PyObject *args,
                                     PyObject *kwargs) {
  // TODO :implement
  Py_RETURN_NONE;
}

static PyObject *c_struct_to_pointer(PyObject *self) {
  // TODO :implement
  Py_RETURN_NONE;
}

static Py_ssize_t c_struct_len(PyObject *self) {
  // TODO: implement
  return 0;
}

static PyObject *c_struct_getitem(PyObject *self, PyObject *attr) {
  // TODO: implement
  Py_RETURN_NONE;
}

static int c_struct_setitem(PyObject *self, PyObject *attr, PyObject *value) {
  // TODO: implement
  return 0;
}

// ----- c_union -----

PyMappingMethods c_union_as_mapping = {
    .mp_length = &c_union_len,
    .mp_subscript = &c_union_getitem,
    .mp_ass_subscript = &c_union_setitem,
};

PyMemberDef c_union_members[] = {
    {"is_array", T_BOOL, offsetof(PyC_c_union, isArray), READONLY,
     "PyC.c_union.is_array"},
    {"__python_representation", T_OBJECT, offsetof(PyC_c_union, pyDictRepr),
     READONLY, "PyC.c_union.__python_representation"},
    {NULL, 0, 0, 0, NULL}};

PyMethodDef c_union_methods[] = {
    {"append", (PyCFunction)&c_union_append, METH_VARARGS, "c_union.append()"},
    {"pop", (PyCFunction)&c_union_pop, METH_NOARGS, "c_union.pop()"},
    {"donot_free", (PyCFunction)&c_union_donot_free,
     METH_VARARGS | METH_KEYWORDS, "c_union.donot_free()"},
    {"to_pointer", (PyCFunction)&c_union_to_pointer, METH_NOARGS,
     "c_union.to_pointer()"},
    {NULL, NULL, 0, NULL}};

PyTypeObject py_c_union_type = {
    PyVarObject_HEAD_INIT(NULL, 0).tp_name = "PyCpp.c_union",
    .tp_basicsize = sizeof(PyC_c_union),
    .tp_itemsize = 0,
    .tp_getattr = &c_union_getattr,
    .tp_setattr = &c_union_setattr,
    .tp_flags = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,
    .tp_call = c_union_call,
    .tp_doc = "PyCpp.c_union",
    .tp_iter = &c_union_iter,
    .tp_iternext = &c_union_next,
    .tp_methods = c_union_methods,
    .tp_members = c_union_members,
    .tp_init = &c_union_init,
    .tp_new = PyType_GenericNew,
    .tp_dealloc = &c_union_finalizer,
};

// c_union methods

// PyC.c_union.__init__
static int c_union_init(PyObject *self, PyObject *args, PyObject *kwargs) {
  // TODO: implement
  return 0;
}

// PyC.c_union.__getattr__
static PyObject *c_union_getattr(PyObject *self, char *attr) {
  PyC_c_union *selfType = (PyC_c_union *)self;

  for (size_t i = 0; i < selfType->u->attrCount; i++) {
    if (!(strcmp(attr, array_str_getat(selfType->u->attrNames, i)))) {
      char *data = (char *)selfType->pointer;

      return cppArg_to_pyArg(
          (selfType->pointer),
          *array_p_ffi_type_getat(selfType->u->attrTypes, i),
          array_CXTypeKind_getat(selfType->u->attrUnderlyingType, i),
          array_p_Structure_getat(selfType->u->attrUnderlyingStructs, i),
          array_p_Union_getat(selfType->u->attrUnderlyingUnions, i),
          selfType->parentModule);
    }
  }

  Py_RETURN_NONE;
}

// PyC.c_union.__setattr__
static int c_union_setattr(PyObject *self, char *attr, PyObject *pValue) {
  PyC_c_union *selfType = (PyC_c_union *)self;

  for (size_t i = 0; i < selfType->u->attrCount; i++) {
    if (!(strcmp(attr, array_str_getat(selfType->u->attrNames, i)))) {
      ffi_type *type = array_p_ffi_type_getat(selfType->u->attrTypes, i);

      bool should_free = false;
      void *data = pyArg_to_cppArg(pValue, *type, &should_free);

      if (type->type == FFI_TYPE_POINTER) {
        memcpy((selfType->pointer), data, type->size);
      } else if (PyObject_IsInstance(pValue, (PyObject *)&py_c_struct_type)) {
        memcpy((selfType->pointer), data,
               ((PyC_c_struct *)pValue)->structure->structSize);
      } else {
        memcpy((selfType->pointer), data, type->size);
      }

      if (should_free) {
        free(data);
      }

      return 0;
    }
  }

  PyErr_SetString(py_CppError, "union with given attribute not found");
  return -1;
}

// PyC.c_union.__del__
static void c_union_finalizer(PyObject *self) {
  // TODO: implement
  PyC_c_union *selfType = (PyC_c_union *)self;

  // free(selfType->pointer);
  Py_DECREF(selfType->parentModule);
  Py_TYPE(self)->tp_free((PyObject *)self);
}

// PyC.c_union.__call__
static PyObject *c_union_call(PyObject *self, PyObject *args,
                              PyObject *kwargs) {
  // TODO: implement
  PyC_c_union *selfType = (PyC_c_union *)self;

  PyObject *obj = PyObject_GetAttrString(PyC, selfType->u->name);
  if (obj) {
    PyC_c_union *result = (PyC_c_union *)PyObject_CallObject(obj, NULL);
    result->u = selfType->u;
    result->pointer = malloc(selfType->u->unionSize); // FIXME: malloc
    Py_INCREF(selfType->parentModule);
    result->parentModule = selfType->parentModule;

    return (PyObject *)result;
  }
  PyErr_SetString(py_BindingError, "Unable to access PyCpp.c_union base class");
  return NULL;
}

// helper function
PyObject *create_py_c_union(Union *u, PyObject *module) {
  typedef struct PyC_c_new {
    PyC_c_union super;
    // PyObject_HEAD;
  } PyC_c_new;

  PyTypeObject *py_c_new_type =
      malloc(sizeof(PyTypeObject)); // TODO: free this malloc

  if (py_c_new_type) {
    array_p_void_append(EXTRA_HEAP_MEMORY, py_c_new_type);
  } else {
    return PyErr_NoMemory();
  }

  char *struct_name = malloc(7 + strlen(u->name)); // TODO: free this malloc
  strcpy(struct_name, "PyCpp.");
  strcat(struct_name, u->name);

  *py_c_new_type = (PyTypeObject){
      PyVarObject_HEAD_INIT(NULL, 0).tp_name = struct_name,
      .tp_basicsize = sizeof(PyC_c_new),
      .tp_itemsize = 0,
      .tp_flags = Py_TPFLAGS_DEFAULT,
      .tp_doc = struct_name,
      .tp_new = PyType_GenericNew,
      .tp_base = &py_c_union_type,
  };

  if (struct_name) {
    array_p_void_append(EXTRA_HEAP_MEMORY, struct_name);
  } else {
    return PyErr_NoMemory();
  }

  if (PyType_Ready(py_c_new_type) < 0) {
    return NULL;
  }

  Py_INCREF(py_c_new_type);
  if (PyModule_AddObject(PyC, u->name, (PyObject *)py_c_new_type) < 0) {
    Py_DECREF(py_c_new_type);
    Py_DECREF(PyC);
    return NULL;
  }

  PyObject *obj = PyObject_GetAttrString(PyC, u->name);
  if (obj) {
    PyC_c_union *result = (PyC_c_union *)PyObject_CallObject(obj, NULL);
    result->u = u;
    result->pointer = malloc(u->unionSize); // FIXME ?
    result->parentModule = module;

    return (PyObject *)result;
  }
  PyErr_SetString(py_BindingError, "Unable to access PyCpp.c_union base class");
  return NULL;
}

static PyObject *c_union_iter(PyObject *self) {
  // TODO: implement
  Py_RETURN_NONE;
}

static PyObject *c_union_next(PyObject *self) {
  // TODO: implement
  Py_RETURN_NONE;
}

static PyObject *c_union_append(PyObject *self, PyObject *args) {
  // TODO :implement
  Py_RETURN_NONE;
}

static PyObject *c_union_pop(PyObject *self) {
  // TODO :implement
  Py_RETURN_NONE;
}

static PyObject *c_union_donot_free(PyObject *self, PyObject *args,
                                    PyObject *kwargs) {
  // TODO :implement
  Py_RETURN_NONE;
}

static PyObject *c_union_to_pointer(PyObject *self) {
  // TODO :implement
  Py_RETURN_NONE;
}

static Py_ssize_t c_union_len(PyObject *self) {
  // TODO: implement
  return 0;
}

static PyObject *c_union_getitem(PyObject *self, PyObject *attr) {
  // TODO: implement
  Py_RETURN_NONE;
}

static int c_union_setitem(PyObject *self, PyObject *attr, PyObject *value) {
  // TODO: implement
  return 0;
}
