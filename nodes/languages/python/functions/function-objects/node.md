---
tag: HJ252G
kind: feature
owner: python
slug: function-objects
name: Function objects and first-class functions
aliases: [first-class function, function as a value, pass a function as an argument, higher-order function, __name__, __defaults__, function attributes, __qualname__]
area: languages/python/functions
level: advanced
summary: >-
  A function is an object. A program can bind it to other names, store it in a list or dict, pass it as an argument and return it. It has attributes such as __name__, __doc__ and __defaults__.
grammar: null
spec: {standard: null, url: "https://docs.python.org/3.15/reference/datamodel.html#user-defined-functions", locator: "3.2.8.1. User-defined functions"}
see_also: [python/defining-functions, python/lambda-expressions, python/closures-and-nonlocal, python/callable-objects, python/functools]
sources:
  - url: https://docs.python.org/3.15/reference/datamodel.html#user-defined-functions
    title: The Python Language Reference, Python 3.15
    locator: 3.2.8.1. User-defined functions; 3.2.8.1.2. Special writable attributes
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/reference/compound_stmts.html#function-definitions
    title: The Python Language Reference, Python 3.15
    locator: 8.7. Function definitions
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/tutorial/controlflow.html#defining-functions
    title: The Python Tutorial, 4. More Control Flow Tools
    locator: 4.8. Defining Functions
    accessed: 2026-11-16
  - url: https://go.dev/ref/spec#Function_types
    title: The Go Programming Language Specification
    locator: Function types
    accessed: 2026-11-16
---

## Synopsis

```python run id=dispatch
def add(a, b):
    return a + b

def mul(a, b):
    return a * b

operations = {"+": add, "*": mul}
for symbol in ["+", "*"]:
    print(symbol, operations[symbol](3, 4))

def apply_twice(func, value):
    return func(func(value, value), value)

print(apply_twice(add, 2))
```

```output for=dispatch
+ 7
* 12
6
```

The dict holds functions as values, and `apply_twice` takes a function as an argument.

## Semantics

**r1.** "Programmer’s note: Functions are first-class objects. A “def” statement executed inside a function definition defines a local function that can be returned or passed around."[^2] Example: [`make-adder`](#make-adder).

**r2.** "A user-defined function object is created by a function definition".[^1]

**r3.** "A function definition associates the function name with the function object in the current symbol table." "Other names can also point to that same function object and can also be used to access the function".[^3]

**r4.** A function has writable attributes. Among them are `__doc__`, "The function’s documentation string, or None if unavailable", `__name__`, "The function’s name", and `__defaults__`, "A tuple containing default parameter values for those parameters that have defaults, or None if no parameters have a default value."[^1] Example: [`attributes`](#attributes).

**r5.** "Function objects also support getting and setting arbitrary attributes, which can be used, for example, to attach metadata to functions. Regular attribute dot-notation is used to get and set such attributes."[^1] Example: [`custom-attribute`](#custom-attribute).

## Examples

```python run id=make-adder
def make_adder(n):
    def add(x):
        return x + n
    return add

add5 = make_adder(5)
print(add5(10), add5.__qualname__)
```

```output for=make-adder
15 make_adder.<locals>.add
```

`make_adder` returns a local function (rule r1). `__qualname__` is "The function’s qualified name".[^1]

```python run id=attributes
def greet(name, greeting="Hello", *, mark="!"):
    """Return a greeting."""
    return f"{greeting}, {name}{mark}"

print(greet.__name__)
print(greet.__doc__)
print(greet.__defaults__)
print(greet.__kwdefaults__)
```

```output for=attributes
greet
Return a greeting.
('Hello',)
{'mark': '!'}
```

The defaults of keyword-only parameters are not in `__defaults__`. `__kwdefaults__` is "A dictionary containing defaults for keyword-only parameters."[^1]

```python run id=custom-attribute
def handler():
    return "ok"

handler.route = "/status"
print(handler.route, handler())
```

```output for=custom-attribute
/status ok
```

## Pitfalls

A function name without parentheses is the function object, not a call:

```python wrong id=no-call
def ready():
    return False

if ready:
    print("ready")
```

```output for=no-call
ready
```

`if ready:` tests the function object, not the result of a call. Write `if ready():`.

## Comparison

Go functions are also values: "A function type denotes the set of all functions with the same parameter and result types."[^4] A Go variable of a function type can hold a function, and "The value of an uninitialized variable of function type is nil."[^4] A Python function is also an object with its own attributes, and a program can add more (rule r5).
