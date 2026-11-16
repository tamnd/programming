```c run id=static-and-automatic
#include <stdio.h>

static int counter;

struct point {
    int x, y, z;
};

int main(void) {
    static double total;
    int local = 5;
    int values[5] = {1, 2};
    struct point p = {.y = 7};
    printf("%d %.1f %d\n", counter, total, local);
    printf("%d %d %d\n", values[1], values[2], values[4]);
    printf("%d %d %d\n", p.x, p.y, p.z);
    return 0;
}
```

```output for=static-and-automatic
0 0.0 5
2 0 0
0 7 0
```

- Verdict: native. Variables with static storage duration, such as `counter` and `total`, are zero without an initializer. An automatic variable such as `local` needs an initializer: without one, its value is indeterminate. An initializer for an array or a struct that gives some members sets the other members to zero.
