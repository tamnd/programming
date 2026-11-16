---
tag: 27YVT5
kind: feature
owner: python
slug: or-as-and-group-patterns
name: OR, AS and group patterns
aliases: [or pattern, as pattern, group pattern, case with vertical bar, case a or b, bind a subpattern with as, parentheses in patterns]
area: languages/python/control-flow
level: core
summary: >-
  P1 | P2 matches when one of its alternatives matches. P as name matches P and binds the whole subject to name. A group pattern, (P), only adds parentheses to make the grouping clear.
grammar: "or_pattern: '|'.closed_pattern+; as_pattern: or_pattern 'as' capture_pattern; group_pattern: '(' pattern ')' (Language Reference, 8.6.4.1, 8.6.4.2 and 8.6.4.7)"
spec: {standard: null, url: "https://docs.python.org/3.15/reference/compound_stmts.html#or-patterns", locator: "8.6.4.1. OR Patterns; 8.6.4.2. AS Patterns; 8.6.4.7. Group Patterns"}
see_also: [python/match-statement, python/literal-capture-and-wildcard-patterns, python/sequence-patterns, python/match-guards]
sources:
  - url: https://docs.python.org/3.15/reference/compound_stmts.html#or-patterns
    title: The Python Language Reference, Python 3.15
    locator: 8.6.4.1. OR Patterns; 8.6.4.2. AS Patterns; 8.6.4.7. Group Patterns
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

```python run id=or-as
def status(code):
    match code:
        case 200 | 201 | 204:
            return "ok"
        case (401 | 403 | 404) as err:
            return f"not allowed ({err})"
        case _:
            return "other"

print(status(201), status(403), status(500))
```

```output for=or-as
ok not allowed (403) other
```

`|` joins alternatives. `as err` binds the subject to `err` when the OR pattern matches.

## Semantics

**r1.** "An OR pattern matches each of its subpatterns in turn to the subject value, until one succeeds. The OR pattern is then considered successful. Otherwise, if none of the subpatterns succeed, the OR pattern fails."[^1] Example: [`or-as`](#or-as).

**r2.** In an OR pattern, "Only the final subpattern may be irrefutable, and each subpattern must bind the same set of names to avoid ambiguity."[^1] Examples: [`same-names`](#same-names), [`different-names`](#different-names).

**r3.** "If the OR pattern fails, the AS pattern fails. Otherwise, the AS pattern binds the subject to the name on the right of the as keyword and succeeds." Also, the "capture_pattern cannot be a _."[^1] Example: [`as-subpattern`](#as-subpattern).

**r4.** "A group pattern allows users to add parentheses around patterns to emphasize the intended grouping." "In simple terms (P) has the same effect as P."[^1]

**r5.** The tutorial shows `as` on a subpattern: `case (Point(x1, y1), Point(x2, y2) as p2): ...` "will capture the second element of the input as p2 (as long as the input is a sequence of two points)".[^2] Example: [`as-subpattern`](#as-subpattern).

## Examples

```python run id=as-subpattern
def move(command):
    match command:
        case ["go", ("north" | "south" | "east" | "west") as direction]:
            return f"going {direction}"
        case ["go", other]:
            return f"cannot go {other}"

print(move(["go", "east"]))
print(move(["go", "up"]))
```

```output for=as-subpattern
going east
cannot go up
```

```python run id=same-names
def first_or_only(value):
    match value:
        case [x, *_] | x:
            return x

print(first_or_only([7, 8]), first_or_only(9))
```

```output for=same-names
7 9
```

Both alternatives bind `x`, and only the last one is irrefutable (rule r2).

## Pitfalls

The alternatives of an OR pattern must bind the same names:

```python fails id=different-names expect=exception:SyntaxError
match (1, 2):
    case (x, 0) | (0, y):
        print(x)
```

Bind the same names in each alternative, or use two case blocks.

In `case 401 | 403 as err`, the `as` applies to the whole OR pattern, because the grammar is `or_pattern 'as' capture_pattern`.[^1] Parentheses, as in the synopsis, make this clear (rule r4).

`|` in a pattern is not the `or` operator. `case 1 or 2:` is a syntax error:

```python fails id=or-keyword expect=exception:SyntaxError
match 2:
    case 1 or 2:
        print("one or two")
```

## Comparison

A Go case can list several expressions. In an expression switch, "the case expressions, which need not be constants, are evaluated left-to-right and top-to-bottom; the first one that equals the switch expression triggers execution of the statements of the associated case".[^3] So `case 200, 201, 204:` in Go is like `case 200 | 201 | 204:` in Python. A Go expression switch has no AS form, because its cases are expressions that Go compares with the switch expression, and they bind no names.
