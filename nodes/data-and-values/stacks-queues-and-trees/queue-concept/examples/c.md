```c run id=queue
#include <stdbool.h>
#include <stdio.h>

#define CAPACITY 4

/* A ring buffer: head is the front, count is the number of items. */
struct queue {
    char items[CAPACITY];
    int head;
    int count;
};

static bool enqueue(struct queue *q, char c) {
    if (q->count == CAPACITY) {
        return false;
    }
    q->items[(q->head + q->count) % CAPACITY] = c;
    q->count++;
    return true;
}

static bool dequeue(struct queue *q, char *out) {
    if (q->count == 0) {
        return false;
    }
    *out = q->items[q->head];
    q->head = (q->head + 1) % CAPACITY;
    q->count--;
    return true;
}

int main(void) {
    struct queue q = {.head = 0, .count = 0};
    for (const char *p = "abc"; *p != '\0'; p++) {
        enqueue(&q, *p);
    }
    char c = 0;
    dequeue(&q, &c);
    printf("dequeue %c, front %c, count %d\n", c, q.items[q.head], q.count);

    /* The tail wraps around to the start of the array. */
    enqueue(&q, 'd');
    enqueue(&q, 'e');
    printf("enqueue f: %s\n", enqueue(&q, 'f') ? "yes" : "no");

    const char *sep = "";
    while (dequeue(&q, &c)) {
        printf("%s%c", sep, c);
        sep = " ";
    }
    printf("\ndequeue on empty: %s\n", dequeue(&q, &c) ? "yes" : "no");
    return 0;
}
```

```output for=queue
dequeue a, front b, count 2
enqueue f: no
b c d e
dequeue on empty: no
```

- Verdict: idiom. C has no queue type. A fixed array with a head index and a count is a bounded queue, and the indexes wrap with `%`.
- The functions return `false` when the queue is full or empty.
