---
tag: NEW-python-annotated-assignment
kind: feature
owner: python
slug: annotated-assignment
name: Annotated assignment
aliases: [variable annotation, PEP 526, Python variable type hints, __annotations__, declare a variable type in Python]
area: languages/python/variables-and-scope
level: core
summary: >-
  x: int = 0 adds an annotation to an assignment. The value part is optional, and Python does not check the annotation. A class or module keeps the annotations of its simple names in __annotations__.
grammar: "annotated_assignment_stmt: augtarget ':' expression ['=' (starred_expression | yield_expression)] (Language Reference, 7.2.2)"
spec: {standard: null, url: "https://docs.python.org/3.15/reference/simple_stmts.html#annotated-assignment-statements", locator: "7.2.2. Annotated assignment statements"}
see_also: [python/assignment-statements, python/dynamic-and-strong-typing, python/annotation-scopes, python/naming-and-binding]
sources:
  - url: https://docs.python.org/3.15/reference/simple_stmts.html#annotated-assignment-statements
    title: The Python Language Reference, Python 3.15
    locator: 7.2.2. Annotated assignment statements
    accessed: 2026-11-16
  - url: https://peps.python.org/pep-0526/#global-and-local-variable-annotations
    title: PEP 526 - Syntax for Variable Annotations
    locator: Abstract; Global and local variable annotations
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/glossary.html#term-variable-annotation
    title: Glossary, Python 3.15 documentation
    locator: variable annotation; type hint
    accessed: 2026-11-16
  - url: https://go.dev/ref/spec#Variable_declarations
    title: The Go Programming Language Specification
    locator: Variable declarations
    accessed: 2026-11-16
---

## Synopsis

```python run id=class-fields
class Point:
    x: int = 0
    y: int
    label: str = "origin"

print(Point.__annotations__)
print(Point.x, hasattr(Point, "y"))
```

```output for=class-fields
{'x': <class 'int'>, 'y': <class 'int'>, 'label': <class 'str'>}
0 False
```

`y: int` has no value. It adds an annotation, but it does not create the attribute `y`.

## Semantics

**r1.** "Annotation assignment is the combination, in a single statement, of a variable or attribute annotation and an optional assignment statement". "The difference from normal Assignment statements is that only a single target is allowed."[^1] Example: [`class-fields`](#class-fields).

**r2.** "The assignment target is considered “simple” if it consists of a single name that is not enclosed in parentheses." "For simple assignment targets, if in class or module scope, the annotations are gathered in a lazily evaluated annotation scope. The annotations can be evaluated using the __annotations__ attribute of a class or module".[^1] Example: [`class-fields`](#class-fields).

**r3.** "If a name is annotated in a function scope, then this name is local for that scope. Annotations are never evaluated and stored in function scopes."[^1] PEP 526 shows that an annotation with no value makes the name local, so a use before an assignment "raises UnboundLocalError".[^2] Examples: [`local-annotation`](#local-annotation), [`annotation-makes-local`](#annotation-makes-local).

**r4.** "If the right hand side is present, an annotated assignment performs the actual assignment as if there was no annotation present."[^1] Python does not check the value against the annotation. "Variable annotations are usually used for type hints", and type hints "are not enforced by Python".[^3] Example: [`not-checked`](#not-checked).

**r5.** "If the assignment target is not simple (an attribute, subscript node, or parenthesized name), the annotation is never evaluated."[^1] Example: [`attribute-target`](#attribute-target).

**r6.** PEP 526 says that the syntax "does not allow one to annotate the types of variables when tuple unpacking is used". Annotate each name first, and then unpack.[^2] Example: [`no-unpacking`](#no-unpacking).

## Examples

```python run id=local-annotation
def f():
    x: NonexistentName = 1
    count: int
    return x

print(f())
```

```output for=local-annotation
1
```

The annotation `NonexistentName` is not defined, but Python does not evaluate annotations in a function, so the call works (rule r3).

```python run id=not-checked
total: int = "not a number"
print(total, type(total).__name__)
```

```output for=not-checked
not a number str
```

```python run id=attribute-target
class Box:
    pass

box = Box()
box.size: int = 3
box.color: str
print(box.size, hasattr(box, "color"))
```

```output for=attribute-target
3 False
```

`box.color: str` has no value, so it assigns nothing (rule r5).

```python fails id=no-unpacking expect=exception:SyntaxError
a, b: int = 1, 2
```

## Pitfalls

An annotation with no value makes the name local in a function, even when the function only reads the global name:

```python fails id=annotation-makes-local expect=exception:UnboundLocalError
a = 1

def f():
    a: int
    print(a)

f()
```

PEP 526 explains this case: the code acts "as if the code were" a function with an `if False: a = 0` before the use.[^2] Remove the annotation, or annotate a different name.

An annotation is not a check. A wrong value runs with no error (example [`not-checked`](#not-checked)). A static type checker can find the mistake before the program runs.[^3]

## Comparison

In Go, "A variable declaration creates one or more variables, binds corresponding identifiers to them, and gives each a type and an initial value". With no value, "each variable is initialized to its zero value".[^4] And "If a type is present, each variable is given that type."[^4] A Python annotation is only information for tools. With no value, the Python name stays unbound (rules r3 and r4).
