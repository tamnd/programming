---
tag: S1J4Q0
kind: feature
owner: python
slug: mapping-patterns
name: Mapping patterns
aliases: [mapping pattern, dict pattern, match a dictionary, double star pattern, match JSON with match, case with rest of keys]
area: languages/python/control-flow
level: core
summary: >-
  A mapping pattern such as {"type": "click", "x": x} matches a dict or another mapping that has each listed key, and it matches each value against a subpattern. Extra keys are ignored.
grammar: "mapping_pattern: '{' [items_pattern] '}'; key_value_pattern: (literal_pattern | value_pattern) ':' pattern | double_star_pattern; double_star_pattern: '**' capture_pattern (Language Reference, 8.6.4.9)"
spec: {standard: null, url: "https://docs.python.org/3.15/reference/compound_stmts.html#mapping-patterns", locator: "8.6.4.9. Mapping Patterns"}
see_also: [python/match-statement, python/sequence-patterns, python/class-patterns, python/literal-capture-and-wildcard-patterns]
sources:
  - url: https://docs.python.org/3.15/reference/compound_stmts.html#mapping-patterns
    title: The Python Language Reference, Python 3.15
    locator: 8.6.4.9. Mapping Patterns
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/tutorial/controlflow.html#match-statements
    title: The Python Tutorial, 4. More Control Flow Tools
    locator: 4.7. match Statements
    accessed: 2026-11-16
  - url: https://go.dev/ref/spec#Type_switches
    title: The Go Programming Language Specification
    locator: Type switches; Index expressions
    accessed: 2026-11-16
---

## Synopsis

```python run id=events
def handle(event):
    match event:
        case {"type": "click", "pos": (x, y)}:
            return f"click at {x},{y}"
        case {"type": "key", "key": key}:
            return f"key {key}"
        case _:
            return "unknown event"

print(handle({"type": "click", "pos": (3, 4), "button": 1}))
print(handle({"type": "key", "key": "q"}))
print(handle({"type": "key"}))
```

```output for=events
click at 3,4
key q
unknown event
```

The first event has an extra key, `"button"`, and it still matches. The third event has no `"key"` key, so it does not match the second case.

## Semantics

**r1.** "A mapping pattern contains one or more key-value patterns. The syntax is similar to the construction of a dictionary."[^1]

**r2.** "If every key given in the mapping pattern is present in the subject mapping, and the pattern for each key matches the corresponding item of the subject mapping, the mapping pattern succeeds."[^1] "Unlike sequence patterns, extra keys are ignored."[^2] Example: [`events`](#events).

**r3.** A key must be a literal pattern or a value pattern, and the value can be any pattern.[^1] Example: [`dotted-key`](#dotted-key).

**r4.** "At most one double star pattern may be in a mapping pattern. The double star pattern must be the last subpattern in the mapping pattern."[^1] The tutorial says: "An unpacking like **rest is also supported. (But **_ would be redundant, so it is not allowed.)"[^2] Example: [`rest`](#rest).

**r5.** "Duplicate keys in mapping patterns are disallowed. Duplicate literal keys will raise a SyntaxError. Two keys that otherwise have the same value will raise a ValueError at runtime."[^1] Example: [`duplicate-key`](#duplicate-key).

**r6.** "Key-value pairs are matched using the two-argument form of the mapping subject’s get() method. Matched key-value pairs must already be present in the mapping, and not created on-the-fly via __missing__() or __getitem__()."[^1] Example: [`defaultdict`](#defaultdict).

## Examples

```python run id=rest
match {"name": "ann", "age": 30, "city": "Oslo"}:
    case {"name": name, **rest}:
        print(name, rest)
```

```output for=rest
ann {'age': 30, 'city': 'Oslo'}
```

```python run id=dotted-key
class Keys:
    NAME = "name"

match {"name": "bob"}:
    case {Keys.NAME: str(value)}:
        print("name is", value)
```

```output for=dotted-key
name is bob
```

```python run id=defaultdict
from collections import defaultdict

counts = defaultdict(int)
match counts:
    case {"missing": n}:
        print("matched", n)
    case _:
        print("no match")
print(dict(counts))
```

```output for=defaultdict
no match
{}
```

A `defaultdict` makes a missing key on `counts["missing"]`, but the pattern uses `get()`, so it does not make the key (rule r6).

## Pitfalls

An empty mapping pattern `{}` matches each mapping, not only an empty one:

```python wrong id=empty-pattern
match {"a": 1}:
    case {}:
        print("empty?")
```

```output for=empty-pattern
empty?
```

The pattern lists no keys, so each key that it lists is present (rule r2). To match only an empty dict, add a guard, as in `case {} if len(subject) == 0`, where `subject` is the value that you match.

Two equal literal keys are a syntax error:

```python fails id=duplicate-key expect=exception:SyntaxError
match {"a": 1}:
    case {"a": x, "a": y}:
        pass
```

`**_` is not allowed:

```python fails id=double-star-underscore expect=exception:SyntaxError
match {"a": 1}:
    case {"a": 1, **_}:
        pass
```

## Comparison

Go has no mapping patterns. A Go switch can compare the dynamic type of an interface value: "A type switch compares types rather than values."[^3] To test a key of a Go map, a program reads it with the comma-ok form. In Go, an index expression on a map in the special form `v, ok = a[x]` "yields an additional untyped boolean value. The value of ok is true if the key x is present in the map, and false otherwise."[^3] A Python mapping pattern does this test for each key in one case (rule r2).
