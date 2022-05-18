#pragma once

#include <iostream>
#include <vector>
#include "ffi.h"
#include "clang-c/Index.h"

extern CXString (*mangled_name_getter_fn)(CXCursor);
CXString GET_MANGLED_NAME(CXCursor cursor);

std::string get_type_string(ffi_type type);

class FunctionType
{
public:
    ffi_type return_type;
    std::vector<ffi_type> args_types;
    size_t args_count;

    FunctionType(ffi_type return_type_, std::vector<ffi_type> args_types_, size_t args_count_)
    {
        return_type = return_type_;
        args_types = args_types_;
        args_count = args_count_;
    }

    FunctionType(CXType cpp_type_)
    {
        return_type = ffi_type_void;
        args_count = 0;
        args_types = {};
    }

    ~FunctionType()
    {
        // TODO: implement
    }
};

class Function
{
public:
    std::string name;
    std::string mangledname;
    FunctionType *type;
    size_t functions_count = 0;
    CXCursor cpp_type;

    Function(std::string name_, CXCursor cpp_type_)
    {
        cpp_type = cpp_type_;
        name = name_;
    }

    ~Function()
    {
        // TODO: implement
    }

    void print_self()
    {
        std::cout << "Name: " << name << "\n";
        // std::cout << "Name: " << name << "; " << clang_getCString(clang_getTypeSpelling(cpp_type)) "\n";
        std::cout << "\tMangledname : " << mangledname << "\n";
        std::cout << "\n";
        // TODO: print types
    }

    void addFuncDecl(std::string mangledname_, FunctionType *type_)
    {
        mangledname = std::string(mangledname_);
        type = type_;
        functions_count++;
    }
};

class Global
{
public:
    std::string name;
    std::string mangledname;
    ffi_type c_type;
    CXType datatype;

    Global(std::string name_, std::string mangledname_, ffi_type c_type_, CXType datatype_)
    {
        name = name_;
        mangledname = mangledname_;
        c_type = c_type_;
        datatype = datatype_;
    }

    ~Global()
    {
        // TODO: implement
    }

    void print_self()
    {
        std::cout << "Name: " << name << " MangledName: " << mangledname << " Type: " << get_type_string(c_type) << "; " << clang_getCString(clang_getTypeSpelling(datatype)) << "\n";
    }
};

class Structure
{
public:
    std::string name;
    std::vector<std::string> attr_names;
    std::vector<ffi_type> types;
    size_t types_count;
    size_t struct_size;
    ffi_type type;
    CXType cpp_type;

    Structure(std::string name_, CXType cpp_type_)
    {
        name = name_;
        attr_names = {};
        types = {};
        types_count = 0;
        struct_size = 0;
        cpp_type = cpp_type_;
    }

    Structure(std::string name_, std::vector<std::string> attr_names_, std::vector<ffi_type> type_, size_t count_, size_t struct_size_, CXType cpp_type_)
    {
        name = name_;
        attr_names = attr_names_;
        types = type_;
        types_count = count_;
        struct_size = struct_size_;
        cpp_type = cpp_type_;

        type = create_ffi_struct_type(types);
    }

    ffi_type create_ffi_struct_type(std::vector<ffi_type> types_list)
    {
        ffi_type **ffi_type_list = (ffi_type **)malloc(sizeof(ffi_type *) * (types_count + 1));

        for (int i = 0; i < types_count; i++)
        {
            ffi_type_list[i] = &types_list.at(i);
        }
        ffi_type_list[types_count] = nullptr;

        ffi_type struct_type;
        struct_type.size = 0;
        struct_type.alignment = 0;
        struct_type.type = FFI_TYPE_STRUCT;
        struct_type.elements = ffi_type_list;

        return struct_type;
    }

    ~Structure()
    {
        // TODO: implement
    }

    void print_self()
    {
        std::cout << "Name: " << name << " struct size: " << struct_size << "; " << clang_getCString(clang_getTypeSpelling(cpp_type)) << "\n";
        for (int i = 0; i < types_count; i++)
        {
            std::cout << "\t" << attr_names.at(i) << " : " << get_type_string(types.at(i)) << "\n";
            // TODO: print type
        }
        std::cout << "\n";
    }
};

class Class
{
public:
    // TODO: implement
    void print_self()
    {
        // TODO: implement
        std::cout << "\n";
    }
};

class Symbols
{
public:
    std::string name;
    std::vector<Function *> funcs;
    std::vector<Structure *> structs;
    std::vector<Class *> classes;
    std::vector<Global *> globals;

    Symbols(std::string name)
    {
        name = std::string(name);
    }

    ~Symbols()
    {
        // TODO: implement
    }

    void print_self()
    {
        std::cout << "Name: " << name << "\n";

        std::cout << "Functions: "
                  << "\n";
        for (Function *&i : funcs)
        {
            i->print_self();
        }

        std::cout << "Structures: "
                  << "\n";
        for (Structure *&i : structs)
        {
            i->print_self();
        }

        std::cout << "Classes: "
                  << "\n";
        for (Class *&i : classes)
        {
            i->print_self();
        }

        std::cout << "Globals: "
                  << "\n";
        for (Global *&i : globals)
        {
            i->print_self();
        }
    }

    void append_function(Function *func)
    {
        funcs.push_back(func);
    }

    void append_struct(Structure *struct_)
    {
        structs.push_back(struct_);
    }

    void append_class(Class *class_)
    {
        classes.push_back(class_);
    }

    void append_global(Global *global)
    {
        globals.push_back(global);
    }

    Function *get_function(const char *name_)
    {
        for (Function *&i : funcs)
        {
            if (i->name == name_)
            {
                return i;
            }
        }

        return nullptr;
    }

    Global *get_global(const char *name_)
    {
        for (Global *&i : globals)
        {
            if (i->name == name_)
            {
                return i;
            }
        }

        return nullptr;
    }
};

Symbols *get_symbols(const char *filename);
int free_symbols(Symbols *symbols);
