```c run id=sizes-and-promotion
#include <complex.h>
#include <limits.h>
#include <stdio.h>

int main(void) {
    /* The sizes are set by the platform; this is a 64-bit system. */
    printf("%zu %zu %zu %zu %d\n", sizeof(short), sizeof(int), sizeof(long), sizeof(long long), INT_MAX);

    /* Integer division truncates, and % takes the sign of the dividend. */
    int n = 7, m = -7;
    printf("%d %d %d %.1f\n", n / 2, m / 2, m % 2, n / 2.0);

    /* Operands smaller than int are promoted to int before the arithmetic. */
    unsigned char a = 200, b = 100;
    printf("%d %zu\n", a + b, sizeof(a + b));

    /* 5 / 2 is an int, so the order of the operations matters. */
    printf("%.1f %.1f\n", n / 2 * 2.0, n / 2.0 * 2);

    /* A float keeps 24 bits, so 16777217 becomes 16777216. */
    int big = 16777217;
    float f = (float)big;
    printf("%.1f\n", f);

    /* C has complex types. */
    double complex z = 3.0 + 4.0 * I;
    printf("%.1f %.1f %.1f\n", creal(z), cimag(z), cabs(z));
    return 0;
}
```

```output for=sizes-and-promotion
2 4 8 8 2147483647
3 -3 -1 3.5
300 4
6.0 7.0
16777216.0
3.0 4.0 5.0
```

- Verdict: native. The sizes of `short`, `int`, `long` and `long long` come from the platform, here 2, 4, 8 and 8 bytes. `/` truncates. Two `unsigned char` values are promoted to `int` before the addition, so the sum is 300 and has the size of an `int`. `7 / 2 * 2.0` is `6.0`, but `7 / 2.0 * 2` is `7.0`. A `float` keeps 24 bits, so 16777217 becomes 16777216. `double complex` is part of the language.
