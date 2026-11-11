---
tag: KYEQF7
kind: concept
slug: test-driven-development
name: Test-driven development
aliases: [TDD, test-first development, red green refactor, behaviour-driven development, BDD]
area: testing/tdd-and-bdd
level: core
summary: >-
  Test-driven development writes a failing automated test before each piece of code, makes it pass with the least code, and then removes duplication. The cycle is red, green, refactor.
applies_to: core-languages
viz: optional
formal: none
history:
  year: 2002
  people: [Kent Beck]
  note: Kent Beck published Test-Driven Development, By Example with Addison-Wesley in 2002. The preface gives the two rules of TDD and the cycle red, green, refactor.
see_also: [unit-testing, test-double, mutation-analysis, property-based-tests, extract-function, static-and-dynamic-typing]
same_as:
  wikidata: Q950250
  wikipedia: https://en.wikipedia.org/wiki/Test-driven_development
sources:
  - book: isbn:9780321146533
    title: Kent Beck, Test-Driven Development, By Example, Addison-Wesley, 2002
    locator: Preface (the two rules; red, green, refactor; clean code that works); Chapter 3, Equality for All, pages 15 to 18 (the to-do list, the fake implementation, Triangulation); Index, green bar patterns (Fake It, pages 151 to 153; Triangulation, pages 153 to 154; Obvious Implementation, pages 154 to 155; One to Many, pages 155 to 156)
  - url: https://www.rfc-editor.org/rfc/rfc3339
    title: G. Klyne and C. Newman, RFC 3339, Date and Time on the Internet, Timestamps, July 2002
    locator: Section 2, Definitions (leap year); Appendix C, Leap Years
  - url: https://docs.python.org/3/library/unittest.html
    title: The Python Standard Library, unittest, Unit testing framework
    locator: Basic example (methods whose names start with test)
  - url: https://nodejs.org/api/test.html
    title: Node.js documentation, Test runner (node:test)
    locator: introduction
  - url: https://pkg.go.dev/testing
    title: Go standard library, package testing
    locator: package overview (func TestXxx and the go test command)
  - url: https://doc.rust-lang.org/book/ch11-01-writing-tests.html
    title: The Rust Programming Language, 11.1 How to Write Tests
    locator: Structuring Test Functions (the test attribute)
  - url: https://dannorth.net/blog/introducing-bdd/
    title: Dan North, Introducing BDD, first published in Better Software magazine, March 2006
    locator: Test method names should be sentences; JBehave emphasizes behaviour over testing; Requirements are behaviour, too; BDD provides a ubiquitous language for analysis; Acceptance criteria should be executable
  - url: https://cucumber.io/docs/gherkin/reference/
    title: Cucumber, Gherkin Reference
    locator: Keywords; Steps (Given, When, Then); the step definition
  - paper: doi:10.1007/s10664-008-9062-z
    title: Nachiappan Nagappan, E. Michael Maximilien, Thirumalesh Bhat and Laurie Williams, Realizing quality improvement through test driven development, results and experiences of four industrial teams, Empirical Software Engineering 13(3), 2008, pages 289 to 302
    locator: Abstract
  - paper: doi:10.1007/s10664-016-9490-0
    title: Ayse Tosun, Oscar Dieste, Davide Fucci and others, An industry experiment on the effects of test-driven development on external quality and productivity, Empirical Software Engineering 22(6), 2017, pages 2763 to 2805
    locator: Abstract; Section 2, Related Work (the meta-analysis of Rafique and Mišić, 2013)
  - paper: doi:10.1109/TSE.2016.2616877
    title: Davide Fucci, Hakan Erdogmus, Burak Turhan, Markku Oivo and Natalia Juristo, A Dissection of the Test-Driven Development Process, Does It Really Matter to Test-First or to Test-Last?, IEEE Transactions on Software Engineering 43(7), 2017, pages 597 to 614
    locator: Abstract (arXiv version 1611.05994)
---

## Idea

Test-driven development (TDD) puts the test before the code. Kent Beck states it as two rules in the preface of his book. First, write code only when an automated test fails. Second, remove duplication.[^1] The rules give an order to the work, and Beck names its three steps red, green and refactor:[^1]

1. **Red.** Write a small test that does not work. At first it may not even compile.
2. **Green.** Make the test pass quickly. Any shortcut is allowed in this step.
3. **Refactor.** Remove the duplication that the shortcut made.

Red and green refer to the bar of the test runner, which is red when a test fails. After the first test of chapter 3, Beck writes: "The bar turns obligingly red."[^1] Beck gives the goal of the practice with a phrase that he credits to Ron Jeffries: "clean code that works".[^1] The test does two jobs. It says what the code must do before the code exists, and it checks the code after every change.

TDD is an order of work, not a kind of test. The tests it makes are mostly [[unit-testing|unit tests]], and that page covers how to write them and how a runner finds them. This page covers the cycle that writes them.

The running example on this page is a function that says if a year is a leap year. RFC 3339 defines the rule: a year is a leap year if 4 divides it, except that a year that 100 divides must also be divisible by 400.[^2] So 2024 and 2000 are leap years, and 2023 and 1900 are not.

The first step is red. The test calls `is_leap`, and `is_leap` does not exist yet. In Python the test fails when it runs, with a `NameError`.

```python fails expect=exception:NameError id=red
# Red: the test comes first, so is_leap does not exist yet.
def test_2024_is_leap():
    assert is_leap(2024)


test_2024_is_leap()
```

The second step is green, with the least code that passes. Beck calls a constant in place of the real computation Fake It. In chapter 3 his first `equals` method returns `true` and nothing else.[^1]

```python run id=fake-it
def is_leap(year):
    return True  # Fake It: a constant passes the one test that exists


def test_2024_is_leap():
    assert is_leap(2024)


test_2024_is_leap()
print("1 test passed")
```

```output for=fake-it
1 test passed
```

The fake is wrong, and the programmer knows it. The next test shows it. A test for 2023 fails with an `AssertionError`, and the cycle starts again at red.

```python fails expect=exception:AssertionError id=second-example
def is_leap(year):
    return True


def test_2024_is_leap():
    assert is_leap(2024)


def test_2023_is_not_leap():
    assert not is_leap(2023)


test_2024_is_leap()
test_2023_is_not_leap()
```

## How it works

### The to-do list

The cycle works from a list of tests that the code will need. In chapter 3, Beck keeps such a to-do list next to the code. He adds `hashCode()` to it when he sees that `equals()` needs it, and he adds "Equal null" and "Equal object" when he finishes equality, because they are "not necessary at the moment".[^1] Each cycle takes one item from the list. An idea that comes up in the middle of a cycle goes onto the list, so that the cycle stays small.

For the leap year, the list has four items: 2024 is a leap year, 2023 is not, 1900 is not, and 2000 is. The order matters. Each item adds one part of the rule, so each test fails for one reason.

### Three ways to get to green

Beck uses three strategies to get to green: Fake It, Triangulation and Obvious Implementation. The index of his book lists them under the green bar patterns, together with a fourth pattern, One to Many.[^1]

- **Fake It.** Return a constant that passes the test. Then change the constant into a variable, step by step, as the refactor step removes the duplication between the test and the code.
- **Triangulation.** Generalize only when two or more examples force it. Beck calls it "the third and most conservative implementation strategy". He writes that "we only generalize code when we have two examples or more".[^1] The test for 2023 above is the second example that forces the constant to go.
- **Obvious Implementation.** When the general code is clear, write it. Beck writes that he uses Triangulation "only when I am completely unsure of how to refactor". When he can see the general solution, he writes it at once.[^1]

The program below replays the whole cycle for the leap year. Each row adds one test or changes the code, and the program runs every test that exists at that point. A row ends red when one test fails and green when all pass.

```python run id=cycle
TESTS = [(2024, True), (2023, False), (1900, False), (2000, True)]

# Each version of the code, in the order the cycle writes it.
CODE = [
    ("return True", lambda y: True),
    ("y % 4 == 0", lambda y: y % 4 == 0),
    ("y % 4 == 0 and y % 100 != 0", lambda y: y % 4 == 0 and y % 100 != 0),
    ("... or y % 400 == 0", lambda y: y % 4 == 0 and y % 100 != 0 or y % 400 == 0),
]

# (number of tests, version of the code) for each row of the cycle.
STEPS = [(1, 0), (2, 0), (2, 1), (3, 1), (3, 2), (4, 2), (4, 3)]

for n, v in STEPS:
    text, is_leap = CODE[v]
    failed = [y for y, want in TESTS[:n] if is_leap(y) != want]
    result = f"red: {failed[0]}" if failed else "green"
    print(f"{n} tests  {text:<30} {result}")
```

```output for=cycle
1 tests  return True                    green
2 tests  return True                    red: 2023
2 tests  y % 4 == 0                     green
3 tests  y % 4 == 0                     red: 1900
3 tests  y % 4 == 0 and y % 100 != 0    green
4 tests  y % 4 == 0 and y % 100 != 0    red: 2000
4 tests  ... or y % 400 == 0            green
```

Each red row names the year of the test that failed. Each code change fixes that test and keeps the earlier tests green. The fake goes when the second example arrives, and each later test adds one clause of the rule.

### Refactor

Green is not the end. The code that passes is often the first code that came to mind, and it can repeat itself. The third step removes the duplication and keeps every test green.[^1] Suppose that the cycle reached green with one `if` statement for each clause of the rule, as in `is_leap_before` below. The refactor turns the statements into one expression, the same one that RFC 3339 gives in its Appendix C.[^2] The tests do not change. They run against the old code and the refactored code, and a second check compares both on every year from 1 to 4000.

```python run id=refactor
def is_leap_before(year):
    if year % 400 == 0:
        return True
    if year % 100 == 0:
        return False
    if year % 4 == 0:
        return True
    return False


def is_leap(year):
    return year % 4 == 0 and (year % 100 != 0 or year % 400 == 0)


TESTS = [(2024, True), (2023, False), (1900, False), (2000, True)]

for version in (is_leap_before, is_leap):
    for year, want in TESTS:
        assert version(year) == want, (version.__name__, year)
    print(f"{version.__name__}: {len(TESTS)} tests passed")

same = all(is_leap_before(y) == is_leap(y) for y in range(1, 4001))
print("same result for years 1 to 4000:", same)
```

```output for=refactor
is_leap_before: 4 tests passed
is_leap: 4 tests passed
same result for years 1 to 4000: True
```

A refactoring changes the structure of the code and keeps its behaviour. [[extract-function|Extract function]] is one example. The tests are what make the change safe. If a refactoring breaks a rule that a test checks, the test turns red at once, and the last change is the cause. This is why the steps are small. After a change of five lines, the cause of a failure is in those five lines. After a day of changes, it can be anywhere.

### What red proves

Red is not a formality. A test that fails before the code exists shows two facts. The test runs, and it can fail. A test that has never failed may not test anything: it may not run at all, or its check may be true for every program. The pitfall below shows such a test.

## In languages

The cycle is the same in every language. What changes is the first red. In a language with static type checks, a call to a function that does not exist stops the compiler, so the first red is a compile error. In a dynamic language, the program starts and fails when the call runs. Beck's words "perhaps doesn't even compile at first" cover the first case.[^1] The page on [[static-and-dynamic-typing|static and dynamic typing]] explains the difference. The example files show both kinds of red in each language.

| Language | Verdict | Form |
|---|---|---|
| Python | stdlib | `unittest`; the first red is a `NameError` when the test runs |
| JavaScript | stdlib | `node:test` in Node.js; the first red is a `ReferenceError` when the test runs |
| TypeScript | stdlib | the JavaScript runners; `tsc` reports the missing name before the test runs, but a runner that only removes the types reports a `ReferenceError` |
| Java | library | JUnit; the first red is a compile error, "cannot find symbol" |
| C | idiom | a test program with `assert`; the first red is a compile error |
| C++ | library | GoogleTest or Catch2; the first red is a compile error |
| C# | library | xUnit.net, NUnit or MSTest; the first red is a compile error |
| Go | stdlib | package `testing` and `go test`; the first red is a build error, "undefined" |
| Rust | native | `#[test]` and `cargo test`; the first red is a compile error |
| Kotlin | library | `kotlin.test` on JUnit; the first red is a compile error |
| PowerShell | library | Pester |
| Bash | library | bats-core, or a script that checks exit statuses |
| SQL | library | pgTAP for PostgreSQL; tSQLt for SQL Server |
| Regex | n/a | a pattern is tested through the language that uses it |
| HTML/CSS | n/a | markup and style rules are tested through a browser |

The verdicts follow the test runner, because the cycle needs a fast way to run tests. Python's `unittest` runs the methods whose names start with `test`.[^3] Node.js has the module `node:test` in its runtime.[^4] Go's `go test` runs each function of the form `func TestXxx(*testing.T)`.[^5] In Rust, a test is a function with the `test` attribute.[^6] Java, C# and C++ need a test framework from outside the standard library for the usual workflow. The example files for those languages use a small runner in `main`, so that they run with the standard library only.

A compile error as red has a cost and a benefit. The cost is that the programmer must write a stub, a function with the right signature and no logic, before the test can fail for the right reason. The benefit is that the type checker joins the tests. A test that calls `is_leap("2024")` with a string does not compile in Rust or Java, so no test is needed for that case. In Python the same call runs, and only a test finds it.

## Variations and related ideas

### Behaviour-driven development

Dan North describes behaviour-driven development (BDD) in an article first published in Better Software magazine in March 2006.[^7] It started as a way to teach TDD. North writes that programmers asked where to start, what to test, what to call their tests and why a test fails. He found that the word "test" was the cause of much of the confusion.[^7]

His first steps changed the names. A tool called agiledox, by his colleague Chris Stevenson, printed JUnit method names as sentences. North then adopted the convention of test method names that start with the word "should".[^7] At the end of 2003 he began JBehave, a replacement for JUnit with the word "test" taken out.[^7] Toward the end of 2004, he and the business analyst Chris Matts decided to apply the same vocabulary to requirements. Their goal was one vocabulary for analysts, testers, developers and the business. They started from a story template already in use at their company, "As a [X], I want [Y], so that [Z]", and they wrote the acceptance criteria of each story as scenarios with the form Given, When, Then.[^7]

The Gherkin language of Cucumber writes such scenarios as plain text. A Given step describes the initial context, a When step describes an event or an action, and a Then step describes an expected outcome.[^8] The text after each keyword is matched to a step definition, which is code.[^8] The program below is a small step runner. A decorator, which is a [[higher-order-functions|higher-order function]], registers each step definition with a regular expression.

```python run id=gherkin
import re

SCENARIOS = """\
Scenario: a century year that 400 does not divide
  Given the year 1900
  When I ask if it is a leap year
  Then the answer is no
Scenario: a century year that 400 divides
  Given the year 2000
  When I ask if it is a leap year
  Then the answer is yes
"""


def is_leap(year):
    return year % 4 == 0 and (year % 100 != 0 or year % 400 == 0)


STEPS = []


def step(pattern):
    def register(function):
        STEPS.append((re.compile(pattern), function))
        return function
    return register


@step(r"the year (\d+)")
def given_year(context, year):
    context["year"] = int(year)


@step(r"I ask if it is a leap year")
def when_ask(context):
    context["answer"] = is_leap(context["year"])


@step(r"the answer is (yes|no)")
def then_answer(context, word):
    assert context["answer"] == (word == "yes")


for line in SCENARIOS.splitlines():
    keyword, _, text = line.strip().partition(" ")
    if keyword == "Scenario:":
        context = {}
        print(line)
        continue
    for pattern, function in STEPS:
        match = pattern.fullmatch(text)
        if match:
            function(context, *match.groups())
            print(f"  ok  {keyword} {text}")
            break
    else:
        print(f"  no step definition for: {text}")
```

```output for=gherkin
Scenario: a century year that 400 does not divide
  ok  Given the year 1900
  ok  When I ask if it is a leap year
  ok  Then the answer is no
Scenario: a century year that 400 divides
  ok  Given the year 2000
  ok  When I ask if it is a leap year
  ok  Then the answer is yes
```

BDD keeps the cycle of TDD. It changes the words of the first test, and it can change who writes it. Because the scenario is in the words of the business, an analyst can write it, and the programmer writes the step definitions and then the code.

### Other related ideas

- **Test-last development.** The programmer writes the code first and the tests after, in small steps. Studies of TDD often compare it with incremental test-last development. Fucci and others found that the order of test and code had no important influence, while the size and evenness of the steps did (see Cost).[^11]
- **[[property-based-tests|Property-based tests]].** TDD drives the code with examples. A property-based test states a rule for all inputs. The leap-year rule from RFC 3339 is such a rule, and a property can check it on thousands of years.
- **[[mutation-analysis|Mutation analysis]].** TDD makes tests that fail before the code exists. Mutation analysis checks if the tests still fail when the finished code changes.
- **[[test-double|Test doubles]].** A test of code that talks to a database or a clock needs a stand-in, so that the cycle stays fast.

## Pitfalls

### A test that never failed

The first rule of TDD says to write code only when a test fails, so each test is seen red before it is seen green. A test that never failed may not run. `unittest` runs only the methods whose names start with `test`.[^3] The method below is named `check_1900`, so the runner skips it, and the suite passes with a bug in the code.

```python wrong id=never-ran
import io
import unittest


def is_leap(year):
    return year % 4 == 0  # bug: 1900 is not a leap year


class LeapTest(unittest.TestCase):
    def test_2024(self):
        self.assertTrue(is_leap(2024))

    def test_2023(self):
        self.assertFalse(is_leap(2023))

    def check_1900(self):  # not a test: the name does not start with "test"
        self.assertFalse(is_leap(1900))


suite = unittest.defaultTestLoader.loadTestsFromTestCase(LeapTest)
result = unittest.TextTestRunner(stream=io.StringIO()).run(suite)
print("tests run:", result.testsRun)
print("all passed:", result.wasSuccessful())
```

```output for=never-ran
tests run: 2
all passed: True
```

In the cycle, this test would have stayed green before the fix for 1900, and the green would have been a warning. Go has the same rule for names: `go test` runs `TestXxx` only when `Xxx` does not start with a lowercase letter.[^5]

### Expected values copied from the code

A test must know the answer from a source other than the code. The test below computes the expected value with the same formula as the code, so the code and the test agree on the bug.

```python wrong id=same-formula
def is_leap(year):
    return year % 4 == 0  # bug: 1900 is not a leap year


def test_matches_rule():
    for year in [1900, 2000, 2023, 2024]:
        assert is_leap(year) == (year % 4 == 0)  # repeats the code


test_matches_rule()
print("test passed")
print("is_leap(1900) =", is_leap(1900))
```

```output for=same-formula
test passed
is_leap(1900) = True
```

The fix is a literal answer from the specification: RFC 3339 says that 1900 is not a leap year.[^2] In TDD the test exists before the code, so the expected value cannot come from the code. That is one of the reasons for the order.

### A fake that stays

Fake It is a step, not a result. Beck's fake `equals` returns `true`, and he writes that the constant is really "5 == 5", which is really "amount == 5", which is really "amount == dollar.amount".[^1] The duplication between the constant in the code and the value in the test is what the refactor step removes. If the cycle stops at green, the fake stays. A constant passes any single example, so a list with one example cannot catch it. Triangulation adds the second example for that reason.

### Tests that fix the design

A test that checks how the code works, not what it returns, breaks when the code is refactored. Then the third step of the cycle turns tests red without a bug, and the tests stop being a safety net. Mocks that check each call to a collaborator tie the test to the code in this way. The page on [[test-double|test doubles]] shows the problem and the cost of over-mocking.

### Reading the studies as proof

The evidence on TDD is mixed, and the results depend on the setting (see Cost). A claim that TDD halves the defects or slows every team is stronger than the studies support.

## Cost

The cost of TDD is the time to write and maintain the tests, and the discipline of small steps. Beck's preface states the hope that TDD can reduce the defect density of code a great deal. It also lists a demand on design: many highly cohesive, loosely coupled components, so that testing stays cheap.[^1] Several studies have tried to measure the effect on defects and on productivity, with mixed results.

**Case studies in industry.** Nagappan, Maximilien, Bhat and Williams studied three development teams at Microsoft and one at IBM that adopted TDD. The pre-release defect density of the four products fell by 40% to 90% compared with similar projects that did not use TDD. The teams estimated, as a subjective judgement, that initial development took 15% to 35% more time.[^9] The limits are in the design of the study. A case study does not assign teams to TDD at random. The comparison projects were similar, not the same, and the time cost is an estimate, not a measurement.

**A meta-analysis.** Tosun and others summarize the meta-analysis of Rafique and Mišić from 2013, which covered 27 empirical studies. External quality improved with TDD in industrial studies, but it was not significantly different in experiments with students. Productivity improved in academic experiments and fell in industrial ones. Both the gain in quality and the loss of productivity were larger in industrial studies.[^10]

**An experiment with professionals.** Tosun and others ran an experiment with 24 professionals at three sites of one company. Each person used TDD and incremental test-last development. The study found no statistical difference in external quality. The subjects were more productive with TDD on a less complex task, but productivity fell significantly with TDD on a complex task in existing code. The authors conclude that the choice of tasks can dominate the results of TDD studies.[^10]

**A study of the process.** Fucci and others analysed 82 data points from 39 professionals. They measured four properties of the process: the order of test and code, the size of the steps, how even the steps were, and the effort spent on refactoring. Small, even steps went with better quality and productivity. The order, test first or test last, had no important influence. Refactoring effort went with worse results on both measures. The authors suggest that the link with lower quality may come from refactoring mixed with other changes. They suggest that the benefits of TDD may come from fine-grained steady steps, not from writing the test first.[^11]

The studies have limits that the authors name. Few experiments use professionals in industry; Tosun and others found only three in the reviews that they checked.[^10] The reviews use inconsistent terms and a loose definition of an experiment, so they count the studies in different ways.[^10] Many studies ignore the process itself, such as the size of the steps, and measure only the label TDD.[^11] The baseline matters as well. The experiments of Tosun and Fucci compare TDD with incremental test-last development, which also writes tests in small steps, so they measure the effect of the order and not the effect of testing.[^10][^11] So the studies support a modest claim. Short cycles with tests help in some settings, and the size of the effect depends on the task and the people.

## History

- **2002.** Kent Beck published Test-Driven Development, By Example. The preface states the two rules and the cycle red, green, refactor, and part III of the book collects the patterns, among them Fake It, Triangulation and Obvious Implementation.[^1]
- **2003.** At the end of the year, Dan North began JBehave, a replacement for JUnit built around the word behaviour.[^7]
- **2004.** Toward the end of the year, North and Chris Matts decided to apply the behaviour vocabulary to requirements, and they wrote acceptance criteria as scenarios with Given, When, Then.[^7]
- **2006.** North published Introducing BDD in Better Software magazine in March.[^7]
- **2008.** Nagappan and others published case studies of TDD in four industrial teams at Microsoft and IBM.[^9]
- **2013.** Rafique and Mišić published a meta-analysis of 27 studies of TDD, as summarized by Tosun and others.[^10]
- **2017.** Tosun and others published an experiment with 24 professionals, and Fucci and others published a study of the TDD process with 39 professionals.[^10][^11]

## References
