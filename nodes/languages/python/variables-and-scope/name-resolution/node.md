---
tag: 4PKDKS
kind: feature
owner: python
slug: name-resolution
name: Name resolution
aliases: [how Python looks up a name, UnboundLocalError, NameError, class scope and comprehensions, Python name lookup rules, nearest enclosing scope]
area: languages/python/variables-and-scope
level: advanced
summary: >-
  Python resolves a name in the nearest enclosing scope that binds it. An unknown name raises NameError, and a local name used before its binding raises UnboundLocalError. Class bodies are special.
grammar: null
spec: {standard: null, url: "https://docs.python.org/3.15/reference/executionmodel.html#resolution-of-names", locator: "4.2.2. Resolution of names"}
see_also: [python/naming-and-binding, python/python-scopes-and-namespaces, python/global-statement, python/nonlocal-statement, python/scope-and-dynamic-features]
sources:
  - url: https://docs.python.org/3.15/reference/executionmodel.html#resolution-of-names
    title: The Python Language Reference, Python 3.15
    locator: 4.2.2. Resolution of names
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/faq/programming.html#why-am-i-getting-an-unboundlocalerror-when-the-variable-has-a-value
    title: Programming FAQ, Python 3.15 documentation
    locator: Why am I getting an UnboundLocalError when the variable has a value?
    accessed: 2026-11-16
  - url: https://go.dev/ref/spec#Declarations_and_scope
    title: The Go Programming Language Specification
    locator: Declarations and scope
    accessed: 2026-11-16
---

## Synopsis

```python run id=nearest
x = "global"

def outer():
    x = "outer"
    def middle():
        def inner():
            return x
        return inner()
    return middle()

print(outer())
```

```output for=nearest
outer
```

`inner` and `middle` do not bind `x`, so Python uses the binding of the nearest enclosing function that binds it.

## Semantics

**r1.** "A scope defines the visibility of a name within a block. If a local variable is defined in a block, its scope includes that block. If the definition occurs in a function block, the scope extends to any blocks contained within the defining one, unless a contained block introduces a different binding for the name."[^1] Example: [`nearest`](#nearest).

**r2.** "When a name is used in a code block, it is resolved using the nearest enclosing scope. The set of all such scopes visible to a code block is called the block's environment."[^1] Example: [`nearest`](#nearest).

**r3.** "When a name is not found at all, a NameError exception is raised." If a local variable "has not yet been bound to a value at the point where the name is used, an UnboundLocalError exception is raised. UnboundLocalError is a subclass of NameError."[^1] Examples: [`unknown-name`](#unknown-name), [`used-before-binding`](#used-before-binding).

**r4.** "If a name binding operation occurs anywhere within a code block, all uses of the name within the block are treated as references to the current block." "Python lacks declarations and allows name binding operations to occur anywhere within a code block. The local variables of a code block can be determined by scanning the entire text of the block for name binding operations."[^1] Example: [`used-before-binding`](#used-before-binding).

**r5.** With a `global` statement, names "are resolved in the top-level namespace by searching the global namespace, i.e. the namespace of the module containing the code block, and the builtins namespace". "The global namespace is searched first."[^1] "If the nearest enclosing scope for a free variable contains a global statement, the free variable is treated as a global."[^1] Example: [`global-in-enclosing`](#global-in-enclosing).

**r6.** In a class body, name resolution has "an exception that unbound local variables are looked up in the global namespace."[^1] Example: [`class-lookup`](#class-lookup).

**r7.** "The scope of names defined in a class block is limited to the class block; it does not extend to the code blocks of methods. This includes comprehensions and generator expressions".[^1] Example: [`class-genexp`](#class-genexp).

## Examples

```python fails id=unknown-name expect=exception:NameError
print(undefined_name)
```

```python run id=subclass
print(issubclass(UnboundLocalError, NameError))
```

```output for=subclass
True
```

```python run id=global-in-enclosing
x = "global"

def outer():
    global x
    x = "set by outer"
    def inner():
        return x
    return inner()

print(outer(), x)
```

```output for=global-in-enclosing
set by outer set by outer
```

`outer` declares `x` global, so the free variable `x` of `inner` is the global `x` (rule r5).

```python run id=class-lookup
x = "global"

def f():
    x = "enclosing"
    class C:
        print(x)
        x = "class"
    return C.x

print(f())
```

```output for=class-lookup
global
class
```

The class body binds `x`, so `x` is local to the class block. At the first `print(x)`, the local `x` is not bound yet, and Python looks in the global namespace. It skips the `x` of `f` (rule r6).

## Pitfalls

An assignment later in a function makes a name local in the whole function:

```python fails id=used-before-binding expect=exception:UnboundLocalError
x = 10

def foo():
    print(x)
    x += 1

foo()
```

The FAQ explains: "when you make an assignment to a variable in a scope, that variable becomes local to that scope and shadows any similarly named variable in the outer scope."[^2]

A generator expression in a class body cannot use the names of the class:

```python fails id=class-genexp expect=exception:NameError
class A:
    a = 42
    b = list(a + i for i in range(10))
```

This is the example of the reference (rule r7). Bind the value in a function parameter, or compute it outside the class.

## Comparison

In Go, "An identifier declared in a block may be redeclared in an inner block. While the identifier of the inner declaration is in scope, it denotes the entity declared by the inner declaration."[^3] So Go also uses the nearest declaration. But in Go, "Every identifier in a program must be declared", and the scope of a local variable begins after its declaration.[^3] In Python, a binding anywhere in a function makes the name local in the whole function, also before the binding (rule r4).
