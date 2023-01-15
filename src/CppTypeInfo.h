#pragma once
#include "DS.h"
#include "ffi.h"
#include "clang-c/Index.h"
#include <stdbool.h>

#define POINTER 0b1000000000000000000000000000000
#define UNSIGNED 0b0100000000000000000000000000000

enum CXX_Type {
  CXX_NotDefined       = 0,
  CXX_Void             = 1,
  CXX_VoidPointer      = CXX_Void | POINTER,
  CXX_Char             = 2,
  CXX_CharPointer      = CXX_Char | POINTER,
  CXX_UChar            = CXX_Char | UNSIGNED,
  CXX_UCharPointer     = CXX_Char | POINTER | UNSIGNED,
  CXX_Bool             = 3,
  CXX_BoolPointer      = CXX_Bool | POINTER,
  CXX_Short            = 4,
  CXX_ShortPointer     = CXX_Short | POINTER,
  CXX_UShort           = CXX_Short | UNSIGNED,
  CXX_UShortPointer    = CXX_Short | POINTER | UNSIGNED,
  CXX_Int              = 5,
  CXX_IntPointer       = CXX_Int | POINTER,
  CXX_UInt             = CXX_Int | UNSIGNED,
  CXX_UIntPointer      = CXX_Int | POINTER | UNSIGNED,
  CXX_Long             = 6,
  CXX_LongPointer      = CXX_Long | POINTER,
  CXX_ULong            = CXX_Long | UNSIGNED,
  CXX_ULongPointer     = CXX_Long | POINTER | UNSIGNED,
  CXX_LongLong         = 7,
  CXX_LongLongPointer  = CXX_LongLong | POINTER,
  CXX_ULongLong        = CXX_LongLong | UNSIGNED,
  CXX_ULongLongPointer = CXX_LongLong | POINTER | UNSIGNED,
  CXX_Float            = 8,
  CXX_FloatPointer     = CXX_Float | POINTER,
  CXX_Double           = 9,
  CXX_DoublePointer    = CXX_Double | POINTER,
  CXX_Struct           = 10,
  CXX_StructPointer    = CXX_Struct | POINTER,
  CXX_Union            = 11,
  CXX_UnionPointer     = CXX_Union | POINTER,
};

typedef struct _Global Global;
typedef struct _Structure Structure;
typedef struct _Union Union;
typedef struct _FunctionType FunctionType;
typedef struct _Function Function;
typedef struct _TypeDef TypeDef;
typedef struct _Enum Enum;
typedef struct _Symbols Symbols;

DS_ARRAY_DEC(str, char *);
DS_ARRAY_DEC(CXX_Type, enum CXX_Type);
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

struct _Global {
  char *name;
  ffi_type *typeFFI;
  enum CXX_Type typeCXX;
  void *extraInfo; // to store pointee struct or union, length of array
};

struct _RecordType {
  char *name;
  str_array_t *attrNames;          // list of attribute names
  CXX_Type_array_t *attrTypeCXX;   // array of CXTypeKind
  p_ffi_type_array_t *attrTypeFFI; // vector of attribute's ffi_type
  p_void_array_t
      *attrTypeInfo; // to store pointee struct & union, length of array
  size_t attrCount;
};

struct _Structure {
  char *name;
  str_array_t *attrNames;          // list of attribute names
  CXX_Type_array_t *attrTypeCXX;   // array of CXTypeKind
  p_ffi_type_array_t *attrTypeFFI; // vector of attribute's ffi_type
  p_void_array_t
      *attrTypeInfo; // to store pointee struct & union, length of array
  size_t attrCount;
  long_long_array_t *offsets; // record the offsets of attributes
  ffi_type type;
  size_t structSize;
};

struct _Union {
  char *name;
  str_array_t *attrNames;          // list of attribute names
  CXX_Type_array_t *attrTypeCXX;   // array of CXTypeKind
  p_ffi_type_array_t *attrTypeFFI; // vector of attribute's ffi_type
  p_void_array_t
      *attrTypeInfo; // to store pointee struct & union, length of array
  size_t attrCount;
  ffi_type type;
  size_t unionSize;
};

struct _FunctionType {
  char *mangledName;
  ffi_type *returnTypeFFI;
  enum CXX_Type returnTypeCXX;     //  CXTypeKind
  p_ffi_type_array_t *argsTypeFFI; // vector of ffi_type
  CXX_Type_array_t *argsTypeCXX;   // array of CXTypeKind
  size_t argsCount;
  p_void_array_t
      *argsTypeInfo;    // to store pointee struct or union, length of array
  void *returnTypeInfo; // to store pointee struct or union
  void (*func)();       // pointer to the function
};

struct _Function {
  char *name;
  FunctionType_array_t *functionTypes; // vector of struct FunctionTypes
  size_t funcCount;
};

struct _Enum {
  char *name;
  long long value;
};

struct _TypeDef {
  char *name;
  char *typedef_name;
  enum CXX_Type type;
  void *extraInfo; // to store pointee struct or union, length of array
};

struct _Symbols {
  char *name;

  Function_list_t *funcs; // mapping of function names to struct Function
  size_t funcCount;

  Structure_list_t *structs; // mapping of struct names to struct Structure
  size_t structsCount;

  Union_list_t *unions; // mapping of union names to union Structure
  size_t unionsCount;

  Global_list_t *globals; // mapping of global names to struct Global
  size_t globalsCount;

  TypeDef_list_t *typedefs; // mapping of typedef names to struct TypedDef
  size_t typedefsCount;

  Enum_list_t *enums; // mapping of enum names to struct Enum
  size_t enumsCount;
};

ffi_type Build_Structure_ffi_type(const p_ffi_type_array_t *ffi_type_list);
ffi_type Build_Union_ffi_type(const p_ffi_type_array_t *ffi_type_list);

void print_Global(const Global *global);
void print_Structure(const Structure *structure);
void print_Union(const Union *_union);
void print_Function(const Function *func);
void print_Enum(const Enum *e);
void print_TypeDef(const TypeDef *td);
void print_Symbols(const Symbols *symbols);

Global *Symbols_getGlobal(const Symbols *sym, const char *name);
Structure *Symbols_getStructure(const Symbols *sym, const char *name);
Union *Symbols_getUnion(const Symbols *sym, const char *name);
Function *Symbols_getFunction(const Symbols *sym, const char *name);
Enum *Symbols_getEnum(const Symbols *sym, const char *name);
TypeDef *Symbols_getTypeDef(const Symbols *sym, const char *name);

bool Symbols_appendGlobal(Symbols *sym, Global g);
bool Symbols_appendStructure(Symbols *sym, Structure s);
bool Symbols_appendUnion(Symbols *sym, Union u);
bool Symbols_appendFunction(Symbols *sym, char *name, FunctionType funcType);
bool Symbols_appendEnum(Symbols *sym, char *name, long value);
bool Symbols_appendTypedef(Symbols *sym,
                           char *typedefName,
                           char *typeName,
                           enum CXX_Type type,
                           void *extra_info);

Symbols *create_Symbol(char *name);
void free_Symbols(Symbols *sym);
bool Symbols_parse(Symbols *sym, const char *header);

bool Symbols_clearGlobal(Global *global);
bool Symbols_clearStructure(Structure *structure);
bool Symbols_clearUnion(Union *_union);
bool Symbols_clearFunctionType(FunctionType *funcType);
bool Symbols_clearFunction(Function *func);
bool Symbols_clearEnum(Enum *e);
bool Symbols_clearTypedef(TypeDef *td);
