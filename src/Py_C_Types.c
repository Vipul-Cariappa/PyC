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

#define NEW_PY_CTYPE_DEL(name,                                                 \
                         type,                                                 \
                         base,                                                 \
                         type_check_func,                                      \
                         type_convert_pyobj_to_ctype,                          \
                         type_convert_ctype_to_pyobj,                          \
                         type_convert_ctype_to_pyint)                          \
  PyNumberMethods name##_as_number = {                                         \
      .nb_bool  = name##_to_bool,                                              \
      .nb_int   = name##_to_int,                                               \
      .nb_float = name##_to_float,                                             \
  };                                                                           \
                                                                               \
  PySequenceMethods name##_as_sequence = {                                     \
      .sq_length   = name##_len,                                               \
      .sq_item     = name##_getitem,                                           \
      .sq_ass_item = name##_setitem,                                           \
  };                                                                           \
                                                                               \
  PyMethodDef name##_methods[] = {                                             \
      {"value", (PyCFunction)name##_value, METH_NOARGS, #name ".value()"},     \
      {"append", (PyCFunction)name##_append, METH_O, #name ".append()"},       \
      {"pop", (PyCFunction)name##_pop, METH_NOARGS, #name ".pop()"},           \
      {NULL, NULL, 0, NULL},                                                   \
  };                                                                           \
                                                                               \
  PyMemberDef name##_members[] = {                                             \
      {"is_pointer",                                                           \
       T_BOOL,                                                                 \
       offsetof(PyC_##name, isPointer),                                        \
       READONLY,                                                               \
       "PyC." #name ".is_pointer"},                                            \
      {"is_array",                                                             \
       T_BOOL,                                                                 \
       offsetof(PyC_##name, isArray),                                          \
       READONLY,                                                               \
       "PyC." #name ".is_array"},                                              \
      {NULL, 0, 0, 0, NULL},                                                   \
  };                                                                           \
  PyGetSetDef name##_getsetdef[] = {                                           \
      {"free_on_no_reference",                                                 \
       name##_freeOnDel_getter,                                                \
       name##_freeOnDel_setter,                                                \
       #name ".free_on_no_reference",                                          \
       NULL},                                                                  \
      {NULL, NULL, NULL, NULL},                                                \
  };                                                                           \
                                                                               \
  PyTypeObject py_##name##_type = {                                            \
      PyVarObject_HEAD_INIT(NULL, 0).tp_name = "PyC." #name,                   \
      .tp_basicsize                          = sizeof(PyC_##name),             \
      .tp_itemsize                           = 0,                              \
      .tp_as_number                          = &(name##_as_number),            \
      .tp_as_sequence                        = &(name##_as_sequence),          \
      .tp_flags                              = Py_TPFLAGS_DEFAULT,             \
      .tp_doc                                = "PyC." #name,                   \
      .tp_iter                               = name##_iter,                    \
      .tp_iternext                           = name##_next,                    \
      .tp_methods                            = name##_methods,                 \
      .tp_members                            = name##_members,                 \
      .tp_getset                             = name##_getsetdef,               \
      .tp_init                               = name##_init,                    \
      .tp_new                                = PyType_GenericNew,              \
      .tp_dealloc                            = name##_finalizer,               \
      .tp_base                               = base,                           \
  };                                                                           \
                                                                               \
  /* __init__ */                                                               \
  int name##_init(PyObject *self, PyObject *args, PyObject *kwargs) {          \
    PyC_##name *selfType = (PyC_##name *)self;                                 \
                                                                               \
    PyObject *arg_1 = PyTuple_GetItem(args, 0);                                \
    if (!arg_1) {                                                              \
      PyErr_SetString(PyExc_TypeError,                                         \
                      "Expected " #type " or sequence of " #name);             \
      return -1;                                                               \
    }                                                                          \
                                                                               \
    if (type_check_func(arg_1)) {                                              \
      type value = type_convert_pyobj_to_ctype(arg_1);                         \
                                                                               \
      selfType->value         = value;                                         \
      selfType->freeOnDel     = false;                                         \
      selfType->pointer       = &(selfType->value);                            \
      selfType->isPointer     = false;                                         \
      selfType->isArray       = false;                                         \
      selfType->arraySize     = 0;                                             \
      selfType->arrayCapacity = 0;                                             \
      selfType->_i            = 0;                                             \
                                                                               \
      return 0;                                                                \
    }                                                                          \
                                                                               \
    if (PySequence_Check(arg_1)) {                                             \
      PyErr_Clear();                                                           \
      size_t len = PySequence_Size(arg_1);                                     \
                                                                               \
      selfType->value         = 0;                                             \
      selfType->freeOnDel     = true;                                          \
      selfType->pointer       = calloc(len, sizeof(type));                     \
      selfType->isPointer     = true;                                          \
      selfType->isArray       = true;                                          \
      selfType->arraySize     = len;                                           \
      selfType->arrayCapacity = len;                                           \
      selfType->_i            = 0;                                             \
                                                                               \
      for (size_t i = 0; i < len; i++) {                                       \
        PyObject *element = PySequence_GetItem(arg_1, i);                      \
                                                                               \
        if (type_check_func(element)) {                                        \
          selfType->pointer[i] = type_convert_pyobj_to_ctype(element);         \
        } else {                                                               \
          PyErr_SetString(                                                     \
              PyExc_TypeError,                                                 \
              "Expected all elements of the sequence to be of type " #name);   \
          return -1;                                                           \
        }                                                                      \
      }                                                                        \
      return 0;                                                                \
    }                                                                          \
                                                                               \
    PyErr_SetString(PyExc_TypeError,                                           \
                    "Expected " #type " or sequence of " #name);               \
    return -1;                                                                 \
  }                                                                            \
                                                                               \
  /* __del__ */                                                                \
  void name##_finalizer(PyObject *self) {                                      \
    PyC_##name *selfType = (PyC_##name *)self;                                 \
                                                                               \
    if (selfType->freeOnDel && selfType->isPointer)                            \
      free(selfType->pointer);                                                 \
                                                                               \
    Py_TYPE(self)->tp_free((PyObject *)self);                                  \
  }                                                                            \
                                                                               \
  /* __iter__ */                                                               \
  PyObject *name##_iter(PyObject *self) {                                      \
    PyC_##name *selfType = (PyC_##name *)self;                                 \
                                                                               \
    if (selfType->isArray) {                                                   \
      selfType->_i = 0;                                                        \
      Py_INCREF(self);                                                         \
      return self;                                                             \
    }                                                                          \
                                                                               \
    PyErr_SetString(py_CppError,                                               \
                    "given " #name " instance is not an array type instance"); \
    return NULL;                                                               \
  }                                                                            \
                                                                               \
  /* __next__ */                                                               \
  PyObject *name##_next(PyObject *self) {                                      \
    PyC_##name *selfType = (PyC_##name *)self;                                 \
    if (selfType->_i < selfType->arraySize)                                    \
      return type_convert_ctype_to_pyobj(selfType->pointer[(selfType->_i)++]); \
                                                                               \
    return NULL;                                                               \
  }                                                                            \
                                                                               \
  /* freeOnDel getter */                                                       \
  PyObject *name##_freeOnDel_getter(PyObject *self, void *closure) {           \
    if (((PyC_##name *)self)->freeOnDel) {                                     \
      Py_RETURN_TRUE;                                                          \
    }                                                                          \
    Py_RETURN_FALSE;                                                           \
  }                                                                            \
                                                                               \
  /* freeOnDel setter */                                                       \
  int name##_freeOnDel_setter(PyObject *self,                                  \
                              PyObject *value,                                 \
                              void *closure) {                                 \
    if (!value) {                                                              \
      PyErr_SetString(PyExc_AttributeError,                                    \
                      "Cannot delete " #name                                   \
                      ".free_on_no_reference attribute");                      \
      return -1;                                                               \
    }                                                                          \
                                                                               \
    /* TODO: if isPointer is False raise error */                              \
                                                                               \
    int flag = PyObject_IsTrue(value);                                         \
    if (flag == -1) {                                                          \
      return -1;                                                               \
    }                                                                          \
                                                                               \
    ((PyC_##name *)self)->freeOnDel = flag;                                    \
    return 0;                                                                  \
  }                                                                            \
                                                                               \
  /* append */                                                                 \
  PyObject *name##_append(PyObject *self, PyObject *py_value) {                \
    PyC_##name *selfType = (PyC_##name *)self;                                 \
                                                                               \
    if (selfType->isArray == false) {                                          \
      PyErr_SetString(PyExc_TypeError,                                         \
                      "given instance of " #name                               \
                      "is not an array type instance");                        \
      return NULL;                                                             \
    }                                                                          \
                                                                               \
    if (type_check_func(py_value)) {                                           \
      type value = type_convert_pyobj_to_ctype(py_value);                      \
                                                                               \
      if (selfType->arrayCapacity > (selfType->arraySize + 1)) {               \
        selfType->pointer[selfType->arraySize] = value;                        \
      } else {                                                                 \
        int new_capacity =                                                     \
            selfType->arrayCapacity ? (selfType->arrayCapacity * 2) : 2;       \
        selfType->pointer =                                                    \
            realloc(selfType->pointer, new_capacity * sizeof(type));           \
        selfType->arrayCapacity = new_capacity;                                \
                                                                               \
        selfType->pointer[selfType->arraySize] = value;                        \
      }                                                                        \
                                                                               \
      (selfType->arraySize)++;                                                 \
                                                                               \
      Py_INCREF(py_value);                                                     \
      return py_value;                                                         \
    }                                                                          \
    PyErr_SetString(PyExc_TypeError,                                           \
                    "Expected integer type got some other type");              \
    return NULL;                                                               \
  }                                                                            \
                                                                               \
  /* pop */                                                                    \
  PyObject *name##_pop(PyObject *self) {                                       \
    PyC_##name *selfType = (PyC_##name *)self;                                 \
                                                                               \
    if (selfType->isArray == false) {                                          \
      PyErr_SetString(PyExc_TypeError,                                         \
                      "given instance of " #name                               \
                      "is not an array type instance");                        \
      return NULL;                                                             \
    }                                                                          \
                                                                               \
    if (selfType->arraySize == 0) {                                            \
      PyErr_SetString(PyExc_IndexError, "no elements in the array to pop");    \
      return NULL;                                                             \
    }                                                                          \
                                                                               \
    PyObject *rvalue = type_convert_ctype_to_pyobj(                            \
        selfType->pointer[--(selfType->arraySize)]);                           \
                                                                               \
    if ((selfType->arraySize * 2) < selfType->arrayCapacity) {                 \
      selfType->pointer =                                                      \
          realloc(selfType->pointer, (selfType->arraySize) * sizeof(type));    \
      selfType->arrayCapacity = selfType->arraySize;                           \
    }                                                                          \
                                                                               \
    return rvalue;                                                             \
  }                                                                            \
  /* value */                                                                  \
  PyObject *name##_value(PyObject *self) {                                     \
    return type_convert_ctype_to_pyobj(*(((PyC_##name *)self)->pointer));      \
  }                                                                            \
                                                                               \
  /* __int__ */                                                                \
  PyObject *name##_to_int(PyObject *self) {                                    \
    return type_convert_ctype_to_pyint(*(((PyC_##name *)self)->pointer));      \
  }                                                                            \
                                                                               \
  /* __float__ */                                                              \
  PyObject *name##_to_float(PyObject *self) {                                  \
    return PyFloat_FromDouble(*(((PyC_##name *)self)->pointer));               \
  }                                                                            \
                                                                               \
  /* __bool__ */                                                               \
  int name##_to_bool(PyObject *self) {                                         \
    if (*(((PyC_##name *)self)->pointer)) {                                    \
      return 1;                                                                \
    }                                                                          \
    return 0;                                                                  \
  }                                                                            \
                                                                               \
  /* __len__ */                                                                \
  Py_ssize_t name##_len(PyObject *self) {                                      \
    if (((PyC_##name *)self)->isArray == true)                                 \
      return ((PyC_##name *)self)->arraySize;                                  \
                                                                               \
    PyErr_SetString(py_CppError,                                               \
                    "given " #name " instance is not an array type instance"); \
    return -1;                                                                 \
  }                                                                            \
                                                                               \
  /* __getitem__ */                                                            \
  PyObject *name##_getitem(PyObject *self, Py_ssize_t index) {                 \
    PyC_##name *selfType = (PyC_##name *)self;                                 \
                                                                               \
    if (selfType->isArray == false) {                                          \
      PyErr_SetString(PyExc_TypeError,                                         \
                      "given instance of " #name                               \
                      "is not an array type instance");                        \
      return NULL;                                                             \
    }                                                                          \
                                                                               \
    if (selfType->arraySize > index) {                                         \
      return type_convert_ctype_to_pyobj((selfType->pointer)[index]);          \
    } else {                                                                   \
      PyErr_SetString(PyExc_IndexError, "Index out of range");                 \
      return NULL;                                                             \
    }                                                                          \
                                                                               \
    PyErr_SetString(PyExc_TypeError,                                           \
                    "Expected integer type got some other type");              \
    return NULL;                                                               \
  }                                                                            \
                                                                               \
  /* __setitem__ */                                                            \
  int name##_setitem(PyObject *self, Py_ssize_t index, PyObject *py_value) {   \
    PyC_##name *selfType = (PyC_##name *)self;                                 \
                                                                               \
    if (selfType->isArray == false) {                                          \
      PyErr_SetString(PyExc_TypeError,                                         \
                      "given instance of " #name                               \
                      "is not an array type instance");                        \
      return -1;                                                               \
    }                                                                          \
                                                                               \
    if (type_check_func(py_value)) {                                           \
      if (selfType->arraySize > index) {                                       \
        type value                 = type_convert_pyobj_to_ctype(py_value);    \
        (selfType->pointer)[index] = value;                                    \
        return 0;                                                              \
      } else {                                                                 \
        PyErr_SetString(PyExc_IndexError, "Index out of range");               \
        return -1;                                                             \
      }                                                                        \
    }                                                                          \
                                                                               \
    PyErr_SetString(PyExc_TypeError,                                           \
                    "Expected integer type got some other type");              \
    return -1;                                                                 \
  }

// TODO: better error msg fro c_utypes (unsigned types)
// TODO: raise error when setting free_on_no_reference for non pointers
// TODO: change free_on_no_reference attribute to method for c_struct & c_union

// ----- c_type ------
PyTypeObject py_c_type_type = {
    PyVarObject_HEAD_INIT(NULL, 0).tp_name = "PyCpp.c_type",
    .tp_basicsize                          = 0,
    .tp_itemsize                           = 0,
    .tp_flags = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,
    .tp_doc   = "PyCpp.c_type",
    .tp_new   = PyType_GenericNew,
};

long long longlong_from_PyNumber(PyObject *value) {
  PyObject *tmp    = PyNumber_Long(value);
  long long result = PyLong_AsLongLong(tmp);
  Py_DECREF(tmp);
  return result;
}

unsigned long long ulonglong_from_PyNumber(PyObject *value) {
  PyObject *tmp             = PyNumber_Long(value);
  unsigned long long result = PyLong_AsUnsignedLongLong(tmp);
  Py_DECREF(tmp);
  return result;
}

double double_from_PyNumber(PyObject *value) {
  PyObject *tmp = PyNumber_Float(value);
  double result = PyFloat_AsDouble(tmp);
  Py_DECREF(tmp);
  return result;
}

bool CPyBool_Check(PyObject *obj) {
  return PyObject_HasAttrString(obj, "__bool__");
}

bool CPyFloat_Check(PyObject *obj) {
  PyFloat_AsDouble(obj);
  return PyErr_Occurred() ? false : true;
}

bool CPyChar_Check(PyObject *obj) {
  Py_ssize_t size = 0;
  if (PyNumber_Check(obj) ||
      (PyUnicode_Check(obj) && PyUnicode_AsUTF8AndSize(obj, &size) &&
       size <= 1)) {
    return 1;
  }
  return 0;
}

char CPyChar_AsChar(PyObject *obj) {
  Py_ssize_t size = 0;
  if (PyUnicode_AsUTF8AndSize(obj, &size)) {
    if (size == 0) {
      return 0;
    }
    return PyUnicode_ReadChar(obj, 0);
  }

  return PyLong_AsLong(obj);
}

PyObject *CPyChar_FromChar(char c) {
  const char str[2] = {c, 0};
  return PyUnicode_FromFormat("s", str);
}

PyObject *c_char_str(PyObject *self) {
  return PyUnicode_FromFormat("const char *format, ...");
}

NEW_PY_CTYPE_DEL(c_char,
                 char,
                 &py_c_type_type,
                 CPyChar_Check,
                 PyLong_AsLong,
                 PyLong_FromLong,
                 PyLong_FromLong);

NEW_PY_CTYPE_DEL(c_uchar,
                 unsigned char,
                 &py_c_type_type,
                 PyNumber_Check,
                 PyLong_AsUnsignedLong,
                 PyLong_FromUnsignedLong,
                 PyLong_FromUnsignedLong);

NEW_PY_CTYPE_DEL(c_short,
                 short,
                 &py_c_type_type,
                 PyNumber_Check,
                 longlong_from_PyNumber,
                 PyLong_FromLong,
                 PyLong_FromLong);

NEW_PY_CTYPE_DEL(c_ushort,
                 unsigned short,
                 &py_c_type_type,
                 PyNumber_Check,
                 ulonglong_from_PyNumber,
                 PyLong_FromUnsignedLong,
                 PyLong_FromUnsignedLong);

NEW_PY_CTYPE_DEL(c_int,
                 int,
                 &py_c_type_type,
                 PyNumber_Check,
                 longlong_from_PyNumber,
                 PyLong_FromLong,
                 PyLong_FromLong);

NEW_PY_CTYPE_DEL(c_uint,
                 unsigned int,
                 &py_c_type_type,
                 PyNumber_Check,
                 ulonglong_from_PyNumber,
                 PyLong_FromUnsignedLong,
                 PyLong_FromUnsignedLong);

NEW_PY_CTYPE_DEL(c_long,
                 long,
                 &py_c_type_type,
                 PyNumber_Check,
                 longlong_from_PyNumber,
                 PyLong_FromLong,
                 PyLong_FromLong);

NEW_PY_CTYPE_DEL(c_ulong,
                 unsigned long,
                 &py_c_type_type,
                 PyNumber_Check,
                 ulonglong_from_PyNumber,
                 PyLong_FromUnsignedLong,
                 PyLong_FromUnsignedLong);

NEW_PY_CTYPE_DEL(c_float,
                 float,
                 &py_c_type_type,
                 CPyFloat_Check,
                 double_from_PyNumber,
                 PyFloat_FromDouble,
                 PyLong_FromDouble);

NEW_PY_CTYPE_DEL(c_double,
                 double,
                 &py_c_type_type,
                 CPyFloat_Check,
                 double_from_PyNumber,
                 PyFloat_FromDouble,
                 PyLong_FromDouble);

NEW_PY_CTYPE_DEL(c_bool,
                 bool,
                 &py_c_type_type,
                 CPyBool_Check,
                 PyObject_IsTrue,
                 PyBool_FromLong,
                 PyLong_FromLong);

// ----- c_void ------

PyMethodDef c_void_methods[] = {{NULL, NULL, 0, NULL}};

PyMemberDef c_void_members[] = {{NULL, 0, 0, 0, NULL}};

PyGetSetDef c_void_getsetdef[] = {
    {"free_on_no_reference",
     &c_void_freeOnDel_getter,
     &c_void_freeOnDel_setter,
     C_TYPE_FREE_ON_NO_REFERENCE_DOC,
     NULL},
    {NULL, NULL, NULL, NULL},
};

PyTypeObject py_c_void_type = {
    PyVarObject_HEAD_INIT(NULL, 0).tp_name = "PyCpp.c_void",
    .tp_basicsize                          = sizeof(PyC_c_void),
    .tp_itemsize                           = 0,
    .tp_flags                              = Py_TPFLAGS_DEFAULT,
    .tp_doc                                = "PyCpp.c_void",
    .tp_methods                            = c_void_methods,
    .tp_members                            = c_void_members,
    .tp_getset                             = c_void_getsetdef,
    .tp_init                               = &c_void_init,
    .tp_new                                = PyType_GenericNew,
    .tp_dealloc                            = &c_void_finalizer,
    .tp_base                               = &py_c_type_type,
};

// ----- c_void: functions and methods -----

// PyC.c_void.__init__
static int c_void_init(PyObject *self, PyObject *args, PyObject *kwargs) {
  PyC_c_void *selfType = (PyC_c_void *)self;
  selfType->pointer    = NULL;
  selfType->freeOnDel  = true;
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

static int
c_void_freeOnDel_setter(PyObject *self, PyObject *value, void *closure) {
  PyC_c_void *selfType = (PyC_c_void *)self;

  if (!value) {
    PyErr_SetString(PyExc_AttributeError,
                    "Cannot delete c_void.free_on_no_reference attrubute");
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
    .mp_length        = &c_struct_len,
    .mp_subscript     = &c_struct_getitem,
    .mp_ass_subscript = &c_struct_setitem,
};

PyMemberDef c_struct_members[] = {
    {"is_array",
     T_BOOL,
     offsetof(PyC_c_struct, isArray),
     READONLY,
     "PyC.c_struct.is_array"},
    // {"__python_representation", T_OBJECT, offsetof(PyC_c_struct,
    // pyDictRepr),
    //  READONLY, "PyC.c_struct.__python_representation"},
    {NULL, 0, 0, 0, NULL}};

PyMethodDef c_struct_methods[] = {
    {"append",
     (PyCFunction)&c_struct_append,
     METH_VARARGS,
     "c_struct.append()"},
    {"pop", (PyCFunction)&c_struct_pop, METH_NOARGS, "c_struct.pop()"},
    {NULL, NULL, 0, NULL}};

PyGetSetDef c_struct_getsetdef[] = {
    {"free_on_no_reference",
     &c_struct_freeOnDel_getter,
     &c_struct_freeOnDel_setter,
     C_TYPE_FREE_ON_NO_REFERENCE_DOC,
     NULL},
    {NULL, NULL, NULL, NULL},
};

PyTypeObject py_c_struct_type = {
    PyVarObject_HEAD_INIT(NULL, 0).tp_name = "PyCpp.c_struct",
    .tp_basicsize                          = sizeof(PyC_c_struct),
    .tp_itemsize                           = 0,
    .tp_getattr                            = &c_struct_getattr,
    .tp_setattr                            = &c_struct_setattr,
    .tp_as_mapping                         = &c_struct_as_mapping,
    .tp_flags = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE | Py_TPFLAGS_HAVE_GC,
    .tp_doc   = "PyCpp.c_struct",
    .tp_iter  = &c_struct_iter,
    .tp_iternext = &c_struct_next,
    .tp_methods  = c_struct_methods,
    .tp_members  = c_struct_members,
    .tp_getset   = c_struct_getsetdef,
    .tp_init     = &c_struct_init,
    .tp_new      = PyType_GenericNew,
    .tp_dealloc  = &c_struct_finalizer,
    .tp_traverse = &c_struct_Traverse,
    .tp_clear    = &c_struct_Clear,
    .tp_base     = &py_c_type_type,
};

// c_struct methods

// PyC.c_struct.__init__
static int c_struct_init(PyObject *self, PyObject *args, PyObject *kwargs) {
  PyC_c_struct *selfType = (PyC_c_struct *)self;
  Structure *s           = ((struct Custom_s_PyTypeObject *)self->ob_type)->s;

  selfType->structure     = s;
  selfType->freeOnDel     = true;
  selfType->isArray       = false;
  selfType->arraySize     = 0;
  selfType->arrayCapacity = 0;

  selfType->parentModule =
      ((struct Custom_s_PyTypeObject *)self->ob_type)->parentModule;
  Py_INCREF(selfType->parentModule);

  selfType->pointer = malloc(s->structSize);

  size_t args_len = PyTuple_Size(args);
  if (args_len == 0) {
    return 0;
  }

  if (args_len == 1 && PySequence_Check(PyTuple_GetItem(args, 0))) {
    PyObject *arr  = PyTuple_GetItem(args, 0);
    size_t arr_len = PySequence_Size(arr);

    free(selfType->pointer);
    selfType->pointer       = calloc(arr_len, s->structSize);
    selfType->isArray       = true;
    selfType->arraySize     = arr_len;
    selfType->arrayCapacity = arr_len;

    for (size_t i = 0; i < arr_len; i++) {
      PyObject *element = PySequence_GetItem(arr, i);

      if (Py_TYPE(element) != Py_TYPE(element)) {
        PyErr_SetString(py_BindingError,
                        "Expected all elements of the sequence to be of the "
                        "same struct type");
        return -1;
      }

      PyC_c_struct *elementType = (PyC_c_struct *)element;
      memcpy(selfType->pointer + (s->structSize * i),
             elementType->pointer,
             s->structSize);
    }

    return 0;
  }

  // initilising struct with attributes
  if (args_len != s->attrCount) {
    PyErr_SetString(py_BindingError,
                    "Args count does not match struct's attribute count. "
                    "Cannot initilise the struct.");
    return -1;
  }

  for (size_t i = 0; i < selfType->structure->attrCount; i++) {
    ffi_type *type =
        p_ffi_type_array_getat(selfType->structure->attrTypeFFI, i);
    enum CXX_Type cxx_type =
        CXX_Type_array_getat(selfType->structure->attrTypeCXX, i);
    PyObject *attr = PyTuple_GetItem(args, i);

    bool should_free = false;
    void *data       = pyArg_to_cppArg(attr, cxx_type, &should_free);
    if (!data) {
      return -1;
    }

    memcpy((char *)(selfType->pointer) +
               (long_long_array_getat(selfType->structure->offsets, i) / 8),
           data,
           type->size);

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
    if (strcmp(attr, str_array_getat(selfType->structure->attrNames, i)) == 0) {
      char *data = (char *)selfType->pointer;

      return cppArg_to_pyArg(
          data + (long_long_array_getat(selfType->structure->offsets, i) / 8),
          CXX_Type_array_getat(selfType->structure->attrTypeCXX, i),
          p_void_array_getat(selfType->structure->attrTypeInfo, i),
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
    int result    = PyObject_GenericSetAttr(self, key, pValue);
    Py_DECREF(key);
    return result;
  }

  for (size_t i = 0; i < selfType->structure->attrCount; i++) {
    if (!(strcmp(attr, str_array_getat(selfType->structure->attrNames, i)))) {
      enum CXX_Type cxx_type =
          CXX_Type_array_getat(selfType->structure->attrTypeCXX, i);
      ffi_type *type =
          p_ffi_type_array_getat(selfType->structure->attrTypeFFI, i);

      bool should_free = false;
      void *data       = pyArg_to_cppArg(pValue, cxx_type, &should_free);
      if (!data) {
        return -1;
      }

      memcpy((char *)(selfType->pointer) +
                 (long_long_array_getat(selfType->structure->offsets, i) / 8),
             data,
             type->size);

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
  return 0;
}

static int c_struct_Clear(PyObject *self) {
  PyC_c_struct *selfType = (PyC_c_struct *)self;

  Py_CLEAR(selfType->parentModule);
  return 0;
}

// helper function
PyObject *create_py_c_struct(Structure *structure, PyObject *module) {
  // BUG: only do PyType_Ready and return the type itself
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
              .tp_basicsize                          = sizeof(PyC_c_struct),
              .tp_itemsize                           = 0,
              .tp_flags                              = Py_TPFLAGS_DEFAULT,
              .tp_doc                                = struct_name,
              .tp_new                                = PyType_GenericNew,
              .tp_base                               = &py_c_struct_type,
          },
      .s            = structure,
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
  size_t index     = selfType->_i;

  if (selfType->arraySize > index) {
    rvalue = cppArg_to_pyArg(selfType->pointer +
                                 (selfType->structure->structSize * index),
                             CXX_Struct,
                             selfType->structure,
                             selfType->parentModule);
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
           valueType->pointer,
           selfType->structure->structSize);
  } else {
    int new_capacity =
        selfType->arrayCapacity ? (selfType->arrayCapacity * 2) : 2;
    selfType->pointer       = realloc(selfType->pointer,
                                new_capacity * selfType->structure->structSize);
    selfType->arrayCapacity = new_capacity;

    memcpy(selfType->pointer +
               (selfType->structure->structSize * selfType->arraySize),
           valueType->pointer,
           selfType->structure->structSize);
  }

  void *data = selfType->pointer +
               (selfType->structure->structSize * selfType->arraySize);

  (selfType->arraySize)++;

  return cppArg_to_pyArg(data,
                         CXX_Struct,
                         selfType->structure,
                         selfType->parentModule);
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
    PyErr_SetString(PyExc_IndexError, "no elements in the array to pop");
    return NULL;
  }

  // BUG: I guess this is flawed logic to pop the memory should be copied before
  // pop
  void *data = selfType->pointer +
               (selfType->structure->structSize * selfType->arraySize - 1);
  PyObject *tmp = cppArg_to_pyArg(data,
                                  CXX_Struct,
                                  selfType->structure,
                                  selfType->parentModule);

  (selfType->arraySize)--;

  if ((selfType->arraySize * 2 + 1) < selfType->arrayCapacity) {
    selfType->pointer =
        realloc(selfType->pointer,
                (selfType->arraySize + 1) * selfType->structure->structSize);
    selfType->arrayCapacity = selfType->arraySize * 2 + 1;
  }

  return tmp;
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

  if (selfType->arraySize > index) {
    void *data = selfType->pointer + (selfType->structure->structSize * index);
    return cppArg_to_pyArg(data,
                           CXX_Struct,
                           selfType->structure,
                           selfType->parentModule);
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
           valueType->pointer,
           selfType->structure->structSize);

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
static int
c_struct_freeOnDel_setter(PyObject *self, PyObject *value, void *closure) {
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
    .mp_length        = &c_union_len,
    .mp_subscript     = &c_union_getitem,
    .mp_ass_subscript = &c_union_setitem,
};

PyMemberDef c_union_members[] = {
    {"is_array",
     T_BOOL,
     offsetof(PyC_c_union, isArray),
     READONLY,
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
    {"free_on_no_reference",
     &c_union_freeOnDel_getter,
     &c_union_freeOnDel_setter,
     C_TYPE_FREE_ON_NO_REFERENCE_DOC,
     NULL},
    {NULL, NULL, NULL, NULL},
};

PyTypeObject py_c_union_type = {
    PyVarObject_HEAD_INIT(NULL, 0).tp_name = "PyCpp.c_union",
    .tp_basicsize                          = sizeof(PyC_c_union),
    .tp_itemsize                           = 0,
    .tp_getattr                            = &c_union_getattr,
    .tp_setattr                            = &c_union_setattr,
    .tp_as_mapping                         = &c_union_as_mapping,
    .tp_flags = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE | Py_TPFLAGS_HAVE_GC,
    .tp_doc   = "PyCpp.c_union",
    .tp_iter  = &c_union_iter,
    .tp_iternext = &c_union_next,
    .tp_methods  = c_union_methods,
    .tp_members  = c_union_members,
    .tp_getset   = c_union_getsetdef,
    .tp_init     = &c_union_init,
    .tp_new      = PyType_GenericNew,
    .tp_dealloc  = &c_union_finalizer,
    .tp_traverse = &c_union_Traverse,
    .tp_clear    = &c_union_Clear,
    .tp_base     = &py_c_type_type,
};

// c_union methods

// PyC.c_union.__init__
static int c_union_init(PyObject *self, PyObject *args, PyObject *kwargs) {
  PyC_c_union *selfType = (PyC_c_union *)self;
  Union *s              = ((struct Custom_u_PyTypeObject *)self->ob_type)->u;

  selfType->u         = s;
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
    PyObject *arr  = PyTuple_GetItem(args, 0);
    size_t arr_len = PySequence_Size(arr);

    free(selfType->pointer);
    selfType->pointer       = calloc(arr_len, s->unionSize);
    selfType->isArray       = true;
    selfType->arraySize     = arr_len;
    selfType->arrayCapacity = arr_len;

    for (size_t i = 0; i < arr_len; i++) {
      PyObject *element = PySequence_GetItem(arr, i);

      if (Py_TYPE(element) != Py_TYPE(element)) {
        PyErr_SetString(py_BindingError,
                        "Expected all elements of the sequence to be of the "
                        "same union type");
        return -1;
      }

      PyC_c_struct *elementType = (PyC_c_struct *)element;
      memcpy(selfType->pointer + (s->unionSize * i),
             elementType->pointer,
             s->unionSize);
    }

    return 0;
  }

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
      return cppArg_to_pyArg((selfType->pointer),
                             CXX_Type_array_getat(selfType->u->attrTypeCXX, i),
                             p_void_array_getat(selfType->u->attrTypeInfo, i),
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
    int result    = PyObject_GenericSetAttr(self, key, pValue);
    Py_DECREF(key);
    return result;
  }

  for (size_t i = 0; i < selfType->u->attrCount; i++) {
    if (!(strcmp(attr, str_array_getat(selfType->u->attrNames, i)))) {
      enum CXX_Type cxx_type =
          CXX_Type_array_getat(selfType->u->attrTypeCXX, i);
      ffi_type *type = p_ffi_type_array_getat(selfType->u->attrTypeFFI, i);

      bool should_free = false;
      void *data       = pyArg_to_cppArg(pValue, cxx_type, &should_free);
      if (data == NULL)
        return -1;

      memcpy((selfType->pointer), data, type->size);

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

  PyObject_GC_UnTrack(self);
  c_union_Clear(self);

  if (selfType->pointer && selfType->freeOnDel) {
    free(selfType->pointer);
  }

  Py_TYPE(self)->tp_free((PyObject *)self);
}

static int c_union_Traverse(PyObject *self, visitproc visit, void *arg) {
  PyC_c_union *selfType = (PyC_c_union *)self;

  Py_VISIT(selfType->parentModule);
  return 0;
}

static int c_union_Clear(PyObject *self) {
  PyC_c_union *selfType = (PyC_c_union *)self;

  Py_CLEAR(selfType->parentModule);
  return 0;
}

// helper function
PyObject *create_py_c_union(Union *u, PyObject *module) {
  // BUG: only do PyType_Ready and return the type itself
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
              .tp_basicsize                          = sizeof(PyC_c_union),
              .tp_itemsize                           = 0,
              .tp_flags                              = Py_TPFLAGS_DEFAULT,
              .tp_doc                                = struct_name,
              .tp_new                                = PyType_GenericNew,
              .tp_base                               = &py_c_union_type,
          },
      .u            = u,
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
  size_t index     = selfType->_i;

  if (selfType->arraySize > index) {
    void *data = selfType->pointer + (selfType->u->unionSize * index);
    return cppArg_to_pyArg(data,
                           CXX_Struct,
                           selfType->u,
                           selfType->parentModule);
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
           valueType->pointer,
           selfType->u->unionSize);
  } else {
    int new_capacity =
        selfType->arrayCapacity ? (selfType->arrayCapacity * 2) : 2;
    selfType->pointer =
        realloc(selfType->pointer, new_capacity * selfType->u->unionSize);
    selfType->arrayCapacity = new_capacity;

    memcpy(selfType->pointer + (selfType->u->unionSize * selfType->arraySize),
           valueType->pointer,
           selfType->u->unionSize);
  }

  void *data =
      selfType->pointer + (selfType->u->unionSize * selfType->arraySize);

  (selfType->arraySize)++;

  return cppArg_to_pyArg(data, CXX_Union, selfType->u, selfType->parentModule);
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
    PyErr_SetString(PyExc_IndexError, "no elements in the array to pop");
    return NULL;
  }

  // BUG: I guess this is flawed logic to pop the memory should be copied before
  // pop
  void *data =
      selfType->pointer + (selfType->u->unionSize * selfType->arraySize - 1);
  PyObject *tmp =
      cppArg_to_pyArg(data, CXX_Union, selfType->u, selfType->parentModule);

  (selfType->arraySize)--;

  if ((selfType->arraySize * 2 + 1) < selfType->arrayCapacity) {
    selfType->pointer =
        realloc(selfType->pointer,
                (selfType->arraySize + 1) * selfType->u->unionSize);
    selfType->arrayCapacity = selfType->arraySize * 2 + 1;
  }

  return tmp;
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

  if (selfType->arraySize > index) {
    void *data = selfType->pointer + (selfType->u->unionSize * index);
    return cppArg_to_pyArg(data,
                           CXX_Union,
                           selfType->u,
                           selfType->parentModule);
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
           valueType->pointer,
           selfType->u->unionSize);

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

static int
c_union_freeOnDel_setter(PyObject *self, PyObject *value, void *closure) {
  PyC_c_union *selfType = (PyC_c_union *)self;

  if (!value) {
    PyErr_SetString(PyExc_AttributeError,
                    "Cannot delete c_union.free_on_no_reference attrubute");
    return -1;
  }

  int flag = PyObject_IsTrue(value);
  if (flag == -1) {
    return -1;
  }

  selfType->freeOnDel = flag;
  return 0;
}
