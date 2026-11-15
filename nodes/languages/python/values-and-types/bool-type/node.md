---
tag: NAH8QC
kind: feature
owner: python
slug: bool-type
name: The bool type
aliases: [Python bool, True and False, bool is a subclass of int, bool(), Python Boolean]
area: languages/python/values-and-types
level: intro
summary: >-
  bool has exactly two values, True and False. It is a subclass of int, so True and False act like 1 and 0 in arithmetic. bool() converts any value with the truth value rules.
grammar: "atom: 'True' | 'False' | ... (Language Reference, 6.2)"
spec: {standard: null, url: "https://docs.python.org/3.15/builtins/stdtypes.html#boolean-type-bool", locator: "Boolean Type - bool"}
see_also: [python/truth-value-testing, python/boolean-operators, boolean-type]
sources:
  - url: https://docs.python.org/3.15/builtins/stdtypes.html#boolean-type-bool
    title: Built-in Types, Python 3.15 documentation
    locator: Boolean Type - bool
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/builtins/functions.html#bool
    title: Built-in Functions, Python 3.15 documentation
    locator: bool()
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/reference/datamodel.html#numbers-integral
    title: The Python Language Reference, Python 3.15
    locator: 3.2.4.1. numbers.Integral, Booleans (bool)
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/builtins/constants.html
    title: Built-in Constants, Python 3.15 documentation
    locator: False, True
    accessed: 2026-11-16
  - url: https://go.dev/ref/spec#Boolean_types
    title: The Go Programming Language Specification
    locator: Boolean types; Arithmetic operators
    accessed: 2026-11-16
---

## Synopsis

```python run id=bool-basics
print(True + True, True * 10, sum([True, False, True]))
print(isinstance(True, int), int(True), str(True))
print(bool(0), bool(42), bool(""), bool("False"), bool([]))
```

```output for=bool-basics
2 10 2
True 1 True
False True False True False
```

`True` and `False` add like `1` and `0`, but print as words. `bool()` gives the truth value of any object.

## Semantics

**r1.** "The bool type has exactly two constant instances: True and False."[^1] They are "the only Boolean objects".[^3] An assignment to `True` or `False` raises a `SyntaxError`.[^4]

**r2.** `bool` is a subclass of `int`, and in many numeric contexts `False` and `True` behave like `0` and `1`. Built-in Types discourages this use and says to convert explicitly with `int()`.[^1] The one exception is conversion to a string, which gives `"False"` or `"True"`.[^3] Example: [`bool-basics`](#bool-basics).

**r3.** `bool(x)` converts `x` with the standard truth testing procedure: it returns `False` if `x` is false or omitted, and `True` otherwise.[^2] Example: [`string-to-bool`](#string-to-bool).

**r4.** `bool` "cannot be subclassed further".[^2] Example: [`no-subclass`](#no-subclass).

**r5.** The bitwise operators `&`, `|` and `^` on two booleans return a `bool` with the result of "and", "or" and "xor". Built-in Types prefers the logical operators `and`, `or` and `!=`. The bitwise inversion `~` on a `bool` is deprecated, and Built-in Types says that Python 3.16 will make it an error.[^1] Example: [`bitwise-on-bool`](#bitwise-on-bool).

## Examples

`&`, `|` and `^` keep the `bool` type for `bool` operands (rule r5).

```python run id=bitwise-on-bool
print(True & False, True | False, True ^ True)
print(type(True ^ True).__name__, True != True)
```

```output for=bitwise-on-bool
False True False
bool False
```

`bool` is final (rule r4).

```python run id=no-subclass
try:
    class MyBool(bool):
        pass
except TypeError as e:
    print("TypeError:", e)
```

```output for=no-subclass
TypeError: type 'bool' is not an acceptable base type
```

## Pitfalls

### `bool("False")` is `True`

`bool()` tests the truth value and does not parse text (rule r3). Every non-empty string is true. Compare the text to parse it.

```python wrong id=string-to-bool
answer = "False"
print(bool(answer))
print(answer == "True")
```

```output for=string-to-bool
True
False
```

### `True` and `1` are the same key

`True == 1` and their hashes are equal (rule r2), so they are one dictionary key. The first key stays, and the last value wins.

```python wrong id=bool-key
d = {1: "one", True: "true"}
print(d, hash(True) == hash(1))
```

```output for=bool-key
{1: 'true'} True
```

## Comparison

In Go, `bool` is a separate type, the set of truth values "denoted by the predeclared constants true and false".[^5] The Go specification also says: "Arithmetic operators apply to numeric values".[^5] So `true + true` does not compile in Go. In Python, `True + True` is `2` (rule r2).
