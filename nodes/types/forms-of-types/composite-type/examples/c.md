```c run id=struct-union-array
#include <stdio.h>
#include <string.h>

struct point { int x; int y; };
struct mixed { char c; int i; char d; };
union number { int i; float f; };
struct node { int value; struct node *next; };

int main(void) {
    struct point p = { 1, 2 };
    struct point q = p;   /* assignment copies the struct */
    q.x = 5;
    printf("%d %d\n", p.x, q.x);

    /* Padding: the size can be more than the sum of the members. */
    printf("%zu %zu\n", sizeof(struct mixed), 2 * sizeof(char) + sizeof(int));

    /* A union is as large as its largest member. */
    printf("%zu\n", sizeof(union number));

    int a[3] = { 1, 2, 3 };
    int b[3];
    memcpy(b, a, sizeof a);   /* an array cannot be assigned, so copy its bytes */
    printf("%zu %d\n", sizeof a / sizeof a[0], b[2]);

    /* A struct can point to its own type. */
    struct node second = { 2, NULL };
    struct node first = { 1, &second };
    for (struct node *n = &first; n != NULL; n = n->next) {
        printf("%d\n", n->value);
    }
    return 0;
}
```

```output for=struct-union-array
1 5
12 6
4
3 3
1
2
```

- Verdict: native. Assignment copies a struct, but C has no `==` for structs and no assignment for arrays, so the program copies an array with `memcpy`. On this compiler, `struct mixed` takes 12 bytes for 6 bytes of members, because the compiler aligns `int i` and pads the end. The union takes the 4 bytes of its largest member. `struct node` holds a pointer to its own type, which makes a linked list.
