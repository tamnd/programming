---
tag: JAPK28
kind: feature
owner: python
slug: match-guards
name: Match guards and irrefutable cases
aliases: [match guard, case if, guard clause in match, irrefutable pattern, catch-all case, case _ must be last]
area: languages/python/control-flow
level: core
summary: >-
  A guard, case P if condition, selects the case only when the pattern matches and the condition is true. An irrefutable case, such as case _ or case x, matches everything and must be the last case.
grammar: "case_block: 'case' patterns [guard] ':' suite; guard: 'if' assignment_expression (Language Reference, 8.6 and 8.6.2)"
spec: {standard: null, url: "https://docs.python.org/3.15/reference/compound_stmts.html#guards", locator: "8.6.2. Guards; 8.6.3. Irrefutable Case Blocks"}
see_also: [python/match-statement, python/literal-capture-and-wildcard-patterns, python/or-as-and-group-patterns, python/class-patterns]
sources:
  - url: https://docs.python.org/3.15/reference/compound_stmts.html#guards
    title: The Python Language Reference, Python 3.15
    locator: 8.6.2. Guards; 8.6.3. Irrefutable Case Blocks
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

```python run id=diagonal
def place(point):
    match point:
        case (x, y) if x == y:
            return f"Y=X at {x}"
        case (x, y):
            return "Not on the diagonal"

print(place((2, 2)), "/", place((1, 3)))
```

```output for=diagonal
Y=X at 2 / Not on the diagonal
```

The first case needs both a matching pattern and a true guard. When the guard is false, Python tries the next case.

## Semantics

**r1.** "A guard (which is part of the case) must succeed for code inside the case block to execute. It takes the form: if followed by an expression."[^1]

**r2.** The order is: "Check that the pattern in the case block succeeded. If the pattern failed, the guard is not evaluated and the next case block is checked." "If the pattern succeeded, evaluate the guard."[^1] Example: [`order`](#order).

**r3.** "If the guard raises an exception during evaluation, the exception bubbles up."[^1] Example: [`guard-error`](#guard-error).

**r4.** "Guard evaluation must proceed from the first to the last case block, one at a time, skipping case blocks whose pattern(s) don’t all succeed." "Guard evaluation must stop once a case block is selected."[^1]

**r5.** The tutorial notes "that value capture happens before the guard is evaluated".[^2] So a guard can use the names of its pattern. Example: [`diagonal`](#diagonal).

**r6.** "An irrefutable case block is a match-all case block. A match statement may have at most one irrefutable case block, and it must be last."[^1] "A case block is considered irrefutable if it has no guard and its pattern is irrefutable."[^1] Capture patterns and wildcard patterns are irrefutable.[^1] Example: [`not-last`](#not-last).

## Examples

```python run id=order
def check(label, result):
    print("guard", label)
    return result

match [1, 2]:
    case [a] if check("one item", True):
        print("one")
    case [a, b] if check("two items", False):
        print("two, first guard")
    case [a, b] if check("two items again", True):
        print("two, second guard")
```

```output for=order
guard two items
guard two items again
two, second guard
```

The first pattern fails, so Python does not evaluate its guard (rule r2).

```python run id=guarded-catch-all
def sign(n):
    match n:
        case x if x < 0:
            return "negative"
        case 0:
            return "zero"
        case _:
            return "positive"

print(sign(-3), sign(0), sign(5))
```

```output for=guarded-catch-all
negative zero positive
```

`case x if x < 0` has a guard, so it is not irrefutable, and other cases can follow it (rule r6).

```python fails id=guard-error expect=exception:ZeroDivisionError
match (1, 0):
    case (a, b) if a / b > 1:
        print("big")
```

## Pitfalls

A catch-all case that is not the last case is a syntax error:

```python fails id=not-last expect=exception:SyntaxError
match 3:
    case _:
        print("anything")
    case 3:
        print("three")
```

A capture pattern binds its name even when the guard is false, so the name can have a value after a failed case:

```python wrong id=bound-after-guard
match 5:
    case n if n > 10:
        print("big")
    case _:
        print("small, n =", n)
```

```output for=bound-after-guard
small, n = 5
```

The tutorial says that value capture happens before the guard (rule r5). The reference adds: "Do not rely on bindings being made for a failed match. Conversely, do not rely on variables remaining unchanged after a failed match."[^1] So do not use names from a case that was not selected.

## Comparison

A Go expression switch with no switch expression works as a list of guards: "A missing switch expression is equivalent to the boolean value true."[^3] So `switch { case x < 0: ... }` selects the first case whose expression is true. Go has no patterns, so a Go case cannot bind names and test them in one step, as `case (x, y) if x == y` does in Python. In Go, "There can be at most one default case and it may appear anywhere" in the switch statement.[^3] A Python catch-all case must be last (rule r6).
