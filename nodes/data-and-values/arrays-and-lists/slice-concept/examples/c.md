```c run id=pointer-and-length
#include <stddef.h>
#include <stdio.h>

/* C has no slice type. A slice is a pointer to the first item and a count. */
typedef struct {
    int *data;
    size_t len;
} IntSlice;

/* Return the items [lo, hi) of s, or an empty slice if the bounds are wrong. */
static IntSlice slice(IntSlice s, size_t lo, size_t hi) {
    if (lo > hi || hi > s.len) {
        return (IntSlice){NULL, 0};
    }
    return (IntSlice){s.data + lo, hi - lo};
}

static void print(const char *name, IntSlice s) {
    printf("%s:", name);
    for (size_t i = 0; i < s.len; i++) {
        printf(" %d", s.data[i]);
    }
    printf(" (len %zu)\n", s.len);
}

int main(void) {
    int arr[] = {10, 20, 30, 40, 50};
    IntSlice all = {arr, sizeof arr / sizeof arr[0]};
    IntSlice mid = slice(all, 1, 4);
    print("mid", mid);

    /* The slice points into arr, so a write through it changes arr. */
    mid.data[0] = 99;
    print("all", all);

    /* A slice of a slice uses offsets from its own start. */
    IntSlice inner = slice(mid, 1, 3);
    print("inner", inner);
    printf("%td\n", inner.data - arr);

    /* The bounds check is the program's job. This range is refused. */
    IntSlice bad = slice(all, 3, 9);
    printf("%d\n", bad.data == NULL);
    return 0;
}
```

```output
mid: 20 30 40 (len 3)
all: 10 99 30 40 50 (len 5)
inner: 30 40 (len 2)
2
1
```

- Verdict: idiom. C has no slice type. A struct with a pointer and a count works as a slice, and a write through it changes the array. A slice of a slice is a pointer further into the same array. The bounds check is the program's own code.
