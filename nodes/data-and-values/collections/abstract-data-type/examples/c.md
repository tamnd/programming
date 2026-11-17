```c run id=stack-adt
#include <stdio.h>
#include <stdlib.h>

/* stack.h: the interface. The struct has no body here. */
typedef struct stack stack;
stack *stack_new(void);
void stack_push(stack *s, int v);
int stack_pop(stack *s);
int stack_is_empty(const stack *s);
void stack_free(stack *s);

/* client code: uses only the functions above */
static void drain(const char *name, stack *s) {
    for (int v = 1; v <= 3; v++) {
        stack_push(s, v);
    }
    printf("%s:", name);
    while (!stack_is_empty(s)) {
        printf(" %d", stack_pop(s));
    }
    printf("\n");
}

/* stack.c: one implementation, on linked cells. */
struct cell {
    int v;
    struct cell *rest;
};
struct stack {
    struct cell *top;
};

stack *stack_new(void) {
    stack *s = malloc(sizeof *s);
    if (s == NULL) {
        exit(1);
    }
    s->top = NULL;
    return s;
}
void stack_push(stack *s, int v) {
    struct cell *c = malloc(sizeof *c);
    if (c == NULL) {
        exit(1);
    }
    c->v = v;
    c->rest = s->top;
    s->top = c;
}
int stack_pop(stack *s) {
    struct cell *c = s->top;
    int v = c->v;
    s->top = c->rest;
    free(c);
    return v;
}
int stack_is_empty(const stack *s) { return s->top == NULL; }
void stack_free(stack *s) {
    while (!stack_is_empty(s)) {
        stack_pop(s);
    }
    free(s);
}

int main(void) {
    stack *s = stack_new();
    drain("LinkedStack", s);
    stack_free(s);
    return 0;
}
```

```output for=stack-adt
LinkedStack: 3 2 1
```

- Verdict: idiom. The header declares `struct stack` without its fields, so code that includes only the header can hold a `stack *` but cannot look inside it. The `.c` file gives the body.
- A C program has one implementation for each set of function names. A second implementation needs other names, or a struct of function pointers.
- This example puts the header, the client and the implementation in one file. `drain` comes before the struct body, so the compiler stops it from reading the fields.
