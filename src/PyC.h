#pragma once

#include "CppTypeInfo.h"
#include "DS.h"
#include "Python.h"
#include "ffi.h"

extern PyObject *PyC;                     // python PyC module
void PyClear_PyC(void *self);             // module clear function
extern p_void_array_t *EXTRA_HEAP_MEMORY; // allocated memory to be freed at destruction of module

extern PyObject *py_CppError;     // python Exception CppError
extern PyObject *py_BindingError; // python Exception BindingError

extern PyTypeObject py_CppModuleType;
extern PyTypeObject py_CppFunctionType;
extern PyModuleDef PyC_Module;

#define PYERR_MEM(data)                                                                                                \
    if (data == NULL) {                                                                                                \
        return PyErr_NoMemory();                                                                                       \
    }

char *CXX_Type_TO_char_p(enum CXX_Type type, const void *extraInfo);
int match_ffi_type_to_defination(Function *funcs, PyObject *ffi_type_list);
PyObject *cppArg_to_pyArg(void *arg, enum CXX_Type type, void *extra_type_info, PyObject *module);
void *pyArg_to_cppArg(PyObject *arg, enum CXX_Type type, bool *should_free);

PyObject *load_cpp(PyObject *self, PyObject *args, PyObject *kwargs);
PyObject *print_PyC_CppModule(PyObject *self, PyObject *args, PyObject *kwargs);

static int Cpp_ModuleInit(PyObject *self, PyObject *args, PyObject *kwargs);
static PyObject *Cpp_ModuleGet(PyObject *self, char *attr);
static int Cpp_ModuleSet(PyObject *self, char *attr, PyObject *pValue);
static void Cpp_ModuleGC(PyObject *self);
static int Cpp_ModuleTraverse(PyObject *self, visitproc visit, void *arg);
static int Cpp_ModuleClear(PyObject *self);

static PyObject *Cpp_FunctionCall(PyObject *self, PyObject *args, PyObject *kwargs);
static void Cpp_FunctionGC(PyObject *self);
static int Cpp_FunctionTraverse(PyObject *self, visitproc visit, void *arg);
static int Cpp_FunctionClear(PyObject *self);

static PyObject *Cpp_StructGet(PyObject *self, char *attr);
static int Cpp_StructSet(PyObject *self, char *attr, PyObject *pValue);
static PyObject *Cpp_StructCall(PyObject *self, PyObject *args, PyObject *kwargs);
static void Cpp_StructGC(PyObject *self);

CXString GET_MANGLED_NAME(CXCursor cursor);

// ----- Doc String -----

#define CPP_MODULE_DOC_STRING "wrapper around C/C++ module"

#define CPP_FUNCTION_DOC_STRING "wrapper around C/C++ function"

#define C_INT_DOC_STRING "wrapper class around C/C++ int datatype"

#define LOAD_CPP_DOC_STRING                                                                                            \
    "loads given dynamic library to the application memory and parses the "                                            \
    "header file for global declarations\n"                                                                            \
    "Positional Arguments: \n"                                                                                         \
    "    library: path to dynamic library\n"                                                                           \
    "    header: path to header file\n"                                                                                \
    "Keyword Argument:\n"                                                                                              \
    "    cpp: boolean True of the file is cpp file. (Default: False)"

#define PRINT_CPPMODULE_DOC_STRING "prints all the global declarations parsed from translation unit to stdout"

#define C_TYPE_FREE_ON_NO_REFERENCE_DOC                                                                                \
    "Set free_on_no_reference to true if allocated memory should be free when "                                        \
    "not the variable goes out of scope. The default is true for any c_type "                                          \
    "created by user and false for whenever a c_type is returned from a "                                              \
    "function call"
