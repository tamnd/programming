```c run id=struct-with-operations-table
#include <stdio.h>

struct counter;

struct counter_ops {
    void (*inc)(struct counter *self, int by);
    int (*value)(const struct counter *self);
};

struct counter {
    const struct counter_ops *ops; /* behavior, shared by all counters */
    int n;                         /* state, one copy per counter */
};

static void inc(struct counter *self, int by) { self->n += by; }
static int value(const struct counter *self) { return self->n; }

static const struct counter_ops counter_ops = {inc, value};

int main(void) {
    struct counter a = {&counter_ops, 0};
    struct counter b = {&counter_ops, 10};
    a.ops->inc(&a, 5); /* send the message inc to a */
    printf("%d %d\n", a.ops->value(&a), b.ops->value(&b));

    struct counter *p = &a; /* a second way to reach a */
    p->ops->inc(p, 1);
    printf("%d %d\n", a.n, p == &a);

    int x = 7; /* in C, x is also an object: a region of storage */
    printf("%zu %d\n", sizeof x, x);
    return 0;
}
```

```output for=struct-with-operations-table
5 10
6 1
4 7
```

- Verdict: idiom. The C standard calls any region of storage that holds a value an object, so `x` is an object with no behavior. Objects in the sense of this page are written by hand: a struct holds the state, and a pointer to a shared table of function pointers holds the behavior. A call through `a.ops->inc(&a, 5)` sends a message and passes the receiver explicitly. The Linux kernel, GTK and many C libraries use this pattern.
