---
tag: CHT661
kind: feature
owner: python
slug: string-methods
name: String methods
aliases: [str methods, split, join, strip, replace, find, startswith, removeprefix, partition, casefold, isdigit]
area: languages/python/strings-and-text
level: intro
summary: >-
  str has methods to split, join, strip, search, replace and change case. Each method returns a new string or another value. The original string does not change, because str is immutable.
grammar: null
spec: {standard: null, url: "https://docs.python.org/3.15/builtins/stdtypes.html#string-methods", locator: "String Methods"}
see_also: [python/str-type, python/strings, python/manual-string-formatting]
sources:
  - url: https://docs.python.org/3.15/builtins/stdtypes.html#string-methods
    title: Built-in Types, Python 3.15 documentation
    locator: String Methods
    accessed: 2026-11-16
  - url: https://peps.python.org/pep-0616/
    title: "PEP 616: String methods to remove prefixes and suffixes"
    locator: Abstract
    accessed: 2026-11-16
  - url: https://pkg.go.dev/strings
    title: Package strings, Go standard library
    locator: func Cut; func Fields
    accessed: 2026-11-16
---

## Synopsis

```python run id=common
line = "  name=Ada Lovelace  "
key, sep, value = line.strip().partition("=")
print(key, "|", value)
print(value.split(), "-".join(value.split()))
print(value.upper(), value.replace("Ada", "A."), value.find("Love"))
```

```output for=common
name | Ada Lovelace
['Ada', 'Lovelace'] Ada-Lovelace
ADA LOVELACE A. Lovelace 4
```

Each method returns a new value. The string `line` stays the same.

## Semantics

**r1.** "Strings implement all of the common sequence operations, along with the additional methods described below."[^1]

**r2.** `split(sep)` returns "a list of the words in the string, using sep as the delimiter string."[^1] "If sep is given, consecutive delimiters are not grouped together and are deemed to delimit empty strings".[^1] "If sep is not specified or is None, a different splitting algorithm is applied: runs of consecutive whitespace are regarded as a single separator".[^1] Example: [`split-forms`](#split-forms).

**r3.** `join(iterable)` returns "a string which is the concatenation of the strings in iterable. A TypeError will be raised if there are any non-string values in iterable, including bytes objects. The separator between elements is the string providing this method."[^1] Example: [`join-numbers`](#join-numbers).

**r4.** For `strip(chars)`, "The chars argument is not a prefix or suffix; rather, all combinations of its values are stripped."[^1] `removeprefix(prefix)` removes one exact prefix: "If the string starts with the prefix string, return string[len(prefix):]. Otherwise, return a copy of the original string".[^1] Example: [`strip-chars`](#strip-chars).

**r5.** `find(sub)` returns "the lowest index in the string where substring sub is found".[^1] It returns -1 if sub is not found.[^1] `index()` is "Like find(), but raise ValueError when the substring is not found."[^1] "To check if sub is a substring or not, use the in operator".[^1]

**r6.** `partition(sep)` splits "at the first occurrence of sep, and return a 3-tuple containing the part before the separator, the separator itself, and the part after the separator. If the separator is not found, return a 3-tuple containing the string itself, followed by two empty strings."[^1]

**r7.** `casefold()` is "similar to lowercasing but more aggressive because it is intended to remove all case distinctions in a string."[^1] Use it to compare text without case. Example: [`casefold`](#casefold).

**r8.** The `is` methods test all characters. For example, `isdigit()` returns "True if all characters in the string are digits and there is at least one character, False otherwise."[^1]

## Examples

```python run id=split-forms
print("1,,2".split(","))
print("  a  b \n c ".split())
print("1,2,3".split(",", maxsplit=1))
print("".split(","), "".split())
```

```output for=split-forms
['1', '', '2']
['a', 'b', 'c']
['1', '2,3']
[''] []
```

With a separator, two commas give an empty item. With no separator, runs of whitespace count as one (rule r2).

```python run id=search
s = "spam, spam, spam"
print(s.find("sp"), s.find("sp", 5), s.find("eggs"))
print(s.count("spam"), s.startswith("spam"), s.endswith(("am", "x")))
print("Py" in "Python")
```

```output for=search
0 6 -1
3 True True
True
```

`endswith()` and `startswith()` also take a tuple of strings to test.

```python run id=casefold
a, b = "Straße", "STRASSE"
print(a.lower() == b.lower(), a.casefold() == b.casefold())
print("Hello world".title(), "hello".capitalize(), "Hi".swapcase())
```

```output for=casefold
False True
Hello World Hello hI
```

`lower()` keeps "ß", but `casefold()` changes it to "ss" (rule r7).

```python run id=classify
for s in ["123", "12.5", "", "abc", "abc1", "   "]:
    print(repr(s), s.isdigit(), s.isalpha(), s.isalnum(), s.isspace())
```

```output for=classify
'123' True False True False
'12.5' False False False False
'' False False False False
'abc' False True True False
'abc1' False False True False
'   ' False False False True
```

An empty string gives False for each test (rule r8).

## Pitfalls

`strip()` removes a set of characters, not a prefix:

```python wrong id=strip-chars
url = "www.comics.com"
print(url.strip("w.com"))
print(url.removeprefix("www.").removesuffix(".com"))
```

```output for=strip-chars
ics
comics
```

The first call removes each `w`, `.`, `c`, `o` and `m` character at the two ends, so it also removes the `com` of `comics`. PEP 616 added `removeprefix()` and `removesuffix()` because users were "surprised that the parameter for lstrip is interpreted as a set of characters, not a substring."[^2] Use these two methods to remove one exact prefix or suffix (rule r4).

`join()` needs strings:

```python fails id=join-numbers expect=exception:TypeError
", ".join([1, 2, 3])
```

Convert the items first: `", ".join(map(str, [1, 2, 3]))` (rule r3).

## Comparison

Go keeps these operations as functions in the strings package, not as methods. For example, `strings.Fields` splits "around each instance of one or more consecutive white space characters".[^3] It is like Python `split()` with no separator. `strings.Cut` "slices s around the first instance of sep, returning the text before and after sep."[^3] It is like Python `partition()` (rule r6).
