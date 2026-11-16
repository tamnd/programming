---
tag: Q51V7H
kind: feature
owner: python
slug: class-definition-syntax
name: Class Definition Syntax
aliases: [class statement, class keyword, class body, class namespace, class decorator, define a class]
area: languages/python/user-types
level: intro
summary: >-
  The class statement runs its body as code in a new namespace. When the body ends, Python makes a class object from that namespace and binds it to the class name. A decorator can change the class.
grammar: null
spec: {standard: null, url: "https://docs.python.org/3.15/reference/compound_stmts.html#class-definitions", locator: "8.8. Class definitions"}
see_also: [python/classes, python/class-objects, python/python-scopes-and-namespaces, python/inheritance]
sources:
  - url: https://docs.python.org/3.15/tutorial/classes.html#class-definition-syntax
    title: The Python Tutorial, 9. Classes
    locator: 9.3.1. Class Definition Syntax
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/reference/compound_stmts.html#class-definitions
    title: The Python Language Reference, 8. Compound statements
    locator: 8.8. Class definitions
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/reference/executionmodel.html#resolution-of-names
    title: The Python Language Reference, 4. Execution model
    locator: 4.2.2. Resolution of names
    accessed: 2026-11-16
  - url: https://go.dev/ref/spec#Method_declarations
    title: The Go Programming Language Specification
    locator: Type declarations; Method declarations
    accessed: 2026-11-16
---

## Synopsis

```python run id=basics
print("before")

class Config:
    print("running the class body")
    debug = False
    level = 1 + 2

    def show(self):
        return f"debug={self.debug} level={self.level}"

print("after")
print(Config.level, Config().show())
print([k for k in Config.__dict__ if not k.startswith("__")])
```

```output for=basics
before
running the class body
after
3 debug=False level=3
['debug', 'level', 'show']
```

The class body runs once, when Python runs the class statement. Its names become the attributes of the class, in the order of the body.

## Semantics

**r1.** "Class definitions, like function definitions (def statements) must be executed before they have any effect. (You could conceivably place a class definition in a branch of an if statement, or inside a function.)"[^1] Example: [`factory`](#factory).

**r2.** "When a class definition is entered, a new namespace is created, and used as the local scope".[^1] "In particular, function definitions bind the name of the new function here."[^1]

**r3.** "The class’s suite is then executed in a new execution frame (see Naming and binding), using a newly created local namespace and the original global namespace."[^2] "A class object is then created using the inheritance list for the base classes and the saved local namespace for the attribute dictionary. The class name is bound to this class object in the original local namespace."[^2]

**r4.** "The order in which attributes are defined in the class body is preserved in the new class’s __dict__."[^2]

**r5.** "Classes can also be decorated: just like when decorating functions".[^2] A class `Foo` with the decorators `@f1(arg)` and then `@f2` is roughly the same as `Foo = f1(arg)(f2(Foo))`.[^2] "The result is then bound to the class name."[^2] Example: [`decorator`](#decorator).

**r6.** "The scope of names defined in a class block is limited to the class block; it does not extend to the code blocks of methods. This includes comprehensions and generator expressions".[^3] Examples: [`class-scope`](#class-scope), [`genexp-scope`](#genexp-scope).

## Examples

```python run id=factory
def make_point_class(dims):
    if dims == 2:
        class Point:
            fields = ("x", "y")
    else:
        class Point:
            fields = ("x", "y", "z")
    return Point

P2 = make_point_class(2)
P3 = make_point_class(3)
print(P2.fields, P3.fields, P2 is P3)
```

```output for=factory
('x', 'y') ('x', 'y', 'z') False
```

Each run of a class statement makes a new class object (rule r1).

```python run id=decorator
registry = {}

def register(cls):
    registry[cls.__name__] = cls
    return cls

@register
class Circle:
    pass

@register
class Square:
    pass

print(sorted(registry), registry["Circle"] is Circle)
```

```output for=decorator
['Circle', 'Square'] True
```

The decorator gets the new class and returns it, and Python binds the result to the class name (rule r5).

## Pitfalls

A method does not see the names of the class body as plain names. Use `self.step` or `Counter.step`:

```python fails id=class-scope expect=exception:NameError
class Counter:
    step = 2

    def next(self, n):
        return n + step

Counter().next(1)
```

The same rule applies to a generator expression in the class body (rule r6):

```python fails id=genexp-scope expect=exception:NameError
class A:
    a = 42
    b = list(a + i for i in range(10))
```

## Comparison

Go has no class statement. "A type declaration binds an identifier, the type name, to a type."[^4] Methods are separate declarations: "A method declaration binds an identifier, the method name, to a method, and associates the method with the receiver's base type."[^4] In Python the methods are function definitions in the class body, and the class body is code that runs (rules r1 and r2).
