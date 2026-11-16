---
tag: EB0RYK
kind: feature
owner: python
slug: pass-statements
name: Pass Statements
aliases: [pass statement, pass keyword, empty block in Python, do nothing statement, placeholder body, Ellipsis as a body]
area: languages/python/control-flow
level: intro
summary: >-
  pass does nothing. Use it where the syntax needs a statement but the program needs no action, such as an empty class, a function stub or an except clause that ignores an error.
grammar: "pass_stmt: 'pass' (Language Reference, 7.4)"
spec: {standard: null, url: "https://docs.python.org/3.15/reference/simple_stmts.html#the-pass-statement", locator: "7.4. The pass statement"}
see_also: [python/if-statements, python/more-control-flow-tools, python/while-statement]
sources:
  - url: https://docs.python.org/3.15/tutorial/controlflow.html#pass-statements
    title: The Python Tutorial, 4. More Control Flow Tools
    locator: 4.6. pass Statements
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/reference/simple_stmts.html#the-pass-statement
    title: The Python Language Reference, Python 3.15
    locator: 7.4. The pass statement
    accessed: 2026-11-16
  - url: https://go.dev/ref/spec#Empty_statements
    title: The Go Programming Language Specification
    locator: Empty statements; Blocks
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/reference/lexical_analysis.html#indentation
    title: The Python Language Reference, Python 3.15
    locator: 2.1.8. Indentation
    accessed: 2026-11-16
---

## Synopsis

```python run id=stubs
class MyEmptyClass:
    pass

def initlog(*args):
    pass  # Remember to implement this!

print(type(MyEmptyClass()).__name__, initlog("start"))
```

```output for=stubs
MyEmptyClass None
```

The class and the function have a body that does nothing. The tutorial says that "even functions without a return statement do return a value", and "This value is called None".[^1]

## Semantics

**r1.** "The pass statement does nothing. It can be used when a statement is required syntactically but the program requires no action."[^1] When pass "is executed, nothing happens."[^2] Example: [`stubs`](#stubs).

**r2.** A common use is "creating minimal classes".[^1] Another is "a place-holder for a function or conditional body when you are working on new code, allowing you to keep thinking at a more abstract level."[^1] Example: [`stubs`](#stubs).

**r3.** Instead of pass, "many people use the ellipsis literal ... instead of pass. This use has no special meaning to Python, and is not part of the language definition (you could use any constant expression here)".[^1] Example: [`ellipsis`](#ellipsis).

## Examples

```python run id=ellipsis
def todo():
    ...

class Point:
    ...

print(todo(), Point.__name__)
```

```output for=ellipsis
None Point
```

```python run id=ignore-error
values = ["1", "x", "3"]
total = 0
for v in values:
    try:
        total += int(v)
    except ValueError:
        pass
print(total)
```

```output for=ignore-error
4
```

The except clause needs a body, so pass fills it.

## Pitfalls

A block cannot be empty. A comment is not a statement, so Python raises IndentationError:

```python fails id=empty-body expect=exception:IndentationError
def later():
    # write this later
print("done")
```

Write `pass` or `...` in the body.

An `except ...: pass` hides each error of that type. Catch only the exception type that you expect, as in the `ignore-error` example, and not all exceptions.

## Comparison

Go also has a statement that does nothing: "The empty statement does nothing."[^3] But in Go, "A block is a possibly empty sequence of declarations and statements within matching brace brackets."[^3] So an empty Go block is written `{}`, and it needs no special keyword. In Python, the indentation of a line "is used to determine the grouping of statements".[^4] So an empty block has no lines, and Python needs the pass statement to fill it (rule r1).
