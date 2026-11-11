```c run id=port
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum config_error { CONFIG_OK, CONFIG_MISSING, CONFIG_NOT_A_NUMBER, CONFIG_OUT_OF_RANGE };

static const char *messages[] = {"ok", "missing key", "not a number", "out of range"};

static enum config_error parse_port(const char *text, int *port) {
    if (text == NULL) {
        return CONFIG_MISSING;
    }
    char *end;
    errno = 0;
    long n = strtol(text, &end, 10);
    if (end == text || *end != '\0') {
        return CONFIG_NOT_A_NUMBER;
    }
    if (errno == ERANGE || n < 1 || n > 65535) {
        return CONFIG_OUT_OF_RANGE;
    }
    *port = (int)n;
    return CONFIG_OK;
}

int main(void) {
    const char *texts[] = {"8080", "x", "70000", NULL};
    for (int i = 0; i < 4; i++) {
        int port = 0;
        enum config_error error = parse_port(texts[i], &port);
        if (error == CONFIG_OK) {
            printf("port %d\n", port);
        } else {
            printf("error %d: %s\n", error, messages[error]);
        }
    }

    FILE *file = fopen("/no/such/dir/settings.conf", "r");
    if (file == NULL) {
        printf("fopen: %s\n", strerror(errno));
    } else {
        fclose(file);
    }
}
```

```output
port 8080
error 2: not a number
error 3: out of range
error 1: missing key
fopen: No such file or directory
```

- Verdict: idiom. C has no error type. A function returns a status code, often `0` or an `enum` for success, and gives the result through a pointer argument.
- Many library functions return a special value, such as `NULL` or `-1`, and store the reason in `errno`. `strerror` turns the number into a message.
- No library function sets `errno` to zero, so the code sets it to zero before `strtol` and tests it after the call.
