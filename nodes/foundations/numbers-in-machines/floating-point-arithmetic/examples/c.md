```c run id=bits
#include <float.h>
#include <math.h>
#include <stdio.h>

int main(void) {
    double x = 0.1 + 0.2;
    printf("0.1 + 0.2 = %.17g\n", x);
    printf("0.3       = %.17g\n", 0.3);
    printf("hex form of 0.1: %a\n", 0.1);
    printf("DBL_MANT_DIG = %d, DBL_DIG = %d\n", DBL_MANT_DIG, DBL_DIG);
    printf("DBL_EPSILON = %g\n", DBL_EPSILON);
    printf("DBL_MAX = %g\n", DBL_MAX);
    printf("DBL_MAX * 2 = %g\n", DBL_MAX * 2);
    printf("smallest normal = %g, smallest subnormal = %g\n", DBL_MIN, nextafter(0.0, 1.0));

    double zero = 0.0, neg = -0.0;
    printf("0.0 == -0.0: %d\n", zero == neg);
    printf("1/0.0 = %g, 1/-0.0 = %g\n", 1 / zero, 1 / neg);
    printf("signbit(-0.0) = %d\n", signbit(neg) != 0);
    double nan = zero / zero;
    printf("0/0 = %g, isnan = %d, nan == nan: %d\n", fabs(nan), isnan(nan) != 0, nan == nan);
}
```

```output
0.1 + 0.2 = 0.30000000000000004
0.3       = 0.29999999999999999
hex form of 0.1: 0x1.999999999999ap-4
DBL_MANT_DIG = 53, DBL_DIG = 15
DBL_EPSILON = 2.22045e-16
DBL_MAX = 1.79769e+308
DBL_MAX * 2 = inf
smallest normal = 2.22507e-308, smallest subnormal = 4.94066e-324
0.0 == -0.0: 1
1/0.0 = inf, 1/-0.0 = -inf
signbit(-0.0) = 1
0/0 = nan, isnan = 1, nan == nan: 0
```

- Verdict: native. On almost all platforms `double` is IEEE 754 binary64. An implementation that defines `__STDC_IEC_60559_BFP__` (C23) or `__STDC_IEC_559__` (older standards) follows Annex F, which binds `float` and `double` to the IEC 60559 formats.
- `%.17g` prints 17 significant digits, which are enough to tell any two doubles apart. `%a` prints the exact bits as a hexadecimal significand and a binary exponent.
- Overflow gives infinity, and division of a nonzero number by zero gives an infinity with the sign of the zero. Neither is an error in IEEE 754 arithmetic.
- The code prints `fabs(nan)`, because the sign of a `NaN` from `0/0` depends on the processor.

Subtraction of two close values cancels the digits that they share. What is left is mostly the earlier rounding error.

```c run id=cancellation
#include <math.h>
#include <stdio.h>

int main(void) {
    double small = 1e-15;
    double naive = (1.0 + small) - 1.0;
    printf("(1 + 1e-15) - 1 = %.17g\n", naive);
    printf("relative error  = %.2g\n", fabs(naive - small) / small);

    double x = 1e-10;
    printf("log(1 + x)  = %.17g\n", log(1.0 + x));
    printf("log1p(x)    = %.17g\n", log1p(x));
}
```

```output
(1 + 1e-15) - 1 = 1.1102230246251565e-15
relative error  = 0.11
log(1 + x)  = 1.000000082690371e-10
log1p(x)    = 9.9999999995000007e-11
```

- `log1p` computes `log(1 + x)` without the rounding of `1 + x`, so it keeps the digits of a small `x`.
