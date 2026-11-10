```c run id=shapes
#include <stdio.h>

enum kind { CIRCLE, RECT, TRIANGLE };

struct shape {
    enum kind kind;
    union {
        struct { double radius; } circle;
        struct { double width, height; } rect;
        struct { double base, height; } triangle;
    } as;
};

static double area(const struct shape *s) {
    switch (s->kind) {
    case CIRCLE:
        return 3.141592653589793 * s->as.circle.radius * s->as.circle.radius;
    case RECT:
        return s->as.rect.width * s->as.rect.height;
    case TRIANGLE:
        return s->as.triangle.base * s->as.triangle.height / 2;
    }
    return 0.0;
}

int main(void) {
    struct shape shapes[] = {
        {.kind = CIRCLE, .as.circle = {1}},
        {.kind = RECT, .as.rect = {2, 3}},
        {.kind = TRIANGLE, .as.triangle = {4, 5}},
    };
    for (size_t i = 0; i < sizeof shapes / sizeof shapes[0]; i++) {
        printf("%.2f\n", area(&shapes[i]));
    }
    printf("%zu %zu\n", sizeof(struct shape), sizeof shapes[0].as);
}
```

```output for=shapes
3.14
6.00
10.00
24 16
```

- Verdict: idiom. A struct holds an `enum` tag and a `union` of the payloads, and a `switch` on the tag selects the member to read.
- The size of a union is the size of its largest member, because the members share their storage.
- The compiler does not tie the tag to the member. Code that sets one tag and writes another member compiles (see Pitfalls on the concept page).
- With `-Wswitch`, which `-Wall` turns on, GCC and Clang warn when a `switch` on an `enum` has no `default` and misses an enumerator.
