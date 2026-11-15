---
tag: JKYGJ2
kind: feature
owner: python
slug: keywords
name: Keywords
aliases: [Python keywords, reserved words, keyword.kwlist, keyword.iskeyword, hard keywords]
area: languages/python/syntax
level: intro
summary: >-
  Python has 35 keywords, such as if, def and None. They are reserved: a keyword cannot be used as a variable, a function or an attribute name in the source code.
grammar: "identifier: <NAME, except keywords> (Language Reference, 2.3 Names (identifiers and keywords))"
spec: {standard: null, url: "https://docs.python.org/3.15/reference/lexical_analysis.html#keywords", locator: "2.3.1 Keywords"}
see_also: [python/soft-keywords, python/identifiers-in-python]
sources:
  - url: https://docs.python.org/3.15/reference/lexical_analysis.html#keywords
    title: The Python Language Reference, Python 3.15
    locator: 2.3.1 Keywords
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/reference/lexical_analysis.html#identifiers
    title: The Python Language Reference, Python 3.15
    locator: 2.3 Names (identifiers and keywords)
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/library/keyword.html
    title: keyword, Python 3.15 documentation
    locator: keyword.iskeyword(), keyword.kwlist, keyword.issoftkeyword()
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/builtins/functions.html#setattr
    title: Built-in Functions, Python 3.15 documentation
    locator: setattr()
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/whatsnew/3.0.html
    title: What's New In Python 3.0
    locator: New Syntax; Changed Syntax; Removed Syntax
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/whatsnew/3.7.html
    title: What's New In Python 3.7
    locator: Summary; Changes in Python Behavior
    accessed: 2026-11-16
  - url: https://peps.python.org/pep-0008/#descriptive-naming-styles
    title: "PEP 8: Style Guide for Python Code"
    locator: "Descriptive: Naming Styles"
    accessed: 2026-11-16
  - url: https://go.dev/ref/spec#Keywords
    title: The Go Programming Language Specification
    locator: Keywords
    accessed: 2026-11-16
  - url: https://go.dev/ref/spec#Predeclared_identifiers
    title: The Go Programming Language Specification
    locator: Predeclared identifiers
    accessed: 2026-11-16
---

## Synopsis

```python run id=kwlist
import keyword

print(len(keyword.kwlist))
for start in range(0, len(keyword.kwlist), 7):
    print(" ".join(keyword.kwlist[start:start + 7]))
```

```output for=kwlist
35
False None True and as assert async
await break class continue def del elif
else except finally for from global if
import in is lambda nonlocal not or
pass raise return try while with yield
```

The list is the one of CPython 3.12. A keyword "cannot be used as ordinary identifiers", and it must be spelled exactly as the list shows.[^1]

## Semantics

**r1.** The keywords are reserved words. They cannot be ordinary identifiers, and they must be spelled exactly as written in the list, so `True` is a keyword and `true` is a name.[^1] Example: [`kw-as-name`](#kw-as-name).

**r2.** The tokenizer makes a `NAME` token for a keyword as for an identifier, and an identifier is a `NAME` "except keywords".[^2] So the parser, not the tokenizer, rejects a keyword in the place of a name.

**r3.** `keyword.kwlist` holds all the keywords of the running interpreter, and `keyword.iskeyword(s)` returns `True` if `s` is one of them. The soft keywords are in a separate list, and `keyword.issoftkeyword(s)` tests them.[^3] Example: [`kwlist`](#kwlist), [`check-names`](#check-names).

**r4.** The rule is about the source code. The name given to `setattr` "need not be a Python identifier", and an attribute whose name is not an identifier "will not be accessible using the dot notation, but is accessible through" `getattr()`.[^4] So an attribute can be named `class`. Example: [`setattr-keyword`](#setattr-keyword).

**r5.** `None`, `True` and `False` are keywords, so a program cannot assign to them.[^1]

## Examples

`iskeyword` and `issoftkeyword` sort some names (rule r3). `print` and `exec` are built-in functions, not keywords.

```python run id=check-names
import keyword

for name in ["while", "True", "true", "match", "type", "print", "exec"]:
    if keyword.iskeyword(name):
        kind = "keyword"
    elif keyword.issoftkeyword(name):
        kind = "soft keyword"
    else:
        kind = "name"
    print(f"{name:6} {kind}")
```

```output for=check-names
while  keyword
True   keyword
true   name
match  soft keyword
type   soft keyword
print  name
exec   name
```

An attribute named `class` can exist, but only the functions that take the name as a string can use it (rule r4).

```python run id=setattr-keyword
class Element:
    pass

element = Element()
setattr(element, "class", "note")
print(getattr(element, "class"))
print(vars(element))
```

```output for=setattr-keyword
note
{'class': 'note'}
```

## Versions

The list changed in two releases. Python 3.0 made `True`, `False` and `None` reserved words, made `nonlocal` "a new reserved word", and removed the keyword `exec`, which "remains as a function".[^5] Python 3.7 made `async` and `await` reserved keywords: "Code using these names as identifiers will now raise a" `SyntaxError`.[^6]

## Pitfalls

### A keyword as a variable

Words such as `class`, `from`, `in`, `is` and `global` are natural names for data, but each one is a keyword. As a name, it is a `SyntaxError` before the program runs (rule r1).

```python fails id=kw-as-name expect=exception:SyntaxError
class = "header"
print(class)
```

PEP 8 gives the convention for this case: a single trailing underscore, `single_trailing_underscore_`, is "used by convention to avoid conflicts with Python keyword", as in `class_='ClassName'`.[^7]

```python run id=trailing-underscore
def tag(name, class_):
    return f'<{name} class="{class_}">'

print(tag("p", class_="note"))
```

```output for=trailing-underscore
<p class="note">
```

## Comparison

Go reserves 25 keywords, which "may not be used as identifiers".[^8] Its list has no values: `true`, `false` and `nil` are predeclared identifiers in Go, "implicitly declared in the universe block",[^9] while `True`, `False` and `None` are keywords in Python (rule r5).
