```c run id=match-guards-c
#include <stdio.h>

static const char *describe(int n) {
    if (n == 0) {
        return "zero";
    }
    if (n < 0) {
        return "negative";
    }
    if (n < 10 && n % 2 == 0) {
        return "small even";
    }
    if (n < 10) {
        return "small odd";
    }
    return "large";
}

int main(void) {
    const int values[] = {-5, 0, 4, 7, 12};
    for (int i = 0; i < 5; i++) {
        printf("%s%s", i > 0 ? ", " : "", describe(values[i]));
    }
    printf("\n");
    return 0;
}
```

```output for=match-guards-c
negative, zero, small even, small odd, large
```

- Verdict: absent. C has no pattern matching. A `switch` case takes only a constant, so a condition such as `n < 0` cannot be a case. A chain of `if` statements tests the conditions in order.
