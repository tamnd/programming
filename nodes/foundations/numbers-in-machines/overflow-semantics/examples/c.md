```c run id=checks
#include <limits.h>
#include <stdio.h>

int main(void) {
    unsigned u = UINT_MAX;
    printf("UINT_MAX + 1: %u\n", u + 1u);

    int x = INT_MAX;
    int r = 0;
    int over = __builtin_add_overflow(x, 1, &r);
    printf("__builtin_add_overflow(x, 1): %s, result %d\n", over ? "overflow" : "ok", r);

    if (x > INT_MAX - 1) {
        printf("x + 1 would overflow\n");
    }

    long long wide = (long long)x + 1;
    int saturated = wide > INT_MAX ? INT_MAX : (int)wide;
    printf("saturated: %d\n", saturated);
    return 0;
}
```

```output for=checks
UINT_MAX + 1: 0
__builtin_add_overflow(x, 1): overflow, result -2147483648
x + 1 would overflow
saturated: 2147483647
```

- Verdict: stdlib. Unsigned arithmetic wraps, and signed overflow is undefined behavior. C23 adds `ckd_add`, `ckd_sub` and `ckd_mul` in `<stdckdint.h>`. This example uses `__builtin_add_overflow`, the GCC and Clang builtin that does the same job, because older compilers do not have the header.
- The test `x > INT_MAX - 1` runs before the addition, so it never overflows.
- A wider type gives the exact sum, and a compare saturates it.
