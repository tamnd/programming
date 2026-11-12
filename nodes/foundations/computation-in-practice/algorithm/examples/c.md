```c run id=qsort-and-bsearch
#include <stdio.h>
#include <stdlib.h>

static int cmp_int(const void *pa, const void *pb) {
    int a = *(const int *)pa, b = *(const int *)pb;
    return (a > b) - (a < b); /* not a - b, which can overflow */
}

int main(void) {
    int xs[] = {90, 12, 58, 31, 47};
    size_t n = sizeof xs / sizeof xs[0];
    qsort(xs, n, sizeof xs[0], cmp_int);
    for (size_t i = 0; i < n; i++)
        printf(i ? " %d" : "%d", xs[i]);
    printf("\n");

    int keys[] = {58, 50};
    for (int k = 0; k < 2; k++) {
        int *p = bsearch(&keys[k], xs, n, sizeof xs[0], cmp_int);
        if (p)
            printf("search %d: found at %td\n", keys[k], p - xs);
        else
            printf("search %d: not found\n", keys[k]);
    }
    return 0;
}
```

```output for=qsort-and-bsearch
12 31 47 58 90
search 58: found at 3
search 50: not found
```

- Verdict: stdlib. `qsort` and `bsearch` from `<stdlib.h>` work on any array through `void` pointers, the size of an item and a comparison function. The comparison returns a negative, zero or positive `int`. The common shortcut `return a - b;` overflows when the values are far apart, which is undefined behavior for `int`, so the file uses `(a > b) - (a < b)`. `bsearch` returns a null pointer when the key is absent and does not give the insertion point. The C standard does not say which algorithm `qsort` uses, and it is not stable.
