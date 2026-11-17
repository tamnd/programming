---
tag: 7EQRPY
kind: feature
owner: python
slug: eafp-and-lbyl
name: EAFP and LBYL
aliases: [EAFP, LBYL, easier to ask for forgiveness than permission, look before you leap, check then act, TOCTOU]
area: languages/python/idioms-and-style
level: core
summary: >-
  EAFP code tries the operation and catches the exception when it fails. LBYL code tests the conditions first with if. A test can go stale before the operation runs, so EAFP is often safer.
grammar: null
spec: {standard: null, url: "https://docs.python.org/3.15/glossary.html#term-EAFP", locator: "Glossary, EAFP and LBYL"}
proposals: []
see_also: [python/pythonic-code, python/handling-exceptions, python/errors-and-exceptions, python/duck-typing, python/dictionaries, python/thread-synchronization, python/atomicity-and-the-memory-model]
sources:
  - url: https://docs.python.org/3.15/glossary.html#term-EAFP
    title: Glossary, Python 3.15 documentation
    locator: duck-typing; EAFP; LBYL
    accessed: 2026-11-18
  - url: https://docs.python.org/3.15/library/os.html#os.access
    title: os, Miscellaneous operating system interfaces, Python 3.15 documentation
    locator: os.access(), Note
    accessed: 2026-11-18
  - url: https://docs.python.org/3.15/builtins/functions.html#hasattr
    title: Built-in Functions, Python 3.15 documentation
    locator: hasattr()
    accessed: 2026-11-18
  - url: https://peps.python.org/pep-0008/#programming-recommendations
    title: PEP 8, Style Guide for Python Code
    locator: Programming Recommendations
    accessed: 2026-11-18
  - url: https://docs.python.org/3.15/tutorial/errors.html#handling-exceptions
    title: The Python Tutorial, Python 3.15 documentation
    locator: 8.3 Handling Exceptions
    accessed: 2026-11-18
  - url: https://docs.python.org/3.15/faq/design.html#how-fast-are-exceptions
    title: Design and History FAQ, Python 3.15 documentation
    locator: How fast are exceptions?
    accessed: 2026-11-18
  - url: https://docs.python.org/3.15/library/stdtypes.html#dict.get
    title: Built-in Types, Python 3.15 documentation
    locator: Mapping Types, dict.get(); dict.setdefault(); str.isdigit()
    accessed: 2026-11-18
  - url: https://docs.python.org/3.15/whatsnew/3.11.html
    title: What’s New In Python 3.11
    locator: Faster CPython, Misc
    accessed: 2026-11-18
---

## Synopsis

```python run id=both-styles
config = {"host": "localhost"}

if "port" in config:
    port = config["port"]
else:
    port = 8080

try:
    host = config["host"]
except KeyError:
    host = "0.0.0.0"

print(host, port)
```

```output for=both-styles
localhost 8080
```

The `if` test is LBYL. The `try` statement is EAFP.

## Semantics

**r1.** EAFP stands for "Easier to ask for forgiveness than permission. This common Python coding style assumes the existence of valid keys or attributes and catches exceptions if the assumption proves false."[^1] It "is characterized by the presence of many try and except statements."[^1]

**r2.** LBYL stands for "Look before you leap. This coding style explicitly tests for pre-conditions before making calls or lookups."[^1] It "is characterized by the presence of many if statements."[^1] The glossary calls it "common to many other languages such as C."[^1]

**r3.** The main risk of LBYL is a race. "In a multi-threaded environment, the LBYL approach can risk introducing a race condition between “the looking” and “the leaping”."[^1] For example, `if key in mapping: return mapping[key]` "can fail if another thread removes key from mapping after the test, but before the lookup."[^1] The fix is a lock or the EAFP style.[^1] Example: [`check-then-use`](#check-then-use).

**r4.** Files are the common case. The `os.access` documentation warns that a test before `open()` "creates a security hole, because the user might exploit the short time interval between checking and opening the file to manipulate it. It’s preferable to use EAFP techniques."[^2] Example: [`open-or-default`](#open-or-default).

**r5.** EAFP fits duck typing. Duck-typed code "typically employs hasattr() tests or EAFP programming."[^1] `hasattr` itself works by "calling getattr(object, name) and seeing whether it raises an AttributeError or not."[^3] Example: [`duck`](#duck).

**r6.** Keep the `try` clause small. PEP 8 says to "limit the try clause to the absolute minimum amount of code necessary. Again, this avoids masking bugs".[^4] Put the code that runs on success in the `else` clause: it "avoids accidentally catching an exception that wasn’t raised by the code being protected by the try … except statement."[^5] Catch a named exception, not all of them. Example: [`broad-try`](#broad-try).

**r7.** Cost: "A try/except block is extremely efficient if no exceptions are raised. Actually catching an exception is expensive."[^6] So the FAQ says that the `try` form of a dictionary lookup "only made sense when you expected the dict to have the key almost all the time."[^6]

**r8.** Some tools remove the choice. `dict.get` returns a default, "so that this method never raises a KeyError."[^7] `dict.setdefault` inserts the default when the key is missing.[^7] Example: [`no-test-needed`](#no-test-needed).

## Examples

```python run id=open-or-default
import os
import tempfile


def load(path):
    try:
        f = open(path, encoding="utf-8")
    except FileNotFoundError:
        return "default"
    with f:
        return f.read()


with tempfile.TemporaryDirectory() as folder:
    path = os.path.join(folder, "settings.txt")
    print(load(path))
    with open(path, "w", encoding="utf-8") as f:
        f.write("custom")
    print(load(path))
```

```output for=open-or-default
default
custom
```

There is no time between a test and the `open`, because there is no test (rule r4). The `try` clause holds only the `open` call, and the `with` block reads the file after it (rule r6).

```python run id=duck
import io


def read_all(source):
    try:
        read = source.read
    except AttributeError:
        return str(source)
    return read()


print(read_all(io.StringIO("from a file")), read_all(42))
```

```output for=duck
from a file 42
```

The function does not ask for a type. It tries the attribute `read`, and it falls back when the object has none (rule r5). The call `read()` is outside the `try`, so an `AttributeError` inside `read` is not hidden.

```python run id=no-test-needed
from collections import defaultdict

counts = {}
for word in "a b a c a".split():
    counts[word] = counts.get(word, 0) + 1
print(counts)

groups = defaultdict(list)
for word in ["apple", "avocado", "banana"]:
    groups[word[0]].append(word)
print(dict(groups))
```

```output for=no-test-needed
{'a': 3, 'b': 1, 'c': 1}
{'a': ['apple', 'avocado'], 'b': ['banana']}
```

`get` and `defaultdict` need neither an `if` nor a `try` (rule r8).

## Versions

> [!since v="3.11"]
> "“Zero-cost” exceptions are implemented, eliminating the cost of try statements when no exception is raised."[^8] So a `try` statement costs almost nothing when no exception is raised, and the EAFP style is cheap on the success path.

## Pitfalls

```python fails id=check-then-use expect=exception:KeyError
cache = {"k": 1}


def other_thread():
    cache.pop("k", None)


def lookup(key):
    if key in cache:
        other_thread()
        return cache[key]
    return None


print(lookup("k"))
```

The call `other_thread()` stands for another thread that runs between the test and the lookup. The test said yes, but the key is gone when the lookup runs (rule r3). With real threads, the failure is rare and hard to reproduce. Write `try: return cache[key]` with `except KeyError`, or use `cache.get(key)`.

```python wrong id=check-differs
for text in ["42", "-5", " 7 ", "²"]:
    print(repr(text), text.isdigit(), end=" ")
    try:
        print(int(text))
    except ValueError:
        print("ValueError")
```

```output for=check-differs
'42' True 42
'-5' False -5
' 7 ' False 7
'²' True ValueError
```

A test can disagree with the operation. `isdigit` is false for `-5` and for ` 7 `, but `int` accepts both. `isdigit` is true for the superscript two, because digits include "the compatibility superscript digits", but `int` rejects it.[^7] The only exact test for `int(text)` is to call it.

```python wrong id=broad-try
def handle(value):
    return value["name"]


def show(record, key):
    try:
        return handle(record[key])
    except KeyError:
        return "missing key"


print(show({"user": {"id": 1}}, "user"))
```

```output for=broad-try
missing key
```

The key `user` is there. The `KeyError` came from `handle`, but the handler reports a missing key. PEP 8 shows the fix: look up the key in the `try` clause, and call `handle` in the `else` clause (rule r6).

## Comparison

Both styles are correct when nothing changes between the test and the act, and when the test checks exactly what the act needs. LBYL reads better when failure is common, because catching an exception is slower than an `if` test (rule r7). EAFP is safer with files, threads and other shared state. For the `try` statement itself, see [[python/handling-exceptions]]. For locks, see [[python/thread-synchronization]].
