#define PY_SSIZE_T_CLEAN
#include "CppTypeInfo.h"
#include "PyC.h"
#include "ffi.h"
#include "clang-c/Index.h"
#include <assert.h>
#include <errno.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SIZE 1024

void (*nullFunc)() = NULL;

void str_clear(char **s) {
    free(*s);
}

DS_ARRAY_DEF(str, char *, str_clear);
DS_ARRAY_DEF(CXX_Type, enum CXX_Type, nullFunc);
DS_ARRAY_DEF(p_ffi_type, ffi_type *, nullFunc);
DS_ARRAY_DEF(p_Structure, Structure *, nullFunc);
DS_ARRAY_DEF(p_Union, Union *, nullFunc);
DS_ARRAY_DEF(long_long, long long, nullFunc);
DS_ARRAY_DEF(p_void, void *, nullFunc);

DS_ARRAY_DEF(FunctionType, FunctionType, Symbols_clearFunctionType);

DS_LIST_DEF(Function, Function, Symbols_clearFunction);
DS_LIST_DEF(Global, Global, Symbols_clearGlobal);
DS_LIST_DEF(Structure, Structure, Symbols_clearStructure);
DS_LIST_DEF(Union, Union, Symbols_clearUnion);
DS_LIST_DEF(TypeDef, TypeDef, Symbols_clearTypedef);
DS_LIST_DEF(Enum, Enum, Symbols_clearEnum);

enum CXChildVisitResult visitor(CXCursor cursor, CXCursor parent,
                                CXClientData client_data);

char *clangString_to_CString(CXString cxstring_object) {
    const char *string        = clang_getCString(cxstring_object);
    char       *return_string = strdup(string);
    clang_disposeString(cxstring_object);
    return return_string;
}

void print_Global(const Global *global) {
    printf("    %s: %s\n", global->name,
           CXX_Type_TO_char_p(global->typeCXX, global->extraInfo));
}

void print_Structure(const Structure *const structure) {
    assert(str_array_size(structure->attrNames) == structure->attrCount);
    assert(p_ffi_type_array_size(structure->attrTypeFFI) ==
           structure->attrCount);

    printf("    struct %s\n", structure->name);

    for (size_t i = 0; i < structure->attrCount; i++) {
        enum CXX_Type type = CXX_Type_array_getat(structure->attrTypeCXX, i);
        const char   *attr_name = str_array_getat(structure->attrNames, i);
        const void *extra_info = p_void_array_getat(structure->attrTypeInfo, i);

        printf("      %s: %s\n", attr_name,
               CXX_Type_TO_char_p(type, extra_info));
    }
}

void print_Union(const Union *_union) {
    assert(str_array_size(_union->attrNames) == _union->attrCount);
    assert(p_ffi_type_array_size(_union->attrTypeFFI) == _union->attrCount);

    printf("    union %s\n", _union->name);

    for (size_t i = 0; i < _union->attrCount; i++) {
        enum CXX_Type type       = CXX_Type_array_getat(_union->attrTypeCXX, i);
        const char   *attr_name  = str_array_getat(_union->attrNames, i);
        const void   *extra_info = p_void_array_getat(_union->attrTypeInfo, i);

        printf("      %s: %s\n", attr_name,
               CXX_Type_TO_char_p(type, extra_info));
    }
}

void print_Function(const Function *func) {
    assert(FunctionType_array_size(func->functionTypes) == func->funcCount);

    printf("    %s\n", func->name);

    for (size_t i = 0; i < func->funcCount; i++) {
        FunctionType func_type =
            FunctionType_array_getat(func->functionTypes, i);

        printf("      %s %s(",
               CXX_Type_TO_char_p(func_type.returnTypeCXX,
                                  func_type.returnTypeInfo),
               func_type.mangledName);

        assert(func_type.argsCount ==
               p_ffi_type_array_size(func_type.argsTypeFFI));
        assert(func_type.argsCount ==
               CXX_Type_array_size(func_type.argsTypeCXX));

        for (size_t j = 0; j < func_type.argsCount; j++) {
            printf("%s, ", CXX_Type_TO_char_p(
                               CXX_Type_array_getat(func_type.argsTypeCXX, j),
                               p_void_array_getat(func_type.argsTypeInfo, j)));
        }
        printf(")\n");
    }
}

void print_Enum(const Enum *e) {
    printf("    %s: %lli\n", e->name, e->value);
    return;
}

void print_TypeDef(const TypeDef *td) {
    printf("    %s: %s\n", td->name,
           CXX_Type_TO_char_p(td->type, td->extraInfo));
}

void print_Symbols(const Symbols *symbols) {
    printf("Name: %s\n", symbols->name);

    printf("\n  Functions: \n");
    DS_LIST_PTR_FOREACH (symbols->funcs, func) { print_Function(func); }

    printf("\n  Globals: \n");
    DS_LIST_PTR_FOREACH (symbols->globals, global) { print_Global(global); }

    printf("\n  Structures: \n");
    DS_LIST_PTR_FOREACH (symbols->structs, s) { print_Structure(s); }

    printf("\n  Union: \n");
    DS_LIST_PTR_FOREACH (symbols->unions, u) { print_Union(u); }

    printf("\n  TypeDefs: \n");
    DS_LIST_PTR_FOREACH (symbols->typedefs, td) { print_TypeDef(td); }

    printf("\n  Enums: \n");
    DS_LIST_PTR_FOREACH (symbols->enums, e) { print_Enum(e); }
}

Function *Symbols_getFunction(const Symbols *sym, const char *name) {
    DS_LIST_PTR_FOREACH (sym->funcs, elem) {
        if (!strcmp(elem->name, name)) {
            return elem;
        }
    }
    return NULL;
}

Structure *Symbols_getStructure(const Symbols *sym, const char *name) {
    DS_LIST_PTR_FOREACH (sym->structs, elem) {
        if (!strcmp(elem->name, name)) {
            return elem;
        }
    }
    return NULL;
}

Union *Symbols_getUnion(const Symbols *sym, const char *name) {
    DS_LIST_PTR_FOREACH (sym->unions, elem) {
        if (!strcmp(elem->name, name)) {
            return elem;
        }
    }
    return NULL;
}

Global *Symbols_getGlobal(const Symbols *sym, const char *name) {
    DS_LIST_PTR_FOREACH (sym->globals, elem) {
        if (!strcmp(elem->name, name)) {
            return elem;
        }
    }
    return NULL;
}

TypeDef *Symbols_getTypeDef(const Symbols *sym, const char *name) {
    DS_LIST_PTR_FOREACH (sym->typedefs, elem) {
        if (!strcmp(elem->typedef_name, name)) {
            return elem;
        }
    }
    return NULL;
}

Enum *Symbols_getEnum(const Symbols *sym, const char *name) {
    DS_LIST_PTR_FOREACH (sym->enums, elem) {
        if (!strcmp(elem->name, name)) {
            return elem;
        }
    }
    return NULL;
}

bool Symbols_appendFunction(Symbols *sym, char *name, FunctionType funcType) {
    Function *f = Symbols_getFunction(sym, name);
    if (f) {
        free(name);
        // appending funcType
        if (!FunctionType_array_append(f->functionTypes, funcType))
            goto raise_error;

        f->funcCount++; // incrementing new func decl in same name

        return true;
    }
    // No function found with given name
    // creating struct Function
    Function q;
    q.name = name;

    // creating array_FunctionType_t and appending funcType
    q.functionTypes = FunctionType_array_new();
    if (!(q.functionTypes))
        goto raise_error;

    if (!FunctionType_array_append(q.functionTypes, funcType))
        goto raise_error;

    q.funcCount = 1; // incrementing new func decl in same name

    // inserting the struct Function to symbols
    if (!Function_list_append(sym->funcs, q))
        goto raise_error;

    sym->funcCount++; // incrementing function count in symbols

    return true;

raise_error:
    if (errno == ENOMEM)
        PyErr_NoMemory();
    else if (errno == EINVAL)
        PyErr_SetString(py_BindingError,
                        "Incompatible function name found in translation unit");
    else
        PyErr_SetString(py_BindingError, "Unknown error occured");
    return false;
}

bool Symbols_clearFunction(Function *func) {
    FunctionType_array_clear(func->functionTypes);
    free(func->name);
    return true;
}

bool Symbols_clearFunctionType(FunctionType *funcType) {
    p_ffi_type_array_clear(funcType->argsTypeFFI);
    CXX_Type_array_clear(funcType->argsTypeCXX);
    p_void_array_clear(funcType->argsTypeInfo);
    free(funcType->mangledName);
    return true;
}

ffi_type Build_Structure_ffi_type(const p_ffi_type_array_t *ffi_type_list) {
    ffi_type type  = {0};
    type.size      = 0;
    type.alignment = 0;
    type.type      = FFI_TYPE_STRUCT;

    size_t len    = p_ffi_type_array_size(ffi_type_list);
    type.elements = calloc(len + 1, sizeof(ffi_type *));

    DS_ARRAY_FOREACH (ffi_type_list, elem) { type.elements[_i] = elem; }
    type.elements[len] = NULL;

    ffi_cif cif;
    if (ffi_prep_cif(&cif, FFI_DEFAULT_ABI, 0, &type, NULL) != FFI_OK) {
        free(type.elements);
        // PyErr_SetString(py_BindingError,
        //                 "Could not figure out attribute type of the given
        //                 struct");
        return (ffi_type){0};
    }

    return type;
}

bool Symbols_appendStructure(Symbols *sym, Structure s) {
    s.type = Build_Structure_ffi_type(s.attrTypeFFI);
    // if (PyErr_Occurred()) {
    //   return false;
    // }

    if (!Symbols_getStructure(sym, s.name)) {

        if (!Structure_list_append(sym->structs, s)) {
            free(s.type.elements);
            goto raise_error;
        }

        sym->structsCount++;
    } else {
        DS_LIST_FOREACH (sym->structs, elem) {
            if (strcmp(elem.name, s.name) != 0) {
                continue;
            }

            if (!Structure_list_setat(sym->structs, s, _i)) {
                free(s.type.elements);
                goto raise_error;
            }
            break;
        }
    }

    return true;

raise_error:
    if (errno == ENOMEM)
        PyErr_NoMemory();
    else if (errno == EINVAL)
        PyErr_SetString(
            py_BindingError,
            "Incompatible structure name found in translation unit");
    else
        PyErr_SetString(py_BindingError, "Unknown error occured");
    return false;
}

bool Symbols_clearStructure(Structure *structure) {
    str_array_clear(structure->attrNames);
    p_ffi_type_array_clear(structure->attrTypeFFI);
    p_void_array_clear(structure->attrTypeInfo);
    CXX_Type_array_clear(structure->attrTypeCXX);
    long_long_array_clear(structure->offsets);
    free(structure->name);
    free(structure->type.elements);

    return true;
}

ffi_type Build_Union_ffi_type(const p_ffi_type_array_t *ffi_type_list) {
    ffi_type type;
    type.type = FFI_TYPE_STRUCT;
    type.size = type.alignment = 0;

    size_t len    = p_ffi_type_array_size(ffi_type_list);
    type.elements = calloc(len + 1, sizeof(ffi_type *));

    DS_ARRAY_FOREACH (ffi_type_list, elem) {
        type.elements[_i] = elem;

        ffi_cif cif;
        if (ffi_prep_cif(&cif, FFI_DEFAULT_ABI, 0, type.elements[_i], NULL) ==
            FFI_OK) {
            if (type.elements[_i]->size > type.size) {
                type.size = type.elements[_i]->size;
            }
            if (type.elements[_i]->alignment > type.alignment) {
                type.alignment = type.elements[_i]->alignment;
            }
        }
    }

    type.elements[len] = NULL;

    return type;
}

bool Symbols_appendUnion(Symbols *sym, Union u) {
    ffi_type union_type = Build_Union_ffi_type(u.attrTypeFFI);

    if (!Symbols_getUnion(sym, u.name)) {

        if (!Union_list_append(sym->unions, u)) {
            free(union_type.elements);
            goto raise_error;
        }

        sym->unionsCount++;

    } else {

        DS_LIST_PTR_FOREACH (sym->unions, elem) {
            if (strcmp(elem->name, u.name) != 0) {
                continue;
            }

            if (!Union_list_setat(sym->unions, u, _i)) {
                free(union_type.elements);
                goto raise_error;
            }
        }
    }
    return true;

raise_error:
    if (errno == ENOMEM)
        PyErr_NoMemory();
    else if (errno == EINVAL)
        PyErr_SetString(py_BindingError,
                        "Incompatible union name found in translation unit");
    else
        PyErr_SetString(py_BindingError, "Unknown error occured");
    return false;
}

bool Symbols_clearUnion(Union *_union) {
    str_array_clear(_union->attrNames);
    p_ffi_type_array_clear(_union->attrTypeFFI);
    CXX_Type_array_clear(_union->attrTypeCXX);
    p_void_array_clear(_union->attrTypeInfo);
    free(_union->name);
    free(_union->type.elements);
    return true;
}

bool Symbols_appendGlobal(Symbols *sym, Global g) {
    if (Global_list_append(sym->globals, g)) {
        sym->globalsCount++;
        return true;
    }

    if (errno == ENOMEM)
        PyErr_NoMemory();
    else if (errno == EINVAL)
        PyErr_SetString(py_BindingError,
                        "Incompatible global name found in translation unit");
    else
        PyErr_SetString(py_BindingError, "Unknown error occured");
    return false;
}

bool Symbols_clearGlobal(Global *global) {
    free(global->name);
    return true;
}

bool Symbols_appendTypedef(Symbols *sym, char *typedef_name, char *type_name,
                           enum CXX_Type type, void *extra_info) {
    TypeDef typeDef;
    typeDef.name         = type_name;
    typeDef.typedef_name = typedef_name;
    typeDef.type         = type;
    typeDef.extraInfo    = extra_info;

    if (TypeDef_list_append(sym->typedefs, typeDef)) {
        sym->typedefsCount++;
        return true;
    }

    if (errno == ENOMEM)
        PyErr_NoMemory();
    else
        PyErr_SetString(py_BindingError, "Unknown error occured");

    return false;
}

bool Symbols_clearTypedef(TypeDef *td) {
    free(td->name);
    free(td->typedef_name);

    return true;
}

bool Symbols_appendEnum(Symbols *sym, char *name, long value) {
    if (Enum_list_append(sym->enums, (Enum){name, value})) {
        sym->enumsCount++;
        return true;
    }

    if (errno == ENOMEM)
        PyErr_NoMemory();
    else
        PyErr_SetString(py_BindingError, "Unknown error occured");

    return false;
}

bool Symbols_clearEnum(Enum *e) {
    free(e->name);
    return true;
}

Symbols *create_Symbol(char *name) {
    Symbols *sym = malloc(sizeof(Symbols));
    sym->name    = name;

    sym->funcs     = Function_list_new();
    sym->funcCount = 0;

    sym->structs      = Structure_list_new();
    sym->structsCount = 0;

    sym->unions      = Union_list_new();
    sym->unionsCount = 0;

    sym->globals      = Global_list_new();
    sym->globalsCount = 0;

    sym->typedefs      = TypeDef_list_new();
    sym->typedefsCount = 0;

    sym->enums      = Enum_list_new();
    sym->enumsCount = 0;

    return sym;
}

void free_Symbols(Symbols *sym) {
    Function_list_clear(sym->funcs);

    Structure_list_clear(sym->structs);

    Union_list_clear(sym->unions);

    Global_list_clear(sym->globals);

    TypeDef_list_clear(sym->typedefs);

    Enum_list_clear(sym->enums);

    free(sym);
}

bool Symbols_parse(Symbols *sym, const char *header) {
    CXIndex           index = clang_createIndex(1, 0);
    CXTranslationUnit unit;

    // TODO: support comments with
    // CXTranslationUnit_IncludeBriefCommentsInCodeCompletion
    enum CXErrorCode err_code = clang_parseTranslationUnit2(
        index, header, NULL, 0, NULL, 0, CXTranslationUnit_SkipFunctionBodies,
        &unit);

    if (err_code != CXError_Success)
        return false;

    CXCursor cursor = clang_getTranslationUnitCursor(unit);
    clang_visitChildren(cursor, visitor, sym);

    if (PyErr_Occurred())
        return false;

    clang_disposeTranslationUnit(unit);
    clang_disposeIndex(index);

    return true;
}

void *get_underlyingTypeInfo(CXType cxType, const Symbols *sym,
                             enum CXX_Type *cxx_type,
                             ffi_type     **objects_ffi_type) {
    // TODO: set and raise error; if could not figure out type
    CXType underlying_type = cxType;

    if (cxType.kind == CXType_Typedef) {
        underlying_type = clang_getTypedefDeclUnderlyingType(
            clang_getTypeDeclaration(cxType));
        return get_underlyingTypeInfo(underlying_type, sym, cxx_type,
                                      objects_ffi_type);

    } else if (cxType.kind == CXType_Elaborated) {
        underlying_type = clang_Type_getNamedType(cxType);
        return get_underlyingTypeInfo(underlying_type, sym, cxx_type,
                                      objects_ffi_type);

    } else if (cxType.kind == CXType_Pointer) {
        underlying_type = clang_getPointeeType(cxType);

        if (underlying_type.kind == CXType_Pointer) {
            // The CXType is a pointer to pointer, will return void*
            *cxx_type         = CXX_VoidPointer;
            *objects_ffi_type = NULL;
            return NULL;
        }

        *cxx_type |= POINTER;
        *objects_ffi_type = &ffi_type_pointer;
        return get_underlyingTypeInfo(underlying_type, sym, cxx_type,
                                      objects_ffi_type);

    } else if (cxType.kind == CXType_Record) {
        char *record_name =
            clangString_to_CString(clang_getTypeSpelling(cxType));

        if (strncmp(record_name, "struct ", 7) == 0) {
            *cxx_type |= CXX_Struct;
            Structure *underlying_struct =
                Symbols_getStructure(sym, record_name + 7);
            *objects_ffi_type = &(underlying_struct->type);

            free(record_name);
            return underlying_struct;
        }

        if (strncmp(record_name, "union ", 6) == 0) {
            *cxx_type |= CXX_Union;
            Union *underlying_union = Symbols_getUnion(sym, record_name + 6);
            *objects_ffi_type       = &(underlying_union->type);

            free(record_name);
            return underlying_union;
        }

        free(record_name);

        *cxx_type         = CXX_NotDefined;
        *objects_ffi_type = NULL;
        return NULL;

    } else if (cxType.kind == CXType_Enum) {
        return get_underlyingTypeInfo(
            clang_getEnumDeclIntegerType(clang_getTypeDeclaration(cxType)), sym,
            cxx_type, objects_ffi_type);
    }

    // else
    switch (underlying_type.kind) {
    case CXType_Void:
        *cxx_type |= CXX_Void;
        *objects_ffi_type =
            *objects_ffi_type ? *objects_ffi_type : &ffi_type_void;
        break;
    case CXType_Char_U:
    case CXType_UChar:
        *cxx_type |= CXX_UChar;
        *objects_ffi_type =
            *objects_ffi_type ? *objects_ffi_type : &ffi_type_uchar;
        break;
    case CXType_Char_S:
    case CXType_SChar:
        *cxx_type |= CXX_Char;
        *objects_ffi_type =
            *objects_ffi_type ? *objects_ffi_type : &ffi_type_schar;
        break;
    case CXType_Bool:
        *cxx_type |= CXX_Bool;
        *objects_ffi_type =
            *objects_ffi_type ? *objects_ffi_type : &ffi_type_uint8;
        break;
    case CXType_Short:
        *cxx_type |= CXX_Short;
        *objects_ffi_type =
            *objects_ffi_type ? *objects_ffi_type : &ffi_type_sshort;
        break;
    case CXType_UShort:
        *cxx_type |= CXX_UShort;
        *objects_ffi_type =
            *objects_ffi_type ? *objects_ffi_type : &ffi_type_ushort;
        break;
    case CXType_Int:
        *cxx_type |= CXX_Int;
        *objects_ffi_type =
            *objects_ffi_type ? *objects_ffi_type : &ffi_type_sint;
        break;
    case CXType_UInt:
        *cxx_type |= CXX_UInt;
        *objects_ffi_type =
            *objects_ffi_type ? *objects_ffi_type : &ffi_type_uint;
        break;
    case CXType_Long:
        *cxx_type |= CXX_Long;
        *objects_ffi_type =
            *objects_ffi_type ? *objects_ffi_type : &ffi_type_slong;
        break;
    case CXType_ULong:
        *cxx_type |= CXX_ULong;
        *objects_ffi_type =
            *objects_ffi_type ? *objects_ffi_type : &ffi_type_ulong;
        break;
    case CXType_LongLong:
        *cxx_type |= CXX_LongLong;
        *objects_ffi_type =
            *objects_ffi_type ? *objects_ffi_type
                              : &ffi_type_slong; // FIXME: no ffi_type long long
        break;
    case CXType_ULongLong:
        *cxx_type |= CXX_ULongLong;
        *objects_ffi_type =
            *objects_ffi_type ? *objects_ffi_type
                              : &ffi_type_ulong; // FIXME: no ffi_type long long
        break;
    case CXType_Float:
        *cxx_type |= CXX_Float;
        *objects_ffi_type =
            *objects_ffi_type ? *objects_ffi_type : &ffi_type_float;
        break;
    case CXType_Double:
        *cxx_type |= CXX_Double;
        *objects_ffi_type =
            *objects_ffi_type ? *objects_ffi_type : &ffi_type_double;
        break;
    default:
        *cxx_type         = CXX_NotDefined;
        *objects_ffi_type = NULL;
    }
    return NULL;
}

enum CXChildVisitResult enum_visitor(CXCursor cursor, CXCursor parent,
                                     CXClientData client_data) {
    // TODO: group enum's with parents

    long long value = clang_getEnumConstantDeclValue(cursor);
    char     *name  = clangString_to_CString(clang_getCursorSpelling(cursor));

    if (!Symbols_appendEnum(client_data, name, value)) {
        free(name);
        return CXChildVisit_Break;
    }

    return CXChildVisit_Continue;
}

enum CXChildVisitResult record_visitor(CXCursor cursor, CXCursor parent,
                                       CXClientData client_data) {
    void              **info      = client_data;
    struct _RecordType *structure = (struct _RecordType *)info[0];
    const Symbols      *sym       = (Symbols *)info[1];

    enum CXX_Type cxx_attr_type = 0;
    ffi_type     *ffi_attr_type = NULL;

    CXType attr_type = clang_getCursorType(cursor);

    void *extra_arg_type_info =
        get_underlyingTypeInfo(attr_type, sym, &cxx_attr_type, &ffi_attr_type);

    if (cxx_attr_type == CXX_NotDefined || ffi_attr_type == NULL) {
        // could not get type info
        return CXChildVisit_Break;
    }

    char *name = clangString_to_CString(clang_getCursorSpelling(cursor));
    str_array_append(structure->attrNames, name);

    CXX_Type_array_append(structure->attrTypeCXX, cxx_attr_type);
    p_ffi_type_array_append(structure->attrTypeFFI, ffi_attr_type);
    p_void_array_append(structure->attrTypeInfo, extra_arg_type_info);

    structure->attrCount++;

    return CXChildVisit_Continue;
}

FunctionType Function_parser(CXCursor funcCursor, const Symbols *symbols) {
    FunctionType func = {0};

    enum CXX_Type cxx_return_type = 0;
    ffi_type     *ffi_return_type = NULL;

    void *extra_return_type_info = get_underlyingTypeInfo(
        clang_getResultType(clang_getCursorType(funcCursor)), symbols,
        &cxx_return_type, &ffi_return_type);

    if (cxx_return_type == CXX_NotDefined || ffi_return_type == NULL) {
        // could not get type info
        return (FunctionType){0};
    }

    func.returnTypeCXX  = cxx_return_type;
    func.returnTypeFFI  = ffi_return_type;
    func.returnTypeInfo = extra_return_type_info;

    int args_len = clang_Cursor_getNumArguments(funcCursor);

    func.argsCount    = args_len;
    func.argsTypeCXX  = CXX_Type_array_new();
    func.argsTypeFFI  = p_ffi_type_array_new();
    func.argsTypeInfo = p_void_array_new();

    for (int i = 0; i < args_len; i++) {
        CXCursor arg      = clang_Cursor_getArgument(funcCursor, i);
        CXType   arg_type = clang_getCursorType(arg);

        enum CXX_Type cxx_arg_type = 0;
        ffi_type     *ffi_arg_type = NULL;

        void *extra_arg_type_info = get_underlyingTypeInfo(
            arg_type, symbols, &cxx_arg_type, &ffi_arg_type);

        if (cxx_arg_type == CXX_NotDefined || ffi_arg_type == NULL) {
            // could not get type info
            CXX_Type_array_clear(func.argsTypeCXX);
            p_ffi_type_array_clear(func.argsTypeFFI);
            p_void_array_clear(func.argsTypeInfo);
            return (FunctionType){0};
        }

        CXX_Type_array_append(func.argsTypeCXX, cxx_arg_type);
        p_ffi_type_array_append(func.argsTypeFFI, ffi_arg_type);
        p_void_array_append(func.argsTypeInfo, extra_arg_type_info);
    }

    func.mangledName = clangString_to_CString(GET_MANGLED_NAME(funcCursor));
    func.func        = NULL;

    return func;
}

Union Union_parser(CXCursor unionCursor, Symbols *symbols) {
    char *name = clangString_to_CString(clang_getCursorSpelling(unionCursor));

    Union u;
    u.name         = name;
    u.attrNames    = str_array_new();
    u.attrTypeFFI  = p_ffi_type_array_new();
    u.attrTypeCXX  = CXX_Type_array_new();
    u.attrTypeInfo = p_void_array_new();
    u.type         = (ffi_type){0, 0, 0, NULL};
    u.attrCount    = 0;
    u.unionSize    = clang_Type_getSizeOf(clang_getCursorType(unionCursor));

    void *info[] = {&u, symbols};
    clang_visitChildren(unionCursor, record_visitor, info);

    return u;
}

Structure Structure_parser(CXCursor structCursor, Symbols *symbols) {
    char *name = clangString_to_CString(clang_getCursorSpelling(structCursor));

    Structure structure;
    structure.name         = name;
    structure.attrNames    = str_array_new();
    structure.attrTypeFFI  = p_ffi_type_array_new();
    structure.attrTypeCXX  = CXX_Type_array_new();
    structure.attrTypeInfo = p_void_array_new();
    structure.offsets      = long_long_array_new();
    structure.type         = (ffi_type){0, 0, 0, NULL};
    structure.attrCount    = 0;
    structure.structSize =
        clang_Type_getSizeOf(clang_getCursorType(structCursor));

    void *info[] = {&structure, symbols};
    clang_visitChildren(structCursor, record_visitor, info);

    // offset
    for (int i = 0; i < structure.attrCount; i++) {
        long_long_array_append(
            structure.offsets,
            clang_Type_getOffsetOf(clang_getCursorType(structCursor),
                                   str_array_getat(structure.attrNames, i)));
    }

    return structure;
}

enum CXChildVisitResult visitor(CXCursor cursor, CXCursor parent,
                                CXClientData client_data) {
    Symbols *symbols = client_data;

    // function
    if (clang_getCursorKind(cursor) == CXCursor_FunctionDecl) {
        char *func_name =
            clangString_to_CString(clang_getCursorSpelling(cursor));

        FunctionType func_type = Function_parser(cursor, symbols);

        if (func_type.mangledName == NULL) {
            free(func_name);
            return CXChildVisit_Break;
        }

        if (!Symbols_appendFunction(symbols, func_name, func_type)) {
            free(func_name);
            return CXChildVisit_Break;
        }
    }

    // struct
    else if (clang_getCursorKind(cursor) == CXCursor_StructDecl) {

        Structure structure = Structure_parser(cursor, symbols);

        if (!Symbols_appendStructure(symbols, structure)) {
            return CXChildVisit_Break;
        }
    }

    // unions
    else if (clang_getCursorKind(cursor) == CXCursor_UnionDecl) {
        Union _union = Union_parser(cursor, symbols);

        if (!Symbols_appendUnion(symbols, _union)) {
            return CXChildVisit_Break;
        }
    }

    // typedef
    else if (clang_getCursorKind(cursor) == CXCursor_TypedefDecl) {
        CXType underlying_type = clang_getTypedefDeclUnderlyingType(cursor);
        CXType cursor_cxtype   = clang_getCursorType(cursor);
        char  *typedef_name =
            clangString_to_CString(clang_getTypedefName(cursor_cxtype));
        char *type_name =
            clangString_to_CString(clang_getTypeSpelling(underlying_type));

        enum CXX_Type td_cxx_type = 0;
        ffi_type     *td_ffi_type = NULL;

        void *underlying_type_info = get_underlyingTypeInfo(
            underlying_type, symbols, &td_cxx_type, &td_ffi_type);

        if (!Symbols_appendTypedef(symbols, typedef_name, type_name,
                                   td_cxx_type, underlying_type_info)) {
            free(type_name);
            free(typedef_name);
            return CXChildVisit_Break;
        }
    }

    // global variables
    else if (clang_getCursorKind(cursor) == CXCursor_VarDecl) {
        // TODO: add support for struct & unions
        CXType type = clang_getCursorType(cursor);

        Global global = {0};
        global.name   = clangString_to_CString(clang_getCursorSpelling(cursor));

        void *underlying_type_info = get_underlyingTypeInfo(
            type, symbols, &(global.typeCXX), &(global.typeFFI));

        global.extraInfo = underlying_type_info;

        if (!Symbols_appendGlobal(symbols, global)) {
            free(global.name);
            return CXChildVisit_Break;
        }
    }

    // enums
    else if (clang_getCursorKind(cursor) == CXCursor_EnumDecl) {
        clang_visitChildren(cursor, enum_visitor, symbols);
    }

    // else
    else {
        // raising error because unknow type found
        char *name = clangString_to_CString(clang_getCursorSpelling(cursor));
        char *type_name = clangString_to_CString(
            clang_getTypeSpelling(clang_getCursorType(cursor)));

        PyErr_Format(py_BindingError,
                     "Could not figure out type info\nGiven cursor: %s: cursor "
                     "kind: %i\n\ttype: %s type kind: %i",
                     name, clang_getCursorKind(cursor), type_name,
                     clang_getCursorType(cursor).kind);

        free(name);
        free(type_name);

        return CXChildVisit_Break;
    }

    return CXChildVisit_Continue;
}
