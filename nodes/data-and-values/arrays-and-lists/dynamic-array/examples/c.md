```c run id=realloc-vector
#include <stdio.h>
#include <stdlib.h>

/* A dynamic array: a pointer to the items, the number of items and the capacity. */
typedef struct {
    int *items;
    size_t len;
    size_t cap;
} IntVec;

static int push(IntVec *v, int x, int *grows) {
    if (v->len == v->cap) {
        size_t new_cap = v->cap ? v->cap * 2 : 4;
        int *p = realloc(v->items, new_cap * sizeof *p);
        if (p == NULL) {
            return -1; /* the old block is still valid */
        }
        v->items = p;
        v->cap = new_cap;
        (*grows)++;
    }
    v->items[v->len++] = x;
    return 0;
}

int main(void) {
    IntVec v = {NULL, 0, 0};
    int grows = 0;
    for (int i = 0; i < 1000; i++) {
        if (push(&v, i * i, &grows) != 0) {
            free(v.items);
            return 1;
        }
    }
    printf("%zu %zu %d %d\n", v.len, v.cap, grows, v.items[999]);
    free(v.items);
    return 0;
}
```

```output
1000 1024 9 998001
```

- Verdict: idiom. The C library has no dynamic array. A struct keeps a pointer, a length and a capacity, and push doubles the capacity with realloc. 1000 pushes grow the array 9 times, from 4 slots to 1024. The result of realloc goes into a second variable, so a failure does not lose the old block.
