from PyC import *
import PyC
from PyC import LoadCpp, print_CppModule

cache = dict()


def loader(library, header, cpp=False):
    if (library, header) in cache:
        return cache[(library, header)]

    module = LoadCpp(library, header, cpp=cpp)

    cache[(library, header)] = module

    return module


if __name__ == "__main__":
    cModule = loader("tests/libcmodule.so", "tests/c/module.h")
    cppModule = loader("tests/libcppmodule.so", "tests/c/module.hpp", cpp=True)

    print_CppModule(cppModule)
    print_CppModule(cModule)
