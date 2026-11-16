---
tag: CK9YCH
kind: feature
owner: python
slug: scope-and-dynamic-features
name: Scope and dynamic features
aliases: [exec and scope, eval and scope, globals() dictionary, late binding of free variables, exec cannot see enclosing variables, dynamic name lookup]
area: languages/python/variables-and-scope
level: advanced
summary: >-
  Python looks up free variables when the code runs. Code run by exec() and eval() sees only the local and global namespaces of the caller, and it does not see the variables of enclosing functions.
grammar: null
spec: {standard: null, url: "https://docs.python.org/3.15/reference/executionmodel.html#interaction-with-dynamic-features", locator: "4.2.6. Interaction with dynamic features"}
see_also: [python/name-resolution, python/global-statement, python/the-builtins-namespace, python/python-scopes-and-namespaces]
sources:
  - url: https://docs.python.org/3.15/reference/executionmodel.html#interaction-with-dynamic-features
    title: The Python Language Reference, Python 3.15
    locator: 4.2.6. Interaction with dynamic features; footnote 1
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/builtins/functions.html#exec
    title: Built-in Functions, Python 3.15 documentation
    locator: exec(); eval(); globals()
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/reference/simple_stmts.html#the-global-statement
    title: The Python Language Reference, Python 3.15
    locator: 7.12. The global statement, Programmer's note
    accessed: 2026-11-16
  - url: https://go.dev/ref/spec#Declarations_and_scope
    title: The Go Programming Language Specification
    locator: Declarations and scope
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/faq/programming.html#why-do-lambdas-defined-in-a-loop-with-different-values-all-return-the-same-result
    title: Programming FAQ, Python 3.15 documentation
    locator: Why do lambdas defined in a loop with different values all return the same result?
    accessed: 2026-11-16
---

## Synopsis

```python run id=late-lookup
i = 10

def f():
    print(i)

i = 42
f()
```

```output for=late-lookup
42
```

`f` reads the global `i` when it runs, so it prints the value at the time of the call. This is the example of the reference.

## Semantics

**r1.** "Name resolution of free variables occurs at runtime, not at compile time."[^1] Example: [`late-lookup`](#late-lookup).

**r2.** "The eval() and exec() functions do not have access to the full environment for resolving names. Names may be resolved in the local and global namespaces of the caller. Free variables are not resolved in the nearest enclosing namespace, but in the global namespace."[^1] The reason is that "the code that is executed by these operations is not available at the time the module is compiled."[^1] Example: [`exec-enclosing`](#exec-enclosing).

**r3.** "The exec() and eval() functions have optional arguments to override the global and local namespace. If only one namespace is specified, it is used for both."[^1] Example: [`own-namespace`](#own-namespace).

**r4.** For `exec()`, "if the optional parts are omitted, the code is executed in the current scope." "Be aware that the nonlocal, yield, and return statements may not be used outside of function definitions even within the context of code passed to the exec() function."[^2]

**r5.** `globals()` returns "the dictionary implementing the current module namespace. For code within functions, this is set when the function is defined and remains the same regardless of where the function is called."[^2] Example: [`globals-dict`](#globals-dict).

**r6.** "global is a directive to the parser. It applies only to code parsed at the same time as the global statement." A `global` statement in a string given to `exec()` "does not affect the code block containing the function call".[^3] Example: [`global-in-exec`](#global-in-exec).

## Examples

```python run id=own-namespace
ns = {}
exec("y = 2 * 3", ns)
print(ns["y"], "y" in globals())
print(eval("a + b", {"a": 1}, {"b": 2}))
```

```output for=own-namespace
6 False
3
```

`exec()` binds `y` in the dictionary `ns`, not in the module (rule r3).

```python run id=globals-dict
globals()["created"] = "by globals()"
print(created)
```

```output for=globals-dict
by globals()
```

`globals()` is the module namespace itself, so a new key is a new global variable (rule r5).

```python run id=global-in-exec
x = "global"

def f():
    exec("global x")
    x = "local"
    return x

print(f(), x)
```

```output for=global-in-exec
local global
```

The `global x` in the string does not apply to `f`, so the assignment in `f` binds a local name (rule r6).

## Pitfalls

`eval()` and `exec()` in a nested function do not see the variables of the enclosing function:

```python wrong id=exec-enclosing
x = "global"

def outer():
    x = "enclosing"
    def inner():
        return eval("x")
    return inner()

print(outer())
```

```output for=exec-enclosing
global
```

`inner` does not use `x` in its own code, so `x` is not a free variable of `inner`, and `eval()` finds the global `x` (rule r2). Pass the value in a namespace, as in `eval('x', {'x': x})`, or avoid `eval()`.

The value of a free variable can change after a function is defined (rule r1). Functions made in a loop all see the last value of the loop variable:

```python wrong id=loop-lambdas
squares = []
for x in range(3):
    squares.append(lambda: x ** 2)
print([f() for f in squares])
```

```output for=loop-lambdas
[4, 4, 4]
```

The FAQ says: "you need to save the values in variables local to the lambdas". With `lambda n=x: n**2`, the default value "creates a new variable n local to the lambda and computed when the lambda is defined".[^5]

The documentation of `exec()` warns that "Calling it with untrusted user-supplied input will lead to security vulnerabilities."[^2]

## Comparison

In Go, "Every identifier in a program must be declared", and "The scope of a declared identifier is the extent of source text in which the identifier denotes the specified constant, type, variable, function, label, or package."[^4] So the scope of each Go name is fixed by the source text. In Python, `exec()` and `globals()` can create names while the program runs, and Python looks up free variables at run time (rules r1 and r5).
