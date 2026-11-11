```c run id=vtable
#include <stdio.h>

struct shape;

struct shape_ops {
    double (*area)(const struct shape *s);
    const char *name;
};

struct shape {
    const struct shape_ops *ops;
};

struct circle {
    struct shape base;
    double r;
};

struct square {
    struct shape base;
    double side;
};

static double circle_area(const struct shape *s)
{
    const struct circle *c = (const struct circle *)s;
    return 3.14159 * c->r * c->r;
}

static double square_area(const struct shape *s)
{
    const struct square *q = (const struct square *)s;
    return q->side * q->side;
}

static const struct shape_ops circle_ops = { circle_area, "circle" };
static const struct shape_ops square_ops = { square_area, "square" };

int main(void)
{
    struct circle c = { { &circle_ops }, 1.0 };
    struct square q = { { &square_ops }, 2.0 };
    struct shape *all[] = { &c.base, &q.base };

    for (int i = 0; i < 2; i++)
        printf("%s %.2f\n", all[i]->ops->name, all[i]->ops->area(all[i]));
    return 0;
}
```

```output for=vtable
circle 3.14
square 4.00
```

- Verdict: idiom. C has no classes or methods, but the pattern is common. A table of function pointers acts as a vtable, and each object starts with a pointer to its table.
- Because `base` is the first member, a pointer to a `struct circle` can be converted to a pointer to its `struct shape` and back. The Linux kernel uses this pattern for files, devices and file systems.
