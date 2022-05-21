#define PY_SSIZE_T_CLEAN
#include <stdbool.h>
#include <stdlib.h>
#include <dlfcn.h>
#include "Python.h"
#include "PyC.h"
#include "CppTypeInfo.h"


CXString (*mangled_name_getter_fn)(CXCursor) = &clang_getCursorSpelling;

CXString GET_MANGLED_NAME(CXCursor cursor)
{
    return mangled_name_getter_fn(cursor);
}


typedef struct PyC_CppModule
{
    PyObject_HEAD
    const char *library_name;
    const char *header_name;
    void *so;
    Symbols *symbols;
    int loaded;
} PyC_CppModule;

typedef struct PyC_CppFunction
{
    PyObject_HEAD
    Function *funcType;
    void *func;
} PyC_CppFunction;

typedef struct PyC_CppStruct
{
    PyObject_HEAD
    Structure *structType;
    void *data;
} PyC_CppStruct;


PyTypeObject py_CppModuleType = {
    PyVarObject_HEAD_INIT(NULL, 0).tp_name = "PyCpp.CppModule",
    .tp_basicsize = sizeof(PyC_CppModule),
    .tp_itemsize = 0,
    .tp_getattr = &Cpp_ModuleGet,
    .tp_setattr = &Cpp_ModuleSet,
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_doc = "PyCpp.CppModule",
    .tp_init = &Cpp_ModuleInit,
    .tp_new = PyType_GenericNew,
    .tp_finalize = &Cpp_ModuleGC,
};

PyTypeObject py_CppFunctionType = {
    PyVarObject_HEAD_INIT(NULL, 0).tp_name = "PyCpp.CppFunction",
    .tp_basicsize = sizeof(PyC_CppFunction),
    .tp_itemsize = 0,
    .tp_call = &Cpp_FunctionCall,
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_doc = "PyCpp.CppModule",
    .tp_new = PyType_GenericNew,
    .tp_finalize = &Cpp_FunctionGC,
};

PyTypeObject py_CppStructType = {
    PyVarObject_HEAD_INIT(NULL, 0).tp_name = "PyCpp.CppStruct",
    .tp_basicsize = sizeof(PyC_CppStruct),
    .tp_itemsize = 0,
    .tp_getattr = &Cpp_StructGet,
    .tp_setattr = &Cpp_StructSet,
    .tp_call = &Cpp_StructCall,
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_doc = "PyCpp.CppStruct",
    .tp_new = PyType_GenericNew,
    .tp_finalize = &Cpp_StructGC,
};


PyMethodDef PyC_Methods[] = {
    {"LoadCpp", (PyCFunction)load_cpp, METH_VARARGS | METH_KEYWORDS, "Load C/C++ shared object"},
    {"print_CppModule", (PyCFunction)print_PyC_CppModule, METH_VARARGS, "print CppModule symbols"},
    {NULL, NULL, 0, NULL}
};

PyModuleDef PyC_Module = {
    PyModuleDef_HEAD_INIT,
    "PyCpp",
    "PyCpp",
    -1,
    PyC_Methods
};

// loads *.so file into the running program and parses the header file for symbols
static PyObject *load_cpp(PyObject *self, PyObject *args, PyObject *kwargs)
{
    PyObject *obj = PyObject_GetAttrString(PyC, "CppModule");
    if (obj) 
    {
        return PyObject_Call(obj, args, kwargs);
    }
    
    PyErr_SetString(py_BindingError, "Unable to access PyCpp.CppModule");
    return NULL;
}

static PyObject *print_PyC_CppModule(PyObject *self, PyObject *args, PyObject *kwargs)
{
    PyObject *py_PyC_CppModule;

    if (!PyArg_ParseTuple(args, "O", &py_PyC_CppModule))
        return NULL;

    PyC_CppModule *type_PyC_CppModule = (PyC_CppModule*)py_PyC_CppModule;
    print_Symbols(type_PyC_CppModule->symbols);

    Py_RETURN_NONE;
}

static int Cpp_ModuleInit(PyObject *self, PyObject *args, PyObject *kwargs)
{
    PyC_CppModule *selfType = (PyC_CppModule*)self;

    const char *library;
    const char *header;
    if (!PyArg_ParseTuple(args, "ss", &library, &header))
    {
        return 0;
    }

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
                // TODO: raise "TypeError: Got non boolean value for mode"
                abort();
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
    selfType->symbols = create_Symbol(header);
    if (!Symbols_parse(selfType->symbols, header))
    {
        free_Symbols(selfType->symbols);

        PyErr_SetString(py_BindingError, "Unable to parse translation unit. Quitting.");
        return -1;
    }

    // print_Symbols(selfType->symbols);

    // opening the shared library
    void *so = dlopen(library, RTLD_NOW);
    if (!so)
    {
        // TODO: raise error "Unable to load the shared library. Quitting."
        abort();
    }

    selfType->so = so;

    return 0;
}

static PyObject *Cpp_ModuleGet(PyObject *self, char *attr)
{
    PyC_CppModule *selfType = (PyC_CppModule*)self;

    Function *funcType = Symbols_getFunction(selfType->symbols, attr);

    if (funcType)
    {
        void *func = dlsym(selfType->so, qlist_getlast(funcType->mangledNames, NULL, false));

        if (!func)
        {
            // TODO: raise error "Function Not found. Quitting."
            abort();
        }

        PyObject *obj = PyObject_GetAttrString(PyC, "CppFunction");
        if (obj)
        {
            PyC_CppFunction *pyCppFunction = (PyC_CppFunction*)PyObject_CallObject(obj, NULL);
            pyCppFunction->func = func;
            pyCppFunction->funcType = funcType;

            return (PyObject *)pyCppFunction;
        }
    }

    Global *globalVar = Symbols_getGlobal(selfType->symbols, attr);

    if (globalVar)
    {
        void *var = dlsym(selfType->so, attr);

        if (!var)
        {
            // TODO: raise error "Global Variable Not found. Quitting."
            abort();
        }

        return cppArg_to_pyArg(var, globalVar->type);
    }

    Structure *structVar = Symbols_getStructure(selfType->symbols, attr);

    if (structVar)
    {
        return new_PyCpp_CppStruct(structVar);
    }

    Py_RETURN_NONE;
}

static PyObject *new_PyCpp_CppStruct(Structure *structure)
{
    PyObject *obj = PyObject_GetAttrString(PyC, "CppStruct");
    if (obj)
    {
        PyC_CppStruct *result = (PyC_CppStruct*)PyObject_CallObject(obj, NULL);
        result->structType = structure;
        result->data = malloc(structure->structSize);

        return (PyObject *)result;
    }
    PyErr_SetString(py_BindingError, "Unable to access PyCpp.CppStruct");
    return NULL;
}

static int Cpp_ModuleSet(PyObject *self, char *attr, PyObject *pValue)
{
    PyErr_SetString(py_BindingError, "Cannot Assign Values to LuaModule Objects");
    return -1;
}

static void Cpp_ModuleGC(PyObject *self)
{
    PyC_CppModule *selfType = (PyC_CppModule*)self;
    dlclose(selfType->so);
    free_Symbols(selfType->symbols);
}

PyObject *Cpp_FunctionCall(PyObject *self, PyObject *args, PyObject *kwargs)
{
    PyC_CppFunction *selfType = (PyC_CppFunction *)self;

    FunctionType *funcType = qvector_getlast(selfType->funcType->functionTypes, false);
    size_t args_count = funcType->argsCount;

    qvector_t *args_list = funcType->argsType;
    ffi_cif cif;
    ffi_type **ffi_args = (ffi_type **)malloc(sizeof(ffi_type *) * (args_count + 1));
    void *rc = malloc(sizeof(funcType->returnType.size));

    for (int i = 0; i < args_count; i++)
    {
        ffi_args[i] = qvector_getat(args_list, i, false);
    }
    ffi_args[args_count] = NULL;

    void **args_values = pyArgs_to_cppArgs(args, args_list);
    if (ffi_prep_cif(&cif, FFI_DEFAULT_ABI, args_count, &funcType->returnType, ffi_args) == FFI_OK)
    {
        ffi_call(&cif, (void (*)())selfType->func, rc, args_values);
    }

    return cppArg_to_pyArg(rc, funcType->returnType);
}

static void Cpp_FunctionGC(PyObject *self)
{
    // TODO: implement
    PyC_CppFunction *selfType = (PyC_CppFunction *)self;
    return;
}

static PyObject *Cpp_StructGet(PyObject *self, char *attr)
{
    // PyC_CppStruct *selfType = (PyC_CppStruct *)self;
    // int index = 0;
    // for (std::string &i : selfType->structType->attr_names)
    // {
    //     if (i == attr)
    //     {
    //         long long offset = clang_Type_getOffsetOf(selfType->structType->cpp_type, attr) / 8;
    //         char *data = (char *)selfType->data;
    //         return cppArg_to_pyArg(data + offset, selfType->structType->types.at(index));
    //     }
    //     index++;
    // }

    // TODO: implement Cpp_StructGet
    Py_RETURN_NONE;
}

static int Cpp_StructSet(PyObject *self, char *attr, PyObject *pValue)
{
    // PyC_CppStruct *selfType = (PyC_CppStruct *)self;
    // int index = 0;
    // for (std::string &i : selfType->structType->attr_names)
    // {
    //     if (i == attr)
    //     {
    //         long long offset = clang_Type_getOffsetOf(selfType->structType->cpp_type, attr) / 8;
    //         ffi_type type = selfType->structType->types.at(index);
    //         char *data = (char *)selfType->data;
    //         void *value = pyArg_to_cppArg(pValue, type);
    //         memcpy(data+offset, value, type.size);
    //         free(value);
    //         return 0;
    //     }
    //     index++;
    // }
    // PyErr_SetString(py_CppError, "Struct with given attribute not found");
    // return -1;

    // TODO: implement Cpp_StructSet
    return 0;
}

PyObject *Cpp_StructCall(PyObject *self, PyObject *args, PyObject *kwargs)
{
    return self;
}

static void Cpp_StructGC(PyObject *self)
{
    // TODO: implement Cpp_StructGC
    PyC_CppStruct *selfType = (PyC_CppStruct *)self;
}
