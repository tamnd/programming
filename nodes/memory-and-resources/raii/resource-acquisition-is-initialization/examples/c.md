```c run id=goto-cleanup
#include <stdio.h>
#include <stdlib.h>

struct resource {
    const char *name;
};

static struct resource *open_resource(const char *name, int fail) {
    if (fail) {
        printf("open %s failed\n", name);
        return NULL;
    }
    struct resource *r = malloc(sizeof *r);
    if (r != NULL) {
        r->name = name;
        printf("open %s\n", name);
    }
    return r;
}

static void close_resource(struct resource *r) {
    printf("close %s\n", r->name);
    free(r);
}

// Returns 0 on success; every exit path goes through the labels below.
static int copy(int fail_target) {
    int status = -1;
    struct resource *target = NULL;
    struct resource *source = open_resource("source", 0);
    if (source == NULL) {
        goto out;
    }
    target = open_resource("target", fail_target);
    if (target == NULL) {
        goto close_source;
    }
    printf("copy %s -> %s\n", source->name, target->name);
    status = 0;
    close_resource(target);
close_source:
    close_resource(source);
out:
    return status;
}

int main(void) {
    printf("status %d\n", copy(0));
    printf("status %d\n", copy(1));
}
```

```output
open source
open target
copy source -> target
close target
close source
status 0
open source
open target failed
close source
status -1
```

- Verdict: absent. C has no destructors and no statement that runs code on scope exit. Each release is a call that the programmer writes.
- The `goto` cleanup idiom puts the releases at the end of the function, in reverse order of acquisition. A failure jumps to the label that releases only what was acquired.
- `target` is declared at the top and set to `NULL`, so no `goto` jumps past its initialization.
- GCC has `__attribute__((cleanup(f)))`, which calls `f` when a local variable leaves scope. Clang accepts it too. It is an extension, not standard C.
