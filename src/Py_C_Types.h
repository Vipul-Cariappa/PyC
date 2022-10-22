#pragma once
#include "PyC.h"
#include "Python.h"
#include <stdbool.h>

// c_type: c_void
extern PyTypeObject py_c_void_type;

typedef struct PyC_c_void {
  PyObject_HEAD void *pointer;
  bool freeOnDel;
  bool isArray;
  size_t arraySize;
  size_t arrayCapacity;
  size_t _i; // for iteration purpose
} PyC_c_void;

static int c_void_init(PyObject *self, PyObject *args, PyObject *kwargs);
static void c_void_finalizer(PyObject *self);
static PyObject *c_void_freeOnDel_getter(PyObject *self, void *closure);
static int c_void_freeOnDel_setter(PyObject *self, PyObject *value,
                                   void *closure);

// c_type: c_int
extern PyTypeObject py_c_int_type;
extern PyTypeObject py_c_uint_type;

typedef struct PyC_c_int {
  PyObject_HEAD int value;
  int *pointer;
  bool freeOnDel;
  bool isPointer;
  bool isArray;
  size_t arraySize;
  size_t arrayCapacity;
  size_t _i; // for iteration purpose
} PyC_c_int;

static int c_int_init(PyObject *self, PyObject *args, PyObject *kwargs);
static PyObject *c_int_iter(PyObject *self);
static PyObject *c_int_next(PyObject *self);
static void c_int_finalizer(PyObject *self);
static PyObject *c_int_append(PyObject *self, PyObject *args);
static PyObject *c_int_pop(PyObject *self);
static PyObject *c_int_value(PyObject *self);
static PyObject *c_int_freeOnDel_getter(PyObject *self, void *closure);
static int c_int_freeOnDel_setter(PyObject *self, PyObject *value,
                                  void *closure);
static PyObject *c_int_to_int(PyObject *self);
static PyObject *c_int_to_float(PyObject *self);
static int c_int_to_bool(PyObject *self);
static Py_ssize_t c_int_len(PyObject *self);
static PyObject *c_int_getitem(PyObject *self, PyObject *attr);
static int c_int_setitem(PyObject *self, PyObject *attr, PyObject *value);

// c_type: c_double
extern PyTypeObject py_c_double_type;

typedef struct PyC_c_double {
  PyObject_HEAD double value;
  double *pointer;
  bool freeOnDel;
  bool isPointer;
  bool isArray;
  size_t arraySize;
  size_t arrayCapacity;
  size_t _i; // for iteration purpose
} PyC_c_double;

static int c_double_init(PyObject *self, PyObject *args, PyObject *kwargs);
static PyObject *c_double_iter(PyObject *self);
static PyObject *c_double_next(PyObject *self);
static void c_double_finalizer(PyObject *self);
static PyObject *c_double_append(PyObject *self, PyObject *args);
static PyObject *c_double_pop(PyObject *self);
static PyObject *c_double_value(PyObject *self);
static PyObject *c_double_freeOnDel_getter(PyObject *self, void *closure);
static int c_double_freeOnDel_setter(PyObject *self, PyObject *value,
                                     void *closure);
static PyObject *c_double_to_int(PyObject *self);
static PyObject *c_double_to_float(PyObject *self);
static int c_double_to_bool(PyObject *self);
static Py_ssize_t c_double_len(PyObject *self);
static PyObject *c_double_getitem(PyObject *self, PyObject *attr);
static int c_double_setitem(PyObject *self, PyObject *attr, PyObject *value);

// c_type: c_bool
extern PyTypeObject py_c_bool_type;

typedef struct PyC_c_bool {
  PyObject_HEAD bool value;
  bool *pointer;
  bool freeOnDel;
  bool isPointer;
  bool isArray;
  size_t arraySize;
  size_t arrayCapacity;
  size_t _i; // for iteration purpose
} PyC_c_bool;

static int c_bool_init(PyObject *self, PyObject *args, PyObject *kwargs);
static PyObject *c_bool_iter(PyObject *self);
static PyObject *c_bool_next(PyObject *self);
static PyObject *c_bool_getattr(PyObject *self, char *attr);
static void c_bool_finalizer(PyObject *self);
static PyObject *c_bool_append(PyObject *self, PyObject *args);
static PyObject *c_bool_pop(PyObject *self);
static PyObject *c_bool_value(PyObject *self);
static PyObject *c_bool_freeOnDel_getter(PyObject *self, void *closure);
static int c_bool_freeOnDel_setter(PyObject *self, PyObject *value,
                                   void *closure);
static PyObject *c_bool_to_int(PyObject *self);
static PyObject *c_bool_to_float(PyObject *self);
static int c_bool_to_bool(PyObject *self);
static Py_ssize_t c_bool_len(PyObject *self);
static PyObject *c_bool_getitem(PyObject *self, PyObject *attr);
static int c_bool_setitem(PyObject *self, PyObject *attr, PyObject *value);

// c_type: c_char
extern PyTypeObject py_c_char_type;

typedef struct PyC_c_char {
  PyObject_HEAD char value;
  char *pointer;
  bool freeOnDel;
  bool isPointer;
  bool isArray;
  size_t arraySize;
  size_t arrayCapacity;
  size_t _i; // for iteration purpose
} PyC_c_char;

static int c_char_init(PyObject *self, PyObject *args, PyObject *kwargs);
static PyObject *c_char_iter(PyObject *self);
static PyObject *c_char_next(PyObject *self);
static PyObject *c_char_getattr(PyObject *self, char *attr);
static void c_char_finalizer(PyObject *self);
static PyObject *c_char_append(PyObject *self, PyObject *args);
static PyObject *c_char_pop(PyObject *self);
static PyObject *c_char_value(PyObject *self);
static PyObject *c_char_freeOnDel_getter(PyObject *self, void *closure);
static int c_char_freeOnDel_setter(PyObject *self, PyObject *value,
                                   void *closure);
static PyObject *c_char_to_str(PyObject *self);
static Py_ssize_t c_char_len(PyObject *self);
static PyObject *c_char_getitem(PyObject *self, PyObject *attr);
static int c_char_setitem(PyObject *self, PyObject *attr, PyObject *value);

// c_type: c_float
extern PyTypeObject py_c_float_type;

typedef struct PyC_c_float {
  PyObject_HEAD float value;
  float *pointer;
  bool freeOnDel;
  bool isPointer;
  bool isArray;
  size_t arraySize;
  size_t arrayCapacity;
  size_t _i; // for iteration purpose
} PyC_c_float;

static int c_float_init(PyObject *self, PyObject *args, PyObject *kwargs);
static PyObject *c_float_iter(PyObject *self);
static PyObject *c_float_next(PyObject *self);
static void c_float_finalizer(PyObject *self);
static PyObject *c_float_append(PyObject *self, PyObject *args);
static PyObject *c_float_pop(PyObject *self);
static PyObject *c_float_value(PyObject *self);
static PyObject *c_float_freeOnDel_getter(PyObject *self, void *closure);
static int c_float_freeOnDel_setter(PyObject *self, PyObject *value,
                                    void *closure);
static PyObject *c_float_to_int(PyObject *self);
static PyObject *c_float_to_float(PyObject *self);
static int c_float_to_bool(PyObject *self);
static Py_ssize_t c_float_len(PyObject *self);
static PyObject *c_float_getitem(PyObject *self, PyObject *attr);
static int c_float_setitem(PyObject *self, PyObject *attr, PyObject *value);

// c_type: c_short
extern PyTypeObject py_c_short_type;
extern PyTypeObject py_c_ushort_type;

typedef struct PyC_c_short {
  PyObject_HEAD short value;
  short *pointer;
  bool freeOnDel;
  bool isPointer;
  bool isArray;
  size_t arraySize;
  size_t arrayCapacity;
  size_t _i; // for iteration purpose
} PyC_c_short;

static int c_short_init(PyObject *self, PyObject *args, PyObject *kwargs);
static PyObject *c_short_iter(PyObject *self);
static PyObject *c_short_next(PyObject *self);
static void c_short_finalizer(PyObject *self);
static PyObject *c_short_append(PyObject *self, PyObject *args);
static PyObject *c_short_pop(PyObject *self);
static PyObject *c_short_value(PyObject *self);
static PyObject *c_short_freeOnDel_getter(PyObject *self, void *closure);
static int c_short_freeOnDel_setter(PyObject *self, PyObject *value,
                                    void *closure);
static PyObject *c_short_to_int(PyObject *self);
static PyObject *c_short_to_float(PyObject *self);
static int c_short_to_bool(PyObject *self);
static Py_ssize_t c_short_len(PyObject *self);
static PyObject *c_short_getitem(PyObject *self, PyObject *attr);
static int c_short_setitem(PyObject *self, PyObject *attr, PyObject *value);

// c_type: c_long
extern PyTypeObject py_c_long_type;
extern PyTypeObject py_c_ulong_type;

typedef struct PyC_c_long {
  PyObject_HEAD long value;
  long *pointer;
  bool freeOnDel;
  bool isPointer;
  bool isArray;
  size_t arraySize;
  size_t arrayCapacity;
  size_t _i; // for iteration purpose
} PyC_c_long;

static int c_long_init(PyObject *self, PyObject *args, PyObject *kwargs);
static PyObject *c_long_iter(PyObject *self);
static PyObject *c_long_next(PyObject *self);
static void c_long_finalizer(PyObject *self);
static PyObject *c_long_append(PyObject *self, PyObject *args);
static PyObject *c_long_pop(PyObject *self);
static PyObject *c_long_value(PyObject *self);
static PyObject *c_long_freeOnDel_getter(PyObject *self, void *closure);
static int c_long_freeOnDel_setter(PyObject *self, PyObject *value,
                                   void *closure);
static PyObject *c_long_to_int(PyObject *self);
static PyObject *c_long_to_float(PyObject *self);
static int c_long_to_bool(PyObject *self);
static Py_ssize_t c_long_len(PyObject *self);
static PyObject *c_long_getitem(PyObject *self, PyObject *attr);
static int c_long_setitem(PyObject *self, PyObject *attr, PyObject *value);

// c_struct
extern PyTypeObject py_c_struct_type;

typedef struct PyC_c_struct {
  PyObject_HEAD Structure *structure;
  void *pointer;
  bool freeOnDel;
  bool isArray;
  size_t arraySize;
  size_t arrayCapacity;
  size_t _i; // for iteration purpose
  // PyObject *pyDictRepr;   // TODO: python dict representation of struct:
  // key: attr name value: c_type
  PyObject *parentModule;
  PyObject *child_ptrs;
  PyObject *arrayPtrs; // PyList of member array's PyC_c_unions
} PyC_c_struct;

static int c_struct_init(PyObject *self, PyObject *args, PyObject *kwargs);
static PyObject *c_struct_getattr(PyObject *self, char *attr);
static int c_struct_setattr(PyObject *self, char *attr, PyObject *pValue);
static void c_struct_finalizer(PyObject *self);
static int c_struct_Traverse(PyObject *self, visitproc visit, void *arg);
static int c_struct_Clear(PyObject *self);
static PyObject *c_struct_call(PyObject *self, PyObject *args,
                               PyObject *kwargs);
static PyObject *c_struct_iter(PyObject *self);
static PyObject *c_struct_next(PyObject *self);
static PyObject *c_struct_append(PyObject *self, PyObject *args);
static PyObject *c_struct_pop(PyObject *self);
static PyObject *c_struct_freeOnDel_getter(PyObject *self, void *closure);
static int c_struct_freeOnDel_setter(PyObject *self, PyObject *value,
                                     void *closure);
static Py_ssize_t c_struct_len(PyObject *self);
static PyObject *c_struct_getitem(PyObject *self, PyObject *attr);
static int c_struct_setitem(PyObject *self, PyObject *attr, PyObject *value);
PyObject *create_py_c_struct(Structure *structure,
                             PyObject *module); // helper function

// c_union
extern PyTypeObject py_c_union_type;

typedef struct PyC_c_union {
  PyObject_HEAD Union *u;
  void *pointer;
  bool freeOnDel;
  bool isArray;
  size_t arraySize;
  size_t arrayCapacity;
  size_t _i; // for iteration purpose
  // PyObject *pyDictRepr; // TODO: python dict representation of union:
  // key: attr name value: c_type
  PyObject *parentModule;
  PyObject *arrayPtrs; // PyList of member array's PyC_c_unions
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
static int c_union_freeOnDel_setter(PyObject *self, PyObject *value,
                                    void *closure);
static Py_ssize_t c_union_len(PyObject *self);
static PyObject *c_union_getitem(PyObject *self, PyObject *attr);
static int c_union_setitem(PyObject *self, PyObject *attr, PyObject *value);
PyObject *create_py_c_union(Union *u,
                            PyObject *module); // helper function
