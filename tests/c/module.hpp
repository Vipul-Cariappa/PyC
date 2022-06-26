extern const int MAX = 5000000;
extern const char *PROGRAM = "C/C++";
extern const float f = 3.14f;

typedef struct RECT RECT;
struct RECT {
  int x;
  int y;
};

int add(int x, int y);
char *add(char *s, int n);
int copy_int(int *destination, int *source);
char *concat(char *x, char *y);
double pi(int n);
long increment_1(long x);
short add_short(short x, short y);

// RECT *get_rect(int x, int y);
// RECT *rect_add(RECT *a, RECT *b);
// int get_area(RECT a);
// int get_periment(RECT a);
