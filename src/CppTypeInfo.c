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

void str_clear(char *s) { free(s); }

DS_ARRAY_DEF(str, char *, str_clear);
DS_ARRAY_DEF(CXTypeKind, enum CXTypeKind, nullFunc);
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
DS_LIST_DEF(Class, Class, nullFunc);

enum CXChildVisitResult visitor(CXCursor cursor, CXCursor parent,
                                CXClientData client_data);

const char *clangString_to_CString(CXString str) {
  const char *s = clang_getCString(str);
  s = strdup(s);
  clang_disposeString(str);
  return s;
}

void print_Function(Function func) {
  printf("    %s\n", func.name);

  assert(FunctionType_array_size(func.functionTypes) == func.funcCount);

  for (size_t i = 0; i < func.funcCount; i++) {
    FunctionType type = FunctionType_array_getat(func.functionTypes, i);
    const char *mangledName = type.mangledName;

    if (type.returnsUnderlyingType != CXType_Invalid) {
      printf("      %s %s(", CXTypeKind_TO_char_p(type.returnsUnderlyingType),
             mangledName);
    } else {
      printf("      %s %s(", ffi_type_To_char_p(type.returnType), mangledName);
    }

    assert(type.argsCount == p_ffi_type_array_size(type.argsType));
    assert(type.argsCount == CXTypeKind_array_size(type.argsUnderlyingType));

    for (size_t j = 0; j < type.argsCount; j++) {
      ffi_type *ffi_type_p = p_ffi_type_array_getat(type.argsType, j);
      if (CXTypeKind_array_getat(type.argsUnderlyingType, j)) {
        printf("%s, ", CXTypeKind_TO_char_p(
                           CXTypeKind_array_getat(type.argsUnderlyingType, j)));
      } else {
        printf("%s, ", ffi_type_To_char_p(*ffi_type_p));
      }
    }
    printf(")\n");
  }
}

void print_Structure(Structure structure) {
  str_array_t *attrNames = structure.attrNames;
  p_ffi_type_array_t *attrTypes = structure.attrTypes;
  size_t attrCount = structure.attrCount;

  assert(str_array_size(attrNames) == attrCount);
  assert(p_ffi_type_array_size(attrTypes) == attrCount);

  printf("    struct %s\n", structure.name);

  for (size_t i = 0; i < attrCount; i++) {
    ffi_type *attrType = p_ffi_type_array_getat(attrTypes, i);
    const char *attrName = str_array_getat(attrNames, i);

    printf("      %s: %s\n", attrName, ffi_type_To_char_p(*attrType));
  }
}

void print_Union(Union _union) {
  str_array_t *attrNames = _union.attrNames;
  p_ffi_type_array_t *attrTypes = _union.attrTypes;
  size_t attrCount = _union.attrCount;

  assert(str_array_size(attrNames) == attrCount);
  assert(p_ffi_type_array_size(attrTypes) == attrCount);

  printf("    union %s\n", _union.name);

  for (size_t i = 0; i < attrCount; i++) {
    ffi_type *attrType = p_ffi_type_array_getat(attrTypes, i);
    const char *attrName = str_array_getat(attrNames, i);

    printf("      %s: %s\n", attrName, ffi_type_To_char_p(*attrType));
  }
}

void print_Global(Global global) {
  printf("    %s: %s\n", global.name, ffi_type_To_char_p(global.type));
}

void print_Class(Class cls) {
  // TODO: implement print_Class
}

void print_Symbols(Symbols *symbols) {
  printf("Name: %s\n", symbols->name);

  size_t len;

  // printing function info
  printf("\n  Functions: \n");

  len = Function_list_size(symbols->funcs);
  assert(len == symbols->funcCount);
  for (size_t i = 0; i < len; i++) {
    Function s = Function_list_getat(symbols->funcs, i);
    print_Function(s);
  }

  // printing structure info
  printf("\n  Structures: \n");

  len = Structure_list_size(symbols->structs);
  assert(len == symbols->structsCount);
  for (size_t i = 0; i < len; i++) {
    Structure s = Structure_list_getat(symbols->structs, i);
    print_Structure(s);
  }

  // printing Union info
  printf("\n  Union: \n");

  len = Union_list_size(symbols->unions);
  assert(len == symbols->unionsCount);
  for (size_t i = 0; i < len; i++) {
    Union u = Union_list_getat(symbols->unions, i);
    print_Union(u);
  }

  // printing globals info
  printf("\n  Globals: \n");

  len = Global_list_size(symbols->globals);
  assert(len == symbols->globalsCount);
  for (size_t i = 0; i < len; i++) {
    Global s = Global_list_getat(symbols->globals, i);
    print_Global(s);
  }

  // printing classes info
  // TODO: implement printing classes info
}

Function *Symbols_getFunction(Symbols *sym, const char *name) {
  for (size_t i = 0; i < sym->funcCount; i++) {
    Function *func = Function_list_get_ptr_at(sym->funcs, i);

    if (!strcmp(func->name, name)) {
      return func;
    }
  }
  errno = 0;
  return NULL;
}

Structure *Symbols_getStructure(Symbols *sym, const char *name) {
  for (size_t i = 0; i < sym->structsCount; i++) {
    Structure *func = Structure_list_get_ptr_at(sym->structs, i);

    if (!strcmp(func->name, name)) {
      return func;
    }
  }
  errno = 0;
  return NULL;
}

Union *Symbols_getUnion(Symbols *sym, const char *name) {
  for (size_t i = 0; i < sym->unionsCount; i++) {
    Union *func = Union_list_get_ptr_at(sym->unions, i);

    if (!strcmp(func->name, name)) {
      return func;
    }
  }
  errno = 0;
  return NULL;
}

Global *Symbols_getGlobal(Symbols *sym, const char *name) {
  for (size_t i = 0; i < sym->globalsCount; i++) {
    Global *func = Global_list_get_ptr_at(sym->globals, i);

    if (!strcmp(func->name, name)) {
      return func;
    }
  }
  errno = 0;
  return NULL;
}

Class *Symbols_getClass(Symbols *sym, const char *name) {
  // TODO: implement Symbols_getClass
  return NULL;
}

TypeDef *Symbols_getTypeDef(Symbols *sym, const char *name) {
  for (size_t i = 0; i < sym->typedefsCount; i++) {
    TypeDef *func = TypeDef_list_get_ptr_at(sym->typedefs, i);

    if (!strcmp(func->name, name)) {
      return func;
    }
  }
  errno = 0;
  return NULL;
}

bool Symbols_appendFunction(Symbols *sym, const char *name,
                            const char *mangledName, FunctionType funcType) {
  Function *f = Symbols_getFunction(sym, name);
  if (f) {
    free((void *)name);
    // appending funcType
    if (!FunctionType_array_append(f->functionTypes, funcType))
      goto raise_error;

    f->funcCount++; // incrementing new func decl in same name

    return true;
  } else {
    // creating struct Functiom
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
  }

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

bool Symbols_clearFunction(Function s) {
  FunctionType_array_clear(s.functionTypes);
  free((void *)s.name);
  return true;
}

bool Symbols_clearFunctionType(FunctionType s) {
  p_ffi_type_array_clear(s.argsType);
  CXTypeKind_array_clear(s.argsUnderlyingType);
  p_Structure_array_clear(s.argsUnderlyingStructs);
  p_Union_array_clear(s.argsUnderlyingUnions);
  free((void *)s.mangledName);
  return true;
}

bool Symbols_appendStructure(Symbols *sym, Structure s) {
  // createing ffi_type for the given struct
  s.type.size = 0;
  s.type.alignment = 0;
  s.type.type = FFI_TYPE_STRUCT;

  if (s.attrCount) {
    s.type.elements = calloc(s.attrCount + 1, sizeof(ffi_type *));

    for (size_t i = 0; i < s.attrCount; i++) {
      s.type.elements[i] = p_ffi_type_array_getat(s.attrTypes, i);
    }
    s.type.elements[s.attrCount] = NULL;

    ffi_cif cif;
    if (ffi_prep_cif(&cif, FFI_DEFAULT_ABI, 0, &s.type, NULL) != FFI_OK) {
      PyErr_SetString(
          py_BindingError,
          "Could not figure out attribute type of the given struct");
      return false;
    }
  }

  Structure *old_entry;
  if ((NULL == (old_entry = Symbols_getStructure(sym, s.name)))) {

    if (!Structure_list_append(sym->structs, s)) {
      free(s.type.elements);
      if (errno == ENOMEM)
        PyErr_NoMemory();
      else if (errno == EINVAL)
        PyErr_SetString(py_BindingError,
                        "Incompatible union name found in translation unit");
      else
        PyErr_SetString(py_BindingError, "Unknown error occured");
      return false;
    }

    sym->structsCount++;
  } else {
    if (old_entry->type.elements) {
      free(old_entry->type.elements);
    }

    for (size_t i = 0; i < sym->structsCount; i++) {
      Structure S = Structure_list_getat(sym->structs, i);

      if (!strcmp(S.name, s.name)) {
        Structure_list_setat(sym->structs, s, i);
        break;
      }
    }
  }

  return true;
}

bool Symbols_clearStructure(Structure s) {
  str_array_clear(s.attrNames);
  p_ffi_type_array_clear(s.attrTypes);
  CXTypeKind_array_clear(s.attrUnderlyingType);
  p_Structure_array_clear(s.attrUnderlyingStructs);
  p_Union_array_clear(s.attrUnderlyingUnions);
  long_long_array_clear(s.offsets);
  free((void *)s.name);

  if (s.type.elements) {
    free(s.type.elements);
  }

  return true;
}

bool Symbols_appendUnion(Symbols *sym, Union u) {
  ffi_type union_type;
  union_type.size = union_type.alignment = 0;
  union_type.type = FFI_TYPE_STRUCT;

  if (u.attrCount) {
    ffi_type **union_elements = calloc(u.attrCount + 1, sizeof(ffi_type *));
    for (size_t i = 0; i < u.attrCount; i++) {
      union_elements[i] = p_ffi_type_array_getat(u.attrTypes, i);

      ffi_cif cif;
      if (ffi_prep_cif(&cif, FFI_DEFAULT_ABI, 0, union_elements[i], NULL) ==
          FFI_OK) {
        if (union_elements[i]->size > union_type.size) {
          union_type.size = union_elements[i]->size;
        }
        if (union_elements[i]->alignment > union_type.alignment)
          union_type.alignment = union_elements[i]->alignment;
      }
    }
    union_elements[u.attrCount] = NULL;

    union_type.elements = union_elements;

    u.type = union_type;
  }

  Union *old_entry;
  if ((NULL == (old_entry = Symbols_getUnion(sym, u.name)))) {

    if (!Union_list_append(sym->unions, u)) {
      free(union_type.elements);
      if (errno == ENOMEM)
        PyErr_NoMemory();
      else if (errno == EINVAL)
        PyErr_SetString(py_BindingError,
                        "Incompatible union name found in translation unit");
      else
        PyErr_SetString(py_BindingError, "Unknown error occured");
      return false;
    }

    sym->unionsCount++;
  } else {
    if (old_entry->type.elements) {
      free(old_entry->type.elements);
    }
    for (size_t i = 0; i < sym->unionsCount; i++) {
      Union U = Union_list_getat(sym->unions, i);
      Symbols_clearUnion(U);

      if (!strcmp(U.name, u.name)) {
        Union_list_setat(sym->unions, u, i);
        break;
      }
    }
  }

  return true;
}

bool Symbols_clearUnion(Union s) {
  str_array_clear(s.attrNames);
  p_ffi_type_array_clear(s.attrTypes);
  CXTypeKind_array_clear(s.attrUnderlyingType);
  p_Structure_array_clear(s.attrUnderlyingStructs);
  p_Union_array_clear(s.attrUnderlyingUnions);
  free((void *)s.name);

  if (s.type.elements) {
    free(s.type.elements);
  }

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

bool Symbols_clearGlobal(Global s) {
  free((void *)s.name);
  return true;
}

bool Symbols_appendTypedef(Symbols *sym, const char *typedef_name,
                           const char *type_name, CXType type) {
  TypeDef typeDef;
  typeDef.type_name = type_name;
  typeDef.name = typedef_name;
  typeDef.type = type.kind;

  if (type.kind == CXType_Pointer) {
    typeDef.underlying_type = clang_getPointeeType(type).kind;
  }

  if (!TypeDef_list_append(sym->typedefs, typeDef)) {
    if (errno == ENOMEM)
      PyErr_NoMemory();
    else
      PyErr_SetString(py_BindingError, "Unknown error occured");
    return false;
  }

  sym->typedefsCount++;

  return true;
}

bool Symbols_clearTypedef(TypeDef s) {
  free((void *)s.name);
  free((void *)s.type_name);

  return true;
}

bool Symbols_appendClass(Symbols *sym, Class c) {
  // TODO: implement Symbols_appendClass
  return true;
}

Symbols *create_Symbol(const char *name) {
  Symbols *sym = malloc(sizeof(Symbols));
  sym->name = name;

  sym->funcs = Function_list_new();
  sym->funcCount = 0;

  sym->structs = Structure_list_new();
  sym->structsCount = 0;

  sym->unions = Union_list_new();
  sym->unionsCount = 0;

  sym->globals = Global_list_new();
  sym->globalsCount = 0;

  sym->classes = Class_list_new();
  sym->classesCount = 0;

  sym->typedefs = TypeDef_list_new();
  sym->typedefsCount = 0;

  return sym;
}

void free_Symbols(Symbols *sym) {
  Function_list_clear(sym->funcs);

  Structure_list_clear(sym->structs);

  Union_list_clear(sym->unions);

  Global_list_clear(sym->globals);

  Class_list_clear(sym->classes);

  TypeDef_list_clear(sym->typedefs);

  free(sym);
}

bool Symbols_parse(Symbols *sym, const char *header) {
  CXIndex index = clang_createIndex(1, 0);
  CXTranslationUnit unit = clang_parseTranslationUnit(
      index, header, NULL, 0, NULL, 0, CXTranslationUnit_None);

  if (unit == NULL)
    return false;

  CXCursor cursor = clang_getTranslationUnitCursor(unit);
  clang_visitChildren(cursor, visitor, sym);

  if (PyErr_Occurred())
    return false;

  clang_disposeTranslationUnit(unit);
  clang_disposeIndex(index);

  return true;
}

CXType get_underlyingTypeInfo(CXType cxType, Symbols *sym, Structure **_s,
                              Union **_u, bool *_p) {
  CXType actual_type = cxType;

  if (cxType.kind == CXType_Typedef) {
    actual_type =
        clang_getTypedefDeclUnderlyingType(clang_getTypeDeclaration(cxType));
    return get_underlyingTypeInfo(actual_type, sym, _s, _u, _p);
  }

  else if (cxType.kind == CXType_Elaborated) {
    actual_type = clang_Type_getNamedType(cxType);
    return get_underlyingTypeInfo(actual_type, sym, _s, _u, _p);
  }

  else if (actual_type.kind == CXType_Pointer) {
    actual_type = clang_getPointeeType(cxType);
    *_p = true;
    return get_underlyingTypeInfo(actual_type, sym, _s, _u, _p);
  }

  else if (actual_type.kind == CXType_Record) {
    const char *record_name =
        clangString_to_CString(clang_getTypeSpelling(actual_type));

    if (strncmp(record_name, "struct ", 7) == 0) {
      Structure *s = Symbols_getStructure(sym, record_name + 7);
      *_s = s;

    } else {
      *_s = NULL;
    }

    if (strncmp(record_name, "union ", 6) == 0) {
      Union *u = Symbols_getUnion(sym, record_name + 6);
      *_u = u;
    } else {
      *_u = NULL;
    }
    free((void *)record_name);
  }

  return actual_type;
}

enum CXChildVisitResult union_visitor(CXCursor cursor, CXCursor parent,
                                      CXClientData client_data) {
  void **info = client_data;
  Union *obj = (Union *)info[0];
  Symbols *sym = (Symbols *)info[1];

  CXType attr_type = clang_getCursorType(cursor);

  const char *name = clangString_to_CString(clang_getCursorSpelling(cursor));
  str_array_append(obj->attrNames, (char *)name);

  Structure *underlyingAttrStructType = NULL;
  Union *underlyingAttrUnionType = NULL;
  bool isPointer = false;

  attr_type = get_underlyingTypeInfo(attr_type, sym, &underlyingAttrStructType,
                                     &underlyingAttrUnionType, &isPointer);

  if (isPointer) {
    p_ffi_type_array_append(obj->attrTypes, &ffi_type_pointer);

  } else if (underlyingAttrStructType) {
    p_ffi_type_array_append(obj->attrTypes, &(underlyingAttrStructType->type));

  } else if (underlyingAttrUnionType) {
    p_ffi_type_array_append(obj->attrTypes, &(underlyingAttrUnionType->type));

  } else {
    const char *tmp = clangString_to_CString(clang_getTypeSpelling(attr_type));
    ffi_type *return_ffi_type = get_ffi_type(attr_type, sym, tmp);
    free((void *)tmp);

    if (!return_ffi_type) {
      return CXChildVisit_Break;
    }

    p_ffi_type_array_append(obj->attrTypes, return_ffi_type);
  }

  p_Structure_array_append(obj->attrUnderlyingStructs,
                           underlyingAttrStructType);
  p_Union_array_append(obj->attrUnderlyingUnions, underlyingAttrUnionType);

  if (isPointer) {
    CXTypeKind_array_append(obj->attrUnderlyingType, attr_type.kind);

  } else {
    CXTypeKind_array_append(obj->attrUnderlyingType, 0);
  }

  obj->attrCount++;

  return CXChildVisit_Continue;
}

enum CXChildVisitResult struct_visitor(CXCursor cursor, CXCursor parent,
                                       CXClientData client_data) {
  void **info = client_data;
  Structure *obj = (Structure *)info[0];
  Symbols *sym = (Symbols *)info[1];

  CXType attr_type = clang_getCursorType(cursor);

  const char *name = clangString_to_CString(clang_getCursorSpelling(cursor));
  str_array_append(obj->attrNames, (char *)name);

  Structure *underlyingAttrStructType = NULL;
  Union *underlyingAttrUnionType = NULL;
  bool isPointer = false;

  attr_type = get_underlyingTypeInfo(attr_type, sym, &underlyingAttrStructType,
                                     &underlyingAttrUnionType, &isPointer);

  if (isPointer) {
    p_ffi_type_array_append(obj->attrTypes, &ffi_type_pointer);

  } else if (underlyingAttrStructType) {
    p_ffi_type_array_append(obj->attrTypes, &(underlyingAttrStructType->type));

  } else if (underlyingAttrUnionType) {
    p_ffi_type_array_append(obj->attrTypes, &(underlyingAttrUnionType->type));

  } else {
    const char *tmp = clangString_to_CString(clang_getTypeSpelling(attr_type));
    ffi_type *return_ffi_type = get_ffi_type(attr_type, sym, tmp);
    free((void *)tmp);

    if (!return_ffi_type) {
      return CXChildVisit_Break;
    }

    p_ffi_type_array_append(obj->attrTypes, return_ffi_type);
  }

  p_Structure_array_append(obj->attrUnderlyingStructs,
                           underlyingAttrStructType);
  p_Union_array_append(obj->attrUnderlyingUnions, underlyingAttrUnionType);

  if (isPointer) {
    CXTypeKind_array_append(obj->attrUnderlyingType, attr_type.kind);

  } else {
    CXTypeKind_array_append(obj->attrUnderlyingType, 0);
  }

  obj->attrCount++;

  return CXChildVisit_Continue;
}

enum CXChildVisitResult visitor(CXCursor cursor, CXCursor parent,
                                CXClientData client_data) {
  Symbols *symbols = client_data;

  // function
  if (clang_getCursorKind(cursor) == CXCursor_FunctionDecl) {
    // Function Declaration
    const char *funcName =
        clangString_to_CString(clang_getCursorSpelling(cursor));
    const char *mangledName = clangString_to_CString(GET_MANGLED_NAME(cursor));

    FunctionType funcType;
    funcType.mangledName = mangledName;
    funcType.func = NULL;

    Structure *underlyingReturnStructType = NULL;
    Union *underlyingReturnUnionType = NULL;
    bool isPointer = false;

    CXType returnType = get_underlyingTypeInfo(
        clang_getCursorResultType(cursor), symbols, &underlyingReturnStructType,
        &underlyingReturnUnionType, &isPointer);

    if (isPointer) {
      funcType.returnType = ffi_type_pointer;
    } else if (underlyingReturnStructType) {
      funcType.returnType = underlyingReturnStructType->type;
    } else if (underlyingReturnUnionType) {
      funcType.returnType = underlyingReturnUnionType->type;
    } else {
      const char *tmp =
          clangString_to_CString(clang_getTypeSpelling(returnType));

      ffi_type *return_ffi_type = get_ffi_type(returnType, symbols, tmp);
      free((void *)tmp);

      funcType.returnType = *return_ffi_type;

      if (!return_ffi_type) {
        return CXChildVisit_Break;
      }
    }

    funcType.returnUnderlyingStruct = underlyingReturnStructType;
    funcType.returnUnderlyingUnion = underlyingReturnUnionType;

    if (isPointer) {
      funcType.returnsUnderlyingType = returnType.kind;
    } else {
      funcType.returnsUnderlyingType = 0;
    }

    funcType.argsCount = clang_Cursor_getNumArguments(cursor);
    funcType.argsType = p_ffi_type_array_new();

    funcType.argsUnderlyingType = CXTypeKind_array_new();
    funcType.argsUnderlyingStructs = p_Structure_array_new();
    funcType.argsUnderlyingUnions = p_Union_array_new();

    for (int i = 0; i < funcType.argsCount; i++) {
      CXCursor arg = clang_Cursor_getArgument(cursor, i);
      CXType arg_type = clang_getCursorType(arg);

      Structure *underlyingArgStructType = NULL;
      Union *underlyingArgUnionType = NULL;
      bool isPointer = false;

      arg_type =
          get_underlyingTypeInfo(arg_type, symbols, &underlyingArgStructType,
                                 &underlyingArgUnionType, &isPointer);

      if (isPointer) {
        p_ffi_type_array_append(funcType.argsType, &ffi_type_pointer);

      } else if (underlyingArgStructType) {
        p_ffi_type_array_append(funcType.argsType,
                                &(underlyingArgStructType->type));

      } else if (underlyingArgUnionType) {
        p_ffi_type_array_append(funcType.argsType,
                                &(underlyingArgUnionType->type));

      } else {
        const char *tmp =
            clangString_to_CString(clang_getTypeSpelling(arg_type));
        ffi_type *return_ffi_type = get_ffi_type(arg_type, symbols, tmp);
        free((void *)tmp);

        if (!return_ffi_type) {
          return CXChildVisit_Break;
        }

        p_ffi_type_array_append(funcType.argsType, return_ffi_type);
      }

      p_Structure_array_append(funcType.argsUnderlyingStructs,
                               underlyingArgStructType);
      p_Union_array_append(funcType.argsUnderlyingUnions,
                           underlyingArgUnionType);

      if (isPointer) {
        CXTypeKind_array_append(funcType.argsUnderlyingType, arg_type.kind);

      } else {
        CXTypeKind_array_append(funcType.argsUnderlyingType, 0);
      }
    }

    if (!Symbols_appendFunction(symbols, funcName, mangledName, funcType)) {
      return CXChildVisit_Break;
    }
  }

  // struct
  else if (clang_getCursorKind(cursor) == CXCursor_StructDecl) {
    const char *name = clangString_to_CString(clang_getCursorSpelling(cursor));

    Structure obj;
    obj.name = name;
    obj.attrNames = str_array_new();
    obj.attrTypes = p_ffi_type_array_new();
    obj.attrUnderlyingType = CXTypeKind_array_new();
    obj.attrUnderlyingStructs = p_Structure_array_new();
    obj.attrUnderlyingUnions = p_Union_array_new();
    obj.offsets = long_long_array_new();
    obj.type = (ffi_type){0, 0, 0, NULL};
    obj.attrCount = 0;
    obj.structSize = clang_Type_getSizeOf(clang_getCursorType(cursor));

    void *info[] = {&obj, symbols};
    clang_visitChildren(cursor, struct_visitor, info);

    // offset
    for (int i = 0; i < obj.attrCount; i++) {
      long_long_array_append(
          obj.offsets,
          clang_Type_getOffsetOf(clang_getCursorType(cursor),
                                 str_array_getat(obj.attrNames, i)));
    }

    if (!Symbols_appendStructure(symbols, obj)) {
      return CXChildVisit_Break;
    }
  }

  // unions
  else if (clang_getCursorKind(cursor) == CXCursor_UnionDecl) {
    const char *name = clangString_to_CString(clang_getCursorSpelling(cursor));

    Union obj;
    obj.name = name;
    obj.attrNames = str_array_new();
    obj.attrTypes = p_ffi_type_array_new();
    obj.attrUnderlyingType = CXTypeKind_array_new();
    obj.attrUnderlyingStructs = p_Structure_array_new();
    obj.attrUnderlyingUnions = p_Union_array_new();
    obj.type = (ffi_type){0, 0, 0, NULL};
    obj.attrCount = 0;
    obj.unionSize = clang_Type_getSizeOf(clang_getCursorType(cursor));

    void *info[] = {&obj, symbols};
    clang_visitChildren(cursor, union_visitor, info);

    if (!Symbols_appendUnion(symbols, obj)) {
      return CXChildVisit_Break;
    }

  }
  // typedef
  else if (clang_getCursorKind(cursor) == CXCursor_TypedefDecl) {
    CXType underlying_type = clang_getTypedefDeclUnderlyingType(cursor);
    CXType cursor_cxtype = clang_getCursorType(cursor);
    CXString name = clang_getTypedefName(cursor_cxtype);
    const char *typedef_name = clangString_to_CString(name);
    const char *type_name =
        clangString_to_CString(clang_getTypeSpelling(underlying_type));

    if (!Symbols_appendTypedef(symbols, typedef_name, type_name,
                               underlying_type)) {
      return CXChildVisit_Break;
    }
  }

  // global variables
  else if (clang_getCursorKind(cursor) == CXCursor_VarDecl) {
    // TODO: add support for struct & unions
    CXType type = clang_getCursorType(cursor);

    Global global;
    global.name = clangString_to_CString(clang_getCursorSpelling(cursor));

    ffi_type *type_ffi_type = get_ffi_type(type, symbols, global.name);
    if (!type_ffi_type) {
      return CXChildVisit_Break;
    }

    global.type = *type_ffi_type;

    if (type.kind == CXType_Pointer) {
      global.underlyingType = clang_getPointeeType(type).kind;
    } else {
      global.underlyingType = 0;
    }

    if (!Symbols_appendGlobal(symbols, global)) {
      return CXChildVisit_Break;
    }
  } else {
    // raising error because unknow type found
    const char *name = clangString_to_CString(clang_getCursorSpelling(cursor));
    const char *type_name = clangString_to_CString(
        clang_getTypeSpelling(clang_getCursorType(cursor)));

    PyErr_Format(py_BindingError,
                 "Could not figure out type info\nGiven cursor: %s: cursor "
                 "kind: %i\n\ttype: %s type kind: %i",
                 name, clang_getCursorKind(cursor), type_name,
                 clang_getCursorType(cursor).kind);

    free((void *)name);
    free((void *)type_name);

    return CXChildVisit_Break;
  }

  return CXChildVisit_Continue;
}
