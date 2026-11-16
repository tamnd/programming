```c run id=codes-c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum { OK, EMPTY, NOT_A_NUMBER, OUT_OF_RANGE };
static const char *names[] = {"ok", "empty", "not a number", "out of range"};

/* Returns a code. On success, writes the port to *port. */
static int parse_port(const char *text, int *port) {
    size_t n = strlen(text);
    if (n == 0) return EMPTY;
    if (strspn(text, "0123456789") != n) return NOT_A_NUMBER;
    if (n > 5 || atoi(text) > 65535) return OUT_OF_RANGE;
    *port = atoi(text);
    return OK;
}

int main(void) {
    const char *texts[] = {"8080", "", "x", "99999"};
    for (int i = 0; i < 4; i++) {
        int port = 0;
        int code = parse_port(texts[i], &port);
        if (code != OK) {
            printf("'%s' -> error code %d (%s)\n", texts[i], code, names[code]);
        } else {
            printf("'%s' -> port %d\n", texts[i], port);
        }
    }
    return 0;
}
```

```output for=codes-c
'8080' -> port 8080
'' -> error code 1 (empty)
'x' -> error code 2 (not a number)
'99999' -> error code 3 (out of range)
```

- Verdict: native. C has no exceptions, so error codes are the usual mechanism. Several standard library functions "indicate errors by writing positive integers to errno", and many functions return a code, as `parse_port` does here.
