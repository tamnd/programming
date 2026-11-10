```c run id=records
#include <stdio.h>
#include <string.h>

enum level { LEVEL_DEBUG, LEVEL_INFO, LEVEL_WARN, LEVEL_ERROR };
static const char *level_names[] = {"debug", "info", "warn", "error"};
static enum level min_level = LEVEL_INFO;

// Writes s in double quotes, with backslash escapes for quotes,
// backslashes and control characters, so that s stays on one line.
static void put_quoted(FILE *out, const char *s) {
    fputc('"', out);
    for (; *s != '\0'; s++) {
        unsigned char c = (unsigned char)*s;
        if (c == '"' || c == '\\') {
            fprintf(out, "\\%c", c);
        } else if (c == '\n') {
            fputs("\\n", out);
        } else if (c < 0x20) {
            fprintf(out, "\\x%02x", c);
        } else {
            fputc(c, out);
        }
    }
    fputc('"', out);
}

// Writes one record: the level, the message and one string field.
static void log_kv(enum level level, const char *msg, const char *key, const char *value) {
    if (level < min_level) {
        return;
    }
    printf("level=%s msg=", level_names[level]);
    put_quoted(stdout, msg);
    printf(" %s=", key);
    put_quoted(stdout, value);
    putchar('\n');
}

int main(void) {
    log_kv(LEVEL_DEBUG, "cart loaded", "items", "3"); // below info: dropped
    log_kv(LEVEL_INFO, "order placed", "order", "1042");
    log_kv(LEVEL_WARN, "stock low", "sku", "A-17");
    const char *user = "bob\nlevel=info msg=\"login ok\" user=admin"; // from a request
    log_kv(LEVEL_INFO, "login failed", "user", user);
}
```

```output for=records
level=info msg="order placed" order="1042"
level=warn msg="stock low" sku="A-17"
level=info msg="login failed" user="bob\nlevel=info msg=\"login ok\" user=admin"
```

Without escaping, the user's newline starts a forged record.

```c wrong id=forged
#include <stdio.h>

int main(void) {
    const char *user = "bob\nlevel=info msg=\"login ok\" user=admin"; // from a request
    printf("level=info msg=\"login failed\" user=%s\n", user);
}
```

```output for=forged
level=info msg="login failed" user=bob
level=info msg="login ok" user=admin
```

- Verdict: idiom. ISO C has no logger. A program writes records with `fprintf`. POSIX systems also have `syslog`.
- `put_quoted` escapes quotes, backslashes and control characters, so each value stays inside its quotes and on one line.
- A field of another type must be converted to a string first, so the example passes the numbers as strings.
