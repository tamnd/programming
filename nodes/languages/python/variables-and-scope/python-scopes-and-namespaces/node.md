---
tag: TW40FH
kind: feature
owner: python
slug: python-scopes-and-namespaces
name: Python Scopes and Namespaces
aliases: [LEGB rule, Python scope rules, local enclosing global built-in, Python namespace, variable scope in Python, Python block scope]
area: languages/python/variables-and-scope
level: core
summary: >-
  A namespace maps names to objects. A scope is a region of the program where a namespace is directly accessible. Python searches the local, enclosing, global and built-in scopes, in this order.
grammar: null
spec: {standard: null, url: "https://docs.python.org/3.15/tutorial/classes.html#python-scopes-and-namespaces", locator: "9.2. Python Scopes and Namespaces"}
see_also: [python/naming-and-binding, python/name-resolution, python/global-statement, python/nonlocal-statement, python/the-builtins-namespace, python/a-word-about-names-and-objects]
sources:
  - url: https://docs.python.org/3.15/tutorial/classes.html#python-scopes-and-namespaces
    title: The Python Tutorial, 9. Classes
    locator: 9.2. Python Scopes and Namespaces; 9.2.1. Scopes and Namespaces Example
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/faq/programming.html#what-are-the-rules-for-local-and-global-variables-in-python
    title: Programming FAQ, Python 3.15 documentation
    locator: What are the rules for local and global variables in Python?
    accessed: 2026-11-16
  - url: https://go.dev/ref/spec#Declarations_and_scope
    title: The Go Programming Language Specification
    locator: Blocks; Declarations and scope
    accessed: 2026-11-16
---

## Synopsis

```python run id=scope-test
def scope_test():
    def do_local():
        spam = "local spam"

    def do_nonlocal():
        nonlocal spam
        spam = "nonlocal spam"

    def do_global():
        global spam
        spam = "global spam"

    spam = "test spam"
    do_local()
    print("After local assignment:", spam)
    do_nonlocal()
    print("After nonlocal assignment:", spam)
    do_global()
    print("After global assignment:", spam)

scope_test()
print("In global scope:", spam)
```

```output for=scope-test
After local assignment: test spam
After nonlocal assignment: nonlocal spam
After global assignment: nonlocal spam
In global scope: global spam
```

This is the example of the tutorial. A plain assignment binds a local name. `nonlocal` rebinds the name of the enclosing function, and `global` rebinds the name of the module.

## Semantics

**r1.** "A namespace is a mapping from names to objects." Examples are "the set of built-in names (containing functions such as abs(), and built-in exception names); the global names in a module; and the local names in a function invocation." "there is absolutely no relation between names in different namespaces".[^1] Example: [`namespaces`](#namespaces).

**r2.** Namespaces have different lifetimes. The built-in namespace "is created when the Python interpreter starts up, and is never deleted". "The global namespace for a module is created when the module definition is read in". "The local namespace for a function is created when the function is called, and deleted when the function returns or raises an exception that is not handled within the function." "recursive invocations each have their own local namespace."[^1] Example: [`recursion`](#recursion).

**r3.** "A scope is a textual region of a Python program where a namespace is directly accessible." At any time, there are 3 or 4 nested scopes. Python searches "the innermost scope, which is searched first", then "the scopes of any enclosing functions", then "the current module's global names", and last "the namespace containing built-in names".[^1] Example: [`lookup-order`](#lookup-order).

**r4.** "if no global or nonlocal statement is in effect – assignments to names always go into the innermost scope. Assignments do not copy data — they just bind names to objects."[^1] Without `nonlocal`, an outer variable is read-only: "an attempt to write to such a variable will simply create a new local variable in the innermost scope, leaving the identically named outer variable unchanged".[^1] Example: [`scope-test`](#scope-test).

**r5.** The FAQ gives the same rule: "variables that are only referenced inside a function are implicitly global. If a variable is assigned a value anywhere within the function's body, it's assumed to be a local unless explicitly declared as global."[^2] Example: [`assigned-anywhere`](#assigned-anywhere).

**r6.** "scopes are determined textually: the global scope of a function defined in a module is that module's namespace, no matter from where or by what alias the function is called." "In fact, local variables are already determined statically."[^1]

**r7.** "Class definitions place yet another namespace in the local scope."[^1] Names of a class body are not in the scope of its methods (see naming and binding). Example: [`class-namespace`](#class-namespace).

## Examples

```python run id=namespaces
import math

def maximize():
    return "local maximize"

pi = "my pi"
print(maximize(), pi, math.pi)
```

```output for=namespaces
local maximize my pi 3.141592653589793
```

The global name `pi` and the attribute `math.pi` are in different namespaces, so they do not conflict (rule r1).

```python run id=recursion
def collect(n):
    if n == 0:
        return [n]
    return collect(n - 1) + [n]

print(collect(3))
```

```output for=recursion
[0, 1, 2, 3]
```

Each call has its own local `n` (rule r2).

```python run id=lookup-order
x = "global"

def outer():
    x = "enclosing"
    def inner():
        return x, len("abc")
    return inner()

print(outer())
```

```output for=lookup-order
('enclosing', 3)
```

`inner` finds `x` in the enclosing function and `len` in the built-in names (rule r3).

```python run id=class-namespace
class Counter:
    step = 2

    def next(self, n):
        return n + Counter.step

print(Counter().next(1))
```

```output for=class-namespace
3
```

The method uses `Counter.step`. A plain `step` in the method body is not in its scope.

## Pitfalls

An assignment anywhere in a function makes the name local in the whole function, also before the assignment:

```python fails id=assigned-anywhere expect=exception:UnboundLocalError
x = 10

def foo():
    print(x)
    x += 1

foo()
```

Use `global x` to change the module variable, or give the local variable a different name (rule r5).

Python has no block scope for `if` and `for`. A name bound in a loop is a local name of the function, so it is still bound after the loop:

```python run id=no-block-scope
def last_item(items):
    for item in items:
        pass
    return item

print(last_item([1, 2, 3]))
```

```output for=no-block-scope
3
```

The tutorial lists only the local, enclosing, global and built-in scopes (rule r3), so an indented block does not make a new scope.

## Comparison

"Go is lexically scoped using blocks". In Go, "The scope of a constant or variable identifier declared inside a function begins at the end of the ConstSpec or VarSpec (ShortVarDecl for short variable declarations) and ends at the end of the innermost containing block." "An identifier declared in a block may be redeclared in an inner block."[^3] So a Go variable declared in a `for` body is not visible after the loop. In Python, the function is the smallest scope, and an indented block does not make a new scope (example [`no-block-scope`](#no-block-scope)).
