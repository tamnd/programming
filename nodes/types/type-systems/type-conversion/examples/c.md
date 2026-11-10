```c run id=conversions
#include <stdio.h>

int main(void) {
    unsigned char a = 200;
    unsigned char b = 100;
    int sum = a + b;
    unsigned char wrapped = (unsigned char)(a + b);
    double half = 7 / 2;
    int truncated = (int)-3.99;
    printf("%d %d %.1f %d\n", sum, wrapped, half, truncated);
    printf("%zu %zu\n", sizeof a, sizeof(+a));
    return 0;
}
```

```output for=conversions
300 44 3.0 -3
1 4
```

`atoi` cannot report an error. `strtol` reports where it stopped and sets `errno` on overflow, so a caller can reject bad input.

```c run id=parse
#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

static int parse_int(const char *text, int *out) {
    char *end;
    errno = 0;
    long value = strtol(text, &end, 10);
    if (end == text || *end != '\0') {
        return 0;
    }
    if (errno == ERANGE || value < INT_MIN || value > INT_MAX) {
        return 0;
    }
    *out = (int)value;
    return 1;
}

int main(void) {
    const char *inputs[] = {"42", "42abc", "", "99999999999"};
    for (int i = 0; i < 4; i++) {
        int n;
        if (parse_int(inputs[i], &n)) {
            printf("\"%s\" -> %d\n", inputs[i], n);
        } else {
            printf("\"%s\" -> rejected\n", inputs[i]);
        }
    }
    printf("atoi(\"42abc\") = %d\n", atoi("42abc"));
    return 0;
}
```

```output for=parse
"42" -> 42
"42abc" -> rejected
"" -> rejected
"99999999999" -> rejected
atoi("42abc") = 42
```

- Verdict: native. The integer promotions and the usual arithmetic conversions run on every arithmetic operator.
- `a + b` is 300, because both `unsigned char` operands become `int` first. `sizeof(+a)` shows the promoted type: 4 bytes for `int` on the machine that recorded the output.
- A conversion to an unsigned type wraps modulo 2ⁿ, so 300 becomes 44. A conversion of a floating value out of the range of the integer type is undefined behavior.
- `atoi("42abc")` returns 42 and gives no sign that it ignored text.
