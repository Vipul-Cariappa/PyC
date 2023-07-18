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
