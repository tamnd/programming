```c run id=types
#include <stdio.h>

int main(void) {
    double price = 7.9;
    int whole = price;
    unsigned int big = -1;
    printf("%d\n", whole);
    printf("%u\n", big);
    printf("%zu %zu %zu\n", sizeof(char), sizeof(int), sizeof(double));
    return 0;
}
```

```output
7
4294967295
1 4 8
```

- Verdict: native. C is statically typed. The compiler fixes the type of each object and expression, and nothing records the type at run time.
- The compiler converts between arithmetic types with no error. A `double` that goes into an `int` loses its fraction, and `-1` that goes into an `unsigned int` becomes the maximum value of that type.
- `sizeof` gives the size of a type in bytes. The sizes of `int` and `double` depend on the platform.

The compiler rejects an assignment between types that have no conversion, such as a struct and an `int`.

```c fails expect=compile id=reject
struct point {
    int x;
    int y;
};

int main(void) {
    struct point p = {1, 2};
    int n = p;
    return n;
}
```
