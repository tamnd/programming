---
tag: YY92BY
kind: feature
owner: python
slug: duck-typing
name: Duck typing
aliases: [duck-typing, EAFP, LBYL, structural typing at run time, file-like object, protocol by convention]
area: languages/python/user-types
level: core
summary: >-
  Duck typing means code uses the methods it needs and does not check the type of the object. Any object with those methods works, and a missing method shows up as an exception when the code runs.
grammar: null
spec: {standard: null, url: "https://docs.python.org/3.15/glossary.html#term-duck-typing", locator: "Glossary, duck-typing"}
see_also: [python/abstract-base-classes, python/instance-and-subclass-checks, python/special-methods, python/classes]
sources:
  - url: https://docs.python.org/3.15/glossary.html#term-duck-typing
    title: Glossary
    locator: duck-typing; EAFP; LBYL
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/tutorial/classes.html#odds-and-ends
    title: The Python Tutorial, 9. Classes
    locator: 9.7. Odds and Ends
    accessed: 2026-11-16
  - url: https://go.dev/doc/effective_go#interfaces
    title: Effective Go
    locator: Interfaces
    accessed: 2026-11-16
  - url: https://go.dev/doc/faq#implements_interface
    title: Frequently Asked Questions (FAQ), The Go Programming Language
    locator: Why doesn’t Go have “implements” declarations?; How can I guarantee my type satisfies an interface?
    accessed: 2026-11-16
---

## Synopsis

```python run id=basics
class Duck:
    def speak(self):
        return "Quack"

class Robot:
    def speak(self):
        return "Beep"

def chorus(things):
    return " ".join(t.speak() for t in things)

print(chorus([Duck(), Robot(), Duck()]))
```

```output for=basics
Quack Beep Quack
```

Duck and Robot have no shared base class. `chorus` only calls `speak`, so it accepts both.

## Semantics

**r1.** Duck typing is "A programming style which does not look at an object’s type to determine if it has the right interface; instead, the method or attribute is simply called or used".[^1] "Duck-typing avoids tests using type() or isinstance()."[^1]

**r2.** "A piece of Python code that expects a particular abstract data type can often be passed a class that emulates the methods of that data type instead."[^2] Example: [`file-like`](#file-like).

**r3.** Duck typing "typically employs hasattr() tests or EAFP programming."[^1] EAFP "assumes the existence of valid keys or attributes and catches exceptions if the assumption proves false."[^1] Example: [`eafp`](#eafp).

**r4.** LBYL "explicitly tests for pre-conditions before making calls or lookups."[^1] "In a multi-threaded environment, the LBYL approach can risk introducing a race condition between “the looking” and “the leaping”."[^1]

**r5.** "duck-typing can be complemented with abstract base classes."[^1]

## Examples

```python run id=file-like
import io

def count_lines(f):
    return sum(1 for _ in f)

class FakeFile:
    def __init__(self, text):
        self.lines = text.splitlines(keepends=True)

    def __iter__(self):
        return iter(self.lines)

print(count_lines(io.StringIO("a\nb\nc\n")), count_lines(FakeFile("x\ny\n")))
```

```output for=file-like
3 2
```

`count_lines` only iterates, so a StringIO or a small class with `__iter__` takes the place of a file (rule r2).

```python run id=eafp
def area(shape):
    try:
        return shape.area()
    except AttributeError:
        return None

class Sq:
    def area(self):
        return 4

print(area(Sq()), area("not a shape"))
```

```output for=eafp
4 None
```

The function tries the call and handles the failure (rule r3).

```python run id=lbyl
def area(shape):
    if hasattr(shape, "area"):
        return shape.area()
    return None

class Sq:
    def area(self):
        return 4

print(area(Sq()), area(42))
```

```output for=lbyl
4 None
```

The same function in the LBYL style tests first with `hasattr` (rule r4).

## Pitfalls

A type check rejects objects that would work:

```python wrong id=type-check
import io

def first_line(f):
    if type(f) is not io.TextIOWrapper:
        return "not a file"
    return f.readline()

print(first_line(io.StringIO("hello\nworld\n")))
```

```output for=type-check
not a file
```

StringIO has `readline`, but the type test refuses it. Call `readline` and let a missing method raise (rule r1).

An `except AttributeError` around the whole call also hides bugs inside the method:

```python wrong id=hidden
class Broken:
    def area(self):
        return self.sise * 2

def area(shape):
    try:
        return shape.area()
    except AttributeError:
        return None

print(area(Broken()))
```

```output for=hidden
None
```

The typo `sise` raises AttributeError inside `area`, and the handler treats it as a missing method. Get the method first with `getattr(shape, 'area', None)`, and call it outside the try statement.

## Comparison

"Interfaces in Go provide a way to specify the behavior of an object: if something can do this, then it can be used here."[^3] "A Go type implements an interface by implementing the methods of that interface, nothing more."[^4] So a Go type, like a Python class, does not name the interface. But a Go function parameter has an interface type, and the FAQ says: "You can ask the compiler to check that the type T implements the interface I by attempting an assignment".[^4] In Python nothing lists the methods, and a missing one raises an exception when the code runs (rule r1).
