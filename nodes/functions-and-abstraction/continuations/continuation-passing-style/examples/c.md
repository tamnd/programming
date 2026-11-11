```c run id=continuations
#include <stdio.h>
#include <stdlib.h>

/* A continuation is a function pointer and the data that it needs. */
typedef void (*cont)(long result, void *env);

struct frame { long n; cont k; void *env; };

static void times_n(long r, void *p)
{
    struct frame *f = p;
    f->k(f->n * r, f->env);
}

static void fact(long n, cont k, void *env)
{
    if (n == 0) {
        k(1, env);
        return;
    }
    struct frame f = { n, k, env };
    fact(n - 1, times_n, &f);   /* f lives until its continuation has run */
}

static void print_with(long r, void *env) { printf("%s%ld\n", (const char *)env, r); }
static void store(long r, void *env) { *(long *)env = r; }

/* Two continuations: one for success, one for failure. */
static void parse(const char *s, void (*ok)(long), void (*fail)(const char *))
{
    char *end;
    long n = strtol(s, &end, 10);
    if (*s == '\0' || *end != '\0')
        fail(s);
    else
        ok(n);
}

static void ok(long n) { printf("ok %ld\n", n + 1); }
static void fail(const char *s) { printf("error: not a number: %s\n", s); }

int main(void)
{
    fact(5, print_with, "5! = ");
    long out = 0;
    fact(10, store, &out);
    printf("10! = %ld\n", out);
    parse("42", ok, fail);
    parse("4x", ok, fail);
    return 0;
}
```

```output for=continuations
5! = 120
10! = 3628800
ok 43
error: not a number: 4x
```

- Verdict: idiom. C has no closures, so a continuation is a pair: a function pointer and a `void *` that points to its data. The `frame` struct holds what a closure would capture, the number and the next continuation. The C library uses the same pair for callbacks, as `qsort` takes a comparison function and POSIX `pthread_create` takes a start function and its argument.
- The frames live on the stack, which is safe here because every continuation runs before `fact` returns. A continuation that runs later, after an event, must have its data on the heap, and the code that runs it must free the data.
