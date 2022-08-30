#pragma once
#include "DataStructures.h"
#include "ffi.h"
#include "clang-c/Index.h"
#include <stdbool.h>

struct _Symbols {
  const char *name;

  list_Function_t *funcs; // mapping of function names to struct Function
  size_t funcCount;

  list_Structure_t *structs; // mapping of struct names to struct Structure
  size_t structsCount;

  list_Union_t *unions; // mapping of union names to union Structure
  size_t unionsCount;

  list_Class_t *classes; // mapping of class names to struct Class
  size_t classesCount;

  list_Global_t *globals; // mapping of global names to struct Global
  size_t globalsCount;

  list_TypeDef_t *typedefs; // mapping of typedef names to struct TypedDef
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
