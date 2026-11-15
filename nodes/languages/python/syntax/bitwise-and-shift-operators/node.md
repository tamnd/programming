---
tag: 8SRX65
kind: feature
owner: python
slug: bitwise-and-shift-operators
name: Bitwise and shift operators
aliases: [Python bitwise operators, bitwise and, bitwise or, xor operator, left shift, right shift, invert operator]
area: languages/python/syntax
level: core
summary: >-
  Python's bitwise operators &, |, ^ and ~ and its shifts << and >> work on integers as if in two's complement with infinite sign bits. They bind looser than arithmetic and tighter than comparisons.
grammar: "shift_expr: a_expr | shift_expr ('<<' | '>>') a_expr; and_expr: shift_expr | and_expr '&' shift_expr; xor_expr: and_expr | xor_expr '^' and_expr; or_expr: xor_expr | or_expr '|' xor_expr (Language Reference, 6.8 and 6.9)"
spec: {standard: null, url: "https://docs.python.org/3.15/reference/expressions.html#binary-bitwise-operations", locator: "6.8 Shifting operations; 6.9 Binary bitwise operations"}
see_also: [python/arithmetic-operators, python/numeric-literals, python/operators-and-delimiters]
sources:
  - url: https://docs.python.org/3.15/builtins/stdtypes.html#bitwise-operations-on-integer-types
    title: Built-in Types, Python 3.15 documentation
    locator: Bitwise Operations on Integer Types
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/reference/expressions.html#binary-bitwise-operations
    title: The Python Language Reference, Python 3.15
    locator: 6.9 Binary bitwise operations
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/reference/expressions.html#shifting-operations
    title: The Python Language Reference, Python 3.15
    locator: 6.8 Shifting operations
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/builtins/stdtypes.html#typesnumeric
    title: Built-in Types, Python 3.15 documentation
    locator: Numeric Types, int, float, complex
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/reference/expressions.html#unary-arithmetic-and-bitwise-operations
    title: The Python Language Reference, Python 3.15
    locator: 6.6 Unary arithmetic and bitwise operations
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/builtins/stdtypes.html#boolean-type-bool
    title: Built-in Types, Python 3.15 documentation
    locator: Boolean Type, bool
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/builtins/stdtypes.html#set-types-set-frozenset
    title: Built-in Types, Python 3.15 documentation
    locator: Set Types, set, frozenset
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/builtins/stdtypes.html#typesmapping
    title: Built-in Types, Python 3.15 documentation
    locator: Mapping types, dict, frozendict
    accessed: 2026-11-16
  - url: https://go.dev/ref/spec#Operator_precedence
    title: The Go Programming Language Specification
    locator: Operator precedence
    accessed: 2026-11-16
  - url: https://go.dev/ref/spec#Arithmetic_operators
    title: The Go Programming Language Specification
    locator: Arithmetic operators
    accessed: 2026-11-16
---

## Synopsis

```python run id=bit-ops
a, b = 0b1100, 0b1010
print(bin(a & b), bin(a | b), bin(a ^ b))
print(~a, a << 2, a >> 2)
```

```output for=bit-ops
0b1000 0b1110 0b110
-13 48 3
```

## Semantics

**r1.** Bitwise operations make sense only for integers, and the result is computed "as though carried out in two's complement with an infinite number of sign bits".[^1] Example: [`negative-bits`](#negative-bits).

**r2.** `&` gives the bitwise AND, `^` the bitwise XOR and `|` the bitwise OR of two integers. A custom class can support them with `__and__()`, `__xor__()` and `__or__()` and their reflected forms.[^2] Example: [`bit-ops`](#bit-ops).

**r3.** `x << n` and `x >> n` shift `x` by `n` bits. "A right shift by *n* bits is defined as floor division by" `pow(2,n)`, and a left shift by `n` bits is multiplication with `pow(2,n)`.[^3] Since integers have unlimited precision, a left shift never overflows.[^4] Example: [`big-shift`](#big-shift).

**r4.** "Negative shift counts are illegal and cause a" `ValueError`.[^1] Example: [`negative-shift`](#negative-shift).

**r5.** The unary `~` gives the bitwise inversion of an integer, defined as `-(x+1)`. It has the same priority as unary `-` and `+`. An argument of the wrong type raises `TypeError`.[^5] Example: [`bit-ops`](#bit-ops).

**r6.** The binary bitwise operators have priorities "lower than the numeric operations and higher than the comparisons".[^1] The shifts bind tighter than `&`, `&` tighter than `^`, and `^` tighter than `|`.[^2] [^3] Example: [`bit-precedence`](#bit-precedence).

**r7.** Applied to two bools, `&`, `|` and `^` return a bool that matches `and`, `or` and xor, but the docs prefer `and`, `or` and `!=`. The 3.12 release deprecated `~` on a bool, and the docs say it will raise an error in Python 3.16.[^6] Example: [`bool-bits`](#bool-bits).

**r8.** Sets reuse the symbols: `|` is the union, `&` the intersection and `^` the symmetric difference.[^7] For dicts, `d | other` makes a new dict whose values from `other` win on shared keys; this operator needs Python 3.9 or later.[^8] Example: [`sets-and-dicts`](#sets-and-dicts).

## Examples

Infinite sign bits (rule r1): a right shift of a negative number stays negative, and a mask cuts a negative number down to a fixed width.

```python run id=negative-bits
print(-1 >> 10, -7 >> 1, -7 // 2, ~0)
print(-5 & 0xFF, bin(-5 & 0xFF))
```

```output for=negative-bits
-1 -4 -4 -1
251 0b11111011
```

Shifts are exact on big integers (rule r3).

```python run id=big-shift
big = 1 << 100
print(big, big.bit_length(), big >> 98)
```

```output for=big-shift
1267650600228229401496703205376 101 4
```

Permission flags: set a bit with `|`, test it with `&`, clear it with `& ~`, and toggle it with `^`.

```python run id=flags
READ, WRITE, EXEC = 4, 2, 1
mode = READ | WRITE
print(mode, bool(mode & WRITE), bool(mode & EXEC))
mode &= ~WRITE
print(mode)
mode ^= EXEC
print(mode, bin(mode))
```

```output for=flags
6 True False
4
5 0b101
```

The priorities of rule r6: `6 & 3 == 2` groups as `(6 & 3) == 2`.

```python run id=bit-precedence
print(6 & 3 == 2, 1 | 2 ^ 3 & 1, 1 | (2 ^ (3 & 1)))
```

```output for=bit-precedence
True 3 3
```

Bitwise operators on bools (rule r7).

```python run id=bool-bits
print(True & False, True | False, True ^ True)
print(type(True & True).__name__)
```

```output for=bool-bits
False True False
bool
```

The set and dict forms (rule r8).

```python run id=sets-and-dicts
a, b = {1, 2, 3}, {2, 3, 4}
print(a | b, a & b, a ^ b)
print({"x": 1, "y": 2} | {"y": 20, "z": 3})
```

```output for=sets-and-dicts
{1, 2, 3, 4} {2, 3} {1, 4}
{'x': 1, 'y': 20, 'z': 3}
```

## Pitfalls

### `^` is not the power operator

`^` is XOR, so `2 ^ 3` is `1`. Use `**` for a power.

```python wrong id=caret-power
print(2 ^ 3, 2 ** 3)
```

```output for=caret-power
1 8
```

### A shift binds looser than `+`

Arithmetic happens first (rule r6), so `1 << 2 + 1` is `1 << 3`.

```python wrong id=shift-plus
print(1 << 2 + 1, (1 << 2) + 1)
```

```output for=shift-plus
8 5
```

### Negative shift counts

A negative count raises `ValueError` (rule r4). Shift the other way instead.

```python fails id=negative-shift expect=exception:ValueError
print(1 << -1)
```

### Floats have no bits

The bitwise operators reject a float (rules r1 and r2). Convert with `int()` first.

```python fails id=float-bits expect=exception:TypeError
print(3.0 & 1)
```

## Comparison

Go puts `<<`, `>>` and `&` in the same precedence level as `*`, and `|` and `^` with `+`.[^9] So in Go `1 + 2 << 1` is `1 + (2 << 1)`, while in Python it is `(1 + 2) << 1` (rule r6). Go writes the bitwise complement as the unary `^x`, and a negative shift count at run time causes a panic.[^10] Python uses `~x` (rule r5) and raises `ValueError` (rule r4).
