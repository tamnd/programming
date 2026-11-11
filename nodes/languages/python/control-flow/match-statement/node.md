---
tag: 77MP54
kind: feature
owner: python
slug: match-statement
name: The match statement
area: languages/python/control-flow
level: core
summary: >-
  The match statement compares one value with a list of patterns, in order. The first pattern that fits runs its block, and it can bind parts of the value to names.
grammar: "match_stmt in the Python grammar (Language Reference, 8.6 and 10. Full Grammar specification)"
spec: {standard: null, url: "https://docs.python.org/3.15/reference/compound_stmts.html#the-match-statement", locator: "8.6"}
proposals: [pep-634, pep-635, pep-636]
compat: compat.yaml
since: "3.10"
sources:
  - url: https://docs.python.org/3.15/reference/compound_stmts.html#the-match-statement
    title: The Python Language Reference, Python 3.15
    locator: 8.6 The match statement; 8.6.1 Overview
    accessed: 2026-11-09
  - url: https://docs.python.org/3.15/reference/compound_stmts.html#guards
    title: The Python Language Reference, Python 3.15
    locator: 8.6.2 Guards
  - url: https://docs.python.org/3.15/reference/compound_stmts.html#irrefutable-case-blocks
    title: The Python Language Reference, Python 3.15
    locator: 8.6.3 Irrefutable Case Blocks
  - url: https://docs.python.org/3.15/reference/compound_stmts.html#literal-patterns
    title: The Python Language Reference, Python 3.15
    locator: 8.6.4.3 Literal Patterns
  - url: https://docs.python.org/3.15/reference/compound_stmts.html#capture-patterns
    title: The Python Language Reference, Python 3.15
    locator: 8.6.4.4 Capture Patterns; 8.6.4.5 Wildcard Patterns
  - url: https://docs.python.org/3.15/reference/compound_stmts.html#value-patterns
    title: The Python Language Reference, Python 3.15
    locator: 8.6.4.6 Value Patterns; 8.6.4.7 Group Patterns
  - url: https://docs.python.org/3.15/reference/compound_stmts.html#sequence-patterns
    title: The Python Language Reference, Python 3.15
    locator: 8.6.4.8 Sequence Patterns
  - url: https://docs.python.org/3.15/reference/compound_stmts.html#mapping-patterns
    title: The Python Language Reference, Python 3.15
    locator: 8.6.4.9 Mapping Patterns
  - url: https://docs.python.org/3.15/reference/compound_stmts.html#class-patterns
    title: The Python Language Reference, Python 3.15
    locator: 8.6.4.10 Class Patterns
  - url: https://docs.python.org/3.15/reference/compound_stmts.html#or-patterns
    title: The Python Language Reference, Python 3.15
    locator: 8.6.4.1 OR Patterns; 8.6.4.2 AS Patterns
  - url: https://docs.python.org/3.15/reference/lexical_analysis.html#soft-keywords
    title: The Python Language Reference, Python 3.15
    locator: 2.3.2 Soft Keywords
  - url: https://peps.python.org/pep-0634/
    title: "PEP 634: Structural Pattern Matching: Specification"
    locator: The Match Statement; Patterns
  - url: https://peps.python.org/pep-0635/
    title: "PEP 635: Structural Pattern Matching: Motivation and Rationale"
    locator: Rationale and Goals
  - url: https://peps.python.org/pep-0636/
    title: "PEP 636: Structural Pattern Matching: Tutorial"
    locator: Tutorial
  - url: https://docs.python.org/3.15/library/dataclasses.html#dataclasses.dataclass
    title: dataclasses, Python 3.15 documentation
    locator: "@dataclass, parameter match_args"
  - url: https://docs.python.org/3.10/whatsnew/3.10.html#pep-634-structural-pattern-matching
    title: What's New In Python 3.10
    locator: "PEP 634: Structural Pattern Matching"
  - url: https://docs.python.org/3.15/whatsnew/3.15.html#other-language-changes
    title: What's New In Python 3.15
    locator: Other language changes (unary plus in literal patterns, gh-145239)
  - url: https://docs.python.org/3.15/library/dis.html#opcode-MATCH_CLASS
    title: dis, Python 3.15 documentation
    locator: Python Bytecode Instructions, MATCH_CLASS, MATCH_MAPPING, MATCH_SEQUENCE, MATCH_KEYS
  - url: https://doc.pypy.org/en/latest/index-of-release-notes.html
    title: PyPy documentation, Historical release notes
    locator: PyPy v7.3.12, release of python 2.7, 3.9, and 3.10
  - url: https://github.com/micropython/micropython/blob/master/README.md
    title: MicroPython README
    locator: Overview (Python 3.4 syntax with select later features)
  - url: https://doc.rust-lang.org/reference/expressions/match-expr.html
    title: The Rust Reference
    locator: Match expressions, rules expr.match.scrutinee-value and expr.match.guard
  - url: https://doc.rust-lang.org/error_codes/E0004.html
    title: Rust Compiler Error Index
    locator: E0004
  - url: https://scala-lang.org/files/archive/spec/3.4/08-pattern-matching.html
    title: Scala Language Specification, version 3.4
    locator: 8.4 Pattern Matching Expressions
  - url: https://learn.microsoft.com/en-us/dotnet/csharp/language-reference/operators/switch-expression
    title: switch expression, C# language reference
    locator: Non-exhaustive switch expressions; Case guards
  - url: https://openjdk.org/jeps/441
    title: "JEP 441: Pattern Matching for switch"
    locator: Exhaustiveness of switch expressions and statements; Guarded case labels
---

## Synopsis

A `match` statement has a subject expression and one or more `case` blocks. Each `case` has a pattern and an optional guard.[^1] The grammar below is a short form of the rules in the reference; `NAME`, `NUMBER` and `strings` are tokens of the Python grammar.

```text
match_stmt      <- "match" subject ":" NEWLINE INDENT case_block+ DEDENT
subject         <- expression ("," expression)* ","?
case_block      <- "case" patterns guard? ":" block
guard           <- "if" named_expression
patterns        <- open_sequence_pattern / pattern
pattern         <- or_pattern ("as" NAME)?
or_pattern      <- closed_pattern ("|" closed_pattern)*
closed_pattern  <- literal_pattern / capture_pattern / wildcard_pattern
                 / value_pattern / group_pattern / sequence_pattern
                 / mapping_pattern / class_pattern
literal_pattern <- ("+" / "-")? NUMBER (("+" / "-") NUMBER)? / strings / "None" / "True" / "False"
capture_pattern <- !"_" NAME
wildcard_pattern <- "_"
value_pattern   <- NAME ("." NAME)+
group_pattern   <- "(" pattern ")"
sequence_pattern <- "[" items? "]" / "(" (item "," items?)? ")"
items           <- item ("," item)* ","?
item            <- "*" NAME / pattern
mapping_pattern <- "{" (key_value ("," key_value)* ","?)? "}"
key_value       <- (literal_pattern / value_pattern) ":" pattern / "**" NAME
class_pattern   <- NAME ("." NAME)* "(" arguments? ")"
arguments       <- pattern ("," pattern)* ("," NAME "=" pattern)* ","? / NAME "=" pattern ("," NAME "=" pattern)* ","?
```

The forms of a pattern, with one example of each:

| Pattern | Example | Matches when |
|---|---|---|
| literal | `0`, `-1.5`, `+1`, `3 + 4j`, `"go"`, `b"\x00"`, `None`, `True` | the subject equals the literal |
| capture | `name` | always; binds the subject to `name` |
| wildcard | `_` | always; binds nothing |
| value | `Color.RED`, `http.HTTPStatus.OK` | the subject equals the value of the dotted name |
| group | `(1 \| 2)` | the inner pattern matches |
| sequence | `[a, b]`, `(a, b)`, `[first, *rest]` | the subject is a sequence of the right length, and each item matches |
| mapping | `{"id": id, **rest}` | the subject is a mapping with each key, and each value matches |
| class | `Point(x, y=0)`, `str()`, `int(n)` | the subject is an instance of the class, and the arguments match |
| OR | `"y" \| "yes"` | one of the alternatives matches |
| AS | `[_, _] as pair` | the pattern on the left matches; binds the subject to the name |

The words `match`, `case` and `_` are soft keywords. They act as keywords only in this statement, so code that uses them as variable names still works.[^11]

## Semantics

**r1.** The subject expression is evaluated once. Each `case` is then tried from top to bottom. The first `case` whose pattern matches and whose guard is true runs its block, and the statement ends there. Control never falls through into the next `case`.[^1]

**r2.** When no `case` matches, the statement does nothing. It raises no exception.[^1]

**r3.** The guard is evaluated only after its pattern has matched, and after all bindings of that pattern are made. When the guard is false, matching goes on with the next `case`. Guards are evaluated in order, one at a time.[^2]

**r4.** A literal pattern compares the subject with `==`. The literals `None`, `True` and `False` compare with `is`. So `case 1:` matches `1.0` and `True`, but `case True:` does not match `1`.[^4] Example: [`p4`](#p4).

**r5.** A capture pattern is a bare name other than `_`. It always matches and binds the subject to the name. The name follows the scope rules of the assignment expression `:=`: it becomes a local variable of the closest function, unless a `global` or `nonlocal` statement says otherwise. A name may be bound only once in one pattern.[^5]

**r6.** The wildcard `_` always matches and binds nothing. Inside a pattern, `_` is a soft keyword. Outside patterns it stays a normal name.[^5]

**r7.** A value pattern is a dotted name, such as `Color.RED`. It is looked up with the normal name rules and compared with `==`. A bare name is never a value pattern: it is a capture pattern.[^6] Example: [`p1-fix`](#p1-fix).

**r8.** A group pattern `(p)` has the same meaning as `p`. A single pattern in parentheses with no comma is a group, and a single pattern in square brackets is a sequence of length one.[^6] [^7]

**r9.** A sequence pattern matches a subject that is a sequence: a `list`, a `tuple`, a `range`, a `collections.deque` or another type registered as `collections.abc.Sequence`. It never matches `str`, `bytes` or `bytearray`. Square brackets and parentheses mean the same thing. The pattern can hold one starred name, which captures the remaining items as a `list`.[^7] Example: [`commands`](#commands).

**r10.** A mapping pattern matches a subject that is a mapping, such as a `dict`. Each key of the pattern must be in the subject, and each value must match. Extra keys in the subject are ignored. A final `**rest` captures the other items in a new `dict`. Keys must be literals or value patterns. A duplicate literal key is a `SyntaxError`, and two value keys with the same value raise `ValueError` at run time.[^8] Example: [`events`](#events).

**r11.** A class pattern `C(...)` first checks `isinstance(subject, C)`. A keyword argument `attr=p` matches `p` against the attribute `attr`. A missing attribute makes the pattern fail. Positional arguments are turned into keyword arguments with the names in `C.__match_args__`, in order. More positional arguments than names raise `TypeError`.[^9] The decorator `@dataclass` sets `__match_args__` to the names of the fields of `__init__`.[^15] Example: [`shapes`](#shapes).

**r12.** For the built-in types `bool`, `bytearray`, `bytes`, `dict`, `float`, `frozendict`, `frozenset`, `int`, `list`, `set`, `str` and `tuple`, one positional argument matches the whole subject. So `int(n)` checks the type and binds the value to `n`.[^9] Example: [`classify`](#classify).

**r13.** An OR pattern `p1 | p2 | ...` tries its alternatives from left to right and stops at the first match. Each alternative must bind the same names.[^10]

**r14.** An AS pattern `p as name` matches `p`, then binds the whole subject to `name`.[^10]

**r15.** A pattern is irrefutable when its syntax alone proves that it always matches: a capture, a wildcard, or an OR, AS or group pattern that contains one in the right place. A `case` with an irrefutable pattern and no guard must be the last `case`. In an OR pattern, only the last alternative may be irrefutable. A break of these rules is a `SyntaxError`.[^3] [^10] Example: [`p1-two`](#p1-two).

**r16.** The names bound by a successful match stay bound after the statement ends. After a failed match, some names may or may not be bound. The reference leaves this to the implementation, so code must not depend on it.[^1]

## Examples

A command parser. The subject is a list of words, and each sequence pattern checks its length and its first word.

```python run id=commands
def describe(command):
    match command.split():
        case ["quit"]:
            return "stop"
        case ["go", direction]:
            return f"move {direction}"
        case ["drop", *items]:
            return f"drop {len(items)} items"
        case _:
            return "unknown command"


for line in ["quit", "go north", "drop key lamp", "dance"]:
    print(describe(line))
```

```output for=commands
stop
move north
drop 2 items
unknown command
```

Literal, class, OR and AS patterns, with a guard. The order of the cases matters: the bool case comes before the number cases, because `True == 1`.

```python run id=classify
def classify(value):
    match value:
        case None:
            return "nothing"
        case True | False:
            return "a bool"
        case 0 | 0.0:
            return "zero"
        case int(n) | float(n) if n < 0:
            return "negative"
        case str() | bytes() as text:
            return f"text of length {len(text)}"
        case [x, y] | [x, y, _]:
            return f"starts with {x}, {y}"
        case _:
            return "something else"


for value in [None, False, 0.0, -3, "hello", b"hi", [1, 2], (5, 6, 7), {1}]:
    print(repr(value), "->", classify(value))
```

```output for=classify
None -> nothing
False -> a bool
0.0 -> zero
-3 -> negative
'hello' -> text of length 5
b'hi' -> text of length 2
[1, 2] -> starts with 1, 2
(5, 6, 7) -> starts with 5, 6
{1} -> something else
```

The string `"hello"` does not reach the sequence case, and it would not match it anyway (rule r9). The tuple `(5, 6, 7)` matches `[x, y, _]`, because brackets and parentheses mean the same thing.

Mapping patterns on parsed events. Extra keys are ignored, and `**rest` collects them.

```python run id=events
def handle(event):
    match event:
        case {"type": "click", "pos": (x, y)}:
            return f"click at {x},{y}"
        case {"type": "key", "key": str(name)} if len(name) == 1:
            return f"key {name!r}"
        case {"type": "key", **rest}:
            return f"special key, other fields {sorted(rest)}"
        case {"type": other}:
            return f"ignored {other}"
        case _:
            return "not an event"


print(handle({"type": "click", "pos": (3, 4), "button": 1}))
print(handle({"type": "key", "key": "a"}))
print(handle({"type": "key", "key": "Enter", "shift": True}))
print(handle({"type": "scroll"}))
print(handle(["type", "click"]))
```

```output for=events
click at 3,4
key 'a'
special key, other fields ['key', 'shift']
ignored scroll
not an event
```

Class patterns on dataclasses. `@dataclass` sets `__match_args__`, so positional sub-patterns work, and class patterns nest.

```python run id=shapes
from dataclasses import dataclass


@dataclass
class Point:
    x: float
    y: float


@dataclass
class Circle:
    center: Point
    radius: float


@dataclass
class Rect:
    corner: Point
    width: float
    height: float


def where(shape):
    match shape:
        case Circle(Point(0, 0), _) | Rect(Point(0, 0), _, _):
            return "at the origin"
        case Circle(Point(x, y), r) if r > 10:
            return f"large circle at ({x}, {y})"
        case Circle() | Rect():
            return "somewhere else"


print(Point.__match_args__)
print(where(Circle(Point(0, 0), 1)))
print(where(Circle(Point(2, 5), 12)))
print(where(Rect(Point(4, 4), 1, 1)))
```

```output for=shapes
('x', 'y')
at the origin
large circle at (2, 5)
somewhere else
```

## Versions

> [!since v="3.10"]
> The `match` statement, with every pattern form on this page, is in CPython 3.10 and later. On 3.9 and earlier each example above is a `SyntaxError`.[^16]

In 3.15 a literal pattern may start with a unary plus, as in `case +1:`, to mirror `case -1:`. On 3.14 and earlier this is a `SyntaxError` ("invalid syntax").[^17]

```python run id=unary-plus since=3.15
def sign(delta):
    match delta:
        case +1:
            return "up"
        case -1:
            return "down"
        case 0:
            return "flat"


print(sign(1), sign(-1), sign(0))
```

```output for=unary-plus
up down flat
```

The list of built-in types in rule r12 gained `frozendict` in 3.15, when the type itself was added.[^9]

## Pitfalls

### A bare name captures; it does not compare

A bare name in a pattern is a capture pattern (rule r5). It matches any value and rebinds the name. Here `case RED:` matches `"blue"`, and it also sets `RED` to `"blue"` inside the function.

```python wrong id=p1 lint="F841"
RED = "red"


def is_red(color):
    match color:
        case RED:
            return True
    return False


print(is_red("blue"))
```

```output for=p1
True
```

With a second `case` after it, the compiler sees the problem and refuses the code. The message is "name capture 'RED' makes remaining patterns unreachable".

```python fails id=p1-two expect=exception:SyntaxError lint="F841"
RED = "red"
GREEN = "green"


def action(color):
    match color:
        case RED:
            return "stop"
        case GREEN:
            return "go"
```

The fix is a dotted name, which is a value pattern (rule r7). An `Enum` gives the dotted names.

```python run id=p1-fix
from enum import Enum


class Color(Enum):
    RED = "red"
    GREEN = "green"


def action(color):
    match color:
        case Color.RED:
            return "stop"
        case Color.GREEN:
            return "go"
        case _:
            return "unknown"


print(action(Color.GREEN))
print(action("red"))
```

```output for=p1-fix
go
unknown
```

The plain string `"red"` does not match `Color.RED`, because an `Enum` member is not equal to its value.

### The order of the cases

The first matching `case` wins (rule r1). A general pattern placed before a specific one hides it. The compiler does not warn, because `(x, y)` is not irrefutable: it fails on values that are not pairs.

```python wrong id=p2
def kind(point):
    match point:
        case (x, y):
            return f"the point {x}, {y}"
        case (0, 0):
            return "the origin"


print(kind((0, 0)))
```

```output for=p2
the point 0, 0
```

Put the specific case first: `case (0, 0):`, then `case (x, y):`.

### `case str:` is not `case str():`

`case str:` is a capture pattern named `str`. It matches every value and hides the built-in `str` in that function. `case str():` is a class pattern, and it checks the type.

```python wrong id=p3 lint="F841"
def is_text(value):
    match value:
        case str:
            return True
    return False


print(is_text(42))
```

```output for=p3
True
```

```python run id=p3-fix
def is_text(value):
    match value:
        case str():
            return True
    return False


print(is_text(42))
print(is_text("42"))
```

```output for=p3-fix
False
True
```

### Numbers compare with `==`

A number literal uses `==` (rule r4). So `case 1:` matches `True` and `1.0`. To accept only an `int`, write `case int(1):`, and put a `bool` case before it, because `bool` is a subclass of `int`.

```python wrong id=p4
def describe(value):
    match value:
        case 1:
            return "the number one"
        case _:
            return "something else"


print(describe(True))
print(describe(1.0))
```

```output for=p4
the number one
the number one
```

### `{}` matches every mapping

Extra keys are ignored (rule r10), so the empty mapping pattern `{}` matches any mapping, not only an empty one. Test for an empty dict with a guard, as in `case dict() if not config:`, or with `case {} if not config:`.

```python wrong id=p5
def is_empty(config):
    match config:
        case {}:
            return True
    return False


print(is_empty({"debug": True}))
```

```output for=p5
True
```

## Comparison

**Rust** has `match` as an expression that gives a value. The compiler checks that the arms cover every value of the type, and a missing case is error E0004.[^21] [^22] Arms are tried in order, the first match wins, and a guard is written `if cond`. Python checks none of this at compile time: a `match` with no matching `case` does nothing.

**Scala** has `expr match { case p => ... }`, also an expression. When no case matches, it throws `scala.MatchError`. For a `sealed` class hierarchy the compiler can warn that the cases are not exhaustive.[^23] Python's class patterns with `__match_args__` play the role of Scala's extractor patterns (`unapply`).

**C#** has the `switch` expression, `value switch { pattern => result, ... }`, with type, property, positional, relational and list patterns. A guard is written `when cond`. The compiler warns (CS8509) when the arms may not cover every input, and at run time an unmatched value throws `SwitchExpressionException`.[^24]

**Java** has pattern matching for `switch` from Java 21 (JEP 441), with type patterns, record patterns and guards written `when cond`. A `switch` with patterns must be exhaustive, so a missing case over a sealed interface is a compile error.[^25] Arrow labels (`->`) do not fall through.

Python is the only one of the five where the statement is not an expression and has no exhaustiveness check. In exchange it can match on any object at run time, including dicts, with no declared type.

## Implementation notes

CPython compiles each `match` statement to plain bytecode, with no jump table. The instructions `MATCH_SEQUENCE`, `MATCH_MAPPING`, `MATCH_KEYS` and `MATCH_CLASS` do the type tests and the lookups. The patterns are then tested one after the other, as rule r1 says.[^18]

| Implementation | Support | Checked |
|---|---|---|
| CPython | from 3.10 | the examples ran on 3.10, 3.14.8 and 3.15.2, and failed on 3.9 |
| PyPy | from v7.3.12, the first release of PyPy3.10[^19] | `commands` ran on PyPy 7.3.19 (Python 3.10.16) and PyPy 8.0.0 (Python 3.12.14) |
| GraalPy | yes; the first GraalPy release with it was not checked | `commands` ran on GraalPy 24.0.2 (Python 3.10.13) and 25.4.4 (Python 3.13.14) |
| MicroPython | no | `commands` is a `SyntaxError` on MicroPython 1.29.0, which implements the Python 3.4 syntax with some later features[^20] |

## References
