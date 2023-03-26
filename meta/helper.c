#include "CppTypeInfo.h"
#include <stdio.h>
#include <string.h>

char *c_type_to_c_char(unsigned int c_type, void *additional_info) {
    if (c_type & POINTER) {
        char *underlying_type_string = c_type_to_c_char(c_type ^ POINTER, additional_info);
        char *type_string            = malloc(strlen(underlying_type_string) + 2);

        sprintf(type_string, "%s*", underlying_type_string);

        free(underlying_type_string);
        return type_string;

    } else if (c_type == VOID) {
        return strdup("void");
    } else if (c_type == BOOL) {
        return strdup("bool");
    } else if (c_type == CHAR) {
        return strdup("char");
    } else if (c_type == (CHAR | UNSIGNED)) {
        return strdup("unsigned char");
    } else if (c_type == SHORT) {
        return strdup("short");
    } else if (c_type == (SHORT | UNSIGNED)) {
        return strdup("unsigned short");
    } else if (c_type == INT) {
        return strdup("int");
    } else if (c_type == (INT | UNSIGNED)) {
        return strdup("unsigned int");
    } else if (c_type == LONG) {
        return strdup("long");
    } else if (c_type == (LONG | UNSIGNED)) {
        return strdup("unsigned long");
    } else if (c_type == FLOAT) {
        return strdup("float");
    } else if (c_type == DOUBLE) {
        return strdup("double");
    } else if (c_type == STRUCT) {
        char *type_string = malloc(strlen(additional_info) + 8);
        sprintf(type_string, "struct %s", (char *)additional_info);

        return type_string;

    } else if (c_type == UNION) {
        char *type_string = malloc(strlen(additional_info) + 7);
        sprintf(type_string, "union %s", (char *)additional_info);

        return type_string;

    } else if (c_type & ARRAY) {
        Array *arr_info              = (Array *)additional_info;
        char *underlying_type_string = c_type_to_c_char(arr_info->c_type, arr_info->additional_info);
        char *type_string            = malloc(strlen(underlying_type_string) + 42);

        sprintf(type_string, "%s[%lld]", underlying_type_string, arr_info->length);

        free(underlying_type_string);
        return type_string;
    }

    return NULL;
}

void print_GlobalVariable(const GlobalVariable *s) {
    char *type_info = c_type_to_c_char(s->c_type, s->additional_info);
    printf("  %s %s\n", s->name, type_info);
    free(type_info);
}

void print_Record(const Record *s) {
    if (s->is_union) {
        printf("  union %s {\n", s->name);
    } else {
        printf("  struct %s {\n", s->name);
    }

    for (size_t i = 0; i < s->attr_count; i++) {
        string_t *attr_name    = arr_string_get(s->attr_names, i);
        long long offset       = *arr_llong_get(s->offsets, i);
        void **additional_info = *arr_void_get(s->additional_info, i);
        unsigned int c_type    = *arr_uint_get(s->c_types, i);
        char *type_name        = c_type_to_c_char(c_type, additional_info);

        printf("   %s %s @ %lld,\n", string_get_cstr(*attr_name), type_name, offset);

        free(type_name);
    }
    printf("  }\n\n");
}

void print_Enum(const Enum *s) {
    char *type_name = c_type_to_c_char(s->c_type, NULL);
    printf("  %s @ %s:\n", s->name, type_name);
    free(type_name);

    map_string_to_llong_it_t it;
    for (map_string_to_llong_it(it, s->values); !map_string_to_llong_end_p(it); map_string_to_llong_next(it)) {
        const map_string_to_llong_itref_t *fn = map_string_to_llong_cref(it);
        printf("   %s %lld\n", string_get_cstr(fn->key), fn->value);
    }
    printf("\n");
}

void print_TypeDef(const TypeDef *s) {
    char *type_name = c_type_to_c_char(s->underlying_type, s->additional_info);
    printf("  %s -> %s\n", type_name, s->type_defed_name);
    free(type_name);
}

void print_Function(const Function *s) {
    //  TODO
    printf("  Function\n");
}

void print_Symbol(const Symbols *s) {
    printf("%s:\n", s->name);

    {
        printf("\n Global Variable:\n");
        map_GlobalVariable_it_t it;
        for (map_GlobalVariable_it(it, s->globals); !map_GlobalVariable_end_p(it); map_GlobalVariable_next(it)) {
            const map_GlobalVariable_itref_t *fn = map_GlobalVariable_cref(it);
            print_GlobalVariable(&(fn->value));
        }
    }
    {
        printf("\n Struct and Union:\n");
        map_Record_it_t it;
        for (map_Record_it(it, s->records); !map_Record_end_p(it); map_Record_next(it)) {
            const map_Record_itref_t *fn = map_Record_cref(it);
            print_Record(&(fn->value));
        }
    }
    {
        printf("\n Enum:\n");
        map_Enum_it_t it;
        for (map_Enum_it(it, s->enums); !map_Enum_end_p(it); map_Enum_next(it)) {
            const map_Enum_itref_t *fn = map_Enum_cref(it);
            print_Enum(&(fn->value));
        }
    }
    {
        printf("\n TypeDef:\n");
        map_TypeDef_it_t it;
        for (map_TypeDef_it(it, s->typedefs); !map_TypeDef_end_p(it); map_TypeDef_next(it)) {
            const map_TypeDef_itref_t *fn = map_TypeDef_cref(it);
            print_TypeDef(&(fn->value));
        }
    }
    {
        printf("\n Function:\n");
        map_Function_it_t it;
        for (map_Function_it(it, s->funcs); !map_Function_end_p(it); map_Function_next(it)) {
            const map_Function_itref_t *fn = map_Function_cref(it);
            print_Function(&(fn->value));
        }
    }
}
