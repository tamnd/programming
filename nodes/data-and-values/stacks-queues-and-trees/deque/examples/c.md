```c run id=deque
#include <stdbool.h>
#include <stdio.h>

#define CAPACITY 4

/* A ring buffer with a front index. Both ends wrap around. */
struct deque {
    int items[CAPACITY];
    int front;
    int count;
};

static bool push_front(struct deque *d, int v) {
    if (d->count == CAPACITY) {
        return false;
    }
    d->front = (d->front + CAPACITY - 1) % CAPACITY;
    d->items[d->front] = v;
    d->count++;
    return true;
}

static bool push_back(struct deque *d, int v) {
    if (d->count == CAPACITY) {
        return false;
    }
    d->items[(d->front + d->count) % CAPACITY] = v;
    d->count++;
    return true;
}

static bool pop_front(struct deque *d, int *out) {
    if (d->count == 0) {
        return false;
    }
    *out = d->items[d->front];
    d->front = (d->front + 1) % CAPACITY;
    d->count--;
    return true;
}

static bool pop_back(struct deque *d, int *out) {
    if (d->count == 0) {
        return false;
    }
    d->count--;
    *out = d->items[(d->front + d->count) % CAPACITY];
    return true;
}

static void print(const struct deque *d) {
    printf("[");
    for (int i = 0; i < d->count; i++) {
        printf(i == 0 ? "%d" : " %d", d->items[(d->front + i) % CAPACITY]);
    }
    printf("] front index %d\n", d->front);
}

int main(void) {
    struct deque d = {.front = 0, .count = 0};
    push_back(&d, 2);
    push_back(&d, 3);
    push_front(&d, 1);
    print(&d);
    printf("push_back 4: %s\n", push_back(&d, 4) ? "yes" : "no");
    printf("push_back 5: %s\n", push_back(&d, 5) ? "yes" : "no");

    int a = 0;
    int b = 0;
    pop_front(&d, &a);
    pop_back(&d, &b);
    printf("popped %d and %d\n", a, b);
    print(&d);
    return 0;
}
```

```output for=deque
[1 2 3] front index 3
push_back 4: yes
push_back 5: no
popped 1 and 4
[2 3] front index 0
```

- Verdict: idiom. C has no deque type. A fixed array with a front index and a count is a bounded deque.
- `push_front` moves the front index back by one, so it wraps from 0 to the last slot of the array.
