```c run id=printf-family
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

int main(void) {
    double x = 3.14159;
    int width = 8, precision = 2;
    printf("[%*.*f] [%-*s]\n", width, precision, x, 6, "abc");

    size_t n = strlen("hello");
    long long big = 9000000000LL;
    int64_t id = INT64_C(-42);
    printf("%zu %lld %" PRId64 "\n", n, big, id);

    char buf[16];
    int len = snprintf(buf, sizeof buf, "%08.3f", x);
    printf("%s %d\n", buf, len);
    return 0;
}
```

```output for=printf-family
[    3.14] [abc   ]
5 9000000000 -42
0003.142 8
```

- Verdict: stdlib. `printf` and `snprintf` in `<stdio.h>` read the conversion specifications. `*` takes the width or the precision from an argument. The length modifiers must match the type: `%zu` for `size_t`, `%lld` for `long long`, and the macros of `<inttypes.h>`, such as `PRId64`, for the fixed-width types. A wrong type is undefined behavior.
