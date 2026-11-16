```c run id=truncated
#include <stdio.h>
#include <stdlib.h>

static int floor_mod(int a, int n) {
    int r = a % n;
    if (r != 0 && (r < 0) != (n < 0)) {
        r += n;
    }
    return r;
}

static int floor_div(int a, int n) {
    int q = a / n;
    if (a % n != 0 && (a < 0) != (n < 0)) {
        q -= 1;
    }
    return q;
}

int main(void) {
    int a = -7;
    int n = 3;
    printf("-7 / 3: %d\n", a / n);
    printf("-7 %% 3: %d\n", a % n);
    div_t d = div(a, n);
    printf("div(-7, 3): quot %d, rem %d\n", d.quot, d.rem);
    printf("floor_div(-7, 3): %d\n", floor_div(a, n));
    printf("floor_mod(-7, 3): %d\n", floor_mod(a, n));
    printf("-7 %% 2 == 1: %s\n", a % 2 == 1 ? "yes" : "no");
    return 0;
}
```

```output for=truncated
-7 / 3: -2
-7 % 3: -1
div(-7, 3): quot -2, rem -1
floor_div(-7, 3): -3
floor_mod(-7, 3): 2
-7 % 2 == 1: no
```

- Verdict: native. Since C99, `/` truncates toward zero and `%` has the sign of the dividend. `div` from `<stdlib.h>` gives both results in a `div_t`.
- The floored helpers correct the truncated result when the remainder is not zero and the signs differ.
- Division by zero is undefined behavior, so a program must test the divisor first.
