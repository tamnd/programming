```c run id=to-the-interface
#include <stddef.h>
#include <stdio.h>

/* An interface by hand: a struct of function pointers and a context. */
struct int_collection {
    size_t (*size)(const void *self);
    int (*get)(const void *self, size_t i);
    const void *self;
};

struct array {
    const int *items;
    size_t len;
};

static size_t array_size(const void *self) { return ((const struct array *)self)->len; }
static int array_get(const void *self, size_t i) { return ((const struct array *)self)->items[i]; }

static void describe(const char *name, struct int_collection c) {
    int sum = 0;
    size_t n = c.size(c.self);
    for (size_t i = 0; i < n; i++) {
        sum += c.get(c.self, i);
    }
    printf("%s: size %zu, total %d\n", name, n, sum);
}

int main(void) {
    const int list[] = {1, 2, 3};
    struct array a = {list, 3};
    describe("list", (struct int_collection){array_size, array_get, &a});
    return 0;
}
```

```output for=to-the-interface
list: size 3, total 6
```

- Verdict: absent. C has no interfaces and no standard collections. A library can define its own interface as a struct of function pointers, as this example does, and each collection type fills in the pointers.
