---
tag: AJNFR0
kind: feature
owner: python
slug: annotations-at-runtime
name: Annotations at run time
aliases: [__annotations__, __annotate__, annotationlib, get_annotations, deferred evaluation of annotations, lazy annotations, ForwardRef, Format.FORWARDREF, Format.STRING, PEP 649, PEP 749]
area: languages/python/metaprogramming
level: advanced
summary: >-
  From 3.14, Python does not evaluate annotations when a def or class runs. An __annotate__ function evaluates them on first access. annotationlib.get_annotations gives values, ForwardRefs or strings.
grammar: null
spec: {standard: null, url: "https://docs.python.org/3.15/reference/compound_stmts.html#annotations", locator: "8.11 Annotations"}
proposals: [pep-649, pep-749]
see_also: [annotations, python/function-annotations, python/annotation-scopes, python/introspection-with-inspect, python/annotated-assignment]
sources:
  - url: https://docs.python.org/3.15/reference/compound_stmts.html#annotations
    title: The Python Language Reference, Python 3.15
    locator: 8.11 Annotations
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/reference/datamodel.html#object.__annotate__
    title: The Python Language Reference, Python 3.15
    locator: 3.3.12 Annotations, object.__annotations__ and object.__annotate__
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/library/annotationlib.html
    title: annotationlib — Functionality for introspecting annotations, Python 3.15 documentation
    locator: Annotation semantics; Format; get_annotations()
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/howto/annotations.html
    title: Annotations Best Practices, Python 3.15 documentation
    locator: Accessing The Annotations Dict Of An Object In Python 3.10 And Newer; Quirks
    accessed: 2026-11-16
---

## Synopsis

```python run id=forward
def make(a: Cls) -> None:
    pass

class Cls:
    pass

ann = make.__annotations__
print(ann["a"] is Cls, ann["return"])
```

```output for=forward
True None
```

## Semantics

**r1.** "By default, annotations are lazily evaluated in an annotation scope. This means that they are not evaluated when the code containing the annotation is evaluated."[^1] Example: [`forward`](#forward).

**r2.** "The presence of annotations does not change the runtime semantics of the code, except if some mechanism is used that introspects and uses the annotations".[^1]

**r3.** A function, class or module with annotations has an `__annotate__` function. It takes a format and "Returns a new dictionary object mapping attribute/parameter names to their annotation values."[^2]

**r4.** `__annotations__` "is lazily evaluated, so accessing the attribute may execute arbitrary code and raise exceptions. If evaluation is successful, the attribute is set to a dictionary mapping from variable names to annotations."[^2] So the expressions run on the first access only. Example: [`lazy`](#lazy).

**r5.** `annotationlib.get_annotations(obj, format=...)` has three main formats. "VALUE evaluates the annotations and returns their value." "FORWARDREF returns ForwardRef objects for annotations that cannot be resolved". "STRING returns the annotations as a string, similar to how it would appear in the source file."[^3] Example: [`formats`](#formats).

**r6.** "You should avoid accessing __annotations__ directly on any object. Instead, use annotationlib.get_annotations() (Python 3.14+) or inspect.get_annotations() (Python 3.10+)."[^4]

**r7.** With `from __future__ import annotations`, "all annotations are instead stored as strings".[^1] Example: [`future-strings`](#future-strings).

## Examples

```python run id=lazy
def kind():
    print("evaluated")
    return int

def h(x: kind()):
    pass

print("defined")
h.__annotations__
h.__annotations__
```

```output for=lazy
defined
evaluated
```

The annotation ran on the first access, not at `def`, and only once (rule r4).

```python run id=formats
from annotationlib import get_annotations, Format

def g(x: Undefined, y: int) -> str:
    pass

refs = get_annotations(g, format=Format.FORWARDREF)
print(type(refs["x"]).__name__, refs["x"].__forward_arg__, refs["y"])
print(get_annotations(g, format=Format.STRING))
```

```output for=formats
ForwardRef Undefined <class 'int'>
{'x': 'Undefined', 'y': 'int', 'return': 'str'}
```

`Undefined` does not exist, but the two formats still read the annotations (rule r5).

```python run id=class-ann
from annotationlib import get_annotations

class Point:
    x: int
    y: "float"
    label = "p"

class Point3(Point):
    z: int

print(get_annotations(Point))
print(get_annotations(Point3))
```

```output for=class-ann
{'x': <class 'int'>, 'y': 'float'}
{'z': <class 'int'>}
```

`label` has no annotation, so it is not in the result. A subclass gives only its own annotations. A string annotation stays a string; pass `eval_str=True` to evaluate it.

## Versions

> [!since v="3.14"]
> "Annotations are now lazily evaluated by default."[^1] The `annotationlib` module and `__annotate__` are new, from PEP 649 and PEP 749.[^3]

## Pitfalls

```python fails id=missing-name expect=exception:NameError
def g(x: Undefined):
    pass

print(g.__annotations__)
```

`def` works, because the annotation is not evaluated yet. The access evaluates it and raises `NameError` (rule r4). Use `Format.FORWARDREF` to read it without an error.

```python wrong id=future-strings
from __future__ import annotations

def f(x: int) -> str:
    return str(x)

print(f.__annotations__)
print(f.__annotations__["x"] is int)
```

```output for=future-strings
{'x': 'int', 'return': 'str'}
False
```

The future import turns each annotation into a string (rule r7). Code that compares annotations with classes then fails. The reference says this future statement "will be deprecated and removed in a future version of Python".[^1]

## Comparison

From 3.0 to 3.13, annotations were evaluated eagerly, when the `def` ran, so a forward reference raised `NameError`.[^3] A Python annotation can be any expression, and the language does not check it against the values (rule r2). See [[python/function-annotations]] for the syntax and [[annotations]] for other languages.
