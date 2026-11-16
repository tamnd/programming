---
tag: CKW7XN
kind: feature
owner: python
slug: arbitrary-argument-lists
name: Arbitrary argument lists, *args and **kwargs
aliases: [args, kwargs, variadic function, var-positional parameter, var-keyword parameter, star args, double star kwargs, variable number of arguments]
area: languages/python/functions
level: intro
summary: >-
  A *name parameter takes the extra positional arguments of a call as a tuple, and a **name parameter takes the extra keyword arguments as a dict. Then a function accepts any number of arguments.
grammar: "star_parameter: identifier [':' ['*'] expression]; parameter_star_kwargs: '**' parameter [','] (Language Reference, 8.7)"
spec: {standard: null, url: "https://docs.python.org/3.15/reference/compound_stmts.html#function-definitions", locator: "8.7. Function definitions"}
see_also: [python/unpacking-argument-lists, python/keyword-arguments, python/keyword-only-arguments, python/call-semantics]
sources:
  - url: https://docs.python.org/3.15/tutorial/controlflow.html#arbitrary-argument-lists
    title: The Python Tutorial, 4. More Control Flow Tools
    locator: 4.9.4. Arbitrary Argument Lists; 4.9.2. Keyword Arguments
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/reference/compound_stmts.html#function-definitions
    title: The Python Language Reference, Python 3.15
    locator: 8.7. Function definitions
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/reference/expressions.html#calls
    title: The Python Language Reference, Python 3.15
    locator: 6.3.3. Calls
    accessed: 2026-11-16
  - url: https://go.dev/ref/spec#Passing_arguments_to_..._parameters
    title: The Go Programming Language Specification
    locator: Passing arguments to ... parameters
    accessed: 2026-11-16
---

## Synopsis

```python run id=cheeseshop
def cheeseshop(kind, *arguments, **keywords):
    print("Do you have any", kind, "?")
    for arg in arguments:
        print(arg)
    for kw in keywords:
        print(kw, ":", keywords[kw])

cheeseshop("Limburger", "It's very runny, sir.",
           shopkeeper="Michael Palin", client="John Cleese")
```

```output for=cheeseshop
Do you have any Limburger ?
It's very runny, sir.
shopkeeper : Michael Palin
client : John Cleese
```

This is a short form of the example of the tutorial. `arguments` is a tuple, and `keywords` is a dict.

## Semantics

**r1.** "If the form “*identifier” is present, it is initialized to a tuple receiving any excess positional parameters, defaulting to the empty tuple."[^2] Example: [`empty`](#empty).

**r2.** "If the form “**identifier” is present, it is initialized to a new ordered mapping receiving any excess keyword arguments, defaulting to a new empty mapping of the same type."[^2] Example: [`empty`](#empty).

**r3.** "*name must occur before **name."[^1]

**r4.** "Normally, these variadic arguments will be last in the list of formal parameters, because they scoop up all remaining input arguments that are passed to the function. Any formal parameters which occur after the *args parameter are ‘keyword-only’ arguments".[^1] Example: [`after-args`](#after-args).

**r5.** "Formal parameters using the syntax *identifier or **identifier cannot be used as positional argument slots or as keyword argument names."[^3] Example: [`args-by-name`](#args-by-name).

**r6.** "Note that the order in which the keyword arguments are printed is guaranteed to match the order in which they were provided in the function call."[^1]

## Examples

```python run id=empty
def show(*args, **kwargs):
    print(type(args).__name__, args, type(kwargs).__name__, kwargs)

show()
show(1, 2, x=3)
```

```output for=empty
tuple () dict {}
tuple (1, 2) dict {'x': 3}
```

With no extra arguments, `args` is an empty tuple and `kwargs` is an empty dict (rules r1 and r2).

```python run id=after-args
def total(*numbers, start=0):
    return start + sum(numbers)

print(total(1, 2, 3))
print(total(1, 2, 3, start=10))
```

```output for=after-args
6
16
```

```python run id=forward
def logged(func):
    def wrapper(*args, **kwargs):
        print("call", func.__name__, args, kwargs)
        return func(*args, **kwargs)
    return wrapper

def power(base, exp=2):
    return base ** exp

p = logged(power)
print(p(3, exp=3))
```

```output for=forward
call power (3,) {'exp': 3}
27
```

A wrapper takes each argument with `*args` and `**kwargs`, and it passes them on with the same syntax in the call.

## Pitfalls

The name of a `*args` parameter is not a keyword that a call can use:

```python fails id=args-by-name expect=exception:TypeError
def total(*numbers):
    return sum(numbers)

total(numbers=(1, 2))
```

A parameter after `*args` with no default must be given by keyword. `total(1, 2, 3)` puts all three numbers in `numbers`, so a positional `start` is never filled (rule r4).

`**kwargs` before `*args` is a syntax error (rule r3):

```python fails id=kwargs-first expect=exception:SyntaxError
def f(**kwargs, *args):
    pass
```

## Comparison

A Go function can have one final variadic parameter. "If f is variadic with a final parameter p of type ...T, then within f the type of p is equivalent to type []T."[^4] All the variadic arguments must have type T, and Go has no form for extra named arguments. A Python `*args` tuple can hold values of any type, and `**kwargs` holds extra keyword arguments (rules r1 and r2).
