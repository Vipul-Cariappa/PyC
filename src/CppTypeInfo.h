#pragma once
#include "ffi.h"
#include "qlibc.h"
#include "clang-c/Index.h"

typedef struct FunctionType {
  ffi_type returnType;
  qvector_t *argsType;                   // vector of ffi_type
  enum CXTypeKind *argsUnderlyingType;   // array of CXTypeKind
  enum CXTypeKind returnsUnderlyingType; //  CXTypeKind
  size_t argsCount;
} FunctionType;

typedef struct Function {
  const char *name;
  qlist_t *mangledNames;    // vector of mangled names
  qvector_t *functionTypes; // vector of struct FunctionTypes
  size_t funcCount;
} Function;

typedef struct Structure {
  const char *name;
  qlist_t *attrNames;                  // vector of attribute names
  qvector_t *attrTypes;                // vector of attribute's ffi_type
  enum CXTypeKind *attrUnderlyingType; // array of CXTypeKind
  long long *offsets;                  // record the offsets of attributes
  ffi_type type;
  size_t attrCount;
  size_t structSize;
} Structure;

typedef struct Global {
  const char *name;
  // const char *mangledName;
  ffi_type type;
  enum CXTypeKind underlyingType;
} Global;

typedef struct Class {
  // TODO: Implement
} Class;

typedef struct Symbols {
  const char *name;

  qlist_t *funcsNames; // vector of function names
  qhashtbl_t *funcs;   // mapping of function names to struct Function
  size_t funcCount;

  qlist_t *structsNames; // vector of struct names
  qhashtbl_t *structs;   // mapping of struct names to struct Structure
  size_t structsCount;

  qlist_t *classesNames; // vector of class names
  qhashtbl_t *classes;   // mapping of class names to struct Class
  size_t classesCount;

  qlist_t *globalsNames; // vector of global names
  qhashtbl_t *globals;   // mapping of global names to struct Global
  size_t globalsCount;
} Symbols;

void print_Function(Function func);
void print_Structure(Structure structure);
void print_Global(Global global);
void print_Class(Class cls);
void print_Symbols(Symbols *symbols);

Function *Symbols_getFunction(Symbols *sym, const char *name);
Structure *Symbols_getStructure(Symbols *sym, const char *name);
Global *Symbols_getGlobal(Symbols *sym, const char *name);
Class *Symbols_getClass(Symbols *sym, const char *name);

bool Symbols_appendFunction(Symbols *sym, const char *name,
                            const char *mangledName, FunctionType funcType);
bool Symbols_appendStructure(Symbols *sym, Structure s);
bool Symbols_appendGlobal(Symbols *sym, Global g);
bool Symbols_appendClass(Symbols *sym, Class c);

Symbols *create_Symbol(const char *name);
void free_Symbols(Symbols *sym);
bool Symbols_parse(Symbols *sym, const char *header);
