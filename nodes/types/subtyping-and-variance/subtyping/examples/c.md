```c run id=first-member
#include <stdio.h>

struct shape {
    const char *name;
    double (*area)(const struct shape *self);
};

struct square {
    struct shape base;
    double side;
};

struct circle {
    struct shape base;
    double radius;
};

static double square_area(const struct shape *self) {
    const struct square *sq = (const struct square *)self;
    return sq->side * sq->side;
}

static double circle_area(const struct shape *self) {
    const struct circle *c = (const struct circle *)self;
    return 3.141592653589793 * c->radius * c->radius;
}

static void report(const struct shape *s) {
    printf("%s %.2f\n", s->name, s->area(s));
}

int main(void) {
    struct square sq = {{"square", square_area}, 2.0};
    struct circle c = {{"circle", circle_area}, 1.0};
    const struct shape *shapes[] = {&sq.base, (const struct shape *)&c};
    for (size_t i = 0; i < sizeof shapes / sizeof shapes[0]; i++) {
        report(shapes[i]);
    }
    printf("%d\n", (void *)&sq == (void *)&sq.base);
    return 0;
}
```

```output for=first-member
square 4.00
circle 3.14
1
```

- Verdict: idiom. C has no subtyping. The "base" struct is the first member of each "derived" struct.
- A pointer to a structure, converted, points to its first member, and a structure has no padding before its first member. So `&sq.base` and `&sq` have the same address.
- The function pointer in `struct shape` plays the part of a virtual table.
- The compiler does not check the cast from `struct shape *` back to `struct square *`. A cast to the wrong struct type gives undefined behaviour.
