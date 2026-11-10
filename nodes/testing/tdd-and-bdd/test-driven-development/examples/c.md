```c run id=leap
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>

static bool is_leap(int year) {
    return year % 4 == 0 && (year % 100 != 0 || year % 400 == 0);
}

/* The tests in the order of the to-do list. Each one added one clause. */
static void test_divisible_by_4_is_leap(void) { assert(is_leap(2024)); }
static void test_other_year_is_common(void) { assert(!is_leap(2023)); }
static void test_century_is_common(void) { assert(!is_leap(1900)); }
static void test_century_divisible_by_400_is_leap(void) { assert(is_leap(2000)); }

int main(void) {
    struct {
        const char *name;
        void (*run)(void);
    } tests[] = {
        {"divisible by 4 is leap", test_divisible_by_4_is_leap},
        {"other year is common", test_other_year_is_common},
        {"century is common", test_century_is_common},
        {"century divisible by 400 is leap", test_century_divisible_by_400_is_leap},
    };
    for (size_t i = 0; i < sizeof tests / sizeof tests[0]; i++) {
        tests[i].run();
        printf("pass  %s\n", tests[i].name);
    }
    puts("green");
    return 0;
}
```

```output
pass  divisible by 4 is leap
pass  other year is common
pass  century is common
pass  century divisible by 400 is leap
green
```

- Verdict: idiom. The C standard library has no test runner. A test program is an array of function pointers and `assert`, and frameworks such as Unity, CMocka and Check add the rest.
- A failed `assert` prints the expression, the file and the line, and then calls `abort`. The first failure stops the whole program, so the report shows only one red test at a time.

A test with `assert` can pass with wrong code. When the macro `NDEBUG` is defined before `<assert.h>`, `assert` expands to nothing. Release builds often define it, for example with the flag `-DNDEBUG`.

```c wrong id=ndebug
#define NDEBUG
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>

static bool is_leap(int year) {
    return year % 4 == 0; /* bug: 1900 is not a leap year */
}

int main(void) {
    assert(is_leap(2024));
    assert(!is_leap(2023));
    assert(!is_leap(1900)); /* must fail, but NDEBUG removed it */
    puts("all tests passed");
    return 0;
}
```

```output for=ndebug
all tests passed
```

- The cycle catches this mistake. The test for 1900 must be red before the fix, and here it is never red.
- A test program for C can use its own check macro that does not depend on `NDEBUG`, or the build can compile the tests without that flag.

The first red in C is a compile error, because `is_leap` has no declaration.

```c fails expect=compile id=red
#include <stdio.h>

int main(void) {
    /* Red: the test comes first, so is_leap does not exist yet. */
    if (!is_leap(2024)) {
        puts("FAIL 2024");
    }
    return 0;
}
```

- C89 accepted a call to an undeclared function and assumed that it returns `int`. C99 removed that rule, and the compiler used here rejects the call. Even without the declaration, the link step fails, because no definition of `is_leap` exists.
