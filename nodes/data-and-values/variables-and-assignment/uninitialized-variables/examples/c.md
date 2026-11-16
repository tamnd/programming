```c run id=initialize-everything
#include <stdio.h>
#include <string.h>

static int parse_digit(const char *s, int *out) {
    if (s[0] < '0' || s[0] > '9') {
        return 0;
    }
    *out = s[0] - '0';
    return 1;
}

int main(void) {
    int value = -1;
    if (!parse_digit("x", &value)) {
        printf("no value: %d\n", value);
    }
    if (parse_digit("7", &value)) {
        printf("value: %d\n", value);
    }
    char buf[8] = {0};
    printf("%zu\n", strlen(buf));
    return 0;
}
```

```output for=initialize-everything
no value: -1
value: 7
0
```

- Verdict: native. C allows a variable with no value, and a read of it is undefined behavior, so a program that shows such a read has no fixed output. The safe habit is in this example: `value` starts at -1, so it has a known value when `parse_digit` fails and does not write it, and `= {0}` fills the whole buffer with zeros.
