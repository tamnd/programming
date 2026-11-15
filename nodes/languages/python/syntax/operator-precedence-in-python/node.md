---
tag: N9WFB2
kind: feature
owner: python
slug: operator-precedence-in-python
name: Operator precedence in Python
aliases: [Python operator precedence, order of operations, operator priority, associativity, binding strength]
area: languages/python/syntax
level: core
summary: >-
  Python's precedence table runs from primaries, await and ** at the top down to or, if-else, lambda and := at the bottom. Most operators group left to right; ** and if-else group right to left.
grammar: "expression: conditional_expression | lambda_expr; conditional_expression: or_test ['if' or_test 'else' expression] (Language Reference, 6.13; the full order is the table in 6.17)"
spec: {standard: null, url: "https://docs.python.org/3.15/reference/expressions.html#operator-precedence", locator: "6.17 Operator precedence"}
see_also: [python/arithmetic-operators, python/bitwise-and-shift-operators, python/comparison-operators, python/boolean-operators, python/conditional-expressions]
sources:
  - url: https://docs.python.org/3.15/reference/expressions.html#operator-precedence
    title: The Python Language Reference, Python 3.15
    locator: 6.17 Operator precedence
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/reference/expressions.html#the-power-operator
    title: The Python Language Reference, Python 3.15
    locator: 6.5 The power operator
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/reference/expressions.html#parenthesized-groups
    title: The Python Language Reference, Python 3.15
    locator: 6.2.4 Parenthesized groups
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/library/ast.html#ast.BinOp
    title: ast, Abstract Syntax Trees, Python 3.15 documentation
    locator: ast.BinOp, ast.UnaryOp, ast.BoolOp, ast.Compare
    accessed: 2026-11-16
  - url: https://go.dev/ref/spec#Operator_precedence
    title: The Go Programming Language Specification
    locator: Operator precedence
    accessed: 2026-11-16
---

## Synopsis

```python run id=precedence-demo
print(2 + 3 * 4, (2 + 3) * 4)
print(2 ** 3 ** 2, (2 ** 3) ** 2)
print(-2 ** 2, 2 ** -1)
print(1 + 2 << 1, not 1 + 1 == 2)
```

```output for=precedence-demo
14 20
512 64
-4 0.5
6 False
```

## Semantics

**r1.** The precedence table lists the operators "from highest precedence (most binding) to lowest precedence (least binding)", and operators in the same box have the same precedence.[^1] From the top: bindings and displays; subscriptions, slicings, calls and attribute references; `await`; `**`; unary `+x`, `-x` and `~x`; `*`, `@`, `/`, `//` and `%`; `+` and `-`; the shifts; `&`; `^`; `|`; the comparisons, membership and identity tests; `not`; `and`; `or`; `if`-`else`; `lambda`; and `:=`.[^1] Example: [`show-grouping`](#show-grouping).

**r2.** Operators in the same box group left to right, "except for exponentiation and conditional expressions, which group from right to left".[^1] Example: [`left-to-right`](#left-to-right).

**r3.** The power operator binds more tightly than a unary operator on its left and less tightly than a unary operator on its right, so `-1**2` is `-1` and `2**-1` is `0.5`.[^2] [^1] Example: [`precedence-demo`](#precedence-demo).

**r4.** The comparisons, membership tests and identity tests all share one level and chain from left to right instead of grouping.[^1] Example: [`show-grouping`](#show-grouping).

**r5.** `%` keeps its precedence when it formats a string.[^1] Example: [`percent-format`](#percent-format).

**r6.** Parentheses form a group, which overrides or clarifies the precedence; a parenthesized expression yields whatever the expression inside yields.[^3] Example: [`walrus-precedence`](#walrus-precedence).

## Examples

The `ast` module shows how the parser grouped an expression. Its `BinOp`, `UnaryOp`, `BoolOp` and `Compare` nodes are the binary, unary, Boolean and comparison operations;[^4] this function puts parentheses around each one (rules r1, r3 and r4).

```python run id=show-grouping
import ast

SYMBOLS = {ast.Add: "+", ast.Mult: "*", ast.Pow: "**", ast.LShift: "<<", ast.BitAnd: "&",
           ast.BitOr: "|", ast.USub: "-", ast.Not: "not ", ast.Lt: "<", ast.And: "and", ast.Or: "or"}

def show(node):
    match node:
        case ast.BinOp(left, op, right):
            return f"({show(left)} {SYMBOLS[type(op)]} {show(right)})"
        case ast.UnaryOp(op, operand):
            return f"({SYMBOLS[type(op)]}{show(operand)})"
        case ast.BoolOp(op, values):
            return "(" + f" {SYMBOLS[type(op)]} ".join(show(v) for v in values) + ")"
        case ast.Compare(left, ops, comparators):
            parts = [show(left)] + [f"{SYMBOLS[type(o)]} {show(c)}" for o, c in zip(ops, comparators)]
            return "(" + " ".join(parts) + ")"
        case _:
            return ast.unparse(node)

for source in ["1 + 2 * 3 ** 2", "-x ** 2", "a | b & c << 1", "not a < b and c or d", "x < y + 1 < z"]:
    print(f"{source:22} {show(ast.parse(source, mode='eval').body)}")
```

```output for=show-grouping
1 + 2 * 3 ** 2         (1 + (2 * (3 ** 2)))
-x ** 2                (-(x ** 2))
a | b & c << 1         (a | (b & (c << 1)))
not a < b and c or d   (((not (a < b)) and c) or d)
x < y + 1 < z          (x < (y + 1) < z)
```

The chain `x < (y + 1) < z` stays one comparison with two operators (rule r4).

Left-to-right grouping (rule r2): each pair of results differs.

```python run id=left-to-right
print(100 / 10 / 5, 100 / (10 / 5))
print(10 - 4 - 3, 10 - (4 - 3))
print(2 * 3 % 4, 2 * (3 % 4))
```

```output for=left-to-right
2.0 50.0
3 9
2 6
```

## Pitfalls

### `:=` binds loosest

`:=` is at the bottom of the table (rule r1), so `n := len(items) > 10` assigns the result of the comparison. Put the assignment in parentheses (rule r6).

```python wrong id=walrus-precedence
items = list(range(12))
if n := len(items) > 10:
    print("n is", n)
if (n := len(items)) > 10:
    print("n is", n)
```

```output for=walrus-precedence
n is True
n is 12
```

### `%` formatting before `+`

`"total: %d" % 2 + 3` formats first and then adds a number to a string (rule r5).

```python fails id=percent-format expect=exception:TypeError
print("total: %d" % 2 + 3)
```

Write `"total: %d" % (2 + 3)`.

### Unary minus and `**`

`-2 ** 2` is `-4`, not `4` (rule r3). Write `(-2) ** 2`.

## Comparison

Go has five precedence levels for binary operators, and its unary operators have the highest precedence.[^5] Python's table has more levels and puts `**` above the unary operators on its left (rule r3); Go has no power operator. Go's `<<`, `>>` and `&` share a level with `*`, and `|` and `^` share one with `+`,[^5] while Python gives the shifts and each bitwise operator a level of its own, below `+` (rule r1).
