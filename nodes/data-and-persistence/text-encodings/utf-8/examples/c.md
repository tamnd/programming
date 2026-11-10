```c run id=validate
#include <stdio.h>
#include <string.h>

/* Returns the length of the longest valid prefix, as in Table 3-7. */
static size_t valid_up_to(const unsigned char *s, size_t n, size_t *code_points) {
    size_t i = 0;
    *code_points = 0;
    while (i < n) {
        unsigned char b = s[i];
        size_t length;
        unsigned char low = 0x80, high = 0xBF;
        if (b <= 0x7F) {
            length = 1;
        } else if (b >= 0xC2 && b <= 0xDF) {
            length = 2;
        } else if (b >= 0xE0 && b <= 0xEF) {
            length = 3;
            if (b == 0xE0) low = 0xA0;
            if (b == 0xED) high = 0x9F;
        } else if (b >= 0xF0 && b <= 0xF4) {
            length = 4;
            if (b == 0xF0) low = 0x90;
            if (b == 0xF4) high = 0x8F;
        } else {
            return i;
        }
        if (i + length > n) return i;
        for (size_t k = 1; k < length; k++) {
            unsigned char c = s[i + k];
            if (k == 1 ? (c < low || c > high) : (c < 0x80 || c > 0xBF)) return i;
        }
        i += length;
        ++*code_points;
    }
    return i;
}

int main(void) {
    const char *samples[] = {
        "h\xC3\xA9\xE2\x82\xAC\xF0\x9D\x84\x9E",
        "/\xC0\xAF",
        "\xE0\x9F\x80",
        "\xED\xA0\x80",
        "\xF4\x80\x83\x92",
        "\xF4\x90\x80\x80",
        "ok\xE2\x82",
    };
    for (size_t i = 0; i < sizeof samples / sizeof samples[0]; i++) {
        size_t n = strlen(samples[i]);
        size_t count;
        size_t valid = valid_up_to((const unsigned char *)samples[i], n, &count);
        printf("bytes %zu valid %zu code points %zu %s\n", n, valid, count, valid == n ? "ok" : "ill-formed");
    }
    return 0;
}
```

```output for=validate
bytes 10 valid 10 code points 4 ok
bytes 3 valid 1 code points 1 ill-formed
bytes 3 valid 0 code points 0 ill-formed
bytes 3 valid 0 code points 0 ill-formed
bytes 4 valid 4 code points 1 ok
bytes 4 valid 0 code points 0 ill-formed
bytes 4 valid 2 code points 2 ill-formed
```

- Verdict: idiom. ISO C strings are arrays of `char` that end with a NUL byte. The standard library has no function that validates UTF-8 or counts its code points, so the program does it by hand.
- `strlen` counts bytes, not characters. The first sample has four characters in 10 bytes.
- The function `valid_up_to` follows Table 3-7 of the Unicode Standard. It returns the length of the longest valid prefix, like `valid_up_to` in Rust. The samples `E0 9F 80` and `F4 80 83 92` are the examples of the Unicode Standard: the first is ill-formed and the second is well-formed.
- UTF-8 never uses the byte 00 for a character other than U+0000, so the C string functions work on UTF-8 text. They count and compare bytes, not characters.
- The functions of `<wchar.h>` and `<uchar.h>`, such as `mbrtowc`, depend on the locale. Libraries such as ICU and simdutf do validation and conversion without a locale.
