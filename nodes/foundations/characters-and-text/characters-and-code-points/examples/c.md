```c run id=decode-utf8-by-hand
#include <stdio.h>
#include <string.h>

/* Count code points: every byte that is not a continuation byte (10xxxxxx) starts one. */
static size_t count_code_points(const char *s) {
    size_t n = 0;
    for (; *s; s++)
        if (((unsigned char)*s & 0xC0) != 0x80) n++;
    return n;
}

/* Decode the code point that starts at s and store its length in *len. */
static unsigned decode(const unsigned char *s, int *len) {
    if (s[0] < 0x80) { *len = 1; return s[0]; }
    if (s[0] < 0xE0) { *len = 2; return (s[0] & 0x1Fu) << 6 | (s[1] & 0x3Fu); }
    if (s[0] < 0xF0) { *len = 3; return (s[0] & 0x0Fu) << 12 | (s[1] & 0x3Fu) << 6 | (s[2] & 0x3Fu); }
    *len = 4;
    return (s[0] & 0x07u) << 18 | (s[1] & 0x3Fu) << 12 | (s[2] & 0x3Fu) << 6 | (s[3] & 0x3Fu);
}

int main(void) {
    const char *s = "cafe\xCC\x81 \xF0\x9F\x91\x8D\xF0\x9F\x8F\xBD"; /* UTF-8 bytes */
    printf("%zu %zu\n", strlen(s), count_code_points(s));

    const char *sep = "";
    for (const unsigned char *p = (const unsigned char *)s; *p;) {
        int len;
        unsigned cp = decode(p, &len);
        if (cp > 127) {
            printf("%sU+%04X/%d", sep, cp, len);
            sep = " ";
        }
        p += len;
    }
    printf("\n%d\n", (int)sizeof(char));
    return 0;
}
```

```output for=decode-utf8-by-hand
15 8
U+0301/2 U+1F44D/4 U+1F3FD/4
1
```

- Verdict: idiom. In C, `char` is one byte and a string is an array of bytes ending in a zero byte, so `strlen` counts bytes. The standard library has wide characters and multibyte conversions that depend on the locale, but no portable UTF-8 functions. Programs decode UTF-8 by hand, as the example does, or use a library such as ICU or utf8proc.
- Every byte of a UTF-8 sequence after the first has the form `10xxxxxx`, so counting the other bytes counts code points. The example trusts its input; a real decoder must reject truncated sequences, overlong forms and surrogates.
