```c run id=nonzero-conditions-and-if-directive
#include <stdio.h>

#define VERBOSE 1

static int next_value(int *i) {
    static const int values[] = {4, 2, 0};
    return values[(*i)++];
}

int main(void) {
    /* A condition is true when it is not zero. */
    int count = 3;
    if (count) {
        printf("count is non-zero\n");
    }

    /* An else belongs to the nearest if that has no else. */
    int a = 5, b = -1;
    const char *r = "none";
    if (a > 0) {
        if (b > 0) {
            r = "both positive";
        } else {
            r = "a positive, b not";
        }
    }
    printf("%s\n", r);

    /* Assignment in a condition: the extra parentheses say it is on purpose. */
    int i = 0, v;
    while ((v = next_value(&i))) {
        printf("got %d\n", v);
    }

    /* The conditional operator gives a value. */
    int x = 3, y = 9;
    printf("max %d\n", x > y ? x : y);

    /* #if chooses code at compile time, before the program runs. */
#if VERBOSE
    printf("verbose build\n");
#else
    printf("quiet build\n");
#endif
    return 0;
}
```

```output for=nonzero-conditions-and-if-directive
count is non-zero
a positive, b not
got 4
got 2
max 9
verbose build
```

- Verdict: native. Any non-zero value is true in a condition. Braces make the inner `if` own the `else`. The extra parentheses around `v = next_value(&i)` mark an assignment in a condition, and the loop stops when the value is `0`. `x > y ? x : y` chooses a value, and `#if` chooses code at compile time.
