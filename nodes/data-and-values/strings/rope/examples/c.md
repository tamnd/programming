```c run id=shared-subtree-doubling
#include <stdio.h>

/* A rope node is a leaf (s is set) or a join of two ropes (l and r are set). */
typedef struct Rope {
    const char *s;
    const struct Rope *l, *r;
    size_t n;
} Rope;

static Rope pool[64];
static int used;

static const Rope *leaf(const char *s, size_t n) {
    Rope *t = &pool[used++];
    *t = (Rope){s, NULL, NULL, n};
    return t;
}

static const Rope *cat(const Rope *a, const Rope *b) {
    Rope *t = &pool[used++];
    *t = (Rope){NULL, a, b, a->n + b->n};
    return t;
}

/* fetch walks from the root to the leaf that holds character i. */
static char fetch(const Rope *t, size_t i) {
    while (t->s == NULL) {
        if (i < t->l->n) {
            t = t->l;
        } else {
            i -= t->l->n;
            t = t->r;
        }
    }
    return t->s[i];
}

int main(void) {
    /* Join a rope with itself 20 times. Both children are the same node,
       so the tree is a graph that shares pieces, and nothing is copied. */
    const Rope *r = leaf("ab", 2);
    for (int k = 0; k < 20; k++) r = cat(r, r);
    printf("%zu %d\n", r->n, used);
    printf("%c %c\n", fetch(r, 0), fetch(r, 1000001));

    /* Put a prefix and a suffix around the 2 MiB rope: 4 more nodes. */
    const Rope *s = cat(cat(leaf("x = ", 4), r), leaf(";", 1));
    printf("%zu %d %c %c\n", s->n, used, fetch(s, 4), fetch(s, s->n - 1));
    return 0;
}
```

```output for=shared-subtree-doubling
2097152 21
a b
2097157 25 a ;
```

- Verdict: idiom. The C standard library has no rope. The C cords of the Boehm garbage collector are a library. The loop joins a rope with itself 20 times, so 21 nodes describe 2097152 characters, and `fetch` passes 20 join nodes to reach a leaf. Two more joins add a prefix and a suffix.
