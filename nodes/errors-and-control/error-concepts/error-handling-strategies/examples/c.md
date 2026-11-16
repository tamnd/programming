```c run id=strategy-c
#include <stdio.h>

enum { DIV_OK = 0, DIV_BY_ZERO = 1 };

/* Returns an error code. The quotient goes through out. */
static int divide(int a, int b, int *out) {
    if (b == 0) {
        return DIV_BY_ZERO;
    }
    *out = a / b;
    return DIV_OK;
}

int main(void) {
    int pairs[2][2] = {{10, 2}, {10, 0}};
    for (int i = 0; i < 2; i++) {
        int a = pairs[i][0], b = pairs[i][1], q;
        if (divide(a, b, &q) == DIV_BY_ZERO) {
            printf("%d / %d: error: division by zero\n", a, b);
        } else {
            printf("%d / %d = %d\n", a, b, q);
        }
    }
    return 0;
}
```

```output for=strategy-c
10 / 2 = 5
10 / 0: error: division by zero
```

- Verdict: idiom. Strategy: error codes. The function returns a code, and the result goes through a pointer. The C library also uses a special return value, such as -1 or `NULL`, together with `errno`. Nothing makes the caller test the code.
