---
tag: TSDHYZ
kind: feature
owner: python
slug: output-formatting
name: Output Formatting
aliases: [reprlib, pprint, textwrap, locale formatting, pretty print, wrap text]
area: languages/python/io-and-os
level: core
summary: >-
  Four standard modules shape output: reprlib shortens long representations, pprint breaks data over lines, textwrap fits paragraphs to a width, and locale formats numbers for a culture.
grammar: null
spec: {standard: null, url: "https://docs.python.org/3.15/tutorial/stdlib2.html#output-formatting", locator: "11.1. Output formatting"}
see_also: [python/fancier-output-formatting, python/repr-and-str, pretty-print-json, locale-aware-formatting, format-a-number-with-separators]
sources:
  - url: https://docs.python.org/3.15/tutorial/stdlib2.html#output-formatting
    title: The Python Tutorial, 11. Brief tour of the standard library, part II
    locator: 11.1. Output formatting
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/library/reprlib.html
    title: reprlib, Python 3.15 documentation
    locator: module introduction; class reprlib.Repr
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/library/pprint.html
    title: pprint, Python 3.15 documentation
    locator: module introduction; pprint.pp(); pprint.pprint()
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/library/textwrap.html
    title: textwrap, Python 3.15 documentation
    locator: textwrap.wrap(); textwrap.fill(); textwrap.shorten(); textwrap.dedent()
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/library/locale.html
    title: locale, Python 3.15 documentation
    locator: locale.setlocale(); locale.format_string()
    accessed: 2026-11-16
  - url: https://pkg.go.dev/fmt#hdr-Printing
    title: Package fmt, Go standard library
    locator: Printing, the general verbs
    accessed: 2026-11-16
---

## Synopsis

```python run id=synopsis
import pprint, reprlib, textwrap

print(reprlib.repr(list(range(100))))
pprint.pp({'name': 'Ada', 'langs': ['Python', 'Go', 'Rust'], 'year': 1815}, width=30)
print(textwrap.fill('The textwrap module formats paragraphs of text to fit a given width.', width=24))
```

```output for=synopsis
[0, 1, 2, 3, 4, 5, ...]
{'name': 'Ada',
 'langs': ['Python',
           'Go',
           'Rust'],
 'year': 1815}
The textwrap module
formats paragraphs of
text to fit a given
width.
```

`reprlib.repr()` shows the start of a long list. `pprint.pp()` puts the dictionary on several lines because it does not fit in 30 characters. `textwrap.fill()` breaks the sentence into lines of at most 24 characters.

## Semantics

**r1.** "The reprlib module provides a version of repr() customized for abbreviated displays of large or deeply nested containers".[^1] The limits are attributes of `reprlib.Repr`, with the defaults `maxlevel=6`, `maxlist=6` and `maxstring=30`.[^2] Example: [`repr-limits`](#repr-limits).

**r2.** "The pprint module provides a capability to “pretty-print” arbitrary Python data structures in a form which can be used as input to the interpreter."[^3] "The formatted representation keeps objects on a single line if it can, and breaks them onto multiple lines if they don’t fit within the allowed width, adjustable by the width parameter defaulting to 80 characters."[^3]

**r3.** `pprint.pprint()` is an "Alias for pp() with sort_dicts set to True by default".[^3] So `pprint()` sorts the keys of a dictionary, and `pp()` keeps the insertion order. Example: [`pp-sort`](#pp-sort).

**r4.** `pprint.pformat()` returns the same text as a string. The `depth` parameter limits the nesting: "If the data structure being printed is too deep, the next contained level is replaced by ...."[^3]

**r5.** "The textwrap module formats paragraphs of text to fit a given screen width".[^1] `textwrap.wrap()` "Returns a list of output lines, without final newlines."[^4] `fill()` "is shorthand for "\n".join(wrap(text, ...))".[^4] The default width is 70.

**r6.** `textwrap.shorten()` collapses the whitespace, and if the text is still too long, "enough words are dropped from the end so that the remaining words plus the placeholder fit within width".[^4] `textwrap.dedent()` removes "any common leading whitespace from every line in text."[^4] Example: [`shorten-dedent`](#shorten-dedent).

**r7.** "The locale module accesses a database of culture specific data formats."[^1] `locale.format_string()` "Formats a number val according to the current LC_NUMERIC setting."[^5] The format uses the conventions of the `%` operator, and `grouping=True` adds group separators.[^5]

**r8.** "Initially, when a program is started, the locale is the C locale, no matter what the user’s preferred locale is."[^5] The one exception is LC_CTYPE, which Python sets to the encoding of the user.[^5] `locale.setlocale(locale.LC_ALL, '')` sets "the locale for all categories to the user’s default setting (typically specified in the LANG environment variable)."[^5] Example: [`locale-c`](#locale-c).

## Examples

```python run id=repr-limits
import reprlib

r = reprlib.Repr(maxlist=3, maxstring=12)
print(r.repr([10, 20, 30, 40, 50]))
print(r.repr('a long piece of text'))
print(reprlib.repr(set('supercalifragilisticexpialidocious')))
```

```output for=repr-limits
[10, 20, 30, ...]
'a l...text'
{'a', 'c', 'd', 'e', 'f', 'g', ...}
```

A `Repr` object with its own limits keeps the first items of a list and the two ends of a string (rule r1). The last line is the example of the tutorial.

```python run id=pp-sort
import pprint

data = {'zeta': 1, 'alpha': 2, 'mid': 3}
pprint.pp(data)
pprint.pprint(data)
print(pprint.pformat([[1, [2, [3, [4]]]]], depth=2))
```

```output for=pp-sort
{'zeta': 1, 'alpha': 2, 'mid': 3}
{'alpha': 2, 'mid': 3, 'zeta': 1}
[[1, [...]]]
```

`pp()` keeps the order of the keys and `pprint()` sorts them (rule r3). With `depth=2`, the deeper lists become `[...]` (rule r4).

```python run id=shorten-dedent
import textwrap

print(textwrap.shorten('Hello   brave new world', width=15))
print(textwrap.wrap('one two three four', width=9))
code = '''
    def f():
        return 1
'''
print(textwrap.dedent(code).strip())
```

```output for=shorten-dedent
Hello [...]
['one two', 'three', 'four']
def f():
    return 1
```

`shorten()` drops words from the end and adds the placeholder `' [...]'`. `wrap()` returns the lines as a list. `dedent()` removes the four spaces that all lines share (rule r6).

```python run id=locale-c
import locale

print(locale.setlocale(locale.LC_NUMERIC))
print(locale.format_string('%d', 1234567, grouping=True))
print(locale.format_string('%.2f', 1234567.891, grouping=True))
```

```output for=locale-c
C
1234567
1234567.89
```

In the C locale, `grouping=True` adds no separator (rule r8). A locale with a group separator, such as the user's locale after `locale.setlocale(locale.LC_ALL, '')`, adds the separators. The names of the locales differ between systems; the docs say: "name and availability varies with platform".[^5]

## Pitfalls

`setlocale()` changes global state:

The docs warn: "setlocale() is not thread-safe on most systems."[^5] Call it once at the start of the program, not in a function that a thread can call. For a fixed group separator, the format specifier `f'{n:,}'` gives `1,234,567` in every locale.

`pprint` output is not always valid Python:

"If the formatted structures include objects which are not fundamental Python types, the representation may not be loadable."[^3] For data that another program reads, use the `json` module instead.

## Comparison

`reprlib` and `pprint` print the Python form of a value, as `repr()` does. Go has a verb for the same job: "%#v a Go-syntax representation of the value".[^6] For data that other languages read, `json.dumps(data, indent=2)` gives a pretty form. See [pretty print JSON](pretty-print-json).
