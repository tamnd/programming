---
tag: FN6HCT
kind: feature
owner: python
slug: the-string-format-method
name: The String format() Method
aliases: [str.format, format method, format string syntax, replacement field, format_map, positional fields, named fields]
area: languages/python/strings-and-text
level: core
summary: >-
  str.format() replaces each field in braces with an argument. A field names an argument by position or keyword, can select an attribute or an item, and can add a conversion and a format spec.
grammar: "replacement_field: \"{\" [field_name] [\"!\" conversion] [\":\" format_spec] \"}\" (string module, Format string syntax)"
spec: {standard: null, url: "https://docs.python.org/3.15/library/string.html#format-string-syntax", locator: "Format string syntax"}
see_also: [python/formatted-string-literals, python/format-specification-mini-language, python/repr-and-str]
sources:
  - url: https://docs.python.org/3.15/tutorial/inputoutput.html#the-string-format-method
    title: The Python Tutorial, 7. Input and Output
    locator: 7.1.2. The String format() Method
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/library/string.html#format-string-syntax
    title: string, Common string operations
    locator: Format string syntax
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/builtins/stdtypes.html#str.format
    title: Built-in Types, Python 3.15 documentation
    locator: str.format(); str.format_map()
    accessed: 2026-11-16
  - url: https://pkg.go.dev/fmt
    title: Package fmt, Go standard library
    locator: Explicit argument indexes
    accessed: 2026-11-16
---

## Synopsis

```python run id=fields
print('We are the {} who say "{}!"'.format("knights", "Ni"))
print("{1} and {0}".format("spam", "eggs"))
print("This {food} is {adjective}.".format(food="spam", adjective="absolutely horrible"))
print("{0:2d} {1:3d} {2:4d}".format(3, 9, 27))
```

```output for=fields
We are the knights who say "Ni!"
eggs and spam
This spam is absolutely horrible.
 3   9   27
```

A field can be empty, a number or a name. A format spec after the colon sets the width.

## Semantics

**r1.** "Format strings contain “replacement fields” surrounded by curly braces {}. Anything that is not contained in braces is considered literal text, which is copied unchanged to the output. If you need to include a brace character in the literal text, it can be escaped by doubling: {{ and }}."[^2]

**r2.** The `arg_name` of a field "is either a number or a keyword. If it’s a number, it refers to a positional argument, and if it’s a keyword, it refers to a named keyword argument."[^2] "If the numerical arg_names in a format string are 0, 1, 2, … in sequence, they can all be omitted (not just some)".[^2] Example: [`mixed-numbering`](#mixed-numbering).

**r3.** "An expression of the form '.name' selects the named attribute using getattr(), while an expression of the form '[index]' does an index lookup using __getitem__()."[^2] "Because arg_name is not quote-delimited, it is not possible to specify arbitrary dictionary keys (e.g., the strings '10' or ':-]') within a format string."[^2] Example: [`lookup`](#lookup).

**r4.** "Three conversion flags are currently supported: '!s' which calls str() on the value, '!r' which calls repr() and '!a' which calls ascii()."[^2]

**r5.** "A format_spec field can also include nested replacement fields within it."[^2] "The replacement fields within the format_spec are substituted before the format_spec string is interpreted."[^2]

**r6.** Unlike an f-string, the syntax "does not support arbitrary expressions in interpolations."[^2] A field can only name an argument and then select attributes and items.

**r7.** `format_map(mapping)` is "Similar to str.format(**mapping), except that mapping is used directly and not copied to a dict."[^3] Example: [`format-map`](#format-map).

## Examples

```python run id=lookup
import types

point = types.SimpleNamespace(x=3, y=4)
table = {"Jack": 4098, "Sjoerd": 4127}
print("x={0.x} y={0.y}".format(point))
print("Jack: {0[Jack]:d}; Sjoerd: {0[Sjoerd]:d}".format(table))
print("Jack: {Jack:d}".format(**table))
print("{0[1]} {{literal}}".format(["a", "b"]))
```

```output for=lookup
x=3 y=4
Jack: 4098; Sjoerd: 4127
Jack: 4098
b {literal}
```

`[Jack]` looks up the key "Jack" with no quotes. `[1]` is the integer 1 (rule r3).

```python run id=reuse
template = "{name:>8} | {score:6.1f} | {name!r}"
for name, score in [("Ada", 97.5), ("Grace", 88.0)]:
    print(template.format(name=name, score=score))
print("{:{width}.{prec}f}".format(3.14159, width=8, prec=2))
```

```output for=reuse
     Ada |   97.5 | 'Ada'
   Grace |   88.0 | 'Grace'
    3.14
```

A program can keep a format string and fill it many times. The last line uses nested fields for the width and the precision (rule r5).

```python run id=format-map
class Default(dict):
    def __missing__(self, key):
        return "?" + key

print("{name} was born in {country}".format_map(Default(name="Guido")))
```

```output for=format-map
Guido was born in ?country
```

The mapping is used as it is, so its `__missing__` method runs for the missing key (rule r7).

## Pitfalls

A format string cannot mix empty fields and numbered fields:

```python fails id=mixed-numbering expect=exception:ValueError
"{} and {1}".format("spam", "eggs")
```

A missing keyword argument is a KeyError:

```python fails id=missing-key expect=exception:KeyError
"Hello, {name}".format(nmae="Ada")
```

A field cannot hold an expression:

```python fails id=no-expression expect=exception:KeyError
"{a + b}".format(a=1, b=2)
```

The text `a + b` is read as one argument name (rule r6). Use an f-string to evaluate an expression.

## Comparison

Go `fmt.Sprintf` uses verbs that take the arguments in order. "However, the notation [n] immediately before the verb indicates that the nth one-indexed argument is to be formatted instead."[^4] Python numbers fields from 0, as in `{1}` (rule r2), and it also has named fields, which Go verbs do not have.
