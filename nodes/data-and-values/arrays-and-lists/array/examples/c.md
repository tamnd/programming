```c run id=sizeof-and-decay
#include <stddef.h>
#include <stdio.h>

struct triple {
    int v[3];
};

/* A parameter written as int items[] has the type int *, so this is the same function. */
static size_t size_in_function(int *items) {
    return sizeof items;
}

int main(void) {
    /* The initializer gives three items. The other two items are 0. */
    int a[5] = {1, 2, 3};
    size_t n = sizeof a / sizeof a[0];
    printf("%zu %zu %d %d\n", sizeof a, n, a[3], a[4]);

    /* Item i is at the base address plus i * sizeof(int) bytes. */
    printf("%td %td\n", (char *)&a[3] - (char *)&a[0], &a[3] - a);

    /* The array converts to a pointer to its first item when it is passed. */
    printf("%d\n", size_in_function(a) == sizeof(int *));

    /* An array cannot be assigned, but a struct with an array member can. */
    struct triple s = {{1, 2, 3}};
    struct triple t = s;
    t.v[0] = 9;
    printf("%d %d\n", s.v[0], t.v[0]);

    /* C does not check the index. a[5] is outside the array and is undefined behaviour. */
    for (size_t i = 0; i < n; i++) {
        printf("%d%s", a[i], i + 1 < n ? " " : "\n");
    }
    return 0;
}
```

```output
20 5 0 0
12 3
1
1 9
1 2 3 0 0
```

- Verdict: native. The initializer sets the missing items to 0. sizeof a gives 20 bytes, and the address of a[3] is 12 bytes after a[0]. A function gets a pointer, so sizeof there is the size of a pointer. An array cannot be assigned, but a struct with an array member is copied. C does not check indices.
