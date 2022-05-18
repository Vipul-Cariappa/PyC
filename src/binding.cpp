#define PY_SSIZE_T_CLEAN
#include "Python.h"
#include <iostream>
#include <dlfcn.h>
#include "binding.hpp"

static PyObject *PyCpp;
static PyObject *CppError;
static PyObject *BindingError;

CXString (*mangled_name_getter_fn)(CXCursor) = &clang_getCursorSpelling;

CXString GET_MANGLED_NAME(CXCursor cursor)
{
    return mangled_name_getter_fn(cursor);
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
        throw std::runtime_error("Could not convert Cpp type to Python type");
    }
}

void **pyArgs_to_cppArgs(PyObject *args, std::vector<ffi_type> args_type)
{
    // TODO: match arg count of both args and args_type
    size_t args_len = args_type.size();

    void **rvalue = (void **)malloc(sizeof(void *) * (args_len + 1));

    for (int i = 0; i < args_len; i++)
    {
        ffi_type type = args_type.at(i);
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
                throw std::runtime_error("Could not convert Python type to Cpp type");
            }
            rvalue[i] = x;
        }
        else if (PyUnicode_Check(pyArg))
        {
            if ((type.type == FFI_TYPE_POINTER)) // char pointer
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
            throw std::runtime_error("Could not convert Python type to Cpp type");
        }
    }
    rvalue[args_len] = nullptr;
    return rvalue;
}

typedef struct PyCpp_CppModule
{
    PyObject_HEAD const char *library_name;
    const char *header_name;
    void *so;
    Symbols *symbols;
    int loaded;
} PyCpp_CppModule;

typedef struct PyCpp_CppFunction
{
    PyObject_HEAD
        Function *funcType;
    void *func;
} PyCpp_CppFunction;

typedef struct PyCpp_CppStruct
{
    PyObject_HEAD
        Structure *structType;
    void *data;
} PyCpp_CppStruct;

static int Cpp_ModuleInit(PyObject *self, PyObject *args, PyObject *kwargs)
{
    // std::cout << "Init CppModule\n";

    PyCpp_CppModule *selfType = (PyCpp_CppModule *)self;

    const char *library;
    const char *header;
    if (!PyArg_ParseTuple(args, "ss", &library, &header))
    {
        return 0;
    }
    // std::cout << "Library: " << library << " Header: " << header << std::endl;

    if (kwargs)
    {
        PyObject *cpp = PyUnicode_FromString("cpp");
        if (PyDict_Contains(kwargs, cpp))
        {
            PyObject *mode = PyDict_GetItem(kwargs, cpp);

            if (mode == Py_True)
            {
                mangled_name_getter_fn = &clang_Cursor_getMangling;
            }
            else if (mode == Py_False)
            {
                mangled_name_getter_fn = &clang_getCursorSpelling;
            }
            else
            {
                std::runtime_error("TypeError: Got non boolean value for mode");
            }
        }
        Py_DECREF(cpp);
    }
    else
    {
        mangled_name_getter_fn = &clang_getCursorSpelling;
    }

    selfType->header_name = header;
    selfType->library_name = library;
    selfType->symbols = get_symbols(header);

    // selfType->symbols->print_self();  //

    // opening the shared library
    void *so = dlopen(library, RTLD_NOW);
    if (!so)
    {
        throw std::runtime_error("Unable to load the shared library. Quitting.");
    }

    selfType->so = so;

    return 0;
}

static PyObject *Cpp_ModuleGet(PyObject *self, char *attr)
{
    PyCpp_CppModule *selfType = (PyCpp_CppModule *)self;

    Function *funcType = selfType->symbols->get_function(attr);

    if (funcType)
    {
        void *func = dlsym(selfType->so, funcType->mangledname.c_str());

        if (!func)
        {
            throw std::runtime_error("Function Not found. Quitting.");
        }

        PyObject *obj = PyObject_GetAttrString(PyCpp, "CppFunction");
        if (obj)
        {
            PyCpp_CppFunction *pyCppFunction = (PyCpp_CppFunction *)PyObject_CallObject(obj, nullptr);
            pyCppFunction->func = func;
            pyCppFunction->funcType = funcType;

            return (PyObject *)pyCppFunction;
        }
    }

    Global *globalVar = selfType->symbols->get_global(attr);

    if (globalVar)
    {
        void *var = dlsym(selfType->so, globalVar->mangledname.c_str());

        if (!var)
        {
            throw std::runtime_error("Global Variable Not found. Quitting.");
        }

        return cppArg_to_pyArg(var, globalVar->c_type);
    }

    Py_RETURN_NONE;
}

static int Cpp_ModuleSet(PyObject *self, char *attr, PyObject *pValue)
{
    PyErr_SetString(BindingError, "Cannot Assign Values to LuaModule Objects");
    return -1;
}

static void Cpp_ModuleGC(PyObject *self)
{
    // TODO: implement
    PyCpp_CppModule *selfType = (PyCpp_CppModule *)self;
    dlclose(selfType->so);
    return;
}

PyObject *Cpp_FunctionCall(PyObject *self, PyObject *args, PyObject *kwargs)
{
    PyCpp_CppFunction *selfType = (PyCpp_CppFunction *)self;

    size_t args_count = selfType->funcType->type->args_count;
    std::vector<ffi_type> args_list = selfType->funcType->type->args_types;
    ffi_cif cif;
    ffi_type **ffi_args = (ffi_type **)malloc(sizeof(ffi_type *) * (args_count + 1));
    void *rc = malloc(sizeof(selfType->funcType->type->return_type.size));

    for (int i = 0; i < args_count; i++)
    {
        ffi_args[i] = &args_list.at(i);
    }
    ffi_args[args_count] = nullptr;

    void **args_values = pyArgs_to_cppArgs(args, args_list);
    if (ffi_prep_cif(&cif, FFI_DEFAULT_ABI, args_count, &selfType->funcType->type->return_type, ffi_args) == FFI_OK)
    {
        ffi_call(&cif, (void (*)())selfType->func, rc, args_values);
    }

    return cppArg_to_pyArg(rc, selfType->funcType->type->return_type);
}

static void Cpp_FunctionGC(PyObject *self)
{
    // TODO: implement
    PyCpp_CppFunction *selfType = (PyCpp_CppFunction *)self;
    return;
}

static PyObject *new_PyCpp_CppStruct(Structure _struct)
{
    PyObject *obj = PyObject_GetAttrString(PyCpp, "CppStruct");
    if (obj)
    {
        PyCpp_CppStruct *result = (PyCpp_CppStruct *)PyObject_Call(obj, nullptr, nullptr);
        result->structType = &_struct;
        result->data = malloc(_struct.struct_size);
    }
    PyErr_SetString(BindingError, "Unable to access PyCpp.CppStruct");
    return nullptr;
}

static PyObject *Cpp_StructGet(PyObject *self, char *attr)
{
    PyCpp_CppStruct *selfType = (PyCpp_CppStruct *)self;
    Py_RETURN_NONE;
}

static int Cpp_StructSet(PyObject *self, char *attr, PyObject *pValue)
{
    PyCpp_CppStruct *selfType = (PyCpp_CppStruct *)self;
    return 0;
}

static void Cpp_StructGC(PyObject *self)
{
    // TODO: implement
    PyCpp_CppStruct *selfType = (PyCpp_CppStruct *)self;
    return;
}

static PyTypeObject CppModule_Type = {
    PyVarObject_HEAD_INIT(NULL, 0)
        .tp_name = "PyCpp.CppModule",
    .tp_basicsize = sizeof(PyCpp_CppModule),
    .tp_itemsize = 0,
    .tp_getattr = &Cpp_ModuleGet,
    .tp_setattr = &Cpp_ModuleSet,
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_doc = "PyCpp.CppModule",
    .tp_init = &Cpp_ModuleInit,
    .tp_new = PyType_GenericNew,
    .tp_finalize = &Cpp_ModuleGC,
};

static PyTypeObject CppFunction_Type = {
    PyVarObject_HEAD_INIT(NULL, 0)
        .tp_name = "PyCpp.CppFunction",
    .tp_basicsize = sizeof(PyCpp_CppFunction),
    .tp_itemsize = 0,
    .tp_call = &Cpp_FunctionCall,
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_doc = "PyCpp.CppModule",
    .tp_new = PyType_GenericNew,
    .tp_finalize = &Cpp_FunctionGC,
};

static PyTypeObject CppStruct_Type = {
    PyVarObject_HEAD_INIT(NULL, 0)
        .tp_name = "PyCpp.CppStruct",
    .tp_basicsize = sizeof(PyCpp_CppStruct),
    .tp_itemsize = 0,
    .tp_getattr = &Cpp_StructGet,
    .tp_setattr = &Cpp_StructSet,
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_doc = "PyCpp.CppStruct",
    .tp_new = PyType_GenericNew,
    .tp_finalize = &Cpp_StructGC,
};

static PyObject *load_cpp(PyObject *self, PyObject *args, PyObject *kwargs)
{
    // const char* library;
    // const char* header;
    // if (!PyArg_ParseTuple(args, "ss", &library, &header))
    // {
    //     return NULL;
    // }
    // std::cout << "Library: " << library << " Header: " << header << std::endl;

    PyObject *obj = PyObject_GetAttrString(PyCpp, "CppModule");
    if (obj)
    {
        return PyObject_Call(obj, args, kwargs);
    }
    PyErr_SetString(BindingError, "Unable to access PyCpp.CppModule");
    return nullptr;
}

static PyMethodDef PyCppMethods[] = {
    {"LoadCpp", (PyCFunction)load_cpp, METH_VARARGS | METH_KEYWORDS, "Execute a shell command."},
    {NULL, NULL, 0, NULL}};

static struct PyModuleDef PyCppmodule = {
    PyModuleDef_HEAD_INIT,
    "PyCpp",
    "PyCpp",
    -1,
    PyCppMethods};

PyMODINIT_FUNC PyInit_PyC(void)
{
    PyObject *m;

    m = PyModule_Create(&PyCppmodule);
    if (m == NULL)
        return NULL;

    if (PyType_Ready(&CppModule_Type) < 0)
    {
        return NULL;
    }

    Py_INCREF(&CppModule_Type);
    if (PyModule_AddObject(m, "CppModule", (PyObject *)&CppModule_Type) < 0)
    {
        Py_DECREF(&CppModule_Type);
        Py_DECREF(m);
        return NULL;
    }

    if (PyType_Ready(&CppFunction_Type) < 0)
    {
        return NULL;
    }

    Py_INCREF(&CppFunction_Type);
    if (PyModule_AddObject(m, "CppFunction", (PyObject *)&CppFunction_Type) < 0)
    {
        Py_DECREF(&CppFunction_Type);
        Py_DECREF(m);
        return NULL;
    }

    CppError = PyErr_NewException("PyCpp.CppError", NULL, NULL);
    Py_XINCREF(CppError);
    if (PyModule_AddObject(m, "CppError", CppError) < 0)
    {
        Py_XDECREF(CppError);
        Py_CLEAR(CppError);
        Py_DECREF(m);
        return NULL;
    }

    BindingError = PyErr_NewException("PyCpp.BindingError", NULL, NULL);
    Py_XINCREF(CppError);
    if (PyModule_AddObject(m, "BindingError", BindingError) < 0)
    {
        Py_XDECREF(BindingError);
        Py_CLEAR(BindingError);
        Py_DECREF(m);
        return NULL;
    }

    PyCpp = m;
    return m;
}
