printf "\nCompiling test module\n"
g++ -g -Wall -shared -fPIC -I./tests/c/ -o tests/libcmodule.so tests/c/module.cpp

printf "\nRunning the tests\n"
python tests/test.py
