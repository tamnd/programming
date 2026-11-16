---
tag: PG2FN0
kind: feature
owner: python
slug: properties
name: Properties
aliases: [property, property decorator, getter, setter, deleter, managed attribute, computed attribute, cached_property]
area: languages/python/user-types
level: core
summary: >-
  A property makes an attribute that runs methods. Reading it calls the getter, assigning calls the setter, and del calls the deleter. Without a setter the attribute is read-only.
grammar: null
spec: {standard: null, url: "https://docs.python.org/3.15/builtins/functions.html#property", locator: "Built-in Functions, property"}
see_also: [python/descriptors, python/attribute-access-customization, python/private-variables, python/slots]
sources:
  - url: https://docs.python.org/3.15/builtins/functions.html#property
    title: Built-in Functions
    locator: class property; getter, setter, deleter
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/library/functools.html#functools.cached_property
    title: functools, Higher-order functions and operations on callable objects
    locator: "@functools.cached_property"
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/reference/datamodel.html#invoking-descriptors
    title: The Python Language Reference, 3. Data model
    locator: 3.3.2.3. Invoking Descriptors
    accessed: 2026-11-16
  - url: https://go.dev/doc/effective_go#Getters
    title: Effective Go
    locator: Getters
    accessed: 2026-11-16
---

## Synopsis

```python run id=basics
class Temperature:
    def __init__(self, celsius):
        self.celsius = celsius

    @property
    def celsius(self):
        return self._celsius

    @celsius.setter
    def celsius(self, value):
        if value < -273.15:
            raise ValueError("below absolute zero")
        self._celsius = value

    @property
    def fahrenheit(self):
        return self._celsius * 9 / 5 + 32

t = Temperature(25)
print(t.celsius, t.fahrenheit)
t.celsius = 100
print(t.fahrenheit)
try:
    t.celsius = -300
except ValueError as e:
    print(e)
```

```output for=basics
25 77.0
212.0
below absolute zero
```

The caller uses plain attribute syntax. The setter checks every assignment, and the `__init__` assignment also goes through it. `fahrenheit` has no setter, so it is read-only.

## Semantics

**r1.** `property(fget=None, fset=None, fdel=None, doc=None)` returns "a property attribute."[^1] "If c is an instance of C, c.x will invoke the getter, c.x = value will invoke the setter, and del c.x the deleter."[^1]

**r2.** "The @property decorator turns the voltage() method into a “getter” for a read-only attribute with the same name".[^1] Example: [`read-only`](#read-only).

**r3.** "A property object has getter, setter, and deleter methods usable as decorators that create a copy of the property with the corresponding accessor function set to the decorated function."[^1] "Be sure to give the additional functions the same name as the original property".[^1] Example: [`other-name`](#other-name).

**r4.** "The @property decorator is implemented as a data descriptor. Accordingly, instances cannot override the behavior of a property."[^3] Example: [`no-override`](#no-override).

**r5.** `functools.cached_property` turns "a method of a class into a property whose value is computed once and then cached as a normal attribute for the life of the instance."[^2] "The cached value can be cleared by deleting the attribute. This allows the cached_property method to run again."[^2] Example: [`cached`](#cached).

**r6.** "A regular property blocks attribute writes unless a setter is defined. In contrast, a cached_property allows writes."[^2] A cached_property "will not work with" classes "that specify __slots__ without including __dict__ as one of the defined slots".[^2]

## Examples

```python run id=no-override
class Box:
    @property
    def size(self):
        return 3

b = Box()
b.__dict__["size"] = 99
print(b.size, b.__dict__)
```

```output for=no-override
3 {'size': 99}
```

The instance dictionary has a `size` entry, but the property still wins (rule r4).

```python run id=cached
from functools import cached_property

class Report:
    def __init__(self, rows):
        self.rows = rows

    @cached_property
    def total(self):
        print("summing")
        return sum(self.rows)

r = Report([1, 2, 3])
print(r.total)
print(r.total)
del r.total
r.rows.append(4)
print(r.total)
```

```output for=cached
summing
6
6
summing
10
```

The second read uses the cached value. After `del r.total` the method runs again (rule r5).

```python run id=deleter
class Account:
    def __init__(self):
        self._token = "abc"

    @property
    def token(self):
        return self._token

    @token.deleter
    def token(self):
        print("revoking")
        del self._token

a = Account()
del a.token
print(hasattr(a, "token"))
```

```output for=deleter
revoking
False
```

`del a.token` calls the deleter. After that the getter raises AttributeError, so `hasattr` is False.

## Pitfalls

A property without a setter refuses assignment:

```python fails id=read-only expect=exception:AttributeError
class Circle:
    def __init__(self, r):
        self.r = r

    @property
    def area(self):
        return 3.14159 * self.r ** 2

c = Circle(2)
c.area = 10
```

Add a setter, or assign to the attribute that the getter reads (rule r2).

A setter with a different name makes a second property and leaves the first one read-only:

```python wrong id=other-name
class C:
    @property
    def x(self):
        return self._x

    @x.setter
    def set_x(self, value):
        self._x = value

c = C()
try:
    c.x = 1
except AttributeError as e:
    print(type(e).__name__)
print(C.x.fset, C.set_x.fset is not None)
```

```output for=other-name
AttributeError
None True
```

`x.setter` returns a copy. The copy is bound to `set_x`, and `x` keeps the old property with no setter (rule r3). Name the setter `x`.

A getter that reads its own name calls itself:

```python fails id=recursion expect=exception:RecursionError
class Name:
    @property
    def value(self):
        return self.value

Name().value
```

Store the data under another name, such as `self._value`.

## Comparison

"Go doesn't provide automatic support for getters and setters."[^4] Effective Go says that for a field called `owner`, "the getter method should be called Owner (upper case, exported), not GetOwner."[^4] "A setter function, if needed, will likely be called SetOwner."[^4] The Go caller writes `obj.Owner()`, so a reader can see the method call. In Python, `t.celsius = 100` looks like a plain assignment but runs the setter (rule r1).
