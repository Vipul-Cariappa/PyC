#pragma once
#include "DataStructures.h"
#include "ffi.h"
#include "clang-c/Index.h"
#include <stdbool.h>

typedef struct _Global Global;
typedef struct _Structure Structure;
typedef struct _Union Union;
typedef struct _Class Class;
typedef struct _FunctionType FunctionType;
typedef struct _Function Function;
typedef struct _TypeDef TypeDef;
typedef struct _Symbols Symbols;

typedef struct _array_long_long_t array_long_long_t;
typedef struct _array_size_t_t array_size_t_t;
typedef struct _array_str_t array_str_t;
typedef struct _array_CXTypeKind_t array_CXTypeKind_t;
typedef struct _array_p_ffi_type_t array_p_ffi_type_t;
typedef struct _array_Structure_t array_Structure_t;
typedef struct _array_Union_t array_Union_t;
typedef struct _array_p_Structure_t array_p_Structure_t;
typedef struct _array_p_Union_t array_p_Union_t;
typedef struct _array_Global_t array_Global_t;
typedef struct _array_TypeDef_t array_TypeDef_t;
typedef struct _array_FunctionType_t array_FunctionType_t;
typedef struct _array_Function_t array_Function_t;
typedef struct _array_Class_t array_Class_t;

struct _Global {
  const char *name;
  // const char *mangledName;
  ffi_type type;
  enum CXTypeKind underlyingType;
};

struct _Structure {
  const char *name;
  array_str_t *attrNames;                   // list of attribute names
  array_p_ffi_type_t *attrTypes;            // vector of attribute's ffi_type
  array_CXTypeKind_t *attrUnderlyingType;   // array of CXTypeKind
  array_Structure_t *attrUnderlyingStructs; // array of underlying
                                            // struct type for pointers
  array_Union_t *attrUnderlyingUnions;      // array of underlying
                                            // unions type for pointers
  array_long_long_t *offsets;               // record the offsets of attributes
  ffi_type type;
  size_t attrCount;
  size_t structSize;
};

struct _Union {
  const char *name;
  array_str_t *attrNames;                   // list of attribute names
  array_p_ffi_type_t *attrTypes;            // vector of attribute's ffi_type
  array_CXTypeKind_t *attrUnderlyingType;   // array of CXTypeKind
  array_Structure_t *attrUnderlyingStructs; // array of underlying
                                            // struct type for pointers
  array_Union_t *attrUnderlyingUnions;      // array of underlying
                                            // unions type for pointers
  ffi_type type;
  size_t attrCount;
  size_t unionSize;
};

struct _Class {
  // TODO: Implement
};

struct _FunctionType {
  char *mangledName;
  ffi_type returnType;
  array_p_ffi_type_t *argsType;           // vector of ffi_type
  array_CXTypeKind_t *argsUnderlyingType; // array of CXTypeKind
  enum CXTypeKind returnsUnderlyingType;  //  CXTypeKind
  array_Structure_t
      *argsUnderlyingStructs;          // checking for proper struct matching
  array_Union_t *argsUnderlyingUnions; // checking for proper union matching
  Structure *returnUnderlyingStruct;   // for type convertion
  Union *returnUnderlyingUnion;        // for type convertion
  size_t argsCount;
};

struct _Function {
  const char *name;
  array_FunctionType_t *functionTypes; // vector of struct FunctionTypes
  size_t funcCount;
};

struct _TypeDef {
  const char *name;
  const char *type_name;
  enum CXTypeKind type;
  enum CXTypeKind underlying_type; // Used if underlying type is pointer
};

// typedef struct _funcTuple {
//   char* name;
//   Function func;
// } funcTuple;

// LIST_DEL(list_funcTuple, funcTuple);

struct _Symbols {
  const char *name;

  array_Function_t *funcs; // mapping of function names to struct Function
  size_t funcCount;

  array_Structure_t *structs; // mapping of struct names to struct Structure
  size_t structsCount;

  array_Union_t *unions; // mapping of union names to union Structure
  size_t unionsCount;

  array_Class_t *classes; // mapping of class names to struct Class
  size_t classesCount;

  array_Global_t *globals; // mapping of global names to struct Global
  size_t globalsCount;

  array_TypeDef_t *typedefs; // mapping of typedef names to struct TypedDef
  size_t typedefsCount;
};

void print_Function(Function func);
void print_Structure(Structure structure);
void print_Union(Union _union);
void print_Global(Global global);
void print_Class(Class cls);
void print_Symbols(Symbols *symbols);

Function *Symbols_getFunction(Symbols *sym, const char *name);
Structure *Symbols_getStructure(Symbols *sym, const char *name);
Union *Symbols_getUnion(Symbols *sym, const char *name);
Global *Symbols_getGlobal(Symbols *sym, const char *name);
Class *Symbols_getClass(Symbols *sym, const char *name);
TypeDef *Symbols_getTypeDef(Symbols *sym, const char *name);

bool Symbols_appendFunction(Symbols *sym, const char *name,
                            const char *mangledName, FunctionType funcType);
bool Symbols_appendStructure(Symbols *sym, Structure s);
bool Symbols_appendUnion(Symbols *sym, Union u);
bool Symbols_appendGlobal(Symbols *sym, Global g);
bool Symbols_appendClass(Symbols *sym, Class c);
bool Symbols_appendTypedef(Symbols *sym, const char *typedef_name,
                           const char *type_name, CXType type);

Symbols *create_Symbol(const char *name);
void free_Symbols(Symbols *sym);
bool Symbols_parse(Symbols *sym, const char *header);
