---
tag: M1YYG5
kind: feature
owner: python
slug: introspection-with-inspect
name: Introspection with inspect
aliases: [inspect, inspect.signature, Signature, Parameter, getmembers, getsource, isfunction, ismethod, inspect.stack, getattr_static, getdoc]
area: languages/python/metaprogramming
level: advanced
summary: >-
  The inspect module reads live objects. signature() gives the parameters of a callable, getmembers() lists members, the is functions test kinds, getsource() returns code and stack() gives the frames.
grammar: null
spec: {standard: null, url: "https://docs.python.org/3.15/library/inspect.html", locator: "inspect — Inspect live objects"}
proposals: [pep-362]
see_also: [runtime-reflection, python/dynamic-attribute-access, python/the-dir-function, python/function-objects, python/decorators]
sources:
  - url: https://docs.python.org/3.15/library/inspect.html
    title: inspect — Inspect live objects, Python 3.15 documentation
    locator: Types and members; Retrieving source code; Introspecting callables with the Signature object; The interpreter stack; Fetching attributes statically
    accessed: 2026-11-16
---

## Synopsis

```python run id=signature
import inspect

def greet(name, *, greeting="Hello", times: int = 1) -> str:
    """Return a greeting."""
    return f"{greeting}, {name}!" * times

sig = inspect.signature(greet)
print(sig)
for p in sig.parameters.values():
    print(p.name, p.kind.name, p.default is p.empty)
print(inspect.getdoc(greet))
```

```output for=signature
(name, *, greeting='Hello', times: int = 1) -> str
name POSITIONAL_OR_KEYWORD True
greeting KEYWORD_ONLY False
times KEYWORD_ONLY False
Return a greeting.
```

## Semantics

**r1.** The module helps "get information about live objects such as modules, classes, methods, functions, tracebacks, frame objects, and code objects." It has "four main kinds of services": "type checking, getting source code, inspecting classes and functions, and examining the interpreter stack."[^1]

**r2.** `signature(callable)` returns a `Signature` object. It "represents the call signature of a function and its return annotation. For each parameter accepted by the function it stores a Parameter object in its parameters collection."[^1] Example: [`signature`](#signature).

**r3.** `Signature.bind(*args, **kwargs)` maps arguments to parameters, as a call would. It "Returns BoundArguments if *args and **kwargs match the signature, or raises a TypeError."[^1] Example: [`bind`](#bind).

**r4.** `getmembers(obj, predicate)` returns "all the members of an object in a list of (name, value) pairs sorted by name." The predicates such as `isfunction()`, `ismethod()` and `isclass()` test the kind of an object.[^1] Example: [`members`](#members).

**r5.** `getsource(obj)` returns "the text of the source code for an object." "An OSError is raised if the source code cannot be retrieved. A TypeError is raised if the object is a built-in module, class, or function."[^1] Example: [`source`](#source).

**r6.** `stack()` returns a list of `FrameInfo` objects. "The first entry in the returned list represents the caller; the last entry represents the outermost call on the stack."[^1] Example: [`stack`](#stack).

**r7.** "Both getattr() and hasattr() can trigger code execution when fetching or checking for the existence of attributes." `getattr_static()` gets an attribute "without triggering dynamic lookup via the descriptor protocol, __getattr__() or __getattribute__()."[^1] Example: [`static`](#static).

## Examples

```python run id=bind
import inspect

def greet(name, *, greeting="Hello"):
    return f"{greeting}, {name}!"

sig = inspect.signature(greet)
bound = sig.bind("Ann")
bound.apply_defaults()
print(bound.arguments)
try:
    sig.bind()
except TypeError as e:
    print(e)
```

```output for=bind
{'name': 'Ann', 'greeting': 'Hello'}
missing a required argument: 'name'
```

`bind()` checks the arguments without a call to the function (rule r3).

```python run id=members
import inspect

class Shape:
    sides = 0

    def area(self):
        return 0

print([name for name, _ in inspect.getmembers(Shape, inspect.isfunction)])
print(inspect.isfunction(Shape().area), inspect.ismethod(Shape().area))
```

```output for=members
['area']
False True
```

On the class, `area` is a plain function. On an instance, it is a bound method (rule r4).

```python run id=source
import inspect

def square(x):
    return x * x

print(inspect.getsource(square), end="")
```

```output for=source
def square(x):
    return x * x
```

```python run id=stack
import inspect

def outer():
    inner()

def inner():
    for info in inspect.stack()[:3]:
        print(info.function)

outer()
```

```output for=stack
inner
outer
<module>
```

```python run id=static
import inspect

class Lazy:
    @property
    def value(self):
        print("computing")
        return 1

print(type(inspect.getattr_static(Lazy(), "value")).__name__)
print(Lazy().value)
```

```output for=static
property
computing
1
```

`getattr_static()` returned the property object and did not run it (rule r7).

## Versions

> [!since v="3.3"]
> The `Signature` object and `signature()` were added.[^1]

> [!since v="3.11"]
> `stack()` returns a list of `FrameInfo` objects.[^1]

## Pitfalls

```python fails id=builtin-source expect=exception:TypeError
import inspect

print(inspect.getsource(len))
```

`len` is written in C, so there is no Python source (rule r5).

```python fails id=no-signature expect=exception:ValueError
import inspect

print(inspect.signature(dict.update))
```

`signature()` "Raises ValueError if no signature can be provided", and "in CPython, some built-in functions defined in C provide no metadata about their arguments."[^1] Check for `ValueError` when the callable can be a built-in.

## Comparison

`dir()` and `getattr()` give names and values. `inspect` adds structure on top of them: the parameters of a function, the kind of each member, and the frames of the stack (rules r2, r4 and r6). See [[python/the-dir-function]] and [[python/dynamic-attribute-access]].
