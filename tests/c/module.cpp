#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "module.hpp"

int add(int x, int y)
{
    return x + y;
}

int copy_int(int *destination, int *source)
{
    *destination = *source;
    return *destination;
}

char *concat(char *x, char *y)
{
    size_t len_x = strlen(x);
    size_t len = len_x + strlen(y) + 1;
    char *result = (char *)malloc(len);
    strcpy(result, x);
    stpcpy(result + len_x, y);

    return result;
}

double pi(int n)
{
    long int i;
    double sum = 0.0, term, pi;

    /* Applying Leibniz Formula */
    for (i = 0; i < n; i++)
    {
        term = pow(-1, i) / (2 * i + 1);
        sum += term;
    }
    pi = 4 * sum;

    return pi;
}

// RECT *get_rect(int x, int y)
// {
//     RECT *result = (RECT *)malloc(sizeof(RECT));
//     result->x = x;
//     result->y = y;
//     return result;
// }

// RECT *rect_add(RECT *a, RECT *b)
// {
//     RECT *result = (RECT *)malloc(sizeof(RECT));
//     result->x = a->x + b->x;
//     result->y = a->y + b->y;
//     return result;
// }

// int get_area(RECT a)
// {
//     return a.x * a.y;
// }

// int get_periment(RECT a)
// {
//     return 2 * (a.x + a.y);
// }

// // Simple datatypes arguments and return type
// short check_short(short x) { return x; }
// int check_int(int x) { return x; }
// long check_long(long x) { return x; }
// long long check_long_long(long long x) { return x; }
// float check_float(float x) { return x; }
// double check_double(double x) { return x; }
// unsigned short check_unsigned_short(unsigned short x) { return x; }
// unsigned int check_unsigned_int(unsigned int x) { return x; }
// unsigned long check_unsigned_long(unsigned long x) { return x; }
// unsigned long long check_unsigned_long_long(unsigned long long x) { return x; }
// char check_char(char x) { return x; }
// void check_void_1() { return; }
// void check_void_2(void) { return; }

// short check_const_short(const short x) { return 3; }
// int check_const_int(const int x) { return 3; }
// long check_const_long(const long x) { return 3; }
// long check_const_long_long(const long long x) { return 3; }
// float check_const_float(const float x) { return 3.14; }
// double check_const_double(const double x) { return 3.14; }
// short check_const_unsigned_short(const unsigned short x) { return 3; }
// int check_const_unsigned_int(const unsigned int x) { return 3; }
// long check_const_unsigned_long(const unsigned long x) { return 3; }
// long check_const_unsigned_long_long(const unsigned long long x) { return 3; }
// char check_const_char(const char x) { return '!'; }

// // Simple pointer datatypes arguments and return type
// short *check_short_ptr(short *x)
// {
//     *x = *x + 1;
//     return x;
// }
// int *check_int_ptr(int *x)
// {
//     *x = *x + 1;
//     return x;
// }
// long *check_long_ptr(long *x)
// {
//     *x = *x + 1;
//     return x;
// }
// long long *check_long_long_ptr(long long *x)
// {
//     *x = *x + 1;
//     return x;
// }
// float *check_float_ptr(float *x)
// {
//     *x = *x + 1;
//     return x;
// }
// double *check_double_ptr(double *x)
// {
//     *x = *x + 1;
//     return x;
// }
// unsigned short *check_unsigned_short_ptr(unsigned short *x)
// {
//     *x = *x + 1;
//     return x;
// }
// unsigned int *check_unsigned_int_ptr(unsigned int *x)
// {
//     *x = *x + 1;
//     return x;
// }
// unsigned long *check_unsigned_long_ptr(unsigned long *x)
// {
//     *x = *x + 1;
//     return x;
// }
// unsigned long long *check_unsigned_long_long_ptr(unsigned long long *x)
// {
//     *x = *x + 1;
//     return x;
// }
// char *check_char_ptr(char *x)
// {
//     char *r = (char *)malloc(sizeof(char) * 2);
//     r = (char*)"!";
//     return r;
// }
// void *check_void_ptr(void *x) { return x; }

// short *check_const_short_ptr(const short *x)
// {
//     short *r = (short *)malloc(sizeof(short));
//     *r = *x - 1;
//     return r;
// }
// int *check_const_int_ptr(const int *x)
// {
//     int *r = (int *)malloc(sizeof(int));
//     *r = *x - 1;
//     return r;
// }
// long *check_const_long_ptr(const long *x)
// {
//     long *r = (long *)malloc(sizeof(long));
//     *r = *x - 1;
//     return r;
// }
// long long *check_const_long_long_ptr(const long long *x)
// {
//     long long *r = (long long *)malloc(sizeof(long long));
//     *r = *x - 1;
//     return r;
// }
// float *check_const_float_ptr(const float *x)
// {
//     float *r = (float *)malloc(sizeof(float));
//     *r = *x - 1;
//     return r;
// }
// double *check_const_double_ptr(const double *x)
// {
//     double *r = (double *)malloc(sizeof(double));
//     *r = *x - 1;
//     return r;
// }
// unsigned short *check_const_unsigned_short_ptr(const unsigned short *x)
// {
//     unsigned short *r = (unsigned short *)malloc(sizeof(short));
//     *r = *x - 1;
//     return r;
// }
// unsigned int *check_const_unsigned_int_ptr(const unsigned int *x)
// {
//     unsigned int *r = (unsigned int *)malloc(sizeof(int));
//     *r = *x - 1;
//     return r;
// }
// unsigned long *check_const_unsigned_long_ptr(const unsigned long *x)
// {
//     unsigned long *r = (unsigned long *)malloc(sizeof(long));
//     *r = *x - 1;
//     return r;
// }
// unsigned long long *check_const_unsigned_long_long_ptr(const unsigned long long *x)
// {
//     unsigned long long *r = (unsigned long long *)malloc(sizeof(long long));
//     *r = *x - 1;
//     return r;
// }
// char *check_const_char_ptr(const char *x)
// {
//     char *r = (char *)malloc(sizeof(char) * 2);
//     *r = *x - 1;
//     *(r + 1) = 0;
//     return r;
// }
// void *check_const_void_ptr(const void *x)
// {
//     int *r = (int *)malloc(sizeof(int));
//     *r = 3;
//     x = r;
//     return r;
// }

// short *check_short_ptr_const(short *const x)
// {
//     *x = -3;
//     return x;
// }
// int *check_int_ptr_const(int *const x)
// {
//     *x = -3;
//     return x;
// }
// long *check_long_ptr_const(long *const x)
// {
//     *x = -3;
//     return x;
// }
// long long *check_long_long_ptr_const(long long *const x)
// {
//     *x = -3;
//     return x;
// }
// float *check_float_ptr_const(float *const x)
// {
//     *x = -3.14;
//     return x;
// }
// double *check_double_ptr_const(double *const x)
// {
//     *x = -3.14;
//     return x;
// }
// unsigned short *check_unsigned_short_ptr_const(unsigned short *const x)
// {
//     *x = 3;
//     return x;
// }
// unsigned int *check_unsigned_int_ptr_const(unsigned int *const x)
// {
//     *x = 3;
//     return x;
// }
// unsigned long *check_unsigned_long_ptr_const(unsigned long *const x)
// {
//     *x = 3;
//     return x;
// }
// unsigned long long *check_unsigned_long_long_ptr_const(unsigned long long *const x)
// {
//     *x = 3;
//     return x;
// }
// char *check_char_ptr_const(char *const x)
// { /* *x = '!'; */
//     return x;
// }
// void *check_void_ptr_const(void *const x) { return x; }

// short check_const_short_ptr_const(const short *const x) { return *x; }
// int check_const_int_ptr_const(const int *const x) { return *x; }
// long check_const_long_ptr_const(const long *const x) { return *x; }
// long long check_const_long_long_ptr_const(const long long *const x) { return *x; }
// float check_const_float_ptr_const(const float *const x) { return *x; }
// double check_const_double_ptr_const(const double *const x) { return *x; }
// unsigned short check_const_unsigned_short_ptr_const(const unsigned short *const x) { return *x; }
// unsigned int check_const_unsigned_int_ptr_const(const unsigned int *const x) { return *x; }
// unsigned long check_const_unsigned_long_ptr_const(const unsigned long *const x) { return *x; }
// unsigned long long check_const_unsigned_long_long_ptr_const(const unsigned long long *const x) { return *x; }
// char check_const_char_ptr_const(const char *const x) { return *x; }
// void check_const_void_ptr_const(const void *const x) { return; }

// compile with: g++ -Wall -shared -fPIC -o libcmodule.so module.c
// copy to: /usr/lib/
