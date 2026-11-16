```c run id=members-by-hand
#include <stdio.h>

struct divmod {
    int q;
    int r;
};

static struct divmod divide(int n, int d) {
    struct divmod res = { n / d, n % d };
    return res;
}

int main(void) {
    struct divmod res = divide(17, 5);
    int q = res.q;
    int r = res.r;
    printf("%d %d\n", q, r);

    int a = 1, b = 2;
    int tmp = a;
    a = b;
    b = tmp;
    printf("%d %d\n", a, b);
    return 0;
}
```

```output for=members-by-hand
3 2
2 1
```

- Verdict: absent. C has no destructuring and no parallel assignment. A function returns a struct, and the caller reads each member. A swap needs a temporary variable.
