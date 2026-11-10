```c run id=schema
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

enum { ORIGINAL, GT_TO_GE, GT_TO_LT, LEFT_TO_BEST, START_AT_0, COUNT };
static const char *const names[COUNT] = {"original", "> to >=", "> to <", "xs[i] to xs[best]", "start at 0"};
static int mutant = ORIGINAL;

/* Returns the index of the first largest item; n must be at least 1. */
static size_t argmax(const int *xs, size_t n) {
    size_t best = 0;
    for (size_t i = mutant == START_AT_0 ? 0 : 1; i < n; i++) {
        int left = mutant == LEFT_TO_BEST ? xs[best] : xs[i];
        bool bigger;
        switch (mutant) {
        case GT_TO_GE: bigger = left >= xs[best]; break;
        case GT_TO_LT: bigger = left < xs[best]; break;
        default: bigger = left > xs[best]; break;
        }
        if (bigger) {
            best = i;
        }
    }
    return best;
}

struct test { int xs[3]; size_t want; };
static const struct test suite[] = {{{1, 2, 3}, 2}, {{1, 3, 2}, 1}, {{3, 1, 2}, 0}, {{2, 2, 1}, 0}};

static bool passes(size_t tests) {
    for (size_t t = 0; t < tests; t++) {
        if (argmax(suite[t].xs, 3) != suite[t].want) {
            return false;
        }
    }
    return true;
}

int main(void) {
    const size_t weak = 3, strong = 4; /* the strong suite adds the tie */
    if (!passes(strong)) {
        fprintf(stderr, "the original must pass first\n");
        return 1;
    }
    printf("%-20s%-8s4 tests\n", "mutant", "3 tests");
    int weak_killed = 0, strong_killed = 0;
    for (mutant = GT_TO_GE; mutant < COUNT; mutant++) {
        bool weak_dead = !passes(weak), strong_dead = !passes(strong);
        weak_killed += weak_dead;
        strong_killed += strong_dead;
        printf("%-20s%-8s%s\n", names[mutant], weak_dead ? "killed" : "lived", strong_dead ? "killed" : "lived");
    }
    printf("%-20s%d of 4  %d of 4\n", "killed", weak_killed, strong_killed);
    return 0;
}
```

```output
mutant              3 tests 4 tests
> to >=             lived   killed
> to <              killed  killed
xs[i] to xs[best]   killed  killed
start at 0          lived   lived
killed              2 of 4  3 of 4
```

- Verdict: library. The C standard library has no mutation tool. Mull is a mutation testing tool for C and C++ built on LLVM.
- The global `mutant` and the `switch` form the mutant schema. One build holds all the mutants, so the program compiles once. The `for` loop in `main` sets the global and runs both suites for each mutant.
- `bool` from `<stdbool.h>` converts to `1` or `0` when it is added to an `int`, so `weak_killed += weak_dead` counts the kills.

In C, a mutant can make the program read past the end of an array. The read is undefined behavior. In a plain build the mutant below can give the right answers on all four tests and live. It did so on a Clang build without the sanitizer when this page was written. AddressSanitizer stops it at the first bad read, so a mutation run with the sanitizer kills it.

```c fails expect=sanitizer:address timeout=30 id=off-by-one
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* The mutant: the loop test i < n became i <= n. */
static size_t argmax(const int *xs, size_t n) {
    size_t best = 0;
    for (size_t i = 1; i <= n; i++) {
        if (xs[i] > xs[best]) {
            best = i;
        }
    }
    return best;
}

int main(void) {
    const int cases[][3] = {{1, 2, 3}, {1, 3, 2}, {3, 1, 2}, {2, 2, 1}};
    for (size_t t = 0; t < 4; t++) {
        int *xs = malloc(sizeof cases[t]); /* exactly three ints, as a caller would pass */
        memcpy(xs, cases[t], sizeof cases[t]);
        printf("argmax = %zu\n", argmax(xs, 3));
        free(xs);
    }
    return 0;
}
```

- The mutant changes `i < n` into `i <= n`, the boundary rule that PIT and other tools apply to `<`. On the last pass it reads `xs[3]`.
- Each test array is a heap copy of exactly three `int` values, so the sanitizer sees the read as a heap buffer overflow. Without the sanitizer, the byte after the array decides the result, and a mutation run can report this mutant as lived on one machine and killed on another.
