```c run id=null-terminated-arrays
#include <stdio.h>
#include <string.h>

int main(void) {
    /* A C string is an array of char that ends with a null character. */
    char word[] = "cat";
    printf("%zu %zu\n", strlen(word), sizeof word);

    /* The array is mutable, so one char can change. */
    word[0] = 'b';
    printf("%s\n", word);

    /* strlen counts bytes up to the first null character. */
    char text[] = "ab\0cd";
    printf("%zu %zu\n", strlen(text), sizeof text);

    /* The bytes are UTF-8 when the source and the compiler use UTF-8. */
    const char *s = "café";
    printf("%zu:", strlen(s));
    for (const unsigned char *p = (const unsigned char *)s; *p != '\0'; p++) {
        printf(" %02X", *p);
    }
    printf("\n");

    /* strcpy and strcat need room in the target array. */
    char buf[16];
    strcpy(buf, "cat");
    strcat(buf, "s");
    printf("%s %zu\n", buf, strlen(buf));
    return 0;
}
```

```output for=null-terminated-arrays
3 4
bat
2 6
5: 63 61 66 C3 A9
cats 4
```

- Verdict: idiom. C has no string type. A string is an array of `char` that ends with a null character, so `sizeof word` is 4 and `strlen(word)` is 3. The array can change. `strlen` stops at the first null character, so it gives 2 for `"ab\0cd"`. The bytes of `"café"` are UTF-8. `strcpy` and `strcat` need room in the target array.
