---
tag: JK65VW
kind: feature
owner: python
slug: annotation-scopes
name: Annotation scopes
aliases: [type parameter scope, PEP 695 scopes, def695, type statement scope, lazy evaluation of type aliases, __type_params__]
area: languages/python/variables-and-scope
level: research
summary: >-
  Type parameter lists, type statements and annotations get an annotation scope. It acts like a function scope, but it can see the names of an enclosing class. Most are evaluated lazily.
grammar: null
spec: {standard: null, url: "https://docs.python.org/3.15/reference/executionmodel.html#annotation-scopes", locator: "4.2.3. Annotation scopes; 4.2.4. Lazy evaluation"}
see_also: [python/name-resolution, python/naming-and-binding, python/annotated-assignment, python/python-scopes-and-namespaces]
sources:
  - url: https://docs.python.org/3.15/reference/executionmodel.html#annotation-scopes
    title: The Python Language Reference, Python 3.15
    locator: 4.2.3. Annotation scopes; 4.2.4. Lazy evaluation
    accessed: 2026-11-16
  - url: https://peps.python.org/pep-0695/#scoping-behavior
    title: PEP 695 - Type Parameter Syntax
    locator: Abstract; Scoping Behavior
    accessed: 2026-11-16
  - url: https://go.dev/ref/spec#Declarations_and_scope
    title: The Go Programming Language Specification
    locator: Declarations and scope
    accessed: 2026-11-16
---

## Synopsis

```python run id=class-names
class A:
    type Alias = Nested

    class Nested:
        pass

print(A.Alias.__value__.__qualname__)
```

```output for=class-names
A.Nested
```

The value of `Alias` uses `Nested`, a name of the class body. The annotation scope of the `type` statement can see it, and the value is evaluated only when `__value__` is read.

## Semantics

**r1.** "Annotations, type parameter lists and type statements introduce annotation scopes, which behave mostly like function scopes, but with some exceptions".[^1] The contexts include function and variable annotations, "Type parameter lists for generic type aliases", "Type parameter lists for generic functions", "Type parameter lists for generic classes", the bounds, constraints and defaults of type parameters, and "The value of type aliases".[^1]

**r2.** "Annotation scopes have access to their enclosing class namespace." Code in such a scope "can use names defined in the class scope as if it were executed directly within the class body. This contrasts with regular functions defined within classes, which cannot access names defined in the class scope."[^1] Example: [`class-names`](#class-names).

**r3.** "Expressions in annotation scopes cannot contain yield, yield from, await, or := expressions."[^1] Example: [`no-walrus`](#no-walrus).

**r4.** "Names defined in annotation scopes cannot be rebound with nonlocal statements in inner scopes. This includes only type parameters".[^1] Example: [`no-nonlocal`](#no-nonlocal).

**r5.** "the __qualname__ of such objects is as if the object were defined in the enclosing scope."[^1] Example: [`qualname`](#qualname).

**r6.** "Most annotation scopes are lazily evaluated." They "are only evaluated when necessary, for example when the __value__ attribute on a type alias is accessed."[^1] Examples: [`lazy-alias`](#lazy-alias), [`lazy-bound`](#lazy-bound).

**r7.** PEP 695 added annotation scopes. It describes them as "a new kind of scope that behaves differently from existing scopes in Python".[^2] Later versions use annotation scopes for more contexts, such as type parameter defaults and annotations.[^1]

## Examples

```python run id=qualname
def make():
    def f[T](x: T) -> T:
        return x
    return f

f = make()
print(f.__qualname__, f.__type_params__)
```

```output for=qualname
make.<locals>.f (T,)
```

The type parameter list of `f` has its own annotation scope, but the qualified name of `f` does not show that scope (rule r5).

```python fails id=lazy-alias expect=exception:ZeroDivisionError
type Alias = 1 / 0
print("alias created")
Alias.__value__
```

The `type` statement runs with no error. The error comes only when `__value__` evaluates `1 / 0` (rule r6).

```python fails id=lazy-bound expect=exception:ZeroDivisionError
def func[T: 1 / 0]():
    pass

T = func.__type_params__[0]
T.__bound__
```

```python run id=recursive-alias
type Tree = int | list[Tree]
print(Tree.__value__)
```

```output for=recursive-alias
int | list[Tree]
```

Lazy evaluation lets the alias refer to itself.

## Pitfalls

A type alias cannot contain `:=`:

```python fails id=no-walrus expect=exception:SyntaxError
type X = (y := int)
```

A nested function cannot rebind a type parameter:

```python fails id=no-nonlocal expect=exception:SyntaxError
def f[T]():
    def g():
        nonlocal T
        T = int
    return g
```

The rules for annotations changed when annotations started to use annotation scopes (rule r7). Test code that reads annotations on each Python version that you support.

## Comparison

Go also has type parameters with their own scope: "The scope of an identifier denoting a type parameter of a function or declared by a method receiver begins after the name of the function and ends at the end of the function body. The scope of an identifier denoting a type parameter of a type begins after the name of the type and ends at the end of the TypeSpec."[^3] A Python type parameter has a similar scope, but its bound and the value of a type alias are evaluated only when a program reads them (rule r6).
