```c run id=byte-index-and-utf8-walk
#include <stdio.h>
#include <string.h>

/* cp_offset returns the byte offset of code point i in the UTF-8 string s,
   or -1 if s has fewer code points. A code point starts at each byte that
   is not a continuation byte (10xxxxxx). */
static long cp_offset(const char *s, size_t i) {
    size_t seen = 0;
    for (size_t b = 0; s[b] != '\0'; b++) {
        if (((unsigned char)s[b] & 0xC0) != 0x80) {
            if (seen == i) return (long)b;
            seen++;
        }
    }
    return -1;
}

int main(void) {
    const char *s = "naïve 😀";

    /* s[i] is a byte. */
    printf("%zu %02X %02X\n", strlen(s), (unsigned char)s[2], (unsigned char)s[3]);

    /* Code point 6 starts at byte 7. */
    long off = cp_offset(s, 6);
    printf("%ld %s\n", off, s + off);
    printf("%ld\n", cp_offset(s, 20));

    /* C does not check an index. s[20] is past the end, so the program must check. */
    size_t i = 20;
    printf("%s\n", i < strlen(s) ? "in range" : "out of range");
    return 0;
}
```

```output for=byte-index-and-utf8-walk
11 C3 AF
7 😀
-1
out of range
```

- Verdict: idiom. `s[2]` and `s[3]` are the bytes `C3 AF`, and `strlen` counts 11 bytes. The function `cp_offset` skips the continuation bytes and finds that code point 6 starts at byte 7, and it returns -1 for a code point past the end. C does not check an index, so the program checks it.
