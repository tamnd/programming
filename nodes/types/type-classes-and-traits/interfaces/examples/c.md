```c run id=ops-table
#include <stdio.h>

struct shape_ops {
    const char *name;
    double (*area)(const void *self);
};

struct shape {
    const struct shape_ops *ops;
    const void *self;
};

struct square {
    double side;
};

struct circle {
    double radius;
};

static double square_area(const void *self) {
    const struct square *s = self;
    return s->side * s->side;
}

static double circle_area(const void *self) {
    const struct circle *c = self;
    return 3.141592653589793 * c->radius * c->radius;
}

static const struct shape_ops square_ops = {"square", square_area};
static const struct shape_ops circle_ops = {"circle", circle_area};

static double total(const struct shape *shapes, size_t n) {
    double sum = 0;
    for (size_t i = 0; i < n; i++) {
        sum += shapes[i].ops->area(shapes[i].self);
    }
    return sum;
}

int main(void) {
    struct square sq = {2.0};
    struct circle c = {1.0};
    struct shape shapes[] = {{&square_ops, &sq}, {&circle_ops, &c}};
    for (size_t i = 0; i < 2; i++) {
        printf("%s %.2f\n", shapes[i].ops->name, shapes[i].ops->area(shapes[i].self));
    }
    printf("%.2f\n", total(shapes, 2));
    return 0;
}
```

```output
square 4.00
circle 3.14
7.14
```

- Verdict: idiom. C has no interfaces. A struct of function pointers is the interface, and one constant instance of it for each type is the implementation.
- `struct shape` is a pair of an operations pointer and a data pointer. It has the same shape as a Go interface value or a Rust `&dyn Trait`.
- The compiler does not check that `self` points to the type that the operations expect. A wrong pair gives undefined behaviour, so code builds each pair in one place.
