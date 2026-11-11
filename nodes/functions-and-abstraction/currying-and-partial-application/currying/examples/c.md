```c run id=bound
#include <stdio.h>

typedef int (*binary_fn)(int, int);

struct bound {
    binary_fn f;
    int first;
};

static int call_bound(struct bound b, int second) {
    return b.f(b.first, second);
}

static int add(int a, int b) {
    return a + b;
}

int main(void) {
    struct bound add_ten = {add, 10};
    printf("%d\n", call_bound(add_ten, 5));
    return 0;
}
```

```output for=bound
15
```

- Verdict: absent. C has function pointers but no closures, so a function cannot capture an argument. A struct that holds the function pointer and the fixed arguments is the usual replacement.
- Callback APIs in C, such as `pthread_create` and `qsort_r`, take an extra `void *` argument for the same reason.
