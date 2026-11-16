---
tag: TZYBET
kind: feature
owner: python
slug: naming-and-binding
name: Naming and binding
aliases: [Python name binding, binding operations, code blocks in Python, local and free variables, what makes a variable local in Python, del unbinds a name]
area: languages/python/variables-and-scope
level: advanced
summary: >-
  Names refer to objects. Binding operations such as assignment, def, class, import, for and as introduce names. A name bound in a block is local to that block unless it is declared global or nonlocal.
grammar: null
spec: {standard: null, url: "https://docs.python.org/3.15/reference/executionmodel.html#binding-of-names", locator: "4.2.1. Binding of names"}
see_also: [python/name-resolution, python/python-scopes-and-namespaces, python/global-statement, python/nonlocal-statement, python/assignment-statements]
sources:
  - url: https://docs.python.org/3.15/reference/executionmodel.html#binding-of-names
    title: The Python Language Reference, Python 3.15
    locator: 4.1. Structure of a program; 4.2.1. Binding of names; 4.2.2. Resolution of names
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/reference/compound_stmts.html#except-clause
    title: The Python Language Reference, Python 3.15
    locator: 8.4.1. except clause
    accessed: 2026-11-16
  - url: https://go.dev/ref/spec#Declarations_and_scope
    title: The Go Programming Language Specification
    locator: Declarations and scope
    accessed: 2026-11-16
---

## Synopsis

```python run id=binders
def f(param):
    import math
    for i in range(1):
        pass
    class K:
        pass
    def g():
        pass
    if (n := 2):
        pass
    return sorted(locals())

print(f(0))
```

```output for=binders
['K', 'g', 'i', 'math', 'n', 'param']
```

Each of these constructs binds a name in the local namespace of `f`.

## Semantics

**r1.** "A Python program is constructed from code blocks. A block is a piece of Python program text that is executed as a unit. The following are blocks: a module, a function body, and a class definition."[^1]

**r2.** "Names refer to objects. Names are introduced by name binding operations." The constructs that bind names are formal parameters, class definitions, function definitions, assignment expressions, targets of assignments, `for` loop headers, names after `as` in `with`, `except` and patterns, capture patterns, `import` statements, `type` statements and type parameter lists.[^1] Example: [`binders`](#binders).

**r3.** "A target occurring in a del statement is also considered bound for this purpose (though the actual semantics are to unbind the name)."[^1] Example: [`del-unbinds`](#del-unbinds).

**r4.** "The import statement of the form from ... import * binds all names defined in the imported module, except those beginning with an underscore. This form may only be used at the module level."[^1]

**r5.** "If a name is bound in a block, it is a local variable of that block, unless declared as nonlocal or global. If a name is bound at the module level, it is a global variable. (The variables of the module code block are local and global.) If a variable is used in a code block but not defined there, it is a free variable."[^1] Examples: [`module-level`](#module-level), [`free-variable`](#free-variable).

**r6.** "Each occurrence of a name in the program text refers to the binding of that name established by the following name resolution rules."[^1] See name resolution for these rules.

## Examples

```python run id=module-level
x = 1
print(locals() is globals(), "x" in globals())
```

```output for=module-level
True True
```

At the module level, the local and global namespaces are one dictionary (rule r5).

```python run id=free-variable
def outer():
    v = 1
    def inner():
        return v + 1
    return inner

fn = outer()
print(fn(), fn.__code__.co_freevars)
```

```output for=free-variable
2 ('v',)
```

`inner` uses `v` but does not bind it, so `v` is a free variable of `inner` (rule r5).

```python fails id=del-unbinds expect=exception:NameError
x = 1
del x
print(x)
```

## Pitfalls

A name after `except ... as` is unbound at the end of the `except` clause. The reference says that "it is cleared at the end of the except clause", as if the clause ended with `del N`.[^2]

```python wrong id=except-name
err = "before"
try:
    1 / 0
except ZeroDivisionError as err:
    pass
print("err" in globals())
```

```output for=except-name
False
```

The `except` clause binds `err` and then deletes it, so the old value is lost too. "This means the exception must be assigned to a different name to be able to refer to it after the except clause."[^2]

A `for` loop and a `with` statement bind their targets in the current block. They do not make a new block, so the names stay bound after the statement (rule r1).

## Comparison

In Go, "A declaration binds a non-blank identifier to a constant, type, type parameter, variable, function, label, or package. Every identifier in a program must be declared. No identifier may be declared twice in the same block".[^3] The Python reference says: "Python lacks declarations and allows name binding operations to occur anywhere within a code block."[^1] A binding operation anywhere in a block makes the name local to that block (rule r5).
