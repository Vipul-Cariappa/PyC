#pragma once

#include "ffi.h"
#include "m-array.h"
#include "m-dict.h"
#include "m-string.h"
#include "clang-c/Index.h"

#define POINTER  0b01000000000000000000000000000000
#define ARRAY    0b00100000000000000000000000000000
#define UNSIGNED 0b00010000000000000000000000000000
#define VOID     1
#define BOOL     2
#define CHAR     3
#define SHORT    4
#define INT      5
#define LONG     6
#define FLOAT    7
#define DOUBLE   8
#define STRUCT   9
#define UNION    10

#define char_ptr_to_string(str, name)                                                                                  \
    string_t name;                                                                                                     \
    string_init(name);                                                                                                 \
    string_set_str(name, str)

#define CXString_to_string(str, name)                                                                                  \
    string_t name;                                                                                                     \
    {                                                                                                                  \
        string_init(name);                                                                                             \
        CXString tmp = (str);                                                                                          \
        string_set_str(name, clang_getCString(tmp));                                                                   \
        clang_disposeString(tmp);                                                                                      \
    }

#define CXString_to_char_ptr(str, name)                                                                                \
    char *name;                                                                                                        \
    {                                                                                                                  \
        CXString tmp = (str);                                                                                          \
        name         = strdup(clang_getCString(tmp));                                                                  \
        clang_disposeString(tmp);                                                                                      \
    }

static inline void ffi_type_ptr_clear(ffi_type *obj) {}

ARRAY_DEF(arr_uint, unsigned int)
ARRAY_DEF(arr_void, void *)
ARRAY_DEF(arr_size, size_t)
ARRAY_DEF(arr_llong, long long)
ARRAY_DEF(arr_string, string_t)
ARRAY_DEF(arr_ffi_type_ptr, ffi_type *, (CLEAR(ffi_type_ptr_clear), TYPE(ffi_type *)))
DICT_DEF2(map_string_to_llong, string_t, long long)

// Array should only be assigned to additional_info attribute of other struct(s). And should be cleared be the
// parent struct before clearing itself
typedef struct _Array {
    unsigned int c_type; // underlying type
    long long length;
    void *additional_info; // to store info about the struct or pointer
} Array;

static inline void clear_Array(Array *s) {
    if (s->additional_info) {
        free(s->additional_info);
    }
}

static inline void init_Array(Array *s) {
    *s = (Array){0};
}

static inline void init_set_Array(Array *s, const Array a) {
    *s = a;
}

typedef struct _GlobalVariable {
    const char *name;
    unsigned int c_type;
    void *additional_info; // to store info about the struct or pointer
} GlobalVariable;

static inline void clear_GlobalVariable(GlobalVariable *s) {
    free((void *)s->name);

    if (s->c_type & ARRAY) {
        clear_Array(s->additional_info);
    }
    free(s->additional_info);
}

static inline void init_GlobalVariable(GlobalVariable *s) {
    *s = (GlobalVariable){0};
}

static inline void init_set_GlobalVariable(GlobalVariable *s, const GlobalVariable a) {
    *s = a;
}

void print_GlobalVariable(const GlobalVariable *s);

typedef struct _Record {
    const char *name;
    arr_string_t attr_names;
    arr_uint_t c_types;
    arr_llong_t offsets;
    arr_void_t additional_info; // to store info about the struct or pointer
    ffi_type type;
    size_t attr_count;
    long long size;
    bool is_union;
} Record;

static inline void clear_Record(Record *s) {
    free((void *)s->name);
    arr_string_clear(s->attr_names);
    arr_llong_clear(s->offsets);

    // clearing additional info of attribute's type
    arr_uint_it_t it;
    int i = 0;
    for (arr_uint_it(it, s->c_types); !arr_uint_end_p(it); arr_uint_next(it)) {
        unsigned int c_type   = *arr_uint_cref(it);
        void **additional_info = arr_void_get(s->additional_info, i);

        if (c_type & ARRAY) {
            clear_Array(*additional_info);
        }
        free(*additional_info);

        ++i;
    }

    arr_uint_clear(s->c_types);
    arr_void_clear(s->additional_info);
    free(s->type.elements);
}

static inline void init_Record(Record *s) {
    *s = (Record){0};
    arr_string_init(s->attr_names);
    arr_uint_init(s->c_types);
    arr_llong_init(s->offsets);
    arr_void_init(s->additional_info);
}

static inline void init_set_Record(Record *s, const Record a) {
    *s = a;
}

void print_Record(const Record *s);

typedef struct _Enum {
    const char *name;
    map_string_to_llong_t values;
    unsigned int c_type; // size of enum
} Enum;

static inline void clear_Enum(Enum *s) {
    free((void *)s->name);
    map_string_to_llong_clear(s->values);
}

static inline void init_Enum(Enum *s) {
    *s = (Enum){0};
    map_string_to_llong_init(s->values);
}

static inline void init_set_Enum(Enum *s, const Enum a) {
    *s = a;
}

void print_Enum(const Enum *s);

typedef struct _TypeDef {
    const char *type_defed_name;
    unsigned int underlying_type;
    void *additional_info; // to store info about the struct or pointer
} TypeDef;

static inline void clear_TypeDef(TypeDef *s) {
    free((void *)s->type_defed_name);

    // clearing additional info of underlying type
    if (s->underlying_type & ARRAY) {
        clear_Array(s->additional_info);
    }
    free(s->additional_info);
}

static inline void init_TypeDef(TypeDef *s) {
    *s = (TypeDef){0};
}

static inline void init_set_TypeDef(TypeDef *s, const TypeDef a) {
    *s = a;
}

void print_TypeDef(const TypeDef *s);

typedef struct _Function {
    const char *name;
    unsigned int return_type;
    void *return_type_additional_info; // to store info about the struct or pointer
    arr_uint_t args_type;
    arr_void_t additional_info; // to store info about the struct or pointer
    ffi_cif *cif;               // will be initialized on first call to the function
} Function;

static inline void clear_Function(Function *s) {
    free((void *)s->name);

    // clearing additional info of return type
    if (s->return_type & ARRAY) {
        clear_Array(s->return_type_additional_info);
    }
    free(s->additional_info);

    // clearing additional info of arguments type
    arr_uint_it_t it;
    int i = 0;
    for (arr_uint_it(it, s->args_type); !arr_uint_end_p(it); arr_uint_next(it)) {
        unsigned int c_type    = *arr_uint_cref(it);
        void **additional_info = arr_void_get(s->additional_info, i);

        if (c_type & ARRAY) {
            clear_Array(*additional_info);
        }
        free(*additional_info);

        ++i;
    }

    arr_uint_clear(s->args_type);
    arr_void_clear(s->additional_info);
    free(s->cif);
}

static inline void init_Function(Function *s) {
    *s = (Function){0};
    arr_uint_init(s->args_type);
    arr_void_init(s->additional_info);
}

static inline void init_set_Function(Function *s, const Function a) {
    *s = a;
}

void print_Function(const Function *s);

DICT_DEF2(map_GlobalVariable, string_t, STRING_OPLIST, GlobalVariable,
          (INIT(API_2(init_GlobalVariable)), INIT_SET(API_2(init_set_GlobalVariable)),
           SET(API_1(M_SET_THROUGH_INIT_SET)), CLEAR(API_2(clear_GlobalVariable)), TYPE(GlobalVariable)))

DICT_DEF2(map_Record, string_t, STRING_OPLIST, Record,
          (INIT(API_2(init_Record)), INIT_SET(API_2(init_set_Record)), SET(API_1(M_SET_THROUGH_INIT_SET)),
           CLEAR(API_2(clear_Record)), TYPE(Record)))

DICT_DEF2(map_Enum, string_t, STRING_OPLIST, Enum,
          (INIT(API_2(init_Enum)), INIT_SET(API_2(init_set_Enum)), SET(API_1(M_SET_THROUGH_INIT_SET)),
           CLEAR(API_2(clear_Enum)), TYPE(Enum)))

DICT_DEF2(map_TypeDef, string_t, STRING_OPLIST, TypeDef,
          (INIT(API_2(init_TypeDef)), INIT_SET(API_2(init_set_TypeDef)), SET(API_1(M_SET_THROUGH_INIT_SET)),
           CLEAR(API_2(clear_TypeDef)), TYPE(TypeDef)))

DICT_DEF2(map_Function, string_t, STRING_OPLIST, Function,
          (INIT(API_2(init_Function)), INIT_SET(API_2(init_set_Function)), SET(API_1(M_SET_THROUGH_INIT_SET)),
           CLEAR(API_2(clear_Function)), TYPE(Function)))

typedef struct _Symbols {
    const char *name;
    map_Function_t funcs;         // mapping of function names to struct Function
    map_Record_t records;         // mapping of struct names to struct Structure
    map_GlobalVariable_t globals; // mapping of global names to struct Global
    map_TypeDef_t typedefs;       // mapping of typedef names to struct TypedDef
    map_Enum_t enums;             // mapping of enum names to struct Enum
} Symbols;

static inline void init_Symbols(Symbols *s) {
    map_Function_init(s->funcs);
    map_Record_init(s->records);
    map_GlobalVariable_init(s->globals);
    map_TypeDef_init(s->typedefs);
    map_Enum_init(s->enums);
}

static inline void clear_Symbols(Symbols *s) {
    free((void *)s->name);
    map_Function_clear(s->funcs);
    map_Record_clear(s->records);
    map_GlobalVariable_clear(s->globals);
    map_TypeDef_clear(s->typedefs);
    map_Enum_clear(s->enums);
}

void print_Symbol(const Symbols *s);

#define RED   "\x1B[31m"
#define GRN   "\x1B[32m"
#define YEL   "\x1B[33m"
#define BLU   "\x1B[34m"
#define MAG   "\x1B[35m"
#define CYN   "\x1B[36m"
#define WHT   "\x1B[37m"
#define RESET "\x1B[0m"
