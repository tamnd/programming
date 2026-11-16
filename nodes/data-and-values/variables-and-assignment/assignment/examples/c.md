```c run id=expression
#include <stdio.h>

int main(void) {
    int a, b, c;
    a = b = c = 5;
    printf("%d\n", a + b + c);
    int ch;
    const char *text = "hi";
    int i = 0;
    while ((ch = text[i]) != '\0') {
        printf("%c", ch - 32);
        i += 1;
    }
    printf("\n");
    return 0;
}
```

```output for=expression
15
HI
```

- Verdict: native. An assignment is an expression whose value is the new value of the left side, so assignments chain, and a loop can assign and test in one condition. The parentheses around `ch = text[i]` are needed, because `!=` binds tighter than `=`.
