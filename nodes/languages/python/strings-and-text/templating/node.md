---
tag: 7VS1XP
kind: feature
owner: python
slug: templating
name: Templating
aliases: [string.Template, Template strings, dollar substitution, $-strings, safe_substitute, PEP 292, mail merge, user-editable templates]
area: languages/python/strings-and-text
level: core
summary: >-
  string.Template replaces $name placeholders with values from a dict or keyword arguments. The syntax is small, so end users can edit a template. safe_substitute keeps a missing placeholder as it is.
grammar: null
spec: {standard: null, url: "https://docs.python.org/3.15/library/string.html#template-strings-strings", locator: "Template strings ($-strings)"}
see_also: [python/the-string-format-method, python/old-string-formatting, python/template-string-literals, python/formatted-string-literals]
sources:
  - url: https://docs.python.org/3.15/library/string.html#template-strings-strings
    title: string, Common string operations
    locator: Template strings ($-strings)
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/tutorial/stdlib2.html#templating
    title: The Python Tutorial, 11. Brief Tour of the Standard Library, Part II
    locator: 11.2. Templating
    accessed: 2026-11-16
  - url: https://peps.python.org/pep-0292/
    title: "PEP 292: Simpler String Substitutions"
    locator: Abstract; Rationale; A Simpler Proposal
    accessed: 2026-11-16
  - url: https://pkg.go.dev/text/template
    title: Package text/template, Go standard library
    locator: Overview
    accessed: 2026-11-16
---

## Synopsis

```python run id=basics
from string import Template

t = Template("${village}folk send $$10 to $cause.")
print(t.substitute(village="Nottingham", cause="the ditch fund"))
s = Template("$who likes $what")
print(s.substitute({"who": "tim", "what": "kung pao"}))
```

```output for=basics
Nottinghamfolk send $10 to the ditch fund.
tim likes kung pao
```

A placeholder is `$` and a name. The values come from a mapping or from keyword arguments.

## Semantics

**r1.** "Template strings provide simpler string substitutions as described in PEP 292. A primary use case for template strings is for internationalization (i18n) since in that context, the simpler syntax and functionality makes it easier to translate than other built-in string formatting facilities in Python."[^1] The tutorial calls the syntax "suitable for editing by end-users".[^2]

**r2.** "$$ is an escape; it is replaced with a single $."[^1] `$identifier` "names a substitution placeholder matching a mapping key".[^1] "The first non-identifier character after the $ character terminates this placeholder specification."[^1]

**r3.** "${identifier} is equivalent to $identifier. It is required when valid identifier characters follow the placeholder but are not part of the placeholder".[^1] "Any other appearance of $ in the string will result in a ValueError being raised."[^1] Example: [`bad-dollar`](#bad-dollar).

**r4.** `substitute()` takes a mapping, keyword arguments, or both. "When both mapping and kwds are given and there are duplicates, the placeholders from kwds take precedence."[^1] A missing key raises KeyError.[^1] Examples: [`both`](#both), [`missing`](#missing).

**r5.** safe_substitute() works "Like substitute(), except that if placeholders are missing from mapping and kwds, instead of raising a KeyError exception, the original placeholder will appear in the resulting string intact."[^1] It also returns a lone `$` instead of raising ValueError.[^1] Example: [`safe`](#safe).

**r6.** is_valid() "Returns False if the template has invalid placeholders that will cause substitute() to raise ValueError."[^1] get_identifiers() "Returns a list of the valid identifiers in the template, in the order they first appear, ignoring any invalid identifiers."[^1] Example: [`safe`](#safe).

**r7.** A subclass can change the class attributes `delimiter`, `idpattern`, `braceidpattern`, `flags` or `pattern`.[^1] "Note further that you cannot change the delimiter after class creation".[^1] Example: [`delimiter`](#delimiter).

## Examples

```python run id=safe
from string import Template

t = Template("Return the $item to $owner.")
d = {"item": "unladen swallow"}
print(t.safe_substitute(d))
print(t.get_identifiers(), t.is_valid())
price = Template("Give $who $100")
print(price.safe_substitute(who="tim"))
print(price.is_valid())
```

```output for=safe
Return the unladen swallow to $owner.
['item', 'owner'] True
Give tim $100
False
```

safe_substitute() keeps `$owner` because the dict has no value for it. `$100` is not a valid placeholder, so is_valid() returns False (rules r5 and r6).

```python run id=both
from string import Template

t = Template("$x and $y")
print(t.substitute({"x": 1, "y": 2}, x=10))
```

```output for=both
10 and 2
```

The keyword argument wins over the dict key (rule r4).

```python run id=delimiter
from string import Template

class BatchRename(Template):
    delimiter = "%"

t = BatchRename("Ashley_%n%f")
for i, ext in enumerate([".jpg", ".png"]):
    print(t.substitute(n=i, f=ext))
```

```output for=delimiter
Ashley_0.jpg
Ashley_1.png
```

The subclass sets the delimiter in its class body (rule r7).

```python run id=names-only
from string import Template

t = Template("${user.name} wrote $count lines")
print(t.is_valid(), t.get_identifiers())
```

```output for=names-only
False ['count']
```

A placeholder is a name only. A dot is not part of an identifier, so the first placeholder is not valid (rules r2 and r6).

## Pitfalls

A missing value is an error with substitute():

```python fails id=missing expect=exception:KeyError
from string import Template

Template("$who likes $what").substitute(who="tim")
```

Use safe_substitute() if the data can be incomplete (rule r5).

A `$` that does not start a placeholder is an error. Write `$$` for a literal dollar sign:

```python fails id=bad-dollar expect=exception:ValueError
from string import Template

Template("Give $who $100").substitute(who="tim")
```

safe_substitute() hides mistakes in the template. The library documentation warns that it "may be anything other than safe, since it will silently ignore malformed templates containing dangling delimiters, unmatched braces, or placeholders that are not valid Python identifiers."[^1] Call is_valid() to find these mistakes (rule r6).

A string.Template is not a t-string. The library documentation says that this feature "is unrelated to template string literals (t-strings)".[^1]

## Comparison

PEP 292 added Template because "%-formatting codes are also error prone, even for experienced Python programmers."[^3] Go text/template is larger: "Package template implements data-driven templates for generating textual output."[^4] Its actions can read fields of a struct and keys of a map, and it has control structures.[^4] A Python Template only puts values in for names (rule r2). Both leave escaping to the caller: Go text/template "does not auto-escape output".[^4]
