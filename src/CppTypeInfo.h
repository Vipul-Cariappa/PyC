#pragma once
#include "DS.h"
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
typedef struct _Enum Enum;
typedef struct _Symbols Symbols;

DS_ARRAY_DEC(str, char *);
DS_ARRAY_DEC(CXTypeKind, enum CXTypeKind);
DS_ARRAY_DEC(p_ffi_type, ffi_type *);
DS_ARRAY_DEC(p_Structure, Structure *);
DS_ARRAY_DEC(p_Union, Union *);
DS_ARRAY_DEC(long_long, long long);
DS_ARRAY_DEC(p_void, void *);

DS_ARRAY_DEC(FunctionType, FunctionType);

DS_LIST_DEC(Function, Function);
DS_LIST_DEC(Global, Global);
DS_LIST_DEC(Structure, Structure);
DS_LIST_DEC(Union, Union);
DS_LIST_DEC(TypeDef, TypeDef);
DS_LIST_DEC(Enum, Enum);
DS_LIST_DEC(Class, Class);

struct _Global {
  const char *name;
  // const char *mangledName;
  ffi_type type;
  enum CXTypeKind underlyingType;
  Structure *underlyingStruct; // for type convertion
  Union *underlyingUnion;      // for type convertion
};

struct _Structure {
  const char *name;
  str_array_t *attrNames;                     // list of attribute names
  p_ffi_type_array_t *attrTypes;              // vector of attribute's ffi_type
  CXTypeKind_array_t *attrUnderlyingType;     // array of CXTypeKind
  p_Structure_array_t *attrUnderlyingStructs; // array of underlying
                                              // struct type for pointers
  p_Union_array_t *attrUnderlyingUnions;      // array of underlying
                                              // unions type for pointers
  long_long_array_t *offsets; // record the offsets of attributes
  ffi_type type;
  size_t attrCount;
  size_t structSize;
};

struct _Union {
  const char *name;
  str_array_t *attrNames;                     // list of attribute names
  p_ffi_type_array_t *attrTypes;              // vector of attribute's ffi_type
  CXTypeKind_array_t *attrUnderlyingType;     // array of CXTypeKind
  p_Structure_array_t *attrUnderlyingStructs; // array of underlying
                                              // struct type for pointers
  p_Union_array_t *attrUnderlyingUnions;      // array of underlying
                                              // unions type for pointers
  ffi_type type;
  size_t attrCount;
  size_t unionSize;
};

struct _Class {
  // TODO: Implement
  int f;
};

struct _FunctionType {
  const char *mangledName;
  ffi_type returnType;
  p_ffi_type_array_t *argsType;           // vector of ffi_type
  CXTypeKind_array_t *argsUnderlyingType; // array of CXTypeKind
  enum CXTypeKind returnsUnderlyingType;  //  CXTypeKind
  p_Structure_array_t
      *argsUnderlyingStructs;            // checking for proper struct matching
  p_Union_array_t *argsUnderlyingUnions; // checking for proper union matching
  Structure *returnUnderlyingStruct;     // for type convertion
  Union *returnUnderlyingUnion;          // for type convertion
  size_t argsCount;
  void (*func)(); // pointer to the function
};

struct _Function {
  const char *name;
  FunctionType_array_t *functionTypes; // vector of struct FunctionTypes
  size_t funcCount;
};

struct _Enum {
  const char *name;
  long long value;
};

struct _TypeDef {
  const char *name;
  const char *type_name;
  enum CXTypeKind type;
  enum CXTypeKind underlying_type; // Used if underlying type is pointer
};

struct _Symbols {
  const char *name;

  Function_list_t *funcs; // mapping of function names to struct Function
  size_t funcCount;

  Structure_list_t *structs; // mapping of struct names to struct Structure
  size_t structsCount;

  Union_list_t *unions; // mapping of union names to union Structure
  size_t unionsCount;

  Class_list_t *classes; // mapping of class names to struct Class
  size_t classesCount;

  Global_list_t *globals; // mapping of global names to struct Global
  size_t globalsCount;

  TypeDef_list_t *typedefs; // mapping of typedef names to struct TypedDef
  size_t typedefsCount;

  Enum_list_t *enums; // mapping of enum names to struct Enum
  size_t enumsCount;
};

void print_Function(Function func);
void print_Structure(Structure structure);
void print_Union(Union _union);
void print_Global(Global global);
void print_TypeDef(TypeDef td);
void print_Enum(Enum e);
void print_Class(Class cls);
void print_Symbols(Symbols *symbols);

Function *Symbols_getFunction(Symbols *sym, const char *name);
Structure *Symbols_getStructure(Symbols *sym, const char *name);
Union *Symbols_getUnion(Symbols *sym, const char *name);
Global *Symbols_getGlobal(Symbols *sym, const char *name);
Class *Symbols_getClass(Symbols *sym, const char *name);
TypeDef *Symbols_getTypeDef(Symbols *sym, const char *name);
Enum *Symbols_getEnum(Symbols *sym, const char *name);

bool Symbols_appendFunction(Symbols *sym, const char *name,
                            const char *mangledName, FunctionType funcType);
bool Symbols_appendStructure(Symbols *sym, Structure s);
bool Symbols_appendUnion(Symbols *sym, Union u);
bool Symbols_appendGlobal(Symbols *sym, Global g);
bool Symbols_appendClass(Symbols *sym, Class c);
bool Symbols_appendTypedef(Symbols *sym, const char *typedef_name,
                           const char *type_name, CXType type);
bool Symbols_appendEnum(Symbols *sym, const char *name, long value);

Symbols *create_Symbol(const char *name);
void free_Symbols(Symbols *sym);
bool Symbols_parse(Symbols *sym, const char *header);

bool Symbols_clearEnum(Enum s);
bool Symbols_clearTypedef(TypeDef s);
bool Symbols_clearGlobal(Global s);
bool Symbols_clearUnion(Union s);
bool Symbols_clearStructure(Structure s);
bool Symbols_clearFunctionType(FunctionType s);
bool Symbols_clearFunction(Function s);
