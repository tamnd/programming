```c run id=pointers
#include <stdio.h>

static void swap(int *a, int *b) {
    int t = *a;
    *a = *b;
    *b = t;
}

static int sum(const int *values, size_t n) {
    int total = 0;
    for (size_t i = 0; i < n; i++) {
        total += values[i];
    }
    return total;
}

int main(void) {
    int x = 1, y = 2;
    swap(&x, &y);
    int numbers[] = {1, 2, 3, 4};
    printf("%d %d %d\n", x, y, sum(numbers, sizeof numbers / sizeof numbers[0]));
    return 0;
}
```

```output for=pointers
2 1 10
```

- Verdict: native. C passes arguments by value: "each parameter is assigned the value of the corresponding argument". A function changes a variable of the caller only through a pointer to it.
- An array argument becomes a pointer to its first element. The function does not get the length, so the caller passes it as a separate argument.
- C has variadic functions with `...` and `<stdarg.h>`, but they have no type check of the extra arguments. C has no default values and no named arguments.

```c wrong id=array-size
#include <stdio.h>

static size_t count(int values[10]) {
    return sizeof values / sizeof values[0];
}

int main(void) {
    int numbers[10] = {0};
    printf("%zu %zu\n", sizeof numbers / sizeof numbers[0], count(numbers));
    return 0;
}
```

```output for=array-size
10 2
```

The parameter `int values[10]` is adjusted to `int *values`. Inside the function, `sizeof values` is the size of a pointer, 8 bytes on a 64-bit machine, not the size of the array. GCC and Clang warn with `-Wsizeof-array-argument`.
