```c run id=pq
#include <stdbool.h>
#include <stdio.h>

#define CAPACITY 8

struct task {
    int priority;
    const char *name;
};

struct heap {
    struct task items[CAPACITY];
    int count;
    bool (*before)(const struct task *, const struct task *);
};

static bool lower_first(const struct task *a, const struct task *b) {
    return a->priority < b->priority;
}

static bool higher_first(const struct task *a, const struct task *b) {
    return a->priority > b->priority;
}

static void swap(struct task *a, struct task *b) {
    struct task t = *a;
    *a = *b;
    *b = t;
}

static bool push(struct heap *h, struct task t) {
    if (h->count == CAPACITY) {
        return false;
    }
    int i = h->count++;
    h->items[i] = t;
    while (i > 0) {
        int parent = (i - 1) / 2;
        if (!h->before(&h->items[i], &h->items[parent])) {
            break;
        }
        swap(&h->items[i], &h->items[parent]);
        i = parent;
    }
    return true;
}

static bool pop(struct heap *h, struct task *out) {
    if (h->count == 0) {
        return false;
    }
    *out = h->items[0];
    h->count--;
    h->items[0] = h->items[h->count];
    int i = 0;
    for (;;) {
        int left = 2 * i + 1;
        int right = left + 1;
        int best = i;
        if (left < h->count && h->before(&h->items[left], &h->items[best])) {
            best = left;
        }
        if (right < h->count && h->before(&h->items[right], &h->items[best])) {
            best = right;
        }
        if (best == i) {
            break;
        }
        swap(&h->items[i], &h->items[best]);
        i = best;
    }
    return true;
}

int main(void) {
    struct task input[] = {{3, "update docs"}, {1, "fix outage"}, {2, "review patch"}, {4, "plan sprint"}};
    int n = (int)(sizeof input / sizeof input[0]);
    struct task t = {0, ""};

    struct heap tasks = {.count = 0, .before = lower_first};
    for (int i = 0; i < n; i++) {
        push(&tasks, input[i]);
    }
    printf("peek: %s\n", tasks.items[0].name);
    while (pop(&tasks, &t)) {
        printf("%d %s\n", t.priority, t.name);
    }

    struct heap max_heap = {.count = 0, .before = higher_first};
    for (int i = 0; i < n; i++) {
        push(&max_heap, input[i]);
    }
    printf("largest first:");
    while (pop(&max_heap, &t)) {
        printf(" %d", t.priority);
    }
    printf("\n");
    return 0;
}
```

```output for=pq
peek: fix outage
1 fix outage
2 review patch
3 update docs
4 plan sprint
largest first: 4 3 2 1
```

- Verdict: idiom. C has no priority queue type. A fixed array, a count and a comparison function make a bounded binary heap.
- `push` returns `false` when the array is full, and `pop` returns `false` when the heap is empty.
- The same code gives a max-heap with the `higher_first` function.
