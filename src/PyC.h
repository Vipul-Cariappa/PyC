#pragma once

#include "CppTypeInfo.h"
#include "Python.h"
#include "ffi.h"
#include "qlibc.h"

extern PyObject *PyC; // python PyC module

extern PyObject *py_CppError;     // python Exception CppError
extern PyObject *py_BindingError; // python Exception BindingError

extern PyTypeObject py_CppModuleType;
extern PyTypeObject py_CppFunctionType;
extern PyTypeObject py_CppStructType;
extern PyModuleDef PyC_Module;

const char *ffi_type_To_char_p(ffi_type type);
const char *CXTypeKind_TO_char_p(enum CXTypeKind type);
ffi_type *get_ffi_type(CXType type);
void **pyArgs_to_cppArgs(PyObject *args, qvector_t *args_type);
qvector_t *get_ffi_type_from_pyArgs(PyObject *args);
int match_ffi_type_to_defination(Function *funcs, PyObject *ffi_type_list);
PyObject *cppArg_to_pyArg(void *arg, ffi_type type);
void *pyArg_to_cppArg(PyObject *arg, ffi_type type);

static PyObject *load_cpp(PyObject *self, PyObject *args, PyObject *kwargs);
static PyObject *print_PyC_CppModule(PyObject *self, PyObject *args,
                                     PyObject *kwargs);

static int Cpp_ModuleInit(PyObject *self, PyObject *args, PyObject *kwargs);
static PyObject *Cpp_ModuleGet(PyObject *self, char *attr);
static int Cpp_ModuleSet(PyObject *self, char *attr, PyObject *pValue);
static void Cpp_ModuleGC(PyObject *self);

static PyObject *Cpp_FunctionCall(PyObject *self, PyObject *args,
                                  PyObject *kwargs);
static void Cpp_FunctionGC(PyObject *self);

static PyObject *Cpp_StructGet(PyObject *self, char *attr);
static int Cpp_StructSet(PyObject *self, char *attr, PyObject *pValue);
static PyObject *Cpp_StructCall(PyObject *self, PyObject *args,
                                PyObject *kwargs);
static void Cpp_StructGC(PyObject *self);

static PyObject *new_PyCpp_CppStruct(Structure *structure);

CXString GET_MANGLED_NAME(CXCursor cursor);
