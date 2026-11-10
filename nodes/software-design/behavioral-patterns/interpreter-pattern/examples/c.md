```c run id=expr
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* The grammar as a tagged union. */
enum kind { NUM, VAR, ADD, MUL };

struct expr {
    enum kind kind;
    union {
        int value;                                /* NUM */
        const char *name;                         /* VAR */
        struct { struct expr *left, *right; } op; /* ADD, MUL */
    } as;
};

struct binding { const char *name; int value; };
struct env { const struct binding *items; size_t len; };

static struct expr *make(enum kind kind) {
    struct expr *e = malloc(sizeof *e);
    if (e == NULL) abort();
    e->kind = kind;
    return e;
}
static struct expr *num(int v) { struct expr *e = make(NUM); e->as.value = v; return e; }
static struct expr *var(const char *n) { struct expr *e = make(VAR); e->as.name = n; return e; }
static struct expr *binary(enum kind k, struct expr *l, struct expr *r) {
    struct expr *e = make(k);
    e->as.op.left = l;
    e->as.op.right = r;
    return e;
}

static void free_expr(struct expr *e) {
    if (e->kind == ADD || e->kind == MUL) {
        free_expr(e->as.op.left);
        free_expr(e->as.op.right);
    }
    free(e);
}

/* Returns false and sets *missing if a variable has no value. */
static bool evaluate(const struct expr *e, struct env env, int *out, const char **missing) {
    int l, r;
    switch (e->kind) {
    case NUM:
        *out = e->as.value;
        return true;
    case VAR:
        for (size_t i = 0; i < env.len; i++) {
            if (strcmp(env.items[i].name, e->as.name) == 0) {
                *out = env.items[i].value;
                return true;
            }
        }
        *missing = e->as.name;
        return false;
    case ADD:
    case MUL:
        if (!evaluate(e->as.op.left, env, &l, missing)) return false;
        if (!evaluate(e->as.op.right, env, &r, missing)) return false;
        *out = e->kind == ADD ? l + r : l * r;
        return true;
    }
    abort();
}

static void show(const struct expr *e);

/* A sum inside a product needs parentheses. */
static void wrap(const struct expr *e) {
    if (e->kind == ADD) { putchar('('); show(e); putchar(')'); }
    else show(e);
}

static void show(const struct expr *e) {
    switch (e->kind) {
    case NUM: printf("%d", e->as.value); break;
    case VAR: fputs(e->as.name, stdout); break;
    case ADD: show(e->as.op.left); fputs(" + ", stdout); show(e->as.op.right); break;
    case MUL: wrap(e->as.op.left); fputs(" * ", stdout); wrap(e->as.op.right); break;
    }
}

static void report(const struct expr *e, struct env env) {
    for (size_t i = 0; i < env.len; i++)
        printf("%s%s=%d", i ? " " : "", env.items[i].name, env.items[i].value);
    int value;
    const char *missing;
    if (evaluate(e, env, &value, &missing)) printf(": %d\n", value);
    else printf(": error: unknown variable %s\n", missing);
}

int main(void) {
    struct expr *e = binary(MUL, var("x"), binary(ADD, var("y"), num(2)));
    const struct binding b1[] = {{"x", 3}, {"y", 4}};
    const struct binding b2[] = {{"x", 5}, {"y", 1}};
    fputs("expression: ", stdout); show(e); putchar('\n');
    report(e, (struct env){b1, 2});
    report(e, (struct env){b2, 2});

    struct expr *f = binary(ADD, var("z"), num(1));
    fputs("expression: ", stdout); show(f); putchar('\n');
    report(f, (struct env){b1, 2});

    free_expr(e);
    free_expr(f);
    return 0;
}
```

```output
expression: x * (y + 2)
x=3 y=4: 18
x=5 y=1: 15
expression: z + 1
x=3 y=4: error: unknown variable z
```

- Verdict: idiom. A tagged union holds the nodes, and a `switch` on the tag gives each operation. C has no classes, so this is the function-per-operation form.
- A `switch` on an `enum` with no `default` lets the compiler warn (with `-Wswitch`, part of `-Wall` in GCC and Clang) when a case for one of the kinds is missing.
- `evaluate` reports failure through its return value and an output parameter, because C has no exceptions.
- The client owns the tree and frees it with `free_expr`, which walks the tree in the same way as the other operations.
