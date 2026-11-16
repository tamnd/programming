```c run id=static-local
#include <stdio.h>

static int next_id(void) {
    static int id = 0;
    id++;
    return id;
}

static int fresh_count(void) {
    int n = 0;
    n++;
    return n;
}

int main(void) {
    int first = next_id();
    int second = next_id();
    int third = next_id();
    printf("%d %d %d\n", first, second, third);
    int x = fresh_count();
    int y = fresh_count();
    printf("%d %d\n", x, y);
    return 0;
}
```

```output for=static-local
1 2 3
1 1
```

- Verdict: native. `id` has static storage duration: it is initialized once and keeps its value from call to call, though only `next_id` can see it. `n` has automatic storage duration: each call makes a new `n` that starts at 0 and ends when the call returns.
