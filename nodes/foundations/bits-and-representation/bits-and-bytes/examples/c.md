```c run id=masks-and-manual-byte-order
#include <stdint.h>
#include <stdio.h>

enum { READ = 1u << 0, WRITE = 1u << 1, EXEC = 1u << 2 };

static int popcount(uint32_t x) {
    int n = 0;
    for (; x != 0; x &= x - 1) /* clear the lowest set bit */
        n++;
    return n;
}

int main(void) {
    unsigned perm = READ | WRITE;
    perm &= ~(unsigned)WRITE;
    perm ^= EXEC;
    printf("%u %d %d\n", perm, popcount(perm), (perm & EXEC) != 0);

    uint32_t x = 0x12345678;
    uint8_t le[4];
    for (int i = 0; i < 4; i++)
        le[i] = (uint8_t)(x >> (8 * i)); /* least significant byte first */
    printf("%02x %02x %02x %02x\n", le[0], le[1], le[2], le[3]);
    uint32_t back = (uint32_t)le[0] << 24 | (uint32_t)le[1] << 16 | (uint32_t)le[2] << 8 | le[3];
    printf("%08x %u\n", (unsigned)back, 0xF0u >> 4);
    return 0;
}
```

```output for=masks-and-manual-byte-order
5 2 1
78 56 34 12
78563412 15
```

```c wrong id=and-equals-precedence
#include <stdio.h>

enum { READ = 1, WRITE = 2 };

int main(void) {
    unsigned perm = READ; /* no write permission */
    if (perm & WRITE == 0) /* parsed as perm & (WRITE == 0) */
        printf("read only\n");
    else
        printf("writable\n");
    if ((perm & WRITE) == 0)
        printf("read only\n");
    return 0;
}
```

```output for=and-equals-precedence
writable
read only
```

- Verdict: native. C has `&`, `|`, `^`, `~`, `<<` and `>>`, and the bit operations of most other languages come from it. Use unsigned types for bit patterns: shifting a negative value left, shifting into the sign bit, or shifting by a count at or above the width is undefined behavior, and right shift of a negative value is implementation-defined. Building bytes with shifts and masks, as the example does, gives the same result on every machine, whatever its byte order.
- The wrong block writes `perm & WRITE == 0`. In C, `==` binds more tightly than `&`, so the test is `perm & (WRITE == 0)`, which is always 0. Put parentheses around every bitwise expression inside a comparison. The compiler warns with `-Wparentheses`.
