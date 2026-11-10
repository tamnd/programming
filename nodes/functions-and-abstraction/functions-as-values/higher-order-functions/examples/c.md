```c run id=hof
#include <stdio.h>
#include <stdlib.h>

static int triple(int n) { return n * 3; }
static int square(int n) { return n * n; }

static int twice(int (*f)(int), int x) { return f(f(x)); }

static void map_ints(int *items, size_t n, int (*f)(int)) {
    for (size_t i = 0; i < n; i++) {
        items[i] = f(items[i]);
    }
}

static int compare_ints(const void *a, const void *b) {
    int x = *(const int *)a;
    int y = *(const int *)b;
    return (x > y) - (x < y);
}

int main(void) {
    printf("%d\n", twice(triple, 2));
    int items[] = {5, -2, 9, 1};
    size_t n = sizeof items / sizeof items[0];
    map_ints(items, n, square);
    qsort(items, n, sizeof items[0], compare_ints);
    for (size_t i = 0; i < n; i++) {
        printf("%d ", items[i]);
    }
    printf("\n");
    return 0;
}
```

```output for=hof
18
1 4 25 81
```

- Verdict: native, in a limited form. A pointer to a function can be a parameter, a variable or a return value. A function name in an expression converts to such a pointer.
- C has no lambda expressions and no closures. A callback that needs state takes an extra `void *` argument that points to the state.
- `qsort` calls the comparison function through the pointer for each comparison. The comparator returns a negative, zero or positive `int` (see Pitfalls).
