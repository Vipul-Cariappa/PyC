from PyC import LoadCpp

cppModule = LoadCpp("tests/libcmodule.so", "tests/c/module.hpp")
print(cppModule)
print(cppModule.add)
print(cppModule.add(1, 2))
print(cppModule.pi)
print(cppModule.pi(1))
print(cppModule.pi(1_000_000))
print(cppModule.concat)
print(cppModule.concat("Vipul", " Cariappa"))
print(cppModule.MAX)
print(cppModule.PROGRAM)
print(cppModule.f)
