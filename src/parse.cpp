#include "clang-c/Index.h"
#include "ffi.h"
#include <iostream>
#include <stdexcept>
#include "binding.hpp"

std::ostream &operator<<(std::ostream &stream, const CXString &str)
{
    stream << clang_getCString(str);
    clang_disposeString(str);
    return stream;
}

ffi_type get_ffi_type(CXType type)
{
    switch (type.kind)
    {
    case CXType_Void:
        return ffi_type_void;
    case CXType_Int:
        return ffi_type_sint;
    case CXType_UInt:
        return ffi_type_uint;
    case CXType_Short:
        return ffi_type_sshort;
    case CXType_UShort:
        return ffi_type_ushort;
    case CXType_Float:
        return ffi_type_float;
    case CXType_Double:
        return ffi_type_double;
    case CXType_LongDouble:
        return ffi_type_longdouble;
    case CXType_Long:
        return ffi_type_sint64;
    case CXType_ULong:
        return ffi_type_uint64;
    case CXType_LongLong:
        return ffi_type_sint64;
    case CXType_ULongLong:
        return ffi_type_uint64;
    case CXType_Char_U:
        return ffi_type_uchar;
    case CXType_Char_S:
        return ffi_type_schar;
    case CXType_Pointer:
        return ffi_type_pointer;
    // case CXType_Typedef:
    //     return get_ffi_type(type);
    default:
        std::cerr << type.kind << "\n";
        throw std::runtime_error("Could not Identify the type");
    }
}

std::string get_type_string(ffi_type type)
{
    switch (type.type)
    {
    case FFI_TYPE_VOID:
        return "void";
    case FFI_TYPE_INT:
        return "int";
    case FFI_TYPE_FLOAT:
        return "float";
    case FFI_TYPE_DOUBLE:
        return "double";
    case FFI_TYPE_LONGDOUBLE:
        return "long double";
    case FFI_TYPE_UINT8:
        return "u_int8_t";
    case FFI_TYPE_UINT16:
        return "u_int16_t";
    case FFI_TYPE_UINT32:
        return "u_int32_t";
    case FFI_TYPE_UINT64:
        return "u_int64_t";
    case FFI_TYPE_SINT8:
        return "int8_t";
    case FFI_TYPE_SINT16:
        return "int16_t";
    case FFI_TYPE_SINT32:
        return "int32_t";
    case FFI_TYPE_SINT64:
        return "int64_t";
    case FFI_TYPE_STRUCT:
        return "struct";
    case FFI_TYPE_COMPLEX:
        return "complex";
    case FFI_TYPE_POINTER:
        return "void*";
    default:
        return "Could not figure out";
    }
}

CXChildVisitResult field_visitor(CXCursor cursor, CXCursor parent, CXClientData client_data)
{
    if (clang_getCursorKind(cursor) == CXCursor_FieldDecl)
    {
        CXString name = clang_getCursorSpelling(cursor);
        CXType type = clang_getCursorType(cursor);

        // std::cout << "Name: " << name << " Type: " << clang_getTypeSpelling(type) << "\n";
    }

    return CXChildVisit_Continue;
}

CXChildVisitResult visitor(CXCursor cursor, CXCursor parent, CXClientData client_data)
{
    Symbols *symbols = (Symbols *)client_data;

    std::cout << "Cursor '" << clang_getCursorSpelling(cursor) << "' of kind '"
              << clang_getCursorKindSpelling(clang_getCursorKind(cursor)) << "' "
              << clang_getCursorType(cursor).kind << "\n";

    // function
    if (clang_getCursorKind(cursor) == CXCursor_FunctionDecl)
    {
        // Function Declaration

        Function *func = new Function(clang_getCString(clang_getCursorSpelling(cursor)));
        std::string mangledname = clang_getCString(clang_Cursor_getMangling(cursor));
        std::vector<ffi_type> args_types = {};

        int argc = clang_Cursor_getNumArguments(cursor);
        for (int i = 0; i < argc; i++)
        {
            CXCursor arg = clang_Cursor_getArgument(cursor, i);
            CXType arg_type = clang_getCursorType(arg);

            args_types.push_back(get_ffi_type(arg_type));
            std::cout << "Type of arg " << i << ": " << clang_getTypeSpelling(arg_type) << "\n";
        }
        ffi_type return_type = get_ffi_type(clang_getCursorResultType(cursor));

        FunctionType *funcType = new FunctionType(return_type, args_types, argc);
        func->addFuncDecl(mangledname, funcType);
        symbols->append_function(func);
        std::cout << "Return type: " << clang_getTypeSpelling(clang_getCursorResultType(cursor)) << "\n";
        std::cout << std::endl;
    }

    // struct
    else if (clang_getCursorKind(cursor) == CXCursor_StructDecl)
    {
        std::string name = clang_getCString(clang_getCursorSpelling(cursor));

        Structure *obj = new Structure(name);

        clang_visitChildren(
            cursor,
            [](CXCursor cursor, CXCursor parent, CXClientData client_data)
            {
                if (clang_getCursorKind(cursor) == CXCursor_FieldDecl)
                {
                    std::string name = clang_getCString(clang_getCursorSpelling(cursor));
                    CXType type = clang_getCursorType(cursor);

                    Structure *obj = (Structure *)client_data;
                    obj->types_count++;
                    obj->attr_names.push_back(name);
                    obj->types.push_back(get_ffi_type(type));

                    std::cout << "Name: " << name << " Type: " << clang_getTypeSpelling(type) << "\n";
                }

                return CXChildVisit_Continue;
            },
            obj); // change field_visitor to lambda function

        // obj->type = obj->create_ffi_struct_type(obj->types);
        symbols->append_struct(obj);
    }

    // typedef
    else if (clang_getCursorKind(cursor) == CXCursor_TypedefDecl)
    {
        CXType underlying_type = clang_getTypedefDeclUnderlyingType(cursor);
        CXString name = clang_getCursorSpelling(cursor);

        std::cout << "Name: " << name << " Type: " << clang_getTypeSpelling(underlying_type) << "\n";
    }

    // global variables
    else if (clang_getCursorKind(cursor) == CXCursor_VarDecl)
    {
        std::string name = clang_getCString(clang_getCursorSpelling(cursor));
        std::string mangledname = clang_getCString(clang_Cursor_getMangling(cursor));
        CXType type = clang_getCursorType(cursor);

        Global *global = new Global(name, mangledname, get_ffi_type(type), type);

        symbols->append_global(global);

        std::cout << "Name: " << clang_getCursorSpelling(cursor) << " Type: " << clang_getTypeSpelling(clang_getCursorType(cursor)) << "\n";
    }

    std::cout << "\n";

    return CXChildVisit_Continue;
}

Symbols *get_symbols(const char *filename)
{
    Symbols *symbols = new Symbols(filename);

    CXIndex index = clang_createIndex(0, 0);
    CXTranslationUnit unit = clang_parseTranslationUnit(
        index,
        filename,
        nullptr,
        0,
        nullptr,
        0,
        CXTranslationUnit_None);

    if (unit == nullptr)
    {
        // std::cerr << "Unable to parse translation unit. Quitting." << std::endl;
        throw std::runtime_error("Unable to parse translation unit. Quitting.");
    }

    CXCursor cursor = clang_getTranslationUnitCursor(unit);
    clang_visitChildren(
        cursor,
        visitor,
        symbols);

    clang_disposeTranslationUnit(unit);
    clang_disposeIndex(index);

    return symbols;
}

int free_symbols(Symbols *symbols)
{
    return 0;
}
