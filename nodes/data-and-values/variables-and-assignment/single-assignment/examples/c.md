```c run id=const-locals
#include <stdio.h>

static int square(int n) {
    return n * n;
}

int main(void) {
    const int base = 10;
    const int doubled = base * 2;
    const int area = square(doubled);
    const char *const name = "Ada";
    printf("%d %d %d %s\n", base, doubled, area, name);
    return 0;
}
```

```output for=const-locals
10 20 400 Ada
```

- Verdict: idiom. A local variable with `const` must get its value in the initializer, and the compiler rejects a later assignment. The value can come from a call at run time, as `area` does. For a pointer, `const char *const` makes both the pointer and the characters constant.
