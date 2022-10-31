#pragma once
#include "PyC.h"
#include "Python.h"
#include <stdbool.h>

#define NEW_PY_CTYPE_DEC(name, type)                                           \
  extern PyTypeObject py_##name##_type;                                        \
                                                                               \
  typedef struct _PyC_##name {                                                 \
    PyObject ob_base;                                                          \
    type *pointer;                                                             \
    bool freeOnDel;                                                            \
    bool isPointer;                                                            \
    bool isArray;                                                              \
    size_t arraySize;                                                          \
    size_t arrayCapacity;                                                      \
    size_t _i; /* for iteration purpose */                                     \
    type value;                                                                \
  } PyC_##name;                                                                \
                                                                               \
  int name##_init(PyObject *self, PyObject *args, PyObject *kwargs);           \
  void name##_finalizer(PyObject *self);                                       \
  PyObject *name##_iter(PyObject *self);                                       \
  PyObject *name##_next(PyObject *self);                                       \
  PyObject *name##_freeOnDel_getter(PyObject *self, void *closure);            \
  int name##_freeOnDel_setter(PyObject *self, PyObject *value, void *closure); \
  PyObject *name##_append(PyObject *self, PyObject *args);                     \
  PyObject *name##_pop(PyObject *self);                                        \
  PyObject *name##_value(PyObject *self);                                      \
  PyObject *name##_to_int(PyObject *self);                                     \
  PyObject *name##_to_float(PyObject *self);                                   \
  int name##_to_bool(PyObject *self);                                          \
  Py_ssize_t name##_len(PyObject *self);                                       \
  PyObject *name##_getitem(PyObject *self, Py_ssize_t index);                  \
  int name##_setitem(PyObject *self, Py_ssize_t index, PyObject *value)

// c_type: c_type
extern PyTypeObject py_c_type_type;

NEW_PY_CTYPE_DEC(c_char, char);
PyObject *c_char_str(PyObject *self);
NEW_PY_CTYPE_DEC(c_uchar, unsigned char);
NEW_PY_CTYPE_DEC(c_short, short);
NEW_PY_CTYPE_DEC(c_ushort, unsigned short);
NEW_PY_CTYPE_DEC(c_int, int);
NEW_PY_CTYPE_DEC(c_uint, unsigned int);
NEW_PY_CTYPE_DEC(c_long, long);
NEW_PY_CTYPE_DEC(c_ulong, unsigned long);
NEW_PY_CTYPE_DEC(c_float, float);
NEW_PY_CTYPE_DEC(c_double, double);
NEW_PY_CTYPE_DEC(c_bool, bool);

// c_type: c_void
extern PyTypeObject py_c_void_type;

typedef struct PyC_c_void {
  PyObject ob_base;
  void *pointer;
  bool freeOnDel;
  bool isPointer;
  bool isArray;
  size_t arraySize;
  size_t arrayCapacity;
  size_t _i; // for iteration purpose
} PyC_c_void;

static int c_void_init(PyObject *self, PyObject *args, PyObject *kwargs);
static void c_void_finalizer(PyObject *self);
static PyObject *c_void_freeOnDel_getter(PyObject *self, void *closure);
static int
c_void_freeOnDel_setter(PyObject *self, PyObject *value, void *closure);

// c_struct
extern PyTypeObject py_c_struct_type;

typedef struct PyC_c_struct {
  PyObject ob_base;
  void *pointer;
  bool freeOnDel;
  bool isPointer;
  bool isArray;
  size_t arraySize;
  size_t arrayCapacity;
  size_t _i; // for iteration purpose
  Structure *structure;
  // PyObject *pyDictRepr;   // TODO: python dict representation of struct:
  // key: attr name value: c_type
  PyObject *parentModule;
} PyC_c_struct;

static int c_struct_init(PyObject *self, PyObject *args, PyObject *kwargs);
static PyObject *c_struct_getattr(PyObject *self, char *attr);
static int c_struct_setattr(PyObject *self, char *attr, PyObject *pValue);
static void c_struct_finalizer(PyObject *self);
static int c_struct_Traverse(PyObject *self, visitproc visit, void *arg);
static int c_struct_Clear(PyObject *self);
static PyObject *
c_struct_call(PyObject *self, PyObject *args, PyObject *kwargs);
static PyObject *c_struct_iter(PyObject *self);
static PyObject *c_struct_next(PyObject *self);
static PyObject *c_struct_append(PyObject *self, PyObject *args);
static PyObject *c_struct_pop(PyObject *self);
static PyObject *c_struct_freeOnDel_getter(PyObject *self, void *closure);
static int
c_struct_freeOnDel_setter(PyObject *self, PyObject *value, void *closure);
static Py_ssize_t c_struct_len(PyObject *self);
static PyObject *c_struct_getitem(PyObject *self, PyObject *attr);
static int c_struct_setitem(PyObject *self, PyObject *attr, PyObject *value);
PyObject *create_py_c_struct(Structure *structure,
                             PyObject *module); // helper function

// c_union
extern PyTypeObject py_c_union_type;

typedef struct PyC_c_union {
  PyObject ob_base;
  void *pointer;
  bool freeOnDel;
  bool isPointer;
  bool isArray;
  size_t arraySize;
  size_t arrayCapacity;
  size_t _i; // for iteration purpose
  Union *u;
  // PyObject *pyDictRepr; // TODO: python dict representation of union:
  // key: attr name value: c_type
  PyObject *parentModule;
} PyC_c_union;

static int c_union_init(PyObject *self, PyObject *args, PyObject *kwargs);
static PyObject *c_union_getattr(PyObject *self, char *attr);
static int c_union_setattr(PyObject *self, char *attr, PyObject *pValue);
static void c_union_finalizer(PyObject *self);
static int c_union_Traverse(PyObject *self, visitproc visit, void *arg);
static int c_union_Clear(PyObject *self);
static PyObject *c_union_call(PyObject *self, PyObject *args, PyObject *kwargs);
static PyObject *c_union_iter(PyObject *self);
static PyObject *c_union_next(PyObject *self);
static PyObject *c_union_append(PyObject *self, PyObject *args);
static PyObject *c_union_pop(PyObject *self);
static PyObject *c_union_freeOnDel_getter(PyObject *self, void *closure);
static int
c_union_freeOnDel_setter(PyObject *self, PyObject *value, void *closure);
static Py_ssize_t c_union_len(PyObject *self);
static PyObject *c_union_getitem(PyObject *self, PyObject *attr);
static int c_union_setitem(PyObject *self, PyObject *attr, PyObject *value);
PyObject *create_py_c_union(Union *u,
                            PyObject *module); // helper function
