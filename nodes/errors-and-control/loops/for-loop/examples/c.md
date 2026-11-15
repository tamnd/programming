```c run id=half-open-bounds-and-unsigned-countdown
#include <stddef.h>
#include <stdio.h>

int main(void) {
    int a[] = {10, 20, 30, 40};
    size_t n = sizeof a / sizeof a[0];

    /* Count up with the half-open bound i < n. */
    for (size_t i = 0; i < n; i++) {
        printf("%s%d", i ? " " : "", a[i]);
    }
    printf("\n");

    /* Count down with an unsigned counter: test first, then decrement. */
    for (size_t i = n; i-- > 0;) {
        printf("%s%d", i + 1 < n ? " " : "", a[i]);
    }
    printf("\n");

    /* The comma operator steps two counters. */
    for (int lo = 0, hi = 9; lo < hi; lo += 4, hi -= 4) {
        printf("%d %d\n", lo, hi);
    }

    /* A counter declared before the loop keeps its value after it. */
    int k;
    for (k = 0; k * k <= 30; k++) {
    }
    printf("k = %d\n", k);
    return 0;
}
```

```output for=half-open-bounds-and-unsigned-countdown
10 20 30 40
40 30 20 10
0 9
4 5
k = 6
```

- Verdict: native. The loop `i < n` visits the indices 0 to n - 1. A `size_t` counter cannot go below 0, so the downward loop tests `i-- > 0`, which stops after index 0. The comma operator updates two counters in the last part. `k` is declared before the loop, so its final value 6 is still there after the loop.
