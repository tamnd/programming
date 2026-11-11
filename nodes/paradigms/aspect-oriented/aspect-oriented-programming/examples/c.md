```c run id=wrapper
#include <stdio.h>

typedef int (*op)(int, int);

static int add(int a, int b) { return a + b; }
static int mul(int a, int b) { return a * b; }

struct advised {
    const char *name;
    op fn;
};

static int call(const struct advised *f, int a, int b)
{
    printf("before %s(%d, %d)\n", f->name, a, b);
    int r = f->fn(a, b);
    printf("after %s -> %d\n", f->name, r);
    return r;
}

int main(void)
{
    const struct advised ops[] = { { "add", add }, { "mul", mul } };
    int x = call(&ops[0], 2, 3);
    int y = call(&ops[1], x, 4);
    printf("%d\n", y);
    return 0;
}
```

```output for=wrapper
before add(2, 3)
after add -> 5
before mul(5, 4)
after mul -> 20
20
```

- Verdict: absent. C has no way to attach code to a function without changing its callers. A wrapper that takes a function pointer gives advice for calls that go through it. Toolchains add a few hooks outside the language: the GCC and Clang flag `-finstrument-functions` calls two hook functions at the entry and exit of every function, and the GNU linker option `--wrap` sends calls of a symbol to a wrapper.
