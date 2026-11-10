```c run flags="-finstrument-functions" id=instrument
#include <stdio.h>

// With -finstrument-functions the compiler calls these two hooks at the entry
// and the exit of each function. The hooks must not be instrumented themselves.
#define MAX_FUNCS 8
static void *funcs[MAX_FUNCS];
static long counts[MAX_FUNCS];

__attribute__((no_instrument_function)) void __cyg_profile_func_enter(void *fn, void *site) {
    (void)site;
    for (int i = 0; i < MAX_FUNCS; i++) {
        if (funcs[i] == fn || funcs[i] == NULL) {
            funcs[i] = fn;
            counts[i]++;
            return;
        }
    }
}

__attribute__((no_instrument_function)) void __cyg_profile_func_exit(void *fn, void *site) {
    (void)fn;
    (void)site;
}

static int fib(int n) { return n < 2 ? n : fib(n - 1) + fib(n - 2); }

static int square(int x) { return x * x; }

static int sum_squares(int n) {
    int sum = 0;
    for (int i = 1; i <= n; i++) {
        sum += square(i);
    }
    return sum;
}

__attribute__((no_instrument_function)) static long count_of(void *fn) {
    for (int i = 0; i < MAX_FUNCS; i++) {
        if (funcs[i] == fn) {
            return counts[i];
        }
    }
    return 0;
}

int main(void) {
    printf("fib(15) = %d\n", fib(15));
    printf("sum_squares(10) = %d\n", sum_squares(10));
    printf("calls: main %ld, fib %ld, sum_squares %ld, square %ld\n", count_of((void *)main), count_of((void *)fib),
           count_of((void *)sum_squares), count_of((void *)square));
}
```

```output for=instrument
fib(15) = 610
sum_squares(10) = 385
calls: main 1, fib 1973, sum_squares 1, square 10
```

- Verdict: library. The C standard has no profiler. GCC and Clang insert calls to two hooks with `-finstrument-functions`, and tools outside the language do the rest: `perf` samples the stacks on Linux, and `gprof` reads the counts of a program built with `-pg`.
- The hooks receive the address of each function. A table maps addresses to counts, and the program compares them with the addresses of its own functions.
- The GCC manual suggests the attribute `no_instrument_function` for the hooks, so that the compiler does not instrument them.
