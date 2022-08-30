#pragma once
// #include "CppTypeInfo.h"
#include "ffi.h"
#include "clang-c/Index.h"
#include <stdbool.h>
#include <stdlib.h>

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
typedef struct _list_Structure_t list_Structure_t;
typedef struct _list_Union_t list_Union_t;
typedef struct _array_p_Structure_t array_p_Structure_t;
typedef struct _array_p_Union_t array_p_Union_t;
typedef struct _array_Global_t array_Global_t;
typedef struct _array_TypeDef_t array_TypeDef_t;
typedef struct _array_FunctionType_t array_FunctionType_t;
typedef struct _array_Function_t array_Function_t;
typedef struct _array_Class_t array_Class_t;
typedef struct _list_Global_t list_Global_t;
typedef struct _list_TypeDef_t list_TypeDef_t;
typedef struct _list_FunctionType_t list_FunctionType_t;
typedef struct _list_Function_t list_Function_t;
typedef struct _list_Class_t list_Class_t;

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
  array_str_t *attrNames;                     // list of attribute names
  array_p_ffi_type_t *attrTypes;              // vector of attribute's ffi_type
  array_CXTypeKind_t *attrUnderlyingType;     // array of CXTypeKind
  array_p_Structure_t *attrUnderlyingStructs; // array of underlying
                                              // struct type for pointers
  array_p_Union_t *attrUnderlyingUnions;      // array of underlying
                                              // unions type for pointers
  array_long_long_t *offsets; // record the offsets of attributes
  ffi_type type;
  size_t attrCount;
  size_t structSize;
};

struct _Union {
  const char *name;
  array_str_t *attrNames;                     // list of attribute names
  array_p_ffi_type_t *attrTypes;              // vector of attribute's ffi_type
  array_CXTypeKind_t *attrUnderlyingType;     // array of CXTypeKind
  array_p_Structure_t *attrUnderlyingStructs; // array of underlying
                                              // struct type for pointers
  array_p_Union_t *attrUnderlyingUnions;      // array of underlying
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
  array_p_ffi_type_t *argsType;           // vector of ffi_type
  array_CXTypeKind_t *argsUnderlyingType; // array of CXTypeKind
  enum CXTypeKind returnsUnderlyingType;  //  CXTypeKind
  array_p_Structure_t
      *argsUnderlyingStructs;            // checking for proper struct matching
  array_p_Union_t *argsUnderlyingUnions; // checking for proper union matching
  Structure *returnUnderlyingStruct;     // for type convertion
  Union *returnUnderlyingUnion;          // for type convertion
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

/* array of long long declarations */

struct _array_long_long_t {
  long long *array;
  size_t size;
  size_t capacity;
};

array_long_long_t *array_long_long_new();
size_t array_long_long_size(array_long_long_t *arr);
bool array_long_long_append(array_long_long_t *arr, long long val);
long long array_long_long_pop(array_long_long_t *arr);
bool array_long_long_setat(array_long_long_t *arr, long long val, size_t index);
long long array_long_long_getat(array_long_long_t *arr, size_t index);
bool array_long_long_clear(array_long_long_t *arr);

/* array of size_t declarations */

struct _array_size_t_t {
  size_t *array;
  size_t size;
  size_t capacity;
};

array_size_t_t *array_size_t_new();
size_t array_size_t_size(array_size_t_t *arr);
bool array_size_t_append(array_size_t_t *arr, size_t val);
size_t array_size_t_pop(array_size_t_t *arr);
bool array_size_t_setat(array_size_t_t *arr, size_t val, size_t index);
size_t array_size_t_getat(array_size_t_t *arr, size_t index);
bool array_size_t_clear(array_size_t_t *arr);

/* array of string declarations */

struct _array_str_t {
  char **array;
  size_t size;
  size_t capacity;
};

array_str_t *array_str_new();
size_t array_str_size(array_str_t *arr);
bool array_str_append(array_str_t *arr, char *val);
char *array_str_pop(array_str_t *arr);
bool array_str_setat(array_str_t *arr, char *val, size_t index);
char *array_str_getat(array_str_t *arr, size_t index);
bool array_str_clear(array_str_t *arr);

/* array of CXTypeKind declarations */

struct _array_CXTypeKind_t {
  enum CXTypeKind *array;
  size_t size;
  size_t capacity;
};

array_CXTypeKind_t *array_CXTypeKind_new();
size_t array_CXTypeKind_size(array_CXTypeKind_t *arr);
bool array_CXTypeKind_append(array_CXTypeKind_t *arr, enum CXTypeKind val);
enum CXTypeKind array_CXTypeKind_pop(array_CXTypeKind_t *arr);
bool array_CXTypeKind_setat(array_CXTypeKind_t *arr, enum CXTypeKind val,
                            size_t index);
enum CXTypeKind array_CXTypeKind_getat(array_CXTypeKind_t *arr, size_t index);
bool array_CXTypeKind_clear(array_CXTypeKind_t *arr);

/* array of p_ffi_type declarations */

struct _array_p_ffi_type_t {
  ffi_type **array;
  size_t size;
  size_t capacity;
};

array_p_ffi_type_t *array_p_ffi_type_new();
size_t array_p_ffi_type_size(array_p_ffi_type_t *arr);
bool array_p_ffi_type_append(array_p_ffi_type_t *arr, ffi_type *val);
ffi_type *array_p_ffi_type_pop(array_p_ffi_type_t *arr);
bool array_p_ffi_type_setat(array_p_ffi_type_t *arr, ffi_type *val,
                            size_t index);
ffi_type *array_p_ffi_type_getat(array_p_ffi_type_t *arr, size_t index);
bool array_p_ffi_type_clear(array_p_ffi_type_t *arr);

/* array of Structure declarations */

struct _array_Structure_t {
  Structure *array;
  size_t size;
  size_t capacity;
};

array_Structure_t *array_Structure_new();
size_t array_Structure_size(array_Structure_t *arr);
bool array_Structure_append(array_Structure_t *arr, Structure val);
Structure array_Structure_pop(array_Structure_t *arr);
bool array_Structure_setat(array_Structure_t *arr, Structure val, size_t index);
Structure array_Structure_getat(array_Structure_t *arr, size_t index);
Structure *array_Structure_get_ptr_at(array_Structure_t *arr, size_t index);
bool array_Structure_clear(array_Structure_t *arr);

/* list of Structure declarations */
typedef struct _node_list_Structure node_list_Structure;

struct _node_list_Structure {
  Structure element;
  node_list_Structure *next;
  node_list_Structure *previous;
};

struct _list_Structure_t {
  node_list_Structure *first;
  node_list_Structure *last;
  size_t size;
};

list_Structure_t *list_Structure_new();
size_t list_Structure_size(list_Structure_t *arr);
bool list_Structure_append(list_Structure_t *arr, Structure val);
Structure list_Structure_pop(list_Structure_t *arr);
bool list_Structure_setat(list_Structure_t *arr, Structure val, size_t index);
Structure list_Structure_getat(list_Structure_t *arr, size_t index);
Structure *list_Structure_get_ptr_at(list_Structure_t *arr, size_t index);
bool list_Structure_clear(list_Structure_t *arr);

/* array of pointers of Structure declarations */

struct _array_p_Structure_t {
  Structure **array;
  size_t size;
  size_t capacity;
};

array_p_Structure_t *array_p_Structure_new();
size_t array_p_Structure_size(array_p_Structure_t *arr);
bool array_p_Structure_append(array_p_Structure_t *arr, Structure *val);
Structure *array_p_Structure_pop(array_p_Structure_t *arr);
bool array_p_Structure_setat(array_p_Structure_t *arr, Structure *val,
                             size_t index);
Structure *array_p_Structure_getat(array_p_Structure_t *arr, size_t index);
bool array_p_Structure_clear(array_p_Structure_t *arr);

/* array of Union declarations */

struct _array_Union_t {
  Union *array;
  size_t size;
  size_t capacity;
};

array_Union_t *array_Union_new();
size_t array_Union_size(array_Union_t *arr);
bool array_Union_append(array_Union_t *arr, Union val);
Union array_Union_pop(array_Union_t *arr);
bool array_Union_setat(array_Union_t *arr, Union val, size_t index);
Union array_Union_getat(array_Union_t *arr, size_t index);
Union *array_Union_get_ptr_at(array_Union_t *arr, size_t index);
bool array_Union_clear(array_Union_t *arr);

/* list of Structure declarations */
typedef struct _node_list_Union node_list_Union;

struct _node_list_Union {
  Union element;
  node_list_Union *next;
  node_list_Union *previous;
};

struct _list_Union_t {
  node_list_Union *first;
  node_list_Union *last;
  size_t size;
};

list_Union_t *list_Union_new();
size_t list_Union_size(list_Union_t *arr);
bool list_Union_append(list_Union_t *arr, Union val);
Union list_Union_pop(list_Union_t *arr);
bool list_Union_setat(list_Union_t *arr, Union val, size_t index);
Union list_Union_getat(list_Union_t *arr, size_t index);
Union *list_Union_get_ptr_at(list_Union_t *arr, size_t index);
bool list_Union_clear(list_Union_t *arr);

/* array of pointers of Union declarations */

struct _array_p_Union_t {
  Union **array;
  size_t size;
  size_t capacity;
};

array_p_Union_t *array_p_Union_new();
size_t array_p_Union_size(array_p_Union_t *arr);
bool array_p_Union_append(array_p_Union_t *arr, Union *val);
Union *array_p_Union_pop(array_p_Union_t *arr);
bool array_p_Union_setat(array_p_Union_t *arr, Union *val, size_t index);
Union *array_p_Union_getat(array_p_Union_t *arr, size_t index);
bool array_p_Union_clear(array_p_Union_t *arr);

/* array of Global declarations */

struct _array_Global_t {
  Global *array;
  size_t size;
  size_t capacity;
};

array_Global_t *array_Global_new();
size_t array_Global_size(array_Global_t *arr);
bool array_Global_append(array_Global_t *arr, Global val);
Global array_Global_pop(array_Global_t *arr);
bool array_Global_setat(array_Global_t *arr, Global val, size_t index);
Global array_Global_getat(array_Global_t *arr, size_t index);
Global *array_Global_get_ptr_at(array_Global_t *arr, size_t index);
bool array_Global_clear(array_Global_t *arr);

/* list of Global declarations */
typedef struct _node_list_Global node_list_Global;

struct _node_list_Global {
  Global element;
  node_list_Global *next;
  node_list_Global *previous;
};

struct _list_Global_t {
  node_list_Global *first;
  node_list_Global *last;
  size_t size;
};

list_Global_t *list_Global_new();
size_t list_Global_size(list_Global_t *arr);
bool list_Global_append(list_Global_t *arr, Global val);
Global list_Global_pop(list_Global_t *arr);
bool list_Global_setat(list_Global_t *arr, Global val, size_t index);
Global list_Global_getat(list_Global_t *arr, size_t index);
Global *list_Global_get_ptr_at(list_Global_t *arr, size_t index);
bool list_Global_clear(list_Global_t *arr);

/* array of TypeDef declarations */

struct _array_TypeDef_t {
  TypeDef *array;
  size_t size;
  size_t capacity;
};

array_TypeDef_t *array_TypeDef_new();
size_t array_TypeDef_size(array_TypeDef_t *arr);
bool array_TypeDef_append(array_TypeDef_t *arr, TypeDef val);
TypeDef array_TypeDef_pop(array_TypeDef_t *arr);
bool array_TypeDef_setat(array_TypeDef_t *arr, TypeDef val, size_t index);
TypeDef array_TypeDef_getat(array_TypeDef_t *arr, size_t index);
TypeDef *array_TypeDef_get_ptr_at(array_TypeDef_t *arr, size_t index);
bool array_TypeDef_clear(array_TypeDef_t *arr);

/* list of TypeDef declarations */
typedef struct _node_list_TypeDef node_list_TypeDef;

struct _node_list_TypeDef {
  TypeDef element;
  node_list_TypeDef *next;
  node_list_TypeDef *previous;
};

struct _list_TypeDef_t {
  node_list_TypeDef *first;
  node_list_TypeDef *last;
  size_t size;
};

list_TypeDef_t *list_TypeDef_new();
size_t list_TypeDef_size(list_TypeDef_t *arr);
bool list_TypeDef_append(list_TypeDef_t *arr, TypeDef val);
TypeDef list_TypeDef_pop(list_TypeDef_t *arr);
bool list_TypeDef_setat(list_TypeDef_t *arr, TypeDef val, size_t index);
TypeDef list_TypeDef_getat(list_TypeDef_t *arr, size_t index);
TypeDef *list_TypeDef_get_ptr_at(list_TypeDef_t *arr, size_t index);
bool list_TypeDef_clear(list_TypeDef_t *arr);

/* array of FunctionType declarations */

struct _array_FunctionType_t {
  FunctionType *array;
  size_t size;
  size_t capacity;
};

array_FunctionType_t *array_FunctionType_new();
size_t array_FunctionType_size(array_FunctionType_t *arr);
bool array_FunctionType_append(array_FunctionType_t *arr, FunctionType val);
FunctionType array_FunctionType_pop(array_FunctionType_t *arr);
bool array_FunctionType_setat(array_FunctionType_t *arr, FunctionType val,
                              size_t index);
FunctionType array_FunctionType_getat(array_FunctionType_t *arr, size_t index);
FunctionType *array_FunctionType_get_ptr_at(array_FunctionType_t *arr,
                                            size_t index);
bool array_FunctionType_clear(array_FunctionType_t *arr);

/* list of FunctionType declarations */
typedef struct _node_list_FunctionType node_list_FunctionType;

struct _node_list_FunctionType {
  FunctionType element;
  node_list_FunctionType *next;
  node_list_FunctionType *previous;
};

struct _list_FunctionType_t {
  node_list_FunctionType *first;
  node_list_FunctionType *last;
  size_t size;
};

list_FunctionType_t *list_FunctionType_new();
size_t list_FunctionType_size(list_FunctionType_t *arr);
bool list_FunctionType_append(list_FunctionType_t *arr, FunctionType val);
FunctionType list_FunctionType_pop(list_FunctionType_t *arr);
bool list_FunctionType_setat(list_FunctionType_t *arr, FunctionType val,
                             size_t index);
FunctionType list_FunctionType_getat(list_FunctionType_t *arr, size_t index);
FunctionType *list_FunctionType_get_ptr_at(list_FunctionType_t *arr,
                                           size_t index);
bool list_FunctionType_clear(list_FunctionType_t *arr);

/* array of Function declarations */

struct _array_Function_t {
  Function *array;
  size_t size;
  size_t capacity;
};

array_Function_t *array_Function_new();
size_t array_Function_size(array_Function_t *arr);
bool array_Function_append(array_Function_t *arr, Function val);
Function array_Function_pop(array_Function_t *arr);
bool array_Function_setat(array_Function_t *arr, Function val, size_t index);
Function array_Function_getat(array_Function_t *arr, size_t index);
Function *array_Function_get_ptr_at(array_Function_t *arr, size_t index);
bool array_Function_clear(array_Function_t *arr);

/* list of Function declarations */
typedef struct _node_list_Function node_list_Function;

struct _node_list_Function {
  Function element;
  node_list_Function *next;
  node_list_Function *previous;
};

struct _list_Function_t {
  node_list_Function *first;
  node_list_Function *last;
  size_t size;
};

list_Function_t *list_Function_new();
size_t list_Function_size(list_Function_t *arr);
bool list_Function_append(list_Function_t *arr, Function val);
Function list_Function_pop(list_Function_t *arr);
bool list_Function_setat(list_Function_t *arr, Function val, size_t index);
Function list_Function_getat(list_Function_t *arr, size_t index);
Function *list_Function_get_ptr_at(list_Function_t *arr, size_t index);
bool list_Function_clear(list_Function_t *arr);

/* array of Class declarations */

struct _array_Class_t {
  Class *array;
  size_t size;
  size_t capacity;
};

array_Class_t *array_Class_new();
size_t array_Class_size(array_Class_t *arr);
bool array_Class_append(array_Class_t *arr, Class val);
Class array_Class_pop(array_Class_t *arr);
bool array_Class_setat(array_Class_t *arr, Class val, size_t index);
Class array_Class_getat(array_Class_t *arr, size_t index);
Class *array_Class_get_ptr_at(array_Class_t *arr, size_t index);
bool array_Class_clear(array_Class_t *arr);

/* list of Class declarations */
typedef struct _node_list_Class node_list_Class;

struct _node_list_Class {
  Class element;
  node_list_Class *next;
  node_list_Class *previous;
};

struct _list_Class_t {
  node_list_Class *first;
  node_list_Class *last;
  size_t size;
};

list_Class_t *list_Class_new();
size_t list_Class_size(list_Class_t *arr);
bool list_Class_append(list_Class_t *arr, Class val);
Class list_Class_pop(list_Class_t *arr);
bool list_Class_setat(list_Class_t *arr, Class val, size_t index);
Class list_Class_getat(list_Class_t *arr, size_t index);
Class *list_Class_get_ptr_at(list_Class_t *arr, size_t index);
bool list_Class_clear(list_Class_t *arr);

// FIXME: dont know where to put this functions
bool Symbols_clearUnion(Union *s);
bool Symbols_clearStructure(Structure *s);
bool Symbols_clearFunction(Function *s);
bool Symbols_clearFunctionType(FunctionType *s);
bool Symbols_clearGlobal(Global *s);
bool Symbols_clearTypedef(TypeDef *s);
