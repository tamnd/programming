```c run id=interpreter
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum { NUM, VAR, BIN, LET, ASSIGN, BLOCK, WHILE, PRINT } Kind;

typedef struct Node {
    Kind kind;
    long value;              /* NUM */
    const char *name;        /* VAR, LET, ASSIGN */
    char op;                 /* BIN */
    struct Node *a, *b;      /* BIN: operands; LET, ASSIGN, PRINT: a; WHILE: a = cond, b = body */
    struct Node **body;      /* BLOCK: NULL-terminated list */
} Node;

/* A binding is one cell of a linked list; an environment is a list plus a parent. */
typedef struct Binding {
    const char *name;
    long value;
    struct Binding *next;
} Binding;

typedef struct Env {
    Binding *first;
    struct Env *parent;
} Env;

static int nodes, lookups, links;

static Node *mk(Kind kind) {
    Node *n = calloc(1, sizeof *n);
    n->kind = kind;
    return n;
}
static Node *num(long v) { Node *n = mk(NUM); n->value = v; return n; }
static Node *var(const char *s) { Node *n = mk(VAR); n->name = s; return n; }
static Node *bin(char op, Node *a, Node *b) { Node *n = mk(BIN); n->op = op; n->a = a; n->b = b; return n; }
static Node *let(const char *s, Node *e) { Node *n = mk(LET); n->name = s; n->a = e; return n; }
static Node *assign(const char *s, Node *e) { Node *n = mk(ASSIGN); n->name = s; n->a = e; return n; }
static Node *loop(Node *c, Node *body) { Node *n = mk(WHILE); n->a = c; n->b = body; return n; }
static Node *print(Node *e) { Node *n = mk(PRINT); n->a = e; return n; }
static Node *block(Node **stmts, size_t count) {
    Node *n = mk(BLOCK);
    n->body = calloc(count + 1, sizeof *n->body);
    memcpy(n->body, stmts, count * sizeof *stmts);
    return n;
}

static Binding *find(Env *env, const char *name) {
    lookups++;
    for (Env *e = env; e != NULL; e = e->parent) {
        for (Binding *b = e->first; b != NULL; b = b->next)
            if (strcmp(b->name, name) == 0) return b;
        links++;
    }
    fprintf(stderr, "undefined variable '%s'\n", name);
    exit(1);
}

static long evaluate(Node *n, Env *env) {
    nodes++;
    switch (n->kind) {
    case NUM: return n->value;
    case VAR: return find(env, n->name)->value;
    case BIN: {
        long x = evaluate(n->a, env);
        long y = evaluate(n->b, env);
        switch (n->op) {
        case '+': return x + y;
        case '-': return x - y;
        case '*': return x * y;
        case '<': return x < y;
        }
    }
    default:
        fprintf(stderr, "unknown expression\n");
        exit(1);
    }
}

static void execute(Node *n, Env *env) {
    nodes++;
    switch (n->kind) {
    case LET: {
        Binding *b = malloc(sizeof *b);
        b->name = n->name;
        b->value = evaluate(n->a, env);
        b->next = env->first;
        env->first = b;
        break;
    }
    case ASSIGN: {
        long v = evaluate(n->a, env);
        find(env, n->name)->value = v;
        break;
    }
    case BLOCK: {
        Env inner = {NULL, env};
        for (Node **s = n->body; *s != NULL; s++) execute(*s, &inner);
        while (inner.first != NULL) { /* the block ends: free its bindings */
            Binding *next = inner.first->next;
            free(inner.first);
            inner.first = next;
        }
        break;
    }
    case WHILE:
        while (evaluate(n->a, env) != 0) execute(n->b, env);
        break;
    case PRINT:
        printf("%ld\n", evaluate(n->a, env));
        break;
    default:
        fprintf(stderr, "unknown statement\n");
        exit(1);
    }
}

int main(void) {
    Node *inner[] = {
        let("step", var("n")),
        assign("acc", bin('*', var("acc"), var("step"))),
        assign("n", bin('-', var("n"), num(1))),
    };
    Node *outer[] = {
        let("n", num(5)),
        let("acc", num(1)),
        loop(bin('<', num(0), var("n")), block(inner, 3)),
        print(var("acc")),
    };
    Node *program = block(outer, 4);

    Env global = {NULL, NULL};
    execute(program, &global);
    printf("nodes visited: %d\n", nodes);
    printf("variable lookups: %d, parent links followed: %d\n", lookups, links);
    return 0; /* the tree is freed when the process exits */
}
```

```output for=interpreter
120
nodes visited: 81
variable lookups: 37, parent links followed: 25
```

- Verdict: idiom. One struct with a `kind` tag stands for every node type, and `switch` on the tag selects the case. Unused fields waste a little memory; a `union` would save it at the cost of more syntax.
- Each environment is a linked list of bindings with a pointer to its parent. A lookup compares names with `strcmp`, one binding at a time, so its cost grows with the number of names in each scope.
- The interpreter frees the bindings of a block when the block ends. A language with closures could not do this, because a closure may still point to the environment. It would need reference counts or a [[garbage-collection|garbage collector]].
