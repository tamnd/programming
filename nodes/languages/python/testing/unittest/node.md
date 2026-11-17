---
tag: STEHWH
kind: feature
owner: python
slug: unittest
name: The unittest framework
aliases: [unittest, unittest.TestCase, TestCase, assertEqual, assertRaises, setUp, tearDown, addCleanup, subTest, skipIf, expectedFailure, python -m unittest, test discovery]
area: languages/python/testing
level: core
summary: >-
  unittest is the xUnit framework in the standard library. Tests are methods named test* on TestCase subclasses, checked with assert* methods, with setUp and tearDown, subtests, skips and discovery.
grammar: null
spec: {standard: null, url: "https://docs.python.org/3.15/library/unittest.html", locator: "unittest, Unit testing framework"}
see_also: [python/quality-control, python/testing-with-pytest, python/unittest-mock, python/assert-statement, python-unittest, pytest]
sources:
  - url: https://docs.python.org/3.15/library/unittest.html
    title: unittest, Python 3.15 documentation
    locator: module introduction; basic example; command-line interface; test discovery; organizing test code; skipping tests and expected failures; subtests; TestCase; TestLoader; TestResult
    accessed: 2026-11-16
  - url: https://pkg.go.dev/testing
    title: Package testing, Go standard library
    locator: package overview; Subtests and Sub-benchmarks; T.Cleanup
    accessed: 2026-11-16
---

## Synopsis

```python test id=synopsis
import unittest


def slugify(title):
    return '-'.join(title.lower().split())


class SlugifyTest(unittest.TestCase):
    def test_words(self):
        self.assertEqual(slugify('Hello World'), 'hello-world')

    def test_spaces(self):
        self.assertEqual(slugify('  two  spaces '), 'two-spaces')

    def test_not_a_string(self):
        with self.assertRaises(AttributeError):
            slugify(None)
```

```output for=synopsis stream=stderr
test_not_a_string (test_main.SlugifyTest.test_not_a_string) ... ok
test_spaces (test_main.SlugifyTest.test_spaces) ... ok
test_words (test_main.SlugifyTest.test_words) ... ok

----------------------------------------------------------------------
Ran 3 tests in $TIME

OK
```

A test class subclasses `unittest.TestCase`, and each method whose name starts with `test` is one test. `python -m unittest -v test_main.py` loads the file, runs the methods in name order and writes the report to stderr.

## Semantics

**r1.** The module "supports test automation, sharing of setup and shutdown code for tests, aggregation of tests into collections, and independence of the tests from the reporting framework."[^1] Its four concepts are the test fixture, the test case, the test suite and the test runner. "A test case is the individual unit of testing. It checks for a specific response to a particular set of inputs."[^1]

**r2.** "A test case is created by subclassing unittest.TestCase. The three individual tests are defined with methods whose names start with the letters test. This naming convention informs the test runner about which methods represent tests."[^1] The prefix is the loader attribute `testMethodPrefix`, whose "default value is 'test'."[^1]

**r3.** Checks use the assert methods: "assertEqual() to check for an expected result; assertTrue() or assertFalse() to verify a condition; or assertRaises() to verify that a specific exception gets raised. These methods are used instead of the assert statement so the test runner can accumulate all test results and produce a report."[^1] When both arguments of `assertEqual` are lists, tuples, dicts, sets, frozensets or strings of the same type, "the type-specific equality function will be called in order to generate a more useful default error message".[^1]

**r4.** A failed assert method makes the test a failure. "Any other exceptions will be treated as errors."[^1] The summary line counts the two apart, as `failures=` and `errors=`.

**r5.** `setUp()` runs before each test method and `tearDown()` after it. "If setUp() succeeded, tearDown() will be run whether the test method succeeded or not."[^1] "A new TestCase instance is created as a unique test fixture used to execute each individual test method. Thus setUp(), tearDown(), and TestCase.__init__() will be called once per test."[^1] The methods run in an order "determined by sorting the test method names with respect to the built-in ordering for strings."[^1]

**r6.** `addCleanup(function, /, *args, **kwargs)` registers a function "to be called after tearDown() to cleanup resources used during the test. Functions will be called in reverse order to the order they are added (LIFO)." "If setUp() fails, meaning that tearDown() is not called, then any cleanup functions added will still be called."[^1]

**r7.** `with self.subTest(**params):` marks one iteration of a loop inside a test. The params "are displayed whenever a subtest fails, allowing you to identify them clearly."[^1] "Without using a subtest, execution would stop after the first failure, and the error would be less easy to diagnose".[^1]

**r8.** `@unittest.skip(reason)`, `@unittest.skipIf(condition, reason)` and `@unittest.skipUnless(condition, reason)` skip a test. `@unittest.expectedFailure` marks one that should fail: "If the test fails or errors in the test function itself (rather than in one of the test fixture methods) then it will be considered a success. If the test passes, it will be considered a failure."[^1] "Skipped tests will not have setUp() or tearDown() run around them."[^1]

**r9.** `python -m unittest` takes modules, classes, methods or file paths; "The file specified must still be importable as a module."[^1] The `-v` flag gives one line per test. "When executed without arguments Test Discovery is started".[^1] Discovery looks for files that match the "Pattern to match test files (test*.py default)", and "all of the test files must be modules or packages importable from the top-level directory of the project (this means that their filenames must be valid identifiers)."[^1]

**r10.** The same machinery is available as objects. `TestLoader.loadTestsFromTestCase()` builds a suite, a runner such as `TextTestRunner` runs it, and the `TestResult` it returns has `testsRun`, "The total number of tests run so far", and `wasSuccessful()`, which returns "True if all tests run so far have passed, otherwise returns False."[^1]

## Examples

### Fixtures with setUp and addCleanup

```python test id=fixture
import os
import tempfile
import unittest


class NotesFileTest(unittest.TestCase):
    def setUp(self):
        self.dir = tempfile.TemporaryDirectory()
        self.addCleanup(self.dir.cleanup)
        self.path = os.path.join(self.dir.name, 'notes.txt')

    def test_starts_empty(self):
        self.assertFalse(os.path.exists(self.path))

    def test_write_then_read(self):
        with open(self.path, 'w', encoding='utf-8') as f:
            f.write('hello\n')
        with open(self.path, encoding='utf-8') as f:
            self.assertEqual(f.read(), 'hello\n')
```

```output for=fixture stream=stderr
test_starts_empty (test_main.NotesFileTest.test_starts_empty) ... ok
test_write_then_read (test_main.NotesFileTest.test_write_then_read) ... ok

----------------------------------------------------------------------
Ran 2 tests in $TIME

OK
```

Each test gets a fresh instance and a fresh directory, so the file written by one test is not there in the other. The cleanup removes the directory even when the test fails.

### Subtests report every failing case

```python test fails expect=test-failure id=subtest
import unittest


class NumbersTest(unittest.TestCase):
    def test_even(self):
        for i in range(0, 6):
            with self.subTest(i=i):
                self.assertEqual(i % 2, 0)
```

```output for=subtest stream=stderr
test_even (test_main.NumbersTest.test_even) ...
  test_even (test_main.NumbersTest.test_even) (i=1) ... FAIL
  test_even (test_main.NumbersTest.test_even) (i=3) ... FAIL
  test_even (test_main.NumbersTest.test_even) (i=5) ... FAIL

======================================================================
FAIL: test_even (test_main.NumbersTest.test_even) (i=1)
----------------------------------------------------------------------
Traceback (most recent call last):
  File "$WORK/test_main.py", line 8, in test_even
    self.assertEqual(i % 2, 0)
AssertionError: 1 != 0

======================================================================
FAIL: test_even (test_main.NumbersTest.test_even) (i=3)
----------------------------------------------------------------------
Traceback (most recent call last):
  File "$WORK/test_main.py", line 8, in test_even
    self.assertEqual(i % 2, 0)
AssertionError: 1 != 0

======================================================================
FAIL: test_even (test_main.NumbersTest.test_even) (i=5)
----------------------------------------------------------------------
Traceback (most recent call last):
  File "$WORK/test_main.py", line 8, in test_even
    self.assertEqual(i % 2, 0)
AssertionError: 1 != 0

----------------------------------------------------------------------
Ran 1 test in $TIME

FAILED (failures=3)
```

The loop is one test, but each odd `i` is its own failure with its parameter in the heading. Without `subTest`, the test would stop at `i=1`.

### Skips and expected failures

```python test id=skip
import os
import unittest


class PlatformTest(unittest.TestCase):
    def test_join(self):
        self.assertEqual(os.path.join('a', 'b'), 'a' + os.sep + 'b')

    @unittest.skipIf(os.name != 'nt', 'Windows only')
    def test_drive_letter(self):
        self.assertEqual(os.path.splitdrive('C:\\x')[0], 'C:')

    @unittest.expectedFailure
    def test_known_bug(self):
        self.assertEqual(round(2.675, 2), 2.68)
```

```output for=skip stream=stderr
test_drive_letter (test_main.PlatformTest.test_drive_letter) ... skipped 'Windows only'
test_join (test_main.PlatformTest.test_join) ... ok
test_known_bug (test_main.PlatformTest.test_known_bug) ... expected failure

----------------------------------------------------------------------
Ran 3 tests in $TIME

OK (skipped=1, expected failures=1)
```

A skip and an expected failure both leave the run green, and the summary counts them.

### Running tests from code

```python run id=runner
import io
import unittest


class StackTest(unittest.TestCase):
    def test_push(self):
        self.assertEqual([1] + [2], [1, 2])

    def check_pop(self):
        self.assertEqual([1, 2].pop(), 3)


suite = unittest.TestLoader().loadTestsFromTestCase(StackTest)
result = unittest.TextTestRunner(stream=io.StringIO()).run(suite)
print(result.testsRun, len(result.failures), result.wasSuccessful())
```

```output for=runner
1 0 True
```

The loader finds one test. `check_pop` would fail, but its name does not start with `test`, so it never runs.

## Pitfalls

- A method named `check_pop` or `tset_pop` is not a test, and nothing warns about it. The runner example above reports one test and a green result. Keep the `test` prefix on every test method (r2).
- A bare `assert` in a test fails the test, but the message is only `AssertionError` with no values. `assertEqual` prints both sides and, for lists, dicts and strings, a diff (r3).
- Test files must be importable modules for discovery. A file named `test-parser.py` is not found by `python -m unittest`, because the hyphen does not make a valid identifier (r9).
- Tests should not depend on the order they run in. The order is alphabetical by method name, not the order in the class body (r5), and each test gets a new instance, so attributes set in one test are gone in the next.
- When `setUp` raises, `tearDown` does not run. A resource opened in `setUp` before the failure leaks unless it is released with `addCleanup` (r5, r6).
- An `expectedFailure` test that starts to pass turns the run red, because a passing expected failure "will be considered a failure" (r8, r10). Remove the decorator when the bug is fixed.

## Comparison

The `go test` command "automates execution of any function of the form func TestXxx(*testing.T)".[^2] A Go test is a function, not a method on a class. To write a suite, "create a file that contains the TestXxx functions as described here, and give that file a name ending in" `_test.go`.[^2] Go has no assert methods: "Within these functions, use T.Error, T.Fail or related methods to signal failure."[^2] Subtests come from `t.Run`: "The T.Run and B.Run methods allow defining subtests and sub-benchmarks, without having to define separate functions for each."[^2] Unlike a `subTest` block, a Go subtest can be picked from the command line: "Each subtest and sub-benchmark has a unique name", and the `-run` flag matches it. "Subtests can also be used to control parallelism."[^2] `t.Cleanup` matches `addCleanup`: "Cleanup functions will be called in last added, first called order."[^2]

[^1]: unittest, Python 3.15 documentation.
[^2]: Package testing, Go standard library.
