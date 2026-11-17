```c run id=generic-fnptr
#include <stdio.h>

// _Generic: the compiler picks an expression by the type of the argument.
#define show(x) _Generic((x), int: show_int, double: show_double, const char *: show_str, char *: show_str)(x)

static void show_int(int x) { printf("int %d\n", x); }
static void show_double(double x) { printf("double %.1f\n", x); }
static void show_str(const char *x) { printf("string %s\n", x); }

// A table of function pointers: the object carries its own operations.
struct shape;
struct shape_ops {
    double (*area)(const struct shape *);
    const char *name;
};
struct shape {
    const struct shape_ops *ops;
    double size;
};

static double square_area(const struct shape *s) { return s->size * s->size; }
static double circle_area(const struct shape *s) { return 3.14159 * s->size * s->size; }

static const struct shape_ops square_ops = {square_area, "square"};
static const struct shape_ops circle_ops = {circle_area, "circle"};

int main(void) {
    show(1);
    show(2.5);
    show("a");

    struct shape shapes[] = {{&square_ops, 2}, {&circle_ops, 1}};
    for (int i = 0; i < 2; i++) {
        printf("%s %.2f\n", shapes[i].ops->name, shapes[i].ops->area(&shapes[i]));
    }
    return 0;
}
```

```output for=generic-fnptr
int 1
double 2.5
string a
square 4.00
circle 3.14
```

- Verdict: idiom. C has no overloading and no virtual functions. The `show` macro uses `_Generic`, which picks a function by the type of the argument when the program compiles. Each `struct shape` points to a table of operations, so the call `ops->area` runs the function of the object, as a virtual call does in C++.
