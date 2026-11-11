```c run id=destroy-functions-and-atexit
#include <stdio.h>
#include <stdlib.h>

struct conn {
    const char *name;
};

static struct conn *conn_open(const char *name)
{
    struct conn *c = malloc(sizeof *c);
    if (c != NULL) {
        c->name = name;
        printf("open %s\n", name);
    }
    return c;
}

/* The destroy function is the destructor; the caller must call it once. */
static void conn_close(struct conn *c)
{
    if (c == NULL)
        return;
    printf("close %s\n", c->name);
    free(c);
}

static void flush_logs(void) { puts("atexit: flush logs"); }
static void remove_temp(void) { puts("atexit: remove temp files"); }

int main(void)
{
    atexit(flush_logs);
    atexit(remove_temp); /* handlers run in reverse order of registration */

    struct conn *a = conn_open("a");
    struct conn *b = conn_open("b");
    conn_close(b); /* close in reverse order of opening */
    b = NULL;      /* a dangling pointer must not be used again */
    conn_close(a);
    puts("end of main");
    return 0;
}
```

```output for=destroy-functions-and-atexit
open a
open b
close b
close a
end of main
atexit: remove temp files
atexit: flush logs
```

- Verdict: idiom. C has no destructors and no finalizers. Each type that owns memory or a resource comes with a destroy function, such as `conn_close`, and the caller must call it exactly once, in the reverse order of creation when one object depends on another. A pointer to a destroyed object must not be used again, and setting it to `NULL` makes a later mistake easier to find. `atexit` registers functions that run when the program exits normally, in the reverse order of registration.
