```c run id=strict
#include <stdbool.h>
#include <stdio.h>

static int trace(const char *label, int value) {
    printf("evaluating %s\n", label);
    return value;
}

static int twice(int n) {
    puts("inside twice");
    return 2 * n;
}

int main(void) {
    printf("%d\n", twice(trace("n", 21)));
    bool result = trace("x", 0) && trace("y", 1);
    printf("%d\n", result);
    return 0;
}
```

```output for=strict
evaluating n
inside twice
42
evaluating x
0
```

- Verdict: native. There is a sequence point after the evaluation of all the arguments and before the call, so each argument is a value when the body starts.
- The order of the arguments among themselves is unspecified. A call such as `f(trace("a", 1), trace("b", 2))` can print `b` before `a`, so this example traces only one argument per call.
- `&&`, `||` and `?:` are the non-strict operators of C. A macro can also delay an expression, because it pastes the text in place of the call.
