---
tag: BWXSXK
kind: concept
slug: test-double
name: Test double
aliases: [test doubles, imposter]
area: testing/test-doubles
level: core
summary: >-
  A test double is an object that takes the place of a real component in a test. It has the same interface, so the code under test uses it as if it were real.
applies_to: core-languages
viz: optional
formal: none
history:
  year: 2000
  people: [Tim Mackinnon, Steve Freeman, Philip Craig, Gerard Meszaros]
  note: Mackinnon, Freeman and Craig described Mock Objects at the XP2000 conference. Meszaros gave the general name Test Double and the five kinds in xUnit Test Patterns in 2007.
see_also: [unit-testing, dependency-injection, interfaces, proxy-pattern, higher-order-functions, closures]
same_as:
  wikidata: Q11319265
  wikipedia: Test_double
sources:
  - book: isbn:9780131495050
    title: Gerard Meszaros, xUnit Test Patterns, Refactoring Test Code, Addison-Wesley, 2007
    locator: Test Double (page 522), Test Stub (page 529), Test Spy (page 538), Mock Object (page 544), Fake Object (page 551), Dummy Object (page 728)
  - url: http://xunitpatterns.com/Test%20Double.html
    title: Gerard Meszaros, Test Double, xunitpatterns.com
    locator: the problem statement; How It Works; the variations Test Stub, Test Spy, Mock Object, Fake Object and Dummy Object; the warning on Fragile Tests
  - url: https://martinfowler.com/articles/mocksArentStubs.html
    title: Martin Fowler, Mocks Aren't Stubs, revised 2 January 2007
    locator: The Difference Between Mocks and Stubs; Classical and Mockist Testing; Coupling Tests to Implementations; Significant Revisions
  - url: https://docs.python.org/3.14/library/unittest.mock.html#the-mock-class
    title: The Python Standard Library, unittest.mock, Python 3.14
    locator: The Mock Class; return_value; mock_calls; call
  - url: https://docs.python.org/3.14/library/unittest.mock.html#autospeccing
    title: The Python Standard Library, unittest.mock, Python 3.14
    locator: Autospeccing; create_autospec
  - url: http://xunitpatterns.com/Fragile%20Test.html
    title: Gerard Meszaros, Fragile Test, xunitpatterns.com
    locator: "Cause: Overspecified Software"
  - url: https://www2.ccs.neu.edu/research/demeter/related-work/extreme-programming/MockObjectsFinal.PDF
    title: Tim Mackinnon, Steve Freeman and Philip Craig, Endo-Testing, Unit Testing with Mock Objects, XP2000
    locator: Introduction; Not Just Stubs; Limitations of Mock Objects
  - url: https://nodejs.org/api/test.html#mocking
    title: Node.js documentation, Test runner (node:test)
    locator: Mocking; Class MockTracker; Class MockTimers
  - url: https://doc.rust-lang.org/book/ch15-05-interior-mutability.html#a-use-case-for-interior-mutability-mock-objects
    title: The Rust Programming Language, 15.5 RefCell and the Interior Mutability Pattern
    locator: A Use Case for Interior Mutability, Mock Objects
  - url: https://pkg.go.dev/testing/fstest#MapFS
    title: Go standard library, package testing/fstest
    locator: type MapFS
  - url: https://pkg.go.dev/net/http/httptest
    title: Go standard library, package net/http/httptest
    locator: package overview; ResponseRecorder; NewServer
  - url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/lang/reflect/Proxy.html
    title: Java SE 25 API, java.lang.reflect.Proxy
    locator: class description; newProxyInstance
  - url: https://pester.dev/docs/commands/Mock
    title: Pester documentation, Mock
    locator: Description
  - url: https://www.gnu.org/software/bash/manual/bash.html#Command-Search-and-Execution
    title: Bash Reference Manual
    locator: 3.7.2 Command Search and Execution, step 1
  - url: https://www.sqlite.org/datatype3.html#collating_sequences
    title: SQLite documentation, Datatypes In SQLite
    locator: 7. Collating Sequences (NOCASE)
---

## Idea

A unit often depends on other components: a database, a mail server, a clock, a payment service. In a test, these components are a problem. They can be slow, they can be absent, they can give answers that the test cannot control, or they can have effects that the test must not cause, such as a real email. Meszaros calls such a component a depended-on component (DOC). A test double is an object that replaces a DOC during a test. It does not have to behave like the real component. It only has to provide the same interface, so that the system under test (SUT) uses it as if it were real.[^2] The name comes from the stunt double of the film industry.[^2]

A double gives the test two things that the real component does not. It is a control point: the test decides what the DOC returns to the SUT, which Meszaros calls the indirect inputs. It is also an observation point: the test can see what the SUT sends to the DOC, which he calls the indirect outputs.[^2] This page uses the five names that Meszaros defined in *xUnit Test Patterns*.[^1] [[unit-testing]] covers the test itself: its phases, its framework, and when a test with real collaborators is the better choice.

## How it works

The five kinds differ in what the test does with them.[^2]

| Kind | What it does | What the test checks |
|---|---|---|
| Dummy | fills a parameter that the SUT must receive but does not use | nothing; a call to it is a bug |
| Stub | returns canned answers, the indirect inputs | the result or state of the SUT |
| Spy | a stub that also records the calls it gets | the recorded calls, after the SUT has run |
| Mock | holds the expected calls before the SUT runs, and fails at the first unexpected call | the calls, during and at the end of the run |
| Fake | a working but lighter version of the real component, such as a store in memory | the result or state of the SUT |

The function `register` below needs a user store, a mailer, a clock and a log. The test gives it one double of each kind except the mock. The dummy log raises an error if the SUT uses it. On this path the SUT must not write to the log, so the dummy also checks that.

```python run id=kinds
def register(email, users, mailer, clock, log):
    if users.exists(email):
        log.write(f"duplicate: {email}")
        return "taken"
    users.add(email, clock.now())
    mailer.send(email, "Welcome!")
    return "created"


class DummyLog:  # dummy: fills a parameter and must never be used
    def write(self, line):
        raise AssertionError("the dummy log was used")


class StubClock:  # stub: gives a canned answer
    def now(self):
        return "2000-01-01T00:00:00Z"


class SpyMailer:  # spy: records the calls for later checks
    def __init__(self):
        self.sent = []

    def send(self, to, body):
        self.sent.append((to, body))


class FakeUsers:  # fake: a working store that keeps the rows in a dict
    def __init__(self):
        self.rows = {}

    def exists(self, email):
        return email in self.rows

    def add(self, email, created):
        self.rows[email] = created


users, mailer = FakeUsers(), SpyMailer()
print(register("ada@example.com", users, mailer, StubClock(), DummyLog()))
print("state:", users.rows)
print("calls:", mailer.sent)
```

```output for=kinds
created
state: {'ada@example.com': '2000-01-01T00:00:00Z'}
calls: [('ada@example.com', 'Welcome!')]
```

A mock differs from a spy in when it checks. The test gives the mock its expectations before the SUT runs. The mock compares each call with them as it arrives, and a final `verify` reports the calls that did not come. Meszaros says that a mock is more than a stub with assertions, because the test uses it in a different way.[^2] Here the mock expects no mail for an email that is taken. The second version of `register` sends the mail before it checks the store, and the mock stops it at once.

```python run id=mock
def register(email, users, mailer):
    if users.exists(email):
        return "taken"
    users.add(email)
    mailer.send(email, "Welcome!")
    return "created"


def register_buggy(email, users, mailer):
    mailer.send(email, "Welcome!")  # sends before the check
    if users.exists(email):
        return "taken"
    users.add(email)
    return "created"


class MockMailer:  # mock: knows the expected calls before the SUT runs
    def __init__(self, expected):
        self.expected = list(expected)

    def send(self, to, body):
        if not self.expected:
            raise AssertionError(f"unexpected call send({to!r}, {body!r})")
        want = self.expected.pop(0)
        if (to, body) != want:
            raise AssertionError(f"send({to!r}, {body!r}), expected send{want!r}")

    def verify(self):
        if self.expected:
            raise AssertionError(f"calls not made: {self.expected}")


class TakenUsers:  # stub: every email is taken
    def exists(self, email):
        return True


for unit in (register, register_buggy):
    mailer = MockMailer(expected=[])  # a taken email must send no mail
    try:
        unit("ada@example.com", TakenUsers(), mailer)
        mailer.verify()
        print(f"{unit.__name__}: pass")
    except AssertionError as error:
        print(f"{unit.__name__}: fail: {error}")
```

```output for=mock
register: pass
register_buggy: fail: unexpected call send('ada@example.com', 'Welcome!')
```

Fowler names the two styles of check. State verification looks at the result and the state of the SUT and its fakes after the run. Behaviour verification looks at the calls that the SUT made. A mock always uses behaviour verification. A stub can serve either style.[^3]

Most doubles are not written by hand. A mock library builds a configurable double at run time. Python's `unittest.mock.Mock` creates an attribute the first time the test reads it, returns another `Mock` from every call by default, and records each call in `mock_calls`.[^4] The function `create_autospec` copies the attributes and the call signatures of a real class. A call with the wrong arguments then raises `TypeError`, and the assertion methods match a call whether its arguments were passed by position or by name.[^5]

```python run id=autospec
from unittest import mock


class Mailer:
    def send(self, to, body):
        raise NotImplementedError("talks to an SMTP server")


def welcome(email, mailer):
    mailer.send(email, "Welcome!")


mailer = mock.create_autospec(Mailer, instance=True)
welcome("ada@example.com", mailer)
mailer.send.assert_called_once_with("ada@example.com", "Welcome!")
mailer.send.assert_called_once_with(to="ada@example.com", body="Welcome!")
print(mailer.mock_calls)

try:
    mailer.send("ada@example.com")  # one argument missing
except TypeError as error:
    print("TypeError:", error)
```

```output for=autospec
[call.send('ada@example.com', 'Welcome!')]
TypeError: missing a required argument: 'body'
```

The test can put a double in place only through a seam: a point where the code accepts a component from outside. A parameter is the plainest seam, as above. Other seams are an interface with a constructor parameter ([[dependency-injection]]), a function value ([[higher-order-functions]]), a template parameter in C++, and a struct of function pointers in C. Meszaros lists the ways to install a double as the substitutable dependency patterns.[^2] Python and JavaScript can also replace an attribute of a module or an object while a test runs, which `unittest.mock.patch` and `mock.method` of `node:test` do.[^4][^8] Languages differ most in this part, as the examples show.

## In languages

The example files show the same `register` function with a fake store and a spy mailer in each language. The table gives the verdict for each tier 1 language.

| Language | Verdict | Form |
|---|---|---|
| Python | stdlib | hand-written classes, or `unittest.mock` with `Mock`, `create_autospec` and `patch` |
| JavaScript | stdlib | object literals, or `mock.fn` and `mock.method` of `node:test` |
| TypeScript | stdlib | as JavaScript; an object literal that has the shape of an interface is a double |
| Java | idiom | a class that implements an interface, or a `java.lang.reflect.Proxy` |
| C | idiom | a struct of function pointers and a `void *` context |
| C++ | idiom | a subclass that overrides virtual functions, or a type given to a template parameter |
| C# | idiom | a class that implements an interface |
| Go | idiom | any type with the right methods; `testing/fstest` and `net/http/httptest` give fakes |
| Rust | idiom | a type that implements a trait; `RefCell` for a spy behind `&self` |
| Kotlin | idiom | a class or an `object` expression that implements an interface |
| PowerShell | library | the `Mock` command of the Pester module |
| Bash | idiom | a shell function with the name of a command |
| SQL | n/a | a query has no components to replace |
| Regex | n/a | a pattern has no components to replace |
| HTML/CSS | n/a | markup and style rules have no components to replace |

The verdict says what the language and its standard library give. Mock libraries exist for the languages marked idiom, but they are outside the standard library. Rust's book says that the standard library has no mock functionality, and it builds a recording double with `RefCell`.[^9] Go has no mock package, but its standard library has fakes for tests: `fstest.MapFS` is a file system in memory, and `httptest` gives a recorder for HTTP responses and a test server.[^10][^11]

In Bash, the shell looks for a function with the name of a command before it looks for a builtin or a program on `PATH`.[^14] A test script can define a function called `curl` and then source the script under test, so the script calls the function instead of the program. PowerShell has no doubles in the language, and the Pester module's `Mock` command replaces a command with another implementation for the scope of a test.[^13]

## Variations and related ideas

- **Classical and mockist testing.** Fowler describes two schools of test-first work. The classical style uses real objects where possible and a double only where the real object is awkward. The mockist style uses a mock for every object with interesting behaviour.[^3] The choice decides how much a test knows about the code inside the unit.
- **Hand-written and generated doubles.** Meszaros separates hard-coded doubles, written for one test, from configurable doubles, which a test sets up and which tools can generate.[^2] Java's `Proxy` makes an object that implements any interface at run time and sends each call to one handler.[^12] The Java example builds a spy with it.
- **Self Shunt.** The test class itself implements the interface and acts as the double.[^2]
- **Doubles at the edge of the process.** A test can also replace a component outside the process: a local HTTP server, a database in a container, a fake SMTP server. These tests are slower, but they exercise the real client code. `httptest.NewServer` in Go is a fake server of this kind.[^11]
- **Contract tests.** One set of tests runs against both the real component and its fake. When both pass, the fake is a faithful copy for the behaviour that the tests cover (see the last pitfall).

## Pitfalls

### The mock accepts a method that does not exist

A plain `Mock` creates any attribute that the test or the SUT reads. When a method of the real class is renamed, code that still calls the old name keeps passing its tests. The Python documentation describes this flaw and calls it general to mocking.[^5] Here the real method is `deliver`, but the code calls `send`.

```python wrong id=no-spec
from unittest import mock


class Mailer:
    def deliver(self, to, body):  # was called send() before a rename
        raise NotImplementedError("talks to an SMTP server")


def welcome(email, mailer):
    mailer.send(email, "Welcome!")  # still uses the old name


mailer = mock.Mock()
welcome("ada@example.com", mailer)
mailer.send.assert_called_once_with("ada@example.com", "Welcome!")
print("test passed")
```

```output for=no-spec
test passed
```

A mock with a spec only has the attributes of the real class. With `create_autospec`, the same test stops with `AttributeError: Mock object has no attribute 'send'`.[^5]

```python fails expect=exception:AttributeError id=with-spec
from unittest import mock


class Mailer:
    def deliver(self, to, body):
        raise NotImplementedError("talks to an SMTP server")


def welcome(email, mailer):
    mailer.send(email, "Welcome!")


mailer = mock.create_autospec(Mailer, instance=True)
welcome("ada@example.com", mailer)
```

In a statically typed language, the compiler gives the same protection: a double that implements an interface must have its methods, with the right types.

### A default answer that is true

A `Mock` returns another `Mock` from every call.[^4] The run shows that this `Mock` is true in a Boolean context. A test that forgets to set a return value gets an answer that the SUT reads as yes. Here the store says that every email is taken, so `register` never reaches the code that the test wanted to check.

```python wrong id=truthy
from unittest import mock


def register(email, users, mailer):
    if users.exists(email):
        return "taken"
    users.add(email)
    mailer.send(email, "Welcome!")
    return "created"


users = mock.Mock()  # the test forgot users.exists.return_value = False
mailer = mock.Mock()
print(register("ada@example.com", users, mailer))
answer = users.exists("ada@example.com")
print(type(answer).__name__, bool(answer))
```

```output for=truthy
taken
Mock True
```

Set every return value that the path under test reads, or use a hand-written stub or fake whose answers are explicit.

### Over-mocking couples the test to the code

A test that checks each call that the SUT makes describes how the code works, not what it achieves. Meszaros calls this Overspecified Software, a cause of Fragile Tests, and links it to the heavy use of mock objects.[^6] Fowler makes the same point: mockist tests are coupled to the implementation, and a change in the calls to collaborators usually breaks them.[^3] Here `total_v2` is a correct refactoring of `total_v1`. It looks up each distinct product once. The test that checks the calls fails, and the test that checks the result passes.

```python wrong id=overspecified
from unittest import mock


def total_v1(skus, prices):
    return sum(prices.lookup(sku) for sku in skus)


def total_v2(skus, prices):  # refactored: one lookup for each distinct SKU
    cache = {}
    for sku in skus:
        if sku not in cache:
            cache[sku] = prices.lookup(sku)
    return sum(cache[sku] for sku in skus)


def test_interactions(total):  # checks how the price is found
    prices = mock.Mock()
    prices.lookup.side_effect = {"A": 250, "B": 100}.get
    assert total(["A", "B", "A"], prices) == 600
    assert prices.lookup.mock_calls == [mock.call("A"), mock.call("B"), mock.call("A")]


def test_result(total):  # checks what the caller gets
    class StubPrices:
        def lookup(self, sku):
            return {"A": 250, "B": 100}[sku]

    assert total(["A", "B", "A"], StubPrices()) == 600


for total in (total_v1, total_v2):
    for test in (test_interactions, test_result):
        try:
            test(total)
            outcome = "pass"
        except AssertionError:
            outcome = "FAIL"
        print(f"{total.__name__} {test.__name__}: {outcome}")
```

```output for=overspecified
total_v1 test_interactions: pass
total_v1 test_result: pass
total_v2 test_interactions: FAIL
total_v2 test_result: pass
```

Check calls only when the call is the behaviour that matters, such as an email that is sent or a payment that is charged. Meszaros gives the principle "Use the Front Door First": test through the public interface where possible.[^6] A test that replaces so much that it tests its own doubles is the extreme case. Meszaros warns that a double must never replace the part of the SUT that the test is meant to check.[^2]

### The fake drifts from the real component

A fake is code, and it can be wrong. Mackinnon, Freeman and Craig gave the example of a mock that returns degrees where the real code returns radians. They concluded that functional tests are still needed.[^7] Here the real store is a SQLite table whose key ignores case, and the fake is a Python set. The fake accepts the same address twice.

```python wrong id=drift
import sqlite3


class SqlUsers:  # the real store: the key ignores case
    def __init__(self):
        self.db = sqlite3.connect(":memory:")
        self.db.execute("CREATE TABLE users (email TEXT PRIMARY KEY COLLATE NOCASE)")

    def exists(self, email):
        row = self.db.execute("SELECT 1 FROM users WHERE email = ?", (email,))
        return row.fetchone() is not None

    def add(self, email):
        self.db.execute("INSERT INTO users VALUES (?)", (email,))


class FakeUsers:  # the fake: a set, which does not ignore case
    def __init__(self):
        self.emails = set()

    def exists(self, email):
        return email in self.emails

    def add(self, email):
        self.emails.add(email)


def register(email, users):
    if users.exists(email):
        return "taken"
    users.add(email)
    return "created"


for store in (FakeUsers(), SqlUsers()):
    first = register("ada@example.com", store)
    second = register("Ada@Example.com", store)
    print(f"{type(store).__name__}: {first}, {second}")
```

```output for=drift
FakeUsers: created, created
SqlUsers: created, taken
```

The fix is a contract test: one function that states the behaviour and runs against the real store and the fake. It found the drift, and the fake now folds the case of each address.

```python run id=contract
import sqlite3


class SqlUsers:
    def __init__(self):
        self.db = sqlite3.connect(":memory:")
        self.db.execute("CREATE TABLE users (email TEXT PRIMARY KEY COLLATE NOCASE)")

    def exists(self, email):
        row = self.db.execute("SELECT 1 FROM users WHERE email = ?", (email,))
        return row.fetchone() is not None

    def add(self, email):
        self.db.execute("INSERT INTO users VALUES (?)", (email,))


class FakeUsers:
    def __init__(self):
        self.emails = set()

    def exists(self, email):
        return email.casefold() in self.emails

    def add(self, email):
        self.emails.add(email.casefold())


def users_contract(make_store):
    store = make_store()
    assert not store.exists("ada@example.com")
    store.add("ada@example.com")
    assert store.exists("ada@example.com")
    assert store.exists("Ada@Example.com"), "the key must ignore case"


for make_store in (SqlUsers, FakeUsers):
    users_contract(make_store)
    print(f"{make_store.__name__}: contract holds")
```

```output for=contract
SqlUsers: contract holds
FakeUsers: contract holds
```

`casefold` and `COLLATE NOCASE` agree for these ASCII addresses. SQLite's `NOCASE` folds only the 26 ASCII letters, so a contract for addresses in other scripts needs more cases.[^15]

## Cost

- **Speed.** A double in memory does no input or output, so it answers without a wait for a disk or a network. Meszaros names Slow Tests as one reason to use a double, and he reports a suite that ran 50 times faster after the database was replaced with hash tables in memory.[^2]
- **Code to maintain.** Each hand-written double is code with its own bugs (see the last pitfall).[^7] A mock library removes the code but adds setup lines to each test, and the tests then depend on the API of the library.
- **Coupling.** Every call that a test checks is a fact about the implementation. Such tests break when the code changes without a change in behaviour, so they make refactoring cost more.[^3][^6]
- **Confidence.** A test with doubles shows that the unit works with the doubles, not with the real components. Meszaros advises at least one test that runs without the double.[^2] The Python documentation gives the same advice: tests in isolation still need tests of how the units are wired together.[^5]
- **Run-time mocks.** `unittest.mock` creates attributes on first access and records every call.[^4] A `java.lang.reflect.Proxy` sends every call through one handler method.[^12] This work is small next to the cost of the real component, but it is more than a direct call to a hand-written class.

## History

- **2000.** Tim Mackinnon, Steve Freeman and Philip Craig presented "Endo-Testing: Unit Testing with Mock Objects" at XP2000. Their mock objects replace domain code with implementations that emulate it and that assert how the code under test uses them.[^7][^3]
- **2004 and 2007.** Martin Fowler published "Mocks Aren't Stubs" in July 2004. In the revision of January 2007, he split state verification from behaviour verification and classical from mockist testing, and he took the vocabulary of Meszaros.[^3]
- **2007.** Gerard Meszaros published *xUnit Test Patterns*. The book gives Test Double as the general name and defines Dummy Object, Test Stub, Test Spy, Mock Object and Fake Object.[^1][^2]

## References
