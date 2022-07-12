#define PY_SSIZE_T_CLEAN
#include "CppTypeInfo.h"
#include "PyC.h"
#include "ffi.h"
#include "qlibc.h"
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

  assert(qlist_size(func.mangledNames) == func.funcCount);
  assert(qlist_size(func.mangledNames) == qvector_size(func.functionTypes));

  for (size_t i = 0; i < func.funcCount; i++) {
    const char *mangledName = qlist_getat(func.mangledNames, i, NULL, false);
    FunctionType type =
        *((FunctionType *)qvector_getat(func.functionTypes, i, false));

    printf("      %s %s(", ffi_type_To_char_p(type.returnType), mangledName);

    assert(type.argsCount == qvector_size(type.argsType));
    // assert(type.argsCount == qvector_size(type.argsUnderlyingType));

    for (size_t j = 0; j < type.argsCount; j++) {
      ffi_type *ffi_type_p = qvector_getat(type.argsType, j, false);
      if (type.argsUnderlyingType[j]) {
        printf("%s, ", CXTypeKind_TO_char_p(type.argsUnderlyingType[j]));
      } else {
        printf("%s, ", ffi_type_To_char_p(*ffi_type_p));
      }
    }
    printf(")\n");
  }
}

void print_Structure(Structure structure) {
  qlist_t *attrNames = structure.attrNames;
  qvector_t *attrTypes = structure.attrTypes;
  size_t attrCount = structure.attrCount;

  assert(qlist_size(attrNames) == attrCount);
  assert(qvector_size(attrTypes) == attrCount);

  printf("    struct %s\n", structure.name);

  for (size_t i = 0; i < attrCount; i++) {
    const char *attrName = qlist_getat(attrNames, i, NULL, false);
    ffi_type *attrType = qvector_getat(attrTypes, i, false);

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

  len = qlist_size(symbols->funcsNames);
  assert(len == symbols->funcCount);
  for (size_t i = 0; i < len; i++) {
    const char *name = qlist_getat(symbols->funcsNames, i, NULL, false);
    Function s = *((Function *)qhashtbl_get(symbols->funcs, name, NULL, false));
    print_Function(s);
  }

  // printing structure info
  printf("\n  Structures: \n");

  len = qlist_size(symbols->structsNames);
  assert(len == symbols->structsCount);
  for (size_t i = 0; i < len; i++) {
    const char *name = qlist_getat(symbols->structsNames, i, NULL, false);
    Structure s =
        *((Structure *)qhashtbl_get(symbols->structs, name, NULL, false));
    print_Structure(s);
  }

  // printing globals info
  printf("\n  Globals: \n");

  len = qlist_size(symbols->globalsNames);
  assert(len == symbols->globalsCount);
  for (size_t i = 0; i < len; i++) {
    const char *name = qlist_getat(symbols->globalsNames, i, NULL, false);
    Global s = *((Global *)qhashtbl_get(symbols->globals, name, NULL, false));
    print_Global(s);
  }

  // printing classes info
  // TODO: implement printing classes info
}

Function *Symbols_getFunction(Symbols *sym, const char *name) {
  Function *func = qhashtbl_get(sym->funcs, name, NULL, false);
  if (func) {
    return func;
  } else if (errno == ENOENT) {
    errno = 0;
    return NULL;
  }

  if (errno == EINVAL)
    PyErr_SetString(py_BindingError, "Incompatible C/C++ declaration name");

  if (errno == ENOMEM)
    PyErr_NoMemory();

  return NULL;
}

Structure *Symbols_getStructure(Symbols *sym, const char *name) {
  Structure *structure = qhashtbl_get(sym->structs, name, NULL, false);
  if (structure) {
    return structure;
  } else if (errno == ENOENT) {
    errno = 0;
    return NULL;
  }

  if (errno == EINVAL)
    PyErr_SetString(py_BindingError, "Incompatible C/C++ declaration name");

  if (errno == ENOMEM)
    PyErr_NoMemory();

  return NULL;
}

Global *Symbols_getGlobal(Symbols *sym, const char *name) {
  Global *global = qhashtbl_get(sym->globals, name, NULL, false);
  if (global) {
    return global;
  } else if (errno == ENOENT) {
    errno = 0;
    return NULL;
  }

  if (errno == EINVAL)
    PyErr_SetString(py_BindingError, "Incompatible C/C++ declaration name");

  if (errno == ENOMEM)
    PyErr_NoMemory();

  return NULL;
}

Class *Symbols_getClass(Symbols *sym, const char *name) {
  // TODO: implement Symbols_getClass
  return NULL;
}

bool Symbols_appendFunction(Symbols *sym, const char *name,
                            const char *mangledName, FunctionType funcType) {

  Function *f = qhashtbl_get(sym->funcs, name, NULL, false);
  if (f) {
    // appending mangledName
    if (!qlist_addlast(f->mangledNames, mangledName, strlen(mangledName) + 1))
      goto raise_error;

    // appending funcType
    if (!qvector_addlast(f->functionTypes, &funcType))
      goto raise_error;

    f->funcCount++; // incrementing new func decl in same name

    return true;
  } else {
    // appending function name to symbols
    if (!qlist_addlast(sym->funcsNames, name, strlen(name) + 1))
      goto raise_error;

    // creating struct Functiom
    Function q;
    q.name = name;

    // creating qlist_t and appending the mangledName
    q.mangledNames = qlist(0);
    if (!(q.mangledNames))
      goto raise_error;

    if (!qlist_addlast(q.mangledNames, mangledName, strlen(mangledName) + 1))
      goto raise_error;

    // creating qvector_t and appending funcType
    q.functionTypes =
        qvector(MAX_SIZE, sizeof(FunctionType), QVECTOR_RESIZE_EXACT);
    if (!(q.functionTypes))
      goto raise_error;

    if (!qvector_addlast(q.functionTypes, &funcType))
      goto raise_error;

    q.funcCount = 1; // incrementing new func decl in same name

    // inserting the struct Function to symbols
    if (!qhashtbl_put(sym->funcs, name, &q, sizeof(Function)))
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
  if (!qlist_addlast(sym->structsNames, s.name, strlen(s.name) + 1))
    goto raise_error;

  if (!qhashtbl_put(sym->structs, s.name, &s, sizeof(Structure)))
    goto raise_error;

  sym->structsCount++;

  return true;

raise_error:
  if (errno == ENOMEM)
    PyErr_NoMemory();
  else if (errno == EINVAL)
    PyErr_SetString(py_BindingError,
                    "Incompatible struct name found in translation unit");
  else
    PyErr_SetString(py_BindingError, "Unknown error occured");
  return false;
}

bool Symbols_appendGlobal(Symbols *sym, Global g) {
  if (!qlist_addlast(sym->globalsNames, g.name, strlen(g.name) + 1))
    goto raise_error;

  if (!qhashtbl_put(sym->globals, g.name, &g, sizeof(Global)))
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

bool Symbols_appendClass(Symbols *sym, Class c) {
  // TODO: implement Symbols_appendClass
  return true;
}

Symbols *create_Symbol(const char *name) {
  Symbols *sym = malloc(sizeof(Symbols));
  sym->name = name;

  sym->funcsNames = qlist(0);
  sym->funcs = qhashtbl(0, 0);
  sym->funcCount = 0;

  sym->structsNames = qlist(0);
  sym->structs = qhashtbl(0, 0);
  sym->structsCount = 0;

  sym->globalsNames = qlist(0);
  sym->globals = qhashtbl(0, 0);
  sym->globalsCount = 0;

  sym->classesNames = qlist(0);
  sym->classes = qhashtbl(0, 0);
  sym->classesCount = 0;

  return sym;
}

void free_Symbols(Symbols *sym) {
  // free(sym->name);

  qlist_free(sym->funcsNames);
  // TODO: free FunctionType
  qhashtbl_free(sym->funcs);

  qlist_free(sym->structsNames);
  qhashtbl_free(sym->structs);

  qlist_free(sym->globalsNames);
  qhashtbl_free(sym->globals);

  qlist_free(sym->classesNames);
  qhashtbl_free(sym->classes);

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

enum CXChildVisitResult struct_visitor(CXCursor cursor, CXCursor parent,
                                       CXClientData client_data) {
  if (clang_getCursorKind(cursor) == CXCursor_FieldDecl) {
    const char *name = clang_getCString(clang_getCursorSpelling(cursor));
    CXType type = clang_getCursorType(cursor);

    Structure *obj = (Structure *)client_data;

    qlist_addlast(obj->attrNames, name, strlen(name) + 1);
    qvector_addlast(obj->attrTypes, get_ffi_type(type));

    obj->attrCount++;

    // get underlying types if pointer type
    if (obj->attrCount == 1) {
      obj->attrUnderlyingType = malloc(sizeof(enum CXTypeKind));
    } else {
      obj->attrUnderlyingType = realloc(
          obj->attrUnderlyingType, sizeof(enum CXTypeKind) * obj->attrCount);
    }

    if (type.kind == CXType_Pointer) {
      obj->attrUnderlyingType[obj->attrCount - 1] =
          clang_getPointeeType(type).kind;
    } else {
      obj->attrUnderlyingType[obj->attrCount - 1] = 0;
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
    ffi_type *return_ffi_type = get_ffi_type(returnType);
    if (!return_ffi_type) {
      return CXChildVisit_Break;
    }

    funcType.returnType = *return_ffi_type;

    if (returnType.kind == CXType_Pointer) {
      funcType.returnsUnderlyingType = clang_getPointeeType(returnType).kind;
    } else {
      funcType.returnsUnderlyingType = 0;
    }

    funcType.argsCount = clang_Cursor_getNumArguments(cursor);
    funcType.argsType =
        qvector(MAX_SIZE, sizeof(ffi_type),
                QVECTOR_RESIZE_EXACT); // TODO: change ffi_type to ffi_type*
    funcType.argsUnderlyingType =
        calloc(funcType.argsCount,
               sizeof(enum CXTypeKind)); // TODO: extract Underlying Type Info

    for (int i = 0; i < funcType.argsCount; i++) {
      CXCursor arg = clang_Cursor_getArgument(cursor, i);
      CXType arg_type = clang_getCursorType(arg);

      if (arg_type.kind == CXType_Pointer)
        funcType.argsUnderlyingType[i] = clang_getPointeeType(arg_type).kind;
      else
        funcType.argsUnderlyingType[i] = 0;

      ffi_type *arg_ffi_type = get_ffi_type(arg_type);
      if (!arg_ffi_type) {
        return CXChildVisit_Break;
      }
      qvector_addlast(funcType.argsType, arg_ffi_type);
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
    obj.attrNames = qlist(0);
    obj.attrTypes =
        qvector(MAX_SIZE, sizeof(ffi_type),
                QVECTOR_RESIZE_EXACT); // TODO: change ffi_type to ffi_type*
    obj.attrUnderlyingType = NULL;
    obj.offsets = NULL;
    obj.type = (ffi_type){0, 0, 0, NULL};
    obj.attrCount = 0;
    obj.structSize = clang_Type_getSizeOf(clang_getCursorType(cursor));

    clang_visitChildren(cursor, struct_visitor, &obj);

    // offset
    obj.offsets = malloc(sizeof(long long) * obj.attrCount);
    for (int i; i < obj.attrCount; i++) {
      obj.offsets[i] =
          clang_Type_getOffsetOf(clang_getCursorType(cursor),
                                 qlist_getat(obj.attrNames, i, NULL, false));
    }

    Symbols_appendStructure(symbols, obj);
  }

  // typedef
  else if (clang_getCursorKind(cursor) == CXCursor_TypedefDecl) {
    // TODO: implement parseing typedef
    CXType underlying_type = clang_getTypedefDeclUnderlyingType(cursor);
    CXString name = clang_getCursorSpelling(cursor);
  }

  // global variables
  else if (clang_getCursorKind(cursor) == CXCursor_VarDecl) {
    CXType type = clang_getCursorType(cursor);

    Global global;
    global.name = clang_getCString(clang_getCursorSpelling(cursor));

    ffi_type *type_ffi_type = get_ffi_type(type);
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
  }

  return CXChildVisit_Continue;
}
