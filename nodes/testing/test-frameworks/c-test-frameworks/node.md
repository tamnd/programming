---
tag: P9AWWG
kind: tool
slug: c-test-frameworks
name: C test frameworks
aliases: [Unity, Unity Test, CMocka, cmocka, Check, libcheck, Criterion, unit testing in C]
area: testing/test-frameworks
level: advanced
summary: >-
  C has no test framework in its standard library. This page shows tests with assert, a small home-made runner, and the frameworks Unity, CMocka, Check and Criterion.
role: [test-framework]
see_also: [c, test-framework, unit-testing, googletest, catch2-and-doctest-cpp, test-double, xunit, address-sanitizer]
sources:
  - url: https://libcheck.github.io/check/
    title: Check, Unit Testing Framework for C
    locator: Latest Check Release; What is Check?; Supported Platforms
    accessed: 2026-11-15
  - url: https://github.com/Snaipe/Criterion
    title: Criterion, README
    locator: the introduction; Philosophy; Downloads
    accessed: 2026-11-15
  - url: https://github.com/ThrowTheSwitch/Unity
    title: Unity Test, README
    locator: the introduction; Unity Assertion Summary
    accessed: 2026-11-15
  - url: https://cmocka.org/
    title: cmocka, unit testing framework for C
    locator: cmocka is ...; Mission Statement; Features
    accessed: 2026-11-15
  - url: https://man7.org/linux/man-pages/man3/assert.3.html
    title: Linux manual page, assert(3)
    locator: DESCRIPTION; BUGS
    accessed: 2026-11-15
  - url: https://github.com/ThrowTheSwitch/Unity/blob/master/LICENSE.txt
    title: Unity, LICENSE.txt
    locator: the whole file
    accessed: 2026-11-15
  - url: https://github.com/ThrowTheSwitch/Unity/blob/master/docs/UnityGettingStartedGuide.md
    title: Unity, Getting Started Guide
    locator: Overview; Getting Started; Running Test Functions
    accessed: 2026-11-15
  - url: https://github.com/ThrowTheSwitch/Unity/blob/master/docs/UnityAssertionsReference.md
    title: Unity Assertions Reference
    locator: TEST_ASSERT_X_MESSAGE Variants; TEST_IGNORE
    accessed: 2026-11-15
  - url: https://gitlab.com/cmocka/cmocka
    title: cmocka, README
    locator: Features
    accessed: 2026-11-15
  - url: https://gitlab.com/cmocka/cmocka/-/blob/cmocka-2.0.2/CHANGELOG.md
    title: cmocka, CHANGELOG
    locator: 2.0.2; 2.0.0; 0.2.0; Previous Versions (cmockery)
    accessed: 2026-11-15
  - url: https://api.cmocka.org/
    title: cmocka API documentation
    locator: the overview; Output formats
    accessed: 2026-11-15
  - url: https://github.com/libcheck/check/blob/master/COPYING.LESSER
    title: Check, COPYING.LESSER
    locator: the whole file
    accessed: 2026-11-15
  - url: https://libcheck.github.io/check/doc/check_html/check_3.html
    title: Check manual, 3 Tutorial, Basic Unit Testing
    locator: 3.1 How to Write a Test; 3.5 Creating a Suite; 3.6 SRunner Output
    accessed: 2026-11-15
  - url: https://github.com/Snaipe/Criterion/blob/master/LICENSE
    title: Criterion, LICENSE
    locator: the whole file
    accessed: 2026-11-15
  - url: https://criterion.readthedocs.io/en/master/assert.html
    title: Criterion documentation, Assertion reference
    locator: the introduction; Base Assertions
    accessed: 2026-11-15
  - url: https://github.com/libcheck/check/releases
    title: Check releases on GitHub
    locator: 0.15.2; 0.10.0
    accessed: 2026-11-15
  - url: https://github.com/ThrowTheSwitch/Unity/releases
    title: Unity releases on GitHub
    locator: v2.7.0; v2.6.1; v2.6.0; v2.5.0
    accessed: 2026-11-15
  - url: https://github.com/Snaipe/Criterion/releases
    title: Criterion releases on GitHub
    locator: v2.5.0; v2.4.3; v2.4.0
    accessed: 2026-11-15
---

## What it is

The standard library of [[c]] has one tool for tests: the macro `assert` of `<assert.h>`. A test framework for C adds the rest: macros that report a failure and go on, a way to find or list the tests, a summary, and reports for other tools.

C has no classes and no exceptions, so a C framework cannot use the [[xunit]] pattern of [[googletest]] in the same form. A test is a function, a framework registers it with a macro or with a list in `main`, and a crash in a test can stop the whole program unless the framework runs each test in its own process.[^1][^2]

This page shows four frameworks. Unity is small and is made for embedded targets, CMocka adds mock objects, Check and Criterion run each test in a separate process.[^3][^4][^1][^2]

## Install and first use

The first test of many C programs is a program with `assert`. When the expression is false, `assert` prints a message with the file, the function, the line and the expression to standard error, and calls `abort`.[^5] When the macro `NDEBUG` is defined before `<assert.h>` is included, `assert` generates no code, so a release build with `-DNDEBUG` does not run these checks.[^5]

```c run id=assert-tests
#include <assert.h>
#include <stdio.h>
#include <string.h>

struct item {
    const char *name;
    int cents;
    int quantity;
};

int total(const struct item *items, int n) {
    int sum = 0;
    for (int i = 0; i < n; i++) {
        sum += items[i].cents * items[i].quantity;
    }
    return sum;
}

static void test_empty_cart_is_zero(void) {
    assert(total(NULL, 0) == 0);
}

static void test_total_adds_the_lines(void) {
    struct item cart[] = {{"pen", 150, 2}, {"book", 1200, 1}};
    assert(total(cart, 2) == 1500);
}

int main(void) {
    test_empty_cart_is_zero();
    test_total_adds_the_lines();
    puts("all tests passed");
    return 0;
}
```

```output
all tests passed
```

The first failed `assert` stops the program, so the tests after it do not run. The program below shows what a framework adds. The macro `CHECK` uses `__FILE__`, `__LINE__` and `#cond` to report a failure and goes on; a table of function pointers lists the tests; and the exit status is 1 when a check fails, so a build tool sees the failure. The function `average` has a bug: it divides two integers.

```c fails expect=exit:1 id=home-made-runner
#include <stdio.h>

static int checks, failures;

// CHECK records a failure with the file and the line, and the test goes on.
#define CHECK(cond)                                                  \
    do {                                                             \
        checks++;                                                    \
        if (!(cond)) {                                               \
            failures++;                                              \
            printf("%s:%d: check failed: %s\n", __FILE__, __LINE__, #cond); \
        }                                                            \
    } while (0)

// average has a bug: the division is a division of integers.
double average(const int *values, int n) {
    int sum = 0;
    for (int i = 0; i < n; i++) {
        sum += values[i];
    }
    return sum / n;
}

static void test_average_of_equal_values(void) {
    int values[] = {4, 4};
    CHECK(average(values, 2) == 4.0);
}

static void test_average_of_two_values(void) {
    int values[] = {2, 3};
    CHECK(average(values, 2) == 2.5);
}

// The tests are a table of function pointers with their names.
static const struct {
    const char *name;
    void (*run)(void);
} tests[] = {
    {"average_of_equal_values", test_average_of_equal_values},
    {"average_of_two_values", test_average_of_two_values},
};

int main(void) {
    int n = sizeof tests / sizeof tests[0];
    for (int i = 0; i < n; i++) {
        int before = failures;
        tests[i].run();
        printf("%s %s\n", failures == before ? "PASS" : "FAIL", tests[i].name);
    }
    printf("%d checks, %d failures\n", checks, failures);
    return failures == 0 ? 0 : 1;
}
```

```output
PASS average_of_equal_values
main.c:31: check failed: average(values, 2) == 2.5
FAIL average_of_two_values
2 checks, 1 failures
```

Each framework below is a better form of this program. The runner of this site has none of these libraries, so their blocks are text. The outputs were recorded on Linux with GCC 15.2 and Unity 2.7.0, CMocka 2.0.2, Check 0.15.2 and Criterion 2.5.0.

## Concepts of the tool

### Unity

Unity is "a unit testing framework built for C, with a focus on working with embedded toolchains".[^3] Its core is one C file and two headers, which a project adds to its own build.[^3] Its licence is the MIT licence.[^6]

A test file has a function `setUp`, which runs before each test, and a function `tearDown`, which runs after each test.[^7] `main` calls `UNITY_BEGIN()`, then `RUN_TEST` for each test, and then `UNITY_END()`.[^7] Unity also has a Ruby script, `generate_test_runner.rb`, that writes this `main`, but Ruby is not necessary.[^7]

```text
#include "unity.h"

int total(const int *cents, const int *quantity, int n) {
    int sum = 0;
    for (int i = 0; i < n; i++) {
        sum += cents[i] * quantity[i];
    }
    return sum;
}

void setUp(void) {}    // runs before each test
void tearDown(void) {} // runs after each test

static void test_empty_cart_is_zero(void) {
    TEST_ASSERT_EQUAL_INT(0, total(NULL, NULL, 0));
}

static void test_total_adds_the_lines(void) {
    int cents[] = {150, 1200};
    int quantity[] = {2, 1};
    TEST_ASSERT_EQUAL_INT(1500, total(cents, quantity, 2));
}

static void test_total_of_three_pens(void) {
    int cents[] = {150};
    int quantity[] = {3};
    TEST_ASSERT_EQUAL_INT_MESSAGE(500, total(cents, quantity, 1), "three pens");
}

static void test_tax(void) {
    TEST_IGNORE_MESSAGE("the tax rules are not ready");
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_empty_cart_is_zero);
    RUN_TEST(test_total_adds_the_lines);
    RUN_TEST(test_total_of_three_pens);
    RUN_TEST(test_tax);
    return UNITY_END();
}
```

```text
$ cc -std=c11 -Wall -o test_cart test_cart.c unity.c && ./test_cart
test_cart.c:36:test_empty_cart_is_zero:PASS
test_cart.c:37:test_total_adds_the_lines:PASS
test_cart.c:27:test_total_of_three_pens:FAIL: Expected 500 Was 450:three pens
test_cart.c:31:test_tax:IGNORE: the tax rules are not ready

-----------------------
4 Tests 1 Failures 1 Ignored
FAIL
```

The third test expects the wrong number on purpose. Each assertion has a form with the suffix `_MESSAGE`, which adds a message to the failure, and `TEST_IGNORE_MESSAGE` marks a test as ignored.[^8] `main` returns the value of `UNITY_END()`, and here the exit status of the program is 1. Ceedling is a build tool from the same group, ThrowTheSwitch.org, for projects that test with Unity.[^7]

### CMocka

CMocka is "an elegant unit testing framework for C with support for mock objects".[^4] It needs only the standard C library, so it works on embedded platforms too.[^4] When a test causes a signal such as `SIGSEGV`, CMocka recovers and the suite goes on. It does not use `fork()` for this, so it works on platforms without `fork()` and works better with debuggers.[^4] Its licence is the Apache License 2.0.[^9]

A mock function in CMocka takes its return values from a queue. The test puts values in the queue with `will_return_int`, and the mock reads them with `mock_int()`.[^10] In the test below, `read_sensor` stands for a function that reads hardware.

```text
#include <cmocka.h>

// read_sensor reads the hardware. The test gives its own version.
int read_sensor(void);

// alarm_on is the code under test: it calls read_sensor.
int alarm_on(int limit) {
    return read_sensor() > limit;
}

// The mock takes its return values from a queue that the test fills.
int read_sensor(void) {
    return mock_int();
}

static void test_alarm_above_limit(void **state) {
    (void)state;
    will_return_int(read_sensor, 80);
    assert_true(alarm_on(70));
}

static void test_alarm_below_limit(void **state) {
    (void)state;
    will_return_int(read_sensor, 60);
    assert_false(alarm_on(70));
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_alarm_above_limit),
        cmocka_unit_test(test_alarm_below_limit),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}
```

```text
$ cc -std=c11 -Wall -o test_alarm test_alarm.c -lcmocka && ./test_alarm
[==========] tests: Running 2 test(s).
[ RUN      ] test_alarm_above_limit
[       OK ] test_alarm_above_limit
[ RUN      ] test_alarm_below_limit
[       OK ] test_alarm_below_limit
[==========] tests: 2 test(s) run.
[  PASSED  ] 2 test(s).
```

`cmocka_unit_test` puts a test function in an array, and `cmocka_run_group_tests` runs the group, with optional set-up and tear-down functions for the whole group.[^11] The environment variable `CMOCKA_MESSAGE_OUTPUT` selects the output: `STDOUT`, `SUBUNIT`, `TAP` or `XML` for JUnit XML.[^11] The typed functions such as `will_return_int` and `mock_int` came in version 2.0.0; the older `will_return` without a type is deprecated.[^10]

### Check

Check runs each test in a separate address space, so a failed assertion and a crash such as a segmentation fault are both reported.[^1] On a platform without `fork`, Check turns off the features that need it and still works.[^1] Its licence is the GNU Lesser General Public License 2.1.[^12]

A test is between `START_TEST` and `END_TEST`. Tests go into a test case (`TCase`), test cases go into a suite (`Suite`), and a suite runner (`SRunner`) runs them and counts the failures.[^13]

```text
#include <check.h>
#include <stdlib.h>

int total(const int *cents, const int *quantity, int n) {
    int sum = 0;
    for (int i = 0; i < n; i++) {
        sum += cents[i] * quantity[i];
    }
    return sum;
}

START_TEST(test_total_adds_the_lines) {
    int cents[] = {150, 1200};
    int quantity[] = {2, 1};
    ck_assert_int_eq(total(cents, quantity, 2), 1500);
}
END_TEST

START_TEST(test_crash_is_caught) {
    abort(); // the child process of this test stops with a signal
}
END_TEST

int main(void) {
    Suite *s = suite_create("cart");
    TCase *tc = tcase_create("core");
    tcase_add_test(tc, test_total_adds_the_lines);
    tcase_add_test(tc, test_crash_is_caught);
    suite_add_tcase(s, tc);

    SRunner *sr = srunner_create(s);
    srunner_run_all(sr, CK_NORMAL);
    int failed = srunner_ntests_failed(sr);
    srunner_free(sr);
    return failed == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}
```

```text
$ cc -std=c11 -Wall -o check_cart check_cart.c -lcheck -lm -lpthread -lrt && ./check_cart
Running suite(s): cart
50%: Checks: 2, Failures: 0, Errors: 1
check_cart.c:19:E:core:test_crash_is_caught:0: (after this point) Received signal 6 (Aborted)
```

The second test calls `abort`, and only its child process stops. Check reports it as an error, not a failure, and the program goes on to the summary. `CK_NORMAL` prints the summary and one message for each failed test; `CK_VERBOSE` prints the summary and one message for each test.[^13] Check can also write Subunit, TAP and XML.[^1]

### Criterion

Criterion is "A dead-simple, yet extensible, C and C++ unit testing framework".[^2] The macro `Test(suite, name)` registers a test when it is declared, and Criterion supplies `main`, so a test file needs no list of tests.[^2] Each test runs in its own process, so crashes and signals are reported.[^2] It works with C99 and C++11, it can write TAP, and its licence is the MIT licence.[^2][^14]

```text
#include <criterion/criterion.h>

int total(const int *cents, const int *quantity, int n) {
    int sum = 0;
    for (int i = 0; i < n; i++) {
        sum += cents[i] * quantity[i];
    }
    return sum;
}

Test(cart, total_adds_the_lines) {
    int cents[] = {150, 1200};
    int quantity[] = {2, 1};
    cr_assert_eq(total(cents, quantity, 2), 1500);
}

Test(cart, empty_cart_is_zero) {
    cr_expect_eq(total(NULL, NULL, 0), 0, "an empty cart costs nothing");
}
```

```text
$ cc -std=c11 -Wall -o test_cart test_cart.c -lcriterion && ./test_cart --verbose
[----] Criterion v2.5.0
[====] Running 2 tests from cart:
[RUN ] cart::total_adds_the_lines
[PASS] cart::total_adds_the_lines: (0.00s)
[RUN ] cart::empty_cart_is_zero
[PASS] cart::empty_cart_is_zero: (0.00s)
[====] Synthesis: Tested: 2 | Passing: 2 | Failing: 0 | Crashing: 0
```

`cr_assert` marks the test as failed and stops it; `cr_expect` marks it as failed and lets it go on.[^15] Without `--verbose`, the program prints only the line `Synthesis`. Criterion also has parameterized tests and theories.[^2]

### Choosing a framework

| Framework | Registration of tests | Each test in its own process | Mocks | Licence |
|---|---|---|---|---|
| Unity | `RUN_TEST` in `main`, or a generated runner[^7] | no | no | MIT[^6] |
| CMocka | an array of `cmocka_unit_test`[^11] | no; it handles signals in the process[^4] | yes[^4] | Apache 2.0[^9] |
| Check | suites and test cases in `main`[^13] | yes, where `fork` exists[^1] | no | LGPL 2.1[^12] |
| Criterion | automatic, with `Test`[^2] | yes[^2] | no | MIT[^14] |

A C project can also use a C++ framework such as [[googletest]] when it has a C++ compiler; the tests are C++ and call the C functions.

## Common tasks

```text
cc -o test_cart test_cart.c unity.c && ./test_cart          build and run Unity tests
cc -o test_alarm test_alarm.c -lcmocka && ./test_alarm      build and run CMocka tests
CMOCKA_MESSAGE_OUTPUT=TAP ./test_alarm                      CMocka output in TAP
CMOCKA_MESSAGE_OUTPUT=XML CMOCKA_XML_FILE=report.xml ./test_alarm
                                                            CMocka output in JUnit XML
./check_cart                                                run Check tests, one process for each test
./test_cart --verbose                                       Criterion output for each test
cc -DNDEBUG ...                                             turn off assert in a release build
```

The options are from the documentation of each framework.[^11][^5]

## Configuration

These frameworks have no configuration file. Unity is configured with macros at compile time, CMocka with environment variables such as `CMOCKA_MESSAGE_OUTPUT` and `CMOCKA_XML_FILE`, and Check and Criterion with the arguments of their runners.[^8][^11][^13][^2] A test build can also turn on a sanitizer of the compiler, such as [[address-sanitizer]], so that more memory errors stop a test.

## Alternatives

- [[googletest]] and [[catch2-and-doctest-cpp]]: C++ frameworks that can test C code from C++.
- A shell script that runs small C programs and compares their outputs is the simplest test runner, and it is close to the first two programs of this page.

## History and versions

| Version | Change |
|---|---|
| cmockery 0.1, 26 August 2008 | the first release of cmockery; the change log of CMocka lists the cmockery releases as its previous versions[^10] |
| CMocka 0.2.0, 14 January 2013 | the first entry of CMocka in its change log, with a CMake build[^10] |
| Check 0.15.2, August 2020 | the latest release of Check[^1][^16] |
| CMocka 2.0.0, 4 December 2025 | C99 is required; typed mock functions such as `will_return_int`; TAP 14 output; the legacy support for cmockery is removed[^10] |
| Unity 2.7.0, 16 July 2026 | an option of the runner generator that shuffles the order of tests; an optional detail report, the stack, on failures[^17] |
| Criterion 2.5.0, 3 October 2026 | the options `--default-timeout` and `--show-skipped`; the ABI changes, so tests must be compiled again[^18] |

The latest versions on 12 November 2026 were Unity 2.7.0, CMocka 2.0.2 from 12 January 2026, Check 0.15.2, and Criterion 2.5.0.[^17][^10][^16][^18]

## References
