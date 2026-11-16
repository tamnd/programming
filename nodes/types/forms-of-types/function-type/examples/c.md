```c run id=function-pointers
#include <stdio.h>
#include <stdlib.h>

typedef int (*int_op)(int);

static int inc(int n) { return n + 1; }
static int square(int n) { return n * n; }

static int twice(int (*f)(int), int x) { return f(f(x)); }

static int compare_ints(const void *a, const void *b) {
    int x = *(const int *)a;
    int y = *(const int *)b;
    return (x > y) - (x < y);
}

int main(void) {
    printf("%d %d\n", twice(inc, 5), twice(&square, 3));

    int_op ops[] = { inc, square };
    for (size_t i = 0; i < sizeof ops / sizeof ops[0]; i++) {
        printf("%d\n", ops[i](4));
    }

    /* qsort takes a pointer to a comparison function. */
    int a[] = { 3, 1, 2 };
    qsort(a, 3, sizeof a[0], compare_ints);
    printf("%d %d %d\n", a[0], a[1], a[2]);
    return 0;
}
```

```output for=function-pointers
7 81
5
16
1 2 3
```

- Verdict: native. `int (*)(int)` is a pointer to a function that takes an `int` and returns an `int`, and a `typedef` gives it a short name. A function name converts to a pointer, so `inc` and `&square` both work. An array of function pointers holds several functions. `qsort` takes a comparison function of the type `int (*)(const void *, const void *)`, so the comparison has to convert the pointers itself.
