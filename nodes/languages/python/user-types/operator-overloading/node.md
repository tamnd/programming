---
tag: 2KT3HK
kind: feature
owner: python
slug: operator-overloading
name: Operator overloading
aliases: [__add__, __radd__, __iadd__, __mul__, __neg__, __abs__, __index__, reflected operands, emulating numeric types, arithmetic operators on objects]
area: languages/python/user-types
level: core
summary: >-
  A class defines __add__, __mul__ and the other numeric methods to support +, * and the rest. The reflected methods such as __radd__ handle the swapped case, and __iadd__ handles +=.
grammar: null
spec: {standard: null, url: "https://docs.python.org/3.15/reference/datamodel.html#emulating-numeric-types", locator: "3.3.8. Emulating numeric types"}
see_also: [python/special-methods, python/rich-comparison-methods, python/classes]
sources:
  - url: https://docs.python.org/3.15/reference/datamodel.html#emulating-numeric-types
    title: The Python Language Reference, 3. Data model
    locator: 3.3.8. Emulating numeric types
    accessed: 2026-11-16
  - url: https://go.dev/doc/faq#overloading
    title: Frequently Asked Questions (FAQ), The Go Programming Language
    locator: Why does Go not support overloading of methods and operators?
    accessed: 2026-11-16
---

## Synopsis

```python run id=basics
class Vector:
    def __init__(self, x, y):
        self.x, self.y = x, y

    def __repr__(self):
        return f"Vector({self.x}, {self.y})"

    def __add__(self, other):
        if not isinstance(other, Vector):
            return NotImplemented
        return Vector(self.x + other.x, self.y + other.y)

    def __mul__(self, k):
        if not isinstance(k, (int, float)):
            return NotImplemented
        return Vector(self.x * k, self.y * k)

    __rmul__ = __mul__

    def __neg__(self):
        return Vector(-self.x, -self.y)

    def __abs__(self):
        return (self.x ** 2 + self.y ** 2) ** 0.5

v = Vector(3, 4)
print(v + Vector(1, 1), v * 2, 2 * v, -v, abs(v))
```

```output for=basics
Vector(4, 5) Vector(6, 8) Vector(6, 8) Vector(-3, -4) 5.0
```

`v * 2` calls `__mul__`. In `2 * v`, int cannot multiply by a Vector, so Python calls `__rmul__` of the Vector.

## Semantics

**r1.** "The following methods can be defined to emulate numeric objects. Methods corresponding to operations that are not supported by the particular kind of number implemented (e.g., bitwise operations for non-integral numbers) should be left undefined."[^1]

**r2.** The binary methods implement `+`, `-`, `*`, `@`, `/`, `//`, `%`, divmod(), pow(), `**`, `<<`, `>>`, `&`, `^` and `|`.[^1] "For instance, to evaluate the expression x + y, where x is an instance of a class that has an __add__() method, type(x).__add__(x, y) is called."[^1] "If one of those methods does not support the operation with the supplied arguments, it should return NotImplemented."[^1]

**r3.** The reflected methods, such as `__radd__`, are for "swapped" operands. "These functions are only called if the operands are of different types, when the left operand does not support the corresponding operation".[^1] The right operand's method also goes first if its class is derived from the class of the left operand.[^1] Example: [`no-rmul`](#no-rmul).

**r4.** The in-place methods, such as `__iadd__`, "should attempt to do the operation in-place (modifying self) and return the result (which could be, but does not have to be, self)."[^1] "x += y is equivalent to x = x.__iadd__(y)".[^1] "If __iadd__() does not exist, or if x.__iadd__(y) returns NotImplemented, x.__add__(y) and y.__radd__(x) are considered, as with the evaluation of x + y."[^1] Examples: [`iadd`](#iadd), [`iadd-none`](#iadd-none).

**r5.** `__neg__` and the others are "Called to implement the unary arithmetic operations (-, +, abs() and ~)."[^1]

**r6.** `__index__` is called "whenever Python needs to losslessly convert the numeric object to an integer object (such as in slicing, or in the built-in bin(), hex() and oct() functions)."[^1] "If __int__(), __float__() and __complex__() are not defined then corresponding built-in functions int(), float() and complex() fall back to __index__()."[^1] Example: [`index`](#index).

## Examples

```python run id=iadd
class Bag:
    def __init__(self):
        self.items = []

    def __iadd__(self, item):
        self.items.append(item)
        return self

class Money:
    def __init__(self, cents):
        self.cents = cents

    def __add__(self, other):
        return Money(self.cents + other.cents)

b = Bag()
alias = b
b += "apple"
b += "pear"
print(alias.items, alias is b)

m = Money(100)
old = m
m += Money(50)
print(m.cents, old.cents, m is old)
```

```output for=iadd
['apple', 'pear'] True
150 100 False
```

Bag changes itself in `__iadd__`, so the alias sees the change. Money has no `__iadd__`, so `+=` uses `__add__` and binds a new object to `m` (rule r4).

```python run id=index
class Digit:
    def __init__(self, n):
        self.n = n

    def __index__(self):
        return self.n

letters = "abcdef"
print(letters[Digit(2)], bin(Digit(5)), int(Digit(3)))
```

```output for=index
c 0b101 3
```

## Pitfalls

Without a reflected method, the operation fails when the object is on the right:

```python fails id=no-rmul expect=exception:TypeError
class Scale:
    def __init__(self, k):
        self.k = k

    def __mul__(self, n):
        return Scale(self.k * n)

2 * Scale(3)
```

Define `__rmul__` too, as in the Vector class (rule r3).

An in-place method must return the result. If it returns None, the name gets None:

```python wrong id=iadd-none
class Bag:
    def __init__(self):
        self.items = []

    def __iadd__(self, item):
        self.items.append(item)

b = Bag()
b += "apple"
print(b)
```

```output for=iadd-none
None
```

Python assigns the return value of `__iadd__` to `b` (rule r4). Add `return self`.

## Comparison

Go has no operator overloading. The Go FAQ says: "Regarding operator overloading, it seems more a convenience than an absolute requirement. Again, things are simpler without it."[^2] A Go vector type uses named methods, such as `v.Add(w)`. In Python the same class can support `v + w` (rule r2).
