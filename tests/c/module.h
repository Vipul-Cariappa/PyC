#include <stdbool.h>

extern const int MAX = 5000000;
extern const char *PROGRAM = "C/C++";
extern const float f = 3.14f;

typedef unsigned char _u_int8_t_;
typedef struct _RECT RECT;

struct _RECT {
  int x;
  int y;
};

struct Cuboid {
  RECT r;
  int z;
};

struct Cuboid_p {
  RECT *r;
  long *z;
};

union Number {
  char c;
  int i;
  long l;
  float f;
  double d;
};

struct sNumber {
  union Number num;
  int x;
  int y;
};

struct sNumber_ptr {
  union Number *num;
  int *x;
  int *y;
};

union unionOfStructs {
  RECT r;
  int x;
};

union unionOfUnions {
  union Number n;
  union unionOfStructs uos;
};

union unionOfPtr {
  union Number *n;
  RECT *r;
  int *x;
};

int add(int x, int y);
bool invert(bool x);
int copy_int(int *destination, int *source);
long *returns_pointer();
int write_int(int *destination, int value);
double copy_double(double *destination, double *source);
char *concat(char *x, char *y);
double pi(int n);
long increment_1(long x);
short add_short(short x, short y);
unsigned long long invert_bit(unsigned long long x, unsigned long long pos);
int product_int(int x, int y);
long long add_long_long(long long x, long long y);

struct _RECT get_rect(int x, int y);
RECT *rect_add(RECT *a, RECT *b);
int get_area(RECT a);
int get_perimeter(RECT a);
_u_int8_t_ same_rects(struct _RECT *a, struct _RECT *b);

long get_number(union Number n);
long get_number_ptr(union Number *n);
union Number creat_number_with_int(int x);
union Number *creat_number_ptr_with_int(int x);
