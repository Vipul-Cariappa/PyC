#/usr/bin/sh

printf "\nBuilding PyC\n"
if ! make config=memorytest ; then
    exit 1
fi

printf "\nCopying Files\n" 
if ! cp ./bin/MemoryTest/libPyC.so ./tests/PyC.so ; then
    exit 1
fi

printf "\nBuilding Testing library\n"
if ! g++ -g -Wall -shared -fPIC -I./tests/c/ -o tests/libcppmodule.so tests/c/module.cpp ; then
    exit 1
fi
if ! gcc -g -Wall -shared -fPIC -I./tests/c/ -o tests/libcmodule.so tests/c/module.c ; then
    exit 1
fi

printf "\nRunning the tests\n"
if ! python3.10d -X dev -X showrefcount tests/test.py -v ; then
    exit 1
fi
