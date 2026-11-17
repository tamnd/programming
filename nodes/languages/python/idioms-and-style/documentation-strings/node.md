---
tag: NVPCTR
kind: feature
owner: python
slug: documentation-strings
name: Documentation Strings
aliases: [docstring, docstrings, __doc__, PEP 257, help(), inspect.getdoc, Google style docstring, NumPy style docstring]
area: languages/python/idioms-and-style
level: intro
summary: >-
  A docstring is a string literal that is the first statement of a module, class or function. Python stores it in __doc__, and help() shows it. PEP 257 gives the conventions for its form.
grammar: null
spec: {standard: null, url: "https://peps.python.org/pep-0257/", locator: "PEP 257, Docstring Conventions"}
proposals: [pep-257]
see_also: [python/coding-style, python/function-objects, python/introspection-with-inspect, python/quality-control, python/string-literals, python/formatted-string-literals, python/defining-functions]
sources:
  - url: https://peps.python.org/pep-0257/
    title: PEP 257, Docstring Conventions
    locator: What is a Docstring?; One-line Docstrings; Multi-line Docstrings; Handling Docstring Indentation
    accessed: 2026-11-18
  - url: https://docs.python.org/3.15/glossary.html#term-docstring
    title: Glossary, Python 3.15 documentation
    locator: docstring
    accessed: 2026-11-18
  - url: https://docs.python.org/3.15/reference/lexical_analysis.html#formatted-string-literals
    title: The Python Language Reference, Python 3.15 documentation
    locator: 2.6 Formatted string literals
    accessed: 2026-11-18
  - url: https://docs.python.org/3.15/tutorial/controlflow.html#documentation-strings
    title: The Python Tutorial, Python 3.15 documentation
    locator: 4.9.7 Documentation Strings
    accessed: 2026-11-18
  - url: https://peps.python.org/pep-0008/#documentation-strings
    title: PEP 8, Style Guide for Python Code
    locator: Documentation Strings
    accessed: 2026-11-18
  - url: https://docs.python.org/3.15/library/inspect.html#inspect.getdoc
    title: inspect, Inspect live objects, Python 3.15 documentation
    locator: inspect.getdoc(); inspect.cleandoc()
    accessed: 2026-11-18
  - url: https://docs.python.org/3.15/builtins/functions.html#help
    title: Built-in Functions, Python 3.15 documentation
    locator: help()
    accessed: 2026-11-18
  - url: https://google.github.io/styleguide/pyguide.html
    title: Google Python Style Guide
    locator: 3.8.1 Docstrings; 3.8.3 Functions and Methods
    accessed: 2026-11-18
  - url: https://numpydoc.readthedocs.io/en/latest/format.html
    title: Style guide, numpydoc
    locator: Docstring Standard; Sections
    accessed: 2026-11-18
  - url: https://docs.python.org/3.15/whatsnew/3.13.html
    title: What’s New In Python 3.13
    locator: Other Language Changes
    accessed: 2026-11-18
  - url: https://docs.python.org/3.15/using/cmdline.html#cmdoption-OO
    title: Command line and environment, Python 3.15 documentation
    locator: -OO
    accessed: 2026-11-18
  - url: https://docs.astral.sh/ruff/rules/multi-line-summary-first-line/
    title: multi-line-summary-first-line (D212), Ruff documentation
    locator: Why is this bad?
    accessed: 2026-11-18
---

## Synopsis

```python run id=doc-basics
def area(width, height):
    """Return the area of a rectangle."""
    return width * height


print(area.__doc__)
print(area(3, 4))
```

```output for=doc-basics
Return the area of a rectangle.
12
```

## Semantics

**r1.** "A docstring is a string literal that occurs as the first statement in a module, function, class, or method definition. Such a docstring becomes the __doc__ special attribute of that object."[^1] The suite ignores it when it runs, but "it is recognized by the compiler and put into the __doc__ attribute".[^2] Example: [`doc-basics`](#doc-basics).

**r2.** A string in any other place is not a docstring. A string after another statement is only an expression statement, and it does not set `__doc__`. "Formatted string literals cannot be used as docstrings, even if they do not include expressions".[^3] Example: [`not-first-statement`](#not-first-statement).

**r3.** The first line is a summary. "The first line should always be a short, concise summary of the object’s purpose."[^4] It "should begin with a capital letter and end with a period."[^4] PEP 257 asks for the form of a command: it "prescribes the function or method’s effect as a command (“Do this”, “Return that”), not as a description".[^1]

**r4.** A longer docstring has a summary line, a blank line and then the details. "Multi-line docstrings consist of a summary line just like a one-line docstring, followed by a blank line, followed by a more elaborate description."[^1] For a function, the details "should summarize its behavior and document its arguments, return value(s), side effects, exceptions raised, and restrictions on when it can be called (all if applicable)."[^1] Example: [`multi-line`](#multi-line).

**r5.** Quotes: PEP 257 asks for triple double quotes around each docstring, "For consistency".[^1] A one-line docstring keeps its closing quotes on the same line. In a multi-line docstring, the closing quotes "should be on a line by itself".[^5]

**r6.** Indentation: "The Python parser strips indentation from multi-line string literals when they serve as module, class, or function docstrings."[^4] `inspect.getdoc()` returns the docstring "cleaned up with cleandoc()", and `cleandoc` also removes the blank lines at the start and at the end.[^6] Example: [`multi-line`](#multi-line).

**r7.** `help(obj)` builds a help page from the docstrings: "If the argument is any other kind of object, a help page on the object is generated."[^7] The help page shows the signature from introspection, so a docstring should not repeat it: "The one-line docstring should NOT be a “signature” reiterating the function/method parameters".[^1] Example: [`help-page`](#help-page).

**r8.** Which objects need one: "Write docstrings for all public modules, functions, classes, and methods."[^5] PEP 257 adds that "Public methods (including the __init__ constructor) should also have docstrings."[^1] Example: [`class-and-module`](#class-and-module).

**r9.** PEP 257 does not define a markup for the parts of a docstring. Projects use a docstring style for the arguments and the results. The Google style has sections such as `Args:`, `Returns:` and `Raises:`, where "Each section begins with a heading line, which ends with a colon."[^8] The NumPy style has headings such as `Parameters` and `Returns`, and "Each heading should be underlined in hyphens".[^9] Example: [`docstring-styles`](#docstring-styles).

## Examples

```python run id=multi-line
import inspect


def mean(values):
    """Return the arithmetic mean of values.

    The values must be a non-empty sequence of numbers.
    Raise ZeroDivisionError when it is empty.
    """
    return sum(values) / len(values)


print(repr(mean.__doc__.splitlines()[2]))
print(inspect.getdoc(mean).splitlines())
```

```output for=multi-line
'The values must be a non-empty sequence of numbers.'
['Return the arithmetic mean of values.', '', 'The values must be a non-empty sequence of numbers.', 'Raise ZeroDivisionError when it is empty.']
```

The compiler removed the four spaces in front of the body lines (rule r6). `getdoc` also removed the empty last line, which came from the indentation before the closing quotes.

```python run id=help-page
def area(width, height):
    """Return the area of a rectangle."""
    return width * height


help(area)
print("--")
```

```output for=help-page
Help on function area in module __main__:

area(width, height)
    Return the area of a rectangle.

--
```

`help` prints the signature `area(width, height)` from the function object and then the docstring (rule r7).

```python run id=class-and-module
"""Tools for shapes."""


class Square:
    """A square with a side length."""

    def __init__(self, side):
        """Make a square with the given side."""
        self.side = side


print(__doc__)
print(Square.__doc__)
print(Square.__init__.__doc__)
```

```output for=class-and-module
Tools for shapes.
A square with a side length.
Make a square with the given side.
```

The string at the top of the file is the docstring of the module, so the global name `__doc__` holds it (rule r1).

```python run id=docstring-styles
import inspect


def connect(host, port=80):
    """Open a connection to a server.

    Args:
        host: The name of the server.
        port: The port number.

    Returns:
        A connected socket.

    Raises:
        ConnectionError: If no server answers.
    """


def scale(values, factor):
    """Multiply each value by a factor.

    Parameters
    ----------
    values : list of float
        The numbers to scale.
    factor : float
        The multiplier.
    """
    return [v * factor for v in values]


print(inspect.getdoc(connect).splitlines()[2:4])
print(inspect.getdoc(scale).splitlines()[2:5])
```

```output for=docstring-styles
['Args:', '    host: The name of the server.']
['Parameters', '----------', 'values : list of float']
```

`connect` uses the Google style and `scale` uses the NumPy style (rule r9). A function body can be only a docstring, because the docstring is a statement.

## Versions

> [!since v="3.13"]
> "The compiler now strips common leading whitespace from every line in a docstring."[^10] This makes the bytecode cache smaller. "This change affects tools that use docstrings, such as doctest."[^10]

## Pitfalls

```python wrong id=not-first-statement
def greet(name):
    message = f"Hello, {name}"
    """Return a greeting."""
    return message


VERSION = "1.0"


def info():
    f"""Show version {VERSION}."""


print(greet.__doc__, info.__doc__)
```

```output for=not-first-statement
None None
```

In `greet`, the string comes after an assignment, so it is not the first statement. In `info`, the string is an f-string. Neither one is a docstring (rule r2). Put a plain string literal first in the body.

```python wrong id=optimized-away
def usage():
    """Usage: tool [options] FILE"""


print(usage.__doc__.split()[0])
```

```output for=optimized-away
Usage:
```

With the `-OO` option, Python will "Do -O and also discard docstrings."[^11] Then `usage.__doc__` is `None`, and the call `split()` raises `AttributeError`. This block runs without `-OO`, so the docstring is there. Do not keep data that the program needs only in a docstring.

## Comparison

The Google style guide asks for "a summary line (one physical line not exceeding 80 characters) terminated by a period, question mark, or exclamation point."[^8] NumPy asks that "The length of docstring lines should be kept to 75 characters".[^9] PEP 257 lets the summary start on the line of the opening quotes or on the next line. Linters pick a convention: the Ruff rule D212 "is enabled when using the google convention, and disabled when using the numpy and pep257 conventions."[^12] For the strings themselves, see [[python/string-literals]]. For `inspect`, see [[python/introspection-with-inspect]]. For docstrings that hold tests, see [[python/quality-control]].
