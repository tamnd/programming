```c run id=implicit-conversions-and-unions
#include <stdio.h>
#include <stdlib.h>

union Number {
    float f;
    unsigned u;
};

int main(void) {
    /* C converts between arithmetic types without a cast. */
    double d = 3.99;
    int big = 300;
    int i = d;
    char c = big;
    unsigned u = -1;
    printf("%d %d %u\n", i, c, u);

    /* void * converts to any object pointer type without a cast. */
    void *p = malloc(sizeof(int));
    int *ip = p;
    *ip = 42;
    printf("%d\n", *ip);
    free(p);

    /* A union lets the program read the bits of one type as another. */
    union Number n = {.f = 1.0f};
    printf("0x%X\n", n.u);

    /* An enum value is an int, and an int converts to an enum. */
    enum Color { RED, GREEN, BLUE } color = 7;
    printf("%d %d\n", color, GREEN + BLUE);
    return 0;
}
```

```output for=implicit-conversions-and-unions
3 44 4294967295
42
0x3F800000
7 3
```

- Verdict: native. C converts between arithmetic types without a cast, so `3.99` becomes `3`, `300` becomes `44` in a `char`, and `-1` becomes the largest `unsigned`. `void *` converts to any object pointer type. A union reads the bits of a `float` as an `unsigned`. An enum variable accepts the value 7, which is not one of its constants.
