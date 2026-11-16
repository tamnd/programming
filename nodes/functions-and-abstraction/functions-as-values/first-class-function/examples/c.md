```c run id=first-class-function-c
#include <stdio.h>

static int twice(int x) { return x * 2; }

static int square(int x) { return x * x; }

static int apply_twice(int (*g)(int), int x) { return g(g(x)); }

struct adder {
    int n;
};

static int adder_call(const struct adder *a, int x) { return x + a->n; }

int main(void) {
    int (*f)(int) = twice;
    printf("%d\n", f(5));

    int (*ops[])(int) = {twice, square};
    printf("%d\n", ops[1](4));

    printf("%d\n", apply_twice(twice, 3));

    struct adder add5 = {5};
    printf("%d\n", adder_call(&add5, 10));
    return 0;
}
```

```output for=first-class-function-c
10
16
12
15
```

- Verdict: idiom. A pointer to a function can be stored, put in an array and passed. C cannot make a new function at run time, so the adder is a struct with the value `n` and a function that takes the struct.
