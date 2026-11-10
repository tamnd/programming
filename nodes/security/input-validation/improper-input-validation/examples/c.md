```c wrong id=atoi
#include <stdio.h>
#include <stdlib.h>

int main(void) {
    const char *inputs[] = {"2", "-3", "3 apples", "", "abc"};
    for (int i = 0; i < 5; i++) {
        int quantity = atoi(inputs[i]);
        printf("\"%s\" -> %d, total %d\n", inputs[i], quantity, 1200 * quantity);
    }
    return 0;
}
```

```output for=atoi
"2" -> 2, total 2400
"-3" -> -3, total -3600
"3 apples" -> 3, total 3600
"" -> 0, total 0
"abc" -> 0, total 0
```

```c run flags="-fsanitize=address,undefined" id=strtol
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

/* Returns 0 and sets *out when text is 1 to 3 ASCII digits with a value
   from 1 to 100. Returns -1 otherwise. */
static int parse_quantity(const char *text, int *out) {
    int len = 0;
    for (const char *p = text; *p != '\0'; p++, len++) {
        if (*p < '0' || *p > '9' || len >= 3) {
            return -1;
        }
    }
    if (len == 0) {
        return -1;
    }
    char *end;
    errno = 0;
    long value = strtol(text, &end, 10);
    if (errno != 0 || *end != '\0' || value < 1 || value > 100) {
        return -1;
    }
    *out = (int)value;
    return 0;
}

int main(void) {
    const char *inputs[] = {"2", "-3", "3 apples", "", "abc", "0", "100"};
    for (int i = 0; i < 7; i++) {
        int quantity;
        if (parse_quantity(inputs[i], &quantity) != 0) {
            printf("\"%s\" rejected\n", inputs[i]);
        } else {
            printf("\"%s\" -> %d, total %d\n", inputs[i], quantity, 1200 * quantity);
        }
    }
    return 0;
}
```

```output for=strtol
"2" -> 2, total 2400
"-3" rejected
"3 apples" rejected
"" rejected
"abc" rejected
"0" rejected
"100" -> 100, total 120000
```

- Verdict: stdlib. `strtol` from `<stdlib.h>` reports where the number ends and sets `errno` on overflow; the caller writes the checks.
- `atoi` has no way to report an error. It returns `0` for `""` and for `"abc"`, and `3` for `"3 apples"`. POSIX says that its behaviour is undefined if the value cannot be represented, so a long string of digits is not only wrong but undefined.
- `strtol` skips leading white space and accepts a sign. To know that the whole string was a number, check that `*end` is `'\0'`. To know that it fits, set `errno` to `0` before the call and check it after.
- The parser also limits the length before the conversion. A quantity in C often becomes a size or an index next, where a wrong value is an [[out-of-bounds-write]] and not only a wrong total.
