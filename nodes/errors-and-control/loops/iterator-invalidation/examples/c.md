```c run id=index-not-pointer-across-realloc
#include <stdio.h>
#include <stdlib.h>

int main(void) {
    size_t len = 3, cap = 3;
    int *a = malloc(cap * sizeof *a);
    if (a == NULL)
        return 1;
    for (size_t i = 0; i < len; i++)
        a[i] = (int)(i + 1);

    /* Keep an index, not a pointer: realloc can move the buffer. */
    size_t n = len;
    for (size_t i = 0; i < n; i++) {
        if (len == cap) {
            cap *= 2;
            int *grown = realloc(a, cap * sizeof *a);
            if (grown == NULL) {
                free(a);
                return 1;
            }
            a = grown;
        }
        a[len++] = a[i] * 10;
    }
    for (size_t i = 0; i < len; i++)
        printf("%d%s", a[i], i + 1 < len ? " " : "\n");

    /* Remove the even numbers in place with a read and a write index. */
    size_t w = 0;
    for (size_t r = 0; r < len; r++)
        if (a[r] % 2 != 0)
            a[w++] = a[r];
    len = w;
    printf("odd: %zu left, first %d\n", len, a[0]);
    free(a);
    return 0;
}
```

```output for=index-not-pointer-across-realloc
1 2 3 10 20 30
odd: 2 left, first 1
```

- Verdict: absent. C has no iterators, but a pointer into a buffer has the same problem: after `realloc` moves the buffer, an old pointer points to freed memory. The loop keeps the index `i` and reads `a[i]` through the new pointer after each growth, and it saves the length `n` before the loop, so the appended elements do not add passes. The removal uses two indexes, one to read and one to write, in a single pass.
