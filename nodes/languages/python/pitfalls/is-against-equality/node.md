---
tag: KWACKV
kind: feature
owner: python
slug: is-against-equality
name: is against ==
aliases: [is versus ==, identity against equality, is with a literal, small integer cache, F632, is-literal, SyntaxWarning is with a literal]
area: languages/python/pitfalls
level: intro
summary: >-
  The is operator tests if two names refer to the same object. The == operator tests if two values are equal. Use == for values, and use is only for singletons such as None.
grammar: null
spec: {standard: null, url: "https://docs.python.org/3.15/reference/expressions.html#is-not", locator: "6.10.3 Identity comparisons"}
proposals: []
see_also: [python/comparison-operators, python/object-identity-and-interning, python/objects-values-and-types, python/rich-comparison-methods, python/none-notimplemented-and-ellipsis, python/warnings, python/int-type]
sources:
  - url: https://docs.python.org/3.15/reference/expressions.html#is-not
    title: The Python Language Reference, Python 3.15 documentation
    locator: 6.10.1 Value comparisons; 6.10.3 Identity comparisons
    accessed: 2026-11-18
  - url: https://peps.python.org/pep-0008/#programming-recommendations
    title: PEP 8, Style Guide for Python Code
    locator: Programming Recommendations
    accessed: 2026-11-18
  - url: https://docs.python.org/3.15/c-api/long.html#c.PyLong_FromLong
    title: Integer Objects, Python 3.15 documentation
    locator: PyLong_FromLong(), CPython implementation detail
    accessed: 2026-11-18
  - url: https://docs.python.org/3.15/whatsnew/3.8.html
    title: What’s New In Python 3.8
    locator: Changes in Python behavior
    accessed: 2026-11-18
  - url: https://docs.astral.sh/ruff/rules/is-literal/
    title: is-literal (F632), Ruff documentation
    locator: What it does; Why is this bad?
    accessed: 2026-11-18
  - url: https://docs.python.org/3.14/c-api/long.html#c.PyLong_FromLong
    title: Integer Objects, Python 3.14 documentation
    locator: PyLong_FromLong(), CPython implementation detail
    accessed: 2026-11-18
---

## Synopsis

```python run id=two-tests
a = [1, 2, 3]
b = [1, 2, 3]
c = a

print(a == b, a is b)
print(a == c, a is c)
```

```output for=two-tests
True False
True True
```

`a` and `b` are two lists with equal values. `a` and `c` are two names for one list.

## Semantics

**r1.** "The operators is and is not test for an object’s identity: x is y is true if and only if x and y are the same object."[^1] The `is` test cannot be changed by a class.

**r2.** "The operators <, >, ==, >=, <=, and != compare the values of two objects."[^1] A class can define what `==` means with `__eq__`. So `==` is the right test for numbers, strings, lists and most other values.

**r3.** Use `is` for singletons. PEP 8 says: "Comparisons to singletons like None should always be done with is or is not, never the equality operators."[^2] The same holds for a private sentinel object.

**r4.** Some equal values are the same object by chance. CPython keeps one object for each small integer: "CPython keeps an array of integer objects for all integers between -5 and 1024. When you create an int in that range you actually just get back a reference to the existing object."[^3] Short strings are also often shared. Code that uses `is` for such values works in a test and fails later. Example: [`by-chance`](#by-chance).

**r5.** The language does not promise any of this. The reference says that "Due to automatic garbage-collection, free lists, and the dynamic nature of descriptors, you may notice seemingly unusual behaviour in certain uses of the is operator".[^1]

**r6.** The compiler warns about `is` with a literal. "The compiler now produces a SyntaxWarning when identity checks (is and is not) are used with certain types of literals (e.g. strings, numbers). These can often work by accident in CPython, but are not guaranteed by the language spec."[^4] Example: [`literal-warning`](#literal-warning).

**r7.** Ruff rule F632 "Checks for is and is not comparisons against literals, like integers, strings, or lists."[^5] It also finds `x is []`, which the compiler does not warn about: such comparisons "will compare the identities of the objects instead of their values, which will always evaluate to False."[^5]

## Examples

```python run id=by-chance
small_a = int("100")
small_b = int("100")
big_a = int("100000")
big_b = int("100000")

print(small_a == small_b, small_a is small_b)
print(big_a == big_b, big_a is big_b)
```

```output for=by-chance
True True
True False
```

The two `100` values are one cached object, and the two `100000` values are two objects (rule r4). `==` gives the same answer for both pairs.

```python run id=literal-warning
import warnings

with warnings.catch_warnings(record=True) as caught:
    warnings.simplefilter("always")
    compile("x = 5\nprint(x is 5)\n", "check.py", "exec")

print(caught[0].category.__name__)
print(caught[0].message)
```

```output for=literal-warning
SyntaxWarning
"is" with 'int' literal. Did you mean "=="?
```

The compiler gives the warning when it compiles the code, before the code runs (rule r6).

## Versions

> [!since v="3.8"]
> `is` and `is not` with a literal of a type such as `int` or `str` give a `SyntaxWarning`.[^4]

> [!since v="3.15"]
> The cache of small integers is larger. The 3.14 documentation gives the range "between -5 and 256".[^6] The 3.15 documentation gives "between -5 and 1024".[^3] Code that depends on the range is wrong in all versions.

## Pitfalls

```python wrong id=string-identity
prefix = "user"
name = prefix + "_1"
key = "user_1"

print(name == key, name is key)
```

```output for=string-identity
True False
```

The two strings are equal, but `name` was built at run time, so it is a new object. A check such as `if name is key:` would fail (rule r4).

```python wrong id=custom-eq
class Matrix:
    def __init__(self, rows):
        self.rows = rows

    def __eq__(self, other):
        return [r == other for r in self.rows]


m = Matrix([1, None])
print(m == None)
print(bool(m == None), m is None)
```

```output for=custom-eq
[False, True]
True False
```

A class can return anything from `__eq__`, so `m == None` is a list here, and the list is true. `m is None` gives the right answer (rule r3).

## Comparison

Many languages have the same split between a reference test and a value test, but they spell it in different ways. In Python, `==` is the value test and `is` is the reference test. For identity and the caches of CPython, see [[python/object-identity-and-interning]]. For `==` and the other comparisons, see [[python/comparison-operators]].
