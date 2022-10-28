from distutils.core import setup, Extension


with open("README.md") as f:
    module_discription = f.read()

module = Extension(
    "PyC", 
    sources=[
        "src/CppTypeInfo.c",
        "src/CppWrapper_PyTypes.c",
        "src/PyC.c",
        "src/TypeConversions.c",
        "src/Py_C_Types.c",
    ],
    include_dirs=[
        "src/",
        "/usr/include/x86_64-linux-gnu",
        "/usr/lib/llvm-12/include",
    ],
    libraries=[
        "ffi",
        "clang-12",
    ],
)

setup(
    name="PyC",
    version="0.0.1",
    description="General Purpose Binding between Python & C/C++",
    ext_modules=[module],
    author="Vipul Cariappa",
    author_email="vipulcariappa@gmail.com",
    url="https://github.com/Vipul-Cariappa/PyC",
    long_description=module_discription,
)
