```c run id=error-c
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

/* Returns 0 on success and -1 on error. The number goes through out. */
static int parse(const char *text, long *out) {
    char *end;
    errno = 0;
    long n = strtol(text, &end, 10);
    if (end == text || *end != '\0' || errno == ERANGE) {
        return -1;
    }
    *out = n;
    return 0;
}

int main(void) {
    const char *inputs[] = {"42", "x42"};
    for (int i = 0; i < 2; i++) {
        long n;
        if (parse(inputs[i], &n) == 0) {
            printf("%s -> %ld\n", inputs[i], n);
        } else {
            printf("%s -> error (return value -1)\n", inputs[i]);
        }
    }
    return 0;
}
```

```output for=error-c
42 -> 42
x42 -> error (return value -1)
```

- Verdict: idiom. C has no error mechanism in the language. A function returns a special value, such as -1 or `NULL`, and the library sets `errno` to say which error occurred. `strtol` gives 0 for a string with no digits, so the code tests the end pointer to find the error.
