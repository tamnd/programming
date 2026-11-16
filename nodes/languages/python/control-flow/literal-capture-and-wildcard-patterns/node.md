---
tag: 67V0YV
kind: feature
owner: python
slug: literal-capture-and-wildcard-patterns
name: Literal, capture and wildcard patterns
aliases: [literal pattern, capture pattern, wildcard pattern, value pattern, case underscore, match a constant in a case, dotted name pattern]
area: languages/python/control-flow
level: core
summary: >-
  A literal pattern matches an equal value. A capture pattern, a bare name, matches anything and binds it. The wildcard _ matches anything and binds nothing. A dotted name is a value pattern.
grammar: "literal_pattern: signed_number | signed_number '+' NUMBER | signed_number '-' NUMBER | strings | 'None' | 'True' | 'False'; capture_pattern: !'_' NAME; wildcard_pattern: '_'; value_pattern: attr (Language Reference, 8.6.4.3 to 8.6.4.6)"
spec: {standard: null, url: "https://docs.python.org/3.15/reference/compound_stmts.html#literal-patterns", locator: "8.6.4.3. Literal Patterns to 8.6.4.6. Value Patterns"}
see_also: [python/match-statement, python/or-as-and-group-patterns, python/match-guards, python/sequence-patterns]
sources:
  - url: https://docs.python.org/3.15/reference/compound_stmts.html#literal-patterns
    title: The Python Language Reference, Python 3.15
    locator: 8.6.4.3. Literal Patterns; 8.6.4.4. Capture Patterns; 8.6.4.5. Wildcard Patterns; 8.6.4.6. Value Patterns
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/tutorial/controlflow.html#match-statements
    title: The Python Tutorial, 4. More Control Flow Tools
    locator: 4.7. match Statements
    accessed: 2026-11-16
  - url: https://go.dev/ref/spec#Expression_switches
    title: The Go Programming Language Specification
    locator: Expression switches
    accessed: 2026-11-16
---

## Synopsis

```python run id=http-error
def http_error(status):
    match status:
        case 400:
            return "Bad request"
        case 404:
            return "Not found"
        case 418:
            return "I'm a teapot"
        case _:
            return "Something's wrong with the internet"

print(http_error(404))
print(http_error(500))
```

```output for=http-error
Not found
Something's wrong with the internet
```

This is the example of the tutorial. `400`, `404` and `418` are literal patterns, and `_` is the wildcard.

## Semantics

**r1.** Literal patterns: "In simple terms, LITERAL will succeed only if <subject> == LITERAL. For the singletons None, True and False, the is operator is used."[^1] Example: [`singletons`](#singletons).

**r2.** "Triple-quoted strings are supported. Raw strings and byte strings are supported. f-strings and t-strings are not supported."[^1]

**r3.** "A capture pattern binds the subject value to a name." "Capture patterns always succeed."[^1] The name "becomes a local variable in the closest containing function scope unless there’s an applicable global or nonlocal statement."[^1] Example: [`capture`](#capture).

**r4.** "In a given pattern, a given name can only be bound once."[^1] Example: [`same-name`](#same-name).

**r5.** "A wildcard pattern always succeeds (matches anything) and binds no name." "_ is a soft keyword within any pattern, but only within patterns."[^1] Example: [`http-error`](#http-error).

**r6.** A value pattern is a dotted name. "The dotted name in the pattern is looked up using standard Python name resolution rules. The pattern succeeds if the value found compares equal to the subject value (using the == equality operator)."[^1] Named constants "must be dotted names to prevent them from being interpreted as capture variables".[^2] Example: [`dotted`](#dotted).

## Examples

```python run id=singletons
def kind(value):
    match value:
        case True:
            return "True"
        case 1:
            return "one"
        case None:
            return "None"
        case _:
            return "other"

print(kind(True), kind(1), kind(1.0), kind(None), kind(0))
```

```output for=singletons
True one one None other
```

`case True` uses `is`, so `1` does not match it. `case 1` uses `==`, so `1.0` matches it (rule r1).

```python run id=capture
def describe(command):
    match command:
        case "quit":
            return "bye"
        case other:
            return f"unknown command {other!r}"

print(describe("quit"))
print(describe("jump"))
```

```output for=capture
bye
unknown command 'jump'
```

```python run id=dotted
from enum import Enum

class Color(Enum):
    RED = "red"
    GREEN = "green"

def show(color):
    match color:
        case Color.RED:
            return "I see red!"
        case Color.GREEN:
            return "Grass is green"

print(show(Color("green")))
```

```output for=dotted
Grass is green
```

## Pitfalls

A bare name is a capture pattern, not a comparison with a variable:

```python fails id=bare-name expect=exception:SyntaxError
RED = "red"

match "blue":
    case RED:
        print("red")
    case _:
        print("other")
```

`case RED` captures each value, so it is irrefutable. The reference says that "A match statement may have at most one irrefutable case block, and it must be last."[^1] So Python rejects the program. Use a dotted name, such as `Color.RED`, or a guard such as `case c if c == RED` (rule r6).

A name cannot be bound twice in one pattern:

```python fails id=same-name expect=exception:SyntaxError
match (1, 1):
    case (x, x):
        print("same")
```

Use `case (x, y) if x == y` to test that two items are equal.

## Comparison

A Go expression switch compares values: "the first one that equals the switch expression triggers execution of the statements of the associated case; the other cases are skipped."[^3] The default case of Go is like the wildcard `_` (rule r5). In Go, "the case expressions, which need not be constants", are evaluated, so a Go case can name a variable to compare with it.[^3] In Python, a bare name in a case captures the value, and only a dotted name compares (rule r6).
