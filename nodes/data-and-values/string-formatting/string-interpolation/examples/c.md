```c run id=snprintf
#include <stdio.h>

int main(void) {
    const char *name = "Ada";
    double price = 4.5;
    int count = 2;

    char buf[64];
    int n = snprintf(buf, sizeof buf, "%s pays %.2f for %d items.", name, price * 2, count);
    printf("%s (%d characters)\n", buf, n);

    char small[8];
    n = snprintf(small, sizeof small, "%s pays %.2f", name, price * 2);
    printf("[%s] needs %d characters\n", small, n);
    return 0;
}
```

```output for=snprintf
Ada pays 9.00 for 2 items. (26 characters)
[Ada pay] needs 13 characters
```

- Verdict: absent. C has no string interpolation. `snprintf` writes the formatted text into a buffer of a given size, and it returns the length of the full text. If the buffer is too small, the text is cut, and the return value shows the size that the program needs.
