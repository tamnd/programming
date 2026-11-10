```c run id=parse-walk
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum kind { NUM, VAR, BIN };

struct expr {
    enum kind kind;
    union {
        int value;
        char name[16];
        struct {
            char op;
            struct expr *left, *right;
        } bin;
    } u;
};

static const char *src;
static size_t pos;

static struct expr *node(enum kind kind) {
    struct expr *e = calloc(1, sizeof *e);
    if (!e) abort();
    e->kind = kind;
    return e;
}

static struct expr *bin(char op, struct expr *l, struct expr *r) {
    struct expr *e = node(BIN);
    e->u.bin.op = op;
    e->u.bin.left = l;
    e->u.bin.right = r;
    return e;
}

static char peek(void) {
    while (src[pos] == ' ') pos++;
    return src[pos];
}

static struct expr *expr(void);

static struct expr *factor(void) {
    char c = peek();
    if (c == '(') {
        pos++;
        struct expr *inner = expr();
        if (peek() != ')') {
            fprintf(stderr, "expected ) at %zu\n", pos);
            exit(1);
        }
        pos++;
        return inner;
    }
    if (isdigit((unsigned char)c)) {
        struct expr *e = node(NUM);
        while (isdigit((unsigned char)src[pos])) e->u.value = e->u.value * 10 + (src[pos++] - '0');
        return e;
    }
    if (isalpha((unsigned char)c)) {
        struct expr *e = node(VAR);
        size_t n = 0;
        while (isalpha((unsigned char)src[pos]) && n < sizeof e->u.name - 1) e->u.name[n++] = src[pos++];
        return e;
    }
    fprintf(stderr, "unexpected '%c' at %zu\n", c, pos);
    exit(1);
}

static struct expr *term(void) {
    struct expr *left = factor();
    while (peek() == '*') {
        pos++;
        left = bin('*', left, factor());
    }
    return left;
}

static struct expr *expr(void) {
    struct expr *left = term();
    while (peek() == '+' || peek() == '-') {
        char op = src[pos++];
        left = bin(op, left, term());
    }
    return left;
}

static void show(const struct expr *e, int depth) {
    printf("%*s", 2 * depth, "");
    switch (e->kind) {
    case NUM: printf("Num %d\n", e->u.value); break;
    case VAR: printf("Var %s\n", e->u.name); break;
    case BIN:
        printf("Bin %c\n", e->u.bin.op);
        show(e->u.bin.left, depth + 1);
        show(e->u.bin.right, depth + 1);
        break;
    }
}

static int count(const struct expr *e) {
    return e->kind == BIN ? 1 + count(e->u.bin.left) + count(e->u.bin.right) : 1;
}

static void names(const struct expr *e) {
    if (e->kind == VAR) printf(" %s", e->u.name);
    if (e->kind == BIN) {
        names(e->u.bin.left);
        names(e->u.bin.right);
    }
}

static int eval(const struct expr *e, int x) {
    switch (e->kind) {
    case NUM: return e->u.value;
    case VAR: return x;
    case BIN: {
        int l = eval(e->u.bin.left, x), r = eval(e->u.bin.right, x);
        return e->u.bin.op == '+' ? l + r : e->u.bin.op == '-' ? l - r : l * r;
    }
    }
    return 0;
}

static void free_tree(struct expr *e) {
    if (e->kind == BIN) {
        free_tree(e->u.bin.left);
        free_tree(e->u.bin.right);
    }
    free(e);
}

int main(void) {
    src = "1 + 2 * (3 - x)";
    struct expr *tree = expr();
    show(tree, 0);
    printf("nodes: %d\n", count(tree));
    printf("names:");
    names(tree);
    printf("\nvalue at x = 1: %d\n", eval(tree, 1));
    free_tree(tree);
    return 0;
}
```

```output for=parse-walk
Bin +
  Num 1
  Bin *
    Num 2
    Bin -
      Num 3
      Var x
nodes: 7
names: x
value at x = 1: 5
```

- Verdict: idiom. A tagged struct is the C form of a sum type: an `enum` field says which member of the `union` is valid. The compiler does not check that the code reads the right member.
- Each node is a separate `calloc` allocation, and the program frees the tree with a post-order walk. Forgetting `free_tree` leaks every node.
- The evaluator has one variable, `x`, to keep the example short. A real interpreter looks names up in an environment, as the tree-walking interpreter page shows.
