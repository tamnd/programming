---
tag: 29YJ2X
kind: feature
owner: python
slug: positional-only-parameters
name: Positional-only parameters
aliases: [positional-only parameter, slash in def, / in parameter list, PEP 570, POSITIONAL_ONLY, takes no keyword arguments]
area: languages/python/functions
level: core
summary: >-
  Parameters before a / in a def can be filled only by position. A caller cannot pass them as keywords, and a **kwargs parameter can take the same names.
grammar: "parameter_list: defparameter (',' defparameter)* ',' '/' [',' [parameter_list_no_posonly]] (Language Reference, 8.7)"
spec: {standard: null, url: "https://docs.python.org/3.15/reference/compound_stmts.html#function-definitions", locator: "8.7. Function definitions"}
see_also: [python/special-parameters, python/positional-or-keyword-arguments, python/keyword-only-arguments, python/arbitrary-argument-lists]
sources:
  - url: https://docs.python.org/3.15/tutorial/controlflow.html#positional-only-parameters
    title: The Python Tutorial, 4. More Control Flow Tools
    locator: 4.9.3.2. Positional-Only Parameters; 4.9.3.4. Function Examples; 4.9.3.5. Recap
    accessed: 2026-11-16
  - url: https://peps.python.org/pep-0570/
    title: "PEP 570: Python Positional-Only Parameters"
    locator: Abstract; Specification; Semantic Corner Case
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/reference/compound_stmts.html#function-definitions
    title: The Python Language Reference, Python 3.15
    locator: 8.7. Function definitions
    accessed: 2026-11-16
  - url: https://go.dev/ref/spec#Function_types
    title: The Go Programming Language Specification
    locator: Function types
    accessed: 2026-11-16
---

## Synopsis

```python run id=pos-only
def pos_only_arg(arg, /):
    print(arg)

pos_only_arg(1)
try:
    pos_only_arg(arg=1)
except TypeError:
    print("arg is positional-only")
```

```output for=pos-only
1
arg is positional-only
```

The `/` makes `arg` positional-only. A call with `arg=1` is a TypeError.

## Semantics

**r1.** "If positional-only, the parameters’ order matters, and the parameters cannot be passed by keyword. Positional-only parameters are placed before a / (forward-slash)."[^1]

**r2.** "Parameters following the / may be positional-or-keyword or keyword-only."[^1] Example: [`after-slash`](#after-slash).

**r3.** "Positional-only parameters have no externally-usable name. When a function accepting positional-only parameters is called, positional arguments are mapped to these parameters based solely on their order."[^2]

**r4.** "Once a positional-only parameter is specified with a default, the following positional-only and positional-or-keyword parameters need to have defaults as well."[^2] Example: [`default-order`](#default-order).

**r5.** "the names of positional-only parameters can be used in **kwds without ambiguity."[^1] Example: [`name-in-kwds`](#name-in-kwds).

**r6.** "Use positional-only if you want the name of the parameters to not be available to the user. This is useful when parameter names have no real meaning, if you want to enforce the order of the arguments when the function is called or if you need to take some positional parameters and arbitrary keywords."[^1]

## Examples

```python run id=after-slash
def clamp(value, /, low=0, high=10):
    return max(low, min(value, high))

print(clamp(15), clamp(-3, low=-5), clamp(5, 6, 8))
```

```output for=after-slash
10 -3 6
```

`low` and `high` come after `/`, so a call can give them by position or by name (rule r2).

```python run id=name-in-kwds
def foo(name, /, **kwds):
    return "name" in kwds

print(foo(1, **{"name": 2}))
```

```output for=name-in-kwds
True
```

This is the example of the tutorial and PEP 570. The positional-only `name` and the key `"name"` do not collide (rule r5).

```python run id=builtin
try:
    len(obj="abc")
except TypeError:
    print("keywords not accepted")
print(len("abc"))
```

```output for=builtin
keywords not accepted
3
```

The built-in `len()` takes its argument only by position. With `/`, a function written in Python can do the same (rule r1).

## Pitfalls

Without `/`, the name of the first parameter collides with a key of `**kwds`:

```python fails id=collision expect=exception:TypeError
def foo(name, **kwds):
    return "name" in kwds

foo(1, **{"name": 2})
```

A positional-only parameter with a default needs defaults for the parameters after it, up to `*`:

```python fails id=default-order expect=exception:SyntaxError
def name(p1=None, p2, /):
    pass
```

## Comparison

Go parameters are all positional. A Go signature can even leave the parameters without names: "Within a list of parameters or results, the names (IdentifierList) must either all be present or all be absent."[^4] So a Go parameter name is never part of what a caller writes, as with a Python positional-only parameter (rule r3).
