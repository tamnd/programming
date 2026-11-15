---
tag: FXSCG9
kind: feature
owner: python
slug: conditional-expressions
name: Conditional expressions
aliases: [Python ternary operator, if else expression, inline if, x if c else y]
area: languages/python/syntax
level: intro
summary: >-
  x if C else y is Python's conditional expression, its form of the ternary operator. It evaluates C first, then only one of x and y, and it binds looser than every operator except lambda and :=.
grammar: "conditional_expression: or_test ['if' or_test 'else' expression]; expression: conditional_expression | lambda_expr (Language Reference, 6.13)"
spec: {standard: null, url: "https://docs.python.org/3.15/reference/expressions.html#conditional-expressions", locator: "6.13 Conditional expressions"}
see_also: [python/boolean-operators, python/comparison-operators, python/keywords]
sources:
  - url: https://docs.python.org/3.15/reference/expressions.html#conditional-expressions
    title: The Python Language Reference, Python 3.15
    locator: 6.13 Conditional expressions
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/reference/expressions.html#operator-precedence
    title: The Python Language Reference, Python 3.15
    locator: 6.17 Operator precedence
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/reference/expressions.html#comprehensions
    title: The Python Language Reference, Python 3.15
    locator: 6.2.6 Comprehensions
    accessed: 2026-11-16
  - url: https://peps.python.org/pep-0308/
    title: "PEP 308: Conditional Expressions"
    locator: Adding a conditional expression
    accessed: 2026-11-16
  - url: https://go.dev/doc/faq#Does_Go_have_a_ternary_form
    title: Frequently Asked Questions (FAQ), The Go Programming Language
    locator: "Why does Go not have the ?: operator?"
    accessed: 2026-11-16
---

## Synopsis

```python run id=ternary
for n in [3, -2, 0]:
    sign = "positive" if n > 0 else "negative" if n < 0 else "zero"
    print(n, sign)
```

```output for=ternary
3 positive
-2 negative
0 zero
```

## Semantics

**r1.** A conditional expression, "sometimes called a" ternary operator, is an alternative to the `if`-`else` statement. "As it is an expression, it returns a value and can appear as a sub-expression."[^1] Example: [`cond-in-expr`](#cond-in-expr).

**r2.** `x if C else y` "first evaluates the condition, *C* rather than *x*". If `C` is true, `x` is evaluated and returned; otherwise `y` is evaluated and returned. The other branch is not evaluated.[^1] Example: [`lazy-branch`](#lazy-branch).

**r3.** The conditional expression has a lower priority than `or` and every other operator, except `lambda` and `:=`. Unlike most operators, it groups from right to left, so `a if p else b if q else c` is `a if p else (b if q else c)`.[^2] Example: [`ternary`](#ternary).

**r4.** The `else` part is required: the grammar has no `x if C` form.[^1] Example: [`missing-else`](#missing-else).

**r5.** In a comprehension, a trailing `if` after the `for` clause is a filter, not a conditional expression, and it takes no `else`.[^3] Example: [`comp-if-else`](#comp-if-else).

**r6.** The form was chosen in PEP 308: "On 9/29/2005, Guido decided to add conditional expressions" in this form. "The motivating use case was the prevalence of error-prone attempts to achieve the same effect using" `and` and `or`.[^4] Example: [`and-or-trick`](#and-or-trick).

## Examples

The condition is evaluated first, then only one branch (rule r2).

```python run id=lazy-branch
def show(label, value):
    print("eval", label)
    return value

print(show("x", 1) if show("C", False) else show("y", 2))
```

```output for=lazy-branch
eval C
eval y
2
```

The whole `a + b` is the true branch, because `+` binds tighter (rule r3).

```python run id=cond-precedence
a, b = 1, 2
print(a + b if a > 5 else 0)
print(a + (b if a > 5 else 0))
```

```output for=cond-precedence
0
1
```

A conditional expression inside a list comprehension and an f-string (rule r1).

```python run id=cond-in-expr
items = [4, 7, 10]
print([n if n % 2 == 0 else -n for n in items])
print(f"{len(items)} item{'s' if len(items) != 1 else ''}")
```

```output for=cond-in-expr
[4, -7, 10]
3 items
```

## Pitfalls

### The `else` is not optional

`x if C` alone is a syntax error (rule r4). Use an `if` statement when there is no other value.

```python fails id=missing-else expect=exception:SyntaxError
debug = True
message = "verbose" if debug
```

### A filter `if` takes no `else`

To map values, put the conditional expression before `for`; to drop values, put a plain `if` after it (rule r5).

```python fails id=comp-if-else expect=exception:SyntaxError
items = [4, 7, 10]
print([n for n in items if n > 5 else 0])
```

### The old `and`-`or` trick

`C and x or y` gives `y` whenever `x` is false, so it is wrong for values like `""` or `0`. This is the error-prone idiom that PEP 308 replaced (rule r6).

```python wrong id=and-or-trick
flag = True
print(repr(flag and "" or "none"))
print(repr("" if flag else "none"))
```

```output for=and-or-trick
'none'
''
```

## Comparison

Go has no conditional expression: "There is no ternary testing operation in Go." The Go FAQ suggests an `if`-`else` statement instead, and says the designers had seen the operator "used too often to create impenetrably complex expressions".[^5] In Python the same choice is an expression (rule r1), with the condition in the middle.
