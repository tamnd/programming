```c run id=ring
#include <stdbool.h>
#include <stdio.h>

#define CAPACITY 3

struct ring {
    int items[CAPACITY];
    int start;
    int count;
};

static void push(struct ring *r, int item) {
    r->items[(r->start + r->count) % CAPACITY] = item;
    if (r->count < CAPACITY) {
        r->count++;
    } else {
        r->start = (r->start + 1) % CAPACITY;
    }
}

static bool pop(struct ring *r, int *out) {
    if (r->count == 0) {
        return false;
    }
    *out = r->items[r->start];
    r->start = (r->start + 1) % CAPACITY;
    r->count--;
    return true;
}

static void print_items(const struct ring *r) {
    for (int i = 0; i < r->count; i++) {
        printf(" %d", r->items[(r->start + i) % CAPACITY]);
    }
    printf("\n");
}

int main(void) {
    struct ring r = {.start = 0, .count = 0};
    int readings[] = {10, 20, 30, 40, 50};
    for (int i = 0; i < 5; i++) {
        push(&r, readings[i]);
        printf("after %d:", readings[i]);
        print_items(&r);
    }

    int oldest = 0;
    if (pop(&r, &oldest)) {
        printf("read %d, left:", oldest);
        print_items(&r);
    }
    return 0;
}
```

```output for=ring
after 10: 10
after 20: 10 20
after 30: 10 20 30
after 40: 20 30 40
after 50: 30 40 50
read 30, left: 40 50
```

- Verdict: idiom. A fixed array, a start index and a count make a ring buffer. It allocates no memory.
- `pop` returns `false` when the buffer is empty.
