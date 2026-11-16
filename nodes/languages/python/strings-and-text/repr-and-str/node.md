---
tag: ZREZ0N
kind: feature
owner: python
slug: repr-and-str
name: repr and str
aliases: [repr, __repr__, __str__, str(), repr(), ascii(), official representation, informal representation, !r conversion]
area: languages/python/strings-and-text
level: core
summary: >-
  repr() gives the official representation of an object, often valid Python code. str() gives the readable form. A class sets them with __repr__ and __str__, and __str__ falls back to __repr__.
grammar: null
spec: {standard: null, url: "https://docs.python.org/3.15/reference/datamodel.html#object.__repr__", locator: "3.3.1. Basic customization"}
see_also: [python/str-type, python/formatted-string-literals, python/the-string-format-method, python/interactive-mode]
sources:
  - url: https://docs.python.org/3.15/reference/datamodel.html#object.__repr__
    title: The Python Language Reference, Python 3.15
    locator: 3.3.1. Basic customization, object.__repr__ and object.__str__
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/builtins/functions.html#repr
    title: Built-in Functions, Python 3.15 documentation
    locator: repr(); ascii()
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/tutorial/inputoutput.html#fancier-output-formatting
    title: The Python Tutorial, 7. Input and Output
    locator: 7.1. Fancier Output Formatting
    accessed: 2026-11-16
  - url: https://pkg.go.dev/fmt
    title: Package fmt, Go standard library
    locator: type Stringer; type GoStringer
    accessed: 2026-11-16
---

## Synopsis

```python run id=two-forms
s = "hello, world\n"
print(str(s), end="")
print(repr(s))
print(str(1/7), repr(1/7))
print(repr((32.5, ("spam", "eggs"))))
```

```output for=two-forms
hello, world
'hello, world\n'
0.14285714285714285 0.14285714285714285
(32.5, ('spam', 'eggs'))
```

For a string, str() gives the text and repr() gives a quoted literal. For many other values, the two are the same.

## Semantics

**r1.** "The str() function is meant to return representations of values which are fairly human-readable, while repr() is meant to generate representations which can be read by the interpreter (or will force a SyntaxError if there is no equivalent syntax)."[^3] "Strings, in particular, have two distinct representations."[^3]

**r2.** `__repr__` is "Called by the repr() built-in function".[^1] "If at all possible, this should look like a valid Python expression that could be used to recreate an object with the same value (given an appropriate environment). If this is not possible, a string of the form <...some useful description...> should be returned."[^1]

**r3.** `__str__` is "Called by str(object), the default __format__() implementation, and the built-in function print()".[^1] "The default implementation defined by the built-in type object calls object.__repr__()."[^1] So a class that defines only `__repr__` gets it for both. Example: [`only-repr`](#only-repr).

**r4.** Without a custom `__repr__`, "the representation is a string enclosed in angle brackets that contains the name of the type of the object together with additional information often including the name and address of the object."[^2] Example: [`default`](#default).

**r5.** `ascii()` works "As repr(), return a string containing a printable representation of an object, but escape the non-ASCII characters in the string returned by repr() using \x, \u, or \U escapes."[^2]

**r6.** In an f-string or a format string, `!r` applies repr(), `!s` applies str() and `!a` applies ascii().[^3] Example: [`containers`](#containers).

## Examples

```python run id=class
class Point:
    def __init__(self, x, y):
        self.x, self.y = x, y

    def __repr__(self):
        return f"Point({self.x!r}, {self.y!r})"

    def __str__(self):
        return f"({self.x}, {self.y})"

p = Point(1, 2)
print(p)
print(repr(p))
print(f"{p} {p!r}")
print(eval(repr(p)).y)
```

```output for=class
(1, 2)
Point(1, 2)
(1, 2) Point(1, 2)
2
```

The repr() text is valid Python that makes an equal object (rule r2). print() and f-strings use str() (rule r3).

```python run id=only-repr
class Color:
    def __init__(self, name):
        self.name = name

    def __repr__(self):
        return f"Color({self.name!r})"

print(Color("red"), str(Color("red")))
```

```output for=only-repr
Color('red') Color('red')
```

The class has no `__str__`, so str() uses `__repr__` (rule r3).

```python run id=containers
items = ["a", "b c", 1]
print(items)
print(str(items) == repr(items))
print(ascii("café"), f"{'café'!a}")
```

```output for=containers
['a', 'b c', 1]
True
'caf\xe9' 'caf\xe9'
```

The list shows its string items in quotes, so `str(items)` and `repr(items)` are the same. `!a` gives the same text as ascii() (rules r5 and r6).

## Pitfalls

The default repr() has no data in it:

```python wrong id=default
class User:
    def __init__(self, name):
        self.name = name

text = repr(User("ada"))
print(text.startswith("<__main__.User object at 0x"))
```

```output for=default
True
```

The text shows only the type and an address (rule r4). Define `__repr__` to show the data.

A `__repr__` must return a string:

```python fails id=not-str expect=exception:TypeError
class Bad:
    def __repr__(self):
        return 42

repr(Bad())
```

## Comparison

Go has two interfaces for this. "Stringer is implemented by any value that has a String method".[^4] The String method gives the native format. "GoStringer is implemented by any value that has a GoString method, which defines the Go syntax for that value."[^4] The verb `%v` uses String, and `%#v` uses GoString. They match `__str__` and `__repr__` (rules r2 and r3).
