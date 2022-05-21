#define PY_SSIZE_T_CLEAN
#include "Python.h"
#include "ffi.h"
#include "clang-c/Index.h"
#include "qlibc.h"


void *pyArg_to_cppArg(PyObject *arg, ffi_type type)
{
    void *data = NULL;

    switch (type.type)
    {
    case FFI_TYPE_INT:
    case FFI_TYPE_SINT32:
    case FFI_TYPE_UINT32:
    {
        data = malloc(4);
        int32_t i = (int32_t)PyLong_AsLong(arg);
        memcpy(data, &i, 4);
        break;
    }
    case FFI_TYPE_SINT8:
    case FFI_TYPE_UINT8:
    {
        data = malloc(1);
        int8_t i = (int8_t)PyLong_AsLong(arg);
        memcpy(data, &i, 1);
        break;
    }
    case FFI_TYPE_SINT16:
    case FFI_TYPE_UINT16:
    {
        data = malloc(2);
        int16_t i = (int16_t)PyLong_AsLong(arg);
        memcpy(data, &i, 2);
        break;
    }
    case FFI_TYPE_SINT64:
    case FFI_TYPE_UINT64:
    {
        data = malloc(8);
        int64_t i = (int64_t)PyLong_AsLong(arg);
        memcpy(data, &i, 8);
        break;
    }
    case FFI_TYPE_FLOAT:
    {
        data = malloc(sizeof(float));
        float i = (float)PyFloat_AsDouble(arg);
        memcpy(data, &i, sizeof(float));
        break;
    }
    case FFI_TYPE_DOUBLE:
    {
        data = malloc(sizeof(double));
        double i = (double)PyFloat_AsDouble(arg);
        memcpy(data, &i, sizeof(double));
        break;
    }
    case FFI_TYPE_LONGDOUBLE:
    {
        data = malloc(sizeof(long double));
        long double i = (long double)PyFloat_AsDouble(arg);
        memcpy(data, &i, sizeof(long double));
        break;
    }
    case FFI_TYPE_POINTER: // Assuming it is char pointer
        data = (char *)PyUnicode_AsUTF8(arg);
        break;
    default:
        // TODO: raise error "Could not convert Cpp type to Python type"
        abort();
    }

    return data;
}


PyObject *cppArg_to_pyArg(void *arg, ffi_type type)
{
    switch (type.type)
    {
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
    case FFI_TYPE_LONGDOUBLE:
        return PyFloat_FromDouble(*(long double *)arg);
    case FFI_TYPE_POINTER: // Assuming it is char pointer
        return PyUnicode_FromString(*(char **)arg);
    default:
        // TODO: raise error "Could not convert Cpp type to Python type"
        abort();
    }
}


void **pyArgs_to_cppArgs(PyObject *args, qvector_t *args_type)
{
    // TODO: match arg count of both args and args_type
    size_t args_len = qvector_size(args_type);

    void **rvalue = (void **)malloc(sizeof(void *) * (args_len + 1));

    for (int i = 0; i < args_len; i++)
    {
        // ffi_type type = args_type.at(i);
        ffi_type type = *((ffi_type*)qvector_getat(args_type, i, false));
        PyObject *pyArg = PyTuple_GetItem(args, i);

        if (pyArg == Py_True)
        {
            void *x = malloc(type.size);
            *(char *)x = 1;
            rvalue[i] = x;
        }
        else if (pyArg == Py_False)
        {
            void *x = malloc(type.size);
            memset(x, 0, type.size);
            rvalue[i] = x;
        }
        else if (PyNumber_Check(pyArg))
        {
            void *x = malloc(type.size);
            switch (type.type)
            {
            case FFI_TYPE_INT:
                *(int *)x = (int)PyFloat_AsDouble(pyArg);
                break;
            case FFI_TYPE_SINT8:
                *(int8_t *)x = (int8_t)PyFloat_AsDouble(pyArg);
                break;
            case FFI_TYPE_UINT8:
                *(uint8_t *)x = (uint8_t)PyFloat_AsDouble(pyArg);
                break;
            case FFI_TYPE_SINT16:
                *(int16_t *)x = (int16_t)PyFloat_AsDouble(pyArg);
                break;
            case FFI_TYPE_UINT16:
                *(uint16_t *)x = (uint16_t)PyFloat_AsDouble(pyArg);
                break;
            case FFI_TYPE_SINT32:
                *(int32_t *)x = (int32_t)PyFloat_AsDouble(pyArg);
                break;
            case FFI_TYPE_UINT32:
                *(uint32_t *)x = (uint32_t)PyFloat_AsDouble(pyArg);
                break;
            case FFI_TYPE_SINT64:
                *(int64_t *)x = (int64_t)PyFloat_AsDouble(pyArg);
                break;
            case FFI_TYPE_UINT64:
                *(uint64_t *)x = (uint64_t)PyFloat_AsDouble(pyArg);
                break;
            case FFI_TYPE_FLOAT:
                *(float *)x = (float)PyFloat_AsDouble(pyArg);
                break;
            case FFI_TYPE_DOUBLE:
                *(double *)x = (double)PyFloat_AsDouble(pyArg);
                break;
            case FFI_TYPE_LONGDOUBLE:
                *(long double *)x = (long double)PyFloat_AsDouble(pyArg);
                break;
            default:
                // TODO: raise error "Could not convert Python type to Cpp type"
                abort();
            }
            rvalue[i] = x;
        }
        else if (PyUnicode_Check(pyArg))
        {
            if (type.type == FFI_TYPE_POINTER) // char pointer
            {
                PyObject *encodedString = PyUnicode_AsEncodedString(pyArg, "UTF-8", "strict");
                if (encodedString)
                {
                    char *result = PyBytes_AsString(encodedString);
                    if (result)
                    {
                        void **x = (void **)malloc(sizeof(char **));
                        x[0] = result;
                        rvalue[i] = x;
                    }
                    else
                    {
                        // raise error
                    }
                }
                else
                {
                    // raise error
                }
            }
            else // char and wchar., etc
            {
                // TODO: implement
            }
        }
        else
        {
            // TODO: raise error "Could not convert Python type to Cpp type"
            abort();
        }
    }
    rvalue[args_len] = NULL;
    return rvalue;
}

const char *ffi_type_To_char_p(ffi_type type)
{
    switch (type.type)
    {
    case FFI_TYPE_VOID:
        return "void";
    case FFI_TYPE_INT:
        return "int";
    case FFI_TYPE_FLOAT:
        return "float";
    case FFI_TYPE_DOUBLE:
        return "double";
    case FFI_TYPE_LONGDOUBLE:
        return "long double";
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
        return "struct";
    case FFI_TYPE_COMPLEX:
        return "complex";
    case FFI_TYPE_POINTER:
        return "void*";
    default:
        return "Could not figure out";
    }
}

ffi_type *get_ffi_type(CXType type)
{
    switch (type.kind)
    {
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
    case CXType_Char_U:
        return &ffi_type_uchar;
    case CXType_Char_S:
        return &ffi_type_schar;
    case CXType_Pointer:
        return &ffi_type_pointer;
    // case CXType_Typedef:
    //     return get_ffi_type(type);
    default:
        // TODO: raise error "Could not Identify the type"
        abort();
    }
}