```c run id=bit-set
#include <stdint.h>
#include <stdio.h>

/* A set of small integers 0..63 as the bits of one 64-bit word. */
typedef uint64_t set64;

static set64 add(set64 s, int v) { return s | (UINT64_C(1) << v); }
static int has(set64 s, int v) { return (s >> v) & 1; }

static void show(const char *name, set64 s) {
    printf("%s {", name);
    const char *sep = "";
    for (int v = 0; v < 64; v++) {
        if (has(s, v)) {
            printf("%s%d", sep, v);
            sep = " ";
        }
    }
    printf("}\n");
}

static int count(set64 s) {
    int n = 0;
    for (; s; s &= s - 1) n++; /* clear the lowest bit each time */
    return n;
}

int main(void) {
    set64 a = 0, b = 0;
    int as[] = {1, 2, 3, 3}, bs[] = {2, 3, 4};
    for (int i = 0; i < 4; i++) a = add(a, as[i]);
    for (int i = 0; i < 3; i++) b = add(b, bs[i]);
    printf("%d %d %d\n", count(a), has(a, 2), has(a, 5));
    /* Each set operation is one machine instruction on the words. */
    show("union", a | b);
    show("intersection", a & b);
    show("difference", a & ~b);
    show("symmetric", a ^ b);
    printf("subset %d\n", (add(add(0, 2), 3) & ~a) == 0);
    return 0;
}
```

```output
3 1 0
union {1 2 3 4}
intersection {2 3}
difference {1}
symmetric {1 4}
subset 1
```

- Verdict: idiom. C has no set type. A set of the integers 0 to 63 fits in the bits of one uint64_t, and union, intersection and difference are one bitwise operation each. Larger sets use an array of words, a hash table or a sorted array.
