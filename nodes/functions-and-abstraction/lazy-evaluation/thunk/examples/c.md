```c run id=thunks
#include <stdbool.h>
#include <stdio.h>

struct thunk {
    int (*compute)(const void *context);
    const void *context;
    bool done;
    int value;
};

static int sum(const void *context) {
    const int *items = context;
    puts("summing");
    return items[0] + items[1] + items[2];
}

static int force(struct thunk *t) {
    if (!t->done) {
        t->value = t->compute(t->context);
        t->done = true;
    }
    return t->value;
}

int main(void) {
    static const int items[] = {1, 2, 3};
    struct thunk total = {.compute = sum, .context = items};
    puts("thunk created");
    printf("%d\n", force(&total));
    printf("%d\n", force(&total));
    return 0;
}
```

```output for=thunks
thunk created
summing
6
6
```

- Verdict: idiom. C has no closures, so a thunk is a function pointer plus a pointer to the data that the function needs. The `done` flag and `value` field make it a memoized thunk.
- The context pointer must stay valid until the force. Here `items` is static, so it lives for the whole program.
