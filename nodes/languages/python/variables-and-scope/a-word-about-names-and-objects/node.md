---
tag: 199SQK
kind: feature
owner: python
slug: a-word-about-names-and-objects
name: A Word About Names and Objects
aliases: [Python aliasing, Python names are references, why did changing y change x, Python variables are names, is versus ==, id() in Python]
area: languages/python/variables-and-scope
level: intro
summary: >-
  A Python name refers to an object. Assignment binds a name and does not copy the object, so two names can refer to the same object. A change to a mutable object is visible through each of its names.
grammar: null
spec: {standard: null, url: "https://docs.python.org/3.15/tutorial/classes.html#a-word-about-names-and-objects", locator: "9.1. A Word About Names and Objects"}
see_also: [python/assignment-statements, python/python-scopes-and-namespaces, python/naming-and-binding, python/dynamic-and-strong-typing]
sources:
  - url: https://docs.python.org/3.15/tutorial/classes.html#a-word-about-names-and-objects
    title: The Python Tutorial, 9. Classes
    locator: 9.1. A Word About Names and Objects; 9.2. Python Scopes and Namespaces
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/faq/programming.html#why-did-changing-list-y-also-change-list-x
    title: Programming FAQ, Python 3.15 documentation
    locator: Why did changing list 'y' also change list 'x'?
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/reference/datamodel.html#objects-values-and-types
    title: The Python Language Reference, Python 3.15
    locator: 3.1. Objects, values and types
    accessed: 2026-11-16
  - url: https://go.dev/ref/spec#Variables
    title: The Go Programming Language Specification
    locator: Variables; Assignment statements
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/faq/programming.html#how-do-i-create-a-multidimensional-list
    title: Programming FAQ, Python 3.15 documentation
    locator: How do I create a multidimensional list?
    accessed: 2026-11-16
---

## Synopsis

```python run id=alias
x = []
y = x
y.append(10)
print(x, y, x is y)
```

```output for=alias
[10] [10] True
```

`y = x` does not copy the list. The names `x` and `y` refer to one list, so the change through `y` is visible through `x`.

## Semantics

**r1.** "Objects have individuality, and multiple names (in multiple scopes) can be bound to the same object. This is known as aliasing in other languages."[^1] Example: [`alias`](#alias).

**r2.** "Assignments do not copy data — they just bind names to objects."[^1] The FAQ says: "Variables are simply names that refer to objects. Doing y = x doesn't create a copy of the list – it creates a new variable y that refers to the same object x refers to."[^2] Example: [`alias`](#alias).

**r3.** "Every object has an identity, a type and a value. An object's identity never changes once it has been created". "The is operator compares the identity of two objects; the id() function returns an integer representing its identity."[^3] Example: [`identity`](#identity).

**r4.** Aliasing "can be safely ignored when dealing with immutable basic types (numbers, strings, tuples)". It "has a possibly surprising effect on the semantics of Python code involving mutable objects such as lists, dictionaries, and most other types."[^1] For an int, `x = x + 1` does not change the int. It creates a new object and binds `x` to it.[^2] Example: [`rebind-int`](#rebind-int).

**r5.** "Some operations (for example y.append(10) and y.sort()) mutate the object, whereas superficially similar operations (for example y = y + [10] and sorted(y)) create a new object."[^2] Example: [`mutate-or-rebind`](#mutate-or-rebind).

**r6.** "if a function modifies an object passed as an argument, the caller will see the change". Only a reference is passed, so "passing an object is cheap since only a pointer is passed by the implementation".[^1] Example: [`argument`](#argument).

## Examples

```python run id=identity
a = [1, 2]
b = a
c = [1, 2]
print(a == c, a is c, a is b, id(a) == id(b))
```

```output for=identity
True False True True
```

`a` and `c` have equal values but are two objects. `a` and `b` are one object (rule r3).

```python run id=rebind-int
x = 5
y = x
x = x + 1
print(x, y)
```

```output for=rebind-int
6 5
```

```python run id=mutate-or-rebind
x = [3, 1]
y = x
y = y + [2]
print(x, y)
y = x
y.sort()
print(x, y)
```

```output for=mutate-or-rebind
[3, 1] [3, 1, 2]
[1, 3] [1, 3]
```

`y = y + [2]` makes a new list and binds `y` to it, so `x` does not change. `y.sort()` changes the list that `x` and `y` share (rule r5).

```python run id=argument
def add_item(items):
    items.append("new")

names = ["old"]
add_item(names)
print(names)
```

```output for=argument
['old', 'new']
```

## Pitfalls

A list of lists made with `*` shares one inner list. The FAQ says that "replicating a list with * doesn't create copies, it only creates references to the existing objects", and "Changes to one row will show in all rows".[^5]

```python wrong id=shared-rows
grid = [[0] * 2] * 2
grid[0][0] = 1
print(grid)
```

```output for=shared-rows
[[1, 0], [1, 0]]
```

To get separate rows, make a new list for each row, for example with `[[0] * 2 for _ in range(2)]`.

A method that changes an object usually returns `None`. The FAQ says: "In general in Python (and in all cases in the standard library) a method that mutates an object will return None".[^2] So `y = y.sort()` binds `y` to `None`.

## Comparison

In Go, "A variable is a storage location for holding a value." And "An assignment replaces the current value stored in a variable with a new value specified by an expression."[^4] So a Go assignment such as `y = x` stores a copy of the value of `x` in `y`. To share data, Go uses pointers, slices or maps. In Python, each name refers to an object, and assignment never copies the object (rule r2).
