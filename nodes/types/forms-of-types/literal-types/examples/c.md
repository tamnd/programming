```c run id=enum-int
#include <stdio.h>

enum align { ALIGN_LEFT, ALIGN_RIGHT, ALIGN_CENTER };

static const char *name(enum align a) {
    switch (a) {
    case ALIGN_LEFT:
        return "left";
    case ALIGN_RIGHT:
        return "right";
    case ALIGN_CENTER:
        return "center";
    }
    return "unknown";
}

int main(void) {
    enum align a = ALIGN_CENTER;
    printf("%s %d\n", name(a), a);

    enum align b = 7; /* accepted: an enum converts from int */
    printf("%s %d\n", name(b), b);

    int sum = ALIGN_RIGHT + ALIGN_CENTER; /* enum constants are ints */
    printf("%d\n", sum);
    return 0;
}
```

```output for=enum-int
center 2
unknown 7
3
```

- Verdict: idiom. C has no literal types. An `enum` gives names to integer constants, but the compiler lets any `int` go into an `enum` variable, so `name` needs a fallback after the `switch`. Enum constants take part in arithmetic like any other integer.
