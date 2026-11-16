---
tag: EG6M8K
kind: feature
owner: python
slug: sequence-patterns
name: Sequence patterns
aliases: [sequence pattern, list pattern, tuple pattern, star pattern in match, case with rest, match a list]
area: languages/python/control-flow
level: core
summary: >-
  A sequence pattern such as [x, y] or (x, *rest) matches a list, a tuple or another sequence of the right length, and matches each item against a subpattern. It never matches a string or an iterator.
grammar: "sequence_pattern: '[' [maybe_sequence_pattern] ']' | '(' [open_sequence_pattern] ')'; star_pattern: '*' (capture_pattern | wildcard_pattern) (Language Reference, 8.6.4.8)"
spec: {standard: null, url: "https://docs.python.org/3.15/reference/compound_stmts.html#sequence-patterns", locator: "8.6.4.8. Sequence Patterns"}
see_also: [python/match-statement, python/iterable-unpacking, python/mapping-patterns, python/literal-capture-and-wildcard-patterns]
sources:
  - url: https://docs.python.org/3.15/reference/compound_stmts.html#sequence-patterns
    title: The Python Language Reference, Python 3.15
    locator: 8.6.4.8. Sequence Patterns
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/tutorial/controlflow.html#match-statements
    title: The Python Tutorial, 4. More Control Flow Tools
    locator: 4.7. match Statements
    accessed: 2026-11-16
  - url: https://go.dev/ref/spec#Switch_statements
    title: The Go Programming Language Specification
    locator: Switch statements
    accessed: 2026-11-16
---

## Synopsis

```python run id=point
def where(point):
    match point:
        case (0, 0):
            return "Origin"
        case (0, y):
            return f"Y={y}"
        case (x, 0):
            return f"X={x}"
        case (x, y):
            return f"X={x}, Y={y}"
        case _:
            return "Not a point"

print(where((0, 0)), where((0, 5)), where([3, 0]), where((1, 2)), where((1, 2, 3)))
```

```output for=point
Origin Y=5 X=3 X=1, Y=2 Not a point
```

This is the example of the tutorial. `(x, y)` matches each sequence of two items, also the list `[3, 0]`.

## Semantics

**r1.** "A sequence pattern contains several subpatterns to be matched against sequence elements. The syntax is similar to the unpacking of a list or tuple."[^1]

**r2.** "There is no difference if parentheses or square brackets are used for sequence patterns".[^1] The tutorial says that tuple and list patterns "actually match arbitrary sequences. An important exception is that they don’t match iterators or strings."[^2] Examples: [`point`](#point), [`not-a-string`](#not-a-string).

**r3.** "If the subject value is an instance of str, bytes or bytearray the sequence pattern fails."[^1]

**r4.** "At most one star subpattern may be in a sequence pattern. The star subpattern may occur in any position."[^1] It "matches a list formed of the remaining subject items".[^1] The name after `*` "may also be _, so (x, y, *_) matches a sequence of at least two items without binding the remaining items."[^2] Example: [`star`](#star).

**r5.** For a pattern with no star, "If the length of the subject sequence is not equal to the number of subpatterns, the sequence pattern fails".[^1]

**r6.** "A single pattern enclosed in parentheses without a trailing comma (e.g. (3 | 4)) is a group pattern. While a single pattern enclosed in square brackets (e.g. [3 | 4]) is still a sequence pattern."[^1] Example: [`one-item`](#one-item).

## Examples

```python run id=star
def parts(seq):
    match seq:
        case []:
            return "empty"
        case [first]:
            return f"one: {first}"
        case [first, *middle, last]:
            return f"first={first} middle={middle} last={last}"

print(parts([]))
print(parts([7]))
print(parts((1, 2, 3, 4)))
```

```output for=star
empty
one: 7
first=1 middle=[2, 3] last=4
```

The star subpattern gives a list, also when the subject is a tuple (rule r4).

```python run id=nested
def command(words):
    match words:
        case ["go", ("north" | "south") as direction]:
            return f"going {direction}"
        case ["drop", *objects]:
            return f"dropping {len(objects)} objects"
        case _:
            return "unknown"

print(command("go north".split()))
print(command(["drop", "key", "lamp"]))
```

```output for=nested
going north
dropping 2 objects
```

```python run id=one-item
for subject in [3, [3]]:
    match subject:
        case [3 | 4]:
            print(subject, "is a list with 3 or 4")
        case (3 | 4):
            print(subject, "is 3 or 4")
```

```output for=one-item
3 is 3 or 4
[3] is a list with 3 or 4
```

## Pitfalls

A string is a sequence, but a sequence pattern does not match it:

```python wrong id=not-a-string
match "ab":
    case [a, b]:
        print("two items", a, b)
    case _:
        print("no match")
```

```output for=not-a-string
no match
```

To split a string into characters, match `list(text)` instead (rule r3).

Two star subpatterns in one sequence pattern are a syntax error:

```python fails id=two-stars expect=exception:SyntaxError
match [1, 2, 3]:
    case [*a, *b]:
        pass
```

## Comparison

A Go switch has two forms: "In an expression switch, the cases contain expressions that are compared against the value of the switch expression. In a type switch, the cases contain types that are compared against the type of a specially annotated switch expression."[^3] A Go case cannot look into a slice or bind its items to names. Python sequence patterns test the length and the items, and they bind names in one step (rules r1 and r5).
