```c run id=sizeof-comma-and-conversions
#include <stdio.h>

int main(void) {
    /* sizeof is an operator, and it does not evaluate its operand. */
    int n = 5;
    size_t s = sizeof n++;
    printf("sizeof(int) = %zu, n = %d\n", s, n);

    /* Integer / truncates toward zero, and % has the sign of the left operand. */
    printf("%d %d\n", -7 / 2, -7 % 2);

    /* The comma operator evaluates the left operand, then gives the right one. */
    int i = (n = 3, n * 2);
    printf("i = %d\n", i);

    /* Bitwise and shift operators. */
    unsigned x = 0x0F;
    printf("%u %u %u %u\n", x & 0x3, x | 0x30, x ^ 0xFF, x << 4);

    /* The conditional operator ?: is the only ternary operator. */
    printf("%s\n", n > 2 ? "big" : "small");

    /* The usual arithmetic conversions: -1 becomes a large unsigned value. */
    printf("-1 < 1u is %s\n", -1 < 1u ? "true" : "false");
    return 0;
}
```

```output for=sizeof-comma-and-conversions
sizeof(int) = 4, n = 5
-3 -1
i = 6
3 63 240 240
big
-1 < 1u is false
```

- Verdict: native. `sizeof` is an operator that does not evaluate its operand, so `n` stays `5`, and the compiler warns about the side effect on purpose. Integer `/` truncates toward zero. The comma operator gives its right operand. The usual arithmetic conversions make `-1` an unsigned value, so `-1 < 1u` is false.
