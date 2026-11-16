---
tag: T2VHG5
kind: feature
owner: python
slug: if-statements
name: If Statements
aliases: [if statement, if elif else, elif, conditional statement in Python, else if in Python, switch in Python]
area: languages/python/control-flow
level: intro
summary: >-
  An if statement tests its conditions one by one and runs the block of the first true condition. It can have zero or more elif parts and one optional else part, which runs when no condition is true.
grammar: "if_stmt: 'if' assignment_expression ':' suite ('elif' assignment_expression ':' suite)* ['else' ':' suite] (Language Reference, 8.1)"
spec: {standard: null, url: "https://docs.python.org/3.15/reference/compound_stmts.html#the-if-statement", locator: "8.1. The if statement"}
see_also: [python/conditions, python/match-statement, python/while-statement, python/more-control-flow-tools]
sources:
  - url: https://docs.python.org/3.15/tutorial/controlflow.html#if-statements
    title: The Python Tutorial, 4. More Control Flow Tools
    locator: 4.1. if Statements
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/reference/compound_stmts.html#the-if-statement
    title: The Python Language Reference, Python 3.15
    locator: 8.1. The if statement
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/reference/expressions.html#boolean-operations
    title: The Python Language Reference, Python 3.15
    locator: 6.11. Boolean operations
    accessed: 2026-11-16
  - url: https://go.dev/ref/spec#If_statements
    title: The Go Programming Language Specification
    locator: If statements
    accessed: 2026-11-16
---

## Synopsis

```python run id=sign
def sign(x):
    if x < 0:
        return "negative"
    elif x == 0:
        return "zero"
    else:
        return "positive"

print(sign(-5), sign(0), sign(7))
```

```output for=sign
negative zero positive
```

Python tests `x < 0` first, then `x == 0`. When no condition is true, the `else` block runs.

## Semantics

**r1.** The if statement "selects exactly one of the suites by evaluating the expressions one by one until one is found to be true". Then "that suite is executed (and no other part of the if statement is executed or evaluated). If all expressions are false, the suite of the else clause, if present, is executed."[^2] Examples: [`sign`](#sign), [`first-true`](#first-true).

**r2.** "There can be zero or more elif parts, and the else part is optional. The keyword ‘elif’ is short for ‘else if’, and is useful to avoid excessive indentation."[^1]

**r3.** A condition does not have to be a bool. "In the context of Boolean operations, and also when expressions are used by control flow statements, the following values are interpreted as false: False, None, numeric zero of all types, and empty strings and containers".[^3] "All other values are interpreted as true."[^3] Example: [`truth`](#truth).

**r4.** The condition is an assignment_expression in the grammar, so it can contain `:=`.[^2] Example: [`walrus`](#walrus).

**r5.** "An if … elif … elif … sequence is a substitute for the switch or case statements found in other languages."[^1] To compare one value with several constants, the tutorial also suggests the match statement.[^1]

## Examples

```python run id=first-true
def check(name, result):
    print("test", name)
    return result

if check("a", False):
    print("a")
elif check("b", True):
    print("b")
elif check("c", True):
    print("c")
```

```output for=first-true
test a
test b
b
```

Python does not evaluate the third condition, because the second condition is true (rule r1).

```python run id=truth
for value in [0, 1, "", "text", [], [0], None]:
    if value:
        print(repr(value), "is true")
    else:
        print(repr(value), "is false")
```

```output for=truth
0 is false
1 is true
'' is false
'text' is true
[] is false
[0] is true
None is false
```

```python run id=walrus
data = [3, 1, 4, 1, 5]
if (n := len(data)) > 3:
    print(f"{n} items is too many")
```

```output for=walrus
5 items is too many
```

## Pitfalls

`=` is not a comparison, and a condition cannot contain a plain assignment:

```python fails id=assign-in-if expect=exception:SyntaxError
x = 1
if x = 1:
    print("one")
```

A test of one value against several values needs a full comparison for each value:

```python wrong id=or-constant
color = "blue"
if color == "red" or "green":
    print("warm or green")
```

```output for=or-constant
warm or green
```

The condition is `(color == "red") or "green"`. The string `"green"` is not empty, so the condition is true for each color (rule r3). Write `color in ("red", "green")` or `color == "red" or color == "green"`.

## Comparison

In Go, "If statements specify the conditional execution of two branches according to the value of a boolean expression." The Go condition must be a boolean expression, and "The expression may be preceded by a simple statement, which executes before the expression is evaluated."[^4] Go uses `else if`, and it needs braces around each block. Python uses `elif` and indentation, and its condition can have any type (rule r3).
