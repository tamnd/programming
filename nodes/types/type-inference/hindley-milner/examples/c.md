```c run flags="-std=c23" id=auto
#include <stdio.h>

int main(void) {
    auto x = 3.5;
    auto n = 7 / 2;
    double a[3] = {1, 2, 3};
    auto p = a;
    auto q = &a;
    printf("%g %d\n", x, n);
    printf("%zu %zu %zu\n", sizeof x, sizeof p, sizeof *q);
    printf("%d\n", _Generic(n, int: 1, default: 0));
}
```

```output for=auto
3.5 3
8 8 24
1
```

- Verdict: native. C23 infers the type of an object from its initializer when the declaration has the storage-class specifier `auto`.
- The type comes after array to pointer conversion. `p` is a `double *`, but `q` is a pointer to the whole array of three `double` values.
- `7 / 2` has the type `int`, so `n` is an `int` and holds 3.
- The program needs `-std=c23`.
