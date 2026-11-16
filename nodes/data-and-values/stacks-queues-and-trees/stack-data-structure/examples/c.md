```c run id=stack
#include <stdbool.h>
#include <stdio.h>

#define CAPACITY 3

struct stack {
    char items[CAPACITY];
    int count;
};

static bool push(struct stack *s, char c) {
    if (s->count == CAPACITY) {
        return false; /* overflow */
    }
    s->items[s->count++] = c;
    return true;
}

static bool pop(struct stack *s, char *out) {
    if (s->count == 0) {
        return false; /* underflow */
    }
    *out = s->items[--s->count];
    return true;
}

int main(void) {
    struct stack s = {.count = 0};
    for (const char *p = "abc"; *p != '\0'; p++) {
        push(&s, *p);
    }
    printf("count %d top %c\n", s.count, s.items[s.count - 1]);
    printf("push d: %s\n", push(&s, 'd') ? "yes" : "no");

    char c = 0;
    const char *sep = "";
    while (pop(&s, &c)) {
        printf("%s%c", sep, c);
        sep = " ";
    }
    printf("\npop on empty: %s\n", pop(&s, &c) ? "yes" : "no");
    return 0;
}
```

```output for=stack
count 3 top c
push d: no
c b a
pop on empty: no
```

- Verdict: idiom. C has no stack type. A fixed array and a count of items make a bounded stack.
- `push` and `pop` return `false` on overflow and underflow, so the caller must check them.
