```c run id=const
#include <stdio.h>

struct point {
    int x;
    int y;
};

static int sum(const int *values, size_t n) {
    int total = 0;
    for (size_t i = 0; i < n; i++) {
        total += values[i];
    }
    return total;
}

int main(void) {
    const struct point p = {1, 2};
    struct point q = p;
    q.y = 5;
    printf("%d %d, %d %d\n", p.x, p.y, q.x, q.y);

    int data[] = {1, 2, 3};
    printf("%d\n", sum(data, 3));

    char copy[] = "abc";
    copy[0] = 'X';
    printf("%s\n", copy);
}
```

```output for=const
1 2, 1 5
6
Xbc
```

- Verdict: native. The `const` qualifier makes an object read-only. The compiler rejects an assignment through a `const` lvalue.
- A `const int *` parameter promises that the function does not change the data through that pointer. The caller's array itself need not be `const`.
- A cast can remove `const`. A change to an object that was defined `const` is then undefined behaviour.
- A change to a string literal is undefined behaviour. `char copy[] = "abc"` makes a changeable array copy. A pointer to the literal does not:

```c fails expect=sanitizer:address id=literal
#include <stdio.h>

int main(void) {
    char *text = "abc";
    text[0] = 'X';
    printf("%s\n", text);
}
```

In this run, AddressSanitizer stops the program with `BUS on unknown address`. Without the sanitizer, the behaviour is still undefined.
