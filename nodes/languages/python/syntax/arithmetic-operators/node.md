---
tag: S88ARJ
kind: feature
owner: python
slug: arithmetic-operators
name: Arithmetic operators
aliases: [Python math operators, floor division, true division, modulo operator, power operator, matrix multiplication operator]
area: languages/python/syntax
level: intro
summary: >-
  Python's arithmetic operators are +, -, *, /, //, %, ** and @. / always gives a float, // and % round toward minus infinity, and ** binds tighter than a unary minus on its left.
grammar: "m_expr: u_expr | m_expr '*' u_expr | m_expr '@' m_expr | m_expr '//' u_expr | m_expr '/' u_expr | m_expr '%' u_expr (Language Reference, 6.7)"
spec: {standard: null, url: "https://docs.python.org/3.15/reference/expressions.html#binary-arithmetic-operations", locator: "6.5 The power operator; 6.6 Unary arithmetic and bitwise operations; 6.7 Binary arithmetic operations"}
see_also: [python/numeric-literals, python/operators-and-delimiters, python/atoms-and-primaries]
sources:
  - url: https://docs.python.org/3.15/reference/expressions.html#binary-arithmetic-operations
    title: The Python Language Reference, Python 3.15
    locator: 6.7 Binary arithmetic operations
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/reference/expressions.html#unary-arithmetic-and-bitwise-operations
    title: The Python Language Reference, Python 3.15
    locator: 6.6 Unary arithmetic and bitwise operations
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/builtins/stdtypes.html#typesnumeric
    title: Built-in Types, Python 3.15 documentation
    locator: Numeric Types, int, float, complex
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/reference/expressions.html#the-power-operator
    title: The Python Language Reference, Python 3.15
    locator: 6.5 The power operator
    accessed: 2026-11-16
  - url: https://go.dev/ref/spec#Integer_operators
    title: The Go Programming Language Specification
    locator: Integer operators
    accessed: 2026-11-16
---

## Synopsis

```python run id=operators
print(7 + 2, 7 - 2, 7 * 2)
print(7 / 2, 7 // 2, 7 % 2)
print(2 ** 10, 10 ** -2, -7 // 2, -7 % 2)
```

```output for=operators
9 5 14
3.5 3 1
1024 0.01 -4 1
```

## Semantics

**r1.** Apart from the power operator, there are two priority levels: the multiplicative operators `*`, `@`, `/`, `//` and `%`, and the additive operators `+` and `-`. The unary operators `-`, `+` and `~` share one level above them.[^1] [^2]

**r2.** With operands of different numeric types, the operand of the narrower type is widened to the type of the other. If both are complex, nothing is converted; if either one is a complex or a float, the other is converted to a float; otherwise both are integers. A complex and a real operand are combined by the usual formula, such as `x + complex(u, v) = complex(x + u, v)`.[^3] Example: [`mixed-types`](#mixed-types).

**r3.** `/` and `//` give the quotient. "Division of integers yields a float, while floor division of integers results in an integer": `//` is mathematical division with the floor function applied. Division by zero raises `ZeroDivisionError`.[^1] Example: [`operators`](#operators).

**r4.** `%` gives the remainder, and the result always has "the same sign as its second operand (or zero)". The two are connected by `x == (x//y)*y + (x%y)`, and `divmod(x, y)` returns `(x//y, x%y)`.[^1] Example: [`negative-division`](#negative-division).

**r5.** `**` binds more tightly than a unary operator on its left, and less tightly than one on its right, so `-1**2` is `-1`. A chain of `**` is evaluated from right to left. With int operands the result is an int, unless the exponent is negative: then the result is a float.[^4] Example: [`power-binding`](#power-binding).

**r6.** `*` multiplies two numbers, or repeats a sequence when the other argument is an integer, and a negative factor gives an empty sequence. `+` adds two numbers, or concatenates two sequences of the same type.[^1] Example: [`sequences`](#sequences).

**r7.** `@` is for matrix multiplication, and "No builtin Python types implement this operator". A class supports it with `__matmul__()` and `__rmatmul__()`.[^1] Example: [`matmul`](#matmul).

## Examples

The common type of rule r2.

```python run id=mixed-types
for value in [3 + 4, 3 + 4.0, 3 + 4j, 6 / 3, 6 // 3, 6.0 // 3]:
    print(repr(value), type(value).__name__)
```

```output for=mixed-types
7 int
7.0 float
(3+4j) complex
2.0 float
2 int
2.0 float
```

Floor division and modulo with negative numbers (rule r4). The identity holds in each row.

```python run id=negative-division
for x, y in [(7, 3), (-7, 3), (7, -3), (-7, -3)]:
    q, r = divmod(x, y)
    print(f"{x:3} {y:3} -> {q:3} {r:3}  {q * y + r == x}")
```

```output for=negative-division
  7   3 ->   2   1  True
 -7   3 ->  -3   2  True
  7  -3 ->  -3  -2  True
 -7  -3 ->   2  -1  True
```

Sequence repetition and concatenation (rule r6).

```python run id=sequences
print([0] * 3, "ab" * 2, "ab" * -1 == "")
print([1, 2] + [3], "py" + "thon")
```

```output for=sequences
[0, 0, 0] abab True
[1, 2, 3] python
```

A small class that defines `@` (rule r7).

```python run id=matmul
class Matrix:
    def __init__(self, rows):
        self.rows = rows

    def __matmul__(self, other):
        columns = list(zip(*other.rows))
        return Matrix([[sum(a * b for a, b in zip(row, col)) for col in columns] for row in self.rows])

a = Matrix([[1, 2], [3, 4]])
b = Matrix([[5, 6], [7, 8]])
print((a @ b).rows)
```

```output for=matmul
[[19, 22], [43, 50]]
```

## Pitfalls

### `-2 ** 2` is `-4`

The power binds before the unary minus on its left (rule r5). Put the negative base in parentheses.

```python wrong id=power-binding
print(-2 ** 2)
print((-2) ** 2)
print(2 ** 3 ** 2)
```

```output for=power-binding
-4
4
512
```

`2 ** 3 ** 2` is `2 ** 9`, because the chain groups from the right.

### `/` on integers gives a float

`7 / 2` is `3.5`, and `6 / 3` is `2.0`, not `2` (rule r3). Use `//` for an integer quotient.

### Division by zero

`/`, `//` and `%` raise `ZeroDivisionError` for a zero right argument (rules r3 and r4).

```python fails id=divide-by-zero expect=exception:ZeroDivisionError
total = 10
count = 0
print(total / count)
```

## Comparison

Go uses truncated division for integers: `x / y` is "truncated towards zero", so `-5 / 3` is `-1` and `-5 % 3` is `-2`.[^5] Python floors, so `-5 // 3` is `-2` and `-5 % 3` is `1` (rule r4). The two languages agree when both operands are positive.
