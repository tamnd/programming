---
tag: CG0JCG
kind: feature
owner: python
slug: keyword-arguments
name: Keyword arguments
aliases: [keyword argument, named argument, kwarg=value, kwargs, double star parameter, call by name, got multiple values for argument]
area: languages/python/functions
level: intro
summary: >-
  A call can give an argument as name=value. Keyword arguments come after positional arguments, they can be in any order, and each one must match a parameter name or go to a **kwargs parameter.
grammar: "keyword_item: identifier '=' expression; parameter_star_kwargs: '**' parameter [','] (Language Reference, 6.3.3 and 8.7)"
spec: {standard: null, url: "https://docs.python.org/3.15/reference/expressions.html#calls", locator: "6.3.3. Calls"}
see_also: [python/default-argument-values, python/keyword-only-arguments, python/arbitrary-argument-lists, python/unpacking-argument-lists, python/call-semantics]
sources:
  - url: https://docs.python.org/3.15/tutorial/controlflow.html#keyword-arguments
    title: The Python Tutorial, 4. More Control Flow Tools
    locator: 4.9.2. Keyword Arguments
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/reference/expressions.html#calls
    title: The Python Language Reference, Python 3.15
    locator: 6.3.3. Calls
    accessed: 2026-11-16
  - url: https://go.dev/ref/spec#Calls
    title: The Go Programming Language Specification
    locator: Calls
    accessed: 2026-11-16
---

## Synopsis

```python run id=parrot
def parrot(voltage, state="a stiff", action="voom"):
    return f"{action} at {voltage} volts, {state}"

print(parrot(1000))
print(parrot(voltage=1000))
print(parrot(action="VOOOOOM", voltage=1000000))
print(parrot("a thousand", state="pushing up the daisies"))
```

```output for=parrot
voom at 1000 volts, a stiff
voom at 1000 volts, a stiff
VOOOOOM at 1000000 volts, a stiff
voom at a thousand volts, pushing up the daisies
```

A keyword argument names its parameter, so the order of keyword arguments is not important.

## Semantics

**r1.** "Functions can also be called using keyword arguments of the form kwarg=value."[^1]

**r2.** "In a function call, keyword arguments must follow positional arguments. All the keyword arguments passed must match one of the arguments accepted by the function (e.g. actor is not a valid argument for the parrot function), and their order is not important."[^1] Examples: [`positional-after-keyword`](#positional-after-keyword), [`unknown-keyword`](#unknown-keyword).

**r3.** "No argument may receive a value more than once."[^1] Example: [`twice`](#twice).

**r4.** "When a final formal parameter of the form **name is present, it receives a dictionary" that contains "all keyword arguments except for those corresponding to a formal parameter."[^1] Example: [`kwargs`](#kwargs).

**r5.** "Note that the order in which the keyword arguments are printed is guaranteed to match the order in which they were provided in the function call."[^1] Example: [`kwargs`](#kwargs).

**r6.** For each keyword argument, "the identifier is used to determine the corresponding slot".[^2] "If the slot is already filled, a TypeError exception is raised."[^2]

## Examples

```python run id=kwargs
def tag(name, **attributes):
    parts = [name] + [f'{k}="{v}"' for k, v in attributes.items()]
    return "<" + " ".join(parts) + ">"

print(tag("a", href="/home", title="Home"))
print(tag("br"))
```

```output for=kwargs
<a href="/home" title="Home">
<br>
```

`attributes` is a dict of the keyword arguments that match no parameter. The keys keep the order of the call (rules r4 and r5).

## Pitfalls

A value for the same parameter by position and by keyword is a TypeError:

```python fails id=twice expect=exception:TypeError
def function(a):
    pass

function(0, a=0)
```

A name that matches no parameter is a TypeError when there is no `**` parameter:

```python fails id=unknown-keyword expect=exception:TypeError
def parrot(voltage, state="a stiff"):
    pass

parrot(actor="John Cleese")
```

A positional argument after a keyword argument is a syntax error:

```python fails id=positional-after-keyword expect=exception:SyntaxError
def parrot(voltage, state="a stiff"):
    pass

parrot(voltage=5.0, "dead")
```

## Comparison

Go has no keyword arguments. In a Go call `f(a1, a2, … an)`, Go assigns the arguments to the parameters in order: "the arguments of the call are passed to the function, which means that they are assigned to their corresponding function parameters".[^3] In Python, a keyword argument fills the parameter with the same name (rule r6).
