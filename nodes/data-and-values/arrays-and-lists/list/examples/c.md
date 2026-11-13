```c run id=cons-cells
#include <stdio.h>
#include <stdlib.h>

/* A cons cell: one item and a pointer to the rest of the list. NULL is the empty list. */
typedef struct Cell {
    int first;
    struct Cell *rest;
} Cell;

static Cell *cons(int first, Cell *rest) {
    Cell *c = malloc(sizeof *c);
    if (c == NULL) {
        exit(1);
    }
    c->first = first;
    c->rest = rest;
    return c;
}

static void print_list(const Cell *c) {
    printf("(");
    for (; c != NULL; c = c->rest) {
        printf("%d%s", c->first, c->rest ? " " : "");
    }
    printf(")\n");
}

int main(void) {
    Cell *list = cons(1, cons(2, cons(3, NULL)));
    Cell *longer = cons(0, list); /* shares the three cells of list */
    print_list(list);
    print_list(longer);
    printf("%d\n", longer->rest == list);

    /* Free each cell once. longer owns all four cells now. */
    while (longer != NULL) {
        Cell *next = longer->rest;
        free(longer);
        longer = next;
    }
    return 0;
}
```

```output
(1 2 3)
(0 1 2 3)
1
```

- Verdict: idiom. C has no list type. Each cell is a struct with an item and a pointer to the next cell. Prepending makes one new cell and shares the old chain, so both lists use the same tail.
