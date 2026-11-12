```c run id=terminator-and-length-prefix
#include <stdio.h>
#include <string.h>

/* A length-prefixed string, as in Pascal: the length comes first. */
struct pstring {
    unsigned char len;
    char data[255];
};

int main(void) {
    /* A null-terminated string: the bytes, then a byte with value zero. */
    char c_str[] = "hello";
    printf("strlen=%zu sizeof=%zu last=%d\n", strlen(c_str), sizeof c_str, c_str[5]);

    /* strlen must scan to the null byte. A stored length is read at once. */
    struct pstring p = {5, "hello"};
    printf("pstring len=%d max=%zu\n", p.len, sizeof p.data);

    /* A null byte inside the data ends a C string early. */
    char data[] = {'a', 'b', '\0', 'c', 'd', '\0'};
    printf("strlen=%zu sizeof=%zu\n", strlen(data), sizeof data);

    /* A pointer and a length make a slice without a null byte. */
    const char *text = "hello, world";
    const char *start = text + 7;
    int n = 5;
    printf("%.*s\n", n, start);
    return 0;
}
```

```output for=terminator-and-length-prefix
strlen=5 sizeof=6 last=0
pstring len=5 max=255
strlen=2 sizeof=6
world
```

- Verdict: idiom. The array `"hello"` takes 6 bytes, because the null byte follows the 5 characters. A length-prefixed structure keeps the length in its first byte, so a program reads it at once but the string can have at most 255 characters. A null byte inside the data makes `strlen` stop at 2. A pointer and a length give a slice, and `%.*s` prints it without a null byte.
