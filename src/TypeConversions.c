#define PY_SSIZE_T_CLEAN
#include "PyC.h"
#include "Py_C_Types.h"
#include "Python.h"
#include "ffi.h"
#include "clang-c/Index.h"

#define MAX_SIZE 1024

void *pyArg_to_cppArg(PyObject *arg, enum CXX_Type type, bool *should_free) {
  // TODO: add long long
  *should_free = false;

  switch (type) {

  case CXX_VoidPointer:
    if (PyObject_IsInstance(arg, (PyObject *)&py_c_type_type)) {
      return &((PyC_c_void *)arg)->pointer;
    } else {
      PyErr_SetString(PyExc_TypeError, "Expected c_void got other type");
      return NULL;
    }

  case CXX_Char: {
    char *data = malloc(sizeof(char));
    PYERR_MEM(data);
    PyObject *tmp = PyNumber_Long(arg);
    if (tmp == NULL) {
      free(data);
      return NULL;
    }
    *data = PyLong_AsLongLong(tmp);
    Py_DECREF(tmp);
    *should_free = true;
    return data;
  }

  case CXX_CharPointer:
    if (Py_TYPE(arg) == &py_c_char_type || Py_TYPE(arg) == &py_c_void_type) {
      return &((PyC_c_char *)arg)->pointer;
    } else {
      PyErr_SetString(PyExc_TypeError, "Expected c_char got other type");
      return NULL;
    }

  case CXX_Bool: {
    int x = PyObject_IsTrue(arg);
    if (x == -1) {
      return NULL;
    } else if (x > 0) {
      bool *data = malloc(sizeof(bool));
      PYERR_MEM(data);
      *should_free = true;
      *data        = true;
      return data;
    } else {
      bool *data = malloc(sizeof(bool));
      PYERR_MEM(data);
      *should_free = true;
      *data        = false;
      return data;
    }
  }

  case CXX_BoolPointer:
    if (Py_TYPE(arg) == &py_c_bool_type || Py_TYPE(arg) == &py_c_void_type) {
      return &((PyC_c_bool *)arg)->pointer;
    } else {
      PyErr_SetString(PyExc_TypeError, "Expected c_bool got other type");
      return NULL;
    }

  case CXX_Short: {
    short *data = malloc(sizeof(short));
    PYERR_MEM(data);
    PyObject *tmp = PyNumber_Long(arg);
    if (tmp == NULL) {
      free(data);
      return NULL;
    }
    *data = PyLong_AsLongLong(tmp);
    Py_DECREF(tmp);
    *should_free = true;
    return data;
  }

  case CXX_ShortPointer:
    if (Py_TYPE(arg) == &py_c_short_type || Py_TYPE(arg) == &py_c_void_type) {
      return &((PyC_c_short *)arg)->pointer;
    } else {
      PyErr_SetString(PyExc_TypeError, "Expected c_short got other type");
      return NULL;
    }

  case CXX_UShort: {
    unsigned short *data = malloc(sizeof(unsigned short));
    PYERR_MEM(data);
    PyObject *tmp = PyNumber_Long(arg);
    if (tmp == NULL) {
      free(data);
      return NULL;
    }
    *data = PyLong_AsUnsignedLongLong(tmp);
    Py_DECREF(tmp);
    *should_free = true;
    return data;
  }

  case CXX_UShortPointer:
    if (Py_TYPE(arg) == &py_c_ushort_type || Py_TYPE(arg) == &py_c_void_type) {
      return &((PyC_c_ushort *)arg)->pointer;
    } else {
      PyErr_SetString(PyExc_TypeError, "Expected c_ushort got other type");
      return NULL;
    }

  case CXX_Int: {
    int *data = malloc(sizeof(int));
    PYERR_MEM(data);
    PyObject *tmp = PyNumber_Long(arg);
    if (tmp == NULL) {
      free(data);
      return NULL;
    }
    *data = PyLong_AsLongLong(tmp);
    Py_DECREF(tmp);
    *should_free = true;
    return data;
  }

  case CXX_IntPointer:
    if (Py_TYPE(arg) == &py_c_int_type || Py_TYPE(arg) == &py_c_void_type) {
      return &((PyC_c_int *)arg)->pointer;
    } else {
      PyErr_SetString(PyExc_TypeError, "Expected c_int got other type");
      return NULL;
    }

  case CXX_UInt: {
    unsigned int *data = malloc(sizeof(unsigned int));
    PYERR_MEM(data);
    PyObject *tmp = PyNumber_Long(arg);
    if (tmp == NULL) {
      free(data);
      return NULL;
    }
    *data = PyLong_AsUnsignedLongLong(tmp);
    Py_DECREF(tmp);
    *should_free = true;
    return data;
  }

  case CXX_UIntPointer:
    if (Py_TYPE(arg) == &py_c_uint_type || Py_TYPE(arg) == &py_c_void_type) {
      return &((PyC_c_uint *)arg)->pointer;
    } else {
      PyErr_SetString(PyExc_TypeError, "Expected c_uint got other type");
      return NULL;
    }

  case CXX_Long: {
    long *data = malloc(sizeof(long));
    PYERR_MEM(data);
    PyObject *tmp = PyNumber_Long(arg);
    if (tmp == NULL) {
      free(data);
      return NULL;
    }
    *data = PyLong_AsLongLong(tmp);
    Py_DECREF(tmp);
    *should_free = true;
    return data;
  }

  case CXX_LongPointer:
    if (Py_TYPE(arg) == &py_c_long_type || Py_TYPE(arg) == &py_c_void_type) {
      return &((PyC_c_long *)arg)->pointer;
    } else {
      PyErr_SetString(PyExc_TypeError, "Expected c_long got other type");
      return NULL;
    }

  case CXX_ULong: {
    unsigned long *data = malloc(sizeof(unsigned long));
    PYERR_MEM(data);
    PyObject *tmp = PyNumber_Long(arg);
    if (tmp == NULL) {
      free(data);
      return NULL;
    }
    *data = PyLong_AsUnsignedLongLong(tmp);
    Py_DECREF(tmp);
    *should_free = true;
    return data;
  }

  case CXX_ULongPointer:
    if (Py_TYPE(arg) == &py_c_ulong_type || Py_TYPE(arg) == &py_c_void_type) {
      return &((PyC_c_ulong *)arg)->pointer;
    } else {
      PyErr_SetString(PyExc_TypeError, "Expected c_ulong got other type");
      return NULL;
    }

  case CXX_Float: {
    float *data = malloc(sizeof(float));
    PYERR_MEM(data);
    PyObject *tmp = PyNumber_Float(arg);
    if (tmp == NULL) {
      free(data);
      return NULL;
    }
    *data = PyFloat_AsDouble(arg);
    Py_DECREF(tmp);
    *should_free = true;
    return data;
  }

  case CXX_FloatPointer:
    if (Py_TYPE(arg) == &py_c_float_type || Py_TYPE(arg) == &py_c_void_type) {
      return &((PyC_c_float *)arg)->pointer;
    } else {
      PyErr_SetString(PyExc_TypeError, "Expected c_float got other type");
      return NULL;
    }

  case CXX_Double: {
    double *data = malloc(sizeof(double));
    PYERR_MEM(data);
    PyObject *tmp = PyNumber_Float(arg);
    if (tmp == NULL) {
      free(data);
      return NULL;
    }
    *data = PyFloat_AsDouble(arg);
    Py_DECREF(tmp);
    *should_free = true;
    return data;
  }

  case CXX_DoublePointer:
    if (Py_TYPE(arg) == &py_c_double_type || Py_TYPE(arg) == &py_c_void_type) {
      return &((PyC_c_double *)arg)->pointer;
    } else {
      PyErr_SetString(PyExc_TypeError, "Expected c_double got other type");
      return NULL;
    }

  case CXX_Struct:
    if (PyObject_IsInstance(arg, (PyObject *)&py_c_struct_type)) {
      PyC_c_struct *py_struct_type = (PyC_c_struct *)arg;
      void *data = malloc(py_struct_type->structure->structSize);
      PYERR_MEM(data);
      *should_free = true;
      memcpy(data,
             py_struct_type->pointer,
             py_struct_type->structure->structSize);
      return data;
    } else {
      PyErr_SetString(PyExc_TypeError, "Expected c_struct got other type");
      return NULL;
    }

  case CXX_StructPointer:
    if (PyObject_IsInstance(arg, (PyObject *)&py_c_struct_type)) {
      return &((PyC_c_void *)arg)->pointer;
    } else {
      PyErr_SetString(PyExc_TypeError, "Expected c_struct got other type");
      return NULL;
    }

  case CXX_Union:
    if (PyObject_IsInstance(arg, (PyObject *)&py_c_union_type)) {
      PyC_c_union *py_union_type = (PyC_c_union *)arg;
      void *data                 = malloc(py_union_type->u->unionSize);
      PYERR_MEM(data);
      *should_free = true;
      memcpy(data, py_union_type->pointer, py_union_type->u->unionSize);
      return data;
    } else {
      PyErr_SetString(PyExc_TypeError, "Expected c_union got other type");
      return NULL;
    }

  case CXX_UnionPointer:
    if (PyObject_IsInstance(arg, (PyObject *)&py_c_union_type)) {
      return &((PyC_c_void *)arg)->pointer;
    } else {
      PyErr_SetString(PyExc_TypeError, "Expected c_union got other type");
      return NULL;
    }

  default:
    PyErr_SetString(py_BindingError,
                    "Internal Error which converting python type to cpp type");
    return NULL;
  }
}

PyObject *cppArg_to_pyArg(void *arg,
                          enum CXX_Type type,
                          void *extra_type_info,
                          PyObject *module) {
  switch (type) {

  case CXX_VoidPointer: {
    PyObject *py_arg = _PyObject_New(&py_c_void_type);
    if (py_arg == NULL) {
      return NULL;
    }
    ((PyC_c_void *)py_arg)->pointer       = *(void **)arg;
    ((PyC_c_void *)py_arg)->freeOnDel     = false;
    ((PyC_c_void *)py_arg)->isPointer     = false;
    ((PyC_c_void *)py_arg)->isArray       = false;
    ((PyC_c_void *)py_arg)->arrayCapacity = 0;
    ((PyC_c_void *)py_arg)->arraySize     = 0;
    ((PyC_c_void *)py_arg)->_i            = 0;

    return py_arg;
  }

  case CXX_Char:
    return PyLong_FromLongLong(*(char *)arg);

  case CXX_CharPointer: {
    PyObject *py_arg = _PyObject_New(&py_c_char_type);
    if (py_arg == NULL) {
      return NULL;
    }
    ((PyC_c_char *)py_arg)->pointer       = *(char **)arg;
    ((PyC_c_char *)py_arg)->value         = *((PyC_c_char *)py_arg)->pointer;
    ((PyC_c_char *)py_arg)->freeOnDel     = false;
    ((PyC_c_char *)py_arg)->isPointer     = false;
    ((PyC_c_char *)py_arg)->isArray       = false;
    ((PyC_c_char *)py_arg)->arrayCapacity = 0;
    ((PyC_c_char *)py_arg)->arraySize     = 0;
    ((PyC_c_char *)py_arg)->_i            = 0;

    return py_arg;
  }

  case CXX_Bool:
    if (*(bool *)arg == false) {
      Py_RETURN_FALSE;
    } else {
      Py_RETURN_TRUE;
    }

  case CXX_BoolPointer: {
    PyObject *py_arg = _PyObject_New(&py_c_bool_type);
    if (py_arg == NULL) {
      return NULL;
    }
    ((PyC_c_bool *)py_arg)->pointer       = *(bool **)arg;
    ((PyC_c_bool *)py_arg)->value         = *((PyC_c_bool *)py_arg)->pointer;
    ((PyC_c_bool *)py_arg)->freeOnDel     = false;
    ((PyC_c_bool *)py_arg)->isPointer     = false;
    ((PyC_c_bool *)py_arg)->isArray       = false;
    ((PyC_c_bool *)py_arg)->arrayCapacity = 0;
    ((PyC_c_bool *)py_arg)->arraySize     = 0;
    ((PyC_c_bool *)py_arg)->_i            = 0;

    return py_arg;
  }

  case CXX_Short:
    return PyLong_FromLongLong(*(short *)arg);

  case CXX_ShortPointer: {
    PyObject *py_arg = _PyObject_New(&py_c_short_type);
    if (py_arg == NULL) {
      return NULL;
    }
    ((PyC_c_short *)py_arg)->pointer       = *(short **)arg;
    ((PyC_c_short *)py_arg)->value         = *((PyC_c_short *)py_arg)->pointer;
    ((PyC_c_short *)py_arg)->freeOnDel     = false;
    ((PyC_c_short *)py_arg)->isPointer     = false;
    ((PyC_c_short *)py_arg)->isArray       = false;
    ((PyC_c_short *)py_arg)->arrayCapacity = 0;
    ((PyC_c_short *)py_arg)->arraySize     = 0;
    ((PyC_c_short *)py_arg)->_i            = 0;

    return py_arg;
  }

  case CXX_UShort:
    return PyLong_FromUnsignedLongLong(*(unsigned short *)arg);

  case CXX_UShortPointer: {
    PyObject *py_arg = _PyObject_New(&py_c_ushort_type);
    if (py_arg == NULL) {
      return NULL;
    }
    ((PyC_c_ushort *)py_arg)->pointer   = *(unsigned short **)arg;
    ((PyC_c_ushort *)py_arg)->value     = *((PyC_c_ushort *)py_arg)->pointer;
    ((PyC_c_ushort *)py_arg)->freeOnDel = false;
    ((PyC_c_ushort *)py_arg)->isPointer = false;
    ((PyC_c_ushort *)py_arg)->isArray   = false;
    ((PyC_c_ushort *)py_arg)->arrayCapacity = 0;
    ((PyC_c_ushort *)py_arg)->arraySize     = 0;
    ((PyC_c_ushort *)py_arg)->_i            = 0;

    return py_arg;
  }

  case CXX_Int:
    return PyLong_FromLongLong(*(int *)arg);

  case CXX_IntPointer: {
    PyObject *py_arg = _PyObject_New(&py_c_int_type);
    if (py_arg == NULL) {
      return NULL;
    }
    ((PyC_c_int *)py_arg)->pointer       = *(int **)arg;
    ((PyC_c_int *)py_arg)->value         = *((PyC_c_int *)py_arg)->pointer;
    ((PyC_c_int *)py_arg)->freeOnDel     = false;
    ((PyC_c_int *)py_arg)->isPointer     = false;
    ((PyC_c_int *)py_arg)->isArray       = false;
    ((PyC_c_int *)py_arg)->arrayCapacity = 0;
    ((PyC_c_int *)py_arg)->arraySize     = 0;
    ((PyC_c_int *)py_arg)->_i            = 0;

    return py_arg;
  }

  case CXX_UInt:
    return PyLong_FromUnsignedLongLong(*(unsigned int *)arg);

  case CXX_UIntPointer: {
    PyObject *py_arg = _PyObject_New(&py_c_uint_type);
    if (py_arg == NULL) {
      return NULL;
    }
    ((PyC_c_uint *)py_arg)->pointer       = *(unsigned int **)arg;
    ((PyC_c_uint *)py_arg)->value         = *((PyC_c_uint *)py_arg)->pointer;
    ((PyC_c_uint *)py_arg)->freeOnDel     = false;
    ((PyC_c_uint *)py_arg)->isPointer     = false;
    ((PyC_c_uint *)py_arg)->isArray       = false;
    ((PyC_c_uint *)py_arg)->arrayCapacity = 0;
    ((PyC_c_uint *)py_arg)->arraySize     = 0;
    ((PyC_c_uint *)py_arg)->_i            = 0;

    return py_arg;
  }

  case CXX_Long:
    return PyLong_FromUnsignedLongLong(*(long *)arg);

  case CXX_LongPointer: {
    PyObject *py_arg = _PyObject_New(&py_c_long_type);
    if (py_arg == NULL) {
      return NULL;
    }
    ((PyC_c_long *)py_arg)->pointer       = *(long **)arg;
    ((PyC_c_long *)py_arg)->value         = *((PyC_c_long *)py_arg)->pointer;
    ((PyC_c_long *)py_arg)->freeOnDel     = false;
    ((PyC_c_long *)py_arg)->isPointer     = false;
    ((PyC_c_long *)py_arg)->isArray       = false;
    ((PyC_c_long *)py_arg)->arrayCapacity = 0;
    ((PyC_c_long *)py_arg)->arraySize     = 0;
    ((PyC_c_long *)py_arg)->_i            = 0;

    return py_arg;
  }

  case CXX_ULong:
    return PyLong_FromUnsignedLongLong(*(unsigned long *)arg);

  case CXX_ULongPointer: {
    PyObject *py_arg = _PyObject_New(&py_c_ulong_type);
    if (py_arg == NULL) {
      return NULL;
    }
    ((PyC_c_ulong *)py_arg)->pointer       = *(unsigned long **)arg;
    ((PyC_c_ulong *)py_arg)->value         = *((PyC_c_ulong *)py_arg)->pointer;
    ((PyC_c_ulong *)py_arg)->freeOnDel     = false;
    ((PyC_c_ulong *)py_arg)->isPointer     = false;
    ((PyC_c_ulong *)py_arg)->isArray       = false;
    ((PyC_c_ulong *)py_arg)->arrayCapacity = 0;
    ((PyC_c_ulong *)py_arg)->arraySize     = 0;
    ((PyC_c_ulong *)py_arg)->_i            = 0;

    return py_arg;
  }

  case CXX_Float:
    return PyFloat_FromDouble(*(float *)arg);

  case CXX_FloatPointer: {
    PyObject *py_arg = _PyObject_New(&py_c_float_type);
    if (py_arg == NULL) {
      return NULL;
    }
    ((PyC_c_float *)py_arg)->pointer       = *(float **)arg;
    ((PyC_c_float *)py_arg)->value         = *((PyC_c_float *)py_arg)->pointer;
    ((PyC_c_float *)py_arg)->freeOnDel     = false;
    ((PyC_c_float *)py_arg)->isPointer     = false;
    ((PyC_c_float *)py_arg)->isArray       = false;
    ((PyC_c_float *)py_arg)->arrayCapacity = 0;
    ((PyC_c_float *)py_arg)->arraySize     = 0;
    ((PyC_c_float *)py_arg)->_i            = 0;

    return py_arg;
  }

  case CXX_Double:
    return PyFloat_FromDouble(*(double *)arg);

  case CXX_DoublePointer: {
    PyObject *py_arg = _PyObject_New(&py_c_double_type);
    if (py_arg == NULL) {
      return NULL;
    }
    ((PyC_c_double *)py_arg)->pointer   = *(double **)arg;
    ((PyC_c_double *)py_arg)->value     = *((PyC_c_double *)py_arg)->pointer;
    ((PyC_c_double *)py_arg)->freeOnDel = false;
    ((PyC_c_double *)py_arg)->isPointer = false;
    ((PyC_c_double *)py_arg)->isArray   = false;
    ((PyC_c_double *)py_arg)->arrayCapacity = 0;
    ((PyC_c_double *)py_arg)->arraySize     = 0;
    ((PyC_c_double *)py_arg)->_i            = 0;

    return py_arg;
  }

  case CXX_Struct: {
    PyObject *py_arg = _PyObject_New(&py_c_struct_type);
    if (py_arg == NULL) {
      return NULL;
    }

    ((PyC_c_struct *)py_arg)->pointer       = arg;
    ((PyC_c_struct *)py_arg)->freeOnDel     = false;
    ((PyC_c_struct *)py_arg)->isPointer     = false;
    ((PyC_c_struct *)py_arg)->isArray       = false;
    ((PyC_c_struct *)py_arg)->arrayCapacity = 0;
    ((PyC_c_struct *)py_arg)->arraySize     = 0;
    ((PyC_c_struct *)py_arg)->_i            = 0;
    ((PyC_c_struct *)py_arg)->structure     = extra_type_info;
    Py_INCREF(module);
    ((PyC_c_struct *)py_arg)->parentModule = module;
  }

  case CXX_StructPointer: {
    PyObject *py_arg = _PyObject_New(&py_c_struct_type);
    if (py_arg == NULL) {
      return NULL;
    }

    ((PyC_c_struct *)py_arg)->pointer       = *(void **)arg;
    ((PyC_c_struct *)py_arg)->freeOnDel     = false;
    ((PyC_c_struct *)py_arg)->isPointer     = false;
    ((PyC_c_struct *)py_arg)->isArray       = false;
    ((PyC_c_struct *)py_arg)->arrayCapacity = 0;
    ((PyC_c_struct *)py_arg)->arraySize     = 0;
    ((PyC_c_struct *)py_arg)->_i            = 0;
    ((PyC_c_struct *)py_arg)->structure     = extra_type_info;
    Py_INCREF(module);
    ((PyC_c_struct *)py_arg)->parentModule = module;
  }
  case CXX_Union: {
    PyObject *py_arg = _PyObject_New(&py_c_union_type);
    if (py_arg == NULL) {
      return NULL;
    }

    ((PyC_c_union *)py_arg)->pointer       = arg;
    ((PyC_c_union *)py_arg)->freeOnDel     = false;
    ((PyC_c_union *)py_arg)->isPointer     = false;
    ((PyC_c_union *)py_arg)->isArray       = false;
    ((PyC_c_union *)py_arg)->arrayCapacity = 0;
    ((PyC_c_union *)py_arg)->arraySize     = 0;
    ((PyC_c_union *)py_arg)->_i            = 0;
    ((PyC_c_union *)py_arg)->u             = extra_type_info;
    Py_INCREF(module);
    ((PyC_c_struct *)py_arg)->parentModule = module;
  }
  case CXX_UnionPointer: {
    PyObject *py_arg = _PyObject_New(&py_c_union_type);
    if (py_arg == NULL) {
      return NULL;
    }

    ((PyC_c_union *)py_arg)->pointer       = *(void **)arg;
    ((PyC_c_union *)py_arg)->freeOnDel     = false;
    ((PyC_c_union *)py_arg)->isPointer     = false;
    ((PyC_c_union *)py_arg)->isArray       = false;
    ((PyC_c_union *)py_arg)->arrayCapacity = 0;
    ((PyC_c_union *)py_arg)->arraySize     = 0;
    ((PyC_c_union *)py_arg)->_i            = 0;
    ((PyC_c_union *)py_arg)->u             = extra_type_info;
    Py_INCREF(module);
    ((PyC_c_struct *)py_arg)->parentModule = module;
  }
  default:
    PyErr_SetString(py_BindingError,
                    "Internal Error while converting cpp type to python type");
    return NULL;
  }
}

int match_ffi_type_to_defination(Function *funcs, PyObject *args) {
  int func_num = -1;

  Py_ssize_t args_count = PyTuple_Size(args);

  for (int i = 0; i < funcs->funcCount; i++) {
    FunctionType *func_type =
        FunctionType_array_get_ptr_at(funcs->functionTypes, i);

    if (args_count == func_type->argsCount) {
      func_num = i;

      for (int j = 0; j < args_count; j++) {

        PyObject *py_arg = PyTuple_GetItem(args, j);
        enum CXX_Type arg_type =
            CXX_Type_array_getat(func_type->argsTypeCXX, j);

        switch (arg_type) {
        case CXX_VoidPointer:
          if (PyObject_IsInstance(py_arg, (PyObject *)&py_c_type_type)) {
            func_num = -1;
          }
          break;
        case CXX_CharPointer:
          if (Py_TYPE(py_arg) != &py_c_char_type &&
              Py_TYPE(py_arg) != &py_c_void_type) {
            func_num = -1;
          }
          break;
        case CXX_Bool:
          if (PyBool_Check(py_arg) == 0) {
            func_num = -1;
          }
          break;
        case CXX_BoolPointer:
          if (Py_TYPE(py_arg) != &py_c_bool_type &&
              Py_TYPE(py_arg) != &py_c_void_type) {
            func_num = -1;
          }
          break;
        case CXX_ShortPointer:
          if (Py_TYPE(py_arg) != &py_c_short_type &&
              Py_TYPE(py_arg) != &py_c_void_type) {
            func_num = -1;
          }
          break;
        case CXX_UShortPointer:
          if (Py_TYPE(py_arg) != &py_c_ushort_type &&
              Py_TYPE(py_arg) != &py_c_void_type) {
            func_num = -1;
          }
          break;
        case CXX_IntPointer:
          if (Py_TYPE(py_arg) != &py_c_int_type &&
              Py_TYPE(py_arg) != &py_c_void_type) {
            func_num = -1;
          }
          break;
        case CXX_UIntPointer:
          if (Py_TYPE(py_arg) != &py_c_uint_type &&
              Py_TYPE(py_arg) != &py_c_void_type) {
            func_num = -1;
          }
          break;
        case CXX_LongPointer:
          if (Py_TYPE(py_arg) != &py_c_long_type &&
              Py_TYPE(py_arg) != &py_c_void_type) {
            func_num = -1;
          }
          break;
        case CXX_ULongPointer:
          if (Py_TYPE(py_arg) != &py_c_ulong_type &&
              Py_TYPE(py_arg) != &py_c_void_type) {
            func_num = -1;
          }
          break;
        case CXX_FloatPointer:
          if (Py_TYPE(py_arg) != &py_c_float_type &&
              Py_TYPE(py_arg) != &py_c_void_type) {
            func_num = -1;
          }
          break;
        case CXX_DoublePointer:
          if (Py_TYPE(py_arg) != &py_c_double_type &&
              Py_TYPE(py_arg) != &py_c_void_type) {
            func_num = -1;
          }
          break;
        case CXX_Struct:
          if (Py_TYPE(py_arg) != &py_c_struct_type) {
            func_num = -1;
          }
          break;
        case CXX_StructPointer:
          if (Py_TYPE(py_arg) != &py_c_struct_type &&
              Py_TYPE(py_arg) != &py_c_void_type) {
            func_num = -1;
          }
          break;
        case CXX_Union:
          if (Py_TYPE(py_arg) != &py_c_union_type) {
            func_num = -1;
          }
          break;
        case CXX_UnionPointer:
          if (Py_TYPE(py_arg) != &py_c_union_type &&
              Py_TYPE(py_arg) != &py_c_void_type) {
            func_num = -1;
          }
          break;
        case CXX_Char:
        case CXX_Short:
        case CXX_UShort:
        case CXX_Int:
        case CXX_UInt:
        case CXX_Long:
        case CXX_ULong:
          if (PyNumber_Check(py_arg) == 0) {
            func_num = -1;
          }
          break;
        case CXX_Float:
        case CXX_Double:
          if (PyFloat_Check(py_arg) == 0) {
            func_num = -1;
          }
          break;
        default:
          func_num = -1;
        }
      }

      if (func_num != -1) {
        return func_num;
      }

    } else {
      func_num = -1;
    }
  }
  return func_num;
}

char *CXX_Type_TO_char_p(enum CXX_Type type, const void *extraInfo) {
  switch (type) {
  case CXX_Bool:
    return "bool";
  case CXX_BoolPointer:
    return "bool*";
  case CXX_Void:
    return "void";
  case CXX_VoidPointer:
    return "void*";
  case CXX_Char:
    return "char";
  case CXX_CharPointer:
    return "char*";
  case CXX_UChar:
    return "unsigned char";
  case CXX_UCharPointer:
    return "unsigned char*";
  case CXX_Short:
    return "short";
  case CXX_ShortPointer:
    return "short*";
  case CXX_UShort:
    return "unsigned short";
  case CXX_UShortPointer:
    return "unsigned short*";
  case CXX_Int:
    return "int";
  case CXX_IntPointer:
    return "int*";
  case CXX_UInt:
    return "unsigned int";
  case CXX_UIntPointer:
    return "unsigned int*";
  case CXX_Long:
    return "long";
  case CXX_LongPointer:
    return "long*";
  case CXX_ULong:
    return "unsigned long";
  case CXX_ULongPointer:
    return "unsigned long*";
  case CXX_LongLong:
    return "long long";
  case CXX_LongLongPointer:
    return "long long*";
  case CXX_ULongLong:
    return "unsigned long long";
  case CXX_ULongLongPointer:
    return "unsigned long long*";
  case CXX_Float:
    return "float";
  case CXX_FloatPointer:
    return "float*";
  case CXX_Double:
    return "double";
  case CXX_DoublePointer:
    return "double*";
  case CXX_Struct: // TODO: specify which struct
    return "struct";
    // Structure *s = extraInfo;
    // size_t len = strlen(s->name);
    // char *result = malloc(len + 2);
    // strcpy(result, s->name);
    // strcat(result, "*");
    // return result;
  case CXX_StructPointer:
    return "struct*";
  case CXX_Union:
    return "union";
  case CXX_UnionPointer:
    return "union*";
  default:
    // printf("%i", type);
    return "Could not figure out";
  }
}
