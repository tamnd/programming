```c run id=print-and-parse-bits
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

/* printf has no binary conversion in older C libraries, so write the digits by hand. */
static void print_bits(unsigned value, int width) {
    for (int i = width - 1; i >= 0; i--) putchar(value >> i & 1u ? '1' : '0');
    putchar('\n');
}

int main(void) {
    unsigned n = 0xAC; /* hexadecimal: each digit is 4 bits */
    printf("%u %o %x %X\n", n, n, n, n);
    print_bits(n, 8);
    print_bits((unsigned)-5, 32); /* conversion to unsigned gives the two's complement bits */

    char *end;
    long v = strtol("101101", &end, 2);
    printf("%ld, rest \"%s\"\n", v, end);
    v = strtol("1012", &end, 2); /* stops at the first digit that is not binary */
    printf("%ld, rest \"%s\"\n", v, end);

    printf("bits in unsigned: %zu\n", sizeof(unsigned) * CHAR_BIT);
    return 0;
}
```

```output for=print-and-parse-bits
172 254 ac AC
10101100
11111111111111111111111111111011
45, rest ""
5, rest "2"
bits in unsigned: 32
```

- Verdict: native. C has octal and hexadecimal conversions in `printf`, and the program prints binary with its own loop, because a `%b` conversion is new and not in every C library. Converting −5 to `unsigned` gives its two's complement bits. `strtol` with base 2 stops at the first digit that is not binary and sets `end` to the rest, so the caller must check that nothing is left.
