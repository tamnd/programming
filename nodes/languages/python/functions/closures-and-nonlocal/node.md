---
tag: THZ00V
kind: feature
owner: python
slug: closures-and-nonlocal
name: Closures and nonlocal
aliases: [closure, closure variable, free variable, nested function, inner function, function factory, __closure__, late binding closure, counter with nonlocal]
area: languages/python/functions
level: core
summary: >-
  A nested function can use the variables of the function that contains it, and it keeps them after the outer call returns. To assign to such a variable, the nested function declares it nonlocal.
grammar: "nonlocal_stmt: 'nonlocal' identifier (',' identifier)* (Language Reference, 7.13)"
spec: {standard: null, url: "https://docs.python.org/3.15/reference/executionmodel.html#resolution-of-names", locator: "4.2.2. Resolution of names"}
see_also: [python/nonlocal-statement, python/naming-and-binding, python/name-resolution, python/function-objects, python/lambda-expressions]
sources:
  - url: https://docs.python.org/3.15/glossary.html#term-closure-variable
    title: Glossary, Python 3.15
    locator: closure variable; free variable
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/reference/executionmodel.html#resolution-of-names
    title: The Python Language Reference, Python 3.15
    locator: 4.2.1. Binding of names; 4.2.2. Resolution of names
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/reference/datamodel.html#user-defined-functions
    title: The Python Language Reference, Python 3.15
    locator: 3.2.8.1.1. Special read-only attributes
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/faq/programming.html#why-do-lambdas-defined-in-a-loop-with-different-values-all-return-the-same-result
    title: Programming FAQ, Python 3.15
    locator: Why do lambdas defined in a loop with different values all return the same result?
    accessed: 2026-11-16
  - url: https://go.dev/ref/spec#Function_literals
    title: The Go Programming Language Specification
    locator: Function literals
    accessed: 2026-11-16
---

## Synopsis

```python run id=counter
def make_counter():
    count = 0
    def increment():
        nonlocal count
        count += 1
        return count
    return increment

c = make_counter()
print(c(), c(), c())
d = make_counter()
print(d())
```

```output for=counter
1 2 3
1
```

`increment` keeps `count` after `make_counter` returns. Each call of `make_counter` makes a new `count`.

## Semantics

**r1.** "If a variable is used in a code block but not defined there, it is a free variable."[^2]

**r2.** A closure variable is "A free variable referenced from a nested scope that is defined in an outer scope rather than being resolved at runtime from the globals or builtin namespaces. May be explicitly defined with the nonlocal keyword to allow write access, or implicitly defined if the variable is only being read."[^1] Example: [`read-only`](#read-only).

**r3.** "If the definition occurs in a function block, the scope extends to any blocks contained within the defining one, unless a contained block introduces a different binding for the name."[^2]

**r4.** "If a name is bound in a block, it is a local variable of that block, unless declared as nonlocal or global."[^2] Example: [`no-nonlocal`](#no-nonlocal).

**r5.** "The nonlocal statement causes corresponding names to refer to previously bound variables in the nearest enclosing function scope. SyntaxError is raised at compile time if the given name does not exist in any enclosing function scope."[^2] Example: [`no-enclosing`](#no-enclosing).

**r6.** A function has the attribute `__closure__`: "None or a tuple of cells that contain bindings for the names specified in the co_freevars attribute of the function’s code object."[^3] Example: [`cells`](#cells).

## Examples

```python run id=read-only
def make_multiplier(factor):
    def multiply(x):
        return x * factor
    return multiply

double = make_multiplier(2)
triple = make_multiplier(3)
print(double(5), triple(5))
```

```output for=read-only
10 15
```

`multiply` only reads `factor`, so it needs no nonlocal (rule r2).

```python run id=cells
def outer():
    x = 0
    def inner():
        nonlocal x
        x += 1
        return x
    return inner

f = outer()
f()
f()
print(f.__code__.co_freevars, f.__closure__[0].cell_contents)
```

```output for=cells
('x',) 2
```

This is the example of the glossary. The cell holds the current value of `x` (rule r6).

## Pitfalls

An assignment in the nested function makes a new local name, so `+=` on the outer name fails:

```python fails id=no-nonlocal expect=exception:UnboundLocalError
def make_counter():
    count = 0
    def increment():
        count += 1
        return count
    return increment

make_counter()()
```

`count += 1` binds `count`, so `count` is local to `increment` (rule r4). Add `nonlocal count`.

nonlocal needs a variable in an enclosing function, not a global:

```python fails id=no-enclosing expect=exception:SyntaxError
total = 0

def add(n):
    nonlocal total
    total += n
```

A closure reads the variable, not its value at the time when the closure was made:

```python wrong id=late-binding
functions = []
for i in range(3):
    def show():
        return i
    functions.append(show)

print([f() for f in functions])
```

```output for=late-binding
[2, 2, 2]
```

The FAQ explains this for "lambdas (or even plain functions)": the variable "is accessed when the lambda is called".[^4] Pass the value as a default, `def show(i=i)`, to keep the current value.

## Comparison

Go function literals are also closures: "Function literals are closures: they may refer to variables declared in a surrounding function. Those variables are then shared between the surrounding function and the function literal, and they survive as long as they are accessible."[^5] A Go closure can assign to such a variable with no extra keyword. A Python nested function needs nonlocal to assign to it (rule r4).
