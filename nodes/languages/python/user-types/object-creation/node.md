---
tag: R76J3E
kind: feature
owner: python
slug: object-creation
name: Object creation with __new__ and __init__
aliases: [__new__, __init__, constructor, initializer, alternative constructor, subclass an immutable type, factory classmethod]
area: languages/python/user-types
level: advanced
summary: >-
  A call to a class runs __new__ to make the object and then __init__ to set it up. Override __new__ to subclass an immutable type. A classmethod gives an alternative constructor.
grammar: null
spec: {standard: null, url: "https://docs.python.org/3.15/reference/datamodel.html#object.__new__", locator: "3.3.1. Basic customization, object.__new__()"}
see_also: [python/class-objects, python/inheritance, python/classes]
sources:
  - url: https://docs.python.org/3.15/reference/datamodel.html#object.__new__
    title: The Python Language Reference, 3. Data model
    locator: 3.3.1. Basic customization, object.__new__() and object.__init__()
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/builtins/functions.html#classmethod
    title: Built-in Functions, Python 3.15 documentation
    locator: classmethod
    accessed: 2026-11-16
  - url: https://go.dev/doc/effective_go#allocation_new
    title: Effective Go
    locator: Allocation with new; Constructors and composite literals
    accessed: 2026-11-16
---

## Synopsis

```python run id=basics
class Traced:
    def __new__(cls, *args):
        print("__new__", cls.__name__, args)
        return super().__new__(cls)

    def __init__(self, value):
        print("__init__", value)
        self.value = value

t = Traced(7)
print(t.value)
```

```output for=basics
__new__ Traced (7,)
__init__ 7
7
```

`__new__` gets the class and makes the object. `__init__` gets the new object and the same arguments.

## Semantics

**r1.** `__new__` is "Called to create a new instance of class cls. __new__() is a static method (special-cased so you need not declare it as such) that takes the class of which an instance was requested as its first argument."[^1] "The return value of __new__() should be the new object instance (usually an instance of cls)."[^1]

**r2.** "Typical implementations create a new instance of the class by invoking the superclass’s __new__() method using super().__new__(cls[, ...]) with appropriate arguments and then modifying the newly created instance as necessary before returning it."[^1]

**r3.** "If __new__() is invoked during object construction and it returns an instance of cls, then the new instance’s __init__() method will be invoked".[^1] "If __new__() does not return an instance of cls, then the new instance’s __init__() method will not be invoked."[^1] Examples: [`not-instance`](#not-instance), [`cache`](#cache).

**r4.** `__init__` is "Called after the instance has been created (by __new__()), but before it is returned to the caller."[^1] "no non-None value may be returned by __init__(); doing so will cause a TypeError to be raised at runtime."[^1]

**r5.** "__new__() is intended mainly to allow subclasses of immutable types (like int, str, or tuple) to customize instance creation."[^1] Examples: [`immutable`](#immutable), [`init-immutable`](#init-immutable).

**r6.** "A class method receives the class as an implicit first argument, just like an instance method receives the instance."[^2] "If a class method is called for a derived class, the derived class object is passed as the implied first argument."[^2] So a classmethod that calls `cls(...)` is a constructor that also works for subclasses. Example: [`alt-constructor`](#alt-constructor).

## Examples

```python run id=immutable
class Rounded(float):
    def __new__(cls, value, digits=2):
        return super().__new__(cls, round(value, digits))

r = Rounded(3.14159)
print(r, r * 2, isinstance(r, float))
```

```output for=immutable
3.14 6.28 True
```

The value of a float is fixed when `float.__new__` makes it, so the change must be in `__new__` (rule r5).

```python run id=alt-constructor
class Date:
    def __init__(self, year, month, day):
        self.year, self.month, self.day = year, month, day

    @classmethod
    def from_iso(cls, text):
        year, month, day = map(int, text.split("-"))
        return cls(year, month, day)

    def __repr__(self):
        return f"{type(self).__name__}({self.year}, {self.month}, {self.day})"

class Holiday(Date):
    pass

print(Date.from_iso("2026-11-16"), Holiday.from_iso("2026-12-25"))
```

```output for=alt-constructor
Date(2026, 11, 16) Holiday(2026, 12, 25)
```

`Holiday.from_iso` gets Holiday as `cls`, so it makes a Holiday (rule r6).

```python run id=not-instance
class Weird:
    def __new__(cls):
        return 42

    def __init__(self):
        print("never printed")

print(Weird())
```

```output for=not-instance
42
```

`__new__` returns an int, not a Weird, so `__init__` does not run (rule r3).

## Pitfalls

`__init__` cannot change the value of an immutable type. The arguments go to `float.__new__` first:

```python fails id=init-immutable expect=exception:TypeError
class Rounded(float):
    def __init__(self, value, digits):
        super().__init__()

Rounded(3.14159, 2)
```

`float.__new__` does not accept two arguments. Override `__new__` instead (rule r5).

If `__new__` returns an object that already exists, `__init__` runs on it again:

```python wrong id=cache
class Cached:
    _cache = {}

    def __new__(cls, key):
        if key not in cls._cache:
            cls._cache[key] = super().__new__(cls)
        return cls._cache[key]

    def __init__(self, key):
        print("init", key)
        self.key = key

a = Cached("x")
b = Cached("x")
print(a is b)
```

```output for=cache
init x
init x
True
```

The cached object is an instance of `cls`, so Python calls `__init__` each time (rule r3). Put all the setup in `__new__`, or use a classmethod that looks in the cache.

## Comparison

Go separates allocation from setup in the same way, but setup is not automatic. "new(T) allocates zeroed storage for a new variable of type T and returns its address, a value of type *T."[^3] Go "does not initialize the memory, it only zeros it."[^3] A Go constructor is an ordinary function, such as `NewFile`. In Python a call to the class runs both steps (rules r1 and r4).
