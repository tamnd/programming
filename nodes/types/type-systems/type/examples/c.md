```c run id=generic-selection-and-bits
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define TYPE_NAME(x) _Generic((x), \
    char: "char", int: "int", unsigned int: "unsigned int", long: "long", \
    float: "float", double: "double", bool: "bool", default: "other")

typedef double celsius; /* typedef makes a new name, not a new type */

int main(void) {
    /* The type decides the size and the range. */
    printf("%zu %zu %zu %zu\n", sizeof(char), sizeof(int32_t), sizeof(int64_t), sizeof(double));

    /* The type of an expression follows from the types of its operands. */
    char c = 'A';
    printf("%s %s %s %s\n", TYPE_NAME(c), TYPE_NAME(c + 1), TYPE_NAME(1u + 1), TYPE_NAME(1.0f * 2));

    /* A typedef name and its type are the same type. */
    celsius t = 21.5;
    double d = t;
    printf("%s %.1f\n", TYPE_NAME(t), d);

    /* The same bits mean different values for different types. */
    float f = 1.0f;
    uint32_t bits;
    memcpy(&bits, &f, sizeof bits);
    printf("0x%08X %u\n", (unsigned)bits, (unsigned)bits);

    /* bool is an integer type, and 2 converts to true. */
    bool b = 2;
    printf("%s %d\n", TYPE_NAME(b), b);
    return 0;
}
```

```output for=generic-selection-and-bits
1 4 8 8
char int unsigned int float
double 21.5
0x3F800000 1065353216
bool 1
```

- Verdict: native. `_Generic` selects a string by the type of an expression, which shows that `c + 1` has type `int` and `1u + 1` has type `unsigned int`. A `typedef` name is the same type as `double`. `memcpy` copies the bits of the `float` 1.0 into an integer, which reads them as `0x3F800000`. `bool` is an integer type, and the value 2 becomes 1.
