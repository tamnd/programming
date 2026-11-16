```c run id=recoverable-c
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

static long port_or_default(const char *text, long fallback, const char **note) {
    /* A bad fallback is a bug in the caller: stop the program. */
    if (fallback < 1 || fallback > 65535) {
        abort();
    }
    /* Bad text is an expected error: recover. */
    char *end;
    errno = 0;
    long n = strtol(text, &end, 10);
    if (end == text || *end != '\0' || errno == ERANGE) {
        *note = " (not a number)";
        return fallback;
    }
    *note = "";
    return n;
}

int main(void) {
    const char *inputs[] = {"8080", "x"};
    for (int i = 0; i < 2; i++) {
        const char *note;
        long port = port_or_default(inputs[i], 80, &note);
        printf("%s -> %ld%s\n", inputs[i], port, note);
    }
    return 0;
}
```

```output for=recoverable-c
8080 -> 8080
x -> 80 (not a number)
```

- Verdict: idiom. C has no mechanism for either kind. A function reports an expected error with its return value, and the program calls `abort` for a bug, which raises `SIGABRT` and ends the process. `assert` also calls `abort` when its condition is false.
