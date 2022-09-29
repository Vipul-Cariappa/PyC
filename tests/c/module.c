#include "module.h"
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const int MAX = 5000000;
const char *PROGRAM = "C/C++";
const float f = 3.14f;
int global_flag = 0;

int add_int(int x, int y) { return x + y; }

long mul_long(long x, long y) { return x * y; }

short sub_short(short x, short y) { return x - y; }

char div_char(char x, char y) { return x / y; }

unsigned int add_uint(unsigned int x, unsigned int y) { return x + y; }

unsigned long mul_ulong(unsigned long x, unsigned long y) { return x * y; }

unsigned short sub_ushort(unsigned short x, unsigned short y) {
  return abs(x - y);
}

unsigned char div_uchar(unsigned char x, unsigned char y) { return x / y; }

bool invert(bool x) {
  if (x)
    return false;
  return true;
}

unsigned long invert_bit(unsigned long x, unsigned long pos) {
  unsigned long long num = 1;
  num <<= pos;
  unsigned long long result = x ^ num;
  return result;
}

double pi(int n) {
  long int i;
  double sum = 0.0, term, pi;

  for (i = 0; i < n; i++) {
    term = pow(-1, i) / (2 * i + 1);
    sum += term;
  }
  pi = 4 * sum;

  return pi;
}

float e(int n) {
  float e = 0;

  while (--n) {
    long long term = 1;

    for (int i = 1; i < n; i++) {
      term *= i;
    }
    e += (float)1 / (float)term;
  }

  return e;
}

void set_global_flag(int f) { global_flag = f; }

int get_global_flag() { return global_flag; }

void init_bool_list_with_true(bool *li, int len) {
  for (int i = 0; i < len; i++) {
    li[i] = 1;
  }
}

long *powers_of_two_list(int size) {
  long *list = calloc(size, sizeof(long));

  while (--size) {
    long value = 1;
    for (int i = 0; i < size; i++) {
      value *= 2;
    }
    printf("%li ", value);
    list[size] = value;
  }
  printf("\n");

  return list;
}

int copy_int(int *destination, int *source) {
  *destination = *source;
  return *destination;
}

double copy_double(double *destination, double *source) {
  *destination = *source;
  return *destination;
}

char *string_concat(char *x, char *y) {
  size_t len_x = strlen(x);
  size_t len = len_x + strlen(y) + 1;
  char *result = (char *)malloc(len);
  strcpy(result, x);
  stpcpy(result + len_x, y);

  return result;
}

struct _RECT create_rect(int x, int y) {
  return (RECT){x, y};
}

RECT *rect_add_from_ptr(RECT *a, RECT *b) {
  RECT *result = (RECT *)malloc(sizeof(RECT));
  result->x = a->x + b->x;
  result->y = a->y + b->y;
  return result;
}

int get_area(struct _RECT a) { return a.x * a.y; }

int get_perimeter(RECT a) { return 2 * (a.x + a.y); }

bool same_rects(struct _RECT *a, struct _RECT *b) {
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
  union Number *n = malloc(sizeof(union Number));
  n->i = x;
  return n;
}
