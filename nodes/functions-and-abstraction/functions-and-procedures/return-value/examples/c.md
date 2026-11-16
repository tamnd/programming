```c run id=return-value-c
#include <stdio.h>

static int absolute(int n) {
    if (n < 0) {
        return -n;
    }
    return n;
}

int main(void) {
    printf("%d\n", absolute(-7));
    printf("%d\n", absolute(5) + 1);
    return 0;
}
```

```output for=return-value-c
7
6
```

- Verdict: native. `return` ends the function and gives the value. The compiler does not force a `return` on each path.
