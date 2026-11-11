```c run id=thunk
#include <stdbool.h>
#include <stdio.h>

struct lazy_int {
    bool ready;
    int value;
    int (*compute)(void);
};

static int expensive(void) {
    puts("computing");
    return 42;
}

static int force(struct lazy_int *lazy) {
    if (!lazy->ready) {
        lazy->value = lazy->compute();
        lazy->ready = true;
    }
    return lazy->value;
}

int main(void) {
    struct lazy_int answer = {.compute = expensive};
    puts("nothing computed yet");
    printf("%d\n", force(&answer));
    printf("%d\n", force(&answer));
    return 0;
}
```

```output for=thunk
nothing computed yet
computing
42
42
```

- Verdict: absent. C evaluates eagerly. A struct with a function pointer and a flag is a memoized thunk, and the caller forces it.
- `&&`, `||` and `?:` are the lazy operators of C.
