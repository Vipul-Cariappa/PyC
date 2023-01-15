# PyC
![GitHub Workflow](https://img.shields.io/github/actions/workflow/status/Vipul-Cariappa/PyC/build_and_test.yml?branch=main&style=for-the-badge)
[![codecov](https://img.shields.io/codecov/c/github/Vipul-Cariappa/PyC?style=for-the-badge&token=EWE6SIYGC1)](https://codecov.io/gh/Vipul-Cariappa/PyC)

[![Open in Gitpod](https://gitpod.io/button/open-in-gitpod.svg)](https://gitpod.io/#https://github.com/Vipul-Cariappa/PyC)


General Purpose Binding between Python & C/C++. 
The main goal is to be able to use c/c++ code in python without any modification to the source and without any extra unnecessary compilation step. 
Right now it works on linux with dynamic libraries only.
This is still in development stage and filled with bugs, therefore not recommended for any production use case.

## Example Use
C source
```c
// main.h
double pi(int n);

typedef struct _Rectangle {
    int x;
    int y;
} Rectangle;

int get_area(Rectangle r);
int get_perimeter(Rectangle r);
```

```c
// main.c
#include "main.h"
#include <math.h>

int get_area(Rectangle r) {
    return r.x * r.y;
}

int get_perimeter(Rectangle r) {
    return 2 * (r.x + r.y);
}

double pi(int n) {
    long int i;
    double   sum = 0.0, term, pi;

    for (i = 0; i < n; i++) {
        term = pow(-1, i) / (2 * i + 1);
        sum += term;
    }
    pi = 4 * sum;

    return pi;
}

// compile with: gcc -shared -fPIC -o libmain.so main.c
```

Use in  Python
```python
from PyC import LoadCpp, print_CppModule

main = LoadCpp("./libmain.so", "main.h", cpp=False)
print_CppModule(main)

pi = main.pi(1000)

rect = main.Rectangle(12, 24)
print(f"rect: {rect.x = } {rect.y = }") # rect: rect.x = 12 rect.y = 24

area = main.get_area(rect)
perimeter = main.get_perimeter(rect)

print(f"{pi = }, {area = }, {perimeter = }") # pi = 3.140592653839794, area = 288, perimeter = 72
```

## Build
Install dependencies which are libclang for parsing c/c++ header files for symbols and libffi for calling c/c++ functions
```bash
sudo apt-get install -y libclang-14-dev libffi-dev
```

Clone this repository and install PyC using python
```bash
git clone https://github.com/Vipul-Cariappa/PyC.git
python setup.py install
```

## Known Bugs and stuff not implemented
- Array support (Not implemented)
- Accessing global variables of type struct (Not Implemented)
- union support (Bug)
- typedef related issues (Bug)

## Contributions
All contributions are welcomed. 
You can fork the project and create new PR, if you want to contribute. 
If the PR is related to a bud, creating a issue before creating the PR is recommended.
