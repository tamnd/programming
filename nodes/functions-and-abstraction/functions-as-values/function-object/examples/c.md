```c run id=function-object-c
#include <stdio.h>

struct multiplier {
    int factor;
    int calls;
};

static int multiply(void *ctx, int x) {
    struct multiplier *m = ctx;
    m->calls++;
    return x * m->factor;
}

static void map_ints(int (*f)(void *, int), void *ctx, const int *xs, int *out, int n) {
    for (int i = 0; i < n; i++) {
        out[i] = f(ctx, xs[i]);
    }
}

int main(void) {
    struct multiplier triple = {3, 0};
    printf("%d\n", multiply(&triple, 5));

    int xs[] = {1, 2, 3};
    int out[3];
    map_ints(multiply, &triple, xs, out, 3);
    printf("%d %d %d\n", out[0], out[1], out[2]);
    printf("%d\n", triple.calls);
    return 0;
}
```

```output for=function-object-c
15
3 6 9
4
```

- Verdict: idiom. C has no function objects. The struct holds the data, and the program passes a function pointer and a pointer to the struct together. `map_ints` gives the struct to each call.
