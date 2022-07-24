#include <stdbool.h>

extern const int MAX = 5000000;
extern const char *PROGRAM = "C/C++";
extern const float f = 3.14f;

struct RECT {
  int x;
  int y;
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

struct RECT get_rect(int x, int y);
struct RECT *rect_add(struct RECT *a, struct RECT *b);
int get_area(struct RECT a);
int get_perimeter(struct RECT a);
bool same_rects(struct RECT *a, struct RECT *b);
