#!/bin/sh

printf "\nCompiling the bindings\n"
make

printf "\nCopying the bindings\n"
cp ./bin/Debug/libPyC.so ./tests/PyC.so

printf "\nCompiling test module\n"
g++ -g -Wall -shared -fPIC -I./tests/c/ -o tests/libcmodule.so tests/c/module.cpp

printf "\nRunning the tests\n"
python tests/test.py
