```c run id=q15
#include <stdint.h>
#include <stdio.h>

#define Q 15
#define K (1 << (Q - 1))

static int16_t sat16(int32_t x) {
    if (x > 0x7FFF) {
        return 0x7FFF;
    }
    if (x < -0x8000) {
        return -0x8000;
    }
    return (int16_t)x;
}

static int16_t q_mul(int16_t a, int16_t b) {
    int32_t temp = (int32_t)a * (int32_t)b;
    temp += K;
    return sat16(temp >> Q);
}

static double q_to_double(int16_t x) {
    return x / 32768.0;
}

int main(void) {
    int16_t half = 16384;
    int16_t minus_one = -32768;
    int16_t p = q_mul(half, half);
    printf("0.5 * 0.5: %d = %g\n", p, q_to_double(p));
    p = q_mul(minus_one, minus_one);
    printf("-1 * -1: %d = %.15g\n", p, q_to_double(p));
    p = q_mul(minus_one, half);
    printf("-1 * 0.5: %d = %g\n", p, q_to_double(p));
    return 0;
}
```

```output for=q15
0.5 * 0.5: 8192 = 0.25
-1 * -1: 32767 = 0.999969482421875
-1 * 0.5: -16384 = -0.5
```

- Verdict: idiom. Standard C has no fixed-point type. The block is the Q15 multiplication from Wikipedia's Q notation page: a 32-bit product, a rounding constant, a shift and a saturation to 16 bits.
- -1 × -1 is 1, which Q15 cannot hold, so the result saturates to 32767, the value nearest to 1.
- GCC has `_Fract` and `_Accum` types as an extension, but only on some targets.
