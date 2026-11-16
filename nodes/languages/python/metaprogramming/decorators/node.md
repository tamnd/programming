---
tag: FB7QCN
kind: feature
owner: python
slug: decorators
name: Decorators
aliases: [decorator, "@ syntax", function decorator, class decorator, decorator factory, functools.wraps, wraps, update_wrapper, __wrapped__, PEP 318, PEP 3129, PEP 614]
area: languages/python/metaprogramming
level: core
summary: >-
  A decorator is an expression after @ above a def or class. Python calls it with the new function or class and binds the result to the name. functools.wraps copies the name and docstring to a wrapper.
grammar: |
  decorators: decorator+
  decorator: "@" assignment_expression NEWLINE
spec: {standard: null, url: "https://docs.python.org/3.15/reference/compound_stmts.html#function-definitions", locator: "8.7 Function definitions; 8.8 Class definitions"}
proposals: [pep-318, pep-3129, pep-614]
see_also: [python/function-objects, python/closures-and-nonlocal, python/functools, python/callable-objects, python/metaclasses]
sources:
  - url: https://docs.python.org/3.15/reference/compound_stmts.html#function-definitions
    title: The Python Language Reference, Python 3.15
    locator: 8.7 Function definitions; 8.8 Class definitions
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/glossary.html#term-decorator
    title: Glossary, Python 3.15 documentation
    locator: decorator
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/library/functools.html#functools.wraps
    title: functools — Higher-order functions and operations on callable objects, Python 3.15 documentation
    locator: update_wrapper(); wraps()
    accessed: 2026-11-16
  - url: https://peps.python.org/pep-0318/
    title: "PEP 318: Decorators for Functions and Methods"
    locator: Motivation
    accessed: 2026-11-16
  - url: https://peps.python.org/pep-3129/
    title: "PEP 3129: Class Decorators"
    locator: Abstract
    accessed: 2026-11-16
  - url: https://peps.python.org/pep-0614/
    title: "PEP 614: Relaxing Grammar Restrictions On Decorators"
    locator: Abstract
    accessed: 2026-11-16
---

## Synopsis

```python run id=trace
import functools

def trace(func):
    @functools.wraps(func)
    def wrapper(*args, **kwargs):
        print("call", func.__name__, args)
        return func(*args, **kwargs)
    return wrapper

@trace
def add(a, b):
    """Add two numbers."""
    return a + b

print(add(2, 3))
print(add.__name__, add.__doc__)
print(add.__wrapped__(1, 1))
```

```output for=trace
call add (2, 3)
5
add Add two numbers.
2
```

## Semantics

**r1.** "Decorator expressions are evaluated when the function is defined, in the scope that contains the function definition. The result must be a callable, which is invoked with the function object as the only argument. The returned value is bound to the function name instead of the function object."[^1]

**r2.** The `@` form is a short way to write a call. The glossary says: "The decorator syntax is merely syntactic sugar".[^2] `@trace` above `def add` has the same effect as `add = trace(add)` after it.

**r3.** "Multiple decorators are applied in nested fashion." `@f1(arg)` above `@f2` above `def func` is "roughly equivalent to" `func = f1(arg)(f2(func))`.[^1] So the decorator nearest to `def` is applied first. Example: [`stacking`](#stacking).

**r4.** A decorator with arguments, such as `@repeat(3)`, is a call that returns the real decorator. The expression after `@` can be "any valid assignment_expression".[^1] Example: [`factory`](#factory).

**r5.** A class definition can have decorators too. "The evaluation rules for the decorator expressions are the same as for function decorators. The result is then bound to the class name."[^1] Example: [`register`](#register).

**r6.** `functools.wraps(func)` copies `__module__`, `__name__`, `__qualname__`, `__annotations__`, `__type_params__` and `__doc__` from `func` to the wrapper, and it "automatically adds a __wrapped__ attribute to the wrapper that refers to the function being wrapped."[^3] Example: [`trace`](#trace).

**r7.** The decorator can return any object. If it returns `None`, the name is bound to `None` (rule r1). Example: [`no-return`](#no-return).

## Examples

```python run id=factory
import functools

def repeat(n):
    def decorator(func):
        @functools.wraps(func)
        def wrapper(*args, **kwargs):
            return [func(*args, **kwargs) for _ in range(n)]
        return wrapper
    return decorator

@repeat(3)
def hello():
    return "hi"

print(hello())
```

```output for=factory
['hi', 'hi', 'hi']
```

`repeat(3)` runs first and returns `decorator`, then `decorator(hello)` gives the wrapper (rule r4).

```python run id=stacking
def tag(name):
    def decorator(func):
        print("apply", name)
        def wrapper():
            return f"<{name}>{func()}</{name}>"
        return wrapper
    return decorator

@tag("b")
@tag("i")
def text():
    return "x"

print(text())
```

```output for=stacking
apply i
apply b
<b><i>x</i></b>
```

The lower decorator wraps the function first, and the upper decorator wraps the result (rule r3).

```python run id=register
registry = {}

def register(cls):
    registry[cls.__name__] = cls
    return cls

@register
class Circle:
    pass

@register
class Square:
    pass

print(sorted(registry))
```

```output for=register
['Circle', 'Square']
```

A decorator does not have to wrap. This one records the class and returns it unchanged (rule r5).

```python run id=class-decorator
import functools

class Counter:
    def __init__(self, func):
        functools.update_wrapper(self, func)
        self.func = func
        self.calls = 0

    def __call__(self, *args):
        self.calls += 1
        return self.func(*args)

@Counter
def square(x):
    return x * x

square(2)
square(3)
print(square.calls, square.__name__)
```

```output for=class-decorator
2 square
```

A class is a callable, so it can be a decorator. The name is bound to the `Counter` instance.

## Versions

> [!since v="2.4"]
> PEP 318 added function and method decorators.[^4]

> [!since v="3.0"]
> PEP 3129 added class decorators.[^5]

> [!since v="3.9"]
> PEP 614 removed the old rule that "all decorators consist of a dotted name, optionally followed by a single call."[^6] So `@buttons[1]` is valid.

## Pitfalls

```python wrong id=no-wraps
def plain(func):
    def wrapper(*args):
        return func(*args)
    return wrapper

@plain
def area(r):
    """Area of a square."""
    return r * r

print(area.__name__, area.__doc__)
```

```output for=no-wraps
wrapper None
```

Without `functools.wraps`, the function shows the name and docstring of the wrapper. "If the wrapper function is not updated, the metadata of the returned function will reflect the wrapper definition rather than the original function definition".[^3]

```python fails id=no-return expect=exception:TypeError
def mark(func):
    func.marked = True

@mark
def g():
    return 1

g()
```

`mark` returns `None`, so `g` is `None` after the decorator, and the call fails (rule r7). Add `return func`.

## Comparison

The glossary shows two forms with the same effect: `def f(arg): ...` followed by `f = staticmethod(f)`, and `@staticmethod` above `def f(arg): ...`.[^2] PEP 318 gives the reason for the `@` form: the old form "places the actual transformation after the function body."[^4] A decorator is normal code that runs when `def` runs. It gets a finished function object, so it cannot change the syntax of the body. Python has no macros; see [[syntactic-macros]].
