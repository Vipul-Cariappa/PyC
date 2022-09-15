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

int product_int(int x, int y) { return x * y; }

int copy_int(int *destination, int *source) {
  *destination = *source;
  return *destination;
}

int write_int(int *destination, int value) {
  *destination = value;
  return *destination;
}

long *returns_pointer() {
  long *result = malloc(sizeof(long)); // FIXME: to free
  *result = 0;

  *result = (*result << 8) + 0;
  *result = (*result << 8) + 'l';
  *result = (*result << 8) + 'u';
  *result = (*result << 8) + 'p';
  *result = (*result << 8) + 'i';
  *result = (*result << 8) + 'v';

  return result;
}

long long add_long_long(long long x, long long y) { return x + y; }

double copy_double(double *destination, double *source) {
  *destination = *source;
  return *destination;
}

long increment_1(long x) { return ++x; }

short add_short(short x, short y) { return x + y; }

unsigned long long invert_bit(unsigned long long x, unsigned long long pos) {
  unsigned long long num = 1;
  num <<= pos;
  unsigned long long result = x ^ num;
  return result;
}

char *concat(char *x, char *y) {
  size_t len_x = strlen(x);
  size_t len = len_x + strlen(y) + 1;
  char *result = (char *)malloc(len); // FIXME: to free
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

struct _RECT get_rect(int x, int y) {
  return (RECT){x, y};
}

RECT *rect_add(RECT *a, RECT *b) {
  RECT *result = (RECT *)malloc(sizeof(RECT)); // FIXME: to free
  result->x = a->x + b->x;
  result->y = a->y + b->y;
  return result;
}

int get_area(struct _RECT a) { return a.x * a.y; }

int get_perimeter(RECT a) { return 2 * (a.x + a.y); }

_u_int8_t_ same_rects(struct _RECT *a, struct _RECT *b) {
  if ((a->x == b->x) && (a->y == b->y))
    return true;
  return false;
}

long get_number(union Number n) { return n.l; }

long get_number_ptr(union Number *n) { return n->l; }

union Number creat_number_with_int(int x) {
  union Number n;
  n.i = x;
  return n;
}

union Number *creat_number_ptr_with_int(int x) {
  union Number *n = malloc(sizeof(union Number)); // FIXME: to free
  n->i = x;
  return n;
}
