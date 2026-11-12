```c run id=toupper-bytes-and-ascii-bit
#include <ctype.h>
#include <stdio.h>
#include <string.h>

/* ascii_casecmp compares two strings and ignores ASCII case only. */
static int ascii_casecmp(const char *a, const char *b) {
    for (;; a++, b++) {
        int x = tolower((unsigned char)*a), y = tolower((unsigned char)*b);
        if (x != y || x == '\0') return x - y;
    }
}

int main(void) {
    /* toupper works on one byte. In the "C" locale it changes only a to z. */
    char s[] = "straße";
    for (char *p = s; *p; p++) *p = (char)toupper((unsigned char)*p);
    printf("%s %zu\n", s, strlen(s));

    /* In ASCII, upper and lower case differ in one bit, 0x20. */
    printf("%c %c %02X\n", 'a' ^ 0x20, 'A' | 0x20, 'a' ^ 'A');

    /* A caseless compare in C must be written, or taken from POSIX strcasecmp. */
    printf("%d %d\n", ascii_casecmp("Hello", "hELLO") == 0, ascii_casecmp("straße", "STRASSE") == 0);
    return 0;
}
```

```output for=toupper-bytes-and-ascii-bit
STRAßE 7
A a 20
1 0
```

- Verdict: stdlib. `toupper` works on one byte, so the two bytes of `ß` do not change, and the length stays 7 bytes. Upper case and lower case ASCII letters differ in the bit 0x20. The C standard library has no caseless compare, so `ascii_casecmp` compares with `tolower`. It matches `Hello` and `hELLO`, but not `straße` and `STRASSE`.
