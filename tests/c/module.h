#include <stdbool.h>

extern const int MAX = 5000000;
extern const char *PROGRAM = "C/C++";
extern const float f = 3.14f;

typedef struct RECT {
  int x;
  int y;
} RECT;

int add(int x, int y);
bool invert(bool x);
int copy_int(int *destination, int *source);
int write_int(int *destination, int value);
double copy_double(double *destination, double *source);
char *concat(char *x, char *y);
double pi(int n);
long increment_1(long x);
short add_short(short x, short y);
unsigned long long invert_bit(unsigned long long x, unsigned long long pos);

// RECT *get_rect(int x, int y);
// RECT *rect_add(RECT *a, RECT *b);
// int get_area(RECT a);
// int get_periment(RECT a);
