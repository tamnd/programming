```c run id=engine
#include <stdio.h>
#include <stdlib.h>

/* Values carry their type, as boxed values do in a dynamic language. */
typedef enum { INT, FLOAT } Kind;
typedef struct {
    Kind kind;
    union {
        long i;
        double f;
    } as;
} Value;

static Value int_value(long i) { return (Value){INT, {.i = i}}; }
static Value float_value(double f) { return (Value){FLOAT, {.f = f}}; }
static double as_double(Value v) { return v.kind == INT ? (double)v.as.i : v.as.f; }

static void print_value(Value v) {
    if (v.kind == INT) printf("%ld", v.as.i);
    else printf("%g", v.as.f);
}

/* The body of f(x) = x * x + 1 as a tree. */
typedef enum { NUM, ARG, BIN } Tag;
typedef struct Node {
    Tag tag;
    long num;
    char op;
    const struct Node *left, *right;
} Node;

/* Generic arithmetic: check both tags on every operation. */
static Value arith(char op, Value a, Value b) {
    if (a.kind == INT && b.kind == INT)
        return int_value(op == '+' ? a.as.i + b.as.i : a.as.i * b.as.i);
    double x = as_double(a), y = as_double(b);
    return float_value(op == '+' ? x + y : x * y);
}

/* The interpreter: walk the tree on every call. */
static Value interpret(const Node *n, Value arg) {
    switch (n->tag) {
    case NUM: return int_value(n->num);
    case ARG: return arg;
    default: return arith(n->op, interpret(n->left, arg), interpret(n->right, arg));
    }
}

/* Compiled code: a tree of nodes, each with a pointer to the function that
   runs it. The int code works on raw longs and has no tag checks inside. */
typedef struct Code {
    long (*run_int)(const struct Code *, long);
    Value (*run_generic)(const struct Code *, Value);
    long num;
    char op;
    struct Code *left, *right;
} Code;

static long int_num(const Code *c, long x) { (void)x; return c->num; }
static long int_arg(const Code *c, long x) { (void)c; return x; }
static long int_add(const Code *c, long x) { return c->left->run_int(c->left, x) + c->right->run_int(c->right, x); }
static long int_mul(const Code *c, long x) { return c->left->run_int(c->left, x) * c->right->run_int(c->right, x); }

static Value gen_num(const Code *c, Value a) { (void)a; return int_value(c->num); }
static Value gen_arg(const Code *c, Value a) { (void)c; return a; }
static Value gen_bin(const Code *c, Value a) {
    return arith(c->op, c->left->run_generic(c->left, a), c->right->run_generic(c->right, a));
}

static Code *compile(const Node *n, int for_int) {
    Code *c = calloc(1, sizeof *c);
    if (c == NULL) abort();
    c->num = n->num;
    c->op = n->op;
    if (n->tag == BIN) {
        c->left = compile(n->left, for_int);
        c->right = compile(n->right, for_int);
    }
    if (for_int) /* choose the int function for each node now, once */
        c->run_int = n->tag == NUM ? int_num : n->tag == ARG ? int_arg : n->op == '+' ? int_add : int_mul;
    else
        c->run_generic = n->tag == NUM ? gen_num : n->tag == ARG ? gen_arg : gen_bin;
    return c;
}

static void free_code(Code *c) {
    if (c == NULL) return;
    free_code(c->left);
    free_code(c->right);
    free(c);
}

enum { THRESHOLD = 3 };

typedef struct {
    const char *name;
    const Node *body;
    int calls;       /* calls in the interpreter since the last compilation */
    int seen_int;    /* the profile: the kinds of the arguments */
    int seen_float;
    Code *code;      /* compiled code, or NULL */
    int code_is_int; /* 1 if the code is specialized for int */
    int counts[3];   /* interpreter, compiled, deoptimizations */
} Function;

static void jit_compile(Function *f) {
    f->code_is_int = f->seen_int && !f->seen_float;
    f->code = compile(f->body, f->code_is_int);
    printf("* compile %s: arguments seen: %s; %s\n", f->name,
           f->seen_float ? (f->seen_int ? "float, int" : "float") : "int",
           f->code_is_int ? "specialized for int" : "generic");
}

/* Calls f and writes the tier that ran it into tier. */
static Value call(Function *f, Value arg, char *tier, size_t size) {
    if (f->code == NULL && f->calls >= THRESHOLD) jit_compile(f);
    if (f->code != NULL) {
        if (!f->code_is_int) {
            f->counts[1]++;
            snprintf(tier, size, "compiled, generic");
            return f->code->run_generic(f->code, arg);
        }
        if (arg.kind == INT) { /* the guard */
            f->counts[1]++;
            snprintf(tier, size, "compiled, int");
            return int_value(f->code->run_int(f->code, arg.as.i));
        }
        printf("* deoptimize %s: guard failed, ", f->name);
        print_value(arg);
        printf(" is not an int\n");
        f->counts[2]++;
        free_code(f->code);
        f->code = NULL;
        f->calls = 0;
    }
    f->calls++;
    f->counts[0]++;
    if (arg.kind == INT) f->seen_int = 1;
    else f->seen_float = 1;
    snprintf(tier, size, "interpreter, call %d", f->calls);
    return interpret(f->body, arg);
}

int main(void) {
    const Node x = {ARG, 0, 0, NULL, NULL};
    const Node one = {NUM, 1, 0, NULL, NULL};
    const Node square = {BIN, 0, '*', &x, &x};
    const Node body = {BIN, 0, '+', &square, &one};
    Function f = {"f", &body, 0, 0, 0, NULL, 0, {0, 0, 0}};

    Value inputs[] = {int_value(1), int_value(2), int_value(3), int_value(4),
                      float_value(2.5), int_value(6), int_value(7), int_value(8)};
    char tier[32];
    for (size_t i = 0; i < sizeof inputs / sizeof inputs[0]; i++) {
        Value result = call(&f, inputs[i], tier, sizeof tier);
        printf("f(");
        print_value(inputs[i]);
        printf(") = ");
        print_value(result);
        printf(" [%s]\n", tier);
    }
    printf("interpreter: %d, compiled: %d, deoptimizations: %d\n", f.counts[0], f.counts[1], f.counts[2]);
    free_code(f.code);
    return 0;
}
```

```output for=engine
f(1) = 2 [interpreter, call 1]
f(2) = 5 [interpreter, call 2]
f(3) = 10 [interpreter, call 3]
* compile f: arguments seen: int; specialized for int
f(4) = 17 [compiled, int]
* deoptimize f: guard failed, 2.5 is not an int
f(2.5) = 7.25 [interpreter, call 1]
f(6) = 37 [interpreter, call 2]
f(7) = 50 [interpreter, call 3]
* compile f: arguments seen: float, int; generic
f(8) = 65 [compiled, generic]
interpreter: 6, compiled: 2, deoptimizations: 1
```

- Verdict: idiom. C compiles ahead of time to machine code, so a JIT is something a program builds, as here.
- C has no closures. Compiled code is a tree of `Code` structs, and each struct holds a pointer to the function that runs it. `compile` chooses that function once, so the run does not test the node tag again.
- `Value` is a tagged union, the C form of a sum type. The guard is a test of the tag, `arg.kind == INT`, before the code reads `arg.as.i`.
- The program owns the memory of the compiled code. A deoptimization frees the old tree with `free_code` before the function returns to the interpreter.
