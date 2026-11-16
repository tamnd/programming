---
tag: HAW99E
kind: feature
owner: python
slug: object-identity-and-interning
name: Object identity and interning
aliases: [is operator, id(), is vs ==, sys.intern, string interning, small int cache, sentinel object]
area: languages/python/memory
level: core
summary: >-
  Each object has an identity that is fixed for its lifetime. is compares identities, and == compares values. CPython reuses some immutable objects, so is on ints and strs gives results that vary.
grammar: null
spec: {standard: null, url: "https://docs.python.org/3.15/reference/datamodel.html#objects-values-and-types", locator: "3.1. Objects, values and types"}
see_also: [python/memory-management-in-cpython, python/reference-counting-in-cpython, python/weak-references]
sources:
- url: https://docs.python.org/3.15/reference/datamodel.html#objects-values-and-types
  title: The Python Language Reference, 3. Data model
  locator: 3.1. Objects, values and types; 3.3.1. Basic customization, object.__eq__
  accessed: 2026-11-16
- url: https://docs.python.org/3.15/reference/expressions.html#is-not
  title: The Python Language Reference, 6. Expressions
  locator: 6.10.3. Identity comparisons
  accessed: 2026-11-16
- url: https://docs.python.org/3.15/library/functions.html#id
  title: Built-in Functions
  locator: id()
  accessed: 2026-11-16
- url: https://docs.python.org/3.14/c-api/long.html#c.PyLong_FromLong
  title: Integer Objects, The Python/C API Reference Manual (3.14)
  locator: PyLong_FromLong
  accessed: 2026-11-16
- url: https://docs.python.org/3.15/library/sys.html#sys.intern
  title: sys, System-specific parameters and functions
  locator: sys.intern
  accessed: 2026-11-16
- url: https://docs.python.org/3.15/faq/programming.html#when-can-i-rely-on-identity-tests-with-the-is-operator
  title: Programming FAQ
  locator: Why does the result of id() appear to be not unique?; When can I rely on identity tests with the is operator?
  accessed: 2026-11-16
- url: https://docs.python.org/3.15/c-api/long.html#c.PyLong_FromLong
  title: Integer Objects, The Python/C API Reference Manual (3.15)
  locator: PyLong_FromLong
  accessed: 2026-11-16
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/lang/Object.html#equals(java.lang.Object)
  title: Class Object, Java SE 25 API
  locator: equals(Object)
  accessed: 2026-11-16
- url: https://docs.oracle.com/javase/specs/jls/se25/html/jls-5.html#jls-5.1.7
  title: The Java Language Specification, Java SE 25 Edition, Chapter 5
  locator: 5.1.7. Boxing Conversion
  accessed: 2026-11-16
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/lang/Integer.html#valueOf(int)
  title: Class Integer, Java SE 25 API
  locator: valueOf(int)
  accessed: 2026-11-16
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/lang/String.html#intern()
  title: Class String, Java SE 25 API
  locator: intern()
  accessed: 2026-11-16
- url: https://pkg.go.dev/unique
  title: Package unique, The Go Programming Language
  locator: the overview; type Handle
  accessed: 2026-11-16
---

## Synopsis

```text
x is y          # True if x and y are the same object
x is not y
id(x)           # an int, unique among the objects alive now
x == y          # True if the values are equal (calls __eq__)

import sys
s = sys.intern(s)   # one shared copy of an equal str
```

## Semantics

**r1.** "Every object has an identity, a type and a value. An object’s identity never changes once it has been created; you may think of it as the object’s address in memory."[^1] "The is operator compares the identity of two objects; the id() function returns an integer representing its identity."[^1]

**r2.** `x is y` "is true if and only if x and y are the same object."[^2] `==` compares values, and a class can define what equal values are. Two equal lists can be two objects. Example: [`is`](#is).

**r3.** `id()` returns an integer "which is guaranteed to be unique and constant for this object during its lifetime. Two objects with non-overlapping lifetimes may have the same id() value."[^3] On CPython "This is the address of the object in memory."[^3] So an id is a good key only while the object is alive. Example: [`reuse`](#reuse).

**r4.** The language lets an implementation reuse immutable objects: "for immutable types, operations that compute new values may actually return a reference to any existing object with the same type and value, while for mutable objects this is not allowed."[^1] "after a = 1; b = 1, a and b may or may not refer to the same object with the value one, depending on the implementation."[^1] But "after c = []; d = [], c and d are guaranteed to refer to two different, unique, newly created empty lists."[^1]

**r5.** CPython keeps a cache of small ints. The 3.14 C API reference says that CPython "keeps an array of integer objects for all integers between -5 and 256. When you create an int in that range you actually just get back a reference to the existing object."[^4] The range is not the same in all versions (see Versions). Example: [`small`](#small).

**r6.** `sys.intern()` puts a string in a table and returns the shared copy, so two equal interned strings are one object. It is a speed tool: "the key comparisons (after hashing) can be done by a pointer compare instead of a string compare."[^5] "Normally, the names used in Python programs are automatically interned".[^5] "Interned strings are not immortal; you must keep a reference to the return value of intern() around to benefit from it."[^5] Example: [`intern`](#intern).

**r7.** The FAQ gives the cases where identity is sure.[^6] "After the assignment new = old, it is guaranteed that new is old." "After the list assignment s[0] = x, it is guaranteed that s[0] is x." "After the assignments a = None and b = None, it is guaranteed that a is b because None is a singleton." "In most other circumstances, identity tests are inadvisable and equality tests are preferred."[^6]

## Examples

Equal values and the same object (rule r2). `c = a` makes a second name, not a second list.

```python run id=is
a = [1, 2]
b = [1, 2]
c = a
print(a == b, a is b)
print(a is c, id(a) == id(c))
```

```output for=is
True False
True True
```

Two lists with lifetimes that do not overlap. The first list is freed before the second is made, and the second gets the same address on CPython (rule r3). The FAQ shows the same effect with `id(1000)` and `id(2000)`.[^6]

```python run id=reuse
print(id([]) == id([]))
```

```output for=reuse
True
```

The small int cache of CPython (rule r5). `int("...")` makes the int at run time, so the result does not come from a constant in the code.

```python run id=small
print(int("10") is int("10"))
print(int("-5") is int("-5"))
print(int("5000") is int("5000"))
```

```output for=small
True
True
False
```

Two strings made at run time are equal and are two objects. After `sys.intern()`, both names refer to one object (rule r6).

```python run id=intern
import sys

a = "".join(["hello", " world"])
b = "".join(["hello", " world"])
print(a == b, a is b)
a = sys.intern(a)
b = sys.intern(b)
print(a == b, a is b)
```

```output for=intern
True False
True True
```

A sentinel object. `None` is a valid value in the dict, so the function cannot use `None` to mean that the caller gave no default. A new `object()` is equal only to itself, because "By default, object implements __eq__() by using is".[^1] The FAQ shows this pattern for a method like `dict.pop()`.[^6]

```python run id=sentinel
_missing = object()


def get(mapping, key, default=_missing):
    if key in mapping:
        return mapping[key]
    if default is _missing:
        raise KeyError(key)
    return default


data = {"a": None}
print(get(data, "a"))
print(get(data, "b", None))
try:
    get(data, "b")
except KeyError as err:
    print("KeyError", err)
```

```output for=sentinel
None
None
KeyError 'b'
```

Containers test identity first. "This prevents the code from being confused by objects such as float('NaN') that are not equal to themselves."[^6] A NaN is not equal to itself, but it is found in a list that holds it.

```python run id=nan
x = float("nan")
print(x == x)
print(x in [x])
print([x] == [x])
```

```output for=nan
False
True
True
```

The examples give the same output on CPython 3.12.3, 3.13.14 and 3.14.6.

## Versions

The range of the small int cache changed. The 3.14 C API reference gives "all integers between -5 and 256".[^4] The 3.15 C API reference gives "all integers between -5 and 1024".[^7] Code that depends on either range is wrong (see Pitfalls).

## Pitfalls

### `is` to compare numbers or strings

The result depends on the cache and on how the value was made (rules r4 and r5). The function below is right for small ints and wrong for other ints.

```python wrong id=same
def same(a, b):
    return a is b


print(same(int("10"), int("10")))
print(same(int("5000"), int("5000")))
```

```output for=same
True
False
```

The FAQ says that "identity tests should not be used to check constants such as int and str which aren’t guaranteed to be singletons".[^6] Use `==`.

### `==` to compare with `None`

A class can define `__eq__` to return `True` for any value, so `x == None` can be `True` for an object that is not `None`. "As recommended by PEP 8, an identity test is the preferred way to check for None."[^6]

```python wrong id=none lint="E711"
class AlwaysEqual:
    def __eq__(self, other):
        return True


x = AlwaysEqual()
print(x == None)
print(x is None)
```

```output for=none
True
False
```

### An `id()` that is kept after the object is gone

An id is unique only among objects that are alive at the same time (rule r3). A dict keyed by `id(obj)` can give the data of a dead object to a new object at the same address. Keep a reference to the object as long as the key is in use, or use a [`WeakKeyDictionary`](/python/weak-references/).

## Comparison

**Java** uses `==` on references for identity. A class overrides `equals()` for values; the default `Object.equals` returns true "if and only if x and y refer to the same object (x == y has the value true)."[^8] So Java `==` is like Python `is`, and `equals()` is like Python `==`. Java also caches small boxed integers, and the language requires it: for "an integer in the range -128 to 127 inclusive", two boxing conversions of the same constant give the same object.[^9] `Integer.valueOf` "will always cache values in the range -128 to 127, inclusive, and may cache other values outside of this range."[^10] `String.intern()` is like `sys.intern()`: "for any two strings s and t, s.intern() == t.intern() is true if and only if s.equals(t) is true. All literal strings and string-valued constant expressions are interned."[^11]

**Go** has the `unique` package, which provides "facilities for canonicalizing ("interning") comparable values."[^12] `unique.Make` returns a `Handle`, and "The comparison of two handles is trivial and typically much more efficient than comparing the values used to create them."[^12] It does the same work as `sys.intern()`, for any comparable type and not only for strings.

## Implementation notes

On CPython, `id(x)` "is the memory address where x is stored."[^1] The rule of r3 is part of the language, and the address is a detail of CPython. The small int cache (rule r5) and the interning of names (rule r6) are details of CPython, so a program must not depend on them.

## References
