---
tag: 7WX5QR
kind: feature
owner: python
slug: assignment-expressions
name: Assignment expressions
aliases: [walrus operator, "colon equals operator", named expression, PEP 572, assign inside an if, assign in a while condition]
area: languages/python/variables-and-scope
level: core
summary: >-
  name := expr assigns a value to a name and also returns the value, so an if, a while or a comprehension can test a value and keep it. The target is a single name in the current scope.
grammar: "assignment_expression: [identifier ':='] expression (Language Reference, 6.12)"
spec: {standard: null, url: "https://docs.python.org/3.15/reference/expressions.html#assignment-expressions", locator: "6.12. Assignment expressions"}
see_also: [python/assignment-statements, python/naming-and-binding, python/python-scopes-and-namespaces]
sources:
  - url: https://docs.python.org/3.15/reference/expressions.html#assignment-expressions
    title: The Python Language Reference, Python 3.15
    locator: 6.12. Assignment expressions
    accessed: 2026-11-16
  - url: https://peps.python.org/pep-0572/#scope-of-the-target
    title: PEP 572 - Assignment Expressions
    locator: Scope of the target; Exceptional cases; Differences between assignment expressions and assignment statements
    accessed: 2026-11-16
  - url: https://go.dev/ref/spec#If_statements
    title: The Go Programming Language Specification
    locator: If statements; Blocks; Statements
    accessed: 2026-11-16
---

## Synopsis

```python run id=while-loop
chunks = iter(["ab", "cd", ""])
while chunk := next(chunks):
    print(chunk)
print(chunk == "")
```

```output for=while-loop
ab
cd
True
```

The condition reads a chunk, binds it to `chunk` and tests it. The loop stops at the empty string.

## Semantics

**r1.** "An assignment expression (sometimes also called a “named expression” or “walrus”) assigns an expression to an identifier, while also returning the value of the expression."[^1] Example: [`while-loop`](#while-loop).

**r2.** "Assignment expressions must be surrounded by parentheses when used as expression statements and when used as sub-expressions in slicing, conditional, lambda, keyword-argument, and comprehension-if expressions and in assert, with, and assignment statements." "In all other places where they can be used, parentheses are not required, including in if and while statements."[^1] Examples: [`parentheses`](#parentheses), [`bare-statement`](#bare-statement).

**r3.** "An assignment expression does not introduce a new scope." "it is the current scope. If this scope contains a nonlocal or global declaration for the target, the assignment expression honors that."[^2] Example: [`scope`](#scope).

**r4.** In a comprehension, an assignment expression "binds the target in the containing scope".[^2] So the name is still bound after the comprehension. Example: [`comprehension`](#comprehension).

**r5.** "assignment expressions don't support the advanced features found in assignment statements". "Single assignment targets other than a single NAME are not supported", and unpacking, inline type annotations and augmented assignment are not supported.[^2] Example: [`attribute-target`](#attribute-target).

**r6.** "Priority around commas is different": `x = 1, 2` sets `x` to `(1, 2)`, but `(x := 1, 2)` sets `x` to `1`.[^2] Example: [`commas`](#commas).

## Examples

```python run id=parentheses
import re

data = "id=42"
if m := re.search(r"\d+", data):
    print(m.group())
values = [y := 5, y ** 2]
print(values, y)
```

```output for=parentheses
42
[5, 25] 5
```

In an `if` statement and in a list display, no parentheses are needed (rule r2).

```python run id=comprehension
data = [3, 8, 12]
big = [y for x in data if (y := x * 2) > 10]
print(big, y)
print(any((hit := n) > 5 for n in data), hit)
```

```output for=comprehension
[16, 24] 24
True 8
```

`y` and `hit` stay bound after the comprehension. `hit` keeps the first value that made `any()` stop (rule r4).

```python run id=scope
def count():
    total = 0
    def add(n):
        nonlocal total
        return (total := total + n)
    add(2)
    add(3)
    return total

print(count())
```

```output for=scope
5
```

```python run id=commas
x = 1, 2
print(x)
(x := 1, 2)
print(x)
```

```output for=commas
(1, 2)
1
```

```python fails id=attribute-target expect=exception:SyntaxError
class C:
    pass

c = C()
if (c.value := 3) > 0:
    print(c.value)
```

## Pitfalls

An assignment expression cannot be a statement on its own without parentheses:

```python fails id=bare-statement expect=exception:SyntaxError
y := 3
```

PEP 572 says that this rule makes the choice between the two forms simple, and that `(y := f(x))` is "Valid, though not recommended".[^2] Use a normal assignment statement.

In the grammar, the right side of `:=` is a whole `expression` (see the grammar of this page).[^1] `n := len(a) > 3` binds `n` to the result of the comparison. Put parentheses around the assignment expression when another operator follows it:

```python wrong id=priority
a = [1, 2, 3, 4]
if n := len(a) > 3:
    print(n)
```

```output for=priority
True
```

Write `if (n := len(a)) > 3:` to bind `n` to `4`.

## Comparison

In the Go grammar, an assignment is a statement: "SimpleStmt = EmptyStmt | ExpressionStmt | SendStmt | IncDecStmt | Assignment | ShortVarDecl". An `if` statement can start with a simple statement: "The expression may be preceded by a simple statement, which executes before the expression is evaluated", as in `if x := f(); x < y { ... }`.[^3] Each `if`, `for` and `switch` statement "is considered to be in its own implicit block", so `x` is local to the `if` statement.[^3] A Python name bound by `:=` in an `if` stays bound in the current scope after the statement (rule r3).
