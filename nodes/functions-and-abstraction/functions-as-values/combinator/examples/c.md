```c run id=combinator-c
#include <stdio.h>

typedef int (*binop)(int, int);
typedef int (*pred)(int);

static int identity(int x) { return x; }

static int constant(int x, int ignored) {
    (void)ignored;
    return x;
}

static int call_flipped(binop f, int a, int b) { return f(b, a); }

static int both(pred p, pred q, int x) { return p(x) && q(x); }

static int sub(int a, int b) { return a - b; }

static int is_even(int x) { return x % 2 == 0; }

static int is_positive(int x) { return x > 0; }

int main(void) {
    printf("%d\n", identity(5));
    printf("%d\n", constant(7, 99));
    printf("%d\n", call_flipped(sub, 2, 10));
    const char *sep = "";
    for (int x = -2; x <= 4; x++) {
        if (both(is_even, is_positive, x)) {
            printf("%s%d", sep, x);
            sep = " ";
        }
    }
    printf("\n");
    return 0;
}
```

```output for=combinator-c
5
7
8
2 4
```

- Verdict: absent. A C function cannot return a new function that keeps its arguments. Each helper takes the function pointers and the arguments in one call.
