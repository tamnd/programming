```c run id=bits
#include <limits.h>
#include <stdint.h>
#include <stdio.h>

int main(void) {
    int x = 6;
    printf("-x == ~x + 1: %s\n", -x == ~x + 1 ? "yes" : "no");
    printf("-1 in hex: %x\n", (unsigned int) -1);

    int8_t small = -6;
    int16_t wide = small;
    printf("int8_t -6 as int16_t: %04x\n", (unsigned int) (uint16_t) wide);

    int y = INT_MIN;
    if (y == INT_MIN) {
        printf("-INT_MIN does not fit; INT_MAX is %d\n", INT_MAX);
    } else {
        printf("-y: %d\n", -y);
    }
    return 0;
}
```

```output for=bits
-x == ~x + 1: yes
-1 in hex: ffffffff
int8_t -6 as int16_t: fffa
-INT_MIN does not fit; INT_MAX is 2147483647
```

- Verdict: native. C23 allows only two's complement for signed integers, and the compilers in use did so before.
- The conversion of `int8_t` to `int16_t` keeps the value -6, so the new high bits are copies of the sign bit.
- `-INT_MIN` is undefined behavior, so the program tests for `INT_MIN` before the negation.
