```c run id=function-tables
#include <stdio.h>

/* The cases form a tagged union; each operation is a table of functions, one per tag. */
enum tag { NUM, ADD, NEG, NTAGS };

struct expr {
    enum tag tag;
    int value;
    const struct expr *left, *right;
};

typedef int (*eval_fn)(const struct expr *);
static int eval(const struct expr *e);

static int eval_num(const struct expr *e) { return e->value; }
static int eval_add(const struct expr *e) { return eval(e->left) + eval(e->right); }
static int eval_neg(const struct expr *e) { return -eval(e->left); }

static const eval_fn eval_table[NTAGS] = {
    [NUM] = eval_num, [ADD] = eval_add, [NEG] = eval_neg,
};

static int eval(const struct expr *e) { return eval_table[e->tag](e); }

/* A new operation: a new table, filled for every tag. */
typedef void (*show_fn)(const struct expr *);
static void show(const struct expr *e);

static void show_num(const struct expr *e) { printf("%d", e->value); }
static void show_add(const struct expr *e)
{
    printf("(");
    show(e->left);
    printf(" + ");
    show(e->right);
    printf(")");
}
static void show_neg(const struct expr *e)
{
    printf("-");
    show(e->left);
}

static const show_fn show_table[NTAGS] = {
    [NUM] = show_num, [ADD] = show_add, [NEG] = show_neg,
};

static void show(const struct expr *e) { show_table[e->tag](e); }

int main(void)
{
    struct expr one = {.tag = NUM, .value = 1};
    struct expr two = {.tag = NUM, .value = 2};
    struct expr three = {.tag = NUM, .value = 3};
    struct expr sum = {.tag = ADD, .left = &two, .right = &three};
    struct expr neg = {.tag = NEG, .left = &sum};
    struct expr top = {.tag = ADD, .left = &one, .right = &neg};
    show(&top);
    printf(" = %d\n", eval(&top));
    return 0;
}
```

```output for=function-tables
(1 + -(2 + 3)) = -4
```

```c fails expect=sanitizer:address id=missing-entry
#include <stdio.h>

enum tag { NUM, ADD, MUL, NTAGS }; /* MUL added after the tables were written */

struct expr {
    enum tag tag;
    int value;
    const struct expr *left, *right;
};

typedef int (*eval_fn)(const struct expr *);
static int eval(const struct expr *e);

static int eval_num(const struct expr *e) { return e->value; }
static int eval_add(const struct expr *e) { return eval(e->left) + eval(e->right); }

static const eval_fn eval_table[NTAGS] = {
    [NUM] = eval_num, [ADD] = eval_add, /* no entry for MUL: a null pointer */
};

static int eval(const struct expr *e) { return eval_table[e->tag](e); }

int main(void)
{
    struct expr two = {.tag = NUM, .value = 2}, three = {.tag = NUM, .value = 3};
    struct expr product = {.tag = MUL, .left = &two, .right = &three};
    printf("%d\n", eval(&product));
    return 0;
}
```

- Verdict: idiom. C has no dispatch of its own, so a program builds it: a tag in each value names the case, and each operation is an array of function pointers indexed by the tag. A new operation is a new table. A new case is a new tag and an entry in every table, so the tables of all operations must change.
- The failing block adds the tag `MUL` but no entry in the table for `eval`. The designated initializer leaves the entry as a null pointer, and the call through it crashes, which AddressSanitizer reports as a SEGV. A `switch` over the tag with `-Wswitch` enabled and no `default` lets the compiler warn about the missing case instead.
