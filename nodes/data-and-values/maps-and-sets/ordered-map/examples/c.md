```c run id=sorted-array
#include <stdio.h>
#include <stdlib.h>

/* C has no ordered map. A sorted array of pairs with binary search is a simple one. */
typedef struct {
    int key;
    const char *value;
} pair;

static int cmp(const void *a, const void *b) {
    int x = ((const pair *)a)->key, y = ((const pair *)b)->key;
    return (x > y) - (x < y); /* not x - y, which can overflow */
}

/* The index of the first pair with key >= k, or n if there is none. */
static size_t lower_bound(const pair *p, size_t n, int k) {
    size_t lo = 0, hi = n;
    while (lo < hi) {
        size_t mid = lo + (hi - lo) / 2;
        if (p[mid].key < k) lo = mid + 1;
        else hi = mid;
    }
    return lo;
}

int main(void) {
    pair m[] = {{30, "c"}, {10, "a"}, {50, "e"}, {20, "b"}, {40, "d"}};
    size_t n = sizeof m / sizeof m[0];
    qsort(m, n, sizeof m[0], cmp);
    for (size_t i = 0; i < n; i++) printf("%d=%s%s", m[i].key, m[i].value, i + 1 < n ? " " : "\n");

    /* bsearch finds an exact key. */
    pair want = {40, NULL};
    pair *hit = bsearch(&want, m, n, sizeof m[0], cmp);
    printf("%s\n", hit ? hit->value : "missing");

    /* Floor and ceiling of 35. */
    size_t i = lower_bound(m, n, 35);
    printf("%d %d\n", m[i - 1].key, m[i].key);

    /* A range: the keys from 20 up to but not including 45. */
    for (size_t j = lower_bound(m, n, 20); j < lower_bound(m, n, 45); j++) printf("%d ", m[j].key);
    printf("| no ceiling for 99: %d\n", lower_bound(m, n, 99) == n);
    return 0;
}
```

```output
10=a 20=b 30=c 40=d 50=e
d
30 40
20 30 40 | no ceiling for 99: 1
```

- Verdict: idiom. C has no ordered map. A sorted array of pairs with qsort and bsearch, and a hand-written lower bound, gives exact lookup, floor, ceiling and ranges. Inserts are linear. POSIX tsearch gives a binary tree.
