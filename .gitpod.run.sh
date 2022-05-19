#/usr/bin/sh

printf "\nBuilding PyC\n"
make

printf "\nCopying Files\n" 
cp ./bin/Debug/libPyC.so ./tests/PyC.so

printf "\nBuilding Testing library\n"
g++ -g -Wall -shared -fPIC -I./tests/c/ -o tests/libcppmodule.so tests/c/module.cpp
gcc -g -Wall -shared -fPIC -I./tests/c/ -o tests/libcmodule.so tests/c/module.c

printf "\nRunning the tests\n"
python tests/test.py
