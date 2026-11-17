---
tag: 6WNBX3
kind: feature
owner: python
slug: future-statements
name: Future statements
aliases: [future statement, from __future__ import, __future__ module, from __future__ import annotations, PEP 236, PEP 563, compiler_flag, all_feature_names]
area: languages/python/modules-and-packages
level: advanced
summary: >-
  from __future__ import feature tells the compiler to use a later meaning for one module. It must come first in the file. Only annotations still changes anything, and the other features are always on.
grammar: null
spec: {standard: null, url: "https://docs.python.org/3.15/reference/simple_stmts.html#future-statements", locator: "7.11.2. Future statements"}
see_also: [python/the-import-statement, python/modules, python/lazy-imports, python/module-objects]
sources:
- url: https://docs.python.org/3.15/reference/simple_stmts.html#future-statements
  title: The Python Language Reference, 7. Simple statements
  locator: 7.11.2. Future statements
  accessed: 2026-11-18
- url: https://docs.python.org/3.15/library/__future__.html
  title: __future__ — Future statement definitions
  locator: Module Contents; _Feature.getMandatoryRelease; _Feature.compiler_flag
  accessed: 2026-11-18
- url: https://peps.python.org/pep-0563/
  title: "PEP 563: Postponed Evaluation of Annotations"
  locator: Abstract; Resolving Type Hints at Runtime; Runtime annotation resolution and TYPE_CHECKING
  accessed: 2026-11-18
- url: https://peps.python.org/pep-0749/
  title: "PEP 749: Implementing PEP 649"
  locator: from __future__ import annotations
  accessed: 2026-11-18
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Strict_mode
  title: Strict mode, JavaScript reference, MDN
  locator: Invoking strict mode
  accessed: 2026-11-18
- url: https://peps.python.org/pep-0236/
  title: "PEP 236: Back to the __future__"
  locator: Motivation; Intent
  accessed: 2026-11-18
---

## Synopsis

```text
"""The module docstring."""          # only a docstring, comments, blank lines
# a comment                          # and other future statements may come before it
from __future__ import annotations   # annotations are kept as strings
from __future__ import division, generator_stop   # accepted, no effect: always on

import __future__                    # an ordinary import, not a future statement
__future__.annotations.compiler_flag # the flag for compile()
```

## Semantics

**r1.** "A future statement is a directive to the compiler that a particular module should be compiled using syntax or semantics that will be available in a specified future release of Python where the feature becomes standard."[^1] "It allows use of the new features on a per-module basis before the release in which the feature becomes standard."[^1] Example: [`annotations`](#annotations).

**r2.** "A future statement must appear near the top of the module. The only lines that can appear before a future statement are: the module docstring (if any), comments, blank lines, and other future statements."[^1] Examples: [`annotations`](#annotations), [`not-first`](#not-first).

**r3.** The compiler checks the name: "For any given release, the compiler knows which feature names have been defined, and raises a compile-time error if a future statement contains a feature not known to it."[^1] Example: [`unknown-feature`](#unknown-feature).

**r4.** "The only feature that requires using the future statement is annotations (see PEP 563)."[^1] The older features "are all redundant because they are always enabled, and only kept for backwards compatibility."[^1] Example: [`old-features`](#old-features).

**r5.** At run time it is also an import. "The direct runtime semantics are the same as for any import statement: there is a standard module __future__, described later, and it will be imported in the usual way at the time the future statement is executed."[^1] The plain statement `import __future__` "is not a future statement; it’s an ordinary import statement with no special semantics or syntax restrictions."[^1] Examples: [`import-future`](#import-future), [`feature-table`](#feature-table).

**r6.** A future statement also applies to code that the module compiles. "Code compiled by calls to the built-in functions exec() and compile() that occur in a module M containing a future statement will, by default, use the new syntax or semantics associated with the future statement."[^1] Other code can ask for the feature with a flag. The `compiler_flag` attribute "is the (bitfield) flag that should be passed in the fourth argument to the built-in function compile() to enable the feature in dynamically compiled code."[^2] Example: [`compile-flag`](#compile-flag).

**r7.** The `annotations` feature comes from PEP 563, which changes annotations "so that they are no longer evaluated at function definition time. Instead, they are preserved in __annotations__ in string form."[^3] Example: [`annotations`](#annotations).

## Examples

A docstring and a comment may come before the future statement (rule r2). With the feature, the annotations are strings, so the name `Shape` does not need to exist (rule r7).

```python run id=annotations
"""Area of shapes."""

# The future statement comes after the docstring and comments.
from __future__ import annotations


def area(shape: Shape) -> float:
    return shape.area()


print(area.__annotations__)
```

```output for=annotations
{'shape': 'Shape', 'return': 'float'}
```

The old features are still accepted, but they change nothing (rule r4). Division gives a float with or without the statement.

```python run id=old-features
from __future__ import absolute_import, division, generator_stop, print_function

print(7 / 2, 7 // 2)
```

```output for=old-features
3.5 3
```

`import __future__` is an ordinary import (rule r5). It gives the module, and it does not change the annotations.

```python run id=import-future
import __future__


def scale(x: int) -> float:
    return x * 1.5


print(scale.__annotations__)
print(__future__.annotations)
```

```output for=import-future
{'x': <class 'int'>, 'return': <class 'float'>}
_Feature((3, 7, 0, 'beta', 1), None, 16777216)
```

The module `__future__` records each feature (rule r5). The library page says that it is "a form of executable documentation, and can be inspected programmatically via importing __future__ and examining its contents."[^2]

```python run id=feature-table
import __future__

for name in ("division", "generator_stop", "annotations"):
    feature = getattr(__future__, name)
    print(name, feature.getOptionalRelease()[:2], feature.getMandatoryRelease())
```

```output for=feature-table
division (2, 2) (3, 0, 0, 'alpha', 0)
generator_stop (3, 5) (3, 7, 0, 'alpha', 0)
annotations (3, 7) None
```

Code that `exec()` compiles in a module with the future statement uses the feature too (rule r6). In a module without it, the flag of the feature turns it on for one call of `compile()`.

```python norun reason=fragment file=withfuture.py group=compile-flag
from __future__ import annotations

namespace = {}
exec("def f(x: int): pass", namespace)
print(namespace["f"].__annotations__)
```

```python run file=main.py group=compile-flag id=compile-flag
import __future__

import withfuture

source = "def g(y: int): pass"

plain = {}
exec(compile(source, "<g>", "exec"), plain)
print(plain["g"].__annotations__)

flagged = {}
exec(compile(source, "<g>", "exec", __future__.annotations.compiler_flag), flagged)
print(flagged["g"].__annotations__)
```

```output for=compile-flag
{'x': 'int'}
{'y': <class 'int'>}
{'y': 'int'}
```

The examples give the same output on CPython 3.12.3, 3.13.14, 3.14.6 and 3.15.0b2.

## Versions

The library page says of `annotations`: "from __future__ import annotations was previously scheduled to become mandatory in Python 3.10, but the change was delayed and ultimately canceled. This feature will eventually be deprecated and removed."[^2] So its mandatory release is `None` in [`feature-table`](#feature-table). PEP 749 gives the plan: "Sometime after the last release that did not support PEP 649 semantics (expected to be 3.13) reaches its end-of-life, from __future__ import annotations is deprecated. Compiling any code that uses the future import will emit a DeprecationWarning."[^4] "Code that continues to use the future import will raise a SyntaxError, similar to any other undefined future import."[^4] CPython 3.15.0b2 gives no warning for [`annotations`](#annotations).

## Pitfalls

### A statement before the future statement

An `import` before the future statement is a syntax error (rule r2). The whole module does not compile.

```python fails id=not-first expect=exception:SyntaxError
import sys

from __future__ import annotations

print(sys.version_info >= (3, 7))
```

Put the future statements first, after the docstring.

### A misspelled feature

The name is checked when the module is compiled (rule r3). A misspelled or unknown name is a `SyntaxError`, not an `ImportError`.

```python fails id=unknown-feature expect=exception:SyntaxError
from __future__ import annotation

print("not reached")
```

### String annotations at run time

With the feature, code that reads `__annotations__` gets strings (rule r7). PEP 563 says: "For code that uses type hints, the typing.get_type_hints(obj, globalns=None, localns=None) function correctly evaluates expressions back from its string form."[^3] This fails for a name that exists only for the type checker: "Trying to resolve annotations of a_func at runtime using typing.get_type_hints() will fail since the name expensive_mod is not defined (TYPE_CHECKING variable being False at runtime)."[^3]

```python fails id=type-hints expect=exception:NameError
from __future__ import annotations

import typing

if typing.TYPE_CHECKING:
    from decimal import Decimal


def total(price: Decimal) -> Decimal:
    return price


print(total.__annotations__)
typing.get_type_hints(total)
```

## Comparison

**JavaScript** has one directive of this kind. To invoke strict mode for an entire script, MDN says to put the statement `"use strict";` "before any other statements."[^5] The rule of the position is like rule r2. "JavaScript modules are automatically in strict mode, with no statement needed to initiate it."[^5] A Python future statement names one feature, and each feature has its own release dates in `__future__`.

## Implementation notes

The future statement is handled by the compiler: "Such decisions cannot be pushed off until runtime."[^1] PEP 236 gives the reason for the mechanism: "From time to time, Python makes an incompatible change to the advertised semantics of core language constructs, or changes their accidental (implementation-dependent) behavior in some way."[^6] With a future statement, "old code continues to work by default, for at least one release, although it may start to generate new warning messages."[^6] The library page explains why a real module exists: "To avoid confusing existing tools that analyze import statements and expect to find the modules they’re importing."[^2]

## References
