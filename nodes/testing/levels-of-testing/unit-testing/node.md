---
tag: 9JTT3Z
kind: concept
slug: unit-testing
name: Unit testing
aliases: [unit test, developer testing]
area: testing/levels-of-testing
level: core
summary: >-
  A unit test runs one small part of a program, such as a function or a class, in isolation from the rest. It checks the result automatically and reports pass or fail in milliseconds.
applies_to: core-languages
viz: optional
formal: none
history:
  year: 1994
  people: [Kent Beck, Erich Gamma]
  note: Kent Beck described a Smalltalk testing framework, later called SUnit, in The Smalltalk Report in 1994. Beck and Gamma wrote the first version of JUnit in 1997, and the xUnit family of frameworks grew from it.
see_also: [extract-function]
same_as:
  wikipedia: Unit_testing
sources:
  - paper: Kent Beck, Simple Smalltalk Testing, With Patterns, The Smalltalk Report, October 1994
    title: Kent Beck, Simple Smalltalk Testing, With Patterns, The Smalltalk Report, 1994
    locator: the patterns Fixture, Test Case, Check and Test Suite; failures and errors
  - url: https://martinfowler.com/bliki/Xunit.html
    title: Martin Fowler, Xunit, bliki, 17 January 2006
    locator: the origin of SUnit and JUnit
  - url: https://martinfowler.com/bliki/UnitTest.html
    title: Martin Fowler, Unit Test, bliki, 5 May 2014
    locator: what a unit is; solitary and sociable tests; speed
  - url: https://www.artima.com/weblogs/viewpost.jsp?thread=126923
    title: Michael Feathers, A Set of Unit Testing Rules, Artima Weblogs, 9 September 2005
    locator: the list of things that make a test not a unit test
  - book: isbn:9780131495050
    title: Gerard Meszaros, xUnit Test Patterns, Refactoring Test Code, Addison-Wesley, 2007
    locator: Chapter 19, xUnit Basics Patterns, Four-Phase Test (page 358); Test Double
  - url: https://docs.python.org/3/library/unittest.html
    title: The Python Standard Library, unittest
    locator: introduction; test fixture, test case, test suite, test runner
  - url: https://nodejs.org/api/test.html
    title: Node.js documentation, Test runner (node:test)
    locator: introduction; test reporters
  - url: https://doc.rust-lang.org/book/ch11-03-test-organization.html
    title: The Rust Programming Language, 11.3 Test Organization
    locator: Unit Tests; The Tests Module and #[cfg(test)]; Testing Private Functions
  - url: https://pkg.go.dev/testing
    title: Go standard library, package testing
    locator: package overview; T.Run (subtests)
  - url: https://docs.junit.org/current/user-guide/
    title: JUnit User Guide
    locator: Writing Tests; Test Classes and Methods
  - url: https://xunit.net/docs/getting-started/v2/getting-started
    title: xUnit.net documentation, Getting Started
    locator: Fact and Theory
  - url: https://google.github.io/googletest/primer.html
    title: GoogleTest Primer
    locator: Assertions (EXPECT and ASSERT); Simple Tests
  - url: https://martinfowler.com/bliki/TestPyramid.html
    title: Martin Fowler, Test Pyramid, bliki, 1 May 2012
    locator: the shape of a test suite; Mike Cohn, Succeeding with Agile (2009)
  - url: https://docs.python.org/3/reference/simple_stmts.html#the-assert-statement
    title: The Python Language Reference, The assert statement
    locator: 7.3 The assert statement (__debug__ and -O)
---

## Idea

A unit test is a small program that calls one part of another program and checks the result. The part under test is the **unit**: a function, a method, a class, or a small group of classes that work together. The test gives the unit a known input, runs it, and compares what it gets with what it expects. It passes or fails without a person who reads the output.

The idea answers one question: after a change, does each small part still do what it did before? A person can test a program by hand, but that check is slow, and the person cannot repeat it after each change. A unit test is code, so the machine repeats it in milliseconds, as often as needed. A suite of such tests turns "I think this still works" into a fact that the machine checks.

What counts as a unit is a decision of the team, not a rule. Martin Fowler writes that object-oriented code often takes the class as the unit, and functional code often takes the function, but that the team decides what makes sense.[^3] The more stable part of the definition is the isolation. Michael Feathers gave a rule of thumb in 2005: a test is not a unit test if it talks to the database, communicates across the network, touches the file system, cannot run at the same time as the other unit tests, or needs special changes to the environment to run.[^4] He did not call such tests bad. He wanted them kept apart, so that the unit tests stay fast.

## How it works

Each test has the same four parts. Gerard Meszaros calls this the four-phase test: set up the fixture, exercise the system under test, verify the result, and tear down the fixture.[^5] The fixture is the state the test needs, such as an object, a list or a fake clock. In the examples on this page, the fixture is a list of (price, quantity) pairs, the exercise is a call to `cart_total`, and the verify step is an assertion.

A test framework does four jobs around these tests. It **finds** the tests, often by a name prefix (`test_` in Python, `Test` in Go) or by an annotation (`@Test` in Java, `#[test]` in Rust, `[Fact]` in C#). It **runs** each test apart from the others, so that one failure does not stop the rest. It **classifies** each result. And it **reports** the counts, and the details of each failure. The program below does these four jobs in about 20 lines:

```python run id=runner
def cart_total(items):
    total = 0
    for price, quantity in items:
        if quantity < 0:
            raise ValueError(f"negative quantity: {quantity}")
        total += price * quantity
    return total


def test_empty_cart_is_zero():
    assert cart_total([]) == 0


def test_sums_price_times_quantity():
    assert cart_total([(250, 2), (100, 1)]) == 600


def test_wrong_expectation():
    assert cart_total([(250, 2)]) == 600  # a mistake in the test


def test_bad_input():
    cart_total([(250, None)])  # None < 0 raises TypeError


def run(tests):
    counts = {"pass": 0, "fail": 0, "error": 0}
    for test in tests:
        try:
            test()
        except AssertionError:
            outcome = "fail"
        except Exception as error:
            outcome = f"error ({type(error).__name__})"
        else:
            outcome = "pass"
        counts[outcome.split()[0]] += 1
        print(f"{test.__name__}: {outcome}")
    print(counts)


tests = [value for name, value in globals().items() if name.startswith("test_")]
run(tests)
```

```output for=runner
test_empty_cart_is_zero: pass
test_sums_price_times_quantity: pass
test_wrong_expectation: fail
test_bad_input: error (TypeError)
{'pass': 2, 'fail': 1, 'error': 1}
```

The runner finds the tests by their names. It catches the exception of each test, so the third test does not stop the fourth. It also keeps two kinds of bad result apart. A **failure** is a check that did not hold: the test ran to its assertion, and the value was wrong. An **error** is an exception that the test did not expect: here, `None < 0` raises `TypeError` before any check runs. Kent Beck made this distinction in the first description of his Smalltalk framework. He called a failure an anticipated problem and an error a problem that the test did not check for.[^1] The distinction still shows in the report of `unittest`, which prints `F` for a failure and `E` for an error.[^6]

A test checks a fact with an **assertion**. An assertion is a call that does nothing when its condition holds and stops the test with a message when it does not. Frameworks give a set of them: equality, identity, an expected exception, a value close to another within a tolerance. Some languages use a statement of the language: Python `assert`, the Rust macros `assert!` and `assert_eq!`. A good assertion message shows both values, so the reader knows what went wrong without a debugger. The Python `assert` statement does nothing when Python runs with the `-O` option, so production code must not depend on it.[^14]

Tests also need to see the unit. In Rust, the usual unit test is a `tests` module in the same file as the code, marked `#[cfg(test)]`, which lets it call private functions.[^8] In Go, the tests are in a file whose name ends in `_test.go`, in the same package, so they can use the names that are not exported.[^9] In Java and C#, the tests are in a separate class, often in a separate source tree. They call the public members, or the members that the language opens to tests: package-private members in Java, and `internal` members with the `InternalsVisibleTo` attribute in C#.

## In languages

The example files show the same unit and the same three tests in each language: a cart with no items has a total of zero, the total is the sum of price times quantity, and a negative quantity is an error. The table gives the verdict for each tier 1 language.

| Language | Verdict | Form |
|---|---|---|
| Python | stdlib | `unittest`; pytest is a library |
| JavaScript | stdlib | `node:test` and `node:assert` in Node.js; Jest, Vitest and Mocha are libraries |
| TypeScript | stdlib | the JavaScript runners, with types |
| Java | library | JUnit; TestNG |
| C | idiom | a test program with `assert`; Unity, CMocka and Check are libraries |
| C++ | library | GoogleTest; Catch2 |
| C# | library | xUnit.net, NUnit or MSTest, run by `dotnet test` |
| Go | stdlib | package `testing` and the command `go test` |
| Rust | native | the `#[test]` attribute and the libtest harness, run by `cargo test` |
| Kotlin | library | `kotlin.test` on top of JUnit |
| PowerShell | library | Pester |
| Bash | library | bats-core |
| SQL | library | pgTAP for PostgreSQL; tSQLt for SQL Server |
| Regex | n/a | a pattern is tested by the language that uses it |
| HTML/CSS | n/a | markup and style rules are tested through a browser |

Python's `unittest` follows JUnit. Its documentation says that it was originally inspired by JUnit, and it has the same parts: test fixture, test case, test suite and test runner.[^6] Node.js has the module `node:test`, which registers tests with a function call and reports them with a reporter.[^7] Rust is the one language in the table where the compiler itself knows about tests: `rustc --test` builds a harness from the functions marked `#[test]`.[^8] Go puts the runner in the toolchain: `go test` builds a test binary from the `_test.go` files and runs it, and `t.Run` makes a subtest for each row of a table of cases.[^9] In Java, C# and C++, the test framework is a library. JUnit finds methods with the `@Test` annotation,[^10] xUnit.net finds `[Fact]` and `[Theory]` methods,[^11] and GoogleTest registers each `TEST` macro with its runner.[^12]

## Variations and related ideas

**Solitary and sociable tests.** A solitary test replaces each collaborator of the unit with a test double, so that a fault in one class fails only the tests of that class. A sociable test uses the real collaborators. Fowler credits the two terms to Jay Fields, and he writes that early xUnit tests were mostly sociable.[^3] The two styles go with two schools: the mockist school prefers solitary tests, and the classic school prefers sociable ones.[^3]

**Test doubles.** A test double is an object that takes the place of a real collaborator in a test. Meszaros groups them into dummies, stubs, spies, mocks and fakes.[^5] A double makes the test fast, and it makes a slow or random collaborator (a network, a clock, a random number generator) fixed. The cost is that the test checks the unit against the double, and the double can be wrong about the real collaborator.

**Parameterized tests.** One test function runs once for each row of a table of inputs and expected outputs. Go writes the table as a slice of structs and calls `t.Run` for each row.[^9] xUnit.net uses `[Theory]` with an `[InlineData]` attribute for each row.[^11] Each row is a separate test in the report.

**Test-driven development.** In test-driven development, the programmer writes a unit test before the code, sees it fail, writes the code that makes it pass, and then refactors. Fowler describes unit testing as a signature practice of Extreme Programming, which led to test-driven development.[^3] Unit tests are also what makes refactoring safe: a refactoring such as [extract function](/extract-function/) keeps behaviour, and the tests check that it did.

**Other levels of testing.** Integration tests check that units work together and with real services, such as a database. End-to-end tests drive the whole system, often through its user interface. The test pyramid, which Fowler attributes to Mike Cohn, says that a suite should have many more unit tests than broad tests through a user interface, because the broad tests are slower, more expensive to write and more brittle.[^13]

## Pitfalls

**Tests that share state.** When two tests use the same mutable object, the result of one depends on whether the other ran first:

```python wrong id=order
cart = []  # shared by every test


def test_new_cart_is_empty():
    assert cart == []


def test_add_one_item():
    cart.append((250, 2))
    assert sum(price * quantity for price, quantity in cart) == 500


def run(name, tests):
    cart.clear()  # as if each run were a fresh process
    print(name)
    for test in tests:
        try:
            test()
            print(f"  {test.__name__}: pass")
        except AssertionError:
            print(f"  {test.__name__}: fail")


run("order 1", [test_new_cart_is_empty, test_add_one_item])
run("order 2", [test_add_one_item, test_new_cart_is_empty])
```

```output for=order
order 1
  test_new_cart_is_empty: pass
  test_add_one_item: pass
order 2
  test_add_one_item: pass
  test_new_cart_is_empty: fail
```

Both tests are correct alone. In the second order, `test_add_one_item` leaves an item in the shared cart, and `test_new_cart_is_empty` fails. A runner that changes the order, or runs tests on several threads, exposes the bug at random. The fix is a fixture for each test: each test makes its own cart. xUnit.net makes a separate instance of the test class for each test, and `unittest` calls `setUp` before each test method.[^6][^11]

**Tests that depend on the clock, on chance or on the outside world.** A test that calls `sleep`, reads the time of day, uses an unseeded random number, or calls a server can pass today and fail tomorrow with no change to the code. Such a test is called flaky. The fix is to pass the dependency in, so that the test can give a fixed one:

```python run id=clock
import time


class Session:
    def __init__(self, lifetime, clock=time.monotonic):
        self.clock = clock
        self.expires = clock() + lifetime

    def expired(self):
        return self.clock() >= self.expires


class FakeClock:
    def __init__(self):
        self.now = 1000

    def __call__(self):
        return self.now


def test_session_expires_after_its_lifetime():
    clock = FakeClock()
    session = Session(lifetime=30, clock=clock)
    assert not session.expired()
    clock.now += 29
    assert not session.expired()
    clock.now += 1
    assert session.expired()


test_session_expires_after_its_lifetime()
print("pass: no sleep, same result on every run")
```

```output for=clock
pass: no sleep, same result on every run
```

The session takes the clock as a parameter, with the real clock as the default. The test passes a fake clock and moves it by hand, so it checks the edge of the lifetime exactly and needs no wait.

**Tests that check the implementation instead of the behaviour.** A test that asserts which private methods were called, or in which order a mock was used, fails when the code changes in a way that keeps the result. Such tests make refactoring expensive, because each change breaks tests that found no bug. Test the result that a caller can see.

**Tests that cannot fail.** A test with no assertion, or one that catches the exception it should let out, passes for any code. Watch each test fail once, for example by breaking the code on purpose, before trusting it.

**Too much in one test.** A test that checks ten things stops at the first failure, and its name cannot say what broke. One behaviour for each test, with a name that states it (`rejects_negative_quantity`), makes the report read as a list of facts.

## Cost

A unit test runs in the same process as the code, with no disk and no network, so its cost is close to the cost of the calls it makes. In the runs on this page, `unittest` reports 3 tests in less than a millisecond, and `node:test` reports each test in about a millisecond or less. The start of the process and the build cost more than the tests: a test binary in Rust, Go, Java, C# or C++ must be compiled before it runs. Fowler cites the rule of thumb of Kent Beck that the suite that runs before each commit should take no more than ten minutes.[^3] Feathers keeps the slow tests in a separate set for the same reason: a fast set of tests can run after every change.[^4]

The larger cost is the code itself. Tests are code that the team must read, keep in step with the program and fix when they break for reasons that are not bugs. A test that is coupled to the implementation multiplies this cost with each refactoring. Isolation also has a cost: each test double is code that can drift from the real collaborator, and a suite of solitary tests can pass while the units fail to work together. Integration tests cover that gap.

## History

Kent Beck described a testing framework for Smalltalk in his column "Simple Smalltalk Testing: With Patterns" in The Smalltalk Report in 1994.[^1] The column states the patterns that later frameworks kept: a fixture, a test case, a check, and a test suite that collects test cases. The framework became known as SUnit. Fowler writes that the idea started in Smalltalk but stayed mostly unknown outside the Smalltalk community.[^2]

According to Fowler, Beck and Erich Gamma wrote the first version of JUnit on a flight to the OOPSLA conference in 1997, as a pair, and test first.[^2] JUnit spread the idea to Java and then to other languages. The family took the name xUnit from it, and Fowler writes that JUnit was essential to the growth of Extreme Programming and test-driven development.[^2] Python's `unittest` names JUnit as its model.[^6]

In 2005, Feathers published his rules for what is not a unit test.[^4] In 2007, Meszaros collected the patterns and the names of the xUnit style, such as the four-phase test and the kinds of test double, in xUnit Test Patterns.[^5] Later, test runners moved into language toolchains: Go ships `go test`, Rust has `#[test]` and `cargo test`, and Node.js has `node:test`.[^7][^8][^9]

## References
