```c run id=context-pointers-own-and-shared
#include <stdio.h>

struct job {
    int n;
};

typedef void (*callback)(void *ctx);

static void show(void *ctx) {
    printf("job %d\n", ((struct job *)ctx)->n);
}

int main(void) {
    /* Each callback gets its own context. */
    struct job jobs[3];
    void *ctxs[3];
    for (int i = 0; i < 3; i++) {
        jobs[i].n = i;
        ctxs[i] = &jobs[i];
    }
    callback cb = show;
    for (int i = 0; i < 3; i++)
        cb(ctxs[i]);

    /* The trap: one shared context for all callbacks. */
    struct job shared;
    void *saved[3];
    for (int i = 0; i < 3; i++) {
        shared.n = i;
        saved[i] = &shared;
    }
    for (int i = 0; i < 3; i++)
        cb(saved[i]);
    return 0;
}
```

```output for=context-pointers-own-and-shared
job 0
job 1
job 2
job 2
job 2
job 2
```

- Verdict: absent. C has no closures, so a callback gets its data through a context pointer. The first loop gives each callback its own `struct job`. The second loop stores the address of one shared struct, so all three callbacks see the last value, which is the same trap as a closure over a shared loop variable.
