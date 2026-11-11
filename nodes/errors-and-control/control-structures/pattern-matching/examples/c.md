```c run id=points
#include <stdio.h>

struct point { int x, y; };

static void describe(struct point p, char *out, size_t size) {
    if (p.x == 0 && p.y == 0)
        snprintf(out, size, "origin");
    else if (p.y == 0)
        snprintf(out, size, "on the x axis at %d", p.x);
    else if (p.x == 0)
        snprintf(out, size, "on the y axis at %d", p.y);
    else if (p.x == p.y)
        snprintf(out, size, "on the diagonal at %d", p.x);
    else
        snprintf(out, size, "at %d, %d", p.x, p.y);
}

int main(void) {
    struct point points[] = {{0, 0}, {3, 0}, {0, -2}, {4, 4}, {1, 5}};
    char text[64];
    for (size_t i = 0; i < sizeof points / sizeof points[0]; i++) {
        describe(points[i], text, sizeof text);
        printf("(%d, %d): %s\n", points[i].x, points[i].y, text);
    }
    return 0;
}
```

```output for=points
(0, 0): origin
(3, 0): on the x axis at 3
(0, -2): on the y axis at -2
(4, 4): on the diagonal at 4
(1, 5): at 1, 5
```

- Verdict: absent. C has no patterns. A `switch` compares an integer with integer constants, and a chain of `if` tests does the rest.
- A `case` label falls through to the next label unless a `break` stops it.
