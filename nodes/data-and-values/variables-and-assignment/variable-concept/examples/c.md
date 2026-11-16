```c run id=object
#include <stdio.h>

int main(void) {
    int n = 41;
    int *p = &n;
    *p = *p + 1;
    printf("%d %zu\n", n, sizeof n);
    return 0;
}
```

```output for=object
42 4
```

- Verdict: native. A C variable is an object: a region of storage with a type. `&n` gives the address of the storage, and a write through the pointer changes `n`. `sizeof n` gives the size of the storage in bytes: 4 for an `int` on the machine that ran this example.
