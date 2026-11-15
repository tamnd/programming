```c run id=parts
#include <stdbool.h>
#include <stdio.h>

struct point {
    int x;
    int y;
};

static const char *yn(bool b) { return b ? "yes" : "no"; }

/* C has no == for structures, so compare the members. */
static bool point_equal(struct point a, struct point b) {
    return a.x == b.x && a.y == b.y;
}

int main(void) {
    struct point p = { 3, 4 };           /* members by position */
    struct point q = { .y = 4, .x = 3 }; /* members by name */
    printf("struct: x = %d y = %d\n", p.x, p.y);
    printf("named initializer: x = %d y = %d\n", q.x, q.y);
    printf("same parts equal: %s\n", yn(point_equal(p, q)));
    printf("swapped parts equal: %s\n", yn(point_equal(p, (struct point){ 4, 3 })));
    printf("size: %zu = %zu + %zu\n", sizeof p, sizeof p.x, sizeof p.y);
    return 0;
}
```

```output for=parts
struct: x = 3 y = 4
named initializer: x = 3 y = 4
same parts equal: yes
swapped parts equal: no
size: 8 = 4 + 4
```

- Verdict: native. A `struct` is a product by name. An initializer can give the members by position or, with designators, by name.
- `p == q` does not compile for structures. A function compares the members.
- Here the struct has no padding, so its size is the sum of the sizes of its members. ISO C has no empty struct, so it has no unit type.
