---
tag: NT7GPQ
kind: feature
owner: python
slug: manual-string-formatting
name: Manual String Formatting
aliases: [rjust, ljust, center, zfill, expandtabs, pad a string, align columns, text table]
area: languages/python/strings-and-text
level: intro
summary: >-
  The str methods rjust, ljust and center pad a string to a width, and zfill pads a number string with zeros. A program can build aligned columns with these methods and no format spec.
grammar: null
spec: {standard: null, url: "https://docs.python.org/3.15/tutorial/inputoutput.html#manual-string-formatting", locator: "7.1.3. Manual String Formatting"}
see_also: [python/string-methods, python/format-specification-mini-language, python/formatted-string-literals]
sources:
  - url: https://docs.python.org/3.15/tutorial/inputoutput.html#manual-string-formatting
    title: The Python Tutorial, 7. Input and Output
    locator: 7.1.3. Manual String Formatting
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/builtins/stdtypes.html#str.rjust
    title: Built-in Types, Python 3.15 documentation
    locator: str.ljust(); str.rjust(); str.center(); str.zfill(); str.expandtabs()
    accessed: 2026-11-16
  - url: https://pkg.go.dev/fmt
    title: Package fmt, Go standard library
    locator: Printing, other flags
    accessed: 2026-11-16
---

## Synopsis

```python run id=table
for x in range(1, 11, 3):
    print(repr(x).rjust(2), repr(x * x).rjust(3), repr(x * x * x).rjust(4))
```

```output for=table
 1   1    1
 4  16   64
 7  49  343
10 100 1000
```

Each number is turned into a string and padded on the left to its column width. print() adds one space between the columns.

## Semantics

**r1.** "The str.rjust() method of string objects right-justifies a string in a field of a given width by padding it with spaces on the left. There are similar methods str.ljust() and str.center()."[^1]

**r2.** "These methods do not write anything, they just return a new string."[^1] The fill character is a space by default, and a second argument can give another one.[^2]

**r3.** "If the input string is too long, they don’t truncate it, but return it unchanged; this will mess up your column lay-out but that’s usually better than the alternative, which would be lying about a value."[^1] "If you really want truncation you can always add a slice operation, as in x.ljust(n)[:n]."[^1] Example: [`too-long`](#too-long).

**r4.** `zfill(width)` returns "a copy of the string left filled with ASCII '0' digits to make a string of length width. A leading sign prefix ('+'/'-') is handled by inserting the padding after the sign character rather than before."[^2] Example: [`zfill`](#zfill).

**r5.** `expandtabs(tabsize)` replaces each tab with spaces up to the next tab position. "Tab positions occur every tabsize characters (default is 8, giving tab positions at columns 0, 8, 16 and so on)."[^2]

## Examples

```python run id=fill
print("[" + "Python".ljust(10, ".") + "]")
print("[" + "Python".rjust(10, ".") + "]")
print("[" + "Python".center(10, "-") + "]")
print("[" + "Python".center(4) + "]")
```

```output for=fill
[Python....]
[....Python]
[--Python--]
[Python]
```

The width is less than the length in the last line, so center() returns the string unchanged (rule r3).

```python run id=zfill
print("12".zfill(5), "-3.14".zfill(7), "+7".zfill(4), "3.14159".zfill(3))
```

```output for=zfill
00012 -003.14 +007 3.14159
```

The zeros go after the sign (rule r4).

```python run id=report
rows = [("apples", 3, 1.5), ("kiwis", 12, 0.25), ("figs", 100, 12.0)]
header = "item".ljust(8) + "qty".rjust(5) + "price".rjust(8)
print(header)
print("-" * len(header))
for name, qty, price in rows:
    print(name.ljust(8) + str(qty).rjust(5) + ("%.2f" % price).rjust(8))
```

```output for=report
item      qty   price
---------------------
apples      3    1.50
kiwis      12    0.25
figs      100   12.00
```

Text columns go to the left and number columns go to the right.

```python run id=tabs
line = "a\tbb\tccc"
print(line.expandtabs(4).replace(" ", "."))
print(line.expandtabs().replace(" ", "."))
```

```output for=tabs
a...bb..ccc
a.......bb......ccc
```

Each tab moves to the next multiple of the tab size (rule r5).

## Pitfalls

A long value breaks the column, because the methods do not cut it:

```python wrong id=too-long
for name in ["Ada", "Bartholomew"]:
    print(name.ljust(6) + "|")
```

```output for=too-long
Ada   |
Bartholomew|
```

Add a slice, as in `name.ljust(6)[:6]`, if the column must keep its width (rule r3). The cut hides part of the value.

## Comparison

Go pads with the width in a format verb, not with string methods. The flag `'-'` means "pad with spaces on the right rather than the left (left-justify the field)".[^3] So Go `fmt.Sprintf("%-8s", name)` is like Python `name.ljust(8)`, and `%8s` is like `rjust(8)`.
