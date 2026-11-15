---
tag: FB53M2
kind: feature
owner: python
slug: numbers
name: Numbers
aliases: [Python as a calculator, Python arithmetic, int and float, floor division, underscore variable]
area: languages/python/values-and-types
level: intro
summary: >-
  Python works as a calculator: + - * / ** and parentheses act on int and float values. The / operator always gives a float, // floors and % gives the remainder.
grammar: "m_expr: m_expr '*' u_expr | m_expr '//' u_expr | m_expr '/' u_expr | m_expr '%' u_expr | ...; power: await_or_primary ['**' u_expr] (Language Reference, 6.5 and 6.7)"
spec: {standard: null, url: "https://docs.python.org/3.15/tutorial/introduction.html#numbers", locator: "3.1.1. Numbers"}
see_also: [python/arithmetic-operators, python/numeric-literals, python/operator-precedence-in-python, floating-point-arithmetic]
sources:
  - url: https://docs.python.org/3.15/tutorial/introduction.html#numbers
    title: The Python Tutorial, Python 3.15
    locator: 3.1.1. Numbers, and footnote 1
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/builtins/stdtypes.html#numeric-types-int-float-complex
    title: Built-in Types, Python 3.15 documentation
    locator: Numeric Types, int, float, complex
    accessed: 2026-11-16
  - url: https://go.dev/ref/spec#Arithmetic_operators
    title: The Go Programming Language Specification
    locator: Arithmetic operators
    accessed: 2026-11-16
---

## Synopsis

```python run id=calculator
print(2 + 2, 50 - 5*6, (50 - 5*6) / 4, 8 / 5)
print(17 / 3, 17 // 3, 17 % 3, 5 * 3 + 2)
print(5 ** 2, 2 ** 7, 4 * 3.75 - 1)
```

```output for=calculator
4 20 5.0 1.6
5.666666666666667 5 2 17
25 128 14.0
```

These are the examples of the tutorial. Whole numbers are `int` values, and numbers with a fractional part are `float` values.

## Semantics

**r1.** The operators `+`, `-`, `*` and `/` do arithmetic, and parentheses group. The integer numbers "have type int", and the numbers with a fractional part "have type float".[^1]

**r2.** "Division (/) always returns a float." The `//` operator does floor division and gives an integer result for integer operands, and `%` gives the remainder.[^1] Example: [`result-types`](#result-types).

**r3.** `**` computes powers. It binds more tightly than unary minus, so `-3**2` is `-(3**2)`, which is `-9`; write `(-3)**2` to get `9`.[^1] Example: [`power-and-minus`](#power-and-minus).

**r4.** With mixed operand types, the integer operand is converted to floating point.[^1] Built-in Types states the general rule: when a binary arithmetic operator has operands of different built-in numeric types, the narrower operand is widened to the type of the other. An `int` combined with a `float` is converted to a `float`.[^2]

**r5.** In interactive mode, the last printed expression is assigned to the variable `_`. The tutorial says to treat it as read-only, because an assignment to `_` makes "an independent local variable with the same name masking the built-in variable".[^1] Example: [`underscore`](#underscore).

## Examples

The result type of each operator (rule r2). `6 / 3` is a float even though the division is exact.

```python run id=result-types
print(type(17 // 3).__name__, type(17 / 3).__name__, type(6 / 3).__name__)
print(type(7 // 2.0).__name__, 7 // 2.0)
```

```output for=result-types
int float float
float 3.0
```

The `'single'` mode of `compile()` runs code as interactive input, so it shows the `_` variable at work (rule r5).

```python run id=underscore
import builtins

exec(compile("12.5 / 100 * 100.50\n", "<calc>", "single"))
exec(compile("round(_, 2)\n", "<calc>", "single"))
print(builtins._)
```

```output for=underscore
12.5625
12.56
12.56
```

## Pitfalls

### Unary minus and powers

`**` is applied before the unary minus (rule r3).

```python wrong id=power-and-minus
print(-3 ** 2)
print((-3) ** 2)
```

```output for=power-and-minus
-9
9
```

### Assigning to `_`

After an assignment to `_`, the interactive value is hidden by the new variable (rule r5).

```python wrong id=shadow-underscore
_ = "my own value"
exec(compile("6 * 7\n", "<calc>", "single"))
print(_)
```

```output for=shadow-underscore
42
my own value
```

## Comparison

In Go, arithmetic operators "yield a result of the same type as the first operand", and for binary operators "the operand types must be identical unless the operation involves shifts or untyped constants".[^3] So in Go `8 / 5` is the integer `1`, and `4 * 3.75` with an `int` variable does not compile. Python gives `1.6` and converts the `int` operand to `float` (rules r2 and r4).
