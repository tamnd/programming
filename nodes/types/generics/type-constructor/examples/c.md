```c run id=macro-struct
#include <stdio.h>

// C has no generic types. A macro writes one struct for each element type.
#define DEFINE_PAIR(name, A, B) \
    typedef struct {            \
        A first;                \
        B second;               \
    } name

DEFINE_PAIR(pair_int_str, int, const char *);
DEFINE_PAIR(pair_dbl_dbl, double, double);

int main(void) {
    pair_int_str p = {42, "age"};
    pair_dbl_dbl q = {1.5, 2.5};

    // The built-in constructors: array, pointer and function types.
    int a[3] = {1, 2, 3};
    int *ptr = a;
    int (*fn)(const char *, ...) = printf;

    fn("%d %s %.1f %d %zu\n", p.first, p.second, q.first + q.second, ptr[2], sizeof a / sizeof a[0]);
    return 0;
}
```

```output for=macro-struct
42 age 4.0 3 3
```

- Verdict: idiom. The macro `DEFINE_PAIR` takes two types and makes a struct type, but each use writes a different struct with its own name. C has no name for "a pair of A and B", so the compiler cannot relate `pair_int_str` and `pair_dbl_dbl`. The built-in derived types are type constructors: `int[3]`, `int *` and a pointer to a function are built from other types.
