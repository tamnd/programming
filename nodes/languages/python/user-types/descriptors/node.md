---
tag: AA8Y3X
kind: feature
owner: python
slug: descriptors
name: Descriptors
aliases: [descriptor protocol, __get__, __set__, __delete__, __set_name__, data descriptor, non-data descriptor, managed attribute]
area: languages/python/user-types
level: advanced
summary: >-
  A descriptor is an object with __get__, __set__ or __delete__, stored as a class attribute. The dot operator calls these methods, so the descriptor controls the attribute on every instance.
grammar: null
spec: {standard: null, url: "https://docs.python.org/3.15/reference/datamodel.html#implementing-descriptors", locator: "3.3.2.2. Implementing Descriptors; 3.3.2.3. Invoking Descriptors"}
see_also: [python/properties, python/method-objects, python/attribute-access-customization, python/class-and-instance-variables]
sources:
  - url: https://docs.python.org/3.15/reference/datamodel.html#implementing-descriptors
    title: The Python Language Reference, 3. Data model
    locator: 3.3.2.2. Implementing Descriptors; 3.3.2.3. Invoking Descriptors; object.__set_name__()
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/howto/descriptor.html#closing-thoughts
    title: Descriptor Guide
    locator: Primer, Closing thoughts; Descriptor protocol
    accessed: 2026-11-16
  - url: https://go.dev/doc/effective_go#Getters
    title: Effective Go
    locator: Getters
    accessed: 2026-11-16
---

## Synopsis

```python run id=basics
class Positive:
    def __set_name__(self, owner, name):
        self.private_name = "_" + name

    def __get__(self, obj, objtype=None):
        if obj is None:
            return self
        return getattr(obj, self.private_name)

    def __set__(self, obj, value):
        if value <= 0:
            raise ValueError(f"{self.private_name[1:]} must be positive")
        setattr(obj, self.private_name, value)

class Order:
    quantity = Positive()
    price = Positive()

    def __init__(self, quantity, price):
        self.quantity = quantity
        self.price = price

o = Order(3, 2.5)
print(o.quantity * o.price, vars(o))
try:
    o.quantity = 0
except ValueError as e:
    print(e)
print(type(Order.quantity).__name__)
```

```output for=basics
7.5 {'_quantity': 3, '_price': 2.5}
quantity must be positive
Positive
```

One Positive class checks two attributes. `__set_name__` tells each descriptor its name, and the value goes in the instance under a second name.

## Semantics

**r1.** "A descriptor is what we call any object that defines __get__(), __set__(), or __delete__()."[^2] "Descriptors only work when used as class variables. When put in instances, they have no effect."[^2] Example: [`in-instance`](#in-instance).

**r2.** `__get__` is "Called to get the attribute of the owner class (class attribute access) or of an instance of that class (instance attribute access)."[^1] The instance argument "is the instance that the attribute was accessed through, or None when the attribute is accessed through the owner."[^1] Example: [`binding`](#binding).

**r3.** For an instance, "a.x is transformed into the call: type(a).__dict__['x'].__get__(a, type(a))."[^1] For a class, "A.x is transformed into the call: A.__dict__['x'].__get__(None, A)."[^1]

**r4.** "If the descriptor defines __set__() and/or __delete__(), it is a data descriptor; if it defines neither, it is a non-data descriptor."[^1] "If an instance’s dictionary has an entry with the same name as a data descriptor, the data descriptor takes precedence. If an instance’s dictionary has an entry with the same name as a non-data descriptor, the dictionary entry takes precedence."[^2] Example: [`precedence`](#precedence).

**r5.** `__set_name__` is "Automatically called at the time the owning class owner is created."[^1] "If the class variable is assigned after the class is created, __set_name__() will not be called automatically."[^1]

**r6.** "Python methods (including those decorated with @staticmethod and @classmethod) are implemented as non-data descriptors."[^1] "The @property decorator is implemented as a data descriptor. Accordingly, instances cannot override the behavior of a property."[^1] Example: [`function`](#function).

## Examples

```python run id=precedence
class Ten:
    def __get__(self, obj, objtype=None):
        return 10

class Locked:
    def __get__(self, obj, objtype=None):
        return "locked"

    def __set__(self, obj, value):
        raise AttributeError("read-only")

class A:
    ten = Ten()
    lock = Locked()

a = A()
a.__dict__["ten"] = 99
a.__dict__["lock"] = 99
print(a.ten, a.lock)
```

```output for=precedence
99 locked
```

Ten is a non-data descriptor, so the instance dictionary wins. Locked is a data descriptor, so it wins (rule r4).

```python run id=binding
class Show:
    def __get__(self, obj, objtype=None):
        return (obj, objtype.__name__)

class A:
    x = Show()

a = A()
print(A.x, a.x[0] is a, a.x[1])
```

```output for=binding
(None, 'A') True A
```

Through the class, `obj` is None. Through the instance, `obj` is the instance (rules r2 and r3).

```python run id=function
def f(self):
    return "hi"

class C:
    pass

c = C()
bound = f.__get__(c, C)
print(bound(), bound.__self__ is c, hasattr(f, "__set__"))
```

```output for=function
hi True False
```

A function has `__get__` but not `__set__`, so it is a non-data descriptor. Its `__get__` makes a bound method (rule r6).

## Pitfalls

A descriptor stored on an instance is an ordinary value:

```python wrong id=in-instance
class Ten:
    def __get__(self, obj, objtype=None):
        return 10

class A:
    def __init__(self):
        self.y = Ten()

print(type(A().y).__name__)
```

```output for=in-instance
Ten
```

Put the descriptor in the class body (rule r1).

A descriptor that keeps the value on itself shares the value between all instances, because the class has only one descriptor object:

```python wrong id=shared
class Shared:
    def __get__(self, obj, objtype=None):
        return self.value

    def __set__(self, obj, value):
        self.value = value

class P:
    x = Shared()

p, q = P(), P()
p.x = 1
q.x = 2
print(p.x, q.x)
```

```output for=shared
2 2
```

Keep the value on `obj`, as Positive does in the synopsis.

## Comparison

"Go doesn't provide automatic support for getters and setters."[^3] Effective Go says: "There's nothing wrong with providing getters and setters yourself".[^3] The caller then writes `obj.Owner()` and `obj.SetOwner(user)`. In Python a descriptor runs code on plain attribute syntax such as `o.quantity = 0` (rule r3).
