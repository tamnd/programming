```c run id=macro-copies
#include <stdio.h>

/* The macro writes one function for each type. */
#define DEFINE_MAX(T)            \
    static T max_##T(T a, T b) { \
        return a > b ? a : b;    \
    }

DEFINE_MAX(int)
DEFINE_MAX(double)

int main(void) {
    printf("%d %.1f\n", max_int(3, 9), max_double(2.5, 1.5));
    return 0;
}
```

```output for=macro-copies
9 2.5
```

- Verdict: idiom. C has no templates. A macro makes a copy of the function for each type, as a C++ template does, but the compiler checks only the code that the macro writes.
