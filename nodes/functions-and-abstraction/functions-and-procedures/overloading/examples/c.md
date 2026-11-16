```c run id=overloading-c
#include <stdio.h>

static void show_int(int x) {
    printf("int %d\n", x);
}

static void show_str(const char *s) {
    printf("string %s\n", s);
}

static void show_two_ints(int x, int y) {
    printf("two ints %d %d\n", x, y);
}

#define show(x) _Generic((x), int: show_int, char *: show_str, const char *: show_str)(x)

int main(void) {
    show(42);
    show("hi");
    show_two_ints(3, 4);
    return 0;
}
```

```output for=overloading-c
int 42
string hi
two ints 3 4
```

- Verdict: idiom. C has no overloading. The `_Generic` selection in the macro `show` chooses a function from the type of the argument at compile time. The two-argument case needs its own name.
