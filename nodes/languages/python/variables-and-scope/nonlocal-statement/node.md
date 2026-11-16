---
tag: K661JQ
kind: feature
owner: python
slug: nonlocal-statement
name: The nonlocal statement
aliases: [nonlocal keyword, rebind a variable in an enclosing function, closure counter in Python, PEP 3104, modify outer variable in nested function]
area: languages/python/variables-and-scope
level: core
summary: >-
  nonlocal x makes x refer to the variable of the nearest enclosing function, so a nested function can rebind it. The name must already be bound in an enclosing function, or the code is a SyntaxError.
grammar: "nonlocal_stmt: 'nonlocal' identifier (',' identifier)* (Language Reference, 7.13)"
spec: {standard: null, url: "https://docs.python.org/3.15/reference/simple_stmts.html#the-nonlocal-statement", locator: "7.13. The nonlocal statement"}
see_also: [python/global-statement, python/python-scopes-and-namespaces, python/name-resolution, python/naming-and-binding]
sources:
  - url: https://docs.python.org/3.15/reference/simple_stmts.html#the-nonlocal-statement
    title: The Python Language Reference, Python 3.15
    locator: 7.13. The nonlocal statement
    accessed: 2026-11-16
  - url: https://peps.python.org/pep-3104/#abstract
    title: PEP 3104 - Access to Names in Outer Scopes
    locator: Abstract
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/reference/executionmodel.html#resolution-of-names
    title: The Python Language Reference, Python 3.15
    locator: 4.2.2. Resolution of names
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
```

```output for=counter
1 2 3
```

`increment` rebinds `count` of `make_counter`. The variable lives on after `make_counter` returns.

## Semantics

**r1.** "When the definition of a function or class is nested (enclosed) within the definitions of other functions, its nonlocal scopes are the local scopes of the enclosing functions. The nonlocal statement causes the listed identifiers to refer to names previously bound in nonlocal scopes. It allows encapsulated code to rebind such nonlocal identifiers."[^1] Example: [`counter`](#counter).

**r2.** "If a name is bound in more than one nonlocal scope, the nearest binding is used. If a name is not bound in any nonlocal scope, or if there is no nonlocal scope, a SyntaxError is raised."[^1] Examples: [`nearest-binding`](#nearest-binding), [`not-bound`](#not-bound).

**r3.** "The nonlocal statement applies to the entire scope of a function or class body. A SyntaxError is raised if a variable is used or assigned to prior to its nonlocal declaration in the scope."[^1]

**r4.** The `nonlocal` statement does not reach module globals. It refers "to previously bound variables in the nearest enclosing function scope".[^3] Use `global` for a module variable. Example: [`module-level`](#module-level).

**r5.** "nonlocal is a directive to the parser and applies only to code parsed along with it."[^1]

**r6.** Before PEP 3104, "Python code can refer to a name in any enclosing scope, but it can only rebind names in two scopes: the local scope (by simple assignment) or the module-global scope (using a global declaration)."[^2] The `nonlocal` statement removes this limit.

## Examples

```python run id=nearest-binding
def outer():
    x = "outer"
    def middle():
        x = "middle"
        def inner():
            nonlocal x
            x = "changed"
        inner()
        return x
    return middle(), x

print(outer())
```

```output for=nearest-binding
('changed', 'outer')
```

`inner` rebinds the `x` of `middle`, which is the nearest binding. The `x` of `outer` does not change (rule r2).

```python fails id=not-bound expect=exception:SyntaxError
def outer():
    def inner():
        nonlocal y
        y = 1
    inner()
```

```python fails id=module-level expect=exception:SyntaxError
count = 0

def f():
    nonlocal count
    count = 1
```

## Pitfalls

Without `nonlocal`, an assignment in the nested function makes a new local variable:

```python fails id=forgot-nonlocal expect=exception:UnboundLocalError
def make_counter():
    count = 0
    def increment():
        count += 1
        return count
    return increment

make_counter()()
```

`count += 1` binds `count`, so `count` is local to `increment`, and it has no value when `+=` reads it.

A change to a mutable object needs no `nonlocal`, because it does not rebind the name. For example, `items.append(x)` works in a nested function without `nonlocal items`. Use `nonlocal` only to rebind the name.

## Comparison

In Go, "Function literals are closures: they may refer to variables declared in a surrounding function. Those variables are then shared between the surrounding function and the function literal, and they survive as long as they are accessible."[^4] A Go closure can assign to such a variable with a plain assignment. Python needs `nonlocal` for this, because an assignment in a Python function binds a local name (rule r1).
