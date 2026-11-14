```c run id=sorted-pairs
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* A multimap as a sorted array of pairs. Equal keys sit next to each other,
   so all values of a key are one run that a binary search finds. */
struct pair {
    const char *key;
    int val;
    int seq; /* insertion number, to keep equal keys in insertion order */
};

static int cmp(const void *a, const void *b) {
    const struct pair *x = a, *y = b;
    int c = strcmp(x->key, y->key);
    return c != 0 ? c : x->seq - y->seq;
}

/* The first index whose key is >= k. */
static size_t lower(const struct pair *p, size_t n, const char *k) {
    size_t lo = 0, hi = n;
    while (lo < hi) {
        size_t mid = lo + (hi - lo) / 2;
        if (strcmp(p[mid].key, k) < 0) lo = mid + 1; else hi = mid;
    }
    return lo;
}

static void show(const struct pair *p, size_t n, const char *k) {
    size_t i = lower(p, n, k);
    printf("%s:", k);
    for (; i < n && strcmp(p[i].key, k) == 0; i++) printf(" %d", p[i].val);
    printf("\n");
}

int main(void) {
    struct pair p[] = {
        {"yellow", 1, 0}, {"blue", 2, 1}, {"yellow", 3, 2}, {"blue", 4, 3}, {"red", 1, 4},
    };
    size_t n = sizeof p / sizeof p[0];
    qsort(p, n, sizeof p[0], cmp);
    show(p, n, "blue");
    show(p, n, "yellow");
    show(p, n, "green");

    /* Count the distinct keys: a key starts a run when it differs from the one before. */
    size_t keys = 0;
    for (size_t i = 0; i < n; i++)
        if (i == 0 || strcmp(p[i].key, p[i - 1].key) != 0) keys++;
    printf("%zu keys, %zu pairs\n", keys, n);
    return 0;
}
```

```output
blue: 2 4
yellow: 1 3
green:
3 keys, 5 pairs
```

- Verdict: idiom. C has no maps. The example sorts an array of pairs by key, with an insertion number to keep equal keys in order. A binary search finds the first pair of a key, and the values of the key are the run of pairs from there.
