---
tag: S9KM9F
kind: feature
owner: python
slug: global-statement
name: The global statement
aliases: [global keyword, Python global variable, change a global variable in a function, module-level variable, global x]
area: languages/python/variables-and-scope
level: core
summary: >-
  global x makes x refer to the module-level variable in the whole current scope, so the function can rebind it. Reading a global needs no declaration. Only rebinding needs one.
grammar: "global_stmt: 'global' identifier (',' identifier)* (Language Reference, 7.12)"
spec: {standard: null, url: "https://docs.python.org/3.15/reference/simple_stmts.html#the-global-statement", locator: "7.12. The global statement"}
see_also: [python/nonlocal-statement, python/python-scopes-and-namespaces, python/name-resolution, python/naming-and-binding, python/scope-and-dynamic-features]
sources:
  - url: https://docs.python.org/3.15/reference/simple_stmts.html#the-global-statement
    title: The Python Language Reference, Python 3.15
    locator: 7.12. The global statement
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/faq/programming.html#what-are-the-rules-for-local-and-global-variables-in-python
    title: Programming FAQ, Python 3.15 documentation
    locator: What are the rules for local and global variables in Python?; How do I share global variables across modules?
    accessed: 2026-11-16
  - url: https://go.dev/ref/spec#Declarations_and_scope
    title: The Go Programming Language Specification
    locator: Declarations and scope; Assignment statements
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/reference/executionmodel.html#resolution-of-names
    title: The Python Language Reference, Python 3.15
    locator: 4.2.2. Resolution of names
    accessed: 2026-11-16
---

## Synopsis

```python run id=counter
count = 0

def increment():
    global count
    count += 1

increment()
increment()
print(count)
```

```output for=counter
2
```

Without `global count`, the assignment would make `count` local to `increment`, and `count += 1` would raise `UnboundLocalError`.

## Semantics

**r1.** "The global statement causes the listed identifiers to be interpreted as globals. It would be impossible to assign to a global variable without global, although free variables may refer to globals without being declared global."[^1] Examples: [`counter`](#counter), [`read-only`](#read-only).

**r2.** "The global statement applies to the entire current scope (module, function body or class definition)."[^1]

**r3.** "A SyntaxError is raised if a variable is used or assigned to prior to its global declaration in the scope."[^1] Example: [`use-before-global`](#use-before-global).

**r4.** "At the module level, all variables are global, so a global statement has no effect."[^1]

**r5.** A `global` statement can create a module variable. If the name is not in the global or builtins namespace, "new variables are created in the global namespace".[^4] Example: [`create-global`](#create-global).

**r6.** "global is a directive to the parser. It applies only to code parsed at the same time as the global statement." A `global` statement in a string given to `exec()` does not affect the code that calls `exec()`.[^1]

**r7.** The FAQ gives the reason for the rule: "requiring global for assigned variables provides a bar against unintended side-effects. On the other hand, if global was required for all global references, you'd be using global all the time."[^2]

## Examples

```python run id=read-only
limit = 3
items = []

def add(x):
    if len(items) < limit:
        items.append(x)

add("a")
add("b")
print(items)
```

```output for=read-only
['a', 'b']
```

`add` reads `limit` and calls a method of `items`, but it does not rebind either name, so it needs no `global` statement (rule r1).

```python run id=create-global
def setup():
    global config
    config = {"debug": True}

setup()
print(config)
```

```output for=create-global
{'debug': True}
```

```python fails id=use-before-global expect=exception:SyntaxError
x = 1

def f():
    print(x)
    global x
    x = 2
```

## Pitfalls

A function that forgets `global` makes a new local variable, and the global variable does not change:

```python wrong id=forgot-global
total = 0

def reset_to(n):
    total = n

reset_to(5)
print(total)
```

```output for=forgot-global
0
```

`global` applies only to the module that contains the function. To share a value across modules, the FAQ recommends a module: "The canonical way to share information across modules within a single program is to create a special module (often called config or cfg)."[^2]

## Comparison

In Go, "The scope of an identifier denoting a constant, type, variable, or function (but not method) declared at top level (outside any function) is the package block."[^3] A Go function can assign to a package-level variable with no extra statement, because "An assignment replaces the current value stored in a variable" and does not declare a new one.[^3] In Python, an assignment in a function binds a local name, so `global` is necessary to rebind a module variable (rule r1).
