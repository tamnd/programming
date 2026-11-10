```c run id=countdown
#include <stdbool.h>
#include <stdio.h>

struct countdown {
    int current;
};

static bool countdown_next(struct countdown *it, int *out) {
    if (it->current <= 0) {
        return false;
    }
    *out = it->current;
    it->current--;
    return true;
}

int main(void) {
    struct countdown it = {3};
    int n;
    while (countdown_next(&it, &n)) {
        printf("%d\n", n);
    }

    const char *words[] = {"a", "b", NULL};
    for (const char **p = words; *p != NULL; p++) {
        printf("%s\n", *p);
    }
    return 0;
}
```

```output for=countdown
3
2
1
a
b
```

- Verdict: idiom. C has no iterator protocol and no loop that calls one. The usual form is a struct that holds the position and a `next` function that returns `false` at the end and writes the element through a pointer.
- A pointer is the iterator of an array. The second loop walks an array of strings that ends with `NULL`.
- The caller owns the struct, so the iterator needs no heap allocation. Nothing checks that the data under the iterator stays valid.
