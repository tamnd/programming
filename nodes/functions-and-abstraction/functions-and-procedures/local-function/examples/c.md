```c run id=local-function-c
#include <stdio.h>

static int scale(int x, int factor) {
    return x * factor;
}

static int sum_of_scaled(const int *values, int n, int factor) {
    int total = 0;
    for (int i = 0; i < n; i++) {
        total += scale(values[i], factor);
    }
    return total;
}

int main(void) {
    int a[] = {1, 2, 3};
    int b[] = {4};
    printf("%d\n", sum_of_scaled(a, 3, 10));
    printf("%d\n", sum_of_scaled(b, 1, 2));
    return 0;
}
```

```output for=local-function-c
60
8
```

- Verdict: absent. Standard C has no nested functions. A `static` function at file scope is private to the file, and it gets `factor` as a parameter.
