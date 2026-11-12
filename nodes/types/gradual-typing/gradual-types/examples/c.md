```c run id=tagged-values
#include <stdio.h>

/* A dynamic value: a tag that says which member of the union is valid. */
enum kind { INT, TEXT };
struct value {
    enum kind kind;
    union {
        int i;
        const char *s;
    } as;
};

static int as_int(struct value v, int *out) { /* a checked cast */
    if (v.kind != INT) return 0;
    *out = v.as.i;
    return 1;
}

int main(void) {
    struct value vals[] = {{INT, {.i = 21}}, {TEXT, {.s = "21"}}};
    for (int k = 0; k < 2; k++) {
        int n;
        if (as_int(vals[k], &n))
            printf("double: %d\n", 2 * n);
        else
            printf("cast failed: the value holds text \"%s\"\n", vals[k].as.s);
    }
    return 0;
}
```

```output for=tagged-values
double: 42
cast failed: the value holds text "21"
```

- Verdict: idiom. C has `void *`, which holds a pointer to any type with no check at all, so a wrong cast is undefined behavior, not an error. The checked form is a tagged union: a field records which member is valid, and a function tests the tag before it reads the member, as interpreters for dynamic languages written in C do.
