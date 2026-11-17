---
tag: 51AWVE
kind: feature
owner: python
slug: testing-with-pytest
name: Testing with pytest
aliases: [pytest, plain assert, assertion rewriting, pytest fixtures, conftest.py, pytest.mark.parametrize, pytest.raises, tmp_path, monkeypatch, skipif, xfail]
area: languages/python/testing
level: core
summary: >-
  pytest runs plain functions named test_* as tests. A bare assert reports the values that failed, fixtures arrive as arguments, parametrize repeats a test over a table, and marks skip or xfail tests.
grammar: null
spec: {standard: null, url: "https://docs.pytest.org/en/stable/getting-started.html", locator: "Get Started"}
see_also: [pytest, python/unittest, python/quality-control, python/unittest-mock, python/assert-statement]
sources:
  - url: https://docs.pytest.org/en/stable/getting-started.html
    title: Get Started, pytest documentation
    locator: Run multiple tests; Request a unique temporary directory for functional tests
    accessed: 2026-11-16
  - url: https://docs.pytest.org/en/stable/how-to/assert.html
    title: How to write and report assertions in tests, pytest documentation
    locator: Asserting with the assert statement; Assertions about expected exceptions; Assertion introspection details
    accessed: 2026-11-16
  - url: https://docs.pytest.org/en/stable/how-to/fixtures.html
    title: How to use fixtures, pytest documentation
    locator: Requesting fixtures; conftest.py; Teardown/Cleanup, yield fixtures
    accessed: 2026-11-16
  - url: https://docs.pytest.org/en/stable/how-to/parametrize.html
    title: How to parametrize fixtures and test functions, pytest documentation
    locator: pytest.mark.parametrize
    accessed: 2026-11-16
  - url: https://docs.pytest.org/en/stable/how-to/mark.html
    title: How to mark test functions with attributes, pytest documentation
    locator: builtin markers; Registering marks
    accessed: 2026-11-16
  - url: https://docs.pytest.org/en/stable/how-to/monkeypatch.html
    title: How to monkeypatch/mock modules and environments, pytest documentation
    locator: helper methods
    accessed: 2026-11-16
  - url: https://pkg.go.dev/testing
    title: Package testing, Go standard library
    locator: package overview; Subtests and Sub-benchmarks
    accessed: 2026-11-16
---

## Synopsis

```python norun reason=fragment group=cart file=test_cart.py
def total(prices, discount=0):
    return round(sum(prices) * (1 - discount), 2)


def test_total():
    assert total([1.5, 2.5]) == 4.0


def test_discount():
    prices = [10, 20]
    assert total(prices, discount=0.1) == 28.0
```

```python run group=cart file=main.py id=synopsis
import pytest

code = pytest.main(["-qq", "-rA", "-p", "no:cacheprovider", "test_cart.py"])
print("exit code:", int(code))
```

```output for=synopsis
.F                                                                       [100%]
=================================== FAILURES ===================================
________________________________ test_discount _________________________________

    def test_discount():
        prices = [10, 20]
>       assert total(prices, discount=0.1) == 28.0
E       assert 27.0 == 28.0
E        +  where 27.0 = total([10, 20], discount=0.1)

test_cart.py:11: AssertionError
==================================== PASSES ====================================
=========================== short test summary info ============================
PASSED test_cart.py::test_total
FAILED test_cart.py::test_discount - assert 27.0 == 28.0
exit code: 1
```

A test is a plain function whose name starts with `test`, and a check is a plain `assert`. The second test is wrong on purpose. pytest shows the failing line, the two values it compared, and the call that produced 27.0. The usual command is `pytest` in the project directory; this page calls `pytest.main` from a script so that the report can be shown, and the [[pytest]] page explains the options.

## Semantics

**r1.** "pytest will run all files of the form test_*.py or *_test.py in the current directory and its subdirectories."[^1] In those files it collects the functions whose names start with `test`. There is no base class to subclass and no runner call at the end of the file.

**r2.** "pytest allows you to use the standard Python assert for verifying expectations and values in Python tests."[^2] When an assert fails, pytest shows "the values of the most common subexpressions including calls, attributes, comparisons, and binary and unary operators".[^2] It gets them by changing the test code: "Reporting details about a failing assertion is achieved by rewriting assert statements before they are run."[^2] "pytest only rewrites test modules directly discovered by its test collection process, so asserts in supporting modules which are not themselves test modules will not be rewritten."[^2]

**r3.** `pytest.raises(ExceptionType)` used as a context manager checks that the block raises. Its `match` argument is a regular expression that is "matched with the re.search() function" against the string of the exception.[^2] "Note that pytest.raises will match the exception type or any subclasses (like the standard except statement)."[^2]

**r4.** A fixture is a function marked `@pytest.fixture`. "At a basic level, test functions request fixtures they require by declaring them as arguments."[^3] pytest "looks at the parameters in that test function’s signature, and then searches for fixtures that have the same names as those parameters."[^3] A fixture can request other fixtures the same way. `tmp_path` is a built-in one: "Before the test runs, pytest creates a unique-per-test-invocation temporary directory".[^1]

**r5.** A fixture that uses `yield` instead of `return` cleans up after the test: "Any teardown code for that fixture is placed after the yield."[^3] A fixture in a `conftest.py` file is shared, "so that tests from multiple test modules in the directory can access the fixture function".[^3] The test files do not import it.

**r6.** "The builtin pytest.mark.parametrize decorator enables parametrization of arguments for a test function."[^4] The decorator takes the argument names and a list of value tuples, and pytest runs the test once per tuple. Each run has its own id in brackets, made from the values or from the `ids` argument.

**r7.** Marks attach metadata to tests. The builtin ones include "skip - always skip a test function", "skipif - skip a test function if a certain condition is met" and "xfail - produce an “expected failure” outcome if a certain condition is met".[^5] Custom marks "are commonly used to select tests on the command-line with the -m option."[^5] "Marks can only be applied to tests, having no effect on fixtures."[^5]

**r8.** The `monkeypatch` fixture sets or deletes attributes, dictionary items and environment variables for one test. "All modifications will be undone after the requesting test function or fixture has finished."[^6]

## Examples

### Fixtures, a conftest.py and parametrize

```python norun reason=fragment group=notes file=conftest.py
import pytest


@pytest.fixture
def notes(tmp_path):
    path = tmp_path / "notes.txt"
    path.write_text("first\n", encoding="utf-8")
    yield path
    print("cleanup", path.name)
```

```python norun reason=fragment group=notes file=test_notes.py
import pytest


def test_read(notes):
    assert notes.read_text(encoding="utf-8") == "first\n"


def test_append(notes):
    with notes.open("a", encoding="utf-8") as f:
        f.write("second\n")
    assert notes.read_text(encoding="utf-8").splitlines() == ["first", "second"]


@pytest.mark.parametrize(
    "text, words",
    [("a b", 2), ("", 0), ("  x  ", 1)],
    ids=["two", "empty", "padded"],
)
def test_word_count(text, words):
    assert len(text.split()) == words


def test_bad_int():
    with pytest.raises(ValueError, match="invalid literal"):
        int("ten")
```

```python run group=notes file=main.py id=notes
import pytest

code = pytest.main(["-qq", "-rA", "-p", "no:cacheprovider", "test_notes.py"])
print("exit code:", int(code))
```

```output for=notes
......                                                                   [100%]
==================================== PASSES ====================================
--------------------------- Captured stdout teardown ---------------------------
cleanup notes.txt
--------------------------- Captured stdout teardown ---------------------------
cleanup notes.txt
=========================== short test summary info ============================
PASSED test_notes.py::test_read
PASSED test_notes.py::test_append
PASSED test_notes.py::test_word_count[two]
PASSED test_notes.py::test_word_count[empty]
PASSED test_notes.py::test_word_count[padded]
PASSED test_notes.py::test_bad_int
exit code: 0
```

`test_read` and `test_append` each get a new file in their own `tmp_path`, so the line that `test_append` adds is not seen by `test_read`. The code after `yield` runs once per test, which is why "cleanup notes.txt" appears twice. `test_word_count` runs three times, with the ids from `ids`.

### Marks and monkeypatch

```python norun reason=fragment group=marks file=test_marks.py
import os
import sys

import pytest


def greeting():
    return "hello " + os.environ.get("APP_USER", "guest")


def test_default(monkeypatch):
    monkeypatch.delenv("APP_USER", raising=False)
    assert greeting() == "hello guest"


def test_from_env(monkeypatch):
    monkeypatch.setenv("APP_USER", "ada")
    assert greeting() == "hello ada"


@pytest.mark.skipif(sys.platform != "win32", reason="Windows only")
def test_windows():
    assert os.sep == "\\"


@pytest.mark.xfail(reason="float rounding")
def test_round():
    assert round(2.675, 2) == 2.68
```

```python run group=marks file=main.py id=marks
import pytest

code = pytest.main(["-qq", "-rA", "-p", "no:cacheprovider", "test_marks.py"])
print("exit code:", int(code))
```

```output for=marks
..sx                                                                     [100%]
==================================== PASSES ====================================
=========================== short test summary info ============================
PASSED test_marks.py::test_default
PASSED test_marks.py::test_from_env
SKIPPED [1] test_marks.py:21: Windows only
XFAIL test_marks.py::test_round - float rounding
exit code: 0
```

`s` is a skipped test and `x` an expected failure; neither makes the run fail. The two `monkeypatch` tests change `APP_USER` in opposite ways, and neither change leaks into the other test or into the rest of the process.

## Pitfalls

- Asserts in a helper module are not rewritten (r2). A failed `assert` in `helpers.py` gives a bare `AssertionError`. Keep the asserts in the test files, or have the helper return a value that the test asserts on.
- `pytest.raises(Exception)` passes for any subclass (r3), so it also passes when the code fails for an unrelated reason, such as a `TypeError` from a typo. Name the narrowest exception, and add `match`.
- Every parameter of a test function is a request for a fixture of that name (r4). A misspelled parameter asks for a fixture that does not exist, and an extra parameter that was meant to have a default asks pytest for a fixture too.
- Code before `yield` in a fixture is setup. The pytest docs warn about a fixture that does many setup steps: "if any of those steps in the setup raise an exception, none of the teardown code will run."[^3] Split such setup into several small fixtures, one per resource.
- Changing `os.environ` or a module attribute directly in a test leaks into the tests that run after it. Use `monkeypatch`, which undoes the change (r8).

## Comparison

Go's `testing` package also uses plain functions found by name: `go test` "automates execution of any function of the form func TestXxx(*testing.T)".[^7] Go has no assert statement and no rewriting; a test compares values itself and reports with `t.Errorf`, as in "Within these functions, use T.Error, T.Fail or related methods to signal failure."[^7] Go has no fixtures injected by name; setup is ordinary code at the top of the test. Table-driven tests play the part of `parametrize`: "The T.Run and B.Run methods allow defining subtests and sub-benchmarks, without having to define separate functions for each. This enables uses like table-driven benchmarks and creating hierarchical tests."[^7]

[^1]: Get Started, pytest documentation.
[^2]: How to write and report assertions in tests, pytest documentation.
[^3]: How to use fixtures, pytest documentation.
[^4]: How to parametrize fixtures and test functions, pytest documentation.
[^5]: How to mark test functions with attributes, pytest documentation.
[^6]: How to monkeypatch/mock modules and environments, pytest documentation.
[^7]: Package testing, Go standard library.
