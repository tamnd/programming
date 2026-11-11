```c run id=function-pointers
#include <stdio.h>
#include <stdlib.h>

typedef int (*binop)(int, int);

static int add(int a, int b) { return a + b; }
static int max(int a, int b) { return a > b ? a : b; }

static int fold(const int *xs, size_t n, int acc, binop f)
{
    for (size_t i = 0; i < n; i++)
        acc = f(acc, xs[i]);
    return acc;
}

static int by_value(const void *a, const void *b)
{
    int x = *(const int *)a, y = *(const int *)b;
    return (x > y) - (x < y);
}

int main(void)
{
    int xs[] = { 3, 1, 4, 1, 5 };
    size_t n = sizeof xs / sizeof xs[0];
    printf("%d %d\n", fold(xs, n, 0, add), fold(xs, n, xs[0], max));
    qsort(xs, n, sizeof xs[0], by_value);
    for (size_t i = 0; i < n; i++)
        printf("%d ", xs[i]);
    printf("\n");
    return 0;
}
```

```output for=function-pointers
14 5
1 1 3 4 5 
```

- Verdict: idiom. C has function pointers, so a function can take another function, as `qsort` does. C has no closures: state for the callback must be passed by hand, often as a `void *` argument. `const` marks data that a function promises not to change.
