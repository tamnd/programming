---
tag: T729GH
kind: feature
owner: python
slug: defining-functions
name: Defining functions
aliases: [def statement, define a function, function definition, docstring, function body, def keyword]
area: languages/python/functions
level: intro
summary: >-
  The def statement makes a function object and binds it to a name. The body runs only when the function is called, and each call gets a new local symbol table.
grammar: "funcdef: [decorators] 'def' funcname [type_params] '(' [parameter_list] ')' ['->' expression] ':' suite (Language Reference, 8.7)"
spec: {standard: null, url: "https://docs.python.org/3.15/reference/compound_stmts.html#function-definitions", locator: "8.7. Function definitions"}
see_also: [python/return-statement, python/default-argument-values, python/keyword-arguments, python/function-objects, python/lambda-expressions, python/naming-and-binding]
sources:
  - url: https://docs.python.org/3.15/tutorial/controlflow.html#defining-functions
    title: The Python Tutorial, 4. More Control Flow Tools
    locator: 4.8. Defining Functions
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/reference/compound_stmts.html#function-definitions
    title: The Python Language Reference, Python 3.15
    locator: 8.7. Function definitions
    accessed: 2026-11-16
  - url: https://go.dev/ref/spec#Function_declarations
    title: The Go Programming Language Specification
    locator: Function declarations; Declarations and scope
    accessed: 2026-11-16
---

## Synopsis

```python run id=fib
def fib(n):
    """Return the Fibonacci numbers less than n."""
    result = []
    a, b = 0, 1
    while a < n:
        result.append(a)
        a, b = b, a + b
    return result

print(fib(100))
print(fib.__doc__)
```

```output for=fib
[0, 1, 1, 2, 3, 5, 8, 13, 21, 34, 55, 89]
Return the Fibonacci numbers less than n.
```

`def` gives the name `fib` to a new function. The string on the first line of the body is the docstring.

## Semantics

**r1.** "The keyword def introduces a function definition. It must be followed by the function name and the parenthesized list of formal parameters. The statements that form the body of the function start at the next line, and must be indented."[^1]

**r2.** "A function definition is an executable statement. Its execution binds the function name in the current local namespace to a function object".[^2] Example: [`runs-when-reached`](#runs-when-reached).

**r3.** "The function definition does not execute the function body; this gets executed only when the function is called."[^2] Example: [`body-later`](#body-later).

**r4.** "The first statement of the function body can optionally be a string literal; this string literal is the function’s documentation string, or docstring."[^1] Example: [`fib`](#fib).

**r5.** "The execution of a function introduces a new symbol table used for the local variables of the function. More precisely, all variable assignments in a function store the value in the local symbol table".[^1] Example: [`local`](#local).

**r6.** Arguments "are passed using call by value (where the value is always an object reference, not the value of the object)."[^1] Example: [`object-reference`](#object-reference).

**r7.** "Other names can also point to that same function object and can also be used to access the function".[^1] Example: [`other-name`](#other-name).

**r8.** "Functions are first-class objects. A “def” statement executed inside a function definition defines a local function that can be returned or passed around."[^2]

## Examples

```python run id=body-later
def broken():
    return undefined_name

print("defined")
```

```output for=body-later
defined
```

The body uses a name that does not exist, but Python does not run the body, so there is no error (rule r3).

```python run id=runs-when-reached
fast = True
if fast:
    def speed():
        return "fast"
else:
    def speed():
        return "slow"

print(speed())
```

```output for=runs-when-reached
fast
```

`def` is a statement, so Python runs only the `def` that it reaches (rule r2).

```python run id=local
total = 0

def add(n):
    total = n
    return total

print(add(5), total)
```

```output for=local
5 0
```

The assignment in `add` makes a local `total`. The global `total` does not change (rule r5).

```python run id=object-reference
def grow(items):
    items.append(4)
    items = [0]

numbers = [1, 2, 3]
grow(numbers)
print(numbers)
```

```output for=object-reference
[1, 2, 3, 4]
```

`append` changes the list that the caller gave. The assignment `items = [0]` binds only the local name (rule r6).

```python run id=other-name
def greet(name):
    return "hello " + name

say = greet
print(say("ann"), say is greet, greet.__name__)
```

```output for=other-name
hello ann True greet
```

## Pitfalls

A call before the `def` statement runs is a NameError, because the name is not bound yet (rule r2):

```python fails id=call-before-def expect=exception:NameError
print(square(3))

def square(x):
    return x * x
```

A function with no return statement gives None:

```python wrong id=forgot-return
def double(x):
    x * 2

print(double(4))
```

```output for=forgot-return
None
```

The tutorial says: "even functions without a return statement do return a value, albeit a rather boring one. This value is called None".[^1] Write `return x * 2`.

## Comparison

In Go, "A function declaration binds an identifier, the function name, to a function."[^3] In Go, "The scope of an identifier denoting a constant, type, variable, or function (but not method) declared at top level (outside any function) is the package block."[^3] So a Go function can call a function that is declared later in the file. A Go signature gives a type to each parameter, and "If the function's signature declares result parameters, the function body's statement list must end in a terminating statement."[^3] A Python def has no types that Python checks, and a function without return gives None (rule r1).
