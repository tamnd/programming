---
tag: H6HRNJ
kind: feature
owner: python
slug: literal-and-final
name: Literal, LiteralString and Final
aliases: [Literal, typing.Literal, literal type, LiteralString, Final, typing.Final, final, "@final", final name, final class, PEP 586, PEP 591, PEP 675]
area: languages/python/static-typing
level: advanced
summary: >-
  Literal["r", "w"] accepts only those values. LiteralString accepts only strings from the source code. Final and @final mark names, methods and classes that code must not change or extend.
grammar: null
spec: {standard: null, url: "https://typing.python.org/en/latest/spec/literal.html", locator: "Literals; Type qualifiers, Final"}
proposals: [pep-586, pep-591, pep-675]
see_also: [python/basic-type-annotations, python/enums, python/type-checkers, python/type-aliases, literal-types, python/shell-true-injection]
sources:
  - url: https://peps.python.org/pep-0586/
    title: "PEP 586: Literal Types"
    locator: Abstract
    accessed: 2026-11-18
  - url: https://docs.python.org/3.15/library/typing.html#typing.Literal
    title: typing, Support for type hints, Python 3.15 documentation
    locator: typing.Literal; typing.LiteralString; typing.Final; typing.final
    accessed: 2026-11-18
  - url: https://typing.python.org/en/latest/spec/literal.html
    title: Literals, Specification for the Python type system
    locator: Legal parameters for Literal at type check time
    accessed: 2026-11-18
  - url: https://mypy.readthedocs.io/en/stable/literal_types.html
    title: Literal types and Enums, mypy documentation
    locator: Declaring literal variables
    accessed: 2026-11-18
  - url: https://peps.python.org/pep-0591/
    title: "PEP 591: Adding a final qualifier to typing"
    locator: Abstract
    accessed: 2026-11-18
  - url: https://docs.python.org/3.15/whatsnew/3.8.html#typing
    title: What’s New In Python 3.8
    locator: typing
    accessed: 2026-11-18
  - url: https://peps.python.org/pep-0675/
    title: "PEP 675: Arbitrary Literal String Type"
    locator: Abstract
    accessed: 2026-11-18
  - url: https://github.com/python/mypy/issues/12554
    title: "Support PEP 675 (LiteralString), mypy issue 12554"
    locator: first post; comments; state Open on 2026-11-18
    accessed: 2026-11-18
---

## Synopsis

```python run id=mode
from typing import TYPE_CHECKING, Literal

type Mode = Literal["r", "w"]


def open_log(name: str, mode: Mode) -> str:
    return f"{name} opened with {mode!r}"


print(open_log("app.log", "r"))
if TYPE_CHECKING:
    open_log("app.log", "x")
```

```output for=mode
app.log opened with 'r'
```

The parameter `mode` accepts only the strings `"r"` and `"w"`. Saved as `mode.py`:

```text
$ mypy --strict mode.py
mode.py:12: error: Argument 2 to "open_log" has incompatible type "Literal['x']"; expected "Literal['r', 'w']"  [arg-type]
Found 1 error in 1 file (checked 1 source file)
```

## Semantics

**r1.** "Literal types indicate that some expression has literally a specific value."[^1] `Literal` takes one or more values: "The arguments must be literal values and there must be at least one."[^2] The specification lists the allowed values: "Literal may be parameterized with literal int, str, bytes, and bool objects, instances of enum.Enum subclasses, and None."[^3] A float is not allowed. Example: [`mode`](#mode).

**r2.** A union of literals works like a small set of names. The checker narrows the type after a comparison, so it knows which values are left. Example: [`narrow`](#narrow).

**r3.** A variable gets a literal type only with a hint. "In order to preserve backwards-compatibility, variables without this annotation are not assumed to be literals".[^4] A `Final` name keeps the literal value: "If you find repeating the value of the variable in the type hint to be tedious, you can instead change the variable to be Final".[^4] Example: [`inferred-str`](#inferred-str).

**r4.** `LiteralString` accepts any string that comes from the source code. "Any string literal is compatible with LiteralString, as is another LiteralString. However, an object typed as just str is not. A string created by composing LiteralString-typed objects is also acceptable as a LiteralString."[^2] "LiteralString is useful for sensitive APIs where arbitrary user-generated strings could generate problems."[^2] Example: [`literal-string`](#literal-string).

**r5.** `Final` marks a name that code must not assign again. One purpose in PEP 591 is "Declaring that a variable or attribute should not be reassigned".[^5] "Final names cannot be reassigned in any scope. Final names declared in class scopes cannot be overridden in subclasses."[^2] Example: [`final-name`](#final-name).

**r6.** The decorator `@final` marks a method or a class. "Decorating a method with @final indicates to a type checker that the method cannot be overridden in a subclass. Decorating a class with @final indicates that it cannot be subclassed."[^2] Example: [`final-class`](#final-class).

**r7.** Python does not check these rules. Of `Final` and `@final`, the documentation says: "There is no runtime checking of these properties."[^2] Example: [`not-enforced`](#not-enforced).

## Examples

```python run id=narrow
from typing import TYPE_CHECKING, Literal

type Color = Literal["red", "green", "blue"]


def hex_code(c: Color) -> str:
    if c == "red":
        return "#f00"
    if c == "green":
        return "#0f0"
    if TYPE_CHECKING:
        reveal_type(c)
    return "#00f"


print(hex_code("green"), hex_code("blue"))
```

```output for=narrow
#0f0 #00f
```

After the two tests, only `"blue"` is left (rule r2):

```text
$ mypy --strict narrow.py
narrow.py:12: note: Revealed type is "Literal['blue']"
Success: no issues found in 1 source file
```

```python run id=final-name
from typing import TYPE_CHECKING, Final

MAX_SIZE: Final = 100


class Connection:
    TIMEOUT: Final[int] = 10


if TYPE_CHECKING:
    MAX_SIZE += 1

    class Fast(Connection):
        TIMEOUT = 1

print(MAX_SIZE, Connection.TIMEOUT)
```

```output for=final-name
100 10
```

The checker rejects the new value of the module name and the override in the subclass (rule r5):

```text
$ mypy --strict finalname.py
finalname.py:11: error: Cannot assign to final name "MAX_SIZE"  [misc]
finalname.py:14: error: Cannot assign to final name "TIMEOUT"  [misc]
Found 2 errors in 1 file (checked 1 source file)
```

```python run id=final-class
from typing import TYPE_CHECKING, final


class Base:
    @final
    def done(self) -> str:
        return "done"


@final
class Leaf(Base):
    pass


if TYPE_CHECKING:

    class Other(Leaf):
        pass


print(Leaf().done(), getattr(Leaf, "__final__", False))
```

```output for=final-class
done True
```

The decorator sets the attribute `__final__`, which a program can read at run time. The checker rejects the subclass (rule r6):

```text
$ mypy --strict finalclass.py
finalclass.py:17: error: Cannot inherit from final class "Leaf"  [misc]
Found 1 error in 1 file (checked 1 source file)
```

## Versions

> [!since v="3.8"]
> `Literal`, `Final` and `@final` are added, from PEP 586 and PEP 591. "The final qualifier instructs a static type checker to restrict subclassing, overriding, or reassignment".[^6]

> [!since v="3.11"]
> `LiteralString` is added, from PEP 675.[^2][^7]

## Pitfalls

```python wrong id=inferred-str
from typing import Final, Literal


def open_log(mode: Literal["r", "w"]) -> str:
    return "opened " + mode


mode = "r"
print(open_log(mode))
FIXED: Final = "w"
print(open_log(FIXED))
```

```output for=inferred-str
opened r
opened w
```

The variable `mode` has no hint, so its type is `str`, not `Literal["r"]`, and the checker rejects the first call. The `Final` name keeps its literal type (rule r3):

```text
$ mypy --strict inferstr.py
inferstr.py:9: error: Argument 1 to "open_log" has incompatible type "str"; expected "Literal['r', 'w']"  [arg-type]
Found 1 error in 1 file (checked 1 source file)
```

```python wrong id=not-enforced
from typing import Final

RATE: Final = 0.2
RATE = 0.5
print(RATE)
```

```output for=not-enforced
0.5
```

Python runs the assignment, and only the checker reports it (rule r7):

```text
$ mypy --strict notenforced.py
notenforced.py:4: error: Cannot assign to final name "RATE"  [misc]
Found 1 error in 1 file (checked 1 source file)
```

```python wrong id=literal-string
from typing import TYPE_CHECKING, LiteralString


def run_query(sql: LiteralString) -> str:
    return "ran: " + sql


def find(table: LiteralString, name: str) -> str:
    ok = run_query("SELECT * FROM " + table)
    if TYPE_CHECKING:
        run_query(f"SELECT * FROM users WHERE name = '{name}'")
    return ok


print(find("users", "ann"))
```

```output for=literal-string
ran: SELECT * FROM users
```

The f-string puts a `str` from the caller into the query, so the checkers that support PEP 675 reject the call (rule r4). Mypy does not support it. The mypy issue for PEP 675 is open, and its first step was "to treat LiteralString simply as an alias for str."[^8] A comment says: "mypy currently does not actually support LiteralString."[^8] So mypy accepts the file:

```text
$ mypy --strict litstr.py
Success: no issues found in 1 source file
```

To use `LiteralString` as a guard against injection, run a checker that supports it.

## Comparison

In Python, `Literal` makes a type from a few values, which the checker treats like a small enumeration (rule r2). For a set of named values that also exists at run time, see [[python/enums]]. `Final` is like a constant in other languages, but Python does not enforce it (rule r7). For the theory of literal types, see [[literal-types]].
