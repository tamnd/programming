```c run id=int-comparisons-and-bool
#include <stdio.h>

int main(void) {
    /* In C23, bool, true and false are keywords. */
    bool flag = true;
    printf("%d %zu\n", flag, sizeof(bool));

    /* A comparison gives an int with the value 0 or 1. */
    int x = 5;
    printf("%d %d %zu\n", x > 3, x == 3, sizeof(x > 3));

    /* A condition treats any non-zero value as true. */
    if (x) {
        printf("non-zero is true\n");
    }

    /* Conversion to bool tests for zero, but conversion to int truncates. */
    double half = 0.5;
    int big = 256;
    bool b1 = half;
    int i1 = half;
    bool b2 = big;
    unsigned char c2 = big;
    printf("%d %d %d %d\n", b1, i1, b2, c2);

    /* !! turns any value into 0 or 1. */
    printf("%d %d\n", !!42, !!0);
    return 0;
}
```

```output for=int-comparisons-and-bool
1 1
1 0 4
non-zero is true
1 0 1 0
1 0
```

- Verdict: native. In C23, `bool`, `true` and `false` are keywords. A comparison has type `int`, so `sizeof(x > 3)` is the size of an `int`. A condition treats any non-zero value as true. Conversion to `bool` tests for zero, so `0.5` and `256` become `1`, but conversion to `int` gives `0` and to `unsigned char` gives `0`. `!!` turns any value into `0` or `1`.
