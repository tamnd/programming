```c run id=ports
#include <errno.h>
#include <setjmp.h>
#include <stdio.h>
#include <stdlib.h>

enum { PORT_OK, NOT_A_NUMBER, OUT_OF_RANGE };

static int parse_port(const char *text, int *out) {
    char *end;
    errno = 0;
    long n = strtol(text, &end, 10);
    if (end == text || *end != '\0') {
        return NOT_A_NUMBER;
    }
    if (errno == ERANGE || n < 1 || n > 65535) {
        return OUT_OF_RANGE;
    }
    *out = (int)n;
    return PORT_OK;
}

static jmp_buf on_error;

static void check(int code) {
    if (code != PORT_OK) {
        longjmp(on_error, code);
    }
}

int main(void) {
    const char *texts[] = {"8080", "70000", "abc"};
    int port;
    for (int i = 0; i < 3; i++) {
        int code = parse_port(texts[i], &port);
        if (code == PORT_OK) {
            printf("port %d\n", port);
        } else {
            printf("error %d: %s\n", code, texts[i]);
        }
    }

    volatile int done = 0;
    switch (setjmp(on_error)) {
    case 0:
        check(parse_port("443", &port));
        done++;
        check(parse_port("abc", &port));
        done++;
        printf("not reached\n");
        break;
    case NOT_A_NUMBER:
        printf("longjmp: not a number after %d\n", done);
        break;
    }
}
```

```output
port 8080
error 2: 70000
error 1: abc
longjmp: not a number after 1
```

- Verdict: idiom. C has no exceptions. A function returns a status code, and the caller tests it.
- No library function sets `errno` to zero, so the code does it before `strtol`.
- `longjmp` returns to the point of `setjmp` from a deeper call. No cleanup runs in the skipped frames.
- A local variable that changes after `setjmp` is indeterminate after `longjmp` unless it is `volatile`, like `done`.
