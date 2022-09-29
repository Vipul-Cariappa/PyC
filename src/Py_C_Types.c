#define PY_SSIZE_T_CLEAN
#include "Py_C_Types.h"
#include "PyC.h"
#include "Python.h"
#include "structmember.h"
#include <stddef.h>

struct Custom_s_PyTypeObject {
  PyTypeObject _;
  Structure *s;
  PyObject *parentModule;
};

struct Custom_u_PyTypeObject {
  PyTypeObject _;
  Union *u;
  PyObject *parentModule;
};

// TODO: better error msg fro c_utypes (unsigned types)

// ----- c_void -----

PyMethodDef c_void_methods[] = {{NULL, NULL, 0, NULL}};

PyMemberDef c_void_members[] = {{NULL, 0, 0, 0, NULL}};

PyGetSetDef c_void_getsetdef[] = {
    {"free_on_no_reference", &c_void_freeOnDel_getter, &c_void_freeOnDel_setter,
     C_TYPE_FREE_ON_NO_REFERENCE_DOC, NULL},
    {NULL, NULL, NULL, NULL},
};

PyTypeObject py_c_void_type = {
    PyVarObject_HEAD_INIT(NULL, 0).tp_name = "PyCpp.c_void",
    .tp_basicsize = sizeof(PyC_c_void),
    .tp_itemsize = 0,
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_doc = "PyCpp.c_void",
    .tp_methods = c_void_methods,
    .tp_members = c_void_members,
    .tp_getset = c_void_getsetdef,
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

static PyObject *c_void_freeOnDel_getter(PyObject *self, void *closure) {
  PyC_c_void *selfType = (PyC_c_void *)self;

  if (selfType->freeOnDel) {
    Py_RETURN_TRUE;
  }

  Py_RETURN_FALSE;
}

static int c_void_freeOnDel_setter(PyObject *self, PyObject *value,
                                   void *closure) {
  PyC_c_void *selfType = (PyC_c_void *)self;

  if (!value) {
    PyErr_SetString(PyExc_AttributeError,
                    "Cannot delete c_int.free_on_no_reference attrubute");
    return -1;
  }

  int flag = PyObject_IsTrue(value);
  if (flag == -1) {
    return -1;
  }

  selfType->freeOnDel = flag;
  return 0;
}

// ----- c_int -----
PyNumberMethods c_int_as_int = {
    .nb_int = &c_int_to_int,
    .nb_float = &c_int_to_float,
    .nb_bool = &c_int_to_bool,
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
    {NULL, NULL, 0, NULL}};

PyGetSetDef c_int_getsetdef[] = {
    {"free_on_no_reference", &c_int_freeOnDel_getter, &c_int_freeOnDel_setter,
     C_TYPE_FREE_ON_NO_REFERENCE_DOC, NULL},
    {NULL, NULL, NULL, NULL},
};

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
    .tp_getset = c_int_getsetdef,
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
    .tp_getset = c_int_getsetdef,
    .tp_init = &c_int_init,
    .tp_new = PyType_GenericNew,
    .tp_dealloc = &c_int_finalizer,
};

// ----- c_int: functions and methods -----

// PyC.c_int.__init__
static int c_int_init(PyObject *self, PyObject *args, PyObject *kwargs) {
  PyC_c_int *selfType = (PyC_c_int *)self;

  if (kwargs) {
    // calling c_type with keyword pointer=None
    // only for internal use
    PyObject *key = PyUnicode_FromFormat("pointer");
    if (PyDict_Contains(kwargs, key) == 1) {
      PyObject *pointer_value = PyDict_GetItem(kwargs, key);

      if (pointer_value == Py_None) {
        selfType->value = 0;
        selfType->pointer = NULL;
        selfType->freeOnDel = false;
        selfType->isPointer = true;
        selfType->isArray = false;
        selfType->arraySize = 0;
        selfType->arrayCapacity = 0;
        selfType->_i = 0;
        Py_DECREF(key);
        return 0;
      } else {
        PyErr_SetString(py_BindingError, "calling c_int with pointer keyword "
                                         "is restricted for internal use only");
        Py_DECREF(key);
        return -1;
      }
    }
    Py_DECREF(key);
  }

  PyObject *arg_1 = PyTuple_GetItem(args, 0);
  if (!arg_1) {
    PyErr_SetString(PyExc_TypeError, "Expected int or sequence of ints");
    return -1;
  }

  if (PyNumber_Check(arg_1)) {
    PyObject *tmp = PyNumber_Long(arg_1);
    int value = PyLong_AsLongLong(tmp);

    selfType->value = value;
    selfType->freeOnDel = false;
    selfType->pointer = &(selfType->value);
    selfType->isPointer = false;
    selfType->isArray = false;
    selfType->arraySize = 0;
    selfType->arrayCapacity = 0;
    selfType->_i = 0;

    Py_DECREF(tmp);

    return 0;

  } else if (PySequence_Check(arg_1)) {
    size_t len = PySequence_Size(arg_1);

    selfType->value = 0;
    selfType->freeOnDel = true;
    selfType->pointer = calloc(len + 1, sizeof(int));
    selfType->isPointer = true;
    selfType->isArray = true;
    selfType->arraySize = len;
    selfType->arrayCapacity = len + 1;
    selfType->_i = 0;

    for (size_t i = 0; i < len; i++) {
      PyObject *element = PySequence_GetItem(arg_1, i);

      if (!(PyNumber_Check(element))) {
        free(selfType->pointer);
        selfType->pointer = NULL;
        selfType->isPointer = false;
        selfType->freeOnDel = false;
        selfType->isArray = false;
        selfType->arraySize = 0;
        selfType->arrayCapacity = 0;
        PyErr_SetString(
            PyExc_TypeError,
            "Expected all elements of the sequence to be int or float");
        return -1;
      }

      PyObject *tmp = PyNumber_Long(element);

      int value = PyLong_AsLong(tmp);
      selfType->pointer[i] = value;

      Py_DECREF(tmp);
    }

    selfType->pointer[len] = 0;

    return 0;
  }

  PyErr_SetString(PyExc_TypeError, "Expected int or sequence of ints");
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
  // FIXME: all itearators
  PyC_c_int *selfType = (PyC_c_int *)self;
  PyObject *rvalue = NULL;

  size_t index = selfType->_i;

  if (selfType->arraySize > index) {
    if (PyObject_IsInstance(self, (PyObject *)&py_c_int_type)) {
      rvalue = PyLong_FromLongLong(*(selfType->pointer));
    }

    rvalue = PyLong_FromLongLong((unsigned int)*(selfType->pointer));
  }

  (selfType->_i)++;

  return rvalue;
}

// PyC.c_int.__del__
static void c_int_finalizer(PyObject *self) {
  PyC_c_int *selfType = (PyC_c_int *)self;

  if (selfType->freeOnDel)
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
    return PyLong_FromLongLong(*(selfType->pointer));
  }

  return PyLong_FromLongLong((unsigned int)*(selfType->pointer));
}

// PyC.c_int.__int__
static PyObject *c_int_to_int(PyObject *self) {
  PyC_c_int *selfType = (PyC_c_int *)self;

  if (PyObject_IsInstance(self, (PyObject *)&py_c_int_type)) {
    return PyLong_FromLongLong(*(selfType->pointer));
  }

  return PyLong_FromLongLong((unsigned int)*(selfType->pointer));
}

// PyC.c_int.__float__
static PyObject *c_int_to_float(PyObject *self) {
  PyC_c_int *selfType = (PyC_c_int *)self;

  if (PyObject_IsInstance(self, (PyObject *)&py_c_int_type)) {
    return PyFloat_FromDouble(*(selfType->pointer));
  }

  return PyFloat_FromDouble((unsigned int)*(selfType->pointer));
}

// PyC.c_int.__bool__
static int c_int_to_bool(PyObject *self) {
  PyC_c_int *selfType = (PyC_c_int *)self;

  if (*selfType->pointer) {
    return 1;
  }

  return 0;
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

  long long index = PyLong_AsLongLong(attr);

  if (selfType->arraySize > index) {
    if (PyObject_IsInstance(self, (PyObject *)&py_c_int_type)) {
      return PyLong_FromLongLong((selfType->pointer)[index]);
    }

    return PyLong_FromLongLong((unsigned int)(selfType->pointer)[index]);
  }

  PyErr_SetString(PyExc_IndexError, "Index out of range");
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

  PyErr_SetString(PyExc_IndexError, "Index out of range");
  return -1;
}

// PyC.c_int.free_on_no_reference getter
static PyObject *c_int_freeOnDel_getter(PyObject *self, void *closure) {
  PyC_c_int *selfType = (PyC_c_int *)self;

  if (selfType->freeOnDel) {
    Py_RETURN_TRUE;
  }

  Py_RETURN_FALSE;
}

// PyC.c_int.free_on_no_reference setter
static int c_int_freeOnDel_setter(PyObject *self, PyObject *value,
                                  void *closure) {
  PyC_c_int *selfType = (PyC_c_int *)self;

  if (!value) {
    PyErr_SetString(PyExc_AttributeError,
                    "Cannot delete c_int.free_on_no_reference attrubute");
    return -1;
  }

  int flag = PyObject_IsTrue(value);
  if (flag == -1) {
    return -1;
  }

  selfType->freeOnDel = flag;
  return 0;
}

// ----- c_double -----
PyNumberMethods c_double_as_double = {
    .nb_int = &c_double_to_int,
    .nb_float = &c_double_to_float,
    .nb_bool = &c_double_to_bool,
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
    {NULL, NULL, 0, NULL}};

PyMemberDef c_double_members[] = {
    {"is_pointer", T_BOOL, offsetof(PyC_c_double, isPointer), READONLY,
     "PyC.c_double.is_pointer"},
    {"is_array", T_BOOL, offsetof(PyC_c_double, isArray), READONLY,
     "PyC.c_double.is_array"},
    {NULL, 0, 0, 0, NULL}};

PyGetSetDef c_double_getsetdef[] = {
    {"free_on_no_reference", &c_double_freeOnDel_getter,
     &c_double_freeOnDel_setter, C_TYPE_FREE_ON_NO_REFERENCE_DOC, NULL},
    {NULL, NULL, NULL, NULL},
};

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
    .tp_getset = c_double_getsetdef,
    .tp_init = &c_double_init,
    .tp_new = PyType_GenericNew,
    .tp_dealloc = &c_double_finalizer,
};

// ----- c_double: functions and methods -----

// PyC.c_double.__init__
static int c_double_init(PyObject *self, PyObject *args, PyObject *kwargs) {
  PyC_c_double *selfType = (PyC_c_double *)self;

  if (kwargs) {
    // calling c_type with keyword pointer=None
    // only for internal use
    PyObject *key = PyUnicode_FromFormat("pointer");
    if (PyDict_Contains(kwargs, key) == 1) {
      PyObject *pointer_value = PyDict_GetItem(kwargs, key);

      if (pointer_value == Py_None) {
        selfType->value = 0;
        selfType->pointer = NULL;
        selfType->freeOnDel = false;
        selfType->isPointer = true;
        selfType->isArray = false;
        selfType->arraySize = 0;
        selfType->arrayCapacity = 0;
        selfType->_i = 0;
        Py_DECREF(key);
        return 0;
      } else {
        PyErr_SetString(py_BindingError,
                        "calling c_double with pointer keyword "
                        "is restricted for internal use only");
        Py_DECREF(key);
        return -1;
      }
    }
    Py_DECREF(key);
  }

  PyObject *arg_1 = PyTuple_GetItem(args, 0);
  if (!arg_1) {
    PyErr_SetString(PyExc_TypeError, "Expected float or sequence of floats");
    return -1;
  }

  if (PyNumber_Check(arg_1)) {
    PyObject *tmp = PyNumber_Float(arg_1);
    double value = PyFloat_AsDouble(arg_1);

    selfType->value = value;
    selfType->pointer = &(selfType->value);
    selfType->isPointer = false;
    selfType->isArray = false;
    selfType->arraySize = 0;
    selfType->arrayCapacity = 0;
    selfType->_i = 0;

    Py_DECREF(tmp);

    return 0;

  } else if (PySequence_Check(arg_1)) {
    size_t len = PySequence_Size(arg_1);

    selfType->value = 0;
    selfType->pointer = calloc(len + 1, sizeof(int));
    selfType->isPointer = true;
    selfType->isArray = true;
    selfType->arraySize = len;
    selfType->arrayCapacity = len + 1;
    selfType->_i = 0;

    for (size_t i = 0; i < len; i++) {
      PyObject *element = PySequence_GetItem(arg_1, i);

      if (!(PyNumber_Check(element))) {
        free(selfType->pointer);
        selfType->pointer = NULL;
        selfType->isPointer = false;
        selfType->freeOnDel = false;
        selfType->isArray = false;
        selfType->arraySize = 0;
        selfType->arrayCapacity = 0;
        PyErr_SetString(PyExc_TypeError,
                        "Expected all elements of sequence to be float or int");
        return -1;
      }

      PyObject *tmp = PyNumber_Float(element);

      double value = PyFloat_AsDouble(tmp);
      selfType->pointer[i] = value;

      Py_DECREF(tmp);
    }

    selfType->pointer[len] = 0;

    return 0;
  }

  PyErr_SetString(PyExc_TypeError, "Expected float or sequence of floats");
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
  return PyFloat_FromDouble(*(selfType->pointer));
}

// PyC.c_double.__int__
static PyObject *c_double_to_int(PyObject *self) {
  PyC_c_double *selfType = (PyC_c_double *)self;
  return PyLong_FromDouble(*(selfType->pointer));
}

// PyC.c_double.__float__
static PyObject *c_double_to_float(PyObject *self) {
  PyC_c_double *selfType = (PyC_c_double *)self;
  return PyFloat_FromDouble(*(selfType->pointer));
}

// PyC.c_double.__bool__
static int c_double_to_bool(PyObject *self) {
  PyC_c_double *selfType = (PyC_c_double *)self;

  if (*selfType->pointer) {
    return 1;
  }

  return 0;
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

  PyErr_SetString(PyExc_IndexError, "Index out of range");
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

  PyErr_SetString(PyExc_IndexError, "Index out of range");
  return -1;
}

// PyC.c_double.free_on_no_reference getter
static PyObject *c_double_freeOnDel_getter(PyObject *self, void *closure) {
  PyC_c_double *selfType = (PyC_c_double *)self;

  if (selfType->freeOnDel) {
    Py_RETURN_TRUE;
  }

  Py_RETURN_FALSE;
}

// PyC.c_double.free_on_no_reference setter
static int c_double_freeOnDel_setter(PyObject *self, PyObject *value,
                                     void *closure) {
  PyC_c_double *selfType = (PyC_c_double *)self;

  if (!value) {
    PyErr_SetString(PyExc_AttributeError,
                    "Cannot delete c_double.free_on_no_reference attrubute");
    return -1;
  }

  int flag = PyObject_IsTrue(value);
  if (flag == -1) {
    return -1;
  }

  selfType->freeOnDel = flag;
  return 0;
}

// ----- c_float -----
PyNumberMethods c_float_as_float = {
    .nb_int = &c_float_to_int,
    .nb_float = &c_float_to_float,
    .nb_bool = &c_float_to_bool,
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
    {NULL, NULL, 0, NULL}};

PyMemberDef c_float_members[] = {
    {"is_pointer", T_BOOL, offsetof(PyC_c_float, isPointer), READONLY,
     "PyC.c_float.is_pointer"},
    {"is_array", T_BOOL, offsetof(PyC_c_float, isArray), READONLY,
     "PyC.c_float.is_array"},
    {NULL, 0, 0, 0, NULL}};

PyGetSetDef c_float_getsetdef[] = {
    {"free_on_no_reference", &c_float_freeOnDel_getter,
     &c_float_freeOnDel_setter, C_TYPE_FREE_ON_NO_REFERENCE_DOC, NULL},
    {NULL, NULL, NULL, NULL},
};

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
    .tp_getset = c_float_getsetdef,
    .tp_init = &c_float_init,
    .tp_new = PyType_GenericNew,
    .tp_dealloc = &c_float_finalizer,
};

// ----- c_float: functions and methods -----

// PyC.c_float.__init__
static int c_float_init(PyObject *self, PyObject *args, PyObject *kwargs) {
  PyC_c_float *selfType = (PyC_c_float *)self;

  if (kwargs) {
    // calling c_type with keyword pointer=None
    // only for internal use
    PyObject *key = PyUnicode_FromFormat("pointer");
    if (PyDict_Contains(kwargs, key) == 1) {
      PyObject *pointer_value = PyDict_GetItem(kwargs, key);

      if (pointer_value == Py_None) {
        selfType->value = 0;
        selfType->pointer = NULL;
        selfType->freeOnDel = false;
        selfType->isPointer = true;
        selfType->isArray = false;
        selfType->arraySize = 0;
        selfType->arrayCapacity = 0;
        selfType->_i = 0;
        Py_DECREF(key);
        return 0;
      } else {
        PyErr_SetString(py_BindingError, "calling c_float with pointer keyword "
                                         "is restricted for internal use only");
        Py_DECREF(key);
        return -1;
      }
    }
    Py_DECREF(key);
  }

  PyObject *arg_1 = PyTuple_GetItem(args, 0);
  if (!arg_1) {
    PyErr_SetString(PyExc_TypeError, "Expected int or sequence of floats");
    return -1;
  }

  if (PyNumber_Check(arg_1)) {
    PyObject *tmp = PyNumber_Float(arg_1);
    float value = PyFloat_AsDouble(tmp);

    selfType->value = value;
    selfType->pointer = &(selfType->value);
    selfType->isPointer = false;
    selfType->isArray = false;
    selfType->arraySize = 0;
    selfType->arrayCapacity = 0;
    selfType->_i = 0;

    Py_DECREF(tmp);

    return 0;

  } else if (PySequence_Check(arg_1)) {
    size_t len = PySequence_Size(arg_1);

    selfType->value = 0;
    selfType->pointer = calloc(len + 1, sizeof(int));
    selfType->isPointer = true;
    selfType->isArray = true;
    selfType->arraySize = len;
    selfType->arrayCapacity = len + 1;
    selfType->_i = 0;

    for (size_t i = 0; i < len; i++) {
      PyObject *element = PySequence_GetItem(arg_1, i);

      if (!(PyNumber_Check(element))) {
        free(selfType->pointer);
        selfType->pointer = NULL;
        selfType->isPointer = false;
        selfType->freeOnDel = false;
        selfType->isArray = false;
        selfType->arraySize = 0;
        selfType->arrayCapacity = 0;
        PyErr_SetString(PyExc_TypeError,
                        "Expected all elements of sequence to be float or int");
        return -1;
      }

      PyObject *tmp = PyNumber_Float(element);

      float value = PyFloat_AsDouble(tmp);
      selfType->pointer[i] = value;

      Py_DECREF(tmp);
    }

    selfType->pointer[len] = 0;

    return 0;
  }

  PyErr_SetString(PyExc_TypeError, "Expected int or sequence of floats");
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
  return PyFloat_FromDouble(*(selfType->pointer));
}

// PyC.c_float.__int__
static PyObject *c_float_to_int(PyObject *self) {
  PyC_c_float *selfType = (PyC_c_float *)self;
  return PyLong_FromDouble(*(selfType->pointer));
}

// PyC.c_float.__float__
static PyObject *c_float_to_float(PyObject *self) {
  PyC_c_float *selfType = (PyC_c_float *)self;
  return PyFloat_FromDouble(*(selfType->pointer));
}

// PyC.c_float.__bool__
static int c_float_to_bool(PyObject *self) {
  PyC_c_float *selfType = (PyC_c_float *)self;
  if (*selfType->pointer) {
    return 1;
  }

  return 0;
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

  PyErr_SetString(PyExc_IndexError, "Index out of range");
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

  PyErr_SetString(PyExc_IndexError, "Index out of range");
  return -1;
}

static PyObject *c_float_freeOnDel_getter(PyObject *self, void *closure) {
  PyC_c_float *selfType = (PyC_c_float *)self;

  if (selfType->freeOnDel) {
    Py_RETURN_TRUE;
  }

  Py_RETURN_FALSE;
}

static int c_float_freeOnDel_setter(PyObject *self, PyObject *value,
                                    void *closure) {
  PyC_c_float *selfType = (PyC_c_float *)self;

  if (!value) {
    PyErr_SetString(PyExc_AttributeError,
                    "Cannot delete c_int.free_on_no_reference attrubute");
    return -1;
  }

  int flag = PyObject_IsTrue(value);
  if (flag == -1) {
    return -1;
  }

  selfType->freeOnDel = flag;
  return 0;
}

// ----- c_bool -----
PyNumberMethods c_bool_as_bool = {
    .nb_int = &c_bool_to_int,
    .nb_float = &c_bool_to_float,
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
    {NULL, NULL, 0, NULL}};

PyMemberDef c_bool_members[] = {
    {"is_pointer", T_BOOL, offsetof(PyC_c_bool, isPointer), READONLY,
     "PyC.c_bool.is_pointer"},
    {"is_array", T_BOOL, offsetof(PyC_c_bool, isArray), READONLY,
     "PyC.c_bool.is_array"},
    {NULL, 0, 0, 0, NULL}};

PyGetSetDef c_bool_getsetdef[] = {
    {"free_on_no_reference", &c_bool_freeOnDel_getter, &c_bool_freeOnDel_setter,
     C_TYPE_FREE_ON_NO_REFERENCE_DOC, NULL},
    {NULL, NULL, NULL, NULL},
};

PyTypeObject py_c_bool_type = {
    PyVarObject_HEAD_INIT(NULL, 0).tp_name = "PyCpp.c_bool",
    .tp_basicsize = sizeof(PyC_c_bool),
    .tp_itemsize = 0,
    .tp_as_number = &c_bool_as_bool,
    .tp_as_mapping = &c_bool_as_mapping,
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_doc = "PyCpp.c_bool",
    .tp_iter = &c_bool_iter,
    .tp_iternext = &c_bool_next,
    .tp_methods = c_bool_methods,
    .tp_members = c_bool_members,
    .tp_getset = c_bool_getsetdef,
    .tp_init = &c_bool_init,
    .tp_new = PyType_GenericNew,
    .tp_dealloc = &c_bool_finalizer,
};

// ----- c_bool: functions and methods -----

// PyC.c_bool.__init__
static int c_bool_init(PyObject *self, PyObject *args, PyObject *kwargs) {
  PyC_c_bool *selfType = (PyC_c_bool *)self;

  if (kwargs) {
    // calling c_type with keyword pointer=None
    // only for internal use
    PyObject *key = PyUnicode_FromFormat("pointer");
    if (PyDict_Contains(kwargs, key) == 1) {
      PyObject *pointer_value = PyDict_GetItem(kwargs, key);

      if (pointer_value == Py_None) {
        selfType->value = 0;
        selfType->pointer = NULL;
        selfType->freeOnDel = false;
        selfType->isPointer = true;
        selfType->isArray = false;
        selfType->arraySize = 0;
        selfType->arrayCapacity = 0;
        selfType->_i = 0;
        Py_DECREF(key);
        return 0;
      }

      PyErr_SetString(py_BindingError, "calling c_bool with pointer keyword "
                                       "is restricted for internal use only");
      Py_DECREF(key);
      return -1;
    }
    Py_DECREF(key);
  }

  PyObject *arg_1 = PyTuple_GetItem(args, 0);
  if (!arg_1) {
    PyErr_SetString(PyExc_TypeError, "Expected int or sequence of ints");
    return -1;
  }

  if (PySequence_Check(arg_1)) {
    size_t len = PySequence_Size(arg_1);

    selfType->value = 0;
    selfType->freeOnDel = true;
    selfType->pointer = calloc(len + 1, sizeof(bool));
    selfType->isPointer = true;
    selfType->isArray = true;
    selfType->arraySize = len;
    selfType->arrayCapacity = len + 1;
    selfType->_i = 0;

    for (size_t i = 0; i < len; i++) {
      PyObject *element = PySequence_GetItem(arg_1, i);
      int value = PyObject_IsTrue(element);

      if (value == 1) {
        selfType->pointer[i] = 1;
      } else if (value == 0) {
        selfType->pointer[i] = 0;
      } else {
        free(selfType->pointer);
        selfType->pointer = NULL;
        selfType->isPointer = false;
        selfType->freeOnDel = false;
        selfType->isArray = false;
        selfType->arraySize = 0;
        selfType->arrayCapacity = 0;
        PyErr_SetString(PyExc_TypeError,
                        "Expected all elements of the sequence to be boolean");
        return -1;
      }
    }

    selfType->pointer[len] = 0;

    return 0;
  }

  int value = PyObject_IsTrue(arg_1);

  if (value == 1) {
    selfType->value = 1;
  } else if (value == 0) {
    selfType->value = 0;
  } else {
    PyErr_SetString(PyExc_TypeError,
                    "Expected all elements of the sequence to be boolean");
    return -1;
  }

  selfType->freeOnDel = false;
  selfType->pointer = &(selfType->value);
  selfType->isPointer = false;
  selfType->isArray = false;
  selfType->arraySize = 0;
  selfType->arrayCapacity = 0;
  selfType->_i = 0;

  return 0;
}

// PyC.c_bool.__iter__
static PyObject *c_bool_iter(PyObject *self) {
  PyC_c_bool *selfType = (PyC_c_bool *)self;

  if (selfType->isArray) {
    selfType->_i = 0;
    Py_INCREF(self);
    return self;
  }

  PyErr_SetString(py_CppError,
                  "given c_bool instance is not an array type instance");
  return NULL;
}

// PyC.c_bool.__next__
static PyObject *c_bool_next(PyObject *self) {
  PyC_c_bool *selfType = (PyC_c_bool *)self;

  size_t index = selfType->_i;

  if (selfType->arraySize > index) {
    (selfType->_i)++;

    if (selfType->pointer[index]) {
      Py_RETURN_TRUE;
    } else {
      Py_RETURN_FALSE;
    }
  }

  return NULL;
}

// PyC.c_bool.__del__
static void c_bool_finalizer(PyObject *self) {
  PyC_c_bool *selfType = (PyC_c_bool *)self;

  if (selfType->freeOnDel)
    free(selfType->pointer);

  Py_TYPE(self)->tp_free((PyObject *)self);
}

// PyC.c_bool.append
static PyObject *c_bool_append(PyObject *self, PyObject *args) {
  PyC_c_bool *selfType = (PyC_c_bool *)self;

  int value;
  if (!PyArg_ParseTuple(args, "p", &value)) {
    return NULL;
  }

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

  if (value) {
    Py_RETURN_TRUE;
  }
  Py_RETURN_FALSE;
}

// PyC.c_bool.pop
static PyObject *c_bool_pop(PyObject *self) {
  PyC_c_bool *selfType = (PyC_c_bool *)self;

  if (!(selfType->isArray)) {
    PyErr_SetString(py_CppError,
                    "given instance of c_bool is not an array type instance");
    return NULL;
  }

  if (!(selfType->arraySize)) {
    PyErr_SetString(py_CppError, "no elements in the array to pop");
    return NULL;
  }

  bool value = selfType->pointer[selfType->arraySize - 1];
  (selfType->arraySize)--;

  if ((selfType->arraySize * 2) < selfType->arrayCapacity) {
    selfType->pointer =
        realloc(selfType->pointer, (selfType->arraySize) * sizeof(int));
    selfType->arrayCapacity = selfType->arraySize;
  }

  if (value) {
    Py_RETURN_TRUE;
  }
  Py_RETURN_FALSE;
}

// PyC.c_bool.value
static PyObject *c_bool_value(PyObject *self) {
  PyC_c_bool *selfType = (PyC_c_bool *)self;

  if (*(selfType->pointer)) {
    Py_RETURN_TRUE;
  }
  Py_RETURN_FALSE;
}

// PyC.c_bool.__int__
static PyObject *c_bool_to_int(PyObject *self) {
  PyC_c_bool *selfType = (PyC_c_bool *)self;

  return PyLong_FromLongLong(*(selfType->pointer));
}

// PyC.c_bool.__float__
static PyObject *c_bool_to_float(PyObject *self) {
  PyC_c_bool *selfType = (PyC_c_bool *)self;

  return PyFloat_FromDouble(*(selfType->pointer));
}

// PyC.c_bool.__bool__
static int c_bool_to_bool(PyObject *self) {
  PyC_c_bool *selfType = (PyC_c_bool *)self;

  if (*(selfType->pointer)) {
    return 1;
  }
  return 0;
}

// PyC.c_bool.__len__
static Py_ssize_t c_bool_len(PyObject *self) {
  PyC_c_bool *selfType = (PyC_c_bool *)self;
  return selfType->arraySize;
}

// PyC.c_bool.__getitem__
static PyObject *c_bool_getitem(PyObject *self, PyObject *attr) {
  PyC_c_bool *selfType = (PyC_c_bool *)self;

  if (!(selfType->isArray)) {
    PyErr_SetString(py_CppError,
                    "given instance of c_bool is not an array type instance");
    return NULL;
  }

  if (!(PyNumber_Check(attr))) {
    PyErr_SetString(PyExc_TypeError,
                    "Expected boolean type got some other type");
    return NULL;
  }

  size_t index = PyLong_AsLongLong(attr);

  if (selfType->arraySize > index) {
    if (selfType->pointer[index]) {
      Py_RETURN_TRUE;
    }
    Py_RETURN_FALSE;
  }

  PyErr_SetString(PyExc_IndexError, "Index out of range");
  return NULL;
}

// PyC.c_bool.__setitem__
static int c_bool_setitem(PyObject *self, PyObject *attr, PyObject *value) {
  PyC_c_bool *selfType = (PyC_c_bool *)self;

  if (!(selfType->isArray)) {
    PyErr_SetString(py_CppError,
                    "given instance of c_bool is not an array type instance");
    return -1;
  }

  int _value = PyObject_IsTrue(value);
  if (_value == -1) {
    PyErr_SetString(PyExc_TypeError,
                    "Expected boolean type got some other type");
    return -1;
  }

  size_t index = PyLong_AsLongLong(attr);

  if (selfType->arraySize > index) {
    if (_value == 1) {
      selfType->pointer[index] = 1;
    }
    selfType->pointer[index] = 0;

    return 0;
  }

  PyErr_SetString(PyExc_IndexError, "Index out of range");
  return -1;
}

// PyC.c_bool.free_on_no_reference getter
static PyObject *c_bool_freeOnDel_getter(PyObject *self, void *closure) {
  PyC_c_bool *selfType = (PyC_c_bool *)self;

  if (selfType->freeOnDel) {
    Py_RETURN_TRUE;
  }

  Py_RETURN_FALSE;
}

// PyC.c_bool.free_on_no_reference setter
static int c_bool_freeOnDel_setter(PyObject *self, PyObject *value,
                                   void *closure) {
  PyC_c_bool *selfType = (PyC_c_bool *)self;

  if (!value) {
    PyErr_SetString(PyExc_AttributeError,
                    "Cannot delete c_bool.free_on_no_reference attrubute");
    return -1;
  }

  int flag = PyObject_IsTrue(value);
  if (flag == -1) {
    return -1;
  }

  selfType->freeOnDel = flag;
  return 0;
}

// ----- c_short -----
PyNumberMethods c_short_as_short = {
    .nb_int = &c_short_to_int,
    .nb_float = &c_short_to_float,
    .nb_bool = &c_short_to_bool,
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
    {NULL, NULL, 0, NULL}};

PyMemberDef c_short_members[] = {
    {"is_pointer", T_BOOL, offsetof(PyC_c_short, isPointer), READONLY,
     "PyC.c_short.is_pointer"},
    {"is_array", T_BOOL, offsetof(PyC_c_short, isArray), READONLY,
     "PyC.c_short.is_array"},
    {NULL, 0, 0, 0, NULL}};

PyGetSetDef c_short_getsetdef[] = {
    {"free_on_no_reference", &c_short_freeOnDel_getter,
     &c_short_freeOnDel_setter, C_TYPE_FREE_ON_NO_REFERENCE_DOC, NULL},
    {NULL, NULL, NULL, NULL},
};

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
    .tp_getset = c_short_getsetdef,
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
    .tp_getset = c_short_getsetdef,
    .tp_init = &c_short_init,
    .tp_new = PyType_GenericNew,
    .tp_dealloc = &c_short_finalizer,
};

// ----- c_short: functions and methods -----

// PyC.c_short.__init__
static int c_short_init(PyObject *self, PyObject *args, PyObject *kwargs) {
  PyC_c_short *selfType = (PyC_c_short *)self;

  if (kwargs) {
    // calling c_type with keyword pointer=None
    // only for internal use
    PyObject *key = PyUnicode_FromFormat("pointer");
    if (PyDict_Contains(kwargs, key) == 1) {
      PyObject *pointer_value = PyDict_GetItem(kwargs, key);

      if (pointer_value == Py_None) {
        selfType->value = 0;
        selfType->pointer = NULL;
        selfType->freeOnDel = false;
        selfType->isPointer = true;
        selfType->isArray = false;
        selfType->arraySize = 0;
        selfType->arrayCapacity = 0;
        selfType->_i = 0;
        Py_DECREF(key);
        return 0;
      } else {
        PyErr_SetString(py_BindingError, "calling c_type with pointer keyword "
                                         "is restricted for internal use only");
        Py_DECREF(key);
        return -1;
      }
    }
    Py_DECREF(key);
  }

  PyObject *arg_1 = PyTuple_GetItem(args, 0);
  if (!arg_1) {
    PyErr_SetString(PyExc_TypeError, "Expected int or sequence of ints");
    return -1;
  }

  if (PyNumber_Check(arg_1)) {
    PyObject *tmp = PyNumber_Long(arg_1);
    short value = PyLong_AsLong(tmp);

    selfType->value = value;
    selfType->pointer = &(selfType->value);
    selfType->isPointer = false;
    selfType->isArray = false;
    selfType->arraySize = 0;
    selfType->arrayCapacity = 0;
    selfType->_i = 0;

    Py_DECREF(tmp);

    return 0;

  } else if (PySequence_Check(arg_1)) {
    size_t len = PySequence_Size(arg_1);

    selfType->value = 0;
    selfType->pointer = calloc(len + 1, sizeof(int));
    selfType->isPointer = true;
    selfType->isArray = true;
    selfType->arraySize = len;
    selfType->arrayCapacity = len + 1;
    selfType->_i = 0;

    for (size_t i = 0; i < len; i++) {
      PyObject *element = PySequence_GetItem(arg_1, i);

      if (!(PyNumber_Check(element))) {
        free(selfType->pointer);
        selfType->pointer = NULL;
        selfType->isPointer = false;
        selfType->freeOnDel = false;
        selfType->isArray = false;
        selfType->arraySize = 0;
        selfType->arrayCapacity = 0;
        PyErr_SetString(PyExc_TypeError,
                        "Expected all elements of sequence to be int or float");
        return -1;
      }

      PyObject *tmp = PyNumber_Long(element);

      short value = PyLong_AsLong(tmp);
      selfType->pointer[i] = value;

      Py_DECREF(tmp);
    }

    selfType->pointer[len] = 0;

    return 0;
  }

  PyErr_SetString(PyExc_TypeError, "Expected int or sequence of ints");

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
    return PyLong_FromLongLong((unsigned short)(*(selfType->pointer)));
  }
  return PyLong_FromLongLong(*(selfType->pointer));
}

// PyC.c_short.__float__
static PyObject *c_short_to_float(PyObject *self) {
  PyC_c_short *selfType = (PyC_c_short *)self;

  if (PyObject_IsInstance(self, (PyObject *)&py_c_ushort_type)) {
    return PyFloat_FromDouble((unsigned short)(*(selfType->pointer)));
  }
  return PyFloat_FromDouble(*(selfType->pointer));
}

// PyC.c_short.__int__
static PyObject *c_short_to_int(PyObject *self) {
  PyC_c_short *selfType = (PyC_c_short *)self;

  if (PyObject_IsInstance(self, (PyObject *)&py_c_ushort_type)) {
    return PyLong_FromLongLong((unsigned short)(*(selfType->pointer)));
  }
  return PyLong_FromLongLong(*(selfType->pointer));
}

// PyC.c_short.__bool__
static int c_short_to_bool(PyObject *self) {
  PyC_c_short *selfType = (PyC_c_short *)self;

  if (*selfType->pointer) {
    return 1;
  }

  return 0;
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

  PyErr_SetString(PyExc_IndexError, "Index out of range");
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

  PyErr_SetString(PyExc_IndexError, "Index out of range");
  return -1;
}

// PyC.c_short.free_on_no_reference getter
static PyObject *c_short_freeOnDel_getter(PyObject *self, void *closure) {
  PyC_c_short *selfType = (PyC_c_short *)self;

  if (selfType->freeOnDel) {
    Py_RETURN_TRUE;
  }

  Py_RETURN_FALSE;
}

// PyC.c_short.free_on_no_reference setter
static int c_short_freeOnDel_setter(PyObject *self, PyObject *value,
                                    void *closure) {
  PyC_c_short *selfType = (PyC_c_short *)self;

  if (!value) {
    PyErr_SetString(PyExc_AttributeError,
                    "Cannot delete c_short.free_on_no_reference attrubute");
    return -1;
  }

  int flag = PyObject_IsTrue(value);
  if (flag == -1) {
    return -1;
  }

  selfType->freeOnDel = flag;
  return 0;
}

// ----- c_long -----
PyNumberMethods c_long_as_long = {
    .nb_int = &c_long_to_int,
    .nb_float = &c_long_to_float,
    .nb_bool = &c_long_to_bool,
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
    {NULL, NULL, 0, NULL}};

PyMemberDef c_long_members[] = {
    {"is_pointer", T_BOOL, offsetof(PyC_c_long, isPointer), READONLY,
     "PyC.c_long.is_pointer"},
    {"is_array", T_BOOL, offsetof(PyC_c_long, isArray), READONLY,
     "PyC.c_long.is_array"},
    {NULL, 0, 0, 0, NULL}};

PyGetSetDef c_long_getsetdef[] = {
    {"free_on_no_reference", &c_long_freeOnDel_getter, &c_long_freeOnDel_setter,
     C_TYPE_FREE_ON_NO_REFERENCE_DOC, NULL},
    {NULL, NULL, NULL, NULL},
};

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
    .tp_getset = c_long_getsetdef,
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
    .tp_getset = c_long_getsetdef,
    .tp_init = &c_long_init,
    .tp_new = PyType_GenericNew,
    .tp_dealloc = &c_long_finalizer,
};

// ----- c_long: functions and methods -----

// PyC.c_long.__init__
static int c_long_init(PyObject *self, PyObject *args, PyObject *kwargs) {
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
  if (!arg_1) {
    PyErr_SetString(PyExc_TypeError, "Expected int or sequence of ints");
    return -1;
  }

  if (PyNumber_Check(arg_1)) {
    PyObject *tmp = PyNumber_Long(arg_1);
    long value = PyLong_AsLong(tmp);

    selfType->value = value;
    selfType->pointer = &(selfType->value);
    selfType->isPointer = false;
    selfType->isArray = false;
    selfType->arraySize = 0;
    selfType->arrayCapacity = 0;
    selfType->_i = 0;

    Py_DECREF(tmp);

    return 0;

  } else if (PySequence_Check(arg_1)) {
    size_t len = PySequence_Size(arg_1);

    selfType->value = 0;
    selfType->pointer = calloc(len + 1, sizeof(int));
    selfType->isPointer = true;
    selfType->isArray = true;
    selfType->arraySize = len;
    selfType->arrayCapacity = len + 1;
    selfType->_i = 0;

    for (size_t i = 0; i < len; i++) {
      PyObject *element = PySequence_GetItem(arg_1, i);

      if (!(PyNumber_Check(element))) {
        free(selfType->pointer);
        selfType->pointer = NULL;
        selfType->isPointer = false;
        selfType->freeOnDel = false;
        selfType->isArray = false;
        selfType->arraySize = 0;
        selfType->arrayCapacity = 0;
        PyErr_SetString(PyExc_TypeError,
                        "Expected all elements of sequence to be int or float");
        return -1;
      }
      PyObject *tmp = PyNumber_Long(element);

      long value = PyLong_AsLong(tmp);
      selfType->pointer[i] = value;

      Py_DECREF(tmp);
    }

    selfType->pointer[len] = 0;

    return 0;
  }

  PyErr_SetString(PyExc_TypeError, "Expected int or sequence of ints");

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
    return PyLong_FromLongLong((unsigned long)(*(selfType->pointer)));
  }
  return PyLong_FromLongLong(*(selfType->pointer));
}

// PyC.c_long.__int__
static PyObject *c_long_to_int(PyObject *self) {
  PyC_c_long *selfType = (PyC_c_long *)self;

  if (PyObject_IsInstance(self, (PyObject *)&py_c_ulong_type)) {
    return PyLong_FromLongLong((unsigned long)(*(selfType->pointer)));
  }
  return PyLong_FromLongLong(*(selfType->pointer));
}

// PyC.c_long.__float__
static PyObject *c_long_to_float(PyObject *self) {
  PyC_c_long *selfType = (PyC_c_long *)self;

  if (PyObject_IsInstance(self, (PyObject *)&py_c_ulong_type)) {
    return PyFloat_FromDouble((unsigned long)(*(selfType->pointer)));
  }
  return PyFloat_FromDouble(*(selfType->pointer));
}

// PyC.c_long.__bool__
static int c_long_to_bool(PyObject *self) {
  PyC_c_long *selfType = (PyC_c_long *)self;

  if (*selfType->pointer) {
    return 1;
  }

  return 0;
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

  PyErr_SetString(PyExc_IndexError, "Index out of range");
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

  PyErr_SetString(PyExc_IndexError, "Index out of range");
  return -1;
}

// PyC.c_long.free_on_no_reference getter
static PyObject *c_long_freeOnDel_getter(PyObject *self, void *closure) {
  PyC_c_long *selfType = (PyC_c_long *)self;

  if (selfType->freeOnDel) {
    Py_RETURN_TRUE;
  }

  Py_RETURN_FALSE;
}

// PyC.c_long.free_on_no_reference setter
static int c_long_freeOnDel_setter(PyObject *self, PyObject *value,
                                   void *closure) {
  PyC_c_long *selfType = (PyC_c_long *)self;

  if (!value) {
    PyErr_SetString(PyExc_AttributeError,
                    "Cannot delete c_long.free_on_no_reference attrubute");
    return -1;
  }

  int flag = PyObject_IsTrue(value);
  if (flag == -1) {
    return -1;
  }

  selfType->freeOnDel = flag;
  return 0;
}

// ----- c_char -----
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
    {NULL, NULL, 0, NULL}};

PyGetSetDef c_char_getsetdef[] = {
    {"free_on_no_reference", &c_char_freeOnDel_getter, &c_char_freeOnDel_setter,
     C_TYPE_FREE_ON_NO_REFERENCE_DOC, NULL},
    {NULL, NULL, NULL, NULL},
};

PyTypeObject py_c_char_type = {
    PyVarObject_HEAD_INIT(NULL, 0).tp_name = "PyCpp.c_char",
    .tp_basicsize = sizeof(PyC_c_char),
    .tp_itemsize = 0,
    .tp_as_mapping = &c_char_as_mapping,
    .tp_str = &c_char_to_str,
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_doc = "PyCpp.c_char",
    .tp_iter = &c_char_iter,
    .tp_iternext = &c_char_next,
    .tp_methods = c_char_methods,
    .tp_getset = c_char_getsetdef,
    .tp_init = &c_char_init,
    .tp_new = PyType_GenericNew,
    .tp_dealloc =
        &c_char_finalizer, // TODO: use .tp_getset for PyC_c_char's attributes
};

// ----- c_char: functions and methods -----

// PyC.c_char.__init__
static int c_char_init(PyObject *self, PyObject *args, PyObject *kwargs) {
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

    char *string = malloc(len + 1);
    strcpy(string, value);
    selfType->pointer = string;
    selfType->freeOnDel = true;
    selfType->arraySize = len;
    selfType->arrayCapacity = len + 1;
  } else if (len == 0) {
    selfType->value = 0;
    selfType->pointer = &(selfType->value);
    selfType->isPointer = false;
    selfType->freeOnDel = false;
    selfType->isArray = false;
    selfType->arraySize = 0;
    selfType->arrayCapacity = 0;
  } else {
    selfType->value = value[0];
    selfType->pointer = &(selfType->value);
    selfType->isPointer = false;
    selfType->freeOnDel = false;
    selfType->isArray = false;
    selfType->arraySize = 1;
    selfType->arrayCapacity = 0;
  }

  return 0;
}

// PyC.c_char.__iter__
static PyObject *c_char_iter(PyObject *self) {
  PyC_c_char *selfType = (PyC_c_char *)self;

  if (selfType->isArray) {
    selfType->_i = 0;
    Py_INCREF(self);
    return self;
  }

  PyErr_SetString(py_CppError,
                  "given c_int instance is not an array type instance");
  return NULL;
}

// PyC.c_char.__next__
static PyObject *c_char_next(PyObject *self) {
  PyC_c_char *selfType = (PyC_c_char *)self;
  PyObject *rvalue = NULL;

  size_t index = selfType->_i;

  if (selfType->arraySize > index) {
    rvalue = PyUnicode_FromStringAndSize(selfType->pointer + index, 1);
  }

  (selfType->_i)++;

  return rvalue;
}

// PyC.c_char.__del__
static void c_char_finalizer(PyObject *self) {
  PyC_c_char *selfType = (PyC_c_char *)self;

  if (selfType->isPointer && selfType->freeOnDel) {
    free(selfType->pointer);
  }

  Py_TYPE(self)->tp_free((PyObject *)self);
}

// PyC.c_char.append
static PyObject *c_char_append(PyObject *self, PyObject *args) {
  // TODO: implement c_char_append

  PyC_c_char *selfType = (PyC_c_char *)self;

  PyErr_SetNone(PyExc_NotImplementedError);
  return NULL;
}

// PyC.c_char.pop
static PyObject *c_char_pop(PyObject *self) {
  // TODO: implement c_char_pop

  PyC_c_char *selfType = (PyC_c_char *)self;

  PyErr_SetNone(PyExc_NotImplementedError);
  return NULL;
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
  PyC_c_char *selfType = (PyC_c_char *)self;
  return selfType->arraySize;
}

// PyC.c_char.__getitem__
static PyObject *c_char_getitem(PyObject *self, PyObject *attr) {
  // TODO: implement c_char_getitem

  PyC_c_char *selfType = (PyC_c_char *)self;

  PyErr_SetNone(PyExc_NotImplementedError);
  return NULL;
}

// PyC.c_char.__setitem__
static int c_char_setitem(PyObject *self, PyObject *attr, PyObject *value) {
  // TODO: implement c_char_setitem

  PyC_c_char *selfType = (PyC_c_char *)self;

  PyErr_SetNone(PyExc_NotImplementedError);
  return -1;
}

static PyObject *c_char_freeOnDel_getter(PyObject *self, void *closure) {
  PyC_c_char *selfType = (PyC_c_char *)self;

  if (selfType->freeOnDel) {
    Py_RETURN_TRUE;
  }

  Py_RETURN_FALSE;
}

static int c_char_freeOnDel_setter(PyObject *self, PyObject *value,
                                   void *closure) {
  PyC_c_char *selfType = (PyC_c_char *)self;

  if (!value) {
    PyErr_SetString(PyExc_AttributeError,
                    "Cannot delete c_int.free_on_no_reference attrubute");
    return -1;
  }

  int flag = PyObject_IsTrue(value);
  if (flag == -1) {
    return -1;
  }

  selfType->freeOnDel = flag;
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
    // {"__python_representation", T_OBJECT, offsetof(PyC_c_struct,
    // pyDictRepr),
    //  READONLY, "PyC.c_struct.__python_representation"},
    {NULL, 0, 0, 0, NULL}};

PyMethodDef c_struct_methods[] = {
    {"append", (PyCFunction)&c_struct_append, METH_VARARGS,
     "c_struct.append()"},
    {"pop", (PyCFunction)&c_struct_pop, METH_NOARGS, "c_struct.pop()"},
    {NULL, NULL, 0, NULL}};

PyGetSetDef c_struct_getsetdef[] = {
    {"free_on_no_reference", &c_struct_freeOnDel_getter,
     &c_struct_freeOnDel_setter, C_TYPE_FREE_ON_NO_REFERENCE_DOC, NULL},
    {NULL, NULL, NULL, NULL},
};

PyTypeObject py_c_struct_type = {
    PyVarObject_HEAD_INIT(NULL, 0).tp_name = "PyCpp.c_struct",
    .tp_basicsize = sizeof(PyC_c_struct),
    .tp_itemsize = 0,
    .tp_getattr = &c_struct_getattr,
    .tp_setattr = &c_struct_setattr,
    .tp_as_mapping = &c_struct_as_mapping,
    .tp_flags = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE | Py_TPFLAGS_HAVE_GC,
    .tp_doc = "PyCpp.c_struct",
    .tp_iter = &c_struct_iter,
    .tp_iternext = &c_struct_next,
    .tp_methods = c_struct_methods,
    .tp_members = c_struct_members,
    .tp_getset = c_struct_getsetdef,
    .tp_init = &c_struct_init,
    .tp_new = PyType_GenericNew,
    .tp_dealloc = &c_struct_finalizer,
    .tp_traverse = &c_struct_Traverse,
    .tp_clear = &c_struct_Clear,
};

// c_struct methods

// PyC.c_struct.__init__
static int c_struct_init(PyObject *self, PyObject *args, PyObject *kwargs) {
  PyC_c_struct *selfType = (PyC_c_struct *)self;
  Structure *s = ((struct Custom_s_PyTypeObject *)self->ob_type)->s;

  selfType->structure = s;
  selfType->freeOnDel = true;
  selfType->parentModule =
      ((struct Custom_s_PyTypeObject *)self->ob_type)->parentModule;
  Py_INCREF(selfType->parentModule);

  selfType->isArray = false;
  selfType->arraySize = 0;
  selfType->arrayCapacity = 0;
  selfType->pointer = malloc(s->structSize);

  selfType->child_ptrs = PyDict_New();

  size_t args_len = PyTuple_Size(args);
  if (args_len == 0) {
    return 0;
  }

  if ((args_len == 1) && PySequence_Check(PyTuple_GetItem(args, 0))) {
    PyObject *arr = PyTuple_GetItem(args, 0);
    size_t arr_len = PySequence_Size(arr);

    free(selfType->pointer);
    selfType->pointer = calloc(arr_len, s->structSize);
    selfType->isArray = true;
    selfType->arraySize = args_len + 1;
    selfType->arrayCapacity = args_len + 1;

    for (size_t i = 0; i < arr_len; i++) {
      PyObject *element = PySequence_GetItem(arr, i);

      if (Py_TYPE(element) != Py_TYPE(element)) {
        Py_DECREF(selfType->child_ptrs);
        Py_DECREF(selfType->parentModule);
        free(selfType->pointer);
        PyErr_SetString(py_BindingError,
                        "Expected all elements of the sequence to be of the "
                        "same struct type");
        return -1;
      }

      PyC_c_struct *elementType = (PyC_c_struct *)element;
      memcpy(selfType->pointer + (s->structSize * i), elementType->pointer,
             s->structSize);
    }

    return 0;
  }

  // initilising struct with attributes
  if (args_len != s->attrCount) {
    Py_DECREF(selfType->child_ptrs);
    Py_DECREF(selfType->parentModule);
    free(selfType->pointer);
    PyErr_SetString(py_BindingError,
                    "Args count does not match struct's attribute count. "
                    "Cannot initilise the struct.");
    return -1;
  }

  for (size_t i = 0; i < selfType->structure->attrCount; i++) {
    ffi_type *type = p_ffi_type_array_getat(selfType->structure->attrTypes, i);
    PyObject *attr = PyTuple_GetItem(args, i);

    bool should_free = false;
    void *data = pyArg_to_cppArg(attr, *type, &should_free);
    if (!data) {
      return -1;
    }

    if (type->type == FFI_TYPE_POINTER) {
      memcpy((char *)(selfType->pointer) +
                 (long_long_array_getat(selfType->structure->offsets, i) / 8),
             data, type->size);

      PyDict_SetItemString(selfType->child_ptrs,
                           str_array_getat(s->attrNames, i), attr);
    } else if (PyObject_IsInstance(attr, (PyObject *)&py_c_struct_type)) {
      memcpy((char *)(selfType->pointer) +
                 (long_long_array_getat(selfType->structure->offsets, i) / 8),
             data, ((PyC_c_struct *)attr)->structure->structSize);
    } else {
      memcpy((char *)(selfType->pointer) +
                 (long_long_array_getat(selfType->structure->offsets, i) / 8),
             data, type->size);
    }

    if (should_free) {
      free(data);
    }
  }

  return 0;
}

// PyC.c_struct.__getattr__
static PyObject *c_struct_getattr(PyObject *self, char *attr) {
  PyC_c_struct *selfType = (PyC_c_struct *)self;

  PyObject *value = PyObject_GenericGetAttr(self, PyUnicode_FromString(attr));
  if (value)
    return value;
  else
    PyErr_Clear();

  for (size_t i = 0; i < selfType->structure->attrCount; i++) {
    if (!(strcmp(attr, str_array_getat(selfType->structure->attrNames, i)))) {
      char *data = (char *)selfType->pointer;

      ffi_type *type =
          p_ffi_type_array_getat(selfType->structure->attrTypes, i);

      if (type->type == FFI_TYPE_POINTER) {

        PyObject *result = PyDict_GetItemString(selfType->child_ptrs, attr);
        if (result) {
          Py_INCREF(result);
          return result;
        } else {
          return PyErr_Format(
              py_BindingError,
              "Attribute %s is not set. Cannot access attribute %s", attr,
              attr);
        }
      }

      return cppArg_to_pyArg(
          data + (long_long_array_getat(selfType->structure->offsets, i) / 8),
          *p_ffi_type_array_getat(selfType->structure->attrTypes, i),
          CXTypeKind_array_getat(selfType->structure->attrUnderlyingType, i),
          p_Structure_array_getat(selfType->structure->attrUnderlyingStructs,
                                  i),
          p_Union_array_getat(selfType->structure->attrUnderlyingUnions, i),
          selfType->parentModule);
    }
  }

  PyErr_SetString(PyExc_AttributeError, "No attribute found with given name");
  return NULL;
}

// PyC.c_struct.__setattr__
static int c_struct_setattr(PyObject *self, char *attr, PyObject *pValue) {
  PyC_c_struct *selfType = (PyC_c_struct *)self;

  if (strcmp(attr, "free_on_no_reference") == 0) {
    PyObject *key = PyUnicode_FromString("free_on_no_reference");
    int result = PyObject_GenericSetAttr(self, key, pValue);
    Py_DECREF(key);
    return result;
  }

  for (size_t i = 0; i < selfType->structure->attrCount; i++) {
    if (!(strcmp(attr, str_array_getat(selfType->structure->attrNames, i)))) {
      ffi_type *type =
          p_ffi_type_array_getat(selfType->structure->attrTypes, i);

      bool should_free = false;
      void *data = pyArg_to_cppArg(pValue, *type, &should_free);
      if (!data) {
        return -1;
      }

      if (type->type == FFI_TYPE_POINTER) {
        memcpy((char *)(selfType->pointer) +
                   (long_long_array_getat(selfType->structure->offsets, i) / 8),
               data, type->size);

        PyDict_SetItemString(selfType->child_ptrs, attr, pValue);
      } else if (PyObject_IsInstance(pValue, (PyObject *)&py_c_struct_type)) {
        memcpy((char *)(selfType->pointer) +
                   (long_long_array_getat(selfType->structure->offsets, i) / 8),
               data, ((PyC_c_struct *)pValue)->structure->structSize);
      } else {
        memcpy((char *)(selfType->pointer) +
                   (long_long_array_getat(selfType->structure->offsets, i) / 8),
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
  PyC_c_struct *selfType = (PyC_c_struct *)self;

  PyObject_GC_UnTrack(self);
  c_struct_Clear(self);

  if (selfType->pointer && selfType->freeOnDel) {
    free(selfType->pointer);
  }

  Py_TYPE(self)->tp_free((PyObject *)self);
}

static int c_struct_Traverse(PyObject *self, visitproc visit, void *arg) {
  PyC_c_struct *selfType = (PyC_c_struct *)self;

  Py_VISIT(selfType->parentModule);
  Py_VISIT(selfType->child_ptrs);
  return 0;
}

static int c_struct_Clear(PyObject *self) {
  PyC_c_struct *selfType = (PyC_c_struct *)self;

  Py_CLEAR(selfType->parentModule);
  Py_CLEAR(selfType->child_ptrs);
  return 0;
}

// helper function
PyObject *create_py_c_struct(Structure *structure, PyObject *module) {
  struct Custom_s_PyTypeObject *py_c_new_type =
      malloc(sizeof(struct Custom_s_PyTypeObject)); // TODO: free this malloc

  if (py_c_new_type) {
    p_void_array_append(EXTRA_HEAP_MEMORY, py_c_new_type);
  } else {
    return PyErr_NoMemory();
  }

  char *struct_name =
      malloc(7 + strlen(structure->name)); // TODO: free this malloc
  strcpy(struct_name, "PyCpp.");
  strcat(struct_name, structure->name);

  if (struct_name) {
    p_void_array_append(EXTRA_HEAP_MEMORY, struct_name);
  } else {
    return PyErr_NoMemory();
  }

  *py_c_new_type = (struct Custom_s_PyTypeObject){
      ._ =
          (PyTypeObject){
              PyVarObject_HEAD_INIT(NULL, 0).tp_name = struct_name,
              .tp_basicsize = sizeof(PyC_c_struct),
              .tp_itemsize = 0,
              .tp_flags = Py_TPFLAGS_DEFAULT,
              .tp_doc = struct_name,
              .tp_new = PyType_GenericNew,
              .tp_base = &py_c_struct_type,
          },
      .s = structure,
      .parentModule = module,
  };

  if (PyType_Ready((PyTypeObject *)py_c_new_type) < 0) {
    return NULL;
  }

  Py_INCREF(py_c_new_type);
  if (PyModule_AddObject(PyC, structure->name, (PyObject *)py_c_new_type) < 0) {
    Py_DECREF(py_c_new_type);
    return NULL;
  }

  PyObject *obj = PyObject_GetAttrString(PyC, structure->name);
  if (obj) {
    return obj;
  }

  PyErr_SetString(py_BindingError,
                  "Unable to access PyCpp.c_struct base class");
  return NULL;
}

// PyC.c_struct.__iter__
static PyObject *c_struct_iter(PyObject *self) {
  PyC_c_struct *selfType = (PyC_c_struct *)self;

  if (selfType->isArray) {
    selfType->_i = 0;
    Py_INCREF(self);
    return self;
  }

  PyErr_SetString(py_CppError,
                  "given c_struct instance is not an array type instance");
  return NULL;
}

// PyC.c_struct.__next__
static PyObject *c_struct_next(PyObject *self) {
  PyC_c_struct *selfType = (PyC_c_struct *)self;
  PyObject *rvalue = NULL;

  size_t index = selfType->_i;
  void *data = selfType->pointer + (selfType->structure->structSize * index);

  if (selfType->arraySize > index) {
    // TODO: cache the result
    rvalue = cppArg_to_pyArg(&data, ffi_type_pointer, CXType_Record,
                             selfType->structure, NULL, selfType->parentModule);
  }

  (selfType->_i)++;

  return rvalue;
}

// PyC.c_struct.append
static PyObject *c_struct_append(PyObject *self, PyObject *args) {
  PyC_c_struct *selfType = (PyC_c_struct *)self;

  if (!(selfType->isArray)) {
    PyErr_SetString(py_CppError,
                    "given instance of c_struct is not an array type instance");
    return NULL;
  }

  PyObject *value;
  if (!PyArg_ParseTuple(args, "O", &value)) {
    return NULL;
  }

  if (Py_TYPE(value) != Py_TYPE(self)) {
    PyErr_SetString(py_BindingError,
                    "Expected arg to be of the same struct type");
    return NULL;
  }

  PyC_c_struct *valueType = (PyC_c_struct *)value;

  if (selfType->arrayCapacity > selfType->arraySize) {
    memcpy(selfType->pointer +
               (selfType->structure->structSize * selfType->arraySize),
           valueType->pointer, selfType->structure->structSize);
    (selfType->arraySize)++;
  } else {
    int new_capacity =
        (selfType->arrayCapacity * 2) * selfType->structure->structSize;
    selfType->pointer = realloc(selfType->pointer, new_capacity);
    selfType->arrayCapacity *= 2;

    memcpy(selfType->pointer +
               (selfType->structure->structSize * selfType->arraySize),
           valueType->pointer, selfType->structure->structSize);
    (selfType->arraySize)++;
  }

  Py_INCREF(value);
  return value;
}

// PyC.c_struct.pop
static PyObject *c_struct_pop(PyObject *self) {
  PyC_c_struct *selfType = (PyC_c_struct *)self;

  if (!(selfType->isArray)) {
    PyErr_SetString(py_CppError,
                    "given instance of c_struct is not an array type instance");
    return NULL;
  }

  if (!(selfType->arraySize)) {
    PyErr_SetString(py_CppError, "no elements in the array to pop");
    return NULL;
  }

  void *data = selfType->pointer +
               (selfType->structure->structSize * (selfType->arraySize - 1));

  PyObject *rvalue =
      cppArg_to_pyArg(&data, ffi_type_pointer, CXType_Record,
                      selfType->structure, NULL, selfType->parentModule);

  (selfType->arraySize)--;

  if ((selfType->arraySize * 2) < selfType->arrayCapacity) {
    selfType->pointer =
        realloc(selfType->pointer,
                (selfType->arraySize) * selfType->structure->structSize);
    selfType->arrayCapacity = selfType->arraySize;
  }

  return rvalue;
}

// PyC.c_struct.__len__
static Py_ssize_t c_struct_len(PyObject *self) {
  PyC_c_struct *selfType = (PyC_c_struct *)self;
  return selfType->arraySize;
}

// PyC.c_struct.__getitem__
static PyObject *c_struct_getitem(PyObject *self, PyObject *attr) {
  PyC_c_struct *selfType = (PyC_c_struct *)self;

  if (!(selfType->isArray)) {
    PyErr_SetString(py_CppError,
                    "given instance of c_struct is not an array type instance");
    return NULL;
  }

  if (!(PyNumber_Check(attr))) {
    PyErr_SetString(PyExc_TypeError,
                    "Expected interger type got some other type");
    return NULL;
  }

  long index = PyLong_AsLong(attr);

  void *data = selfType->pointer + (selfType->structure->structSize * index);

  if (selfType->arraySize > index) {
    return cppArg_to_pyArg(&data, ffi_type_pointer, CXType_Record,
                           selfType->structure, NULL, selfType->parentModule);
  }

  PyErr_SetString(PyExc_IndexError, "Index out of range");
  return NULL;
}

// PyC.c_struct.__setitem__
static int c_struct_setitem(PyObject *self, PyObject *attr, PyObject *value) {
  PyC_c_struct *selfType = (PyC_c_struct *)self;

  if (!(selfType->isArray)) {
    PyErr_SetString(py_CppError,
                    "given instance of c_struct is not an array type instance");
    return -1;
  }

  if (!(PyNumber_Check(attr))) {
    PyErr_SetString(PyExc_TypeError,
                    "Expected interger type got some other type");
    return -1;
  }

  if (Py_TYPE(value) != Py_TYPE(self)) {
    PyErr_SetString(py_BindingError,
                    "Expected arg to be of the same struct type");
    return -1;
  }

  long index = PyLong_AsLong(attr);

  if (selfType->arraySize > index) {
    PyC_c_struct *valueType = (PyC_c_struct *)value;

    memcpy(selfType->pointer + (selfType->structure->structSize * index),
           valueType->pointer, selfType->structure->structSize);

    return 0;
  }

  PyErr_SetString(PyExc_IndexError, "Index out of range");
  return -1;
}

// PyC.c_struct.free_on_no_reference getter
static PyObject *c_struct_freeOnDel_getter(PyObject *self, void *closure) {
  PyC_c_struct *selfType = (PyC_c_struct *)self;

  if (selfType->freeOnDel) {
    Py_RETURN_TRUE;
  }

  Py_RETURN_FALSE;
}

// PyC.c_struct.free_on_no_reference setter
static int c_struct_freeOnDel_setter(PyObject *self, PyObject *value,
                                     void *closure) {
  PyC_c_struct *selfType = (PyC_c_struct *)self;

  if (!value) {
    PyErr_SetString(PyExc_AttributeError,
                    "Cannot delete c_struct.free_on_no_reference attrubute");
    return -1;
  }

  int flag = PyObject_IsTrue(value);
  if (flag == -1) {
    return -1;
  }

  selfType->freeOnDel = flag;
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
    // {"__python_representation", T_OBJECT, offsetof(PyC_c_union,
    // pyDictRepr),
    //  READONLY, "PyC.c_union.__python_representation"},
    {NULL, 0, 0, 0, NULL}};

PyMethodDef c_union_methods[] = {
    {"append", (PyCFunction)&c_union_append, METH_VARARGS, "c_union.append()"},
    {"pop", (PyCFunction)&c_union_pop, METH_NOARGS, "c_union.pop()"},
    {NULL, NULL, 0, NULL}};

PyGetSetDef c_union_getsetdef[] = {
    {"free_on_no_reference", &c_union_freeOnDel_getter,
     &c_union_freeOnDel_setter, C_TYPE_FREE_ON_NO_REFERENCE_DOC, NULL},
    {NULL, NULL, NULL, NULL},
};

PyTypeObject py_c_union_type = {
    PyVarObject_HEAD_INIT(NULL, 0).tp_name = "PyCpp.c_union",
    .tp_basicsize = sizeof(PyC_c_union),
    .tp_itemsize = 0,
    .tp_getattr = &c_union_getattr,
    .tp_setattr = &c_union_setattr,
    .tp_as_mapping = &c_union_as_mapping,
    .tp_flags = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,
    .tp_doc = "PyCpp.c_union",
    .tp_iter = &c_union_iter,
    .tp_iternext = &c_union_next,
    .tp_methods = c_union_methods,
    .tp_members = c_union_members,
    .tp_getset = c_union_getsetdef,
    .tp_init = &c_union_init,
    .tp_new = PyType_GenericNew,
    .tp_dealloc = &c_union_finalizer,
};

// c_union methods

// PyC.c_union.__init__
static int c_union_init(PyObject *self, PyObject *args, PyObject *kwargs) {
  PyC_c_union *selfType = (PyC_c_union *)self;
  Union *s = ((struct Custom_u_PyTypeObject *)self->ob_type)->u;

  selfType->u = s;
  selfType->freeOnDel = true;
  selfType->parentModule =
      ((struct Custom_s_PyTypeObject *)self->ob_type)->parentModule;
  Py_INCREF(selfType->parentModule);

  selfType->pointer = malloc(s->unionSize);

  size_t args_len = PyTuple_Size(args);
  if (args_len == 0) {
    return 0;
  }

  if ((args_len == 1) && PySequence_Check(PyTuple_GetItem(args, 0))) {
    PyObject *arr = PyTuple_GetItem(args, 0);
    size_t arr_len = PySequence_Size(arr);

    free(selfType->pointer);
    selfType->pointer = calloc(arr_len, s->unionSize);
    selfType->isArray = true;
    selfType->arraySize = args_len + 1;
    selfType->arrayCapacity = args_len + 1;

    for (size_t i = 0; i < arr_len; i++) {
      PyObject *element = PySequence_GetItem(arr, i);

      if (Py_TYPE(element) != Py_TYPE(element)) {
        Py_DECREF(selfType->parentModule);
        free(selfType->pointer);
        PyErr_SetString(py_BindingError,
                        "Expected all elements of the sequence to be of the "
                        "same union type");
        return -1;
      }

      PyC_c_struct *elementType = (PyC_c_struct *)element;
      memcpy(selfType->pointer + (s->unionSize * i), elementType->pointer,
             s->unionSize);
    }

    return 0;
  }

  free(selfType->pointer);
  Py_DECREF(selfType->parentModule);
  PyErr_SetString(
      PyExc_TypeError,
      "Expected no arguments or sequence of c_union, but got something else");

  return -1;
}

// PyC.c_union.__getattr__
static PyObject *c_union_getattr(PyObject *self, char *attr) {
  PyC_c_union *selfType = (PyC_c_union *)self;

  PyObject *value = PyObject_GenericGetAttr(self, PyUnicode_FromString(attr));
  if (value)
    return value;
  else
    PyErr_Clear();

  for (size_t i = 0; i < selfType->u->attrCount; i++) {
    if (!(strcmp(attr, str_array_getat(selfType->u->attrNames, i)))) {
      char *data = (char *)selfType->pointer;

      return cppArg_to_pyArg(
          (selfType->pointer),
          *p_ffi_type_array_getat(selfType->u->attrTypes, i),
          CXTypeKind_array_getat(selfType->u->attrUnderlyingType, i),
          p_Structure_array_getat(selfType->u->attrUnderlyingStructs, i),
          p_Union_array_getat(selfType->u->attrUnderlyingUnions, i),
          selfType->parentModule);
    }
  }

  PyErr_SetString(PyExc_AttributeError, "No attribute found with given name");
  return NULL;
}

// PyC.c_union.__setattr__
static int c_union_setattr(PyObject *self, char *attr, PyObject *pValue) {
  PyC_c_union *selfType = (PyC_c_union *)self;

  if (strcmp(attr, "free_on_no_reference") == 0) {
    PyObject *key = PyUnicode_FromString("free_on_no_reference");
    int result = PyObject_GenericSetAttr(self, key, pValue);
    Py_DECREF(key);
    return result;
  }

  for (size_t i = 0; i < selfType->u->attrCount; i++) {
    if (!(strcmp(attr, str_array_getat(selfType->u->attrNames, i)))) {
      ffi_type *type = p_ffi_type_array_getat(selfType->u->attrTypes, i);

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
  PyC_c_union *selfType = (PyC_c_union *)self;

  if (selfType->pointer && selfType->freeOnDel) {
    free(selfType->pointer);
  }
  Py_DECREF(selfType->parentModule);
  Py_TYPE(self)->tp_free((PyObject *)self);
}

// helper function
PyObject *create_py_c_union(Union *u, PyObject *module) {
  struct Custom_u_PyTypeObject *py_c_new_type =
      malloc(sizeof(struct Custom_u_PyTypeObject)); // TODO: free this malloc

  if (py_c_new_type) {
    p_void_array_append(EXTRA_HEAP_MEMORY, py_c_new_type);
  } else {
    return PyErr_NoMemory();
  }

  char *struct_name = malloc(7 + strlen(u->name)); // TODO: free this malloc
  strcpy(struct_name, "PyCpp.");
  strcat(struct_name, u->name);

  if (struct_name) {
    p_void_array_append(EXTRA_HEAP_MEMORY, struct_name);
  } else {
    return PyErr_NoMemory();
  }

  *py_c_new_type = (struct Custom_u_PyTypeObject){
      ._ =
          (PyTypeObject){
              PyVarObject_HEAD_INIT(NULL, 0).tp_name = struct_name,
              .tp_basicsize = sizeof(PyC_c_union),
              .tp_itemsize = 0,
              .tp_flags = Py_TPFLAGS_DEFAULT,
              .tp_doc = struct_name,
              .tp_new = PyType_GenericNew,
              .tp_base = &py_c_union_type,
          },
      .u = u,
      .parentModule = module,
  };
  // Py_INCREF(module);

  if (PyType_Ready((PyTypeObject *)py_c_new_type) < 0) {
    return NULL;
  }

  Py_INCREF(py_c_new_type);
  if (PyModule_AddObject(PyC, u->name, (PyObject *)py_c_new_type) < 0) {
    Py_DECREF(py_c_new_type);
    return NULL;
  }

  PyObject *obj = PyObject_GetAttrString(PyC, u->name);
  if (obj) {
    return obj;
  }

  PyErr_SetString(py_BindingError, "Unable to access PyCpp.c_union base class");
  return NULL;
}

// PyC.c_union.__iter__
static PyObject *c_union_iter(PyObject *self) {
  PyC_c_union *selfType = (PyC_c_union *)self;

  if (selfType->isArray) {
    selfType->_i = 0;
    Py_INCREF(self);
    return self;
  }

  PyErr_SetString(py_CppError,
                  "given c_union instance is not an array type instance");
  return NULL;
}

// PyC.c_union.__next__
static PyObject *c_union_next(PyObject *self) {
  PyC_c_union *selfType = (PyC_c_union *)self;

  PyObject *rvalue = NULL;

  size_t index = selfType->_i;

  void *data = selfType->pointer + (selfType->u->unionSize * index);

  if (selfType->arraySize > index) {
    // TODO: cache the result
    rvalue = cppArg_to_pyArg(&data, ffi_type_pointer, CXType_Record, NULL,
                             selfType->u, selfType->parentModule);
  }

  (selfType->_i)++;

  return rvalue;
}

// PyC.c_union.append
static PyObject *c_union_append(PyObject *self, PyObject *args) {
  PyC_c_union *selfType = (PyC_c_union *)self;

  if (!(selfType->isArray)) {
    PyErr_SetString(py_CppError,
                    "given instance of c_union is not an array type instance");
    return NULL;
  }

  PyObject *value;
  if (!PyArg_ParseTuple(args, "O", &value)) {
    return NULL;
  }

  if (Py_TYPE(value) != Py_TYPE(self)) {
    PyErr_SetString(py_BindingError,
                    "Expected arg to be of the same union type");
    return NULL;
  }

  PyC_c_union *valueType = (PyC_c_union *)value;

  if (selfType->arrayCapacity > selfType->arraySize) {
    memcpy(selfType->pointer + (selfType->u->unionSize * selfType->arraySize),
           valueType->pointer, selfType->u->unionSize);
    (selfType->arraySize)++;
  } else {
    int new_capacity = (selfType->arrayCapacity * 2) * selfType->u->unionSize;
    selfType->pointer = realloc(selfType->pointer, new_capacity);
    selfType->arrayCapacity *= 2;

    memcpy(selfType->pointer + (selfType->u->unionSize * selfType->arraySize),
           valueType->pointer, selfType->u->unionSize);
    (selfType->arraySize)++;
  }

  Py_INCREF(value);
  return value;
}

// PyC.c_union.pop
static PyObject *c_union_pop(PyObject *self) {
  PyC_c_union *selfType = (PyC_c_union *)self;

  if (!(selfType->isArray)) {
    PyErr_SetString(py_CppError,
                    "given instance of c_union is not an array type instance");
    return NULL;
  }

  if (!(selfType->arraySize)) {
    PyErr_SetString(py_CppError, "no elements in the array to pop");
    return NULL;
  }

  void *data =
      selfType->pointer + (selfType->u->unionSize * (selfType->arraySize - 1));

  PyObject *rvalue = cppArg_to_pyArg(&data, ffi_type_pointer, CXType_Record,
                                     NULL, selfType->u, selfType->parentModule);

  (selfType->arraySize)--;

  if ((selfType->arraySize * 2) < selfType->arrayCapacity) {
    selfType->pointer = realloc(selfType->pointer,
                                (selfType->arraySize) * selfType->u->unionSize);
    selfType->arrayCapacity = selfType->arraySize;
  }

  return rvalue;
}

// PyC.c_union.__len__
static Py_ssize_t c_union_len(PyObject *self) {
  PyC_c_union *selfType = (PyC_c_union *)self;
  return selfType->arraySize;
}

// PyC.c_union.__getitem__
static PyObject *c_union_getitem(PyObject *self, PyObject *attr) {
  PyC_c_union *selfType = (PyC_c_union *)self;

  if (!(selfType->isArray)) {
    PyErr_SetString(py_CppError,
                    "given instance of c_union is not an array type instance");
    return NULL;
  }

  if (!(PyNumber_Check(attr))) {
    PyErr_SetString(PyExc_TypeError,
                    "Expected interger type got some other type");
    return NULL;
  }

  long index = PyLong_AsLong(attr);

  void *data = selfType->pointer + (selfType->u->unionSize * index);

  if (selfType->arraySize > index) {
    return cppArg_to_pyArg(&data, ffi_type_pointer, CXType_Record, NULL,
                           selfType->u, selfType->parentModule);
  }

  PyErr_SetString(PyExc_IndexError, "Index out of range");
  return NULL;
}

// PyC.c_union.__setitem__
static int c_union_setitem(PyObject *self, PyObject *attr, PyObject *value) {
  PyC_c_union *selfType = (PyC_c_union *)self;

  if (!(selfType->isArray)) {
    PyErr_SetString(py_CppError,
                    "given instance of c_union is not an array type instance");
    return -1;
  }

  if (!(PyNumber_Check(attr))) {
    PyErr_SetString(PyExc_TypeError,
                    "Expected interger type got some other type");
    return -1;
  }

  if (Py_TYPE(value) != Py_TYPE(self)) {
    PyErr_SetString(py_BindingError,
                    "Expected arg to be of the same struct type");
    return -1;
  }

  long index = PyLong_AsLong(attr);

  if (selfType->arraySize > index) {
    PyC_c_struct *valueType = (PyC_c_struct *)value;

    memcpy(selfType->pointer + (selfType->u->unionSize * index),
           valueType->pointer, selfType->u->unionSize);

    return 0;
  }

  PyErr_SetString(PyExc_IndexError, "Index out of range");
  return -1;
}

static PyObject *c_union_freeOnDel_getter(PyObject *self, void *closure) {
  PyC_c_union *selfType = (PyC_c_union *)self;

  if (selfType->freeOnDel) {
    Py_RETURN_TRUE;
  }

  Py_RETURN_FALSE;
}

static int c_union_freeOnDel_setter(PyObject *self, PyObject *value,
                                    void *closure) {
  PyC_c_union *selfType = (PyC_c_union *)self;

  if (!value) {
    PyErr_SetString(PyExc_AttributeError,
                    "Cannot delete c_int.free_on_no_reference attrubute");
    return -1;
  }

  int flag = PyObject_IsTrue(value);
  if (flag == -1) {
    return -1;
  }

  selfType->freeOnDel = flag;
  return 0;
}
