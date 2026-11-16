```c run id=mixed
#include <limits.h>
#include <stdint.h>
#include <stdio.h>

int main(void) {
    int minus_one = -1;
    unsigned int one = 1;
    printf("-1 < 1u: %s\n", minus_one < one ? "yes" : "no");
    printf("(unsigned int) -1 == UINT_MAX: %s\n", (unsigned int) minus_one == UINT_MAX ? "yes" : "no");

    unsigned int three = 3;
    unsigned int five = 5;
    printf("3u - 5u > 0: %s\n", three - five > 0 ? "yes" : "no");

    int16_t small = -1;
    uint8_t byte = 200;
    printf("int16_t -1 < uint8_t 200: %s\n", small < byte ? "yes" : "no");
    return 0;
}
```

```output for=mixed
-1 < 1u: no
(unsigned int) -1 == UINT_MAX: yes
3u - 5u > 0: yes
int16_t -1 < uint8_t 200: yes
```

- Verdict: native. C has an `unsigned` form of each integer type, and `<stdint.h>` has `uint8_t` to `uint64_t`.
- In `-1 < 1u`, both operands have the rank of `int`, so the signed operand becomes `unsigned int`, and -1 becomes the largest value.
- An unsigned subtraction that goes below zero wraps to a large value, so `3u - 5u` is more than 0.
- Types smaller than `int` first become `int`, so the comparison of `int16_t` and `uint8_t` is a signed comparison and gives the right answer.
