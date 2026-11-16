```c run id=propagate-c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Each function returns 0 for success and -1 for an error, with a message in msg. */
static int parse_port(const char *text, int *port, char *msg, size_t n) {
    if (*text == '\0' || strspn(text, "0123456789") != strlen(text)) {
        snprintf(msg, n, "not a number: %s", text);
        return -1;
    }
    *port = atoi(text);
    return 0;
}

static int load_config(const char *line, int *port, char *msg, size_t n) {
    const char *eq = strchr(line, '=');
    char inner[100];
    if (parse_port(eq ? eq + 1 : "", port, inner, sizeof inner) != 0) {
        snprintf(msg, n, "config line '%s': %s", line, inner);
        return -1;
    }
    return 0;
}

int main(void) {
    const char *lines[] = {"port=8080", "port=x"};
    for (int i = 0; i < 2; i++) {
        int port = 0;
        char msg[200];
        if (load_config(lines[i], &port, msg, sizeof msg) != 0) {
            printf("error: %s\n", msg);
            continue;
        }
        printf("port %d\n", port);
    }
    return 0;
}
```

```output for=propagate-c
port 8080
error: config line 'port=x': not a number: x
```

- Verdict: absent. C has no mechanism that carries an error up the stack. Each function tests the result of each call and returns its own code, and the message is passed in a buffer.
