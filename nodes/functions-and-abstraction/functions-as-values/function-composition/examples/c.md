```c run id=function-composition-c
#include <stdio.h>

typedef int fn(int);

static int inc(int x) { return x + 1; }

static int twice(int x) { return x * 2; }

/* Applies fs[n-1] first and fs[0] last, as in fs[0](fs[1](...(x))). */
static int compose_apply(fn *fs[], int n, int x) {
    for (int i = n - 1; i >= 0; i--) {
        x = fs[i](x);
    }
    return x;
}

int main(void) {
    fn *a[] = {twice, inc};
    fn *b[] = {inc, twice};
    fn *c[] = {inc, twice, inc};
    printf("%d\n", compose_apply(a, 2, 5));
    printf("%d\n", compose_apply(b, 2, 5));
    printf("%d\n", compose_apply(c, 3, 5));
    return 0;
}
```

```output for=function-composition-c
12
11
13
```

- Verdict: absent. C cannot make a new function at run time, so it cannot return the composition of two functions. The program keeps the functions in an array of function pointers and applies them in order.
