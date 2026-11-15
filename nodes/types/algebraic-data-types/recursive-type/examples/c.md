```c run id=list
#include <stdio.h>
#include <stdlib.h>

struct node {
    int value;
    struct node *next;
};

static struct node *cons(int value, struct node *next) {
    struct node *n = malloc(sizeof *n);
    if (n == NULL) {
        exit(1);
    }
    n->value = value;
    n->next = next;
    return n;
}

static int total(const struct node *n) {
    return n == NULL ? 0 : n->value + total(n->next);
}

static int length(const struct node *n) {
    return n == NULL ? 0 : 1 + length(n->next);
}

static int release(struct node *n) {
    int count = 0;
    while (n != NULL) {
        struct node *next = n->next;
        free(n);
        n = next;
        count++;
    }
    return count;
}

int main(void) {
    struct node *xs = cons(1, cons(2, cons(3, NULL)));
    printf("sum: %d\n", total(xs));
    printf("length: %d\n", length(xs));
    printf("list:");
    for (const struct node *n = xs; n != NULL; n = n->next) {
        printf(n == xs ? " %d" : " -> %d", n->value);
    }
    printf("\n");
    printf("freed: %d\n", release(xs));
    return 0;
}
```

```output for=list
sum: 6
length: 3
list: 1 -> 2 -> 3
freed: 3
```

- Verdict: native. The struct holds a pointer to its own struct type. The tag `struct node` is in scope inside its own body, so the field `next` can name it.
- A field of type `struct node` (with no `*`) does not compile, because the type is not complete there.
- The program must free each node. `release` uses a loop, so it also works for long lists.
