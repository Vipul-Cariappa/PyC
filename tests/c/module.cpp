#include "module.hpp"
#include <cmath>
#include <string.h>
#include <string>

char *returns_pointer_c() {
  char *r = (char *)malloc(sizeof(char) * 2);
  strcpy(r, "c");
  return r;
}

short *returns_pointer_s() {
  short *r = (short *)malloc(sizeof(short));
  *r       = 420;
  return r;
}

int *returns_pointer_i() {
  int *r = (int *)malloc(sizeof(int));
  *r     = 360;
  return r;
}

long *returns_pointer_l() {
  long *r = (long *)malloc(sizeof(long));
  *r      = 360;
  return r;
}

int add(int x, int y) {
  return x + y;
}

int product_int(int x, int y) {
  return x * y;
}

long increment_1(long x) {
  return ++x;
}

short add_short(short x, short y) {
  return x + y;
}

char *add(char *s, int n) {
  std::string result = std::string("");
  std::string str    = std::string(s);

  for (int i = 0; i < n; i++) {
    result += str;
  }

  return strdup(result.c_str());
}

// long long add_long_long(long long x, long long y) {
//   return x + y;
// }

char *repeat_char(char s, int n) {
  std::string result = std::string("");
  char x[2];
  x[0] = s;
  x[1] = 0;

  std::string str = std::string(x);

  for (int i = 0; i < n; i++) {
    result += str;
  }

  return strdup(result.c_str());
}

int copy_int(int *destination, int *source) {
  *destination = *source;
  return *destination;
}

char *concat(char *x, char *y) {
  return strdup((std::string(x) + std::string(y)).c_str());
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

RECT *get_rect(int x, int y) {
  RECT *result = (RECT *)malloc(sizeof(RECT));
  result->x    = x;
  result->y    = y;
  return result;
}

RECT *rect_add(RECT *a, RECT *b) {
  RECT *result = (RECT *)malloc(sizeof(RECT));
  result->x    = a->x + b->x;
  result->y    = a->y + b->y;
  return result;
}

int get_area(RECT a) {
  return a.x * a.y;
}

int get_periment(RECT a) {
  return 2 * (a.x + a.y);
}
