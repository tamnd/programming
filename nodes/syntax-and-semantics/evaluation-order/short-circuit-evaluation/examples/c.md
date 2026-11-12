```c run id=null-pointer-guard
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

static int calls = 0;

static bool check(bool v) {
    calls++;
    return v;
}

struct item {
    const char *name;
    int qty;
};

static const struct item *find(const struct item *xs, int n, const char *name) {
    for (int i = 0; i < n; i++)
        if (strcmp(xs[i].name, name) == 0)
            return &xs[i];
    return NULL;
}

int main(void) {
    /* && and || have a sequence point after the left operand, and skip the right one when they can. */
    bool r = check(false) && check(true);
    printf("%d calls=%d\n", r, calls);
    calls = 0;
    r = (int)check(false) & (int)check(true); /* & evaluates both, in an unspecified order */
    printf("%d calls=%d\n", r, calls);

    /* The guard pattern: the pointer is tested before it is used. */
    struct item stock[] = {{"pen", 4}, {"ink", 0}};
    const char *names[] = {"pen", "ink", "cap"};
    for (int i = 0; i < 3; i++) {
        const struct item *p = find(stock, 2, names[i]);
        printf("%s: %s\n", names[i], p != NULL && p->qty > 0 ? "in stock" : "none");
    }

    /* A bounds test before an index works the same way. */
    int xs[3] = {5, 6, 7}, k = 3;
    printf("%d\n", k < 3 && xs[k] > 0);
    return 0;
}
```

```output for=null-pointer-guard
0 calls=1
0 calls=2
pen: in stock
ink: none
cap: none
0
```

- Verdict: native. `&&` and `||` have a sequence point after the left operand and skip the right one when they can. `&` evaluates both operands in an unspecified order, and clang warns about `&` on `bool` values, so the example casts to `int`. The test `p != NULL && p->qty > 0` reads `qty` only for an item that exists, and `k < 3 && xs[k] > 0` reads the array only inside its bounds.
