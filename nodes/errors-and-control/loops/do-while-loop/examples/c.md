```c run id=do-while-zero-macro-and-copy
#include <stdio.h>

/* do { } while (0) makes the macro act as one statement. */
#define SWAP(a, b) do { int tmp_ = (a); (a) = (b); (b) = tmp_; } while (0)

int main(void) {
    int x = 1, y = 2;
    if (x < y)
        SWAP(x, y);
    else
        printf("no swap\n");
    printf("x=%d y=%d\n", x, y);

    /* Copy a string with its null character: the body runs before the test. */
    const char *src = "loop";
    char dst[8];
    int k = 0;
    do {
        dst[k] = src[k];
    } while (src[k++] != '\0');
    printf("copied \"%s\" with %d bytes\n", dst, k);
    return 0;
}
```

```output for=do-while-zero-macro-and-copy
x=2 y=1
copied "loop" with 5 bytes
```

- Verdict: native. `do statement while (expression);` tests after the body. The `SWAP` macro is wrapped in `do { } while (0)`, so `SWAP(x, y);` with its semicolon is one statement and the `else` still belongs to the `if`. The copy loop copies the null character too, because the test comes after the copy.
