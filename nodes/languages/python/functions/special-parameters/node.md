---
tag: 43ZQH1
kind: feature
owner: python
slug: special-parameters
name: Special parameters, / and *
aliases: [slash and star in def, parameter kinds, positional-only marker, keyword-only marker, slash and star parameters, how arguments may be passed]
area: languages/python/functions
level: core
summary: >-
  In a parameter list, / and * set how each argument is passed. Parameters before / are positional-only, parameters after * are keyword-only, and the parameters between them can be passed either way.
grammar: "parameter_list: defparameter (',' defparameter)* ',' '/' [',' [parameter_list_no_posonly]] | parameter_list_no_posonly (Language Reference, 8.7)"
spec: {standard: null, url: "https://docs.python.org/3.15/reference/compound_stmts.html#function-definitions", locator: "8.7. Function definitions"}
see_also: [python/positional-or-keyword-arguments, python/positional-only-parameters, python/keyword-only-arguments, python/keyword-arguments, python/call-semantics]
sources:
  - url: https://docs.python.org/3.15/tutorial/controlflow.html#special-parameters
    title: The Python Tutorial, 4. More Control Flow Tools
    locator: 4.9.3. Special parameters; 4.9.3.4. Function Examples; 4.9.3.5. Recap
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/reference/compound_stmts.html#function-definitions
    title: The Python Language Reference, Python 3.15
    locator: 8.7. Function definitions
    accessed: 2026-11-16
  - url: https://go.dev/ref/spec#Calls
    title: The Go Programming Language Specification
    locator: Calls
    accessed: 2026-11-16
---

## Synopsis

```python run id=combined
def combined_example(pos_only, /, standard, *, kwd_only):
    print(pos_only, standard, kwd_only)

combined_example(1, 2, kwd_only=3)
combined_example(1, standard=2, kwd_only=3)
```

```output for=combined
1 2 3
1 2 3
```

`pos_only` is before `/`, `standard` is between `/` and `*`, and `kwd_only` is after `*`.

## Semantics

**r1.** "By default, arguments may be passed to a Python function either by position or explicitly by keyword."[^1]

**r2.** "where / and * are optional. If used, these symbols indicate the kind of parameter by how the arguments may be passed to the function: positional-only, positional-or-keyword, and keyword-only. Keyword parameters are also referred to as named parameters."[^1]

**r3.** "Parameters after “*” or “*identifier” are keyword-only parameters and may only be passed by keyword arguments. Parameters before “/” are positional-only parameters and may only be passed by positional arguments."[^2] Examples: [`three-positional`](#three-positional), [`keyword-for-positional-only`](#keyword-for-positional-only).

**r4.** "If there is no / in the function definition, there are no positional-only parameters."[^1]

**r5.** The tutorial gives this guidance: "Use positional-only if you want the name of the parameters to not be available to the user." "Use keyword-only when names have meaning and the function definition is more understandable by being explicit with names or you want to prevent users relying on the position of the argument being passed."[^1]

**r6.** "For an API, use positional-only to prevent breaking API changes if the parameter’s name is modified in the future."[^1]

## Examples

```python run id=four-kinds
def standard_arg(arg):
    print(arg)

def pos_only_arg(arg, /):
    print(arg)

def kwd_only_arg(*, arg):
    print(arg)

standard_arg(2)
standard_arg(arg=2)
pos_only_arg(1)
kwd_only_arg(arg=3)
```

```output for=four-kinds
2
2
1
3
```

These are the functions of the tutorial.

```python run id=inspect-kinds
import inspect

def f(a, /, b, *, c):
    pass

for name, p in inspect.signature(f).parameters.items():
    print(name, p.kind.name)
```

```output for=inspect-kinds
a POSITIONAL_ONLY
b POSITIONAL_OR_KEYWORD
c KEYWORD_ONLY
```

## Pitfalls

Three positional arguments do not fit two positional slots:

```python fails id=three-positional expect=exception:TypeError
def combined_example(pos_only, /, standard, *, kwd_only):
    pass

combined_example(1, 2, 3)
```

A keyword for a positional-only parameter is a TypeError:

```python fails id=keyword-for-positional-only expect=exception:TypeError
def combined_example(pos_only, /, standard, *, kwd_only):
    pass

combined_example(pos_only=1, standard=2, kwd_only=3)
```

`/` must come before `*`, because the grammar puts the positional-only parameters first:

```python fails id=slash-after-star expect=exception:SyntaxError
def f(*, a, /):
    pass
```

## Comparison

A Go call passes each argument by position. In Go, "the arguments of the call are passed to the function, which means that they are assigned to their corresponding function parameters".[^3] So each Go parameter is like a Python positional-only parameter. Python lets the def statement choose the kind of each parameter (rule r2).
