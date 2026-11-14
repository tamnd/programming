```c run id=fgets
#define _POSIX_C_SOURCE 200809L /* mkstemp and fdopen */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* show prints a string with CR and LF written as \r and \n. */
static void show(const char *s) {
    putchar('"');
    for (; *s; s++) {
        if (*s == '\r') fputs("\\r", stdout);
        else if (*s == '\n') fputs("\\n", stdout);
        else putchar(*s);
    }
    puts("\"");
}

int main(void) {
    char path[] = "/tmp/eol-XXXXXX";
    FILE *f = fdopen(mkstemp(path), "wb");
    fputs("one\ntwo\r\nthree", f);
    fclose(f);

    /* fgets keeps the LF. On POSIX, text mode does not change CR or LF. */
    char buf[64];
    f = fopen(path, "r");
    while (fgets(buf, sizeof buf, f) != NULL) {
        show(buf);
        buf[strcspn(buf, "\r\n")] = '\0'; /* remove the line ending */
        show(buf);
    }
    fclose(f);
    remove(path);
    return 0;
}
```

```output for=fgets
"one\n"
"one"
"two\r\n"
"two"
"three"
"three"
```

- Verdict: stdlib. `fgets` keeps the LF at the end of the line. On Linux, the CR of a CRLF file stays in the text, as the output shows, and `strcspn` with `"\r\n"` finds the end of the text. C streams on Windows convert CRLF to LF on input.
