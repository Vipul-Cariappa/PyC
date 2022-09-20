#define PY_SSIZE_T_CLEAN
#include "DataStructures.h"
#include "PyC.h"
#include "Py_C_Types.h"
#include "Python.h"
#include "ffi.h"
#include "clang-c/Index.h"

#define MAX_SIZE 1024

void *pyArg_to_cppArg(PyObject *arg, ffi_type type, bool *should_free) {
  void *data = NULL;

  switch (type.type) {
  case FFI_TYPE_SINT8:
  case FFI_TYPE_UINT8: {
    data = malloc(1);
    *should_free = true;
    int8_t i = (int8_t)PyLong_AsLong(arg);
    memcpy(data, &i, 1);
    break;
  }
  case FFI_TYPE_SINT16:
  case FFI_TYPE_UINT16: {
    data = malloc(2);
    *should_free = true;
    int16_t i = (int16_t)PyLong_AsLong(arg);
    memcpy(data, &i, 2);
    break;
  }
  case FFI_TYPE_INT:
  case FFI_TYPE_SINT32:
  case FFI_TYPE_UINT32: {
    data = malloc(4);
    *should_free = true;
    int32_t i = (int32_t)PyLong_AsLong(arg);
    memcpy(data, &i, 4);
    break;
  }
  case FFI_TYPE_SINT64:
  case FFI_TYPE_UINT64: {
    data = malloc(8);
    *should_free = true;
    int64_t i = (int64_t)PyLong_AsLong(arg);
    memcpy(data, &i, 8);
    break;
  }
  case FFI_TYPE_FLOAT: {
    data = malloc(sizeof(float));
    *should_free = true;
    float i = (float)PyFloat_AsDouble(arg);
    memcpy(data, &i, sizeof(float));
    break;
  }
  case FFI_TYPE_DOUBLE: {
    data = malloc(sizeof(double));
    *should_free = true;
    double i = (double)PyFloat_AsDouble(arg);
    memcpy(data, &i, sizeof(double));
    break;
  }
  case FFI_TYPE_POINTER:
    if ((PyObject_IsInstance(arg, (PyObject *)&py_c_int_type)) ||
        (PyObject_IsInstance(arg, (PyObject *)&py_c_uint_type))) {
      data = &((PyC_c_int *)arg)->pointer;
      break;

    } else if ((PyObject_IsInstance(arg, (PyObject *)&py_c_short_type)) ||
               (PyObject_IsInstance(arg, (PyObject *)&py_c_ushort_type))) {
      data = &((PyC_c_short *)arg)->pointer;
      break;
    } else if ((PyObject_IsInstance(arg, (PyObject *)&py_c_long_type)) ||
               (PyObject_IsInstance(arg, (PyObject *)&py_c_ulong_type))) {
      data = &((PyC_c_long *)arg)->pointer;
      break;
    } else if (PyObject_IsInstance(arg, (PyObject *)&py_c_double_type)) {
      data = &((PyC_c_double *)arg)->pointer;
      break;
    } else if (PyObject_IsInstance(arg, (PyObject *)&py_c_float_type)) {
      data = &((PyC_c_float *)arg)->pointer;
      break;
    } else if (PyObject_IsInstance(arg, (PyObject *)&py_c_struct_type)) {
      data = &((PyC_c_struct *)arg)->pointer;
      break;
    } else if (PyObject_IsInstance(arg, (PyObject *)&py_c_union_type)) {
      data = &((PyC_c_union *)arg)->pointer;
      break;
    } else { // assume char*; TODO: check type info
      data = (void *)PyUnicode_AsUTF8(arg);
      *should_free = true;
      break;
    }
  case FFI_TYPE_STRUCT: {
    if (PyObject_IsInstance(arg, (PyObject *)&py_c_struct_type)) {
      PyC_c_struct *py_struct_type = (PyC_c_struct *)arg;
      data = malloc(py_struct_type->structure->structSize);
      *should_free = true;
      memcpy(data, py_struct_type->pointer,
             py_struct_type->structure->structSize);
      break;
    }

    if (PyObject_IsInstance(arg, (PyObject *)&py_c_union_type)) {
      PyC_c_union *py_union_type = (PyC_c_union *)arg;
      data = malloc(py_union_type->u->unionSize);
      *should_free = true;
      memcpy(data, py_union_type->pointer, py_union_type->u->unionSize);
      break;
    }
  }
  default:
    PyErr_SetString(py_BindingError,
                    "Could not convert Cpp type to Python type");
  }
  return data;
}

PyObject *cppArg_to_pyArg(void *arg, ffi_type type,
                          enum CXTypeKind underlying_type,
                          Structure *underlying_struct, Union *underlying_union,
                          PyObject *module) {
  switch (type.type) {
  case FFI_TYPE_INT:
    return PyLong_FromLongLong(*(int *)arg);
  case FFI_TYPE_SINT8:
    return PyLong_FromLongLong(*(int8_t *)arg);
  case FFI_TYPE_UINT8:
    return PyLong_FromLongLong(*(uint8_t *)arg);
  case FFI_TYPE_SINT16:
    return PyLong_FromLongLong(*(int16_t *)arg);
  case FFI_TYPE_UINT16:
    return PyLong_FromLongLong(*(uint16_t *)arg);
  case FFI_TYPE_SINT32:
    return PyLong_FromLongLong(*(int32_t *)arg);
  case FFI_TYPE_UINT32:
    return PyLong_FromLongLong(*(uint32_t *)arg);
  case FFI_TYPE_SINT64:
    return PyLong_FromLongLong(*(int64_t *)arg);
  case FFI_TYPE_UINT64:
    return PyLong_FromLongLong(*(uint64_t *)arg);
  case FFI_TYPE_FLOAT:
    return PyFloat_FromDouble(*(float *)arg);
  case FFI_TYPE_DOUBLE:
    return PyFloat_FromDouble(*(double *)arg);
  case FFI_TYPE_POINTER:
    switch (underlying_type) {
    case CXType_Int: {
      PyObject *obj = PyObject_GetAttrString(PyC, "c_int");
      if (!obj) {
        return NULL;
      }

      PyObject *args = PyTuple_New(1);
      PyObject *num = PyLong_FromLongLong(0);
      if (!num && !args) {
        return NULL;
      }

      PyTuple_SetItem(args, 0, num);

      PyObject *result = PyObject_Call(obj, args, NULL);
      if (!result) {
        Py_DECREF(obj);
        Py_DECREF(args);
        return NULL;
      }

      PyC_c_int *c_int = (PyC_c_int *)result;
      c_int->pointer = *(int **)arg;
      c_int->value = *(c_int->pointer);

      Py_DECREF(obj);
      Py_DECREF(args);
      return result;
    }
    case CXType_UInt: {
      PyObject *obj = PyObject_GetAttrString(PyC, "c_uint");
      if (!obj) {
        return NULL;
      }

      PyObject *args = PyTuple_New(1);
      PyObject *num = PyLong_FromLongLong(0);
      if (!num && !args) {
        return NULL;
      }

      PyTuple_SetItem(args, 0, num);

      PyObject *result = PyObject_Call(obj, args, NULL);
      if (!result) {
        Py_DECREF(obj);
        Py_DECREF(args);
        return NULL;
      }

      PyC_c_int *c_int = (PyC_c_int *)result;
      c_int->pointer = *(int **)arg;
      c_int->value = *(c_int->pointer);

      Py_DECREF(obj);
      Py_DECREF(args);
      return result;
    }
    case CXType_Long: {
      PyObject *obj = PyObject_GetAttrString(PyC, "c_long");
      if (!obj) {
        return NULL;
      }

      PyObject *args = PyTuple_New(1);
      PyObject *num = PyLong_FromLongLong(0);
      if (!num && !args) {
        return NULL;
      }

      PyTuple_SetItem(args, 0, num);

      PyObject *result = PyObject_Call(obj, args, NULL);
      if (!result) {
        Py_DECREF(obj);
        Py_DECREF(args);
        return NULL;
      }

      PyC_c_long *c_long = (PyC_c_long *)result;
      c_long->pointer = *(long **)arg;
      c_long->value = *(c_long->pointer);

      Py_DECREF(obj);
      Py_DECREF(args);
      return result;
    }
    case CXType_ULong: {
      PyObject *obj = PyObject_GetAttrString(PyC, "c_ulong");
      if (!obj) {
        return NULL;
      }

      PyObject *args = PyTuple_New(1);
      PyObject *num = PyLong_FromLongLong(0);
      if (!num && !args) {
        return NULL;
      }

      PyTuple_SetItem(args, 0, num);

      PyObject *result = PyObject_Call(obj, args, NULL);
      if (!result) {
        Py_DECREF(obj);
        Py_DECREF(args);
        return NULL;
      }

      PyC_c_long *c_long = (PyC_c_long *)result;
      c_long->pointer = *(long **)arg;
      c_long->value = *(c_long->pointer);

      Py_DECREF(obj);
      Py_DECREF(args);
      return result;
    }
    case CXType_Short: {
      PyObject *obj = PyObject_GetAttrString(PyC, "c_short");
      if (!obj) {
        return NULL;
      }

      PyObject *args = PyTuple_New(1);
      PyObject *num = PyLong_FromLongLong(0);
      if (!num && !args) {
        return NULL;
      }

      PyTuple_SetItem(args, 0, num);

      PyObject *result = PyObject_Call(obj, args, NULL);
      if (!result) {
        Py_DECREF(obj);
        Py_DECREF(args);
        return NULL;
      }

      PyC_c_short *c_short = (PyC_c_short *)result;
      c_short->pointer = *(short **)arg;
      c_short->value = *(c_short->pointer);

      Py_DECREF(obj);
      Py_DECREF(args);
      return result;
    }
    case CXType_UShort: {
      PyObject *obj = PyObject_GetAttrString(PyC, "c_ushort");
      if (!obj) {
        return NULL;
      }

      PyObject *args = PyTuple_New(1);
      PyObject *num = PyLong_FromLongLong(0);
      if (!num && !args) {
        return NULL;
      }

      PyTuple_SetItem(args, 0, num);

      PyObject *result = PyObject_Call(obj, args, NULL);
      if (!result) {
        Py_DECREF(obj);
        Py_DECREF(args);
        return NULL;
      }

      PyC_c_short *c_short = (PyC_c_short *)result;
      c_short->pointer = *(short **)arg;
      c_short->value = *(c_short->pointer);

      Py_DECREF(obj);
      Py_DECREF(args);
      return result;
    }
    case CXType_Float: {
      PyObject *obj = PyObject_GetAttrString(PyC, "c_float");
      if (!obj) {
        return NULL;
      }

      PyObject *args = PyTuple_New(1);
      PyObject *num = PyLong_FromLongLong(0);
      if (!num && !args) {
        return NULL;
      }

      PyTuple_SetItem(args, 0, num);

      PyObject *result = PyObject_Call(obj, args, NULL);
      if (!result) {
        Py_DECREF(obj);
        Py_DECREF(args);
        return NULL;
      }

      PyC_c_float *c_float = (PyC_c_float *)result;
      c_float->pointer = *(float **)arg;
      c_float->value = *(c_float->pointer);

      Py_DECREF(obj);
      Py_DECREF(args);
      return result;
    }
    case CXType_Double: {
      PyObject *obj = PyObject_GetAttrString(PyC, "c_double");
      if (!obj) {
        return NULL;
      }

      PyObject *args = PyTuple_New(1);
      PyObject *num = PyLong_FromLongLong(0);
      if (!num && !args) {
        return NULL;
      }

      PyTuple_SetItem(args, 0, num);

      PyObject *result = PyObject_Call(obj, args, NULL);
      if (!result) {
        Py_DECREF(obj);
        Py_DECREF(args);
        return NULL;
      }

      PyC_c_double *c_double = (PyC_c_double *)result;
      c_double->pointer = *(double **)arg;
      c_double->value = *(c_double->pointer);

      Py_DECREF(obj);
      Py_DECREF(args);
      return result;
    }
    case CXType_Char_U:
    case CXType_Char_S: {
      PyObject *obj = PyObject_GetAttrString(PyC, "c_char");
      if (!obj) {
        return NULL;
      }

      PyObject *str = PyUnicode_FromString("");
      PyObject *args = PyTuple_New(1);
      if (!str && !args) {
        return NULL;
      }

      PyTuple_SetItem(args, 0, str);

      PyObject *result = PyObject_Call(obj, args, NULL);
      if (!result) {
        Py_DECREF(obj);
        Py_DECREF(args);
        return NULL;
      }

      PyC_c_char *c_char = (PyC_c_char *)result;
      c_char->pointer = *(char **)arg;
      c_char->freeOnDel = false;
      c_char->isPointer = true;
      c_char->isArray = false;
      c_char->arrayCapacity = 0;
      c_char->arraySize = 0;

      Py_DECREF(obj);
      Py_DECREF(args);
      return result;
    }

    case CXType_Record:
    case CXType_Elaborated: {
      if (underlying_struct && underlying_struct->name) {
        const char *struct_name = underlying_struct->name;
        PyObject *obj = PyObject_GetAttrString(module, struct_name);

        if (obj) {
          PyObject *result = PyObject_CallObject(obj, PyTuple_New(0));
          PyC_c_struct *resultStruct = (PyC_c_struct *)result;
          free(resultStruct->pointer);
          resultStruct->pointer = *(void **)arg;
          resultStruct->freeOnDel = false;

          Py_DECREF(obj);
          return result;
        } else {
          return NULL;
        }
      } else if (underlying_union && underlying_union->name) {
        const char *struct_name = underlying_union->name;
        PyObject *obj = PyObject_GetAttrString(module, struct_name);

        if (obj) {
          PyObject *result = PyObject_CallObject(obj, PyTuple_New(0));
          PyC_c_union *resultUnion = (PyC_c_union *)result;
          free(resultUnion->pointer);
          resultUnion->pointer = *(void **)arg;
          resultUnion->freeOnDel = false;

          Py_DECREF(obj);
          return result;
        } else {
          return NULL;
        }
      } else {
        return NULL;
      }
    }

    default:
      PyErr_SetString(
          py_BindingError,
          "Could not convert Cpp type (pointer type) to Python type");
      return NULL;
    }

  case FFI_TYPE_STRUCT: {
    if (underlying_struct && underlying_struct->name) {
      const char *name = underlying_struct->name;

      PyObject *obj = PyObject_GetAttrString(module, name);
      if (obj) {
        PyObject *result = PyObject_CallObject(obj, PyTuple_New(0));
        if (!result) {
          return NULL;
        }

        char *arg_data = arg;

        for (size_t i = 0; i < underlying_struct->attrCount; i++) {
          PyObject *attr_name = PyUnicode_FromString(
              array_str_getat(underlying_struct->attrNames, i));

          PyObject *item = cppArg_to_pyArg(
              (char *)arg_data +
                  (array_long_long_getat(underlying_struct->offsets, i) / 8),
              *array_p_ffi_type_getat(underlying_struct->attrTypes, i),
              array_CXTypeKind_getat(underlying_struct->attrUnderlyingType, i),
              array_p_Structure_getat(underlying_struct->attrUnderlyingStructs,
                                      i),
              array_p_Union_getat(underlying_struct->attrUnderlyingUnions, i),
              module);

          if (PyObject_SetAttr(result, attr_name, item)) {
            Py_DECREF(attr_name);
            Py_DECREF(item);
            Py_DECREF(obj);
            return NULL;
          }

          Py_DECREF(attr_name);
          Py_DECREF(item);
        }

        Py_DECREF(obj);
        return result;
      } else {
        return NULL;
      }

    } else if (underlying_union && underlying_union->name) {
      const char *name = underlying_union->name;

      PyObject *obj = PyObject_GetAttrString(module, name);
      if (obj) {
        PyObject *result = PyObject_CallObject(obj, PyTuple_New(0));
        if (!result) {
          return NULL;
        }

        char *arg_data = arg;

        for (size_t i = 0; i < underlying_union->attrCount; i++) {
          PyObject *attr_name = PyUnicode_FromString(
              array_str_getat(underlying_union->attrNames, i));

          PyObject *item = cppArg_to_pyArg(
              (void *)arg_data,
              *array_p_ffi_type_getat(underlying_union->attrTypes, i),
              array_CXTypeKind_getat(underlying_union->attrUnderlyingType, i),
              array_p_Structure_getat(underlying_union->attrUnderlyingStructs,
                                      i),
              array_p_Union_getat(underlying_union->attrUnderlyingUnions, i),
              module);

          if (PyObject_SetAttr(result, attr_name, item)) {
            Py_DECREF(attr_name);
            Py_DECREF(item);
            Py_DECREF(obj);
            return NULL;
          }

          Py_DECREF(attr_name);
          Py_DECREF(item);
        }

        Py_DECREF(obj);
        return result;
      } else {
        return NULL;
      }
    } else {
      PyErr_SetString(
          py_BindingError,
          "Internal error occured. "
          "Not enough type info to convert cpp type to python type.");
      return NULL;
    }
  }
  case FFI_TYPE_VOID:
    Py_RETURN_NONE;
  default:
    PyErr_SetString(py_BindingError,
                    "Could not convert Cpp type to Python type");
    return NULL;
  }
}

void **pyArgs_to_cppArgs(PyObject *args, array_p_ffi_type_t *args_type,
                         bool *free_at, void **extras_to_free) {
  size_t args_len = array_p_ffi_type_size(args_type);

  if (PyTuple_Size(args) != args_len) {
    PyErr_SetString(py_BindingError,
                    "Arguments passed does not match function declaration");
    return NULL;
  }

  void **rvalue = (void **)malloc(sizeof(void *) * (args_len + 1));

  for (int i = 0; i < args_len; i++) {
    ffi_type type = *array_p_ffi_type_getat(args_type, i);
    PyObject *pyArg = PyTuple_GetItem(args, i);

    if (pyArg == Py_True) {
      void *x = malloc(type.size);
      memset(x, 1, type.size);
      rvalue[i] = x;
      free_at[i] = true;

    } else if (pyArg == Py_False) {
      void *x = malloc(type.size);
      memset(x, 0, type.size);
      rvalue[i] = x;
      free_at[i] = true;

    } else if (PyNumber_Check(pyArg)) {
      void *x = malloc(type.size);

      assert(PyErr_Occurred() == NULL);
      PyObject *arg_num = PyNumber_Long(pyArg);
      PyErr_Clear();
      PyObject *arg_float = PyNumber_Float(pyArg);
      PyErr_Clear();

      switch (type.type) {
      case FFI_TYPE_INT:
        *(int *)x = (int)PyLong_AsDouble(arg_num);
        break;
      case FFI_TYPE_SINT8:
        *(int8_t *)x = (int8_t)PyLong_AsDouble(arg_num);
        break;
      case FFI_TYPE_UINT8:
        *(uint8_t *)x = (uint8_t)PyLong_AsDouble(arg_num);
        break;
      case FFI_TYPE_SINT16:
        *(int16_t *)x = (int16_t)PyLong_AsDouble(arg_num);
        break;
      case FFI_TYPE_UINT16:
        *(uint16_t *)x = (uint16_t)PyLong_AsDouble(arg_num);
        break;
      case FFI_TYPE_SINT32:
        *(int32_t *)x = (int32_t)PyLong_AsDouble(arg_num);
        break;
      case FFI_TYPE_UINT32:
        *(uint32_t *)x = (uint32_t)PyLong_AsDouble(arg_num);
        break;
      case FFI_TYPE_SINT64:
        *(int64_t *)x = (int64_t)PyLong_AsDouble(arg_num);
        break;
      case FFI_TYPE_UINT64:
        *(uint64_t *)x = (uint64_t)PyLong_AsDouble(arg_num);
        break;
      case FFI_TYPE_FLOAT:
        *(float *)x = (float)PyFloat_AsDouble(arg_float);
        break;
      case FFI_TYPE_DOUBLE:
        *(double *)x = (double)PyFloat_AsDouble(arg_float);
        break;
      case FFI_TYPE_POINTER:
        free(x);
        // TODO: verify what is it pointing to from UnderlyingType
        if (PyObject_IsInstance(pyArg, (PyObject *)&py_c_int_type) ||
            PyObject_IsInstance(pyArg, (PyObject *)&py_c_uint_type)) {
          void **tmp = malloc(sizeof(void *));
          *tmp = ((PyC_c_int *)pyArg)->pointer;
          x = tmp;
          break;

        } else if (PyObject_IsInstance(pyArg, (PyObject *)&py_c_short_type) ||
                   PyObject_IsInstance(pyArg, (PyObject *)&py_c_ushort_type)) {
          void **tmp = malloc(sizeof(void *));
          *tmp = ((PyC_c_short *)pyArg)->pointer;
          x = tmp;
          break;

        } else if (PyObject_IsInstance(pyArg, (PyObject *)&py_c_long_type) ||
                   PyObject_IsInstance(pyArg, (PyObject *)&py_c_ulong_type)) {
          void **tmp = malloc(sizeof(void *));
          *tmp = ((PyC_c_long *)pyArg)->pointer;
          x = tmp;
          break;

        } else if (PyObject_IsInstance(pyArg, (PyObject *)&py_c_double_type)) {
          void **tmp = malloc(sizeof(void *));
          *tmp = ((PyC_c_double *)pyArg)->pointer;
          x = tmp;
          break;

        } else if (PyObject_IsInstance(pyArg, (PyObject *)&py_c_float_type)) {
          void **tmp = malloc(sizeof(void *));
          *tmp = ((PyC_c_float *)pyArg)->pointer;
          x = tmp;
          break;

        } else {
          Py_XDECREF(arg_num);
          Py_XDECREF(arg_float);

          PyErr_SetString(py_BindingError,
                          "Could not convert Python type to Cpp type (cpp "
                          "module expected pointer)"); // TODO: err msg

          return NULL;
        }
      default:
        Py_XDECREF(arg_num);
        Py_XDECREF(arg_float);

        PyErr_SetString(
            py_BindingError,
            "Could not convert Python type to Cpp type"); // TODO: err msg

        return NULL;
      }

      Py_XDECREF(arg_num);
      Py_XDECREF(arg_float);

      rvalue[i] = x;
      free_at[i] = true;

    } else if (PyUnicode_Check(pyArg)) {
      if (type.type == FFI_TYPE_POINTER) {
        // char pointer
        PyObject *encodedString =
            PyUnicode_AsEncodedString(pyArg, "UTF-8", "strict");
        if (!encodedString) {
          return NULL;
        }

        const char *tmp = PyBytes_AsString(encodedString);
        if (!tmp) {
          return NULL;
        }

        char *result = strdup(tmp);
        extras_to_free[i] = result;

        Py_XDECREF(encodedString);
        if (result) {
          void **x = (void **)malloc(sizeof(char **));
          x[0] = result;
          rvalue[i] = x;
          free_at[i] = true;

        } else {
          PyErr_NoMemory();
          return NULL;
        }

      } else if ((type.type == FFI_TYPE_SINT8) ||
                 (type.type == FFI_TYPE_UINT8)) {
        // TODO: implement python string to c char
      }

    } else if (PyObject_IsInstance(pyArg, (PyObject *)&py_c_char_type)) {
      // char pointer or single char
      PyC_c_char *selfType = (PyC_c_char *)pyArg;

      if (type.type == FFI_TYPE_POINTER) {
        rvalue[i] = &(selfType->pointer);

      } else if ((type.type == FFI_TYPE_SINT8) ||
                 (type.type == FFI_TYPE_UINT8)) {
        rvalue[i] = selfType->pointer;
      }

    } else if (PyObject_IsInstance(pyArg, (PyObject *)&py_c_struct_type)) {
      PyC_c_struct *selfType = (PyC_c_struct *)pyArg;
      if (type.type == FFI_TYPE_STRUCT) {
        rvalue[i] = selfType->pointer;
      } else if (type.type == FFI_TYPE_POINTER) {
        rvalue[i] = &(selfType->pointer);
      }

    } else if (PyObject_IsInstance(pyArg, (PyObject *)&py_c_union_type)) {
      PyC_c_union *selfType = (PyC_c_union *)pyArg;
      if (type.type == FFI_TYPE_STRUCT) {
        rvalue[i] = selfType->pointer;
      } else if (type.type == FFI_TYPE_POINTER) {
        rvalue[i] = &(selfType->pointer);
      }

    } else {
      PyErr_SetString(py_BindingError,
                      "Could not convert Python type to Cpp type");
      return NULL;
    }
  }

  rvalue[args_len] = NULL;
  return rvalue;
}

int match_ffi_type_to_defination(Function *funcs, PyObject *args) {
  int funcNum = -1;
  size_t funcCount = funcs->funcCount;
  Py_ssize_t argsCount = PyTuple_Size(args);

  for (int i = 0; i < funcCount; i++) {
    FunctionType *funcType =
        array_FunctionType_get_ptr_at(funcs->functionTypes, i);

    size_t funcTypeArgsCount = funcType->argsCount;
    if ((argsCount == funcTypeArgsCount) && (argsCount != 0)) {
      for (int j = 0; j < argsCount; j++) {
        ffi_type *type_from_decl =
            array_p_ffi_type_getat(funcType->argsType, j);
        PyObject *pyArg = PyTuple_GetItem(args, j);

        switch (type_from_decl->type) {
        case FFI_TYPE_SINT8:
        case FFI_TYPE_UINT8:
          // TODO: check for python string to c char
          if (PyObject_IsInstance(pyArg, (PyObject *)&py_c_char_type)) {
            funcNum = i;
            continue;
          }
        case FFI_TYPE_SINT16:
        case FFI_TYPE_UINT16:
        case FFI_TYPE_INT:
        case FFI_TYPE_SINT32:
        case FFI_TYPE_UINT32:
        case FFI_TYPE_SINT64:
        case FFI_TYPE_UINT64:
          if ((pyArg == Py_True) || (pyArg == Py_False) ||
              PyNumber_Check(pyArg) ||
              PyObject_IsInstance(pyArg, (PyObject *)&py_c_int_type) ||
              PyObject_IsInstance(pyArg, (PyObject *)&py_c_short_type) ||
              PyObject_IsInstance(pyArg, (PyObject *)&py_c_long_type) ||
              PyObject_IsInstance(pyArg, (PyObject *)&py_c_uint_type) ||
              PyObject_IsInstance(pyArg, (PyObject *)&py_c_ushort_type) ||
              PyObject_IsInstance(pyArg, (PyObject *)&py_c_ulong_type)) {
            funcNum = i;
            continue;
          } else {
            funcNum = -1;
            break;
          }
        case FFI_TYPE_FLOAT:
        case FFI_TYPE_DOUBLE:
          if (PyFloat_Check(pyArg) || PyNumber_Check(pyArg) ||
              PyObject_IsInstance(pyArg, (PyObject *)&py_c_double_type) ||
              PyObject_IsInstance(pyArg, (PyObject *)&py_c_float_type)) {
            funcNum = i;
            continue;
          } else {
            funcNum = -1;
            break;
          }
        case FFI_TYPE_POINTER:
          if (PyObject_IsInstance(pyArg, (PyObject *)&py_c_char_type) ||
              PyObject_IsInstance(pyArg, (PyObject *)&py_c_short_type) ||
              PyObject_IsInstance(pyArg, (PyObject *)&py_c_int_type) ||
              PyObject_IsInstance(pyArg, (PyObject *)&py_c_long_type) ||
              PyObject_IsInstance(pyArg, (PyObject *)&py_c_ushort_type) ||
              PyObject_IsInstance(pyArg, (PyObject *)&py_c_uint_type) ||
              PyObject_IsInstance(pyArg, (PyObject *)&py_c_ulong_type) ||
              PyObject_IsInstance(pyArg, (PyObject *)&py_c_float_type) ||
              PyObject_IsInstance(pyArg, (PyObject *)&py_c_double_type) ||
              PyObject_IsInstance(pyArg, (PyObject *)&py_c_struct_type) ||
              PyObject_IsInstance(pyArg, (PyObject *)&py_c_union_type) ||
              PyUnicode_Check(pyArg)) {
            funcNum = i;
            continue;
          } else {
            funcNum = -1;
            break;
          }
        case FFI_TYPE_STRUCT:
          if ((PyObject_IsInstance(pyArg, (PyObject *)&py_c_struct_type)) ||
              (PyObject_IsInstance(pyArg, (PyObject *)&py_c_union_type))) {
            // TODO: check if they are the same structs
            funcNum = i;
            continue;
          } else {
            funcNum = -1;
            break;
          }
        case FFI_TYPE_VOID:
          funcNum = i;
          continue;
        default:
          return -2;
        }
        if (funcNum == -1)
          break;
      }

    } else {
      funcNum = i;
    }
    if (funcNum != -1)
      return funcNum;
  }

  return -1;
}

const char *CXTypeKind_TO_char_p(enum CXTypeKind type) {
  switch (type) {
  case CXType_Void:
    return "void*";
  case CXType_UInt:
    return "u_int*";
  case CXType_Float:
    return "float*";
  case CXType_Double:
    return "double*";
  case CXType_LongDouble:
    return "long double*";
  case CXType_UShort:
    return "u_short*";
  case CXType_Int:
    return "int*";
  case CXType_Long:
  case CXType_LongLong:
    return "int64_t*";
  case CXType_Short:
    return "int16_t*";
  case CXType_ULong:
  case CXType_ULongLong:
    return "u_int64_t*";
  case CXType_Record:
    return "struct*"; // TODO: specify struct
  case CXType_Char_S:
    return "char*";
  default:
    // printf("%i", type);
    return "Could not figure out";
  }
}

const char *ffi_type_To_char_p(ffi_type type) {
  switch (type.type) {
  case FFI_TYPE_VOID:
    return "void";
  case FFI_TYPE_INT:
    return "int";
  case FFI_TYPE_FLOAT:
    return "float";
  case FFI_TYPE_DOUBLE:
    return "double";
  case FFI_TYPE_UINT8:
    return "u_int8_t";
  case FFI_TYPE_UINT16:
    return "u_int16_t";
  case FFI_TYPE_UINT32:
    return "u_int32_t";
  case FFI_TYPE_UINT64:
    return "u_int64_t";
  case FFI_TYPE_SINT8:
    return "int8_t";
  case FFI_TYPE_SINT16:
    return "int16_t";
  case FFI_TYPE_SINT32:
    return "int32_t";
  case FFI_TYPE_SINT64:
    return "int64_t";
  case FFI_TYPE_STRUCT:
    return "struct"; // TODO: specify struct name
  case FFI_TYPE_COMPLEX:
    return "complex";
  case FFI_TYPE_POINTER:
    return "void*";
  default:
    return "Could not figure out";
  }
}

ffi_type *get_ffi_type(CXType type, Symbols *sym, const char *name) {
  switch (type.kind) {
  case CXType_Void:
    return &ffi_type_void;
  case CXType_Int:
    return &ffi_type_sint;
  case CXType_UInt:
    return &ffi_type_uint;
  case CXType_Short:
    return &ffi_type_sshort;
  case CXType_UShort:
    return &ffi_type_ushort;
  case CXType_Float:
    return &ffi_type_float;
  case CXType_Double:
    return &ffi_type_double;
  case CXType_LongDouble:
    return &ffi_type_longdouble;
  case CXType_Long:
    return &ffi_type_sint64;
  case CXType_ULong:
    return &ffi_type_uint64;
  case CXType_LongLong:
    return &ffi_type_sint64;
  case CXType_ULongLong:
    return &ffi_type_uint64;
  case CXType_UChar:
  case CXType_SChar:
  case CXType_Bool:
    return &ffi_type_uchar;
  case CXType_Char_S:
    return &ffi_type_schar;
  case CXType_Pointer:
    return &ffi_type_pointer;
  case CXType_Elaborated:
    // check if type is pointer type
    if (clang_Type_getNamedType(type).kind == CXType_Pointer) {
      return &ffi_type_pointer;
    }

  case CXType_Record:
  case CXType_Typedef:
    // check if type is struct or union
    if (strncmp(name, "struct ", 7) == 0) {
      Structure *s = Symbols_getStructure(sym, name + 7);
      return &(s->type);
    }
    if (strncmp(name, "union ", 6) == 0) {
      Union *u = Symbols_getUnion(sym, name + 6);
      return &(u->type);
    }

  default:
    PyErr_SetString(
        py_BindingError,
        "Could not identify necessary types from the translation unit");
    return NULL;
  }
}
