```c run id=qsort-three-way
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* The three-way idiom: (x > y) - (x < y) is -1, 0 or 1 and cannot overflow. */
static int ascending(const void *a, const void *b) {
    int x = *(const int *)a, y = *(const int *)b;
    return (x > y) - (x < y);
}

/* Swap the operands to sort in descending order. */
static int descending(const void *a, const void *b) {
    return ascending(b, a);
}

/* qsort passes pointers to the items, so for strings it passes char **. */
static int by_text(const void *a, const void *b) {
    return strcmp(*(const char *const *)a, *(const char *const *)b);
}

static void show(const int *a, size_t n) {
    for (size_t i = 0; i < n; i++) printf("%d ", a[i]);
    printf("\n");
}

int main(void) {
    int nums[] = {3, 1, 2};
    qsort(nums, 3, sizeof nums[0], ascending);
    show(nums, 3);
    qsort(nums, 3, sizeof nums[0], descending);
    show(nums, 3);

    const char *words[] = {"pear", "fig", "apple"};
    qsort(words, 3, sizeof words[0], by_text);
    printf("%s %s %s\n", words[0], words[1], words[2]);

    /* INT_MIN - 1 does not fit in an int, and signed overflow is undefined behavior,
       so a comparator must not return x - y. The three-way idiom gives -1. */
    long long wide = (long long)INT_MIN - 1;
    printf("%lld %d\n", wide, ascending(&(int){INT_MIN}, &(int){1}));
    return 0;
}
```

```output
1 2 3
3 2 1
apple fig pear
-2147483649 -1
```

- Verdict: native. `qsort` takes a pointer to a function that returns a negative, zero or positive `int`. `(x > y) - (x < y)` gives -1, 0 or 1 and cannot overflow, and the descending comparator swaps its arguments. For an array of strings, `qsort` passes pointers to the `char *` items. `INT_MIN - 1` does not fit in an `int`, so the program shows the value in a `long long`.
