---
tag: KR09AF
kind: feature
owner: python
slug: positional-or-keyword-arguments
name: Positional-or-keyword parameters
aliases: [positional or keyword parameter, standard parameter, normal parameter, POSITIONAL_OR_KEYWORD, pass by position or by name]
area: languages/python/functions
level: core
summary: >-
  A parameter that is not before / and not after * is positional-or-keyword. A call can fill it by position or by its name. This is the default kind, so a plain def has only this kind of parameter.
grammar: "parameter_list_no_posonly: defparameter (',' defparameter)* [',' [parameter_list_starargs]] | parameter_list_starargs (Language Reference, 8.7)"
spec: {standard: null, url: "https://docs.python.org/3.15/reference/compound_stmts.html#function-definitions", locator: "8.7. Function definitions"}
see_also: [python/special-parameters, python/positional-only-parameters, python/keyword-only-arguments, python/keyword-arguments]
sources:
  - url: https://docs.python.org/3.15/tutorial/controlflow.html#positional-or-keyword-arguments
    title: The Python Tutorial, 4. More Control Flow Tools
    locator: 4.9.3.1. Positional-or-Keyword Arguments; 4.9.3.4. Function Examples; 4.9.3.5. Recap
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/reference/expressions.html#calls
    title: The Python Language Reference, Python 3.15
    locator: 6.3.3. Calls
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/glossary.html#term-parameter
    title: Glossary, Python 3.15
    locator: parameter
    accessed: 2026-11-16
  - url: https://go.dev/ref/spec#Calls
    title: The Go Programming Language Specification
    locator: Calls
    accessed: 2026-11-16
---

## Synopsis

```python run id=standard
def standard_arg(arg):
    print(arg)

standard_arg(2)
standard_arg(arg=2)
```

```output for=standard
2
2
```

The tutorial calls this "the most familiar form". It "places no restrictions on the calling convention and arguments may be passed by position or keyword".[^1]

## Semantics

**r1.** "If / and * are not present in the function definition, arguments may be passed to a function by position or by keyword."[^1]

**r2.** The glossary lists this kind first: "positional-or-keyword: specifies an argument that can be passed either positionally or as a keyword argument. This is the default kind of parameter".[^3]

**r3.** In a call, "If there are N positional arguments, they are placed in the first N slots. Next, for each keyword argument, the identifier is used to determine the corresponding slot".[^2] Example: [`mixed`](#mixed).

**r4.** "If the slot is already filled, a TypeError exception is raised."[^2] Example: [`both-ways`](#both-ways).

**r5.** "If there are any unfilled slots for which no default value is specified, a TypeError exception is raised."[^2] Example: [`missing`](#missing).

## Examples

```python run id=mixed
def box(width, height, depth=1):
    return width * height * depth

print(box(2, 3))
print(box(2, height=3, depth=4))
print(box(depth=4, height=3, width=2))
```

```output for=mixed
6
24
24
```

The positional arguments fill the first slots, and the keyword arguments fill the slots with their names (rule r3).

## Pitfalls

An argument by position and again by name fills one slot twice:

```python fails id=both-ways expect=exception:TypeError
def box(width, height):
    return width * height

box(2, width=3)
```

A slot with no argument and no default is a TypeError:

```python fails id=missing expect=exception:TypeError
def box(width, height):
    return width * height

box(width=2)
```

The name of a positional-or-keyword parameter is part of the interface of the function, because callers can use it. If you rename the parameter, calls that use the old name fail. The tutorial says: "For an API, use positional-only to prevent breaking API changes if the parameter’s name is modified in the future."[^1]

## Comparison

A Go call passes each argument by position. In Go, "the arguments of the call are passed to the function, which means that they are assigned to their corresponding function parameters".[^4] Go has no call by name, so the name of a Go parameter is not part of what a caller writes. In Python, a caller can use the name of a positional-or-keyword parameter (rule r1).
