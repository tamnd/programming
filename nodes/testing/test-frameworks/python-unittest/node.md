---
tag: R4NH59
kind: library
slug: python-unittest
name: Python unittest
aliases: [unittest, PyUnit]
area: testing/test-frameworks
level: core
summary: >-
  unittest is the test framework in the Python standard library. It follows JUnit: tests are methods of TestCase subclasses, setUp prepares a fixture, and a runner reports failures and errors.
role: [test-framework]
licence: psf-2.0
repo: https://github.com/python/cpython/tree/main/Lib/unittest
homepage: https://docs.python.org/3/library/unittest.html
first_release: 2001-04-17
history:
  year: 2001
  people: [Steve Purcell]
  note: Steve Purcell wrote PyUnit, a port of JUnit, and it came into the standard library as the module unittest in Python 2.1, in April 2001.
see_also: [test-framework, pytest, doctest-python, unit-testing, test-double]
sources:
  - url: https://docs.python.org/3/library/unittest.html
    title: The Python Standard Library, unittest, Unit testing framework
    locator: introduction; Basic example; Command-Line Interface; Test Discovery; Organizing test code; Skipping tests and expected failures; Distinguishing test iterations using subtests; Classes and functions; Class and Module Fixtures
    accessed: 2026-11-15
  - url: https://docs.python.org/3/library/unittest.mock.html
    title: The Python Standard Library, unittest.mock, mock object library
    locator: introduction
    accessed: 2026-11-15
  - url: https://docs.pytest.org/en/stable/how-to/unittest.html
    title: pytest documentation, How to use unittest-based tests with pytest
    locator: introduction
    accessed: 2026-11-15
  - url: https://github.com/python/cpython/blob/main/Lib/unittest/__init__.py
    title: CPython source, Lib/unittest/__init__.py
    locator: the module docstring
    accessed: 2026-11-15
  - url: https://docs.python.org/3/whatsnew/2.1.html
    title: A.M. Kuchling, What's New in Python 2.1
    locator: Introduction; New and Improved Modules
    accessed: 2026-11-15
---

## What it is

`unittest` is the [[test-framework]] of the Python standard library. Its documentation says that it "was originally inspired by JUnit and has a similar flavor as major unit testing frameworks in other languages."[^1] It needs no install, so any Python program can use it.

The documentation names four ideas that the module supports "in an object-oriented way":[^1]

- A **test fixture** "represents the preparation needed to perform one or more tests, and any associated cleanup actions."[^1]
- A **test case** "is the individual unit of testing."[^1] A test case is a subclass of `unittest.TestCase`.
- A **test suite** "is a collection of test cases, test suites, or both."[^1]
- A **test runner** "is a component which orchestrates the execution of tests and provides the outcome to the user."[^1]

## Install and first use

`unittest` comes with Python, so there is nothing to install. A test is a method whose name starts with `test`, in a subclass of `unittest.TestCase`. The method `setUp` prepares the fixture, and methods such as `assertEqual` and `assertRaises` do the checks.

```python test id=first-use
import unittest


class InsufficientFunds(Exception):
    pass


class Account:
    def __init__(self, balance):
        self.balance = balance

    def deposit(self, amount):
        self.balance += amount

    def withdraw(self, amount):
        if amount > self.balance:
            raise InsufficientFunds(f"balance {self.balance}, asked {amount}")
        self.balance -= amount


class AccountTest(unittest.TestCase):
    def setUp(self):
        self.account = Account(100)

    def test_deposit(self):
        self.account.deposit(50)
        self.assertEqual(self.account.balance, 150)

    def test_withdraw(self):
        self.account.withdraw(30)
        self.assertEqual(self.account.balance, 70)

    def test_withdraw_more_than_balance(self):
        with self.assertRaises(InsufficientFunds):
            self.account.withdraw(150)


if __name__ == "__main__":
    unittest.main()
```

```output stream=stderr
test_deposit (test_main.AccountTest.test_deposit) ... ok
test_withdraw (test_main.AccountTest.test_withdraw) ... ok
test_withdraw_more_than_balance (test_main.AccountTest.test_withdraw_more_than_balance) ... ok

----------------------------------------------------------------------
Ran 3 tests in $TIME

OK
```

This page runs the file with `python -m unittest -v`. The option `-v` prints one line for each test. The report goes to standard error, and its last line is `OK` when all tests pass. `unittest.main()` at the end of the file lets `python test_account.py` run the tests too.

## Concepts of the tool

**One instance for each test.** The fixture is new for each test: "A new TestCase instance is created as a unique test fixture used to execute each individual test method. Thus setUp() , tearDown() , and TestCase.__init__() will be called once per test."[^1] `tearDown` cleans up after each test, and "If setUp() succeeded, tearDown() will be run whether the test method succeeded or not."[^1] `addCleanup` registers a function that runs after `tearDown`, and `enterContext` enters a context manager and registers its exit in the same way.[^1] For an expensive fixture, the class methods `setUpClass` and `tearDownClass` run once for a class, and the functions `setUpModule` and `tearDownModule` run once for a module.[^1]

**Assertion methods.** `TestCase` has an assertion method for each common check, such as `assertEqual`, `assertTrue`, `assertIn`, `assertIsNone`, `assertAlmostEqual` and `assertRaises`.[^1] Each method writes a message about the values when the check fails. `assertEqual` uses a special comparison for some types, so a failure on two lists or two strings shows the difference between them.[^1]

**Failures and errors.** The runner keeps two lists. The documentation of `TestResult` says that `errors` holds tests that "raised an unexpected exception", and `failures` holds tests "where a failure was explicitly signalled using the assert* methods".[^1] The following program has one of each. Its `withdraw` has a bug at the limit: it does not let a customer withdraw the whole balance.

```python run id=failures-and-errors
import io
import unittest


class InsufficientFunds(Exception):
    pass


class Account:
    def __init__(self, balance):
        self.balance = balance

    def deposit(self, amount):
        # The first bug: no check for a negative amount.
        self.balance += amount

    def withdraw(self, amount):
        # The second bug: >= in place of >.
        if amount >= self.balance:
            raise InsufficientFunds(f"balance {self.balance}, asked {amount}")
        self.balance -= amount


class AccountTest(unittest.TestCase):
    def test_deposit_rejects_negative(self):
        with self.assertRaises(ValueError):
            Account(100).deposit(-10)

    def test_withdraw_leaves_the_rest(self):
        for amount, rest in [(30, 70), (100, 0), (60, 40)]:
            with self.subTest(amount=amount):
                account = Account(100)
                account.withdraw(amount)
                self.assertEqual(account.balance, rest)


suite = unittest.defaultTestLoader.loadTestsFromTestCase(AccountTest)
result = unittest.TextTestRunner(stream=io.StringIO()).run(suite)
print("tests run:", result.testsRun)
for test, trace in result.failures:
    print("failure:", test)
    print("   ", trace.strip().splitlines()[-1])
for test, trace in result.errors:
    print("error:", test)
    print("   ", trace.strip().splitlines()[-1])
print("successful:", "yes" if result.wasSuccessful() else "no")
```

```output for=failures-and-errors
tests run: 2
failure: test_deposit_rejects_negative (__main__.AccountTest.test_deposit_rejects_negative)
    AssertionError: ValueError not raised
error: test_withdraw_leaves_the_rest (__main__.AccountTest.test_withdraw_leaves_the_rest) (amount=100)
    InsufficientFunds: balance 100, asked 100
successful: no
```

The first test is a failure: `assertRaises` signalled that no `ValueError` came. The second test is an error, because `withdraw` raised an exception that the test did not expect. The rows with 30 and 60 passed, and only the row with 100 is in the report.

**Subtests.** The context manager `subTest` marks each row of a table as its own case. The documentation says: "Without using a subtest, execution would stop after the first failure, and the error would be less easy to diagnose because the value of i wouldn’t be displayed".[^1] In the program above, the report names the row with `(amount=100)`.

**Skips and expected failures.** The decorators `skip`, `skipIf` and `skipUnless` skip a test or a class, and a test can call `self.skipTest`. The decorator `expectedFailure` marks a test that must fail; if it passes, the runner reports an unexpected success.[^1]

**Loaders, suites and runners.** A `TestLoader` makes a `TestSuite` from a class, a module or a directory, and a runner such as `TextTestRunner` runs the suite and returns a `TestResult`.[^1] Most projects let `unittest.main()` or `python -m unittest` do these steps. The program above does them in its own code, to read the result.

**Mock objects.** The module `unittest.mock` "allows you to replace parts of your system under test with mock objects and make assertions about how they have been used."[^2] See [[test-double]].

## Common tasks

Run the tests from the command line:[^1]

```text
python -m unittest
python -m unittest -v test_account
python -m unittest test_account.AccountTest.test_deposit
python -m unittest discover -s tests -p "*_test.py"
python -m unittest -k withdraw
```

With no arguments, `python -m unittest` is the same as `python -m unittest discover`.[^1] Discovery starts in the current directory and loads the files that match `test*.py`. The test files must be modules or packages that Python can import from the top-level directory of the project.[^1] The options of `discover` are `-s` for the start directory, `-p` for the pattern of the file names and `-t` for the top-level directory.[^1]

Other options of the command line:[^1]

| Option | Effect | Since |
|---|---|---|
| `-b`, `--buffer` | Buffer the output of each test, and show it only when the test fails | 3.2 |
| `-c`, `--catch` | The first Control-C waits for the current test to end and then reports the results | 3.2 |
| `-f`, `--failfast` | Stop the run at the first error or failure | 3.2 |
| `--locals` | Show the local variables in tracebacks | 3.5 |
| `-k` | Run only the tests whose names match a pattern or a substring | 3.7 |
| `--durations N` | Show the N slowest test cases | 3.12 |

The exit status tells a build script the result. `unittest.main` exits with "an exit code indicating success (0) or failure (1) of the tests run. An exit code of 5 indicates that no tests were run or skipped."[^1]

## Configuration

`unittest` has no configuration file. A project sets its options on the command line or in the arguments of `unittest.main`, such as `verbosity`, `failfast`, `buffer` and `exit`.[^1] A test module can also control how the loader builds its suite with the `load_tests` protocol: when a module has a function `load_tests`, the loader calls it to get the tests of the module.[^1] [[doctest-python]] uses this protocol to add doctests to a `unittest` run.

## Alternatives

**pytest** is a library, not part of the standard library. It collects plain functions and uses the plain `assert` statement, and it gives fixtures to tests through argument names. pytest also runs `unittest` tests: it "will automatically collect unittest.TestCase subclasses and their test methods in test_*.py or *_test.py files."[^3] A project can therefore keep its `unittest` tests and add pytest tests next to them. See [[pytest]].

**doctest** is also in the standard library. It checks the examples in docstrings and text files. See [[doctest-python]].

## History and versions

Steve Purcell wrote PyUnit as a port of JUnit to Python. The source of the module still says that it is "based on Erich Gamma's JUnit and Kent Beck's Smalltalk testing framework", with the line "Copyright (c) 1999-2003 Steve Purcell".[^4] PyUnit came into the standard library as the module `unittest` in Python 2.1.[^5] The final release of Python 2.1 was on April 17, 2001.[^5] The notes for that release say: "PyUnit, contributed by Steve Purcell, is a unit testing framework inspired by JUnit, which was in turn an adaptation of Kent Beck’s Smalltalk testing framework."[^5]

The documentation of Python 3 gives the version that added each feature:[^1]

| Python | Change |
|---|---|
| 2.1 | The module `unittest` is added[^5] |
| 3.1 | Skips and expected failures; `assertRaises` as a context manager[^1] |
| 3.2 | Test discovery, and the options `-b`, `-c` and `-f`[^1] |
| 3.4 | Subtests with `subTest`[^1] |
| 3.5 | The option `--locals`[^1] |
| 3.7 | The option `-k`[^1] |
| 3.11 | `enterContext`, `enterClassContext` and `enterModuleContext`[^1] |
| 3.12 | The option `--durations`[^1] |
| 3.14 | Colour in the output by default[^1] |

## References
