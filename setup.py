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
    ],
    include_dirs=[
        "src/",
        "/usr/include/python3.9",
        "/usr/local/include",
        "/usr/local/include/qlibc",
        "/usr/lib/llvm-10/include",
    ],
    library_dirs=[
        "/usr/local/lib",
        "usr/lib/x86_64-linux-gnu",
    ],
    libraries=[
        "ffi",
        "clang-10",
        "qlibc",
    ],
    extra_compile_args=[
        "-ftest-coverage",
        "-fprofile-arcs",
    ],
    extra_link_args=[
        "-lgcov",
    ]
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
