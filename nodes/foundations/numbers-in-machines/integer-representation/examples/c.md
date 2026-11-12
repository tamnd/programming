```c run id=stdckdint-and-conversions
#include <inttypes.h>
#include <limits.h>
#include <stdckdint.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

int main(void) {
    /* Unsigned arithmetic is defined to wrap modulo 2^N. */
    uint8_t u = 0;
    u = (uint8_t)(u - 1);
    printf("%u %u\n", (unsigned)u, UINT_MAX);

    /* Signed overflow is undefined behavior, so check before the operation, or use ckd_add. */
    int a = INT_MAX, b = 1, r;
    if (b > 0 && a > INT_MAX - b)
        puts("a + b would overflow");
    if (ckd_add(&r, a, b))
        printf("ckd_add: overflow, wrapped result %d\n", r);
    printf("%d %d\n", INT_MIN, INT_MAX);

    /* Widening a signed value copies the sign bit; widening an unsigned value adds zeros. */
    int8_t s = -5;
    printf("%" PRId32 " %" PRIu32 "\n", (int32_t)s, (uint32_t)(uint8_t)s);

    /* The comparison converts -1 to unsigned, so it is false. */
    int neg = -1;
    unsigned one = 1;
    printf("-1 < 1u: %d\n", neg < one);

    /* The bytes of an int in memory follow the byte order of the machine. */
    int32_t v = -1000;
    unsigned char bytes[4];
    memcpy(bytes, &v, sizeof v);
    printf("%02x %02x %02x %02x\n", bytes[0], bytes[1], bytes[2], bytes[3]);
    return 0;
}
```

```output for=stdckdint-and-conversions
255 4294967295
a + b would overflow
ckd_add: overflow, wrapped result -2147483648
-2147483648 2147483647
-5 251
-1 < 1u: 0
18 fc ff ff
```

- Verdict: native. C has signed and unsigned types and the exact widths of `<stdint.h>`. Unsigned arithmetic wraps modulo 2ᴺ, but signed overflow is undefined behavior, so the program tests before the addition or uses `ckd_add` from `<stdckdint.h>`, which returns true and stores the wrapped result. The comparison `-1 < 1u` converts −1 to unsigned and is false. The last line shows the bytes of an `int32_t` in memory, here on a little-endian machine.
