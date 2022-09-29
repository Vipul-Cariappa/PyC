#include <stdbool.h>

extern const int MAX;
extern const char *PROGRAM;
extern const float f;
extern int global_flag;

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

int add_int(int x, int y);
long mul_long(long x, long y);
short sub_short(short x, short y);
char div_char(char x, char y);

unsigned int add_uint(unsigned int x, unsigned int y);
unsigned long mul_ulong(unsigned long x, unsigned long y);
unsigned short sub_ushort(unsigned short x, unsigned short y);
unsigned long invert_bit(unsigned long x, unsigned long pos);
// FIXME: unsigned char not supported
// unsigned char div_uchar(unsigned char x, unsigned char y);

bool invert(bool x);

double pi(int n);
float e(int n);

void set_global_flag(int f);
int get_global_flag();

int copy_int(int *destination, int *source);
double copy_double(double *destination, double *source);
char *string_concat(char *x, char *y);

void init_bool_list_with_true(bool *li, int len);
long *powers_of_two_list(int size);

struct _RECT create_rect(int x, int y);
RECT *rect_add_from_ptr(RECT *a, RECT *b);
int get_area(RECT a);
int get_perimeter(RECT a);
bool same_rects(struct _RECT *a, struct _RECT *b);

long get_number(union Number n);
long get_number_ptr(union Number *n);
union Number creat_number_with_int(int x);
union Number *creat_number_ptr_with_int(int x);
