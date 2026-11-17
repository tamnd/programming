```c run id=void-list
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* A list of items of any one size: the code does not know the type. */
struct list {
    void *data;
    size_t size, len, cap;
};

static void push(struct list *l, const void *item) {
    if (l->len == l->cap) {
        l->cap = l->cap ? l->cap * 2 : 4;
        l->data = realloc(l->data, l->cap * l->size);
    }
    memcpy((char *)l->data + l->len * l->size, item, l->size);
    l->len++;
}

int main(void) {
    struct list ints = {NULL, sizeof(int), 0, 0};
    for (int i = 1; i <= 5; i++) {
        push(&ints, &i);
    }
    int *p = ints.data; /* the caller knows the type */
    printf("%zu %d %d\n", ints.len, p[0], p[4]);

    struct list doubles = {NULL, sizeof(double), 0, 0};
    double d = 2.5;
    push(&doubles, &d);
    printf("%.1f\n", ((double *)doubles.data)[0]);

    free(ints.data);
    free(doubles.data);
    return 0;
}
```

```output for=void-list
5 1 5
2.5
```

- Verdict: idiom. C has no generic types. The list stores bytes through a `void *` and the size of an item, and the caller converts the pointer to the item type. The compiler cannot check that the caller uses the right type.
