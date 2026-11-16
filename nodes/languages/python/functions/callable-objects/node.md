---
tag: GEQPXT
kind: feature
owner: python
slug: callable-objects
name: Callable objects and __call__
aliases: [callable, __call__, callable(), function object with state, call an instance, functor in Python, object is not callable]
area: languages/python/functions
level: core
summary: >-
  A callable is an object that a call can call. Functions, methods and classes are callables, and an instance is a callable when its class defines __call__. The built-in callable() tests this.
grammar: null
spec: {standard: null, url: "https://docs.python.org/3.15/reference/datamodel.html#object.__call__", locator: "3.3.6. Emulating callable objects"}
see_also: [python/function-objects, python/call-semantics, python/closures-and-nonlocal, python/functools]
sources:
  - url: https://docs.python.org/3.15/reference/datamodel.html#object.__call__
    title: The Python Language Reference, Python 3.15
    locator: 3.3.6. Emulating callable objects
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/glossary.html#term-callable
    title: Glossary, Python 3.15
    locator: callable
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/builtins/functions.html#callable
    title: Built-in Functions, Python 3.15 documentation
    locator: callable()
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/reference/expressions.html#calls
    title: The Python Language Reference, Python 3.15
    locator: 6.3.3. Calls
    accessed: 2026-11-16
  - url: https://go.dev/ref/spec#Method_values
    title: The Go Programming Language Specification
    locator: Method values
    accessed: 2026-11-16
---

## Synopsis

```python run id=counter
class Counter:
    def __init__(self):
        self.calls = 0

    def __call__(self, text):
        self.calls += 1
        return text.upper()

shout = Counter()
print(shout("hi"), shout("there"))
print(shout.calls, callable(shout))
```

```output for=counter
HI THERE
2 True
```

`shout` is an instance, but the class defines `__call__`, so a program can call it like a function. It keeps its count in an attribute.

## Semantics

**r1.** "A callable is an object that can be called, possibly with a set of arguments".[^2] "A function, and by extension a method, is a callable. An instance of a class that implements the __call__() method is also a callable."[^2]

**r2.** `__call__` is "Called when the instance is “called” as a function; if this method is defined, x(arg1, arg2, ...) roughly translates to type(x).__call__(x, arg1, ...)."[^1] Example: [`counter`](#counter).

**r3.** "The object class itself does not provide this method."[^1] Example: [`not-callable`](#not-callable).

**r4.** The primary of a call "must evaluate to a callable object (user-defined functions, built-in functions, methods of built-in objects, class objects, methods of class instances, and all objects having a __call__() method are callable)."[^4]

**r5.** callable() returns "True if the object argument appears callable, False if not. If this returns True, it is still possible that a call fails, but if it is False, calling object will never succeed. Note that classes are callable (calling a class returns a new instance); instances are callable if their class has a __call__() method."[^3] Example: [`kinds`](#kinds).

## Examples

```python run id=kinds
class Plain:
    pass

class WithCall:
    def __call__(self):
        return "called"

for obj in [len, print, Plain, Plain(), WithCall(), "text".upper, 42]:
    print(type(obj).__name__, callable(obj))
```

```output for=kinds
builtin_function_or_method True
builtin_function_or_method True
type True
Plain False
WithCall True
builtin_function_or_method True
int False
```

A class is callable, but an instance of `Plain` is not, because `Plain` has no `__call__` (rule r5).

```python run id=type-lookup
class Greeter:
    def __call__(self):
        return "class method"

g = Greeter()
g.__call__ = lambda: "instance attribute"
print(g())
```

```output for=type-lookup
class method
```

A call uses `type(x).__call__`, so an attribute `__call__` on the instance has no effect (rule r2).

## Pitfalls

A call of an object whose type has no `__call__` is a TypeError:

```python fails id=not-callable expect=exception:TypeError
count = 3
count()
```

A name that is bound to a value can hide a built-in function. After `sum = 10`, the call `sum([1, 2])` raises the same TypeError, because `sum` is now an `int`.

## Comparison

Go has no `__call__` method. A Go method value can stand in for a callable object: "If the expression x has static type T and M is in the method set of type T, x.M is called a method value. The method value x.M is a function value that is callable with the same arguments as a method call of x.M."[^5] So a Go program can pass `c.Count` as a function, and the method keeps its receiver `c`. In Python, the object itself is the callable (rule r2).
