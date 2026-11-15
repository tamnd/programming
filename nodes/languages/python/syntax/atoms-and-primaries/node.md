---
tag: EWXKGS
kind: feature
owner: python
slug: atoms-and-primaries
name: Atoms and primaries
aliases: [Python atoms, primary expressions, attribute reference, subscription, call expression, parenthesized group]
area: languages/python/syntax
level: advanced
summary: >-
  Atoms are the smallest Python expressions: names, literals, constants and bracketed forms. Primaries add attribute references, subscriptions and calls, the most tightly bound operations.
grammar: "primary: atom | attributeref | subscription | call (Language Reference, 6.3 Primaries)"
spec: {standard: null, url: "https://docs.python.org/3.15/reference/expressions.html#atoms", locator: "6.2 Atoms; 6.3 Primaries"}
see_also: [python/identifiers-in-python, python/string-literals, python/numeric-literals, python/operators-and-delimiters]
sources:
  - url: https://docs.python.org/3.15/reference/expressions.html#primaries
    title: The Python Language Reference, Python 3.15
    locator: 6.3 Primaries; 6.3.1 Attribute references
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/library/ast.html
    title: ast, Python 3.15 documentation
    locator: Abstract Grammar; ast.parse()
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/reference/expressions.html#atoms
    title: The Python Language Reference, Python 3.15
    locator: 6.2 Atoms
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/reference/expressions.html#built-in-constants
    title: The Python Language Reference, Python 3.15
    locator: 6.2.1 Built-in constants
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/reference/expressions.html#atom-identifiers
    title: The Python Language Reference, Python 3.15
    locator: 6.2.2 Identifiers (Names)
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/reference/expressions.html#parenthesized-groups
    title: The Python Language Reference, Python 3.15
    locator: 6.2.4 Parenthesized groups; 6.2.5.3 Tuple displays
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/reference/expressions.html#subscriptions
    title: The Python Language Reference, Python 3.15
    locator: 6.3.2 Subscriptions and slicings
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/reference/expressions.html#calls
    title: The Python Language Reference, Python 3.15
    locator: 6.3.3 Calls
    accessed: 2026-11-16
---

## Synopsis

```python run id=primary-chain
import ast

node = ast.parse('config.handlers["save"](path)', mode="eval").body
while True:
    print(type(node).__name__)
    if isinstance(node, ast.Call):
        node = node.func
    elif isinstance(node, (ast.Subscript, ast.Attribute)):
        node = node.value
    else:
        break
```

```output for=primary-chain
Call
Subscript
Attribute
Name
```

The expression is a call of a subscription of an attribute reference of a name. Each primary is built on the one before it, from left to right.[^1] [^2]

## Semantics

**r1.** Atoms are "the most basic elements of expressions": built-in constants, names, literals, and the forms in paired delimiters. Parentheses hold groups, tuple displays, yield atoms and generator expressions; square brackets hold list displays; braces hold dictionary and set displays.[^3]

**r2.** `True`, `False`, `None` and `...` are the built-in constants. They are keywords or tokens, so they "cannot be reassigned or used as attributes".[^4] Example: [`assign-constant`](#assign-constant).

**r3.** A name as an atom evaluates to the object it is bound to. An unbound name raises `NameError`.[^5]

**r4.** A group is one expression in parentheses, with the value of that expression, and it is used to override or clarify precedence. A group cannot end with a comma: a one-element tuple display needs the trailing comma, and `()` is the empty tuple.[^6] Example: [`group-or-tuple`](#group-or-tuple).

**r5.** Primaries are "the most tightly bound operations of the language": an atom, an attribute reference, a subscription or a call.[^1] Example: [`primary-chain`](#primary-chain).

**r6.** An attribute reference `primary.name` asks the object for the attribute. `__getattribute__()` is called first, and `__getattr__()` is the fallback when it raises `AttributeError`.[^1]

**r7.** A subscription `primary[subscript]` evaluates the primary and the subscript, and calls `__getitem__()`, or `__class_getitem__()` for a generic type. A slice `a:b:c` becomes a `slice` object, with `None` for a missing part. Several subscripts separated by commas become one tuple.[^7] Example: [`subscripts`](#subscripts).

**r8.** A call `primary(arguments)` needs a callable object, and "All argument expressions are evaluated before the call is attempted". A call always returns a value, possibly `None`, unless it raises an exception.[^8] Example: [`call-order`](#call-order).

## Examples

What `__getitem__` receives for each form of subscript (rule r7).

```python run id=subscripts
class Show:
    def __getitem__(self, key):
        return key

show = Show()
print(show[1])
print(show[2:3])
print(show[::2])
print(show[1:2, 3])
print(show["spam",])
```

```output for=subscripts
1
slice(2, 3, None)
slice(None, None, 2)
(slice(1, 2, None), 3)
('spam',)
```

The arguments are evaluated before the function runs (rule r8).

```python run id=call-order
def arg(label):
    print("evaluate", label)
    return label

def show(*items):
    print("call with", items)

show(arg("a"), arg("b"))
```

```output for=call-order
evaluate a
evaluate b
call with ('a', 'b')
```

## Pitfalls

### Parentheses do not make a tuple

The comma makes the tuple, not the parentheses (rule r4). Without the comma, the parentheses are a group.

```python wrong id=group-or-tuple
single = ("red")
print(type(single).__name__, len(single))
fixed = ("red",)
print(type(fixed).__name__, len(fixed), type(()).__name__)
```

```output for=group-or-tuple
str 3
tuple 1 tuple
```

### A built-in constant is not a variable

`True`, `False` and `None` are keywords, so assigning to them is a `SyntaxError` (rule r2).

```python fails id=assign-constant expect=exception:SyntaxError
False = 123
```
