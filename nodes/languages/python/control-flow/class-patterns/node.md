---
tag: R74XYY
kind: feature
owner: python
slug: class-patterns
name: Class patterns
aliases: [class pattern, match on type, __match_args__, match a dataclass, isinstance in match, keyword pattern, positional pattern]
area: languages/python/control-flow
level: advanced
summary: >-
  A class pattern such as Point(x=0, y=y) matches an instance of the class and matches its attributes against subpatterns. Positional subpatterns use the __match_args__ attribute of the class.
grammar: "class_pattern: name_or_attr '(' [pattern_arguments ','?] ')'; keyword_pattern: NAME '=' pattern (Language Reference, 8.6.4.10)"
spec: {standard: null, url: "https://docs.python.org/3.15/reference/compound_stmts.html#class-patterns", locator: "8.6.4.10. Class Patterns"}
see_also: [python/match-statement, python/mapping-patterns, python/sequence-patterns, python/match-guards]
sources:
  - url: https://docs.python.org/3.15/reference/compound_stmts.html#class-patterns
    title: The Python Language Reference, Python 3.15
    locator: 8.6.4.10. Class Patterns
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/reference/datamodel.html#customizing-positional-arguments-in-class-pattern-matching
    title: The Python Language Reference, Python 3.15
    locator: 3.3.10. Customizing positional arguments in class pattern matching
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/tutorial/controlflow.html#match-statements
    title: The Python Tutorial, 4. More Control Flow Tools
    locator: 4.7. match Statements
    accessed: 2026-11-16
  - url: https://go.dev/ref/spec#Type_switches
    title: The Go Programming Language Specification
    locator: Type switches
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/builtins/stdtypes.html#boolean-type-bool
    title: Built-in Types, Python 3.15 documentation
    locator: Boolean Type - bool
    accessed: 2026-11-16
---

## Synopsis

```python run id=where-is
class Point:
    def __init__(self, x, y):
        self.x = x
        self.y = y

def where_is(point):
    match point:
        case Point(x=0, y=0):
            return "Origin"
        case Point(x=0, y=y):
            return f"Y={y}"
        case Point(x=x, y=0):
            return f"X={x}"
        case Point():
            return "Somewhere else"
        case _:
            return "Not a point"

print(where_is(Point(0, 0)), where_is(Point(0, 4)), where_is(Point(2, 3)), where_is((0, 0)))
```

```output for=where-is
Origin Y=4 Somewhere else Not a point
```

This is the example of the tutorial. `Point()` matches each `Point`, and the tuple `(0, 0)` is not a `Point`.

## Semantics

**r1.** "If the subject value is not an instance of name_or_attr (tested via isinstance()), the class pattern fails. If no pattern arguments are present, the pattern succeeds."[^1] Example: [`where-is`](#where-is).

**r2.** For each keyword pattern, "The keyword is looked up as an attribute on the subject." "If this raises AttributeError, the class pattern has failed." Else the subpattern is matched "against the subject’s attribute value".[^1]

**r3.** "positional arguments in the pattern are not allowed by default". To allow them, "the class needs to define a __match_args__ attribute."[^2] "each positional argument will be converted into a keyword argument, using the corresponding value in __match_args__ as the keyword."[^2] Example: [`match-args`](#match-args).

**r4.** "If there are more positional patterns than len(cls.__match_args__), TypeError is raised."[^1] Example: [`too-many`](#too-many).

**r5.** For some built-in types, including bool, bytes, dict, float, int, list, str and tuple, a class pattern accepts "a single positional argument, and the pattern there is matched against the whole object rather than an attribute. For example int(0|1) matches the value 0, but not the value 0.0."[^1] Example: [`builtin-types`](#builtin-types).

**r6.** "Only the standalone names (like var above) are assigned to by a match statement. Dotted names (like foo.bar), attribute names (the x= and y= above) or class names (recognized by the “(…)” next to them like Point above) are never assigned to."[^3]

## Examples

```python run id=match-args
from dataclasses import dataclass

@dataclass
class Point:
    x: int
    y: int

def describe(points):
    match points:
        case []:
            return "No points"
        case [Point(0, 0)]:
            return "The origin"
        case [Point(x, y)]:
            return f"Single point {x}, {y}"
        case [Point(0, y1), Point(0, y2)]:
            return f"Two on the Y axis at {y1}, {y2}"
        case _:
            return "Something else"

print(Point.__match_args__)
print(describe([Point(0, 0)]))
print(describe([Point(0, 1), Point(0, 5)]))
```

```output for=match-args
('x', 'y')
The origin
Two on the Y axis at 1, 5
```

The tutorial says: "You can use positional parameters with some builtin classes that provide an ordering for their attributes (e.g. dataclasses)."[^3] The dataclass has `__match_args__`, so the positional pattern `Point(x, y)` works (rule r3).

```python run id=builtin-types
def kind(value):
    match value:
        case bool(b):
            return f"bool {b}"
        case int(0 | 1):
            return "int 0 or 1"
        case int(n):
            return f"int {n}"
        case str() | bytes():
            return "text"
        case _:
            return "other"

print(kind(True), kind(1), kind(7), kind(1.0), kind("a"))
```

```output for=builtin-types
bool True int 0 or 1 int 7 other text
```

The documentation says: "bool is a subclass of int".[^5] So `int(n)` also matches `True`, and the `bool` case must come first. `1.0` is not an `int`, so `int(0 | 1)` does not match it (rule r5).

## Pitfalls

A plain class with no `__match_args__` takes no positional subpatterns:

```python fails id=too-many expect=exception:TypeError
class Plain:
    def __init__(self, x):
        self.x = x

match Plain(1):
    case Plain(1):
        print("one")
```

Use a keyword pattern, `Plain(x=1)`, or set `__match_args__ = ("x",)` in the class.

`case Point:` with no parentheses is a capture pattern. It binds the name `Point` and matches each value. Write `case Point():` to test the type.

## Comparison

A Go type switch tests the dynamic type of an interface value: "A type switch compares types rather than values."[^4] "Cases then match actual types T against the dynamic type of the expression x."[^4] A Go case can test the type, but it cannot test the fields in the same case. A Python class pattern tests the type with isinstance() and then tests attributes (rules r1 and r2).
