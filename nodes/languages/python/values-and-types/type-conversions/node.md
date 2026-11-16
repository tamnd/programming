---
tag: B8AVZ8
kind: feature
owner: python
slug: type-conversions
name: Type conversions
aliases: [Python type casting, int() float() str() bool(), convert string to number in Python, __index__, implicit conversion in Python]
area: languages/python/values-and-types
level: intro
summary: >-
  Python converts a value by calling the target type, such as int(x), float(x) or str(x). Only numbers convert implicitly, and "1" + 1 is a TypeError. Classes can define __int__, __index__ and __str__.
grammar: null
spec: {standard: null, url: "https://docs.python.org/3.15/builtins/functions.html#int", locator: "Built-in Functions"}
see_also: [python/arithmetic-conversions, python/int-type, python/float-type, python/truth-value-testing, type-conversion]
sources:
  - url: https://docs.python.org/3.15/builtins/functions.html#int
    title: Built-in Functions, Python 3.15 documentation
    locator: int(), float(), complex(), bool(), list(), tuple()
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/builtins/stdtypes.html#str
    title: Built-in Types, Python 3.15 documentation
    locator: str()
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/builtins/stdtypes.html#numeric-types-int-float-complex
    title: Built-in Types, Python 3.15 documentation
    locator: Numeric Types - int, float, complex
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/reference/datamodel.html#object.__index__
    title: The Python Language Reference, Python 3.15
    locator: 3.3.1 object.__str__(); 3.3.8 object.__index__()
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/reference/expressions.html#binary-arithmetic-operations
    title: The Python Language Reference, Python 3.15
    locator: 6.7. Binary arithmetic operations
    accessed: 2026-11-16
  - url: https://go.dev/ref/spec#Conversions
    title: The Go Programming Language Specification
    locator: Conversions
    accessed: 2026-11-16
---

## Synopsis

```python run id=convert
print(int("42"), float("4.2"), str(42), complex("1+2j"), bool("x"))
print(list("abc"), tuple([1, 2]), int(4.9), float(3), str([1, "a"]))
print("1" + str(1), int("1") + 1)
```

```output for=convert
42 4.2 42 (1+2j) True
['a', 'b', 'c'] (1, 2) 4 3.0 [1, 'a']
11 2
```

Each conversion calls a type. `"1" + str(1)` joins strings, and `int("1") + 1` adds numbers.

## Semantics

**r1.** The constructors `int()`, `float()` and `complex()` "can be used to produce numbers of a specific type".[^3] `list` and `tuple` are not functions but sequence types, and calling them with an iterable gives a list or a tuple.[^1] Example: [`convert`](#convert).

**r2.** `int()` with a string reads an integer in a base, and `float()` reads a decimal number or a NaN or infinity. A string in the wrong form raises `ValueError`.[^1] Conversion from a float to an int "truncates, discarding the fractional part".[^3] Example: [`bad-string`](#bad-string).

**r3.** `str(object)` returns `type(object).__str__(object)`, the informal, nicely printable string representation. If the object has no `__str__()` method, `str()` returns `repr(object)`.[^2] `__str__()` "must be a str object".[^4] Example: [`convert`](#convert).

**r4.** `bool(x)` converts with "the standard truth testing procedure", so it returns `False` for a false or omitted argument and `True` otherwise.[^1] Example: [`convert`](#convert).

**r5.** A class takes part in the numeric conversions with special methods. `int(x)` returns `x.__int__()` if it is defined, or else `x.__index__()`.[^1] `__index__()` is called "whenever Python needs to losslessly convert the numeric object to an integer object", for example in slicing and in `bin()`, `hex()` and `oct()`. If `__int__()`, `__float__()` and `__complex__()` are not defined, `int()`, `float()` and `complex()` fall back to `__index__()`.[^4] Example: [`index-method`](#index-method).

**r6.** For `+`, the arguments "must either both be numbers or both be sequences of the same type". Numbers are converted to a common real type, and sequences are concatenated.[^5] So the only implicit conversions of `+` are between numbers. Example: [`no-implicit-str`](#no-implicit-str).

## Examples

`__index__()` makes an object usable as an integer in `int()`, `float()`, `bin()` and indexing (rule r5).

```python run id=index-method
class Seven:
    def __index__(self):
        return 7

s = Seven()
print(int(s), float(s), bin(s), "abcdefgh"[s])
```

```output for=index-method
7 7.0 0b111 h
```

## Pitfalls

### `"1" + 1`

A string and a number do not mix with `+` (rule r6). Convert one side explicitly, to `str` to join text or to `int` to add.

```python fails id=no-implicit-str expect=exception:TypeError
count = 1
print("count: " + count)
```

### `int("4.2")` and `int("")`

`int()` does not read a decimal point or an empty string (rule r2). Read the text with `float()` first, or check the input.

```python wrong id=bad-string
for text in ["42", "4.2", ""]:
    try:
        print(int(text))
    except ValueError:
        print("ValueError for", repr(text))
```

```output for=bad-string
42
ValueError for '4.2'
ValueError for ''
```

### `"3" == 3` is false

`==` does not convert a string to a number, so text read from input never equals a number. Convert the text first.

```python wrong id=string-equals-number
print("3" == 3, int("3") == 3, 3 == 3.0)
```

```output for=string-equals-number
False True True
```

## Comparison

In Go, "A conversion changes the type of an expression to the type specified by the conversion", and an explicit conversion has the form `T(x)`.[^6] Python uses the same form, a call of the type, such as `int(x)` or `str(x)`. In Python the call is an ordinary function call of a class, so a class can control it with special methods (rule r5).
