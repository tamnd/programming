---
tag: 4P345W
kind: feature
owner: python
slug: pythonic-code
name: Pythonic code
aliases: [Pythonic, idiomatic Python, Python idioms, The Zen of Python, import this, PEP 20]
area: languages/python/idioms-and-style
level: core
summary: >-
  Pythonic code uses the common idioms of Python: loop over the items, unpack tuples, use with for resources, build lists with comprehensions, test truth directly and use the standard library first.
grammar: null
spec: {standard: null, url: "https://docs.python.org/3.15/glossary.html#term-Pythonic", locator: "Glossary, Pythonic"}
proposals: [pep-20, pep-8]
see_also: [python/coding-style, python/eafp-and-lbyl, python/looping-techniques, python/list-comprehensions, python/iterable-unpacking, python/truth-value-testing, python/the-with-statement, python/the-standard-library-tour]
sources:
  - url: https://docs.python.org/3.15/glossary.html#term-Pythonic
    title: Glossary, Python 3.15 documentation
    locator: Pythonic
    accessed: 2026-11-18
  - url: https://peps.python.org/pep-0020/
    title: PEP 20, The Zen of Python
    locator: The Zen of Python; Easter Egg
    accessed: 2026-11-18
  - url: https://docs.python.org/3.15/tutorial/datastructures.html#looping-techniques
    title: The Python Tutorial, Python 3.15 documentation
    locator: 5.1.3 List Comprehensions; 5.6 Looping Techniques
    accessed: 2026-11-18
  - url: https://docs.python.org/3.15/tutorial/introduction.html
    title: The Python Tutorial, Python 3.15 documentation
    locator: 3.2 First Steps Towards Programming
    accessed: 2026-11-18
  - url: https://peps.python.org/pep-0008/#programming-recommendations
    title: PEP 8, Style Guide for Python Code
    locator: Programming Recommendations
    accessed: 2026-11-18
  - url: https://docs.python.org/3.15/library/stdtypes.html#truth-value-testing
    title: Built-in Types, Python 3.15 documentation
    locator: Truth Value Testing
    accessed: 2026-11-18
  - url: https://docs.python.org/3.15/tutorial/inputoutput.html#reading-and-writing-files
    title: The Python Tutorial, Python 3.15 documentation
    locator: 7.2 Reading and Writing Files
    accessed: 2026-11-18
  - url: https://docs.python.org/3.15/tutorial/stdlib.html#batteries-included
    title: The Python Tutorial, Python 3.15 documentation
    locator: 10.12 Batteries included
    accessed: 2026-11-18
  - url: https://docs.astral.sh/ruff/rules/in-dict-keys/
    title: in-dict-keys (SIM118), Ruff documentation
    locator: Why is this bad?
    accessed: 2026-11-18
  - url: https://docs.python.org/3.15/builtins/functions.html#zip
    title: Built-in Functions, Python 3.15 documentation
    locator: enumerate(); zip()
    accessed: 2026-11-18
  - url: https://docs.astral.sh/ruff/rules/none-comparison/
    title: none-comparison (E711), Ruff documentation
    locator: What it does; Why is this bad?
    accessed: 2026-11-18
---

## Synopsis

```python run id=idioms
food = ["rice", "pho", "banh mi"]
prices = {"rice": 2, "pho": 5, "banh mi": 3}

for number, piece in enumerate(food, start=1):
    print(number, piece, prices[piece])

cheap = [piece for piece in food if prices[piece] < 4]
print(cheap, "pho" in prices, ", ".join(food))
```

```output for=idioms
1 rice 2
2 pho 5
3 banh mi 3
['rice', 'banh mi'] True rice, pho, banh mi
```

## Semantics

**r1.** Pythonic means "An idea or piece of code which closely follows the most common idioms of the Python language, rather than implementing code using concepts common to other languages."[^1] The glossary gives the loop as the example: "a common idiom in Python is to loop over all elements of an iterable using a for statement."[^1] Example: [`idioms`](#idioms).

**r2.** The Zen of Python in PEP 20 states the values behind the idioms in 19 short lines, such as "Beautiful is better than ugly. Explicit is better than implicit. Simple is better than complex."[^2] The statement `import this` prints them.[^2] Example: [`zen`](#zen).

**r3.** Loop over the items, not over the indexes. When the index is also needed, use `enumerate`: "the position index and corresponding value can be retrieved at the same time using the enumerate() function."[^3] For a dictionary, "the key and corresponding value can be retrieved at the same time using the items() method."[^3] For two sequences, "the entries can be paired with the zip() function."[^3]

**r4.** Unpack instead of indexing. An assignment such as `a, b = b, a + b` gives new values to two names at once, and "the expressions on the right-hand side are all evaluated first before any of the assignments take place."[^4] Example: [`unpack`](#unpack).

**r5.** Build a list with a comprehension. "List comprehensions provide a concise way to create lists."[^3] A comprehension also does not leave its loop variable behind, as a `for` loop does.[^3] Join strings with `str.join`, because PEP 8 says that "the ''.join() form should be used" in code that must be fast on all implementations.[^5]

**r6.** Test truth directly. "By default, an object is considered true unless its class defines either a __bool__() method that returns False or a __len__() method that returns zero".[^6] So PEP 8 says: "For sequences, (strings, lists, tuples), use the fact that empty sequences are false".[^5] But "Comparisons to singletons like None should always be done with is or is not, never the equality operators."[^5] Example: [`truthiness`](#truthiness).

**r7.** Use `with` for a resource. "It is good practice to use the with keyword when dealing with file objects. The advantage is that the file is properly closed after its suite finishes, even if an exception is raised at some point."[^7] Example: [`with-files`](#with-files).

**r8.** Use the standard library first. "Python has a “batteries included” philosophy."[^8] A built-in such as `sum`, `max`, `any` or `sorted`, or a module such as `collections`, does the common jobs. Example: [`stdlib-first`](#stdlib-first).

**r9.** PEP 8 lists more idioms: use `startswith()` and `endswith()` instead of slices, use `isinstance()` instead of a type comparison, and "Always use a def statement instead of an assignment statement that binds a lambda expression directly to an identifier".[^5] To test for a key, use `key in d`, not `key in d.keys()`.[^9] Example: [`lambda-name`](#lambda-name).

## Examples

```python run id=zen
import contextlib
import io

buffer = io.StringIO()
with contextlib.redirect_stdout(buffer):
    import this

lines = buffer.getvalue().splitlines()
print(lines[0])
print(len(lines) - 2, "aphorisms")
print(lines[8])
```

```output for=zen
The Zen of Python, by Tim Peters
19 aphorisms
Readability counts.
```

Importing the module `this` prints the text (rule r2). The first two lines are the title and a blank line.

```python run id=unpack
a, b = 0, 1
a, b = b, a + b
print(a, b)

first, *rest = [3, 1, 4, 1, 5]
print(first, rest)

scores = {"ann": 31, "bob": 27}
for name, score in scores.items():
    print(f"{name}: {score}")
```

```output for=unpack
1 1
3 [1, 4, 1, 5]
ann: 31
bob: 27
```

The swap needs no temporary name, the star name takes the rest of the list, and the loop unpacks each pair from `items()` (rules r3 and r4).

```python run id=truthiness
def describe(items=None):
    if items is None:
        return "no argument"
    if not items:
        return "empty"
    return f"{len(items)} items"


print(describe(), describe([]), describe([1, 2]))
print([bool(x) for x in (0, 0.0, "", [], {}, None, "0", [0])])
```

```output for=truthiness
no argument empty 2 items
[False, False, False, False, False, False, True, True]
```

`is None` tests for the missing argument, and `not items` tests for an empty list (rule r6). The string `'0'` and the list `[0]` are true, because they are not empty.

```python run id=with-files
import tempfile
from pathlib import Path

with tempfile.TemporaryDirectory() as folder:
    path = Path(folder) / "notes.txt"
    with open(path, "w", encoding="utf-8") as f:
        f.write("one\ntwo\n")
    with open(path, encoding="utf-8") as f:
        lines = [line.rstrip("\n") for line in f]
    print(lines, f.closed)
```

```output for=with-files
['one', 'two'] True
```

Each `with` block closes its file at the end, and the outer one removes the folder (rule r7). The file object is an iterator over its lines.

```python run id=stdlib-first
from collections import Counter

words = "the cat and the hat and the bat".split()
print(Counter(words).most_common(2))
print(sum(len(w) for w in words), max(words, key=len))
print(any(w.startswith("h") for w in words))
```

```output for=stdlib-first
[('the', 3), ('and', 2)]
24 the
True
```

`Counter` counts the words, and the built-ins take a generator expression, so no list is built (rule r8). `max` returns the first of the longest words.

```python run id=lambda-name
double = lambda x: 2 * x


def triple(x):
    return 3 * x


print(double.__name__, triple.__name__)
```

```output for=lambda-name
<lambda> triple
```

The `def` form gives the function its name, which is "more useful for tracebacks and string representations in general."[^5]

## Versions

> [!since v="3.10"]
> `zip()` has the `strict` argument. With `strict=True`, it "raises a ValueError if one iterable is exhausted before the others".[^10]

## Pitfalls

```python wrong id=truthy-default
def set_timeout(seconds=None):
    if not seconds:
        seconds = 30
    return seconds


print(set_timeout(), set_timeout(0), set_timeout(5))
```

```output for=truthy-default
30 30 5
```

The caller asked for 0 seconds and got 30, because 0 is false. PEP 8 warns: "beware of writing if x when you really mean if x is not None".[^5] Write `if seconds is None:`.

```python wrong id=eq-none
class Anything:
    def __eq__(self, other):
        return True


x = Anything()
print(x == None, x is None)
```

```output for=eq-none
True False
```

A class can define `==` in any way, so `x == None` can be true for an object that is not `None`. The `is` test cannot be changed by a class. Ruff checks this with the rule E711.[^11]

```python wrong id=zip-truncates
names = ["ann", "bob", "cy"]
ages = [31, 27]
print(list(zip(names, ages)))
try:
    list(zip(names, ages, strict=True))
except ValueError as error:
    print(type(error).__name__)
```

```output for=zip-truncates
[('ann', 31), ('bob', 27)]
ValueError
```

By default, `zip` "stops when the shortest iterable is exhausted", so `cy` is lost with no error.[^10] When the lengths must be equal, pass `strict=True`.

## Comparison

An idiom of one language can be a bad habit in another. The glossary notes that people new to Python "sometimes use a numerical counter instead" of a direct loop, because "Many other languages don’t have this type of construct".[^1] For the error-handling idiom of Python, see [[python/eafp-and-lbyl]]. For the details of each idiom, see [[python/looping-techniques]], [[python/iterable-unpacking]], [[python/list-comprehensions]], [[python/truth-value-testing]] and [[python/the-with-statement]].
