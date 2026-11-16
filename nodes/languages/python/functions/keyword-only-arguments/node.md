---
tag: SXJ1C9
kind: feature
owner: python
slug: keyword-only-arguments
name: Keyword-only arguments
aliases: [keyword-only parameter, bare star in def, required keyword argument, PEP 3102, KEYWORD_ONLY, parameters after args]
area: languages/python/functions
level: core
summary: >-
  Parameters after a bare * or after *args in a def can be filled only by keyword. They need no default, and a keyword-only parameter with no default is a required keyword argument.
grammar: "parameter_list_starargs: '*' star_parameter (',' defparameter)* [',' [parameter_star_kwargs]] | '*' (',' defparameter)+ [',' [parameter_star_kwargs]] (Language Reference, 8.7)"
spec: {standard: null, url: "https://docs.python.org/3.15/reference/compound_stmts.html#function-definitions", locator: "8.7. Function definitions"}
see_also: [python/special-parameters, python/positional-only-parameters, python/arbitrary-argument-lists, python/keyword-arguments, python/default-argument-values]
sources:
  - url: https://docs.python.org/3.15/tutorial/controlflow.html#keyword-only-arguments
    title: The Python Tutorial, 4. More Control Flow Tools
    locator: 4.9.3.3. Keyword-Only Arguments; 4.9.3.4. Function Examples; 4.9.3.5. Recap; 4.9.4. Arbitrary Argument Lists
    accessed: 2026-11-16
  - url: https://peps.python.org/pep-3102/
    title: "PEP 3102: Keyword-Only Arguments"
    locator: Abstract; Specification
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

```python run id=compare
def compare(a, b, *, key=None):
    if key is not None:
        a, b = key(a), key(b)
    return (a > b) - (a < b)

print(compare("a", "B"))
print(compare("a", "B", key=str.lower))
```

```output for=compare
1
-1
```

This is the `compare` function of PEP 3102. `key` comes after `*`, so a call must name it.

## Semantics

**r1.** "To mark parameters as keyword-only, indicating the parameters must be passed by keyword argument, place an * in the arguments list just before the first keyword-only parameter."[^1] Example: [`positional-for-keyword-only`](#positional-for-keyword-only).

**r2.** "Any formal parameters which occur after the *args parameter are ‘keyword-only’ arguments, meaning that they can only be used as keywords rather than positional arguments."[^1] Example: [`concat`](#concat).

**r3.** PEP 3102 defines them as "arguments that can only be supplied by keyword and which will never be automatically filled in by a positional argument."[^2]

**r4.** "Keyword-only arguments are not required to have a default value."[^2] "Such arguments must be supplied by the caller, and they must be supplied via keyword."[^2] Example: [`required`](#required).

**r5.** "Use keyword-only when names have meaning and the function definition is more understandable by being explicit with names or you want to prevent users relying on the position of the argument being passed."[^1]

## Examples

```python run id=concat
def concat(*args, sep="/"):
    return sep.join(args)

print(concat("earth", "mars", "venus"))
print(concat("earth", "mars", "venus", sep="."))
```

```output for=concat
earth/mars/venus
earth.mars.venus
```

This is the example of the tutorial. `*args` takes each positional argument, so `sep` can only be a keyword (rule r2).

```python run id=required
def connect(host, *, timeout, retries=3):
    return f"{host} timeout={timeout} retries={retries}"

print(connect("example.org", timeout=5))
try:
    connect("example.org")
except TypeError:
    print("timeout is required")
```

```output for=required
example.org timeout=5 retries=3
timeout is required
```

`timeout` has no default, so each call must give it by keyword (rule r4).

## Pitfalls

A positional argument for a keyword-only parameter is a TypeError:

```python fails id=positional-for-keyword-only expect=exception:TypeError
def kwd_only_arg(*, arg):
    print(arg)

kwd_only_arg(3)
```

A bare `*` must be followed by at least one named parameter:

```python fails id=bare-star-last expect=exception:SyntaxError
def f(a, *):
    pass
```

The grammar has the form `'*' (',' defparameter)+`, so a bare `*` needs a parameter after it.[^3]

## Comparison

Go has no keyword arguments, so Go has no keyword-only parameters. In Go, "the arguments of the call are passed to the function, which means that they are assigned to their corresponding function parameters".[^4] A Go caller gives each argument in the order of the parameters. In Python, a keyword-only parameter makes the caller write the name of the argument (rule r3).
