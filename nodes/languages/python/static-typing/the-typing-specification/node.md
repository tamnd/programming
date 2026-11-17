---
tag: FJGTC9
kind: feature
owner: python
slug: the-typing-specification
name: The typing specification
aliases: [typing spec, Python typing specification, Specification for the Python type system, typing.python.org, Typing Council, typing council, conformance tests, conformance test suite, PEP 729]
area: languages/python/static-typing
level: research
summary: >-
  The typing specification at typing.python.org describes the Python type system for all checkers. The Typing Council of PEP 729 maintains it, and a conformance suite tests each checker against it.
grammar: null
spec: {standard: null, url: "https://typing.python.org/en/latest/spec/index.html", locator: "Specification for the Python type system"}
proposals: [pep-729, pep-484]
see_also: [python/type-hints, python/type-checkers, python/type-narrowing, python/stub-files-and-typeshed, python/dataclass-transform-and-typing-extensions, python/special-typing-forms]
sources:
  - url: https://typing.python.org/en/latest/spec/type-system.html
    title: The Python Type System, Specification for the Python type system
    locator: Purpose; Non-goals
    accessed: 2026-11-18
  - url: https://typing.python.org/en/latest/spec/meta.html
    title: Meta-topics, Specification for the Python type system
    locator: About this specification; Changing the specification
    accessed: 2026-11-18
  - url: https://peps.python.org/pep-0729/
    title: "PEP 729: Typing governance process"
    locator: Abstract; Mandate; Operations and process
    accessed: 2026-11-18
  - url: https://github.com/python/typing-council/blob/main/README.md
    title: Python Typing Council README
    locator: About the Typing Council; Decisions
    accessed: 2026-11-18
  - url: https://github.com/python/typing/blob/main/conformance/README.md
    title: Type System Conformance README
    locator: Motivation; Notes About Tests; Test Case Syntax; Reporting Conformance Results
    accessed: 2026-11-18
  - url: https://github.com/python/typing/blob/main/conformance/results/results.html
    title: Python Type System Conformance Test Results
    locator: header row; annotations_forward_refs
    accessed: 2026-11-18
---

## Synopsis

```python run id=conformance-test
from typing import TYPE_CHECKING, Final

RATE: Final = 0.2

if TYPE_CHECKING:
    ok: int = 1
    RATE = 0.5  # E: cannot assign to a Final name
    bad: int = "1"  # E: str is not assignable to int

print(RATE)
```

```output for=conformance-test
0.2
```

This file has the form of a test in the conformance suite. Each line that a checker must reject has a comment that starts with `# E`. Mypy reports an error on each of these lines and on no other line, so it passes this test. Saved as `conf.py`:

```text
$ mypy --strict conf.py
conf.py:7: error: Cannot assign to final name "RATE"  [misc]
conf.py:8: error: Incompatible types in assignment (expression has type "str", variable has type "int")  [assignment]
Found 2 errors in 1 file (checked 1 source file)
```

## Semantics

**r1.** The specification is the single description of the type system. "This specification aims to provide a full description of the Python type system. For type checker authors, it provides a complete description of expected semantics. For library authors, it provides guarantees to rely on when working with multiple type checkers."[^1]

**r2.** The text started as the PEPs. "The initial text consists of the “Specification” sections of PEP 484 and subsequent typing-related PEPs, pasted together and reorganized."[^2] "This document is intended to replace those PEPs".[^1] Parts of it are not complete: "the PEPs are uneven in depth and do not fully cover all aspects of the type system. Addressing these issues is an ongoing project."[^1]

**r3.** The specification does not make Python a statically typed language. "It should also be emphasized that Python will remain a dynamically typed language, and there is no desire to ever make type hints mandatory, even by convention."[^1] Example: [`not-mandatory`](#not-mandatory).

**r4.** The Typing Council governs the type system. PEP 729 "proposes a new way to govern the Python type system: a council that is responsible for maintaining and developing the Python type system. The council will maintain a specification and conformance test suite and will initially be appointed by the Python Steering Council."[^3] "Its mandate is to ensure that the type system is useful, usable, and stable."[^4] "The council would have three to five members".[^3]

**r5.** A change goes one of three ways. "Minor, non-substantive changes can simply be proposed as PRs to the python/typing repository".[^2] "Substantive changes that do not rise to the level of a PEP must be approved by the Typing Council."[^2] "Major changes should go through the PEP process, as described in PEP 1."[^2]

**r6.** The conformance test suite checks the checkers. "Accompanying the typing specification is this conformance test suite which validates the behavior of static type checkers against the specification."[^5] "Lines that are expected to produce a type checker error should have a comment starting with # E".[^5] A line where the specification allows more than one behavior has the comment `# E?` in place of `# E`.[^5] Examples: [`conformance-test`](#conformance-test), [`scorer`](#scorer).

**r7.** People score the results. "Some level of manual inspection will therefore be needed to determine whether a type checker is fully conformant with all tests in any given test file."[^5] The results page gives the result Pass or Partial for each test and checker, with notes, and it warns: "While specification conformance is important for the ecosystem, we don't recommend using it as the primary basis for choosing a type checker."[^6]

## Examples

```python run id=scorer
import re

test = """\
x: int = 1
y: int = "1"  # E
z: str = 2  # E
w = x + 1
"""
reported = {2, 4}

expected = {n for n, line in enumerate(test.splitlines(), 1) if re.search(r"# E\b", line)}
print("expected", sorted(expected))
print("missed", sorted(expected - reported), "extra", sorted(reported - expected))
print("Pass" if expected == reported else "Partial")
```

```output for=scorer
expected [2, 3]
missed [3] extra [4]
Partial
```

A toy scorer compares the `# E` lines with the lines where a checker reported an error. Here, the checker missed line 3 and reported line 4, which is correct code (rule r6). The real suite has more forms, such as `# E?` and tags, and people check the results (rule r7).

## Versions

The specification has no Python version of its own. The council changes it when a PEP or a council decision is accepted (rule r5), and each feature in it has the version of the PEP that added it. For these versions, see the page of each feature.

## Pitfalls

```python wrong id=not-mandatory
def double(x: int) -> int:
    return x * 2


print(double("ab"), double.__annotations__)
```

```output for=not-mandatory
abab {'x': <class 'int'>, 'return': <class 'int'>}
```

The specification tells checkers what to do, not the interpreter. Python runs the call, and the hints stay as data (rule r3). Only a checker reports the call:

```text
$ mypy --strict notmandatory.py
notmandatory.py:5: error: Argument 1 to "double" has incompatible type "str"; expected "int"  [arg-type]
Found 1 error in 1 file (checked 1 source file)
```

## Comparison

In Python, the type system has a specification that is separate from the language reference and from each checker (rule r1). Before it, the PEPs and the behavior of mypy were the de facto rules. TypeScript has one compiler, and the compiler is the specification. Java and C# have a language specification that includes the type rules, and the compiler must follow it. For the checkers that the suite measures, see [[python/type-checkers]].
