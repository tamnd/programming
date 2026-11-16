```c run id=ties
#include <fenv.h>
#include <math.h>
#include <stdio.h>

int main(void) {
    const int modes[] = {FE_TONEAREST, FE_UPWARD, FE_DOWNWARD, FE_TOWARDZERO};
    const char *names[] = {"FE_TONEAREST", "FE_UPWARD", "FE_DOWNWARD", "FE_TOWARDZERO"};
    volatile double one = 1.0;
    volatile double three = 3.0;
    volatile double tie = 2.5;
    double third[4];
    double pos[4];
    double neg[4];
    for (int i = 0; i < 4; i++) {
        fesetround(modes[i]);
        third[i] = one / three;
        pos[i] = nearbyint(tie);
        neg[i] = nearbyint(-tie);
    }
    fesetround(FE_TONEAREST);
    for (int i = 0; i < 4; i++) {
        printf("%-13s 1/3 = %.20f  nearbyint(2.5) = %2.0f  nearbyint(-2.5) = %2.0f\n", names[i], third[i], pos[i], neg[i]);
    }
    printf("round(2.5) = %.0f, round(-2.5) = %.0f\n", round(tie), round(-tie));
    printf("(int)-2.7 = %d\n", (int)-2.7);
    return 0;
}
```

```output for=ties
FE_TONEAREST  1/3 = 0.33333333333333331483  nearbyint(2.5) =  2  nearbyint(-2.5) = -2
FE_UPWARD     1/3 = 0.33333333333333337034  nearbyint(2.5) =  3  nearbyint(-2.5) = -2
FE_DOWNWARD   1/3 = 0.33333333333333331483  nearbyint(2.5) =  2  nearbyint(-2.5) = -3
FE_TOWARDZERO 1/3 = 0.33333333333333331483  nearbyint(2.5) =  2  nearbyint(-2.5) = -2
round(2.5) = 3, round(-2.5) = -3
(int)-2.7 = -2
```

- Verdict: native. `fesetround` changes the IEEE rounding mode of arithmetic, and `nearbyint` and `rint` round to an integer with the current mode.
- The quotient `1/3` changes with the mode: upward gives the double above one third, and the other modes give the double below it.
- `round` ignores the mode and always rounds half away from zero. A cast to `int` truncates toward zero.
- The operands are `volatile`, so the compiler cannot fold the division at compile time with the default mode. The program sets the mode back to nearest before it calls `printf`, because the C library can use the current mode when it prints.
