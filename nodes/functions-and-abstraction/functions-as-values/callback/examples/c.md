```c run id=callback-c
#include <stdio.h>
#include <stdlib.h>

static int descending(const void *a, const void *b) {
    int x = *(const int *)a;
    int y = *(const int *)b;
    return (y > x) - (y < x);
}

static void repeat(int n, void (*callback)(int)) {
    for (int i = 1; i <= n; i++) {
        callback(i);
    }
}

static void tick(int i) {
    printf("tick %d\n", i);
}

int main(void) {
    int xs[] = {3, 1, 2};
    qsort(xs, 3, sizeof xs[0], descending);
    printf("%d %d %d\n", xs[0], xs[1], xs[2]);
    repeat(2, tick);
    return 0;
}
```

```output for=callback-c
3 2 1
tick 1
tick 2
```

- Verdict: native. A C callback is a function pointer. `qsort` calls `descending` to compare two items, and `repeat` calls `tick` through the parameter `callback`.
