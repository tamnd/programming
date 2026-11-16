```c run id=method-c
#include <stdio.h>

struct rectangle {
    int width;
    int height;
};

static int rectangle_area(const struct rectangle *r) {
    return r->width * r->height;
}

static struct rectangle rectangle_square(int side) {
    struct rectangle r = {side, side};
    return r;
}

int main(void) {
    struct rectangle r = {3, 4};
    struct rectangle s = rectangle_square(5);
    printf("%d\n", rectangle_area(&r));
    printf("%d\n", rectangle_area(&s));
    return 0;
}
```

```output for=method-c
12
25
```

- Verdict: idiom. C has no methods. The functions take a pointer to the struct as the first parameter, which works as the receiver.
