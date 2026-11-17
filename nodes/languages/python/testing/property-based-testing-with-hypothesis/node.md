---
tag: JNYNGJ
kind: feature
owner: python
slug: property-based-testing-with-hypothesis
name: Property-based testing with Hypothesis
aliases: [Hypothesis, hypothesis.given, given, strategies, hypothesis.strategies, shrinking, minimal failing test case, "@example", assume, RuleBasedStateMachine, stateful testing, Hypothesis database, .hypothesis directory, max_examples]
area: languages/python/testing
level: advanced
summary: >-
  Hypothesis runs a test many times with inputs drawn from strategies and shrinks a failing input to a minimal one. @example pins inputs, a database replays failures, and state machines chain actions.
grammar: null
spec: {standard: null, url: "https://hypothesis.readthedocs.io/en/latest/tutorial/introduction.html", locator: "Introduction to Hypothesis"}
see_also: [property-based-tests, python/testing-with-pytest, python/unittest, python/quality-control, pytest]
sources:
  - url: https://hypothesis.readthedocs.io/en/latest/tutorial/introduction.html
    title: Introduction to Hypothesis, Hypothesis documentation
    locator: Defining a simple test; Testing a sorting algorithm; Preventing floats() from generating nan
    accessed: 2026-11-16
  - url: https://hypothesis.readthedocs.io/en/latest/quickstart.html
    title: Quickstart, Hypothesis documentation
    locator: Running in a test suite; Filtering inside a test
    accessed: 2026-11-16
  - url: https://hypothesis.readthedocs.io/en/latest/tutorial/settings.html
    title: Configuring test settings, Hypothesis documentation
    locator: Changing the number of test cases; Other settings options
    accessed: 2026-11-16
  - url: https://hypothesis.readthedocs.io/en/latest/glossary.html
    title: Glossary, Hypothesis documentation
    locator: explicit example; minimal failing test case
    accessed: 2026-11-16
  - url: https://hypothesis.readthedocs.io/en/latest/tutorial/replaying-failures.html
    title: Replaying failed tests, Hypothesis documentation
    locator: The Hypothesis database; Using @example; Prefer @example over the database for correctness
    accessed: 2026-11-16
  - url: https://hypothesis.readthedocs.io/en/latest/stateful.html
    title: Stateful tests, Hypothesis documentation
    locator: Rule-based state machines
    accessed: 2026-11-16
  - url: https://pkg.go.dev/testing/quick
    title: Package quick, Go standard library
    locator: package overview; Check
    accessed: 2026-11-16
  - url: https://pkg.go.dev/testing
    title: Package testing, Go standard library
    locator: Fuzzing
    accessed: 2026-11-16
---

## Synopsis

Hypothesis is a third-party package (`pip install hypothesis`). A test states a property that must hold for every input, and `@given` supplies the inputs:

```python norun reason=fragment group=dedupe file=test_dedupe.py
from hypothesis import given, settings, strategies as st


def dedupe(items):
    return sorted(set(items))  # bug: callers expect first-seen order


@settings(derandomize=True, database=None)
@given(st.lists(st.integers()))
def test_dedupe_keeps_order(items):
    assert dedupe(items) == list(dict.fromkeys(items))
```

```python run group=dedupe file=main.py id=synopsis
from test_dedupe import test_dedupe_keeps_order

try:
    test_dedupe_keeps_order()
except AssertionError as e:
    print(e.__notes__[0])
```

```output for=synopsis
Failing test case: test_dedupe_keeps_order(
    items=[0, -1],
)
```

A `@given` test is still a function, so calling it runs the whole search (r2); pytest does the same call and prints the same lines under the failed assertion. The test fails, and Hypothesis attaches the minimal failing test case to the error as a note: `[0, -1]`, not the longer random list that first broke the property (r3). `derandomize=True` makes the run repeat exactly (r8), and `database=None` keeps the failure out of the `.hypothesis` directory (r6). The output is from Hypothesis 6.170.15; another version may shrink to a different two-element list. The example below shows that search and shrink in plain Python.

## Semantics

**r1.** "Hypothesis tests are defined using two things; @given, and a strategy, which is passed to @given."[^1] "Adding the @given decorator turns this function into a Hypothesis test."[^1] Strategies such as `st.integers()`, `st.floats()` and `st.lists(...)` describe the inputs, and `a | b` "takes two strategies, and returns a new strategy which generates values from either of its strategies."[^1]

**r2.** "A Hypothesis test is still a regular python function, which means pytest or unittest will pick it up and run it in all the normal ways."[^2] "By default, Hypothesis will stop after generating 100 test cases."[^3] The `max_examples` setting changes that number, through `@settings(max_examples=...)` or a settings profile.[^3]

**r3.** When an input fails, Hypothesis shrinks it. The minimal failing test case is "The failing test case which has been fully shrunk" by Hypothesis, and "Hypothesis reports only the minimal failing test case to the user at the end of the test."[^4]

**r4.** Inputs that do not fit the property can be filtered out with `.filter()` on the strategy, or with `assume()`, "which tells Hypothesis to discard any test case with a false-y argument".[^2]

**r5.** `@example(...)` adds fixed inputs: "@example adds an explicit input which Hypothesis will run every time, in addition to the randomly generated test cases."[^5] "Unlike Hypothesis-generated test cases, Hypothesis does not shrink explicit examples."[^4]

**r6.** Failures are saved: "When a test fails, Hypothesis automatically saves the failure so it can be replayed later."[^5] "By default, this is a DirectoryBasedExampleDatabase in the local .hypothesis directory."[^5] "On future runs, Hypothesis retries any failing inputs" before it generates new ones.[^5] "You can disable the database by passing database=None to @settings".[^5] The docs advise against depending on it: "We therefore recommend against relying on the database for the correctness of your tests. If you want to ensure an input is run every time, use @example."[^5]

**r7.** Stateful tests let Hypothesis choose actions as well as values. A rule-based state machine "takes values drawn from strategies and passes them to a user defined test function, which may use assertions to check the system’s behavior."[^6] "The key difference is that where @given based tests must be independent, rules can be chained together - a single test run may involve multiple rule invocations, which may interact in various ways."[^6]

**r8.** `settings.derandomize` "makes Hypothesis deterministic."[^3]

## Examples

### Search and shrink in miniature

This program does by hand, for one property, what `@given` and the shrinker do: draw random lists from a seeded generator until one breaks the property, then try smaller variants while the property still fails.

```python run id=mini
import random


def dedupe(items):
    return sorted(set(items))  # bug: the docstring promised first-seen order


def keeps_order(items):
    return dedupe(items) == list(dict.fromkeys(items))


def find_failure(prop, rng, tries=100):
    for _ in range(tries):
        items = [rng.randint(-50, 50) for _ in range(rng.randint(0, 8))]
        if not prop(items):
            return items
    return None


def shrink(prop, items):
    changed = True
    while changed:
        changed = False
        for i in range(len(items)):
            smaller = items[:i] + items[i + 1:]
            if not prop(smaller):
                items, changed = smaller, True
                break
        else:
            for i, x in enumerate(items):
                for y in (0, x // 2, x - 1 if x > 0 else x + 1):
                    if y != x and abs(y) <= abs(x):
                        candidate = items[:i] + [y] + items[i + 1:]
                        if not prop(candidate):
                            items, changed = candidate, True
                            break
                if changed:
                    break
    return items


rng = random.Random(2024)
failing = find_failure(keeps_order, rng)
print('random failure:', failing)
print('shrunk to:', shrink(keeps_order, failing))
```

```output for=mini
random failure: [-27, 43, 24, -12, -25, 42, 2]
shrunk to: [1, 0]
```

The random failure has seven numbers, but only the order of two of them matters. `[1, 0]` is a two-element list that shows the bug: `dedupe` returns `[0, 1]`. Hypothesis does this for every `@given` test, with a far more thorough shrinker, and ends at the same kind of small input: `[0, -1]` in the synopsis (r3).

### Filtering, explicit examples and settings

```python norun reason=fragment group=props file=test_props.py
from hypothesis import assume, example, given, settings, strategies as st


@given(st.lists(st.integers() | st.floats(allow_nan=False)))
def test_sort_matches_sorted(lst):
    assert sorted(lst, reverse=True)[::-1] == sorted(lst)


@example(0)
@example(2**31 - 1)
@settings(max_examples=500)
@given(st.integers())
def test_int_bytes_round_trip(n):
    assume(n >= 0)
    data = n.to_bytes((n.bit_length() + 7) // 8 or 1, 'big')
    assert int.from_bytes(data, 'big') == n
```

```python run group=props file=main.py id=props
from test_props import test_int_bytes_round_trip, test_sort_matches_sorted

test_sort_matches_sorted()
test_int_bytes_round_trip()
print('both properties held')
```

```output for=props
both properties held
```

`floats(allow_nan=False)` keeps NaN out, because "sorting in the presence of nan is not well defined."[^1] The two `@example` inputs run on every run, `assume` drops negative numbers, and the second test runs 500 cases instead of 100. Each call returns normally because every case passed.

### A rule-based state machine

```python norun reason=fragment group=stack file=test_stack.py
from hypothesis import strategies as st
from hypothesis.stateful import RuleBasedStateMachine, invariant, rule


class Stack:
    def __init__(self):
        self.items = []

    def push(self, x):
        self.items.append(x)

    def pop(self):
        return self.items.pop(0)  # bug: takes from the bottom


class StackMachine(RuleBasedStateMachine):
    def __init__(self):
        super().__init__()
        self.stack = Stack()
        self.model = []

    @rule(x=st.integers())
    def push(self, x):
        self.stack.push(x)
        self.model.append(x)

    @rule()
    def pop(self):
        if self.model:
            assert self.stack.pop() == self.model.pop()

    @invariant()
    def same_size(self):
        assert len(self.stack.items) == len(self.model)


TestStack = StackMachine.TestCase
```

```python run group=stack file=main.py id=stack
from hypothesis import settings
from hypothesis.stateful import run_state_machine_as_test

from test_stack import StackMachine

try:
    run_state_machine_as_test(
        StackMachine, settings=settings(derandomize=True, database=None))
except AssertionError as e:
    for note in e.__notes__:
        if note.startswith('Explanation'):
            break
        print(note)
```

```output for=stack
Failing test case: <state machine>
state = StackMachine()
state.same_size()
state.push(x=0)
state.same_size()
state.push(x=1)
state.same_size()
state.pop()
state.teardown()
```

Each run is a sequence of `push` and `pop` calls chosen by Hypothesis, checked against a plain list as the model. The shrunk failure is three steps long: push 0, push 1, pop. The buggy `pop` returns `0` from the bottom while the model returns `1`, and the invariant ran after every step before that. `StackMachine.TestCase` is the way the docs give to get "a unittest TestCase" from a machine, so pytest and unittest both collect `TestStack`.[^6] The main program skips the test classes: `run_state_machine_as_test` "takes an arbitrary function returning a RuleBasedStateMachine and an optional settings parameter and does the same as the class based runTest provided."[^6] The output is from Hypothesis 6.170.15.

## Pitfalls

- A property that restates the implementation proves nothing. Compare against a simpler model (a plain list, `sorted`, a round trip) rather than re-deriving the answer the same way the code does.
- Heavy filtering with `assume` or `.filter()` throws inputs away (r4). When most inputs are rejected, build a strategy that produces valid inputs directly, for example `st.integers(min_value=0)` in place of `assume(n >= 0)`.
- Floats include NaN and infinities. Tests on sorting or equality fail on NaN unless the strategy excludes it (see the example above).
- The `.hypothesis` directory is a cache, not a test suite (r6). An input that must always be tried belongs in an `@example`.
- Hypothesis tests are random by default, so a failure may show up on one run and not the next. The saved database replays it locally, and `derandomize` makes runs repeat exactly (r6, r8).

## Comparison

Go has two tools in this space. `testing/quick` is the older one: "Check looks for an input to f, any function that returns bool, such that f returns false. It calls f repeatedly, with arbitrary values for each argument."[^7] "The testing/quick package is frozen and is not accepting new features."[^7] The current tool is fuzzing in `go test`, "a testing technique where a function is called with randomly generated inputs to find bugs not anticipated by unit tests."[^8] A fuzz test "maintains a seed corpus", a set of inputs "which are run by default", and those inputs "can also serve as regression tests for bugs identified through fuzzing", much like `@example` and the Hypothesis database.[^8] When fuzzing is on, the target "is called with arguments generated by repeatedly making random changes to the seed inputs", while Hypothesis builds each value from a strategy.[^8]

[^1]: Introduction to Hypothesis, Hypothesis documentation.
[^2]: Quickstart, Hypothesis documentation.
[^4]: Glossary, Hypothesis documentation.
[^5]: Replaying failed tests, Hypothesis documentation.
[^3]: Configuring test settings, Hypothesis documentation.
[^6]: Stateful tests, Hypothesis documentation.
[^7]: Package quick, Go standard library.
[^8]: Package testing, Go standard library.
