```c run id=function-signature-c
#include <stdio.h>

static double average(int a, int b);

int main(void) {
    printf("%.1f\n", average(3, 4));
    return 0;
}

static double average(int a, int b) {
    return (a + b) / 2.0;
}
```

```output for=function-signature-c
3.5
```

- Verdict: native. The first line of the program is a prototype: the signature with no body. The compiler checks the call in `main` against it.
