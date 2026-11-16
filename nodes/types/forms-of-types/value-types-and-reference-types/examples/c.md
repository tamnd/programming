```c run id=struct-assign
#include <stdio.h>

struct point {
    int x;
    int tags[2];
};

static void bump(struct point p) { p.x++; }

static void bump_ptr(struct point *p) { p->x++; }

int main(void) {
    struct point a = {1, {10, 20}};
    struct point b = a; /* copies every member, the array too */
    b.x = 9;
    b.tags[0] = 99;
    printf("%d %d %d %d\n", a.x, a.tags[0], b.x, b.tags[0]);

    bump(a); /* the function gets a copy */
    printf("%d\n", a.x);

    bump_ptr(&a); /* a pointer shares */
    printf("%d\n", a.x);
    return 0;
}
```

```output for=struct-assign
1 10 9 99
1
2
```

- Verdict: idiom. C copies a struct on assignment and on a call, including an array member of the struct. A function that must change the caller's struct takes a pointer.
