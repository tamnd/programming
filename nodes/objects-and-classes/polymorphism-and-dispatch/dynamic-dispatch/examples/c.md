```c run id=shapes
#include <stdio.h>

struct shape;

struct shape_vtable {                       /* one table for each class */
    const char *(*name)(const struct shape *);
    double (*area)(const struct shape *);
};

struct shape { const struct shape_vtable *vt; };   /* every object starts with it */
struct circle { struct shape base; double r; };
struct square { struct shape base; double side; };

static const char *shape_name(const struct shape *s) { (void)s; return "shape"; }
static const char *circle_name(const struct shape *s) { (void)s; return "circle"; }

static double circle_area(const struct shape *s)
{
    const struct circle *c = (const struct circle *)s;
    return 3.141592653589793 * c->r * c->r;
}

static double square_area(const struct shape *s)
{
    const struct square *q = (const struct square *)s;
    return q->side * q->side;
}

static const struct shape_vtable circle_vt = { circle_name, circle_area };
static const struct shape_vtable square_vt = { shape_name, square_area };  /* keeps the default name */

static void describe(const struct shape *s)     /* dispatch: load the table, call the slot */
{
    printf("%s with area %.2f\n", s->vt->name(s), s->vt->area(s));
}

int main(void)
{
    struct circle c = { { &circle_vt }, 1.0 };
    struct square q = { { &square_vt }, 2.0 };
    const struct shape *shapes[] = { &c.base, &q.base };
    for (int i = 0; i < 2; i++)
        describe(shapes[i]);
    return 0;
}
```

```output for=shapes
circle with area 3.14
shape with area 4.00
```

- Verdict: idiom. C has no methods, so programs build dynamic dispatch by hand: a struct of function pointers for each class, and a pointer to it at the start of each object. The cast from `struct shape *` to `struct circle *` is valid because `base` is the first member. This is how C++ compilers lay out single inheritance, and C code bases such as the Linux kernel, with its `file_operations` table, and GLib's GObject use the same pattern.
- Nothing checks the casts or the tables. A wrong table in an object, or a function that casts to the wrong struct, compiles and reads the wrong memory.
