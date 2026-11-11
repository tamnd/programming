```c run id=rpn
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static long stack[64];
static int sp = 0;

static void push(long x) {
    if (sp == 64) { fprintf(stderr, "stack overflow\n"); exit(1); }
    stack[sp++] = x;
}

static long pop(void) {
    if (sp == 0) { fprintf(stderr, "stack underflow\n"); exit(1); }
    return stack[--sp];
}

static const char *defs[][2] = {{"square", "dup *"}, {"cube", "dup square *"}};

static void eval(const char *p) {
    char t[32];
    int used;
    while (sscanf(p, "%31s%n", t, &used) == 1) {
        p += used;
        long a, b;
        int found = 0;
        for (size_t i = 0; i < sizeof defs / sizeof defs[0]; i++)
            if (strcmp(t, defs[i][0]) == 0) { eval(defs[i][1]); found = 1; }
        if (found) continue;
        if (strcmp(t, "+") == 0) { b = pop(); a = pop(); push(a + b); }
        else if (strcmp(t, "*") == 0) { b = pop(); a = pop(); push(a * b); }
        else if (strcmp(t, "dup") == 0) { a = pop(); push(a); push(a); }
        else push(strtol(t, NULL, 10));
    }
}

int main(void) {
    eval("2 3 + square");
    eval("1 cube");
    eval("3 cube +");
    for (int i = 0; i < sp; i++) printf("%ld ", stack[i]);
    printf("\n");
    return 0;
}
```

```output for=rpn
25 28 
```

- Verdict: absent. C applies functions to arguments. A global stack, a table of definitions and a loop over the words give a Forth-like interpreter in a page of code, which is why Forth runs on very small machines. The three calls to `eval` share one stack, so the second and third lines of text continue the first, as in Forth.
