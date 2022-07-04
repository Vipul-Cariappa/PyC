#include "module.h"
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int add(int x, int y) { return x + y; }

bool invert(bool x) {
  if (x)
    return false;
  return true;
}

int product_int(int x, int y)
{
    return x * y;
}

int copy_int(int *destination, int *source) {
  *destination = *source;
  return *destination;
}

int write_int(int *destination, int value) {
  *destination = value;
  return *destination;
}

long long add_long_long(long long x, long long y)
{
    return x + y;
}

double copy_double(double *destination, double *source) {
  *destination = *source;
  return *destination;
}

long increment_1(long x) { return ++x; }

short add_short(short x, short y) { return x + y; }

char *concat(char *x, char *y) {
  size_t len_x = strlen(x);
  size_t len = len_x + strlen(y) + 1;
  char *result = (char *)malloc(len);
  strcpy(result, x);
  stpcpy(result + len_x, y);

  return result;
}

double pi(int n) {
  long int i;
  double sum = 0.0, term, pi;

  /* Applying Leibniz Formula */
  for (i = 0; i < n; i++) {
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
