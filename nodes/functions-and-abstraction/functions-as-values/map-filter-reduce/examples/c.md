```c run id=map-filter-reduce-c
#include <stdio.h>

static int is_even(int x) { return x % 2 == 0; }

static int square(int x) { return x * x; }

static int add(int a, int b) { return a + b; }

static size_t filter(const int *in, size_t n, int (*keep)(int), int *out) {
    size_t count = 0;
    for (size_t i = 0; i < n; i++) {
        if (keep(in[i])) {
            out[count++] = in[i];
        }
    }
    return count;
}

static void map(int *xs, size_t n, int (*f)(int)) {
    for (size_t i = 0; i < n; i++) {
        xs[i] = f(xs[i]);
    }
}

static int reduce(const int *xs, size_t n, int init, int (*f)(int, int)) {
    int acc = init;
    for (size_t i = 0; i < n; i++) {
        acc = f(acc, xs[i]);
    }
    return acc;
}

int main(void) {
    int nums[] = {1, 2, 3, 4, 5, 6};
    int buf[6];
    size_t n = filter(nums, 6, is_even, buf);
    map(buf, n, square);
    for (size_t i = 0; i < n; i++) {
        printf("%s%d", i ? " " : "", buf[i]);
    }
    printf("\n%d\n", reduce(buf, n, 0, add));
    return 0;
}
```

```output for=map-filter-reduce-c
4 16 36
56
```

- Verdict: idiom. C has no map, filter or reduce. Helper functions take a function pointer and loop over an array. The caller gives the memory for the result.
