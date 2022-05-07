#!/bin/sh

printf "\nCompiling the bindings\n"
python setup.py build -v

printf "\nCompiling the installings\n"
python setup.py install

printf "\nCompiling test module\n"
g++ -g -Wall -shared -fPIC -I./tests/c/ -o tests/libcmodule.so tests/c/module.cpp

printf "\nRunning the tests\n"
python tests/test.py
