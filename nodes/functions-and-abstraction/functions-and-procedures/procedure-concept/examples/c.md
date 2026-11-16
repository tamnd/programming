```c run id=procedure-c
#include <stdio.h>

static void greet(const char *name) {
    printf("Hello, %s!\n", name);
}

int main(void) {
    greet("Ada");
    greet("Alan");
    return 0;
}
```

```output for=procedure-c
Hello, Ada!
Hello, Alan!
```

- Verdict: native. A function with the return type `void` is a procedure. It ends at the end of its body or at `return;`.
