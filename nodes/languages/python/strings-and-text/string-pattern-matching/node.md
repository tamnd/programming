---
tag: WXEEBG
kind: feature
owner: python
slug: string-pattern-matching
name: String Pattern Matching
aliases: [re module, regular expressions in Python, re.search, re.match, re.fullmatch, re.findall, re.sub, re.compile, Match object, regex in Python]
area: languages/python/strings-and-text
level: core
summary: >-
  The re module finds text that matches a regular expression. search() looks anywhere, match() looks at the start, findall() and sub() work on every match. Write patterns as raw strings.
grammar: null
spec: {standard: null, url: "https://docs.python.org/3.15/library/re.html", locator: "re, Regular expression operations"}
see_also: [python/string-methods, python/raw-strings, python/strings]
sources:
  - url: https://docs.python.org/3.15/tutorial/stdlib.html#string-pattern-matching
    title: The Python Tutorial, 10. Brief Tour of the Standard Library
    locator: 10.5. String pattern matching
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/library/re.html
    title: re, Regular expression operations
    locator: Module Contents; Functions; Match Objects
    accessed: 2026-11-16
  - url: https://pkg.go.dev/regexp
    title: Package regexp, Go standard library
    locator: Overview
    accessed: 2026-11-16
---

## Synopsis

```python run id=basics
import re

print(re.findall(r"\bf[a-z]*", "which foot or hand fell fastest"))
print(re.sub(r"(\b[a-z]+) \1", r"\1", "cat in the the hat"))
m = re.search(r"(\w+)=(\d+)", "set width=20")
print(m.group(0), m.group(1), m.group(2), m.span())
print("tea for too".replace("too", "two"))
```

```output for=basics
['foot', 'fell', 'fastest']
cat in the hat
width=20 width 20 (4, 12)
tea for two
```

A pattern describes a set of strings. The last line shows a plain string method, which is enough when the text to find is fixed.

## Semantics

**r1.** "The re module provides regular expression tools for advanced string processing."[^1] "When only simple capabilities are needed, string methods are preferred because they are easier to read and debug".[^1] The pattern syntax is like the syntax of Perl: the module "provides regular expression matching operations similar to those found in Perl."[^2]

**r2.** "The solution is to use Python’s raw string notation for regular expression patterns; backslashes are not handled in any special way in a string literal prefixed with 'r'."[^2] Example: [`raw`](#raw).

**r3.** search() scans "through string looking for the first location where the regular expression pattern produces a match".[^2] match() returns a match "If zero or more characters at the beginning of string match the regular expression pattern".[^2] fullmatch() returns a match "If the whole string matches the regular expression pattern".[^2] Each returns None if there is no match.[^2] Example: [`where`](#where).

**r4.** findall() returns "all non-overlapping matches of pattern in string, as a list of strings or tuples."[^2] "If there are no groups, return a list of strings matching the whole pattern. If there is exactly one group, return a list of strings matching that group. If multiple groups are present, return a list of tuples of strings matching the groups."[^2] Example: [`groups`](#groups).

**r5.** sub() replaces each match with repl. "repl can be a string or a function".[^2] In a string, "Backreferences, such as \6, are replaced with the substring matched by group 6 in the pattern."[^2] Example: [`sub-function`](#sub-function).

**r6.** "Match objects always have a boolean value of True."[^2] So `if m:` tests for a match. group() returns "one or more subgroups of the match", and groupdict() returns the named groups.[^2] Example: [`named`](#named).

**r7.** compile() makes a pattern object. Reuse of the object "is more efficient when the expression will be used several times in a single program."[^2] But "The compiled versions of the most recent patterns passed to re.compile() and the module-level matching functions are cached".[^2]

**r8.** "Unicode strings and 8-bit strings cannot be mixed".[^2] A str pattern searches a str, and a bytes pattern searches bytes.

## Examples

```python run id=where
import re

text = "abcdef"
print(re.match("c", text), re.search("c", text))
print(re.fullmatch(r"p.*n", "python") is not None)
print(re.fullmatch(r"p.*n", "pythons"))
```

```output for=where
None <re.Match object; span=(2, 3), match='c'>
True
None
```

match() fails because `c` is not at the start. search() finds it at index 2 (rule r3).

```python run id=groups
import re

text = "set width=20 and height=10"
print(re.findall(r"\w+=\d+", text))
print(re.findall(r"\w+=(\d+)", text))
print(re.findall(r"(\w+)=(\d+)", text))
print(re.split(r"\W+", "Words, words, words."))
```

```output for=groups
['width=20', 'height=10']
['20', '10']
[('width', '20'), ('height', '10')]
['Words', 'words', 'words', '']
```

The number of groups decides the form of the findall() result (rule r4).

```python run id=named
import re

date = re.compile(r"(?P<year>\d{4})-(?P<month>\d{2})-(?P<day>\d{2})")
for line in ["due 2026-11-16", "no date here"]:
    if m := date.search(line):
        print(m["year"], m.group("month"), m.groupdict())
    else:
        print("no match:", line)
```

```output for=named
2026 11 {'year': '2026', 'month': '11', 'day': '16'}
no match: no date here
```

A compiled pattern has the same methods as the module functions. The walrus operator keeps the Match object for the if block (rules r6 and r7).

```python run id=sub-function
import re

def dashrepl(m):
    return " " if m.group(0) == "-" else "-"

print(re.sub("-{1,2}", dashrepl, "pro----gram-files"))
print(re.sub(r"\sAND\s", " & ", "Baked Beans And Spam", flags=re.IGNORECASE))
print(re.escape("1+1=2?"))
```

```output for=sub-function
pro--gram files
Baked Beans & Spam
1\+1=2\?
```

The function gets each Match object and returns the replacement (rule r5). re.escape() makes a literal string safe to use in a pattern.

```python run id=prefixmatch since=3.15
import re

print(re.prefixmatch("c", "abcdef"), re.prefixmatch("a", "abcdef"))
```

```output for=prefixmatch
None <re.Match object; span=(0, 1), match='a'>
```

prefixmatch() is a new name for match(). The documentation explains: "The norm in other languages and regular expression implementations is to use the term match to refer to the behavior of what Python has always called search()."[^2]

## Pitfalls

Without the r prefix, Python reads the backslash escapes first:

```python wrong id=raw
import re

print(re.findall("\bf[a-z]*", "which foot or hand fell fastest"))
print(re.findall(r"\bf[a-z]*", "which foot or hand fell fastest"))
```

```output for=raw
[]
['foot', 'fell', 'fastest']
```

In the first call, `\b` is a backspace character, not a word boundary, so nothing matches. Write each pattern as a raw string (rule r2).

A failed search returns None, and None has no group() method:

```python fails id=none-group expect=exception:AttributeError
import re

re.search(r"\d+", "no digits").group()
```

Test the result before you use it (rule r6).

A str pattern does not search bytes:

```python fails id=mix expect=exception:TypeError
import re

re.search(r"\d+", b"abc 123")
```

## Comparison

Go regexp uses a similar syntax but a different engine: "it is the syntax accepted by RE2".[^3] "The regexp implementation provided by this package is guaranteed to run in time linear in the size of the input."[^3] Go has no separate match() function: "MatchString reports whether the string s contains any match of the regular expression pattern."[^3] This is like Python search() (rule r3).
