#include <string>
#include <cmath>
#include "module.hpp"

int add(int x, int y)
{
    return x + y;
}

long increment_1(long x)
{
    return ++x;
}

short add_short_1(short x, short y)
{
    return x + y;
}

char *add(char *s, int n)
{
    std::string *result = new std::string("");
    std::string str = std::string(s);

    for (int i = 0; i < n; i++)
    {
        *result += str;
    }

    return (char*)result->c_str();
}

int copy_int(int *destination, int *source)
{
    *destination = *source;
    return *destination;
}

char *concat(char* x, char* y)
{
    std::string a = std::string(x);
    std::string b = std::string(y);
    std::string *result = new std::string(a + b);
    return (char*)result->c_str();
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
