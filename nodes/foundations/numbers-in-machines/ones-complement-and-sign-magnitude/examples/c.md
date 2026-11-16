```c run id=forms
#include <math.h>
#include <stdio.h>

static unsigned to_sign_magnitude(int n) { return n < 0 ? 0x80u | (unsigned)-n : (unsigned)n; }
static unsigned to_ones(int n) { return n < 0 ? ~(unsigned)-n & 0xFFu : (unsigned)n; }
static int from_sign_magnitude(unsigned b) { return (b & 0x80u) ? -(int)(b & 0x7Fu) : (int)b; }
static int from_ones(unsigned b) { return (b & 0x80u) ? -(int)(~b & 0xFFu) : (int)b; }

static const char *bin8(unsigned b, char out[9]) {
    for (int i = 0; i < 8; i++) {
        out[i] = (b >> (7 - i)) & 1u ? '1' : '0';
    }
    out[8] = '\0';
    return out;
}

int main(void) {
    char buf[9] = {0};
    printf("-43 in sign-magnitude: %s\n", bin8(to_sign_magnitude(-43), buf));
    printf("-43 in ones' complement: %s\n", bin8(to_ones(-43), buf));
    printf("10101011 in sign-magnitude: %d\n", from_sign_magnitude(0xABu));
    printf("11010100 in ones' complement: %d\n", from_ones(0xD4u));
    printf("10000000 in sign-magnitude: %d\n", from_sign_magnitude(0x80u));
    printf("11111111 in ones' complement: %d\n", from_ones(0xFFu));

    double z = -0.0;
    printf("-0.0 == 0.0: %s\n", z == 0.0 ? "yes" : "no");
    printf("signbit(-0.0): %s\n", signbit(z) ? "yes" : "no");
    printf("1 / -0.0: %f\n", 1.0 / z);
    return 0;
}
```

```output for=forms
-43 in sign-magnitude: 10101011
-43 in ones' complement: 11010100
10101011 in sign-magnitude: -43
11010100 in ones' complement: -43
10000000 in sign-magnitude: 0
11111111 in ones' complement: 0
-0.0 == 0.0: yes
signbit(-0.0): yes
1 / -0.0: -inf
```

- Verdict: idiom. Before C23 an implementation could use either form for its signed types, but C23 allows only two's complement. A program that reads old data makes the forms by hand with unsigned operations.
- The functions use `unsigned` so that `~` and `&` act on known bits.
- `signbit` from `<math.h>` reads the sign of a float, and it finds the sign of `-0.0`.
