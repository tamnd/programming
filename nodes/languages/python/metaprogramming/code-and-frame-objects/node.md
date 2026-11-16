---
tag: 9JN9TM
kind: feature
owner: python
slug: code-and-frame-objects
name: Code and frame objects
aliases: [__code__, code object, co_varnames, co_freevars, frame object, f_back, f_locals, f_code, sys._getframe, inspect.currentframe, traceback object, tb_next, types.CodeType, types.FrameType]
area: languages/python/metaprogramming
level: research
summary: >-
  A code object is the compiled body of a function, without globals or defaults. A frame is one running call, with its locals and a link to the caller. A traceback links the frames of an exception.
grammar: null
spec: {standard: null, url: "https://docs.python.org/3.15/reference/datamodel.html#internal-types", locator: "3.2.13 Internal types"}
proposals: []
see_also: [call-stack-and-heap, runtime-reflection, python/introspection-with-inspect, python/function-objects, python/closures-and-nonlocal, python/tracebacks-and-exception-objects]
sources:
  - url: https://docs.python.org/3.15/reference/datamodel.html#internal-types
    title: The Python Language Reference, Python 3.15
    locator: 3.2.13.1 Code objects; 3.2.13.2 Frame objects; 3.2.13.3 Traceback objects
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/library/sys.html#sys._getframe
    title: sys — System-specific parameters and functions, Python 3.15 documentation
    locator: sys._getframe()
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/library/inspect.html#the-interpreter-stack
    title: inspect — Inspect live objects, Python 3.15 documentation
    locator: The interpreter stack
    accessed: 2026-11-16
---

## Synopsis

```python run id=code
def area(w, h=2, *, unit="m"):
    total = w * h
    return f"{total}{unit}"

c = area.__code__
print(c.co_name, c.co_argcount, c.co_kwonlyargcount)
print(c.co_varnames)
print(area.__defaults__, area.__kwdefaults__)
```

```output for=code
area 2 1
('w', 'h', 'unit', 'total')
(2,) {'unit': 'm'}
```

## Semantics

**r1.** "Code objects represent byte-compiled executable Python code, or bytecode."[^1] A function keeps its code object in `__code__`. Example: [`code`](#code).

**r2.** "the function object contains an explicit reference to the function’s globals (the module in which it was defined), while a code object contains no context; also the default argument values are stored in the function object, not in the code object".[^1] Example: [`swap-code`](#swap-code).

**r3.** "Unlike function objects, code objects are immutable".[^1] To change one, `replace(**kwargs)` returns "a copy of the code object with new values for the specified fields."[^1] Example: [`replace`](#replace).

**r4.** `co_varnames` holds "the names of the local variables in the function (starting with the parameter names)". `co_cellvars` holds locals "referenced from at least one nested scope", and `co_freevars` holds "free (closure) variables that a nested scope references in an outer scope."[^1] Example: [`closure`](#closure).

**r5.** "Frame objects represent execution frames."[^1] `f_back` points "to the previous stack frame (towards the caller)", `f_code` is "The code object being executed in this frame", and `f_locals` is "The mapping used by the frame to look up local variables."[^1]

**r6.** `sys._getframe(depth)` returns "the frame object that many calls below the top of the stack. If that is deeper than the call stack, ValueError is raised."[^2] It "should be used for internal and specialized purposes only. It is not guaranteed to exist in all implementations of Python."[^2] Example: [`caller`](#caller).

**r7.** "Traceback objects represent the stack trace of an exception."[^1] `tb_frame` is "the execution frame of the current level", and `tb_next` is "the next level in the stack trace (towards the frame where the exception occurred)".[^1] Example: [`traceback`](#traceback).

**r8.** "Keeping references to frame objects, as found in the first element of the frame records these functions return, can cause your program to create reference cycles."[^3] `frame.clear()` "clears all references to local variables held by the frame."[^1]

## Examples

```python run id=replace
import types

def add(a, b):
    return a + b

code = add.__code__.replace(co_name="plus", co_qualname="plus")
plus = types.FunctionType(code, globals())
print(plus.__name__, plus(2, 3))
print(plus.__code__ is add.__code__, add.__name__)
```

```output for=replace
plus 5
False add
```

`replace` makes a new code object, and `types.FunctionType` wraps it with a globals dictionary. The original function does not change (rule r3).

```python run id=closure
def counter():
    n = 0
    def step():
        nonlocal n
        n += 1
        return n
    return step

s = counter()
s()
s()
print(counter.__code__.co_cellvars)
print(s.__code__.co_freevars)
print(s.__closure__[0].cell_contents)
```

```output for=closure
('n',)
('n',)
2
```

For the outer function, `n` is a cell variable. For the inner function, it is a free variable. The value is in the cell of `__closure__` (rule r4).

```python run id=caller
import sys

def who_called():
    caller = sys._getframe(1)
    return caller.f_code.co_name, caller.f_locals["order"]

def checkout():
    order = 42
    return who_called()

print(checkout())
```

```output for=caller
('checkout', 42)
```

Depth 1 is the frame of the caller. Its code object gives the name, and `f_locals` gives its local variables (rules r5 and r6). Use `inspect.currentframe()` for the same frame through a public API.[^3]

```python run id=traceback
def parse(s):
    return int(s)

def load(s):
    return parse(s)

try:
    load("x")
except ValueError as e:
    tb = e.__traceback__
    names = []
    while tb is not None:
        names.append(tb.tb_frame.f_code.co_name)
        tb = tb.tb_next
    print(names)
```

```output for=traceback
['<module>', 'load', 'parse']
```

The chain starts at the frame that caught the exception and goes to the frame that raised it (rule r7).

## Pitfalls

```python fails id=swap-code expect=exception:TypeError
def a(x=1):
    return x

def b(x):
    return x * 10

b.__code__ = a.__code__
print(b())
```

The code of `a` moves to `b`, but the default `x=1` stays with the function `a` (rule r2). So `b()` has no value for `x` and raises `TypeError`.

```python fails id=too-deep expect=exception:ValueError
import sys

def top():
    return sys._getframe(50)

top()
```

The stack has far fewer than 50 frames here, so the call raises `ValueError` (rule r6).

```python fails id=clear-running expect=exception:RuntimeError
import sys

def f():
    sys._getframe().clear()

f()
```

"RuntimeError is raised if the frame is currently executing or suspended."[^1] Clear a frame only after its call ends, for example a frame kept from a traceback (rule r8).

## Comparison

Code and frame objects are the data that a debugger and a profiler use. See [[python/tracing-and-monitoring]] for the hooks that get frames, [[python/bytecode-and-dis]] for the instructions in `co_code`, and [[call-stack-and-heap]] for the general model of frames.
