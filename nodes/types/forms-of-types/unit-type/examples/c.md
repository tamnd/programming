```c run id=void-functions
#include <stdio.h>

static int count = 0;

/* A function without a result: it runs for its side effect. */
static void bump(int by) {
    count += by;
}

/* (void) in the parameter list means that the function takes no arguments. */
static int get(void) {
    return count;
}

int main(void) {
    bump(2);
    bump(3);
    printf("%d\n", get());

    /* A cast to void discards a result on purpose. */
    (void)printf("printed\n");

    /* void * is a different idea: a pointer to data of an unknown type. */
    int x = 42;
    void *p = &x;
    printf("%d\n", *(int *)p);
    return 0;
}
```

```output for=void-functions
5
printed
42
```

- Verdict: idiom. C has no unit value. `void` as a result type means that a call gives no value, so `bump(2)` can only be a statement, and `void` in a parameter list means no parameters. The cast `(void)` discards the result of `printf` and tells the reader that this is on purpose. `void *` is a pointer to data of an unknown type, which is not a unit type.
