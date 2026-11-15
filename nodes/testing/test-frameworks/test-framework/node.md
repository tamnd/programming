---
tag: P32KX6
kind: concept
slug: test-framework
name: Test framework
aliases: [testing framework, test automation framework, unit testing framework]
area: testing/test-frameworks
level: intro
summary: >-
  A test framework does the work that is the same for every test: it finds the tests, makes a fixture for each one, checks results with assertions, runs the tests and reports the outcome.
applies_to: core-languages
viz: optional
formal: none
history:
  year: 1997
  people: [Kent Beck, Erich Gamma]
  note: Kent Beck built small testing frameworks in Smalltalk. Beck and Erich Gamma wrote the first version of JUnit in 1997, and the ports of JUnit to other languages became the xUnit family.
see_also: [unit-testing, test-double, test-driven-development, property-based-tests, fuzz-testing, mutation-analysis]
sources:
  - url: http://xunitpatterns.com/Test%20Automation%20Framework.html
    title: Gerard Meszaros, Test Automation Framework, xunitpatterns.com
    locator: the problem and the solution statements; How It Works; Why We Do This
    accessed: 2026-11-15
  - url: https://martinfowler.com/bliki/Xunit.html
    title: Martin Fowler, Xunit, bliki, 17 January 2006
    locator: the whole entry
    accessed: 2026-11-15
  - url: https://dannorth.net/blog/introducing-bdd/
    title: Dan North, Introducing BDD, 20 September 2006
    locator: Test method names should be sentences; JBehave emphasizes behaviour over testing
    accessed: 2026-11-15
  - url: http://xunitpatterns.com/Test%20Discovery.html
    title: Gerard Meszaros, Test Discovery, xunitpatterns.com
    locator: How It Works; When To Use It; Testcase Class Discovery
    accessed: 2026-11-15
  - url: https://docs.pytest.org/en/stable/explanation/goodpractices.html
    title: pytest documentation, Good Integration Practices
    locator: Conventions for Python test discovery
    accessed: 2026-11-15
  - url: https://docs.python.org/3/library/unittest.html
    title: The Python Standard Library, unittest, Unit testing framework
    locator: introduction (test fixture, test case, test suite, test runner); Basic example; TestResult, errors and failures; unittest.main
    accessed: 2026-11-15
  - url: http://xunitpatterns.com/Test%20Runner.html
    title: Gerard Meszaros, Test Runner, xunitpatterns.com
    locator: the solution statement; How It Works
    accessed: 2026-11-15
  - url: https://testanything.org/
    title: Test Anything Protocol, home page
    locator: the introduction and the example stream
    accessed: 2026-11-15
  - url: https://nodejs.org/api/test.html
    title: Node.js documentation, Test runner (node:test)
    locator: introduction; describe() and it() aliases; beforeEach
    accessed: 2026-11-15
  - url: https://pkg.go.dev/testing
    title: Go standard library, package testing
    locator: package overview
    accessed: 2026-11-15
  - url: https://doc.rust-lang.org/book/ch11-01-writing-tests.html
    title: The Rust Programming Language, 11.1 How to Write Tests
    locator: Structuring Test Functions
    accessed: 2026-11-15
  - url: https://docs.junit.org/current/user-guide/
    title: JUnit User Guide, Overview
    locator: What is JUnit?
    accessed: 2026-11-15
  - url: https://rspec.info/about/
    title: RSpec, About RSpec, History
    locator: History
    accessed: 2026-11-15
  - url: https://docs.python.org/3/whatsnew/2.1.html
    title: A.M. Kuchling, What's New in Python 2.1
    locator: Introduction; New and Improved Modules (doctest and PyUnit)
    accessed: 2026-11-15
  - url: https://docs.pytest.org/en/stable/reference/exit-codes.html
    title: pytest documentation, Exit codes
    locator: Exit code 5
    accessed: 2026-11-15
---

## Idea

Each automated test has some logic of its own: it calls the code and checks one result. Around that logic are many steps that are the same for every test. Something must find the tests, prepare the objects that each test needs, run the tests one after another, catch the failures and tell a person what went wrong. A test framework does these common steps, so the author of a test writes only the part that is specific to the test. Gerard Meszaros puts it this way: "We use a framework that provides all the mechanisms needed to run the test logic so the test writer only needs to provide the test-specific logic."[^1]

A test framework has these parts:

- **Discovery** finds the tests, for example each function whose name starts with `test`.
- **Fixtures** prepare the objects and the state that a test needs, and clean them up after the test.
- **Assertions** check a result and stop the test with a clear message when the check is false.
- **The runner** runs each test, keeps the tests apart from each other and records the outcome of each one.
- **The report** tells a person which tests passed and which failed, and gives the build an exit status.

Most frameworks belong to one of two families. The **xUnit** family comes from JUnit: tests are methods or functions with names, and the framework calls a setup method before each one.[^2] The **spec** family writes tests as nested `describe` and `it` blocks, with names that read as sentences about behaviour.[^3]

## How it works

Meszaros lists what the framework does: "These mechanisms include the ability to find individual tests, assemble them into a test suite to run, execute each test in turn, verify expected outcomes have occurred, collect and report any test failures or test errors and to clean up when they do occur."[^1] The following program is a test framework in about 30 lines of Python. It is not for real use, but it has each of these parts.

```python run id=tiny-framework
class Failure(Exception):
    """An assertion that does not hold."""


def assert_equal(actual, expected):
    if actual != expected:
        raise Failure(f"expected {expected!r}, got {actual!r}")


# The fixture: the framework calls it again for each test.
def new_account():
    return {"owner": "Ana", "balance": 100}


# The tests. The framework finds them by the prefix of their names.
def test_deposit(account):
    account["balance"] += 50
    assert_equal(account["balance"], 150)


def test_balance_starts_at_100(account):
    assert_equal(account["balance"], 100)


def test_two_deposits(account):
    account["balance"] += 20 + 30
    assert_equal(account["balance"], 160)


def test_currency(account):
    assert_equal(account["currency"], "EUR")


def run(namespace, fixture):
    counts = {"passed": 0, "failed": 0, "errors": 0}
    # Discovery: each function whose name starts with test_, in the order of the file.
    for name in [n for n in namespace if n.startswith("test_")]:
        try:
            namespace[name](fixture())
        except Failure as e:
            counts["failed"] += 1
            print(f"{name} ... FAIL: {e}")
        except Exception as e:
            counts["errors"] += 1
            print(f"{name} ... ERROR: {type(e).__name__}: {e}")
        else:
            counts["passed"] += 1
            print(f"{name} ... ok")
    # The report: a count of each outcome, and an exit status for the caller.
    print(", ".join(f"{k}: {v}" for k, v in counts.items()))
    return 0 if counts["failed"] + counts["errors"] == 0 else 1


print("exit status:", run(dict(globals()), new_account))
```

```output for=tiny-framework
test_deposit ... ok
test_balance_starts_at_100 ... ok
test_two_deposits ... FAIL: expected 160, got 150
test_currency ... ERROR: KeyError: 'currency'
passed: 2, failed: 1, errors: 1
exit status: 1
```

Each part of the program has a counterpart in a real framework:

1. **Discovery.** The runner looks for names with the prefix `test_`. Meszaros calls this Test Discovery: the framework "uses runtime reflection (or compile time knowledge) to discover all the Test Methods that belong to the test suite".[^4] pytest uses the same rule for functions: it collects "test prefixed test functions or methods outside of class".[^5] Newer frameworks often mark each test with an annotation such as `@Test` in Java, or an attribute such as `[Test]` in C#, in place of a rule for names.[^4]
2. **A new fixture for each test.** `test_deposit` adds 50 to the balance, but `test_balance_starts_at_100` still sees 100, because the runner calls `new_account` again for each test. A fixture that tests share lets one test change the result of the next one.
3. **Failures and errors.** The runner keeps two kinds of bad outcome apart. A failure is a check that is false. An error is any other exception, which often means that the test itself is wrong. Python's `unittest` keeps the same two lists: an error is "a test which raised an unexpected exception", and a failure is "a test where a failure was explicitly signalled using the assert* methods".[^6]
4. **The report and the exit status.** The person reads the lines. The build reads the exit status. `unittest.main` exits with "an exit code indicating success (0) or failure (1) of the tests run".[^6]

A real runner adds more: it runs a subset of the tests by name, runs tests in parallel, captures the output of each test, and writes reports in formats that other tools read. Meszaros notes that the runner can get its tests by "Test Enumeration", "Test Discovery" or "Test Selection".[^7] Two common report formats are the Test Anything Protocol, a text stream of `ok` and `not ok` lines,[^8] and JUnit XML, which continuous integration services read.

## In languages

The example files show the same tests in the main framework of each language: a new account for each test, a deposit, a table of withdrawals and a withdrawal that must fail. The table gives the verdict for each tier 1 language.

| Language | Verdict | Framework |
|---|---|---|
| Python | stdlib | `unittest`; pytest is a library |
| JavaScript | stdlib | `node:test` in Node.js; Jest, Vitest and Mocha are libraries |
| TypeScript | stdlib | `node:test` with type stripping; Jest and Vitest are libraries |
| Java | library | JUnit; TestNG |
| C | idiom | a test program with `assert`; Unity, CMocka and Check are libraries |
| C++ | library | GoogleTest; Catch2 and doctest |
| C# | library | xUnit.net, NUnit and MSTest, run by `dotnet test` |
| Go | stdlib | package `testing` and the command `go test` |
| Rust | native | the `#[test]` attribute, run by `cargo test` |
| Kotlin | library | `kotlin.test` on JUnit; Kotest |
| PowerShell | library | Pester |
| Bash | library | bats-core |
| SQL | library | pgTAP for PostgreSQL; tSQLt for SQL Server |
| Regex | n/a | a pattern is tested by the language that uses it |
| HTML/CSS | n/a | markup and style rules are tested through a browser |

The frameworks differ most in where they live. In Python, the standard library has `unittest`, which "was originally inspired by JUnit".[^6] In Node.js, "The node:test module facilitates the creation of JavaScript tests."[^9] Go puts the framework in the toolchain: "Package testing provides support for automated testing of Go packages", and the command `go test` runs the tests.[^10] Rust puts it in the compiler: "To change a function into a test function, add #[test] on the line before fn".[^11] In Java, C# and C++, the framework is a library. JUnit 6 has three parts: "JUnit 6.1.3 = JUnit Platform + JUnit Jupiter + JUnit Vintage".[^12] The platform is "a foundation for launching testing frameworks on the JVM", and Jupiter is the programming model for new tests, with an engine that runs them on the platform.[^12]

The two families also show in the examples. JavaScript uses `describe` and `it`, the words of the spec family. Node.js gives both sets of names: "describe() is an alias for suite() , and it() is an alias for test()".[^9] The other examples use the xUnit style.

## Variations and related ideas

- **The spec family.** Dan North found that test names work best as sentences that start with the word should. At the end of 2003, North started to write "a replacement for JUnit called JBehave", with "a vocabulary built around verifying behaviour".[^3] RSpec for Ruby "began life in 2005 as an experiment by Steven Baker".[^13] Jasmine, Mocha and Jest use the same `describe` and `it` style. See [[test-driven-development]] for the practice behind it.
- **Table-driven and parameterized tests.** One test function takes a table of inputs and expected results, and the framework reports each row as its own case. JUnit has `@ParameterizedTest`, pytest has `@pytest.mark.parametrize`, and Go tests loop over a table with `t.Run`.
- **Fixtures as arguments.** pytest gives a test its fixtures through the names of its parameters, in place of a `setUp` method. A fixture can also request other fixtures. See [[pytest]].
- **Tests in documentation.** Python's [[doctest-python]] runs the examples of an interactive session that are in a docstring, and checks that the output is the same.[^14]
- **Generated inputs.** A [[property-based-tests]] library generates many inputs for one test and shrinks a failing input to a small one. It runs inside a normal test framework.
- **Test doubles.** A framework often comes with a library for [[test-double]] objects, such as `unittest.mock` in Python or gMock with GoogleTest.

## Pitfalls

**A test that the framework does not find never runs, and the run is still green.** Discovery goes by a rule, and a test that breaks the rule is not an error. In the following program, a method name starts with `check` and not with `test`. The function `withdraw` has a bug: it lets the balance go below zero. The test that would find the bug never runs.

```python wrong id=not-found
import io
import unittest


def withdraw(balance, amount):
    # The bug: no check that the balance is large enough.
    return balance - amount


class WithdrawTest(unittest.TestCase):
    def test_withdraw_part(self):
        self.assertEqual(withdraw(100, 30), 70)

    # The name does not start with "test", so the loader does not see it.
    def check_withdraw_too_much(self):
        with self.assertRaises(ValueError):
            withdraw(100, 150)


suite = unittest.defaultTestLoader.loadTestsFromTestCase(WithdrawTest)
result = unittest.TextTestRunner(stream=io.StringIO()).run(suite)
print("tests run:", result.testsRun)
print("successful:", "yes" if result.wasSuccessful() else "no")
```

```output for=not-found
tests run: 1
successful: yes
```

Read the count of tests in the report, not only the colour. When the name of the method starts with `test`, the run has 2 tests and fails, as it should:

```python run id=found
import io
import unittest


def withdraw(balance, amount):
    return balance - amount


class WithdrawTest(unittest.TestCase):
    def test_withdraw_part(self):
        self.assertEqual(withdraw(100, 30), 70)

    def test_withdraw_too_much(self):
        with self.assertRaises(ValueError):
            withdraw(100, 150)


suite = unittest.defaultTestLoader.loadTestsFromTestCase(WithdrawTest)
result = unittest.TextTestRunner(stream=io.StringIO()).run(suite)
print("tests run:", result.testsRun)
print("successful:", "yes" if result.wasSuccessful() else "no")
```

```output for=found
tests run: 2
successful: no
```

The failure now shows the bug in `withdraw`, and the fix is to raise `ValueError` when the amount is larger than the balance. When no test at all is found, `unittest` reports it: "An exit code of 5 indicates that no tests were run or skipped."[^6] pytest uses exit code 5 for the same case: "No tests were collected".[^15]

**A test that catches every exception also catches its own failure.** An assertion stops a test by raising an exception. A broad `except` in the test catches that exception, and the test passes.

```python wrong id=catch-all
import io
import unittest


def withdraw(balance, amount):
    # The same bug: no check that the balance is large enough.
    return balance - amount


class WithdrawTest(unittest.TestCase):
    def test_withdraw_too_much(self):
        try:
            withdraw(100, 150)
            self.fail("withdraw gave no error")
        except Exception:
            pass  # This also catches the AssertionError of self.fail.


suite = unittest.defaultTestLoader.loadTestsFromTestCase(WithdrawTest)
result = unittest.TextTestRunner(stream=io.StringIO()).run(suite)
print("tests run:", result.testsRun)
print("successful:", "yes" if result.wasSuccessful() else "no")
```

```output for=catch-all
tests run: 1
successful: yes
```

Use the assertion of the framework for an expected exception, and name the type of the exception: `with self.assertRaises(ValueError):` in `unittest`, `pytest.raises` in pytest, `assertThrows` in JUnit and `assert.throws` in Node.js. With that change, this test fails and shows the bug.

**Tests that share state depend on their order.** When two tests use the same object, the first test can change what the second one sees. The tests then pass in one order and fail in another. Make a new fixture for each test, as the frameworks do by default. [[unit-testing]] shows an example.

## Cost

A framework costs little when it runs, but it does work before the first test starts. Discovery must load the code to find the tests: in `unittest`, "Test discovery loads tests by importing them."[^6] A large suite therefore pays to import each test module, even to run one test. A new fixture for each test also costs time when the fixture is large, such as a database. Frameworks let a fixture be shared by a class, a module or the whole run, and that brings back the risk of shared state.

The larger cost is in learning. Meszaros writes that the cost to learn a framework "can be reduced if the framework implements a common protocol such as xUnit which makes it easier for us to learn the second or third such framework once we have experience with the first."[^1]

## History

The idea started in Smalltalk. Martin Fowler writes that Kent Beck "would build a simple framework to organize and run unit tests" to help Beck and the clients of Beck test their code.[^2] There was no single framework at first: Beck liked each team to build its own, and "it only took a couple of hours".[^2]

Fowler also tells how JUnit started: it "was born on a flight from Zurich to the 1997 OOPSLA in Atlanta", where Beck and Erich Gamma wrote the first version together.[^2] JUnit spread the idea, and other languages copied it. Fowler remembers "Michael Feathers putting together CppUnit for C++, which may have been the first port".[^2] The family took its name from JUnit: "XUnit is the family name given to bunch of testing frameworks".[^2] Python 2.1 added two test modules in 2001. One was PyUnit by Steve Purcell, "a unit testing framework inspired by JUnit", and the other was `doctest` by Tim Peters.[^14]

The Test Anything Protocol "started life as part of the test harness for Perl".[^8] In the 2000s the spec family grew out of test-driven development: North began JBehave at the end of 2003,[^3] and RSpec started in 2005.[^13] Later languages put the framework in the toolchain from the start, as Go does with `go test`[^10] and Rust does with `#[test]`.[^11]

## References
