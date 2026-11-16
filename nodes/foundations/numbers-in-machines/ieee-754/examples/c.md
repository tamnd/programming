```c run id=fields
#include <float.h>
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

static uint32_t bits32(float x) {
    uint32_t b;
    memcpy(&b, &x, sizeof b);
    return b;
}

static uint64_t bits64(double x) {
    uint64_t b;
    memcpy(&b, &x, sizeof b);
    return b;
}

int main(void) {
    const double xs[] = {1.0, -2.5, 0.1};
    for (int i = 0; i < 3; i++) {
        printf("%4.1f binary32 %08" PRIx32 " binary64 %016" PRIx64 "\n",
               xs[i], bits32((float)xs[i]), bits64(xs[i]));
    }

    uint64_t b = bits64(-2.5);
    printf("-2.5: sign %" PRIu64 ", exponent %d, fraction %" PRIx64 "\n",
           b >> 63, (int)((b >> 52) & 0x7ff) - 1023, b & ((UINT64_C(1) << 52) - 1));

    printf("FLT_RADIX %d, FLT_MANT_DIG %d, DBL_MANT_DIG %d\n", FLT_RADIX, FLT_MANT_DIG, DBL_MANT_DIG);
    printf("FLT_MAX_EXP %d, DBL_MAX_EXP %d\n", FLT_MAX_EXP, DBL_MAX_EXP);
    printf("DBL_DECIMAL_DIG %d\n", DBL_DECIMAL_DIG);
    return 0;
}
```

```output for=fields
 1.0 binary32 3f800000 binary64 3ff0000000000000
-2.5 binary32 c0200000 binary64 c004000000000000
 0.1 binary32 3dcccccd binary64 3fb999999999999a
-2.5: sign 1, exponent 1, fraction 4000000000000
FLT_RADIX 2, FLT_MANT_DIG 24, DBL_MANT_DIG 53
FLT_MAX_EXP 128, DBL_MAX_EXP 1024
DBL_DECIMAL_DIG 17
```

- Verdict: native. On common platforms, C `float` is binary32 and `double` is binary64. The C standard allows other formats, and the macros of `<float.h>` describe the format that the compiler uses.
- `memcpy` copies the bytes of the float into an integer of the same width. A pointer cast between `double *` and `uint64_t *` breaks the aliasing rules, but `memcpy` is safe.
- `FLT_MANT_DIG` and `DBL_MANT_DIG` count the significand bits with the hidden bit, so they are 24 and 53. `DBL_MAX_EXP` is the largest integer e such that 2 to the power e - 1 is a finite `double`, so it is 1024, one more than the largest exponent of binary64.
- `DBL_DECIMAL_DIG` is 17, the number of decimal digits that always gives back the same `double`.
