```c run id=utf8-decode-and-rule-subset
#include <stdint.h>
#include <stdio.h>
#include <string.h>

/* The C library has no grapheme API. A real program uses ICU (ubrk_open
   with UBRK_CHARACTER). This sketch decodes UTF-8 and applies a small
   subset of the UAX 29 rules, with hand-written ranges for this text. */

static uint32_t decode(const unsigned char *s, size_t *len) {
    if (s[0] < 0x80) { *len = 1; return s[0]; }
    if (s[0] < 0xE0) { *len = 2; return ((s[0] & 0x1Fu) << 6) | (s[1] & 0x3Fu); }
    if (s[0] < 0xF0) { *len = 3; return ((s[0] & 0x0Fu) << 12) | ((s[1] & 0x3Fu) << 6) | (s[2] & 0x3Fu); }
    *len = 4;
    return ((s[0] & 0x07u) << 18) | ((s[1] & 0x3Fu) << 12) | ((s[2] & 0x3Fu) << 6) | (s[3] & 0x3Fu);
}

static int is_extend(uint32_t c) {
    return (c >= 0x300 && c <= 0x36F) || (c >= 0x1F3FB && c <= 0x1F3FF) || c == 0xFE0F;
}

static int is_ri(uint32_t c) { return c >= 0x1F1E6 && c <= 0x1F1FF; }

int main(void) {
    const char *t = "cafe\u0301 \U0001F1EB\U0001F1F7 \U0001F469\U0001F3FD\u200D\U0001F692";
    const unsigned char *s = (const unsigned char *)t;
    size_t n = strlen(t), cps = 0, count = 0, start = 0;
    uint32_t prev = 0;
    int ri_run = 0;

    printf("graphemes (start byte, length):");
    for (size_t i = 0; i < n;) {
        size_t len;
        uint32_t c = decode(s + i, &len);
        int join = i > 0 && (is_extend(c) || c == 0x200D || prev == 0x200D || (is_ri(c) && ri_run % 2 == 1));
        if (!join) {
            if (i > 0) printf(" %zu,%zu", start, i - start);
            start = i;
            count++;
        }
        ri_run = is_ri(c) ? ri_run + 1 : 0;
        prev = c;
        cps++;
        i += len;
    }
    printf(" %zu,%zu\n", start, n - start);
    printf("%zu bytes, %zu code points, %zu graphemes\n", n, cps, count);
    return 0;
}
```

```output for=utf8-decode-and-rule-subset
graphemes (start byte, length): 0,1 1,1 2,1 3,3 6,1 7,8 15,1 16,15
31 bytes, 13 code points, 8 graphemes
```

- Verdict: library. C has no grapheme API, so this sketch decodes UTF-8 by hand and applies a small subset of the rules. A real program uses ICU. The output gives the start byte and the length of each cluster: the accented `e` is 3 bytes, the flag 8 and the emoji 15.
