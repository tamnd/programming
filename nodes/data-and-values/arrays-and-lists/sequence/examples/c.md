```c run id=next-function
#include <stdbool.h>
#include <stdio.h>

/* C has no sequence type. A lazy sequence is a struct that holds the state and a next function. */
typedef struct {
    unsigned long a, b;
} Fib;

static bool fib_next(Fib *f, unsigned long *out) {
    *out = f->a;
    unsigned long next = f->a + f->b;
    if (next < f->b) {
        return false; /* the next sum would wrap around, so the sequence ends */
    }
    f->a = f->b;
    f->b = next;
    return true;
}

/* An indexed sequence is an array and a length. Any function that takes both can read it. */
static int sum(const int *items, size_t len) {
    int total = 0;
    for (size_t i = 0; i < len; i++) {
        total += items[i];
    }
    return total;
}

int main(void) {
    Fib f = {0, 1};
    unsigned long x;
    for (int i = 0; i < 10 && fib_next(&f, &x); i++) {
        printf("%lu ", x);
    }
    printf("\n");

    /* The same state type can run to its natural end. */
    Fib all = {0, 1};
    int count = 0;
    while (fib_next(&all, &x)) {
        count++;
    }
    printf("%d terms fit before the sum overflows\n", count);

    int data[] = {4, 8, 15, 16, 23, 42};
    printf("%d %d\n", sum(data, 6), sum(data + 2, 3));
    return 0;
}
```

```output
0 1 1 2 3 5 8 13 21 34
92 terms fit before the sum overflows
108 54
```

- Verdict: idiom. C has no sequence type. A struct that keeps state and a next function give a sequence one item at a time, and the program stops when the next sum would overflow. A function that takes a pointer and a length works on any array of that type.
