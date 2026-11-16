---
tag: D0MWV2
kind: feature
owner: python
slug: function-annotations
name: Function annotations
aliases: [parameter annotation, return annotation, arrow in def, __annotations__, type hints on functions, PEP 3107, annotate a function]
area: languages/python/functions
level: core
summary: >-
  A parameter can have an annotation after a colon, and a function can have a return annotation after ->. Python stores them in __annotations__ and does not check them. Type checkers read them.
grammar: "parameter: identifier [':' expression]; funcdef: ... '(' [parameter_list] ')' ['->' expression] ':' suite (Language Reference, 8.7)"
spec: {standard: null, url: "https://docs.python.org/3.15/reference/compound_stmts.html#annotations", locator: "8.11. Annotations; 8.7. Function definitions"}
see_also: [python/defining-functions, python/annotated-assignment, python/annotation-scopes, python/lambda-expressions]
sources:
  - url: https://docs.python.org/3.15/tutorial/controlflow.html#function-annotations
    title: The Python Tutorial, 4. More Control Flow Tools
    locator: 4.9.8. Function Annotations
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/reference/compound_stmts.html#annotations
    title: The Python Language Reference, Python 3.15
    locator: 8.11. Annotations; 8.7. Function definitions
    accessed: 2026-11-16
  - url: https://peps.python.org/pep-3107/
    title: "PEP 3107: Function Annotations"
    locator: Fundamentals of Function Annotations
    accessed: 2026-11-16
  - url: https://go.dev/ref/spec#Function_types
    title: The Go Programming Language Specification
    locator: Function types; Calls
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/reference/expressions.html#lambda
    title: The Python Language Reference, Python 3.15
    locator: 6.14. Lambdas
    accessed: 2026-11-16
---

## Synopsis

```python run id=ham
def f(ham: str, eggs: str = "eggs") -> str:
    return ham + " and " + eggs

print(f("spam"))
print(f.__annotations__["ham"], f.__annotations__["return"])
```

```output for=ham
spam and eggs
<class 'str'> <class 'str'>
```

`ham: str` annotates a parameter, and `-> str` annotates the return value. The annotations are in the dict `f.__annotations__`.

## Semantics

**r1.** "Function annotations are completely optional metadata information about the types used by user-defined functions".[^1]

**r2.** "Annotations are stored in the __annotations__ attribute of the function as a dictionary and have no effect on any other part of the function."[^1] Example: [`not-checked`](#not-checked).

**r3.** "Parameter annotations are defined by a colon after the parameter name, followed by an expression evaluating to the value of the annotation. Return annotations are defined by a literal ->, followed by an expression, between the parameter list and the colon denoting the end of the def statement."[^1]

**r4.** "Any parameter may have an annotation, even those of the form *identifier or **identifier."[^2] Example: [`star`](#star).

**r5.** "Annotations are conventionally used for type hints, but this is not enforced by the language, and in general annotations may contain arbitrary expressions."[^2] Example: [`any-expression`](#any-expression).

**r6.** PEP 3107 says: "By itself, Python does not attach any particular meaning or significance to annotations."[^3]

**r7.** The reference says: "By default, annotations are lazily evaluated in an annotation scope. This means that they are not evaluated when the code containing the annotation is evaluated."[^2] So do not write code that depends on the time when Python evaluates an annotation.

## Examples

```python run id=not-checked
def double(x: int) -> int:
    return x * 2

print(double("ab"))
```

```output for=not-checked
abab
```

The annotation says `int`, but Python does not check it (rule r2).

```python run id=star
def log(*messages: str, **options: bool) -> None:
    print(" ".join(messages), options)

log("a", "b", verbose=True)
print(sorted(log.__annotations__))
```

```output for=star
a b {'verbose': True}
['messages', 'options', 'return']
```

```python run id=any-expression
def move(distance: "metres", speed: "metres per second") -> "seconds":
    return distance / speed

print(move(100, 4))
print(move.__annotations__["speed"])
```

```output for=any-expression
25.0
metres per second
```

An annotation can be any expression, such as a string (rule r5).

## Pitfalls

A lambda cannot have annotations. The reference says that "functions created with lambda expressions cannot contain statements or annotations."[^5]

```python fails id=lambda-annotation expect=exception:SyntaxError
f = lambda x: int: x
```

A default comes after the annotation: write `eggs: str = "eggs"`, not `eggs = "eggs": str`.

## Comparison

In Go, the type of each parameter is part of the function type: "A function type denotes the set of all functions with the same parameter and result types."[^4] Go checks that "arguments must be single-valued expressions assignable to the parameter types of F".[^4] A Python annotation is metadata, and Python does not check a call against it (rule r6).
