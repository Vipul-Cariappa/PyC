#include "CppTypeInfo.h"
#include <stdio.h>
#include <time.h>

bool ERROR_OCCURRED = false;

ffi_type *c_type_to_ffi_type(unsigned int c_type) {
    if (c_type & POINTER) {
        return &ffi_type_pointer;
    } else if (c_type == VOID) {
        return &ffi_type_void;
    } else if (c_type == BOOL) {
        return &ffi_type_uchar;
    } else if (c_type == CHAR) {
        return &ffi_type_schar;
    } else if (c_type == (CHAR | UNSIGNED)) {
        return &ffi_type_uchar;
    } else if (c_type == SHORT) {
        return &ffi_type_sshort;
    } else if (c_type == (SHORT | UNSIGNED)) {
        return &ffi_type_ushort;
    } else if (c_type == INT) {
        return &ffi_type_sint;
    } else if (c_type == (INT | UNSIGNED)) {
        return &ffi_type_uint;
    } else if (c_type == LONG) {
        return &ffi_type_slong;
    } else if (c_type == (LONG | UNSIGNED)) {
        return &ffi_type_ulong;
    } else if (c_type == FLOAT) {
        return &ffi_type_float;
    } else if (c_type == DOUBLE) {
        return &ffi_type_double;
    } else if (c_type == STRUCT) {
        // TODO
        return NULL;
    } else if (c_type == UNION) {
        // TODO
        return NULL;
    } else if (c_type & ARRAY) {
        // TODO
        return NULL;
    }

    return NULL;
}

unsigned int CXType_to_c_type(CXType type) {
    enum CXTypeKind type_kind = type.kind;

    if (type_kind == CXType_Void) {
        return VOID;
    } else if (type_kind == CXType_Bool) {
        return BOOL;
    } else if (type_kind == CXType_UChar) {
        return CHAR | UNSIGNED;
    } else if (type_kind == CXType_SChar) {
        return CHAR;
    } else if (type_kind == CXType_Char_S) {
        return CHAR;
    } else if (type_kind == CXType_Short) {
        return SHORT;
    } else if (type_kind == CXType_UShort) {
        return SHORT | UNSIGNED;
    } else if (type_kind == CXType_Int) {
        return INT;
    } else if (type_kind == CXType_UInt) {
        return INT | UNSIGNED;
    } else if (type_kind == CXType_Long) {
        return LONG;
    } else if (type_kind == CXType_ULong) {
        return LONG | UNSIGNED;
    } else if (type_kind == CXType_Float) {
        return FLOAT;
    } else if (type_kind == CXType_Double) {
        return DOUBLE;
    } else if (type_kind == CXType_Record) {
        CXCursor cursor = clang_getTypeDeclaration(type);

        if (cursor.kind == CXCursor_StructDecl) {
            return STRUCT;

        } else if (cursor.kind == CXCursor_UnionDecl) {
            return UNION;

        } else {
            fprintf(stderr, RED "Error: " BLU "Got a CXType of CXType_Record, but CXCursor is neither "
                                "CXCursor_UnionDecl nor CXCursor_StructDecl\n" RESET);
        }

    } else if (type_kind == CXType_Pointer) {
        unsigned int underlying_c_type = CXType_to_c_type(clang_getPointeeType(type));
        if (underlying_c_type == 0) {
            return 0;
        }

        return underlying_c_type | POINTER;

    } else if (type_kind == CXType_ConstantArray) {
        unsigned int underlying_type = CXType_to_c_type(clang_getArrayElementType(type));
        if (underlying_type == 0) {
            return 0;
        }

        return underlying_type | ARRAY;
    } else if (type_kind == CXType_Typedef) {
        return CXType_to_c_type(clang_getTypedefDeclUnderlyingType(clang_getTypeDeclaration(type)));
    } else if (type_kind == CXType_Elaborated) {
        return CXType_to_c_type(clang_Type_getNamedType(type));
    }

    return 0;
}

void *get_additional_info(CXCursor cursor, unsigned int c_type, const Symbols *scope) {
    if ((c_type == STRUCT) || (c_type == (STRUCT | POINTER)) || (c_type == UNION) || (c_type == (UNION | POINTER))) {
        CXType cx_type = clang_getCursorType(cursor);
        CXString_to_string(clang_getCursorSpelling(cursor), struct_name);
        Record *result = map_Record_get(scope->records, struct_name);
        return strdup(string_get_cstr(struct_name));
    }

    if ((c_type & ARRAY)) {
        Array *arr = malloc(sizeof(Array));

        CXType type      = clang_getCursorType(cursor);
        long long length = clang_getArraySize(type);
        arr->length      = length;

        CXType elements_type = clang_getArrayElementType(type);
        arr->c_type          = CXType_to_c_type(elements_type);
        arr->additional_info = get_additional_info(clang_getTypeDeclaration(elements_type), arr->c_type, scope);

        return arr;
    }

    return NULL;
}

enum CXChildVisitResult visitor(CXCursor cursor, CXCursor parent, CXClientData client_data);

int main(int argc, char *argv[]) {
    if (argc == 1) {
        fprintf(stderr, "Expected atleast 1 argument to parse the file and display its contents\n");
        return EXIT_FAILURE;
    }

    char *filename = argv[1];

    CXIndex index = clang_createIndex(1, 0);
    CXTranslationUnit unit;

    enum CXErrorCode err_code =
        clang_parseTranslationUnit2(index, filename, NULL, 0, NULL, 0, CXTranslationUnit_SkipFunctionBodies, &unit);
    if (err_code != CXError_Success) {
        fprintf(stderr, "Could not parse the given file\n");
        return EXIT_FAILURE;
    }

    Symbols sym = {0};
    init_Symbols(&sym);
    sym.name = strdup(argv[1]);

    CXCursor cursor = clang_getTranslationUnitCursor(unit);

    clang_visitChildren(cursor, visitor, &sym);

    clang_disposeTranslationUnit(unit);
    clang_disposeIndex(index);

    print_Symbol(&sym);
    clear_Symbols(&sym);
}

void handle_CXType_void(CXCursor cursor, CXType type) {}
void handle_CXType_Bool(CXCursor cursor, CXType type) {}
void handle_CXType_UChar(CXCursor cursor, CXType type) {}
void handle_CXType_UShort(CXCursor cursor, CXType type) {}
void handle_CXType_UInt(CXCursor cursor, CXType type) {}
void handle_CXType_ULong(CXCursor cursor, CXType type) {}
void handle_CXType_ULongLong(CXCursor cursor, CXType type) {}
void handle_CXType_Char_S(CXCursor cursor, CXType type) {}
void handle_CXType_SChar(CXCursor cursor, CXType type) {}
void handle_CXType_Short(CXCursor cursor, CXType type) {}
void handle_CXType_Int(CXCursor cursor, CXType type) {}
void handle_CXType_Long(CXCursor cursor, CXType type) {}
void handle_CXType_LongLong(CXCursor cursor, CXType type) {}
void handle_CXType_Float(CXCursor cursor, CXType type) {}
void handle_CXType_Double(CXCursor cursor, CXType type) {}
void handle_CXType_LongDouble(CXCursor cursor, CXType type) {}
void handle_CXType_Pointer(CXCursor cursor, CXType type) {}
void handle_CXType_Record(CXCursor cursor, CXType type) {}
void handle_CXType_Enum(CXCursor cursor, CXType type) {}
void handle_CXType_Typedef(CXCursor cursor, CXType type) {}
void handle_CXType_ConstantArray(CXCursor cursor, CXType type) {}
void handle_CXType_IncompleteArray(CXCursor cursor, CXType type) {}
void handle_CXType_Elaborated(CXCursor cursor, CXType type) {}

void handle_CXCursor_VarDecl(CXCursor cursor, Symbols *sym) {
    CXType cx_type = clang_getCursorType(cursor);
    CXString_to_string(clang_getCursorSpelling(cursor), key);
    const char *variable_name      = strdup(string_get_cstr(key));
    unsigned int c_type            = CXType_to_c_type(cx_type);
    void* additional_info = get_additional_info(cursor, c_type, sym);

    if (c_type == 0) {
        // Given c_type is not supported. Skipping
        CXString_to_char_ptr(clang_getTypeKindSpelling(cx_type.kind), type_name);
        printf(GRN "Warning: " RESET
                   "Given Variable type could is not supported\n  Variable Name: %s\t Type Name: %s\n  Skipping\n\n",
               variable_name, type_name);
        string_clear(key);
        free(type_name);
        free((void *)variable_name);
        return;
    }

    GlobalVariable var = {.name = variable_name, .c_type = c_type, .additional_info = additional_info};

    map_GlobalVariable_set_at(sym->globals, key, var);
}

enum CXChildVisitResult record_visitor(CXCursor cursor, CXCursor parent, CXClientData client_data) {
    void **info        = client_data;
    Record *s          = (Record *)info[0];
    const Symbols *sym = (Symbols *)info[1];

    CXType cx_type = clang_getCursorType(cursor);
    CXString_to_string(clang_getCursorSpelling(cursor), attribute_name);
    unsigned int c_type            = CXType_to_c_type(cx_type);
    void* additional_info = get_additional_info(cursor, c_type, sym);
    long long offset = clang_Type_getOffsetOf(clang_getCursorType(parent), string_get_cstr(attribute_name));

    if (c_type == 0) {
        // Given c_type is not supported. Skipping
        CXString_to_char_ptr(clang_getTypeKindSpelling(cx_type.kind), type_name);
        printf(GRN "Warning: " RESET "Given Struct type could not be supported due to following attribute\n  Variable "
                   "Name: %s\t Type Name: %s\n  Skipping\n\n",
               string_get_cstr(attribute_name), type_name);

        string_clear(attribute_name);
        free(type_name);
        ERROR_OCCURRED = true;
        return CXChildVisit_Break;
    }

    arr_string_push_back(s->attr_names, attribute_name);
    arr_uint_push_back(s->c_types, c_type);
    arr_void_push_back(s->additional_info, additional_info);
    arr_llong_push_back(s->offsets, offset);
    ++(s->attr_count);

    return CXChildVisit_Continue;
}

void handle_CXCursor_RecordDecl(CXCursor cursor, Symbols *sym, bool is_union) {
    CXType cx_type = clang_getCursorType(cursor);
    CXString_to_string(clang_getCursorSpelling(cursor), key);

    // if (clang_Cursor_isAnonymousRecordDecl(cursor) || string_empty_p(key)) {
    //     string_clear(key);
    //     string_init_printf(key, "AnonymousRecord%d", clock());
    // }

    Record s;
    init_Record(&s);
    s.name     = strdup(string_get_cstr(key));
    s.size     = clang_Type_getSizeOf(cx_type);
    s.is_union = is_union;

    void *info[2] = {&s, sym};

    clang_visitChildren(cursor, record_visitor, info);

    if (ERROR_OCCURRED) {
        clear_Record(&s);
        ERROR_OCCURRED = false;
        string_clear(key);
        return;
    }

    map_Record_set_at(sym->records, key, s);
}

enum CXChildVisitResult enum_visitor(CXCursor cursor, CXCursor parent, CXClientData client_data) {
    Enum *s = (Enum *)client_data;

    long long value = clang_getEnumConstantDeclValue(cursor);
    CXString_to_string((clang_getCursorSpelling(cursor)), name);

    map_string_to_llong_set_at(s->values, name, value);

    return CXChildVisit_Continue;
}

void handle_CXCursor_EnumDecl(CXCursor cursor, Symbols *sym) {
    CXType cx_type = clang_getCursorType(cursor);
    CXString_to_string(clang_getCursorSpelling(cursor), key);

    Enum s;
    init_Enum(&s);
    s.name   = strdup(string_get_cstr(key));
    s.c_type = CXType_to_c_type(clang_getEnumDeclIntegerType(cursor));

    if (s.c_type == 0) {
        // Given c_type is not supported. Skipping
        CXString_to_char_ptr(clang_getTypeKindSpelling(clang_getEnumDeclIntegerType(cursor).kind), type_name);
        printf(GRN "Warning: " RESET "Given Enum type is not supported\n  Underlying int type: %s  Skipping\n\n",
               type_name);

        free(type_name);
        return;
    }

    clang_visitChildren(cursor, enum_visitor, &s);

    map_Enum_set_at(sym->enums, key, s);
}

void handle_CXCursor_FunctionDecl(CXCursor cursor, Symbols *sym) {}

void handle_CXCursor_TypedefDecl(CXCursor cursor, Symbols *sym) {
    CXType cx_type            = clang_getCursorType(cursor);
    CXType type_defed_cx_type = clang_getTypedefDeclUnderlyingType(cursor);

    CXString_to_string(clang_getTypedefName(cx_type), type_defed_name);
    unsigned int c_type            = CXType_to_c_type(type_defed_cx_type);
    void* additional_info = get_additional_info(clang_getTypeDeclaration(type_defed_cx_type), c_type, sym);

    if (c_type == 0) {
        // Given c_type is not supported. Skipping
        CXString_to_char_ptr(clang_getTypeKindSpelling(type_defed_cx_type.kind), type_name);
        printf(GRN "Warning: " RESET "Given Typedef type is not supported\n  Underlying type: %s\n  Skipping\n\n",
               type_name);

        free(type_name);
        return;
    }

    TypeDef s;
    init_TypeDef(&s);

    s.type_defed_name = strdup(string_get_cstr(type_defed_name));
    s.underlying_type = c_type;
    s.additional_info = additional_info;

    map_TypeDef_set_at(sym->typedefs, type_defed_name, s);
}

enum CXChildVisitResult visitor(CXCursor cursor, CXCursor parent, CXClientData client_data) {
    CXType type = clang_getCursorType(cursor);
    // CXString_to_string(clang_getCursorDisplayName(cursor), cursor_display_name);
    // CXString_to_string(clang_getCursorSpelling(cursor), cursor_name);
    // CXString_to_string(clang_getCursorKindSpelling(cursor.kind), cursor_kind_name);
    // CXString_to_string(clang_getCursorPrettyPrinted(cursor, 0), cursor_pretty_print);
    // CXString_to_string(clang_getTypeSpelling(type), type_name);

    // printf("Cursor Name: %s\n", string_get_cstr(cursor_name));
    // printf("Cursor Display Name: %s\n", string_get_cstr(cursor_display_name));
    // printf("Cursor Kind Name: %s, %d\n", string_get_cstr(cursor_kind_name), cursor.kind);
    // printf("  Type Name: %s, %d\n", string_get_cstr(type_name), type.kind);
    // printf("Cursor Pretty Print: \n%s\n", string_get_cstr(cursor_pretty_print));
    // printf("\n\n");

    // string_clear(cursor_display_name);
    // string_clear(cursor_name);
    // string_clear(cursor_kind_name);
    // string_clear(cursor_pretty_print);
    // string_clear(type_name);

    Symbols *sym = (Symbols *)client_data;

    if (cursor.kind == CXCursor_VarDecl) {
        handle_CXCursor_VarDecl(cursor, sym);
    } else if (cursor.kind == CXCursor_StructDecl) {
        handle_CXCursor_RecordDecl(cursor, sym, false);
    } else if (cursor.kind == CXCursor_UnionDecl) {
        handle_CXCursor_RecordDecl(cursor, sym, true);
    } else if (cursor.kind == CXCursor_EnumDecl) {
        handle_CXCursor_EnumDecl(cursor, sym);
    } else if (cursor.kind == CXCursor_FunctionDecl) {
        // TODO
        // FIXME: Should not allow variable length argument functions
    } else if (cursor.kind == CXCursor_TypedefDecl) {
        handle_CXCursor_TypedefDecl(cursor, sym);
    }

    return CXChildVisit_Continue;
}
