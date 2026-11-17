---
tag: BN2WWG
kind: feature
owner: python
slug: naming-conventions
name: Naming conventions
aliases: [snake_case, CapWords, CamelCase, UPPER_CASE, leading underscore, double underscore, name mangling, trailing underscore, dunder names]
area: languages/python/idioms-and-style
level: intro
summary: >-
  PEP 8 names modules, functions and variables in lowercase_with_underscores, classes in CapWords and constants in UPPER_CASE. A leading underscore marks a name as internal.
grammar: null
spec: {standard: null, url: "https://peps.python.org/pep-0008/#naming-conventions", locator: "PEP 8, Naming Conventions"}
proposals: [pep-8]
see_also: [python/coding-style, python/identifiers-in-python, python/private-variables, python/constants-by-convention, python/keywords, python/soft-keywords, python/the-builtins-namespace, python/importing-from-a-package]
sources:
  - url: https://peps.python.org/pep-0008/#naming-conventions
    title: PEP 8, Style Guide for Python Code
    locator: Naming Conventions; Descriptive, Naming Styles; Prescriptive, Naming Conventions; Designing for Inheritance; Public and Internal Interfaces
    accessed: 2026-11-18
  - url: https://docs.python.org/3.15/tutorial/classes.html#private-variables
    title: The Python Tutorial, Python 3.15 documentation
    locator: 9.6 Private Variables
    accessed: 2026-11-18
  - url: https://docs.python.org/3.15/reference/simple_stmts.html#the-import-statement
    title: The Python Language Reference, Python 3.15 documentation
    locator: 7.11 The import statement
    accessed: 2026-11-18
  - url: https://docs.python.org/3.15/reference/lexical_analysis.html#reserved-classes-of-identifiers
    title: The Python Language Reference, Python 3.15 documentation
    locator: 2.3.3 Reserved classes of identifiers
    accessed: 2026-11-18
  - url: https://docs.astral.sh/ruff/rules/ambiguous-variable-name/
    title: ambiguous-variable-name (E741), Ruff documentation
    locator: What it does
    accessed: 2026-11-18
  - url: https://docs.astral.sh/ruff/rules/builtin-variable-shadowing/
    title: builtin-variable-shadowing (A001), Ruff documentation
    locator: What it does; Why is this bad?
    accessed: 2026-11-18
  - url: https://docs.astral.sh/ruff/rules/invalid-function-name/
    title: invalid-function-name (N802), Ruff documentation
    locator: What it does
    accessed: 2026-11-18
  - url: https://docs.python.org/3.15/reference/compound_stmts.html#type-params
    title: The Python Language Reference, Python 3.15 documentation
    locator: 8.10 Type parameter lists
    accessed: 2026-11-18
  - url: https://google.github.io/styleguide/pyguide.html
    title: Google Python Style Guide
    locator: 3.16.1 Names to Avoid; 3.16.2 Naming Conventions
    accessed: 2026-11-18
---

## Synopsis

```python run id=styles
import keyword

MAX_RETRIES = 3


class HTTPServerError(Exception):
    pass


def parse_header(raw_line):
    name, _, value = raw_line.partition(":")
    return name.strip().lower(), value.strip()


print(parse_header("Content-Type: text/plain"))
print(HTTPServerError.__name__, MAX_RETRIES)
print(keyword.iskeyword("class"), keyword.iskeyword("class_"))
```

```output for=styles
('content-type', 'text/plain')
HTTPServerError 3
True False
```

## Semantics

**r1.** The rules are conventions, and the interpreter does not check them. PEP 8 admits that "The naming conventions of Python’s library are a bit of a mess".[^1] New code should follow them, "but where an existing library has a different style, internal consistency is preferred."[^1] The main principle: "Names that are visible to the user as public parts of the API should follow conventions that reflect usage rather than implementation."[^1]

**r2.** Modules and packages: "Modules should have short, all-lowercase names. Underscores can be used in the module name if it improves readability."[^1] Packages should also be short and all-lowercase, "although the use of underscores is discouraged."[^1]

**r3.** Classes: "Class names should normally use the CapWords convention."[^1] With an acronym, "capitalize all the letters of the acronym. Thus HTTPServerError is better than HttpServerError."[^1] An exception is a class, and "you should use the suffix “Error” on your exception names (if the exception actually is an error)."[^1] Example: [`styles`](#styles).

**r4.** Functions and variables: "Function names should be lowercase, with words separated by underscores as necessary to improve readability. Variable names follow the same convention as function names."[^1] Methods and instance variables use the same rule. "mixedCase is allowed only in contexts where that’s already the prevailing style".[^1]

**r5.** Constants: "Constants are usually defined on a module level and written in all capital letters with underscores separating words. Examples include MAX_OVERFLOW and TOTAL."[^1] Nothing stops a later assignment to a constant (see [[python/constants-by-convention]]).

**r6.** Method arguments: "Always use self for the first argument to instance methods. Always use cls for the first argument to class methods."[^1] Example: [`self-and-cls`](#self-and-cls).

**r7.** A single leading underscore is a "weak “internal use” indicator."[^1] The tutorial says that such a name "should be treated as a non-public part of the API".[^2] The import system uses it too: when a module has no `__all__`, `from module import *` binds only the names that "do not begin with an underscore character ('_')."[^3] Example: [`underscore-star`](#underscore-star).

**r8.** A single trailing underscore is "used by convention to avoid conflicts with Python keyword", as in `class_`.[^1] PEP 8 prefers this to a misspelling: "Thus class_ is better than clss."[^1] Example: [`self-and-cls`](#self-and-cls).

**r9.** Two leading underscores in a class start name mangling. "Any identifier of the form __spam (at least two leading underscores, at most one trailing underscore) is textually replaced with _classname__spam".[^2] Its purpose is to avoid clashes with subclasses, so "double leading underscores should be used only to avoid name conflicts with attributes in classes designed to be subclassed."[^1] Example: [`mangling`](#mangling).

**r10.** Names with two leading and two trailing underscores belong to Python. These are "System-defined names, informally known as “dunder” names."[^4] PEP 8 says: "Never invent such names; only use them as documented."[^1]

**r11.** Names to avoid: "Never use the characters ‘l’ (lowercase letter el), ‘O’ (uppercase letter oh), or ‘I’ (uppercase letter eye) as single character variable names."[^1] A name that hides a built-in, such as `list` or `max`, also makes code hard to read. Ruff checks these with E741 and A001, and it checks function names with N802.[^5][^6][^7] Example: [`shadow-builtin`](#shadow-builtin).

**r12.** Type variables "should normally use CapWords preferring short names: T, AnyStr, Num."[^1] The suffixes `_co` and `_contra` mark covariant and contravariant type variables.[^1]

## Examples

```python run id=self-and-cls
class Celsius:
    def __init__(self, degrees):
        self.degrees = degrees

    @classmethod
    def from_fahrenheit(cls, value):
        return cls((value - 32) * 5 / 9)


def make_tag(name, class_=None):
    if class_ is None:
        return f"<{name}>"
    return f"<{name} class='{class_}'>"


print(Celsius.from_fahrenheit(212).degrees)
print(make_tag("p"), make_tag("p", class_="note"))
```

```output for=self-and-cls
100.0
<p> <p class='note'>
```

`self` and `cls` are only names, but all Python code uses them (rule r6). `class` is a keyword, so the parameter is `class_` (rule r8).

```python run id=underscore-star
import sys
import types

shapes = types.ModuleType("shapes")
exec(
    "def area(w, h):\n    return w * h\n"
    "def _check(w):\n    return w > 0\n"
    "UNIT = 'cm'\n",
    shapes.__dict__,
)
sys.modules["shapes"] = shapes

from shapes import *

print(area(2, 3), UNIT)
print("_check" in dir(), shapes._check(2))
```

```output for=underscore-star
6 cm
False True
```

The star import skips `_check`, because the module has no `__all__` (rule r7). The name is still there: `shapes._check` works. The underscore is a message to the reader, not a lock.

```python run id=mangling
class Base:
    def __init__(self):
        self.__token = "base"

    def token(self):
        return self.__token


class Child(Base):
    def __init__(self):
        super().__init__()
        self.__token = "child"


c = Child()
print(c.token())
print(sorted(k for k in vars(c) if "token" in k))
```

```output for=mangling
base
['_Base__token', '_Child__token']
```

Each class gets its own attribute, so `Child` cannot overwrite the attribute of `Base` by accident (rule r9).

## Versions

> [!since v="3.12"]
> A function, a class or a type alias can declare its type parameters in square brackets, as in `def max[T](args: list[T]) -> T`.[^8] The type parameter names follow the type variable convention of rule r12.

## Pitfalls

```python wrong id=shadow-builtin
list = [3, 1, 2]
print(sorted(list))
try:
    print(list("abc"))
except TypeError as error:
    print(type(error).__name__)
```

```output for=shadow-builtin
[1, 2, 3]
TypeError
```

The variable `list` hides the built-in type, so the call `list` with a string argument calls the list object. Ruff says that such a name "can cause non-obvious errors, as readers may mistake the variable for the builtin and vice versa."[^6] Use a name such as `values` or `items`.

```python fails id=private-from-outside expect=exception:AttributeError
class Account:
    def __init__(self):
        self.__balance = 10


acct = Account()
print(acct._Account__balance)
print(acct.__balance)
```

Mangling happens only inside the class body, so `acct.__balance` outside the class looks for an attribute that does not exist. The mangled name `_Account__balance` still works. The tutorial says that private instance variables "that cannot be accessed except from inside an object don’t exist in Python."[^2]

## Comparison

Other guides follow PEP 8 for names. The Google Python Style Guide says: "In general, follow PEP 8 naming conventions."[^9] It discourages the double underscore "as it impacts readability and testability, and isn’t really private. Prefer a single underscore."[^9] For the rules that the language enforces on names, see [[python/identifiers-in-python]] and [[python/keywords]]. For name mangling in detail, see [[python/private-variables]].
