---
tag: WKVJSP
kind: feature
owner: python
slug: the-standard-library-tour
name: The standard library tour
aliases: [standard library tour, batteries included, Python stdlib, brief tour of the standard library, tutorial chapter 10, tutorial chapter 11]
area: languages/python/stdlib
level: intro
summary: >-
  Python ships with a large standard library, the "batteries included" idea. The two tour chapters of the tutorial show common modules: os, re, math, datetime, zlib, doctest and logging.
grammar: null
spec: {standard: null, url: "https://docs.python.org/3.15/tutorial/stdlib.html", locator: "10. Brief tour of the standard library"}
proposals: []
see_also: [python/standard-library-map, python/the-python-package-index, python/operating-system-interface, python/mathematics, python/data-compression, python/multi-threading, python/decimal-floating-point-arithmetic]
sources:
  - url: https://docs.python.org/3.15/library/index.html
    title: The Python standard library, Python 3.15 documentation
    locator: Introduction paragraphs
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/tutorial/stdlib.html
    title: 10. Brief tour of the standard library, Python 3.15 documentation
    locator: 10.1 to 10.12
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/tutorial/stdlib2.html
    title: 11. Brief tour of the standard library, part II, Python 3.15 documentation
    locator: 11.1 to 11.8
    accessed: 2026-11-16
---

## Synopsis

```python run id=tour
import math
import re
import statistics
import zlib

print(re.findall(r"\bf[a-z]*", "which foot or hand fell fastest"))
print(math.log(1024, 2))
print(statistics.median([2.75, 1.75, 1.25, 0.25, 0.5, 1.25, 3.5]))
s = b"witch which has which witches wrist watch"
print(zlib.decompress(zlib.compress(s)) == s, zlib.crc32(s))
```

```output for=tour
['foot', 'fell', 'fastest']
10.0
1.25
True 226805979
```

## Semantics

**r1.** "Python’s standard library is extensive, offering a wide range of facilities".[^1] It "contains built-in modules (written in C) that provide access to system functionality such as file I/O", "as well as modules written in Python that provide standardized solutions for many problems that occur in everyday programming."[^1]

**r2.** "Python has a “batteries included” philosophy. This is best seen through the sophisticated and robust capabilities of its larger packages."[^2] A program imports a standard module with `import`, with no install step. Example: [`tour`](#tour).

**r3.** The first tour covers the operating system, file wildcards, command-line arguments, error output, string pattern matching, mathematics, internet access, dates and times, data compression, performance measurement and quality control.[^2] Example: [`argparse`](#argparse).

**r4.** "This second tour covers more advanced modules that support professional programming needs. These modules rarely occur in small scripts."[^3] It covers output formatting, templating, binary record layouts, threads, logging, weak references, list tools and decimal arithmetic.[^3] Example: [`struct`](#struct).

**r5.** "In addition to the standard library, there is an active collection of hundreds of thousands of components" on the Python Package Index.[^1]

## Examples

```python run id=argparse
import argparse

parser = argparse.ArgumentParser(prog="top", description="Show top lines from each file")
parser.add_argument("filenames", nargs="+")
parser.add_argument("-l", "--lines", type=int, default=10)
args = parser.parse_args(["--lines=5", "alpha.txt", "beta.txt"])
print(args.lines, args.filenames)
```

```output for=argparse
5 ['alpha.txt', 'beta.txt']
```

"The argparse module provides a more sophisticated mechanism to process command-line arguments."[^2] Here the list replaces `sys.argv`, so the example runs with no real arguments.

```python run id=dates
import datetime as dt

day = dt.date(2003, 12, 2)
print(day.strftime("%d %b %Y is a %A"))
print((day - dt.date(1964, 7, 31)).days)
```

```output for=dates
02 Dec 2003 is a Tuesday
14368
```

"The datetime module supplies classes for manipulating dates and times in both simple and complex ways."[^2] Subtraction of two dates gives a `timedelta`.

```python run id=doctest
import doctest

def average(values):
    """Computes the arithmetic mean of a list of numbers.

    >>> print(average([20, 30, 70]))
    40.0
    """
    return sum(values) / len(values)

print(doctest.testmod())
```

```output for=doctest
TestResults(failed=0, attempted=1)
```

"The doctest module provides a tool for scanning a module and validating tests embedded in a program’s docstrings."[^2]

```python run id=struct
import struct

record = struct.pack("<HI", 7, 1024)
print(len(record), record.hex())
print(struct.unpack("<HI", record))
```

```output for=struct
6 070000040000
(7, 1024)
```

"The struct module provides pack() and unpack() functions for working with variable length binary record formats."[^3] `<` means little-endian with standard sizes, `H` is two bytes and `I` is four bytes.[^3]

## Pitfalls

```python fails id=star-import expect=exception:TypeError
from os import *

with open("notes.txt") as f:
    print(f.read())
```

The tour says: "Be sure to use the import os style instead of from os import *. This will keep os.open() from shadowing the built-in open() function which operates much differently."[^2] Here `open` is `os.open`, which needs a flags argument.

## Comparison

Each tour section has its own page in this guide, for example [[python/operating-system-interface]], [[python/file-wildcards]], [[python/string-pattern-matching]], [[python/mathematics]], [[python/data-compression]], [[python/output-formatting]], [[python/templating]], [[python/multi-threading]], [[python/tools-for-working-with-lists]] and [[python/decimal-floating-point-arithmetic]]. See [[python/standard-library-map]] for all the chapters of the library reference.
