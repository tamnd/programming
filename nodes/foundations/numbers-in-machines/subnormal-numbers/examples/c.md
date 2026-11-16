```c run id=tiny
#include <float.h>
#include <math.h>
#include <stdio.h>

static const char *category(double x) {
    switch (fpclassify(x)) {
    case FP_ZERO:
        return "zero";
    case FP_SUBNORMAL:
        return "subnormal";
    case FP_NORMAL:
        return "normal";
    case FP_INFINITE:
        return "infinite";
    default:
        return "nan";
    }
}

int main(void) {
    volatile double x = 1.25 * DBL_MIN;
    volatile double y = DBL_MIN;
    double values[] = {DBL_MIN, DBL_MIN / 2, DBL_TRUE_MIN, x - y, DBL_TRUE_MIN / 2};
    const char *names[] = {"DBL_MIN", "DBL_MIN / 2", "DBL_TRUE_MIN", "x - y", "DBL_TRUE_MIN / 2"};
    for (int i = 0; i < 5; i++) {
        printf("%-16s %-22a %s\n", names[i], values[i], category(values[i]));
    }
    printf("FLT_TRUE_MIN = %g\n", FLT_TRUE_MIN);
    return 0;
}
```

```output for=tiny
DBL_MIN          0x1p-1022              normal
DBL_MIN / 2      0x0.8p-1022            subnormal
DBL_TRUE_MIN     0x0.0000000000001p-1022 subnormal
x - y            0x0.4p-1022            subnormal
DBL_TRUE_MIN / 2 0x0p+0                 zero
FLT_TRUE_MIN = 1.4013e-45
```

- Verdict: native. C has `DBL_MIN` for the smallest normal value and `DBL_TRUE_MIN` for the smallest subnormal.
- `fpclassify` returns `FP_SUBNORMAL` for a subnormal. The `%a` format shows a subnormal with a leading `0x0.` and the exponent `-1022`.
- The output assumes no flush-to-zero mode. A build with `-ffast-math` can set FTZ and DAZ, and then the subnormal lines can print zero.
