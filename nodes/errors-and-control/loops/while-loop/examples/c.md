```c run id=pointer-walk-and-euclid
#include <stdio.h>

int main(void) {
    /* Walk a string with a pointer until the null character. */
    const char *s = "hello, world";
    const char *p = s;
    int commas = 0;
    while (*p != '\0') {
        if (*p == ',') {
            commas++;
        }
        p++;
    }
    printf("length %d, commas %d\n", (int)(p - s), commas);

    /* Euclid's algorithm. */
    int a = 1071, b = 462;
    while (b != 0) {
        int t = a % b;
        a = b;
        b = t;
    }
    printf("gcd: %d\n", a);

    /* A condition that is false at the start: zero passes. */
    int n = 0;
    while (n > 0) {
        n--;
    }
    printf("n: %d\n", n);
    return 0;
}
```

```output for=pointer-walk-and-euclid
length 12, commas 1
gcd: 21
n: 0
```

- Verdict: native. The first loop runs until the pointer reaches the null character that ends a C string, so the number of passes is the length of the string. Euclid's algorithm runs while the remainder is not 0. The last loop runs zero times.
