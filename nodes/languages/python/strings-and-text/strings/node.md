---
tag: 2JD9RB
kind: feature
owner: python
slug: strings
name: Text
aliases: [Python strings, str, text in Python, string concatenation, string repetition, string indexing, immutable string]
area: languages/python/strings-and-text
level: intro
summary: >-
  Python keeps text in str objects. A program joins strings with +, repeats them with *, and reads characters by index or slice. A string is immutable, so a change makes a new string.
grammar: null
spec: {standard: null, url: "https://docs.python.org/3.15/builtins/stdtypes.html#text-sequence-type-str", locator: "Text Sequence Type: str"}
see_also: [python/str-type, python/string-methods, python/string-literals, python/slicing, python/formatted-string-literals]
sources:
  - url: https://docs.python.org/3.15/tutorial/introduction.html#text
    title: The Python Tutorial, 3. An Informal Introduction to Python
    locator: 3.1.2. Text
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/builtins/stdtypes.html#text-sequence-type-str
    title: Built-in Types, Python 3.15 documentation
    locator: Text Sequence Type, str
    accessed: 2026-11-16
  - url: https://go.dev/ref/spec#String_types
    title: The Go Programming Language Specification
    locator: String types
    accessed: 2026-11-16
---

## Synopsis

```python run id=basics
word = "Python"
print(word[0], word[-1], word[2:5])
print(3 * "un" + "ium")
print("Py" "thon", len(word))
```

```output for=basics
P n tho
unununium
Python 6
```

A string is a sequence of characters. The program reads one character by index and a substring by slice.

## Semantics

**r1.** "Textual data in Python is handled with str objects, or strings."[^2] A string can be in single quotes or double quotes, "with the same result".[^1]

**r2.** "The print() function produces a more readable output, by omitting the enclosing quotes and by printing escaped and special characters".[^1] Example: [`print-and-repr`](#print-and-repr).

**r3.** "Strings can be concatenated (glued together) with the + operator, and repeated with *".[^1] "Two or more string literals (i.e. the ones enclosed between quotes) next to each other are automatically concatenated."[^1] "This only works with two literals though, not with variables or expressions".[^1] Example: [`literal-join`](#literal-join).

**r4.** "Strings can be indexed (subscripted), with the first character having index 0. There is no separate character type; a character is simply a string of size one".[^1] "Indices may also be negative numbers, to start counting from the right".[^1]

**r5.** "Note how the start is always included, and the end always excluded. This makes sure that s[:i] + s[i:] is always equal to s".[^1] "Attempting to use an index that is too large will result in an error".[^1] "However, out of range slice indexes are handled gracefully when used for slicing".[^1] Examples: [`slices`](#slices), [`index-error`](#index-error).

**r6.** "Python strings cannot be changed".[^1] "Therefore, assigning to an indexed position in the string results in an error".[^1] "If you need a different string, you should create a new one".[^1] Example: [`immutable`](#immutable).

**r7.** "The built-in function len() returns the length of a string".[^1]

## Examples

```python run id=print-and-repr
s = "First line.\nSecond line."
print(repr(s))
print(s)
```

```output for=print-and-repr
'First line.\nSecond line.'
First line.
Second line.
```

The string holds a newline character. `repr()` shows it as `\n`, and `print()` starts a new line (rule r2).

```python run id=slices
word = "Python"
print(word[:2] + word[2:])
print(word[-2:], word[4:42], repr(word[42:]))
```

```output for=slices
Python
on on ''
```

A slice with an index past the end gives a shorter string or an empty string (rule r5).

```python run id=immutable
word = "Python"
new = "J" + word[1:]
print(new, word)
```

```output for=immutable
Jython Python
```

The expression makes a new string. The old string does not change (rule r6).

## Pitfalls

Assignment to a character is an error, because a string is immutable:

```python fails id=assign-char expect=exception:TypeError
word = "Python"
word[0] = "J"
```

A literal next to a variable is not joined. Use `+`:

```python fails id=literal-join expect=exception:SyntaxError
prefix = "Py"
print(prefix "thon")
```

An index past the end is an error:

```python fails id=index-error expect=exception:IndexError
word = "Python"
word[42]
```

## Comparison

In Go, "A string value is a (possibly empty) sequence of bytes."[^3] "Strings are immutable: once created, it is impossible to change the contents of a string."[^3] Both languages have immutable strings. An index on a Go string gives a byte, but an index on a Python string gives a string of one character (rule r4).
