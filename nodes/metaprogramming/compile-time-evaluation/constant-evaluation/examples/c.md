```c run id=constexpr-object
#include <stdio.h>

constexpr int rows = 3;
constexpr int cols = 4;
constexpr unsigned int all_bits = -1U;

static_assert(rows * cols == 12, "the grid has 12 cells");
static_assert(sizeof(int) >= 2);

int main(void) {
    int grid[rows * cols] = {0};
    printf("cells: %zu\n", sizeof grid / sizeof grid[0]);
    printf("all_bits: %u\n", all_bits);
    return 0;
}
```

```output for=constexpr-object
cells: 12
all_bits: 4294967295
```

C23 adds `constexpr` for objects and makes `static_assert` a keyword. A `constexpr` object is a named constant, so `rows * cols` is an integer constant expression and `grid` is an ordinary array, not a variable length array. `static_assert` with one argument has no message.

```c fails expect=compile id=minus-one
#include <stdio.h>

constexpr unsigned int minus_one = -1;

int main(void) {
    printf("%u\n", minus_one);
    return 0;
}
```

The value -1 cannot be represented in `unsigned int`. A plain `const unsigned int` accepts this conversion and gets the maximum value. A `constexpr` object must hold its initializer exactly, so the compiler rejects the program. `-1U` is the portable spelling.

```c fails expect=compile id=no-functions
#include <stdio.h>

constexpr int square(int x) {
    return x * x;
}

int main(void) {
    printf("%d\n", square(4));
    return 0;
}
```

C has no `constexpr` functions. A function definition can have only `extern` or `static` as a storage class. The C constant expression also forbids function calls, so a C program computes a constant with a macro or by hand.

- Verdict: native. C23 has `constexpr` objects and `static_assert`. Constant expressions are a fixed set of operators on constants, with no function calls.
- A `const` object is not a named constant in C. After `const int n = 4;`, the declaration `int a[n];` inside a function declares a variable length array.
