```c run id=scopes
#include <stdio.h>

static const char *level = "file";

static const char *reads_file(void) {
    return level;
}

static void caller(void) {
    const char *level = "caller";
    printf("%s, local %s\n", reads_file(), level);
}

int main(void) {
    caller();
    int x = 1;
    {
        int x = 2;
        printf("x in the block: %d\n", x);
    }
    printf("x after the block: %d\n", x);
    return 0;
}
```

```output for=scopes
file, local caller
x in the block: 2
x after the block: 1
```

- Verdict: native. An identifier declared in a block is in scope from the end of its declarator to the end of the block. An inner declaration of the same identifier hides the outer one in the inner scope.
- The compiler accepts the hiding without a message. GCC and Clang report it only with `-Wshadow`, which `-Wall` does not turn on.

```c wrong id=hidden-total
#include <stdio.h>

int main(void) {
    int prices[] = {3, 4, 5};
    int total = 0;
    for (int i = 0; i < 3; i++) {
        int total = 0;
        total += prices[i];
    }
    printf("total: %d\n", total);
    return 0;
}
```

```output for=hidden-total
total: 0
```

The `int total` in the loop body declares a new variable on each pass. The additions go to it, and the outer `total` stays `0`. Remove the inner declaration.
