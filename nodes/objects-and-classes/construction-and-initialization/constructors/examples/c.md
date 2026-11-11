```c run id=init-and-create
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct range {
    int lo, hi;
};

/* An init function: the caller owns the memory, the function checks the values. */
static int range_init(struct range *r, int lo, int hi)
{
    if (lo > hi)
        return -1;
    r->lo = lo;
    r->hi = hi;
    return 0;
}

struct buffer {
    size_t len, cap;
    char *data;
};

/* A create function: allocates and initializes, or returns NULL. */
static struct buffer *buffer_create(size_t cap)
{
    struct buffer *b = malloc(sizeof *b);
    if (b == NULL)
        return NULL;
    b->data = malloc(cap);
    if (b->data == NULL) {
        free(b); /* undo the part that succeeded */
        return NULL;
    }
    b->len = 0;
    b->cap = cap;
    return b;
}

static void buffer_destroy(struct buffer *b)
{
    if (b != NULL)
        free(b->data);
    free(b);
}

struct config {
    const char *name;
    int retries;
    int verbose;
};

int main(void)
{
    struct range r;
    printf("%d ", range_init(&r, 3, 7));
    printf("%d\n", range_init(&r, 9, 1));

    struct config c = {.retries = 3}; /* designated: the other members are zero */
    printf("%s %d %d\n", c.name == NULL ? "(null)" : c.name, c.retries, c.verbose);

    struct buffer *b = buffer_create(16);
    memcpy(b->data, "hi", 3);
    b->len = 2;
    printf("%s %zu %zu\n", b->data, b->len, b->cap);
    buffer_destroy(b);
    return 0;
}
```

```output for=init-and-create
0 -1
(null) 3 0
hi 2 16
```

- Verdict: idiom. C has no constructors. Two conventions replace them. An init function takes a pointer to memory that the caller owns, sets the fields and returns an error code. A create function allocates the memory too and returns a pointer or `NULL`, and a matching destroy function frees it. A create function that fails half way must free what it has already allocated. A designated initializer sets named members and makes all the others zero, which is the closest C comes to default values. A local variable without an initializer has an indeterminate value, and reading it is undefined behavior.
