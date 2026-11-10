```c run id=order-list
#include <stdio.h>
#include <stdlib.h>

struct order_list {
    int *ids;
    size_t count;
    size_t capacity;
};

// The caller owns the result and frees it with order_list_destroy.
struct order_list *order_list_create(void) {
    struct order_list *list = calloc(1, sizeof *list);
    return list;
}

// Returns 0 on success; on failure the list is unchanged.
int order_list_add(struct order_list *list, int id) {
    if (list->count == list->capacity) {
        size_t capacity = list->capacity == 0 ? 2 : list->capacity * 2;
        int *ids = realloc(list->ids, capacity * sizeof *ids);
        if (ids == NULL) {
            return -1;
        }
        list->ids = ids;
        list->capacity = capacity;
    }
    list->ids[list->count++] = id;
    return 0;
}

void order_list_destroy(struct order_list *list) {
    if (list != NULL) {
        free(list->ids);
        free(list);
    }
}

int main(void) {
    struct order_list *list = order_list_create();
    if (list == NULL) {
        return 1;
    }
    for (int id = 100; id < 105; id++) {
        if (order_list_add(list, id) != 0) {
            order_list_destroy(list);
            return 1;
        }
    }
    printf("orders: %zu, capacity: %zu\n", list->count, list->capacity);
    printf("last id: %d\n", list->ids[list->count - 1]);
    order_list_destroy(list);
    return 0;
}
```

```output
orders: 5, capacity: 8
last id: 104
```

- Verdict: native. C has `malloc`, `calloc`, `realloc` and `free` in the standard library, and the program decides when each block is freed.
- `order_list_create` and `order_list_destroy` are a pair. The comment on `order_list_create` states that the caller owns the result.
- `calloc` sets the structure to zero, so `ids` starts as a null pointer. `realloc` with a null pointer acts as `malloc`.
- `order_list_add` keeps the result of `realloc` in a second variable, so a failure leaves the old array in the list.
- `order_list_destroy` frees the inner array before the structure that points to it.
