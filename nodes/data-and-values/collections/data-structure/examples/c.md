```c run id=two-layouts
#include <stdio.h>
#include <stdlib.h>

struct node {
    int value;
    struct node *next;
};

static struct node *push_front(struct node *head, int value) {
    struct node *n = malloc(sizeof *n);
    if (n == NULL) {
        exit(1);
    }
    n->value = value;
    n->next = head;
    return n;
}

int main(void) {
    int array[] = {10, 20, 30, 40, 50};
    struct node *head = NULL;
    for (int i = 4; i >= 0; i--) {
        head = push_front(head, array[i]);
    }

    printf("array[3] = %d (0 steps)\n", array[3]);
    struct node *n = head;
    int steps = 0;
    for (int i = 3; i > 0; i--, steps++) {
        n = n->next;
    }
    printf("linked[3] = %d (%d steps)\n", n->value, steps);

    head = push_front(head, 5);
    printf("after push_front:");
    while (head != NULL) {
        struct node *next = head->next;
        printf(" %d", head->value);
        free(head);
        head = next;
    }
    printf("\n");
    return 0;
}
```

```output for=two-layouts
array[3] = 40 (0 steps)
linked[3] = 40 (3 steps)
after push_front: 5 10 20 30 40 50
```

- Verdict: idiom. C has arrays, structs and pointers. The program writes each other data structure itself, or uses a library.
- `array[3]` is the address of `array` plus three times the size of an `int`. The linked list follows `next` three times.
- Each node comes from `malloc`, so the program must `free` each one.
