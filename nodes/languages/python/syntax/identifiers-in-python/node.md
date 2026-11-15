---
tag: M9HP0W
kind: feature
owner: python
slug: identifiers-in-python
name: Identifiers in Python
aliases: [Python names, NAME token, non-ASCII identifiers, name mangling, dunder names, private names in Python]
area: languages/python/syntax
level: intro
summary: >-
  A Python identifier is a name that is not a keyword: a letter or underscore, then letters, digits and underscores. Non-ASCII letters are allowed and normalized to NFKC.
grammar: "NAME: name_start name_continue* (Language Reference, 2.3 Names (identifiers and keywords))"
spec: {standard: null, url: "https://docs.python.org/3.15/reference/lexical_analysis.html#identifiers", locator: "2.3 Names (identifiers and keywords)"}
see_also: [python/keywords, python/soft-keywords, python/source-code-encoding]
sources:
  - url: https://docs.python.org/3.15/reference/lexical_analysis.html#identifiers
    title: The Python Language Reference, Python 3.15
    locator: 2.3 Names (identifiers and keywords)
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/builtins/stdtypes.html#str.isidentifier
    title: Built-in Types, Python 3.15 documentation
    locator: str.isidentifier()
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/reference/lexical_analysis.html#non-ascii-characters-in-names
    title: The Python Language Reference, Python 3.15
    locator: 2.3.4 Non-ASCII characters in names
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/reference/lexical_analysis.html#reserved-classes-of-identifiers
    title: The Python Language Reference, Python 3.15
    locator: 2.3.3 Reserved classes of identifiers
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/reference/expressions.html#private-name-mangling
    title: The Python Language Reference, Python 3.15
    locator: 6.2.2.1 Private name mangling
    accessed: 2026-11-16
  - url: https://peps.python.org/pep-0008/#naming-conventions
    title: "PEP 8: Style Guide for Python Code"
    locator: Naming Conventions; Names to Avoid; ASCII Compatibility
    accessed: 2026-11-16
  - url: https://go.dev/ref/spec#Identifiers
    title: The Go Programming Language Specification
    locator: Identifiers
    accessed: 2026-11-16
  - url: https://go.dev/ref/spec#Exported_identifiers
    title: The Go Programming Language Specification
    locator: Exported identifiers
    accessed: 2026-11-16
  - url: https://peps.python.org/pep-3131/
    title: "PEP 3131: Supporting Non-ASCII Identifiers"
    locator: Rationale; Specification of Language Changes
    accessed: 2026-11-16
---

## Synopsis

```python run id=valid-names
import keyword

for name in ["total_2", "_private", "ř_1", "蛇", "2nd", "my-var", "r〰2", "€", "class"]:
    usable = name.isidentifier() and not keyword.iskeyword(name)
    print(f"{name:9} isidentifier={name.isidentifier()!s:5} usable={usable}")
```

```output for=valid-names
total_2   isidentifier=True  usable=True
_private  isidentifier=True  usable=True
ř_1       isidentifier=True  usable=True
蛇         isidentifier=True  usable=True
2nd       isidentifier=False usable=False
my-var    isidentifier=False usable=False
r〰2       isidentifier=False usable=False
€         isidentifier=False usable=False
class     isidentifier=True  usable=False
```

An identifier is a name that is not a keyword. `str.isidentifier()` tests the form, and `keyword.iskeyword()` tests for the reserved words.[^1] [^2]

## Semantics

**r1.** A name is made of the letters `A-Z` and `a-z`, the underscore, the digits `0` to `9`, and non-ASCII characters. A digit cannot be the first character. A name has at least one character and "no upper length limit".[^1] Example: [`valid-names`](#valid-names).

**r2.** "Case is significant": `count` and `Count` are two names.[^1] Example: [`case-matters`](#case-matters).

**r3.** The tokenizer makes one `NAME` token for each identifier, keyword and soft keyword. An identifier is a `NAME` that is not a keyword.[^1]

**r4.** A non-ASCII name must start with a character of the set `xid_start` and continue with characters of `xid_continue`. These sets come from the `XID_Start` and `XID_Continue` sets of Unicode annex UAX-31, and Python adds the underscore to `xid_start`.[^3] So `蛇` is a name, and `€` is not. Example: [`valid-names`](#valid-names).

**r5.** The parser converts all names to the normalization form NFKC, so two spellings with the same NFKC form are the same name. The normalization is "done at the lexical level only": functions that take a name as a string, such as `globals()`, do not normalize it.[^3] Example: [`nfkc`](#nfkc).

**r6.** Some patterns of underscores have a meaning. A name that starts with `_` is "Not imported by" `from module import *`. Names of the form `__*__` are system-defined, and a use that does not follow the documentation "is subject to breakage without warning". Names of the form `__*` are class-private.[^4] Example: [`star-import`](#star-import).

**r7.** In a class definition, an identifier that begins with two or more underscores and does not end in two or more underscores is a private name. It is changed to `_ClassName__name` before the code is generated, with the leading underscores of the class name removed.[^5] Example: [`mangling`](#mangling).

## Examples

NFKC turns the typographic variants of letters into their basic form (rule r5). The assignment binds `finalization`, so that is the key in `globals()`.

```python run id=nfkc
ﬁⁿₐˡᵢᶻₐᵗᵢᵒₙ = 3
print(finalization)
print("finalization" in globals(), "ﬁⁿₐˡᵢᶻₐᵗᵢᵒₙ" in globals())
```

```output for=nfkc
3
True False
```

A private name in a class is stored under the mangled name (rule r7). Code outside the class must use the long form.

```python run id=mangling
class Account:
    def __init__(self):
        self.__secret = 42

account = Account()
print(vars(account))
print(account._Account__secret)
print(hasattr(account, "__secret"))
```

```output for=mangling
{'_Account__secret': 42}
42
False
```

A star import skips the names that start with an underscore (rule r6). The module here is built in memory.

```python run id=star-import
import sys
import types

module = types.ModuleType("tools")
exec("def helper(): pass\ndef _internal(): pass\n", module.__dict__)
sys.modules["tools"] = module

from tools import *
print("helper" in globals(), "_internal" in globals())
```

```output for=star-import
True False
```

## Pitfalls

### Case is part of the name

A name with a different case is another name, so a typo of case is a `NameError` when the line runs (rule r2).

```python fails id=case-matters expect=exception:NameError
count = 3
print(Count)
```

### Letters that look the same

Two letters can look the same and still be different characters, and NFKC does not join them. The Cyrillic `а` (U+0430) is not the Latin `a`, so the two names below are two variables.[^3]

```python wrong id=confusable
pаssword = "set"     # the second letter is Cyrillic
password = "other"
print(pаssword == password, len({"pаssword", "password"}))
```

```output for=confusable
False 2
```

PEP 8 asks that identifiers in the standard library be ASCII compatible, and that `l`, `O` and `I` are never used as single-character names, because in some fonts they look like one and zero.[^6]

## Comparison

In Go, an identifier is "a sequence of one or more letters and digits", and "The first character in an identifier must be a letter".[^7] Go uses the case of the first letter for visibility: an identifier of the package block is exported if its first character is "a Unicode uppercase letter".[^8] Python uses a leading underscore for the same purpose, but only as a convention, and only `import *` and the mangling of rule r7 act on it.

PEP 3131 added the non-ASCII names because developers "often desire to define classes and functions with names in their native languages".[^9]
