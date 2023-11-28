cmake_minimum_required(VERSION 3.8)

project(PyC)

find_package(PkgConfig REQUIRED)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
set(LIBRARY_OUTPUT_PATH "${CMAKE_BINARY_DIR}/lib")

pkg_check_modules(FFI REQUIRED libffi)
pkg_check_modules(PYTHON REQUIRED python3)

add_library(PyC SHARED src/CppTypeInfo.c src/CppWrapper_PyTypes.c src/Py_C_Types.c src/PyC.c src/TypeConversions.c)

target_include_directories(PyC PRIVATE "src/")

target_include_directories(PyC PRIVATE ${PYTHON_INCLUDE_DIRS})
target_link_libraries(PyC ${PYTHON_LIBRARIES})
target_compile_options(PyC PRIVATE ${PYTHON_CFLAGS_OTHER})

target_link_libraries(PyC ${FFI_LIBRARIES})
target_include_directories(PyC PRIVATE ${FFI_INCLUDE_DIRS})
target_compile_options(PyC PRIVATE ${FFI_CFLAGS_OTHER})

target_include_directories(PyC PRIVATE "/usr/lib/llvm-15/include")
target_link_libraries(PyC "clang-15")

project(cmodule)
add_library(cmodule SHARED tests/c/module.c)

project(cppmodule)
add_library(cppmodule SHARED tests/c/module.cpp)
