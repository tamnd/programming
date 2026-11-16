```c run id=incomplete-struct
#include <stdio.h>
#include <stdlib.h>

/* The header: the struct has no members here, so it is incomplete. */
struct counter;
struct counter *counter_create(int start);
void counter_add(struct counter *c, int n);
int counter_value(const struct counter *c);
void counter_destroy(struct counter *c);

/* The user code: it can hold pointers, but not a struct counter. */
static void user(void) {
    struct counter *c = counter_create(10);
    counter_add(c, 5);
    printf("%d\n", counter_value(c));
    /* struct counter x;  error: storage size of x is not known */
    /* sizeof(struct counter)  error: incomplete type */
    counter_destroy(c);
}

/* The implementation: this part completes the type. */
struct counter {
    int value;
    int adds;
};

struct counter *counter_create(int start) {
    struct counter *c = malloc(sizeof *c);
    if (c == NULL) {
        exit(1);
    }
    c->value = start;
    c->adds = 0;
    return c;
}

void counter_add(struct counter *c, int n) {
    c->value += n;
    c->adds++;
}

int counter_value(const struct counter *c) { return c->value; }

void counter_destroy(struct counter *c) { free(c); }

int main(void) {
    user();
    printf("%zu\n", sizeof(struct counter));
    return 0;
}
```

```output for=incomplete-struct
15
8
```

- Verdict: native. In a real program the first part is a header and the last part is a separate source file. Before the definition, `struct counter` is an incomplete type, so `user` can pass pointers to it but cannot declare one, read its fields or take its size. The module allocates each counter and gives a destroy function.
