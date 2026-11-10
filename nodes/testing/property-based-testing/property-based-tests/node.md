---
tag: NEW-property-based-tests
kind: concept
slug: property-based-tests
name: Property-based testing
aliases: [property-based test, property test, QuickCheck-style testing, generative testing]
area: testing/property-based-testing
level: core
summary: >-
  A property-based test states a rule that must hold for all inputs of a kind. A tool generates many inputs, checks the rule on each, and shrinks a failing input to a small one.
applies_to: core-languages
viz: optional
formal: none
history:
  year: 2000
  people: [Koen Claessen, John Hughes]
  note: Claessen and Hughes presented QuickCheck, a Haskell library that tests properties written as functions on random inputs from type-driven generators, at ICFP 2000.
see_also: [unit-testing, fuzz-testing, mutation-analysis, higher-order-functions, closures, integer-overflow]
sources:
  - paper: doi:10.1145/351240.351266
    title: Koen Claessen and John Hughes, QuickCheck, A Lightweight Tool for Random Testing of Haskell Programs, ICFP 2000, pages 268 to 279
    locator: 2.1 A Simple Example; 2.3 Conditional Laws; 2.4 Monitoring Test Data; 3.1 Arbitrary; 3.2 Generators for User-Defined Types (sized); 6.1 On Random Testing; 6.2 Correctness Criteria
  - url: https://hackage.haskell.org/package/QuickCheck/docs/Test-QuickCheck.html
    title: QuickCheck package documentation, module Test.QuickCheck
    locator: the module description (minimal counterexample); class Arbitrary, shrink; data Args, replay
  - url: https://www.jstatsoft.org/article/view/v008i14
    title: George Marsaglia, Xorshift RNGs, Journal of Statistical Software 8(14), 2003
    locator: section 3, the 32-bit generator with the shifts 13, 17, 5 and the seed 2463534242
  - url: https://docs.python.org/3.14/library/random.html#notes-on-reproducibility
    title: The Python Standard Library, random, Python 3.14
    locator: Notes on Reproducibility
  - url: https://joss.theoj.org/papers/10.21105/joss.01891
    title: David R. MacIver, Zac Hatfield-Dodds and others, Hypothesis, A new approach to property-based testing, Journal of Open Source Software 4(43), 1891, 2019
    locator: Summary; Hypothesis for Software Testing Research
  - url: https://hypothesis.readthedocs.io/en/latest/tutorial/replaying-failures.html
    title: Hypothesis documentation, Replaying failed tests
    locator: The Hypothesis database; Using @example to run a specific input
  - url: https://fast-check.dev/
    title: fast-check documentation
    locator: the home page (property-based testing for JavaScript and TypeScript)
  - url: https://jqwik.net/
    title: jqwik, Property-Based Testing in Java
    locator: the home page (the JVM, Java and Kotlin)
  - url: https://proptest-rs.github.io/proptest/intro.html
    title: The proptest book
    locator: Introduction
  - url: https://fscheck.github.io/FsCheck/
    title: FsCheck documentation
    locator: the home page (F#, C# and VB; minimal counterexample)
  - url: https://pkg.go.dev/testing/quick
    title: Go standard library, package testing/quick
    locator: package overview (frozen); func Check; type Config
  - url: https://learn.microsoft.com/en-us/powershell/module/microsoft.powershell.utility/get-random#-setseed
    title: PowerShell documentation, Get-Random
    locator: the parameter -SetSeed
  - url: https://www.gnu.org/software/bash/manual/bash.html#index-RANDOM
    title: Bash Reference Manual
    locator: 5.2 Bash Variables, RANDOM
---

## Idea

An example-based test checks one input and one expected output. The author picks the input, so the test checks only the cases that the author thought of. A property-based test states a rule that must hold for every input of some kind. A tool then generates many inputs and checks the rule on each of them. When the rule fails, the tool reports the input, the counterexample.[^1]

The function `pages` below says how many pages a list of items needs. It has a bug: it drops the last page when that page is not full. Two example tests pass. The property "the pages hold all the items" fails on the first generated input.

```python run id=idea
def xorshift32(state):
    state ^= (state << 13) & 0xFFFFFFFF
    state ^= state >> 17
    state ^= (state << 5) & 0xFFFFFFFF
    return state


def pages(items, per_page):
    return items // per_page  # bug: drops the last page when it is not full


assert pages(100, 10) == 10
assert pages(0, 10) == 0
print("examples: pass")

state = 2463534242
for trial in range(1, 101):
    state = xorshift32(state)
    items = state % 1001
    state = xorshift32(state)
    per_page = 1 + state % 50
    if pages(items, per_page) * per_page < items:
        print(f"property: falsified on test {trial} by items={items}, per_page={per_page}")
        break
else:
    print("property: passed 100 tests")
```

```output for=idea
examples: pass
property: falsified on test 1 by items=967, per_page=7
```

The test does not need to know the right answer for 967 items on pages of 7. It only needs a rule that every right answer obeys. This is the main gain of the method. Claessen and Hughes call the question of whether a test passed the oracle problem, and a property is one answer to it.[^1]

## How it works

A property-based testing tool has four parts: properties, generators, a runner and a shrinker.

**Properties.** A property is a function from the generated inputs to true or false. In QuickCheck, a property is a Haskell function whose name starts with `prop_`, such as `prop_RevRev xs = reverse (reverse xs) == xs`.[^1] Some kinds of property occur again and again:

- **Round trip.** Decoding undoes encoding: `parse(format(x)) == x`. Serializers, parsers and codecs have this property.
- **Invariant.** The output always has some feature: a sorted list is ordered, a page count covers all items, a balanced tree stays balanced.
- **Oracle.** The result equals the result of a second implementation, such as a slower one that is clearly correct. Claessen and Hughes name this as a common solution to the oracle problem.[^1]
- **Algebraic law.** An operation is associative, commutative or idempotent, or two operations distribute. QuickCheck's first example tests three laws of `reverse`.[^1]
- **Conditional property.** The rule holds only when a precondition holds. QuickCheck writes this `x <= y ==> max x y == y` and discards the inputs that fail the precondition.[^1]

**Generators.** A generator makes a random value of a type. QuickCheck links generators to types through the type class `Arbitrary`, so a property over `[Int]` gets a generator for lists of integers without more code.[^1] A generator is a function from a source of random numbers to a value, and larger generators are built from smaller ones. QuickCheck also passes a size bound to each generator. It starts with small values and lets the size grow as the run goes on, which makes small counterexamples more likely.[^1]

**The runner.** The runner calls the generator, checks the property, and stops at the first failure. QuickCheck runs 100 tests by default; the paper calls this number arbitrary.[^1] A run must be repeatable, or a failure seen once cannot be debugged. So the runner uses a pseudo-random generator with a seed, prints the seed, and can replay it. QuickCheck's `Args` has a field `replay` for this. Hypothesis also saves each failing input in a local database and tries it first on the next run.[^2][^6]

**The shrinker.** A random counterexample is often large. A shrinker tries smaller inputs and keeps one that still fails, until no smaller candidate fails. QuickCheck's documentation asks a shrinker to try at least the immediate parts of a value, the value with its parts shrunk, and simpler constructors.[^2] For an integer, the checker below tries the minimum first, then values that move toward the failing value in halving steps. The 2000 paper of QuickCheck does not describe shrinking. QuickCheck's library has it now through the method `shrink` of `Arbitrary`.[^1][^2]

The checker below has all four parts. It uses Marsaglia's xorshift generator with 32-bit state and the shifts 13, 17 and 5, seeded with 2463534242, the seed in his paper.[^3] The examples in each language use the same generator, so each prints the same lines.

```python run id=checker
SEED = 2463534242  # the seed in Marsaglia's xorshift paper


class Rng:
    """xorshift32: the same numbers in every language and on every run."""

    def __init__(self, seed):
        self.state = seed

    def next(self):
        y = self.state
        y ^= (y << 13) & 0xFFFFFFFF
        y ^= y >> 17
        y ^= (y << 5) & 0xFFFFFFFF
        self.state = y
        return y

    def between(self, lo, hi):
        return lo + self.next() % (hi - lo + 1)


def check(name, generate, holds, shrink, trials=100):
    rng = Rng(SEED)
    for trial in range(1, trials + 1):
        case = generate(rng)
        if holds(case):
            continue
        print(f"{name}: falsified on test {trial} by {case}")
        steps = 0
        while True:
            smaller = next((c for c in shrink(case) if not holds(c)), None)
            if smaller is None:
                break
            case, steps = smaller, steps + 1
        print(f"{name}: shrunk in {steps} steps to {case}")
        return
    print(f"{name}: passed {trials} tests")


def shrink_int(x, lo):
    """Candidates from the smallest up: lo, then x - d for d = (x - lo) / 2, / 4, ..., 1."""
    candidates = [lo] if x > lo else []
    d = (x - lo) // 2
    while d > 0:
        candidates.append(x - d)
        d //= 2
    return candidates


# Property 1, a round trip: parsing the decimal form gives the number back.
check(
    "round trip",
    lambda rng: rng.between(-(2**31), 2**31 - 1),
    lambda x: int(str(x)) == x,
    lambda x: [],
)


# Property 2, an invariant: the pages hold all the items.
def pages(items, per_page):
    return items // per_page  # bug: drops the last page when it is not full


check(
    "pages",
    lambda rng: (rng.between(0, 1000), rng.between(1, 50)),
    lambda c: pages(*c) * c[1] >= c[0],
    lambda c: [(n, c[1]) for n in shrink_int(c[0], 0)] + [(c[0], p) for p in shrink_int(c[1], 1)],
)
```

```output for=checker
round trip: passed 100 tests
pages: falsified on test 1 by (967, 7)
pages: shrunk in 13 steps to (1, 2)
```

The shrunk input (1, 2) explains the bug at once: one item on pages of two needs one page, and `1 // 2` is 0. The fix is the rounded-up quotient `(items + per_page - 1) // per_page`. The shrinker does not find the smallest input in every case. It finds an input that no single step can make smaller, a local minimum. With a shrinker that tried only the halves, this run stopped at (241, 2), because every even count passes.

## In languages

The example files run the same checker in each language. The table gives the verdict for each tier 1 language. A verdict of library means that a mature property testing library exists; the examples use only the standard library.

| Language | Verdict | Form |
|---|---|---|
| Python | library | Hypothesis: `@given` with strategies[^5] |
| JavaScript | library | fast-check[^7] |
| TypeScript | library | fast-check, with typed arbitraries[^7] |
| Java | library | jqwik, a test engine for the JVM[^8] |
| C | idiom | a loop over a seeded generator, as in the example |
| C++ | idiom | a loop over a seeded generator; a generic checker with templates and lambdas |
| C# | library | FsCheck, which runs properties from F#, C# or VB[^10] |
| Go | stdlib | `testing/quick.Check`, with a seeded `Config.Rand`; it does not shrink[^11] |
| Rust | library | proptest, in the QuickCheck family[^9] |
| Kotlin | library | jqwik, which supports Java and Kotlin[^8] |
| PowerShell | idiom | a loop over `Get-Random -SetSeed`[^12] |
| Bash | idiom | a loop over `$RANDOM`, seeded by an assignment[^13] |
| SQL | n/a | a query has no test runner |
| Regex | n/a | a pattern is tested through the language that uses it |
| HTML/CSS | n/a | markup and style rules have no functions to test |

Go is the only tier 1 language with a property checker in its standard library. `testing/quick.Check` calls a function that returns a Boolean with random arguments made by reflection, and it takes a `*rand.Rand` in its `Config`. The package is frozen and accepts no more features.[^11] Its documentation describes no shrinking. Hypothesis uses one internal representation for all generated data, so it can shrink any input with no shrinker from the user.[^5] Proptest also defines generation and shrinking for each value rather than for each type.[^9]

## Variations and related ideas

- **Type-driven and value-driven generators.** QuickCheck finds the generator from the type of the property through `Arbitrary`.[^1] Hypothesis and proptest pass generators as values, called strategies. The proptest book says that generation and shrinking for each value, not for each type, is more flexible and composes better.[^9]
- **Integrated shrinking.** In QuickCheck, the generator and the shrinker are separate methods of `Arbitrary`, and a type with invariants needs a shrinker that keeps them.[^2] Hypothesis keeps one internal representation for all test data and shrinks that, so the user writes no shrinker.[^5]
- **Explicit examples.** Hypothesis's `@example` adds fixed inputs that run on each run before the generated ones. They do not shrink.[^6] This joins example-based tests and properties in one test.
- **Stateful or model-based testing.** The tool generates a sequence of operations on an object and compares each result with a small model. FsCheck calls this model-based testing.[^10]
- **Fuzz testing.** A fuzzer also runs a program on many generated inputs, but it looks for crashes and hangs, not for the failure of a stated rule. Coverage-guided fuzzers steer the inputs with feedback from the program. See [[fuzz-testing]].
- **Exhaustive testing of small inputs.** For a small domain, a test can check every input up to some size instead of a random sample. The checker then needs no seed and no shrinker.

## Pitfalls

### The generator never makes the input that fails

A property can only fail on an input that the generator makes. Code that pads a column of text often assumes that `str.upper` keeps the length of a string. A generator of ASCII letters supports the assumption.

```python wrong id=ascii-only
def xorshift32(state):
    state ^= (state << 13) & 0xFFFFFFFF
    state ^= state >> 17
    state ^= (state << 5) & 0xFFFFFFFF
    return state


ALPHABET = "abcdefABCDEF"
state = 2463534242
for trial in range(1, 101):
    state = xorshift32(state)
    length = state % 9
    chars = []
    for _ in range(length):
        state = xorshift32(state)
        chars.append(ALPHABET[state % len(ALPHABET)])
    s = "".join(chars)
    if len(s.upper()) != len(s):
        print(f"falsified on test {trial} by {s!r}")
        break
else:
    print("passed 100 tests")
```

```output for=ascii-only
passed 100 tests
```

With letters outside ASCII in the alphabet, the same loop finds a string whose upper case is longer. The German letter `ß` becomes `SS`.

```python run id=wider
def xorshift32(state):
    state ^= (state << 13) & 0xFFFFFFFF
    state ^= state >> 17
    state ^= (state << 5) & 0xFFFFFFFF
    return state


ALPHABET = "abcABCéÉßΣ"
state = 2463534242
for trial in range(1, 101):
    state = xorshift32(state)
    length = state % 9
    chars = []
    for _ in range(length):
        state = xorshift32(state)
        chars.append(ALPHABET[state % len(ALPHABET)])
    s = "".join(chars)
    if len(s.upper()) != len(s):
        print(f"falsified on test {trial} by {s!r}")
        print(f"{s.upper()!r} has {len(s.upper())} characters")
        break
else:
    print("passed 100 tests")
```

```output for=wider
falsified on test 5 by 'ΣBßΣécba'
'ΣBSSΣÉCBA' has 9 characters
```

Measure what the generator makes. QuickCheck has the combinators `classify` and `collect` to show the distribution of the test data.[^1] Add the known edge cases as explicit examples: zero, the minimum and maximum of a type, the empty string, characters outside ASCII.

### A precondition throws most inputs away

A conditional property discards each input that fails its precondition. When the precondition is rare, the property runs on few inputs, and those few are biased toward the cases that meet the precondition by chance. Claessen and Hughes found that a property of insertion into ordered lists ran mostly on lists of length 0 and 1, because a random long list is rarely ordered.[^1]

```python wrong id=discard
def xorshift32(state):
    state ^= (state << 13) & 0xFFFFFFFF
    state ^= state >> 17
    state ^= (state << 5) & 0xFFFFFFFF
    return state


def is_ordered(xs):
    return all(a <= b for a, b in zip(xs, xs[1:]))


state = 2463534242
kept = {}
for _ in range(100):
    state = xorshift32(state)
    xs = []
    for _ in range(state % 7):
        state = xorshift32(state)
        xs.append(state % 10)
    if not is_ordered(xs):
        continue  # the precondition fails: discard the input
    kept[len(xs)] = kept.get(len(xs), 0) + 1
print("kept", sum(kept.values()), "of 100; by length:", dict(sorted(kept.items())))
```

```output for=discard
kept 44 of 100; by length: {0: 16, 1: 14, 2: 10, 3: 4}
```

Fewer than half of the inputs met the precondition, and none of them was longer than three. The fix is a generator that makes only valid inputs. QuickCheck's paper replaces the precondition with a generator of ordered lists.[^1] Here, sorting the generated list makes every input valid, and all lengths occur.

```python run id=no-discard
def xorshift32(state):
    state ^= (state << 13) & 0xFFFFFFFF
    state ^= state >> 17
    state ^= (state << 5) & 0xFFFFFFFF
    return state


state = 2463534242
kept = {}
for _ in range(100):
    state = xorshift32(state)
    xs = []
    for _ in range(state % 7):
        state = xorshift32(state)
        xs.append(state % 10)
    xs.sort()  # generate ordered lists directly
    kept[len(xs)] = kept.get(len(xs), 0) + 1
print("kept", sum(kept.values()), "of 100; by length:", dict(sorted(kept.items())))
```

```output for=no-discard
kept 100 of 100; by length: {0: 16, 1: 14, 2: 15, 3: 17, 4: 9, 5: 13, 6: 16}
```

### The property restates the code

A property that computes the expected value in the same way as the code under test passes whenever the code runs. It cannot find a bug in the logic, because the bug is in both places.

```python wrong id=restate
def xorshift32(state):
    state ^= (state << 13) & 0xFFFFFFFF
    state ^= state >> 17
    state ^= (state << 5) & 0xFFFFFFFF
    return state


def pages(items, per_page):
    return items // per_page  # the same bug as before


state = 2463534242
for trial in range(1, 101):
    state = xorshift32(state)
    items = state % 1001
    state = xorshift32(state)
    per_page = 1 + state % 50
    assert pages(items, per_page) == items // per_page  # restates the code
print("passed 100 tests")
```

```output for=restate
passed 100 tests
```

State what a right answer must satisfy, not how to compute it. For `pages`, there are two rules: the pages hold all the items (`pages * per_page >= items`), and one page fewer does not (`(pages - 1) * per_page < items`). Together they define the rounded-up quotient without computing it.

### A seed that the run does not show

A failure from an unseeded run may not occur again. Print the seed of each run, and let the test accept a seed from outside. Do not depend on a library generator to give the same numbers forever. Python guarantees only that `random()` gives the same sequence for the same seed. Most other functions of the `random` module may change between Python versions.[^4] The examples on this page carry their own xorshift generator for this reason.

## Cost

- **Time per test.** A property runs its body many times, 100 by default in QuickCheck, plus the shrinking steps after a failure.[^1] A property over a function that takes milliseconds makes a test that takes seconds. The count is a parameter in QuickCheck, so a run can trade time for coverage.[^1]
- **Thought per test.** A good property needs a rule about the code, which takes more thought than one input and one output.
- **Generators.** A generator for a domain type is code, and it can be wrong in the ways shown in Pitfalls. QuickCheck reduces the cost through type classes, and Hypothesis through composable strategies.[^1][^5]
- **Flakiness.** A run that is not seeded can fail on one run and pass on the next. A seeded run with a saved database of failures gives a test that fails in the same way until the bug is fixed.[^6]

## History

- **2000.** Koen Claessen and John Hughes presented QuickCheck at ICFP 2000. It tested properties written as Haskell functions on random inputs from generators chosen by type. The paper compares random testing with partition testing and argues that random testing at a fine grain gives good coverage.[^1]
- **2003.** George Marsaglia described the xorshift generators, which need only shifts and exclusive-or operations.[^3]
- **2006.** Arts, Hughes, Johansson and Wiger described tests of telecoms software with Quviq QuickCheck, a version for Erlang.[^5]
- **2019.** David MacIver, Zac Hatfield-Dodds and others described Hypothesis for Python in the Journal of Open Source Software. Hypothesis shrinks a single internal representation of the test data.[^5]

## References
