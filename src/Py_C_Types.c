#define PY_SSIZE_T_CLEAN
#include "Py_C_Types.h"
#include "Python.h"

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

PyTypeObject py_c_int_type = {
    PyVarObject_HEAD_INIT(NULL, 0).tp_name = "PyCpp.c_int",
    .tp_basicsize = sizeof(PyC_c_int),
    .tp_itemsize = 0,
    .tp_as_number = &c_int_as_int,
    .tp_as_mapping = &c_int_as_mapping,
    .tp_getattr = &c_int_getattr,
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_doc = "PyCpp.c_int",
    .tp_iter = &c_int_iter,
    .tp_methods = c_int_methods,
    .tp_init = &c_int_init,
    .tp_new = PyType_GenericNew,
    .tp_finalize =
        &c_int_finalizer, // TODO: use .tp_getset for PyC_c_int's attributes
};

// ----- c_int: functions and methods -----

// PyC.c_int.__init__
static int c_int_init(PyObject *self, PyObject *args, PyObject *kwargs) {
  // TODO: implement init from Pyint
  // TODO: implement init from iter
  // TODO: implement init from c_pointer
  // TODO: implement keyword args: is_pointer, is_array

  PyC_c_int *selfType = (PyC_c_int *)self;

  int value;
  if (!PyArg_ParseTuple(args, "i", &value)) {
    return -1;
  }

  selfType->value = value;
  selfType->pointer = &(selfType->value);
  selfType->isPointer = false;
  selfType->isArray = false;
  selfType->arraySize = 0;
  selfType->arrayCapacity = 0;

  return 0;
}

// PyC.c_int.__iter__
static PyObject *c_int_iter(PyObject *self) {
  // TODO: implement c_int_iter

  PyC_c_int *selfType = (PyC_c_int *)self;
  Py_RETURN_NONE;
}

// PyC.c_int.__getattr__
static PyObject *c_int_getattr(PyObject *self, char *attr) {
  // TODO: implement c_int_getattr

  PyC_c_int *selfType = (PyC_c_int *)self;

  PyObject *value = PyObject_GenericGetAttr(self, PyUnicode_FromString(attr));
  if (value)
    return value;

  PyErr_Clear();
  Py_RETURN_NONE;
}

// PyC.c_int.__del__
static void c_int_finalizer(PyObject *self) {
  // TODO: implement c_int_finalizer

  PyC_c_int *selfType = (PyC_c_int *)self;
  return;
}

// PyC.c_int.append
static PyObject *c_int_append(PyObject *self, PyObject *args) {
  // TODO: implement c_int_append

  PyC_c_int *selfType = (PyC_c_int *)self;
  Py_RETURN_NONE;
}

// PyC.c_int.pop
static PyObject *c_int_pop(PyObject *self) {
  // TODO: implement c_int_pop

  PyC_c_int *selfType = (PyC_c_int *)self;
  Py_RETURN_NONE;
}

// PyC.c_int.value
static PyObject *c_int_value(PyObject *self) {
  PyC_c_int *selfType = (PyC_c_int *)self;
  return PyLong_FromLongLong(selfType->value);
}

// PyC.c_int.donot_free
static PyObject *c_int_donot_free(PyObject *self, PyObject *args,
                                  PyObject *kwargs) {
  // TODO: implement c_int_donot_free

  PyC_c_int *selfType = (PyC_c_int *)self;
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
  return PyLong_FromLong(selfType->value);
}

// PyC.c_int.__len__
static Py_ssize_t c_int_len(PyObject *self) {
  // TODO: implement c_int_len

  PyC_c_int *selfType = (PyC_c_int *)self;
  return 0;
}

// PyC.c_int.__getitem__
static PyObject *c_int_getitem(PyObject *self, PyObject *attr) {
  // TODO: implement c_int_getitem

  PyC_c_int *selfType = (PyC_c_int *)self;
  Py_RETURN_NONE;
}

// PyC.c_int.__setitem__
static int c_int_setitem(PyObject *self, PyObject *attr, PyObject *value) {
  // TODO: implement c_int_setitem

  PyC_c_int *selfType = (PyC_c_int *)self;
  return 0;
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

PyTypeObject py_c_double_type = {
    PyVarObject_HEAD_INIT(NULL, 0).tp_name = "PyCpp.c_double",
    .tp_basicsize = sizeof(PyC_c_double),
    .tp_itemsize = 0,
    .tp_as_number = &c_double_as_double,
    .tp_as_mapping = &c_double_as_mapping,
    .tp_getattr = &c_double_getattr,
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_doc = "PyCpp.c_double",
    .tp_iter = &c_double_iter,
    .tp_methods = c_double_methods,
    .tp_init = &c_double_init,
    .tp_new = PyType_GenericNew,
    .tp_finalize = &c_double_finalizer, // TODO: use .tp_getset for
                                        // PyC_c_double's attributes
};

// ----- c_double: functions and methods -----

// PyC.c_double.__init__
static int c_double_init(PyObject *self, PyObject *args, PyObject *kwargs) {
  // TODO: implement init from PyDouble
  // TODO: implement init from iter
  // TODO: implement init from c_pointer
  // TODO: implement keyword args: is_pointer, is_array

  PyC_c_double *selfType = (PyC_c_double *)self;

  double value;
  if (!PyArg_ParseTuple(args, "d", &value)) {
    return -1;
  }

  selfType->value = value;
  selfType->pointer = &(selfType->value);
  selfType->isPointer = false;
  selfType->isArray = false;
  selfType->arraySize = 0;
  selfType->arrayCapacity = 0;

  return 0;
}

// PyC.c_double.__iter__
static PyObject *c_double_iter(PyObject *self) {
  // TODO: implement c_double_iter

  PyC_c_double *selfType = (PyC_c_double *)self;
  Py_RETURN_NONE;
}

// PyC.c_double.__getattr__
static PyObject *c_double_getattr(PyObject *self, char *attr) {
  // TODO: implement c_double_getattr

  PyC_c_double *selfType = (PyC_c_double *)self;

  PyObject *value = PyObject_GenericGetAttr(self, PyUnicode_FromString(attr));
  if (value)
    return value;

  PyErr_Clear();
  Py_RETURN_NONE;
}

// PyC.c_double.__del__
static void c_double_finalizer(PyObject *self) {
  // TODO: implement c_double_finalizer

  PyC_c_double *selfType = (PyC_c_double *)self;
  return;
}

// PyC.c_double.append
static PyObject *c_double_append(PyObject *self, PyObject *args) {
  // TODO: implement c_double_append

  PyC_c_double *selfType = (PyC_c_double *)self;
  Py_RETURN_NONE;
}

// PyC.c_double.pop
static PyObject *c_double_pop(PyObject *self) {
  // TODO: implement c_double_pop

  PyC_c_double *selfType = (PyC_c_double *)self;
  Py_RETURN_NONE;
}

// PyC.c_double.value
static PyObject *c_double_value(PyObject *self) {
  PyC_c_double *selfType = (PyC_c_double *)self;
  return PyFloat_FromDouble(selfType->value);
}

// PyC.c_double.donot_free
static PyObject *c_double_donot_free(PyObject *self, PyObject *args,
                                     PyObject *kwargs) {
  // TODO: implement c_double_donot_free

  PyC_c_double *selfType = (PyC_c_double *)self;
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
  // TODO: implement c_double_len

  PyC_c_double *selfType = (PyC_c_double *)self;
  return 0;
}

// PyC.c_double.__getitem__
static PyObject *c_double_getitem(PyObject *self, PyObject *attr) {
  // TODO: implement c_double_getitem

  PyC_c_double *selfType = (PyC_c_double *)self;
  Py_RETURN_NONE;
}

// PyC.c_double.__setitem__
static int c_double_setitem(PyObject *self, PyObject *attr, PyObject *value) {
  // TODO: implement c_double_setitem

  PyC_c_double *selfType = (PyC_c_double *)self;
  return 0;
}

// ----- c_bool -----
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
    .tp_finalize =
        &c_bool_finalizer, // TODO: use .tp_getset for PyC_c_bool's attributes
};

// ----- c_bool: functions and methods -----

// PyC.c_bool.__init__
static int c_bool_init(PyObject *self, PyObject *args, PyObject *kwargs) {
  // TODO: implement init from PyTrue / PyFalse
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
  return;
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
