#define PY_SSIZE_T_CLEAN
#include "CppTypeInfo.h"
#include "DataStructures.h"
#include "PyC.h"
#include "ffi.h"
#include "clang-c/Index.h"
#include <assert.h>
#include <errno.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SIZE 1024

enum CXChildVisitResult visitor(CXCursor cursor, CXCursor parent,
                                CXClientData client_data);

void print_Function(Function func) {
  printf("    %s\n", func.name);

  assert(array_FunctionType_size(func.functionTypes) == func.funcCount);

  for (size_t i = 0; i < func.funcCount; i++) {
    FunctionType type = array_FunctionType_getat(func.functionTypes, i);
    const char *mangledName = type.mangledName;

    if (type.returnsUnderlyingType != CXType_Invalid) {
      printf("      %s %s(", CXTypeKind_TO_char_p(type.returnsUnderlyingType),
             mangledName);
    } else {
      printf("      %s %s(", ffi_type_To_char_p(type.returnType), mangledName);
    }

    assert(type.argsCount == array_p_ffi_type_size(type.argsType));
    assert(type.argsCount == array_CXTypeKind_size(type.argsUnderlyingType));

    for (size_t j = 0; j < type.argsCount; j++) {
      ffi_type *ffi_type_p = array_p_ffi_type_getat(type.argsType, j);
      if (array_CXTypeKind_getat(type.argsUnderlyingType, j)) {
        printf("%s, ", CXTypeKind_TO_char_p(
                           array_CXTypeKind_getat(type.argsUnderlyingType, j)));
      } else {
        printf("%s, ", ffi_type_To_char_p(*ffi_type_p));
      }
    }
    printf(")\n");
  }
}

void print_Structure(Structure structure) {
  array_str_t *attrNames = structure.attrNames;
  array_p_ffi_type_t *attrTypes = structure.attrTypes;
  size_t attrCount = structure.attrCount;

  assert(array_str_size(attrNames) == attrCount);
  assert(array_p_ffi_type_size(attrTypes) == attrCount);

  printf("    struct %s\n", structure.name);

  for (size_t i = 0; i < attrCount; i++) {
    ffi_type *attrType = array_p_ffi_type_getat(attrTypes, i);
    const char *attrName = array_str_getat(attrNames, i);

    printf("      %s: %s\n", attrName, ffi_type_To_char_p(*attrType));
  }
}

void print_Union(Union _union) {
  array_str_t *attrNames = _union.attrNames;
  array_p_ffi_type_t *attrTypes = _union.attrTypes;
  size_t attrCount = _union.attrCount;

  assert(array_str_size(attrNames) == attrCount);
  assert(array_p_ffi_type_size(attrTypes) == attrCount);

  printf("    union %s\n", _union.name);

  for (size_t i = 0; i < attrCount; i++) {
    ffi_type *attrType = array_p_ffi_type_getat(attrTypes, i);
    const char *attrName = array_str_getat(attrNames, i);

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

  len = array_Function_size(symbols->funcs);
  assert(len == symbols->funcCount);
  for (size_t i = 0; i < len; i++) {
    Function s = array_Function_getat(symbols->funcs, i);
    print_Function(s);
  }

  // printing structure info
  printf("\n  Structures: \n");

  len = array_Structure_size(symbols->structs);
  assert(len == symbols->structsCount);
  for (size_t i = 0; i < len; i++) {
    Structure s = array_Structure_getat(symbols->structs, i);
    print_Structure(s);
  }

  // printing Union info
  printf("\n  Union: \n");

  len = array_Union_size(symbols->unions);
  assert(len == symbols->unionsCount);
  for (size_t i = 0; i < len; i++) {
    Union u = array_Union_getat(symbols->unions, i);
    print_Union(u);
  }

  // printing globals info
  printf("\n  Globals: \n");

  len = array_Global_size(symbols->globals);
  assert(len == symbols->globalsCount);
  for (size_t i = 0; i < len; i++) {
    Global s = array_Global_getat(symbols->globals, i);
    print_Global(s);
  }

  // printing classes info
  // TODO: implement printing classes info
}

Function *Symbols_getFunction(Symbols *sym, const char *name) {
  for (size_t i = 0; i < sym->funcCount; i++) {
    Function *func = array_Function_get_ptr_at(sym->funcs, i);

    if (!strcmp(func->name, name)) {
      return func;
    }
  }

  return NULL;
}

Structure *Symbols_getStructure(Symbols *sym, const char *name) {
  for (size_t i = 0; i < sym->structsCount; i++) {
    Structure *func = array_Structure_get_ptr_at(sym->structs, i);

    if (!strcmp(func->name, name)) {
      return func;
    }
  }

  return NULL;
}

Union *Symbols_getUnion(Symbols *sym, const char *name) {
  for (size_t i = 0; i < sym->unionsCount; i++) {
    Union *func = array_Union_get_ptr_at(sym->unions, i);

    if (!strcmp(func->name, name)) {
      return func;
    }
  }

  return NULL;
}

Global *Symbols_getGlobal(Symbols *sym, const char *name) {
  for (size_t i = 0; i < sym->globalsCount; i++) {
    Global *func = array_Global_get_ptr_at(sym->globals, i);

    if (!strcmp(func->name, name)) {
      return func;
    }
  }

  return NULL;
}

Class *Symbols_getClass(Symbols *sym, const char *name) {
  // TODO: implement Symbols_getClass
  return NULL;
}

TypeDef *Symbols_getTypeDef(Symbols *sym, const char *name) {
  for (size_t i = 0; i < sym->typedefsCount; i++) {
    TypeDef *func = array_TypeDef_get_ptr_at(sym->typedefs, i);

    if (!strcmp(func->name, name)) {
      return func;
    }
  }

  return NULL;
}

bool Symbols_appendFunction(Symbols *sym, const char *name,
                            const char *mangledName, FunctionType funcType) {

  Function *f = Symbols_getFunction(sym, name);
  if (f) {
    // appending funcType
    if (!array_FunctionType_append(f->functionTypes, funcType))
      goto raise_error;

    f->funcCount++; // incrementing new func decl in same name

    return true;
  } else {
    // creating struct Functiom
    Function q;
    q.name = name;

    // creating array_FunctionType_t and appending funcType
    q.functionTypes = array_FunctionType_new();
    if (!(q.functionTypes))
      goto raise_error;

    if (!array_FunctionType_append(q.functionTypes, funcType))
      goto raise_error;

    q.funcCount = 1; // incrementing new func decl in same name

    // inserting the struct Function to symbols
    if (!array_Function_append(sym->funcs, q))
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

bool Symbols_appendStructure(Symbols *sym, Structure s) {

  // createing ffi_type for the given struct
  s.type.size = 0;
  s.type.alignment = 0;
  s.type.type = FFI_TYPE_STRUCT;
  s.type.elements = NULL;

  if (s.attrCount) {
    s.type.elements = calloc(s.attrCount + 1, sizeof(ffi_type *));

    for (size_t i = 0; i < s.attrCount; i++) {
      s.type.elements[i] = array_p_ffi_type_getat(s.attrTypes, i);
    }
    s.type.elements[s.attrCount] = NULL;

    ffi_cif cif;
    if (ffi_prep_cif(&cif, FFI_DEFAULT_ABI, 0, &s.type, NULL) != FFI_OK) {
      abort(); // TODO: raise error
    }
  }

  Structure *old_entry;
  if ((NULL == (old_entry = Symbols_getStructure(sym, s.name)))) {

    if (!array_Structure_append(sym->structs, s))
      goto raise_error;

    sym->structsCount++;
  } else {
    if (old_entry->type.elements) {
      free(old_entry->type.elements);
    }

    for (size_t i = 0; i < sym->structsCount; i++) {
      Structure S = array_Structure_getat(sym->structs, i);

      if (!strcmp(S.name, s.name)) {
        array_Structure_setat(sym->structs, s, i);
        break;
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

bool Symbols_appendUnion(Symbols *sym, Union u) {
  // TODO: implement

  ffi_type union_type;
  union_type.size = union_type.alignment = 0;
  union_type.type = FFI_TYPE_STRUCT;

  if (u.attrCount) {
    ffi_type **union_elements = calloc(u.attrCount + 1, sizeof(ffi_type *));
    for (size_t i = 0; i < u.attrCount; i++) {
      union_elements[i] = array_p_ffi_type_getat(u.attrTypes, i);

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

    if (!array_Union_append(sym->unions, u))
      goto raise_error;

    sym->unionsCount++;
  } else {
    if (old_entry->type.elements) {
      free(old_entry->type.elements);
    }
    for (size_t i = 0; i < sym->unionsCount; i++) {
      Union U = array_Union_getat(sym->unions, i);

      if (!strcmp(U.name, u.name)) {
        array_Union_setat(sym->unions, u, i);
        break;
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
  return true;
}

bool Symbols_appendGlobal(Symbols *sym, Global g) {
  if (!array_Global_append(sym->globals, g))
    goto raise_error;

  sym->globalsCount++;

  return true;

raise_error:
  if (errno == ENOMEM)
    PyErr_NoMemory();
  else if (errno == EINVAL)
    PyErr_SetString(py_BindingError,
                    "Incompatible global name found in translation unit");
  else
    PyErr_SetString(py_BindingError, "Unknown error occured");
  return false;
}

bool Symbols_appendTypedef(Symbols *sym, const char *typedef_name,
                           const char *type_name, CXType type) {

  // TODO: rewrite
  TypeDef typeDef;
  typeDef.type_name = type_name;
  typeDef.name = typedef_name;
  typeDef.type = type.kind;

  if (type.kind == CXType_Pointer) {
    typeDef.underlying_type = clang_getPointeeType(type).kind;
  }

  if (!array_TypeDef_append(sym->typedefs, typeDef))
    goto raise_error;

  sym->typedefsCount++;

  return true;

raise_error:
  if (errno == ENOMEM)
    PyErr_NoMemory();
  else if (errno == EINVAL)
    PyErr_SetString(py_BindingError,
                    "Incompatible typedef name found in translation unit");
  else
    PyErr_SetString(py_BindingError, "Unknown error occured");
  return false;
}

bool Symbols_appendClass(Symbols *sym, Class c) {
  // TODO: implement Symbols_appendClass
  return true;
}

Symbols *create_Symbol(const char *name) {
  Symbols *sym = malloc(sizeof(Symbols));
  sym->name = name;

  sym->funcs = array_Function_new();
  sym->funcCount = 0;

  sym->structs = array_Structure_new();
  sym->structsCount = 0;

  sym->unions = array_Union_new();
  sym->unionsCount = 0;

  sym->globals = array_Global_new();
  sym->globalsCount = 0;

  sym->classes = array_Class_new();
  sym->classesCount = 0;

  sym->typedefs = array_TypeDef_new();
  sym->typedefsCount = 0;

  return sym;
}

void free_Symbols(Symbols *sym) {
  // free(sym->name);

  // TODO: free FunctionType
  array_Function_clear(sym->funcs);

  array_Structure_clear(sym->structs);

  array_Union_new(sym->unions);

  array_Global_clear(sym->globals);

  array_Class_clear(sym->classes);

  array_TypeDef_clear(sym->typedefs);

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

enum CXChildVisitResult union_visitor(CXCursor cursor, CXCursor parent,
                                      CXClientData client_data) {
  // TODO: implement

  if (clang_getCursorKind(cursor) == CXCursor_FieldDecl) {
    const char *name = clang_getCString(clang_getCursorSpelling(cursor));
    CXType type = clang_getCursorType(cursor);
    if (type.kind == CXType_Typedef) {
      type = clang_getTypedefDeclUnderlyingType(clang_getTypeDeclaration(type));
    }

    void **info = client_data;
    Union *obj = (Union *)info[0];
    Symbols *sym = (Symbols *)info[1];

    array_str_append(obj->attrNames, (char *)name);

    const char *type_name = clang_getCString(clang_getTypeSpelling(type));
    ffi_type *ffi_type_of_element = get_ffi_type(type, sym, type_name);
    if (ffi_type_of_element) {
      array_p_ffi_type_append(obj->attrTypes, ffi_type_of_element);
    } else {
      return CXChildVisit_Break;
    }

    obj->attrCount++;

    // get underlying types if pointer type
    array_CXTypeKind_append(obj->attrUnderlyingType, 0);
    array_p_Structure_append(obj->attrUnderlyingStructs, 0);
    array_p_Union_append(obj->attrUnderlyingUnions, 0);

    CXType underlyingType = clang_getPointeeType(type);
    enum CXTypeKind underlyingTypeKind = clang_getPointeeType(type).kind;

    size_t index = obj->attrCount - 1;

    if (type.kind == CXType_Pointer) {

      array_CXTypeKind_setat(obj->attrUnderlyingType, underlyingTypeKind,
                             index);

      if (underlyingTypeKind == CXType_Typedef) {
        underlyingType = clang_getTypedefDeclUnderlyingType(
            clang_getTypeDeclaration(underlyingType));

        array_CXTypeKind_setat(obj->attrUnderlyingType, underlyingType.kind,
                               index);

        const char *actual_type_name =
            clang_getCString(clang_getTypeSpelling(underlyingType));

        size_t len = strlen(actual_type_name);
        char *updated_type_name = malloc(len);
        strcpy(updated_type_name, actual_type_name + 7);
        for (int i = 0; i < strlen(updated_type_name); i++) {
          if ((updated_type_name[i] == ' ') || (updated_type_name[i] == '*')) {
            updated_type_name[i] = 0;
            break;
          }
        }

        array_p_Structure_setat(obj->attrUnderlyingStructs,
                                Symbols_getStructure(sym, updated_type_name),
                                index);

        strcpy(updated_type_name, actual_type_name + 6);
        for (int i = 0; i < strlen(updated_type_name); i++) {
          if ((updated_type_name[i] == ' ') || (updated_type_name[i] == '*')) {
            updated_type_name[i] = 0;
            break;
          }
        }

        array_p_Union_setat(obj->attrUnderlyingUnions,
                            Symbols_getUnion(sym, updated_type_name), index);

        free(updated_type_name);

      } else if (clang_Type_getNamedType(underlyingType).kind ==
                 CXType_Record) {
        size_t len = strlen(type_name);
        char *updated_type_name = malloc(len);
        strcpy(updated_type_name, type_name + 7);
        for (int i = 0; i < strlen(updated_type_name); i++) {
          if ((updated_type_name[i] == ' ') || (updated_type_name[i] == '*')) {
            updated_type_name[i] = 0;
            break;
          }
        }

        array_p_Structure_setat(obj->attrUnderlyingStructs,
                                Symbols_getStructure(sym, updated_type_name),
                                index);

        strcpy(updated_type_name, type_name + 6);
        for (int i = 0; i < strlen(updated_type_name); i++) {
          if ((updated_type_name[i] == ' ') || (updated_type_name[i] == '*')) {
            updated_type_name[i] = 0;
            break;
          }
        }

        array_p_Union_setat(obj->attrUnderlyingUnions,
                            Symbols_getUnion(sym, updated_type_name), index);

        free(updated_type_name);
      }
    } else {
      array_CXTypeKind_setat(obj->attrUnderlyingType, 0, index);
    }

    if ((type.kind == CXType_Elaborated) &&
        (clang_Type_getNamedType(type).kind == CXType_Record)) {
      array_p_Structure_setat(obj->attrUnderlyingStructs,
                              Symbols_getStructure(sym, type_name + 7), index);

      array_p_Union_setat(obj->attrUnderlyingUnions,
                          Symbols_getUnion(sym, type_name + 6), index);
    }
  }

  return CXChildVisit_Continue;
}

enum CXChildVisitResult struct_visitor(CXCursor cursor, CXCursor parent,
                                       CXClientData client_data) {
  if (clang_getCursorKind(cursor) == CXCursor_FieldDecl) {
    const char *name = clang_getCString(clang_getCursorSpelling(cursor));
    CXType type = clang_getCursorType(cursor);
    if (type.kind == CXType_Typedef) {
      type = clang_getTypedefDeclUnderlyingType(clang_getTypeDeclaration(type));
    }

    void **info = client_data;
    Structure *obj = (Structure *)info[0];
    Symbols *sym = (Symbols *)info[1];

    array_str_append(obj->attrNames, (char *)name);

    const char *type_name = clang_getCString(clang_getTypeSpelling(type));

    array_p_ffi_type_append(obj->attrTypes, get_ffi_type(type, sym, type_name));

    obj->attrCount++;

    // get underlying types if pointer type
    array_CXTypeKind_append(obj->attrUnderlyingType, 0);
    array_p_Structure_append(obj->attrUnderlyingStructs, 0);
    array_p_Union_append(obj->attrUnderlyingUnions, 0);

    CXType underlyingType = clang_getPointeeType(type);
    enum CXTypeKind underlyingTypeKind = clang_getPointeeType(type).kind;

    size_t index = obj->attrCount - 1;

    if (type.kind == CXType_Pointer) {

      array_CXTypeKind_setat(obj->attrUnderlyingType, underlyingTypeKind,
                             index);

      if (underlyingTypeKind == CXType_Typedef) {
        underlyingType = clang_getTypedefDeclUnderlyingType(
            clang_getTypeDeclaration(underlyingType));

        array_CXTypeKind_setat(obj->attrUnderlyingType, underlyingType.kind,
                               index);

        const char *actual_type_name =
            clang_getCString(clang_getTypeSpelling(underlyingType));

        size_t len = strlen(actual_type_name);
        char *updated_type_name = malloc(len);
        strcpy(updated_type_name, actual_type_name + 7);
        for (int i = 0; i < strlen(updated_type_name); i++) {
          if ((updated_type_name[i] == ' ') || (updated_type_name[i] == '*')) {
            updated_type_name[i] = 0;
            break;
          }
        }

        array_p_Structure_setat(obj->attrUnderlyingStructs,
                                Symbols_getStructure(sym, updated_type_name),
                                index);

        strcpy(updated_type_name, actual_type_name + 6);
        for (int i = 0; i < strlen(updated_type_name); i++) {
          if ((updated_type_name[i] == ' ') || (updated_type_name[i] == '*')) {
            updated_type_name[i] = 0;
            break;
          }
        }

        array_p_Union_setat(obj->attrUnderlyingUnions,
                            Symbols_getUnion(sym, updated_type_name), index);

        free(updated_type_name);

      } else if (clang_Type_getNamedType(underlyingType).kind ==
                 CXType_Record) {
        size_t len = strlen(type_name);
        char *updated_type_name = malloc(len);
        strcpy(updated_type_name, type_name + 7);
        for (int i = 0; i < strlen(updated_type_name); i++) {
          if ((updated_type_name[i] == ' ') || (updated_type_name[i] == '*')) {
            updated_type_name[i] = 0;
            break;
          }
        }

        array_p_Structure_setat(obj->attrUnderlyingStructs,
                                Symbols_getStructure(sym, updated_type_name),
                                index);

        strcpy(updated_type_name, type_name + 6);
        for (int i = 0; i < strlen(updated_type_name); i++) {
          if ((updated_type_name[i] == ' ') || (updated_type_name[i] == '*')) {
            updated_type_name[i] = 0;
            break;
          }
        }

        array_p_Union_setat(obj->attrUnderlyingUnions,
                            Symbols_getUnion(sym, updated_type_name), index);

        free(updated_type_name);
      }
    } else {
      array_CXTypeKind_setat(obj->attrUnderlyingType, 0, index);
    }

    if ((type.kind == CXType_Elaborated) &&
        (clang_Type_getNamedType(type).kind == CXType_Record)) {
      array_p_Structure_setat(obj->attrUnderlyingStructs,
                              Symbols_getStructure(sym, type_name + 7), index);

      array_p_Union_setat(obj->attrUnderlyingUnions,
                          Symbols_getUnion(sym, type_name + 6), index);
    }
  }

  return CXChildVisit_Continue;
}

enum CXChildVisitResult visitor(CXCursor cursor, CXCursor parent,
                                CXClientData client_data) {
  Symbols *symbols = client_data;

  // function
  if (clang_getCursorKind(cursor) == CXCursor_FunctionDecl) {
    // Function Declaration
    const char *funcName = clang_getCString(clang_getCursorSpelling(cursor));
    const char *mangledName = clang_getCString(GET_MANGLED_NAME(cursor));

    FunctionType funcType;

    CXType returnType = clang_getCursorResultType(cursor);
    if (returnType.kind == CXType_Typedef) {
      returnType = clang_getTypedefDeclUnderlyingType(
          clang_getTypeDeclaration(returnType));
    }

    ffi_type *return_ffi_type =
        get_ffi_type(returnType, symbols,
                     clang_getCString(clang_getTypeSpelling(returnType)));
    if (!return_ffi_type) {
      return CXChildVisit_Break;
    }

    funcType.returnType = *return_ffi_type;

    if (returnType.kind == CXType_Pointer) {
      funcType.returnsUnderlyingType = clang_getPointeeType(returnType).kind;

      if (clang_getPointeeType(returnType).kind == CXType_Typedef) {
        returnType = clang_getTypedefDeclUnderlyingType(
            clang_getTypeDeclaration(clang_getPointeeType(returnType)));

        if (returnType.kind == CXType_Elaborated) {
          funcType.returnsUnderlyingType =
              clang_Type_getNamedType(returnType).kind;

          const char *struct_name = clang_getCString(
              clang_getTypeSpelling(clang_Type_getNamedType(returnType)));

          funcType.returnUnderlyingStruct =
              Symbols_getStructure(symbols, struct_name + 7);

          funcType.returnUnderlyingUnion =
              Symbols_getUnion(symbols, struct_name + 6);
        }
      }

      if (clang_getPointeeType(returnType).kind == CXType_Elaborated) {
        funcType.returnsUnderlyingType =
            clang_Type_getNamedType(clang_getPointeeType(returnType)).kind;

        if (clang_Type_getNamedType(clang_getPointeeType(returnType)).kind ==
            CXType_Record) {
          const char *struct_name = clang_getCString(clang_getTypeSpelling(
              clang_Type_getNamedType(clang_getPointeeType(returnType))));

          funcType.returnUnderlyingStruct =
              Symbols_getStructure(symbols, struct_name + 7);

          funcType.returnUnderlyingUnion =
              Symbols_getUnion(symbols, struct_name + 6);
        }
      }
    } else {
      funcType.returnsUnderlyingType = 0;

      if (returnType.kind == CXType_Elaborated) {
        if (clang_Type_getNamedType(returnType).kind == CXType_Record) {
          const char *struct_name = clang_getCString(
              clang_getTypeSpelling(clang_Type_getNamedType(returnType)));
          funcType.returnUnderlyingStruct =
              Symbols_getStructure(symbols, struct_name + 7);

          funcType.returnUnderlyingUnion =
              Symbols_getUnion(symbols, struct_name + 6);
        }
      }
    }

    funcType.argsCount = clang_Cursor_getNumArguments(cursor);
    funcType.argsType = array_p_ffi_type_new();
    funcType.argsUnderlyingType =
        array_CXTypeKind_new(); // TODO: extract Underlying Type Info

    for (int i = 0; i < funcType.argsCount; i++) {
      CXCursor arg = clang_Cursor_getArgument(cursor, i);
      CXType arg_type = clang_getCursorType(arg);
      if (arg_type.kind == CXType_Typedef) {
        arg_type = clang_getTypedefDeclUnderlyingType(
            clang_getTypeDeclaration(arg_type));
      }

      CXType underlyingType = clang_getPointeeType(arg_type);
      enum CXTypeKind underlyingTypeKind = underlyingType.kind;

      if (arg_type.kind == CXType_Pointer) {
        if (underlyingTypeKind == CXType_Elaborated) {
          array_CXTypeKind_append(funcType.argsUnderlyingType,
                                  clang_Type_getNamedType(underlyingType).kind);

        } else if (underlyingTypeKind == CXType_Typedef) {
          CXType actual_type =
              clang_Type_getNamedType(clang_getTypedefDeclUnderlyingType(
                  clang_getTypeDeclaration(underlyingType)));

          array_CXTypeKind_append(funcType.argsUnderlyingType,
                                  actual_type.kind);
        } else {
          array_CXTypeKind_append(funcType.argsUnderlyingType,
                                  underlyingTypeKind);
        }
      } else {
        array_CXTypeKind_append(funcType.argsUnderlyingType, 0);
      }

      ffi_type *arg_ffi_type = get_ffi_type(
          arg_type, symbols, clang_getCString(clang_getTypeSpelling(arg_type)));
      if (!arg_ffi_type) {
        return CXChildVisit_Break;
      }
      array_p_ffi_type_append(funcType.argsType, arg_ffi_type);
    }

    if (!Symbols_appendFunction(symbols, funcName, mangledName, funcType)) {
      return CXChildVisit_Break;
    }
  }

  // struct
  else if (clang_getCursorKind(cursor) == CXCursor_StructDecl) {
    const char *name = clang_getCString(clang_getCursorSpelling(cursor));

    Structure obj;
    obj.name = name;
    obj.attrNames = array_str_new();
    obj.attrTypes = array_p_ffi_type_new();
    obj.attrUnderlyingType = array_CXTypeKind_new();
    obj.attrUnderlyingStructs = array_p_Structure_new();
    obj.attrUnderlyingUnions = array_p_Union_new();
    obj.offsets = array_long_long_new();
    obj.type = (ffi_type){0, 0, 0, NULL};
    obj.attrCount = 0;
    obj.structSize = clang_Type_getSizeOf(clang_getCursorType(cursor));

    void *info[] = {&obj, symbols};
    clang_visitChildren(cursor, struct_visitor, info);

    // offset
    for (int i = 0; i < obj.attrCount; i++) {
      array_long_long_append(
          obj.offsets,
          clang_Type_getOffsetOf(clang_getCursorType(cursor),
                                 array_str_getat(obj.attrNames, i)));
    }

    Symbols_appendStructure(symbols, obj);
  }

  // unions
  else if (clang_getCursorKind(cursor) == CXCursor_UnionDecl) {
    const char *name = clang_getCString(clang_getCursorSpelling(cursor));

    Union obj;
    obj.name = name;
    obj.attrNames = array_str_new();
    obj.attrTypes = array_p_ffi_type_new();
    obj.attrUnderlyingType = array_CXTypeKind_new();
    obj.attrUnderlyingStructs = array_p_Structure_new();
    obj.attrUnderlyingUnions = array_p_Union_new();
    obj.type = (ffi_type){0, 0, 0, NULL};
    obj.attrCount = 0;
    obj.unionSize = clang_Type_getSizeOf(clang_getCursorType(cursor));

    void *info[] = {&obj, symbols};
    clang_visitChildren(cursor, union_visitor, info);

    Symbols_appendUnion(symbols, obj);

  }
  // typedef
  else if (clang_getCursorKind(cursor) == CXCursor_TypedefDecl) {
    // TODO: implement parseing typedef
    CXType underlying_type = clang_getTypedefDeclUnderlyingType(cursor);
    CXType cursor_cxtype = clang_getCursorType(cursor);
    CXString name = clang_getTypedefName(cursor_cxtype);
    const char *typedef_name = clang_getCString(name);
    const char *type_name =
        clang_getCString(clang_getTypeSpelling(underlying_type));

    if (!Symbols_appendTypedef(symbols, typedef_name, type_name,
                               underlying_type)) {
      // TODO: check errors
      return CXChildVisit_Break;
    }
    // printf("%s: %s\n", type_name, typedef_name);
  }

  // global variables
  else if (clang_getCursorKind(cursor) == CXCursor_VarDecl) {
    CXType type = clang_getCursorType(cursor);

    Global global;
    global.name = clang_getCString(clang_getCursorSpelling(cursor));

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
    // const char *name = clang_getCString(clang_getCursorSpelling(cursor));
    // const char *type_name =
    //     clang_getCString(clang_getTypeSpelling(clang_getCursorType(cursor)));
    // printf("cursor: %s : %i\ntype: %s : %i\n", name,
    //        clang_getCursorKind(cursor), type_name,
    //        clang_getCursorType(cursor).kind);
    // TODO: display type info
  }

  return CXChildVisit_Continue;
}
