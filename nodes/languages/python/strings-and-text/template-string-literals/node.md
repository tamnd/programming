---
tag: QS4QVP
kind: feature
owner: python
slug: template-string-literals
name: Template string literals
aliases: [t-string, t-strings, template string literal, string.templatelib, Template, Interpolation, PEP 750, safe interpolation]
area: languages/python/strings-and-text
level: advanced
summary: >-
  A t-string has the syntax of an f-string, but it gives a Template object, not a str. Code can read the static strings and each interpolated value, and escape the values before it joins them.
grammar: null
spec: {standard: null, url: "https://docs.python.org/3.15/reference/lexical_analysis.html#template-string-literals", locator: "2.5.8. t-strings"}
see_also: [python/formatted-string-literals, python/string-literals, python/repr-and-str]
sources:
  - url: https://docs.python.org/3.15/builtins/stdtypes.html#template-string-literals-t-strings
    title: Built-in Types, Python 3.15 documentation
    locator: Template String Literals (t-strings)
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/library/string.templatelib.html
    title: string.templatelib, Support for template string literals
    locator: Template strings; Types; Helper functions
    accessed: 2026-11-16
  - url: https://peps.python.org/pep-0750/
    title: "PEP 750: Template Strings"
    locator: Abstract; Motivation
    accessed: 2026-11-16
  - url: https://pkg.go.dev/html/template
    title: Package html/template, Go standard library
    locator: Overview; Introduction
    accessed: 2026-11-16
---

## Synopsis

```python run id=parts since=3.14
from string.templatelib import Template

cheese = "Camembert"
template = t"Ah! We do have {cheese}."
print(type(template) is Template)
print(template.strings)
print(template.values)
item = template.interpolations[0]
print(item.value, item.expression, item.conversion, repr(item.format_spec))
```

```output for=parts
True
('Ah! We do have ', '.')
('Camembert',)
Camembert cheese None ''
```

A t-string does not make a string. It makes a Template that keeps the static text and the values apart.

## Semantics

**r1.** "A t-string (formally a template string literal) is a string literal that is prefixed with t or T. These strings follow the same syntax and evaluation rules as formatted string literals".[^1] The difference: "Rather than evaluating to a str object, template string literals evaluate to a string.templatelib.Template object."[^1]

**r2.** "The format() protocol is not used. Instead, the format specifier and conversions (if any) are passed to a new Interpolation object that is created for each evaluated expression."[^1] "It is up to code that processes the resulting Template object to decide how to handle format specifiers and conversions."[^1] Example: [`spec`](#spec).

**r3.** "The strings tuple has one more element than interpolations and values; the interpolations “belong” between the strings."[^2] "Empty strings are included in the tuple".[^2]

**r4.** Iteration yields "each non-empty string and Interpolation in the correct order".[^2] Example: [`escape`](#escape).

**r5.** "Concatenating a Template and a str is not supported."[^2] Two templates can be joined with `+`.[^2] Example: [`add-str`](#add-str).

**r6.** For `=`, "the text of the expression is appended to the literal string that precedes the relevant interpolation."[^1] The conversion is then set to `'r'` by default.[^1] Example: [`debug`](#debug).

**r7.** PEP 750 gives the reason: "f-strings provide no way to intercept and transform interpolated values before they are combined into a final string. As a result, incautious use of f-strings can lead to security vulnerabilities."[^3] Templates give "access to the string and its interpolated values before they are combined."[^3]

## Examples

```python run id=escape since=3.14
from html import escape
from string.templatelib import Interpolation

def render_html(template):
    parts = []
    for item in template:
        if isinstance(item, Interpolation):
            parts.append(escape(str(item.value)))
        else:
            parts.append(item)
    return "".join(parts)

user = "<script>alert(1)</script>"
print(render_html(t"<p>Hello, {user}!</p>"))
```

```output for=escape
<p>Hello, &lt;script&gt;alert(1)&lt;/script&gt;!</p>
```

The function escapes each value, but it keeps the static HTML as it is (rules r4 and r7).

```python run id=spec since=3.14
from string.templatelib import convert

price, label = 3.14159, "tea"
template = t"{label!r}: {price:.2f}"
for item in template.interpolations:
    print(item.expression, item.conversion, repr(item.format_spec))

def render(template):
    out = []
    for item in template:
        if isinstance(item, str):
            out.append(item)
        else:
            value = convert(item.value, item.conversion)
            out.append(format(value, item.format_spec))
    return "".join(out)

print(render(template))
```

```output for=spec
label r ''
price None '.2f'
'tea': 3.14
```

The Template keeps the conversion and the format spec. The `render` function applies them as an f-string would (rule r2).

```python run id=debug since=3.14
x = 42
template = t"{x=}"
print(template.strings, template.interpolations[0].conversion)
```

```output for=debug
('x=', '') r
```

## Pitfalls

A Template is not a str. It does not join with a str:

```python fails id=add-str expect=exception:TypeError since=3.14
name = "Ada"
t"Hello, {name}" + "!"
```

Wrap the str in a Template, or join two t-strings: `t"Hello, {name}" + t"!"` (rule r5).

## Comparison

Go keeps safe HTML output in a library. "Package template (html/template) implements data-driven templates for generating HTML output safe against code injection."[^4] It escapes each value by its place in the page: "The escaping is contextual, so actions can appear within JavaScript, CSS, and URI contexts."[^4] A Python t-string gives the same split of static text and values in the language (rule r1), and a library function decides how to escape the values (rule r7).
