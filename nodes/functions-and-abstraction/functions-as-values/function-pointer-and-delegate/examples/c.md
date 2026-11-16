```c run id=function-pointer-and-delegate-c
#include <stdio.h>

typedef int (*binop)(int, int);

static int add(int a, int b) { return a + b; }

static int mul(int a, int b) { return a * b; }

static int sub(int a, int b) { return a - b; }

static int apply(binop f, int a, int b) { return f(a, b); }

int main(void) {
    binop op = add;
    printf("%d\n", op(6, 7));
    op = mul;
    printf("%d\n", op(6, 7));
    printf("%d\n", apply(sub, 6, 7));
    return 0;
}
```

```output for=function-pointer-and-delegate-c
13
42
-1
```

- Verdict: native. `binop` is a `typedef` for a pointer to a function that takes two `int` and returns `int`. The variable `op` points first to `add`, then to `mul`.
