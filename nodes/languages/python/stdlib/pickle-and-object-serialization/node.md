---
tag: 23YK2S
kind: feature
owner: python
slug: pickle-and-object-serialization
name: pickle and object serialization
aliases: [pickle, unpickle, pickling, serialization, shelve, copyreg, __getstate__, __setstate__, __reduce__, pickle protocol]
area: languages/python/stdlib
level: core
summary: >-
  pickle turns a Python object structure into bytes and back. It keeps shared references and handles most classes, but loading pickle data can run any code. shelve stores pickled values by key.
grammar: null
spec: {standard: null, url: "https://docs.python.org/3.15/library/pickle.html", locator: "pickle, Python object serialization"}
proposals: []
see_also: [python/file-format-modules, python/data-compression, python/cryptographic-services, python/special-methods, python/the-sqlite3-module]
sources:
  - url: https://docs.python.org/3.15/library/pickle.html
    title: pickle, Python object serialization, Python 3.15 documentation
    locator: Introduction; Comparison with json; Data stream format; What can be pickled and unpickled?; Pickling Class Instances
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/library/shelve.html
    title: shelve, Python object persistence, Python 3.15 documentation
    locator: Introduction; shelve.open()
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/library/copyreg.html
    title: copyreg, Register pickle support functions, Python 3.15 documentation
    locator: Introduction
    accessed: 2026-11-16
---

## Synopsis

```python run id=round-trip
import pickle
from datetime import date

shared = [1, 2]
record = {"when": date(2026, 11, 16), "tags": {"a"}, "pair": (shared, shared)}
blob = pickle.dumps(record)
copy = pickle.loads(blob)
print(type(blob).__name__, copy == record, copy is record)
print(copy["pair"][0] is copy["pair"][1])
print(pickle.DEFAULT_PROTOCOL, blob[:2])
```

```output for=round-trip
bytes True False
True
5 b'\x80\x05'
```

## Semantics

**r1.** "The pickle module implements binary protocols for serializing and de-serializing a Python object structure."[^1] `dumps` returns bytes and `loads` builds a new object from them. `dump` and `load` do the same with a binary file. Example: [`round-trip`](#round-trip).

**r2.** "The pickle module is not secure. Only unpickle data you trust." "It is possible to construct malicious pickle data which will execute arbitrary code during unpickling."[^1] Example: [`code-on-load`](#code-on-load).

**r3.** pickle keeps the shape of the object graph. Two references to one object load as two references to one new object. Example: [`round-trip`](#round-trip).

**r4.** "functions (built-in and user-defined) are pickled by fully qualified name, not by value."[^1] "Similarly, classes are pickled by fully qualified name". "These restrictions are why picklable functions and classes must be defined at the top level of a module."[^1] Example: [`lambda`](#lambda).

**r5.** "when class instances are pickled, their class’s code and data are not pickled along with them. Only the instance data are pickled."[^1] A class controls its state with `__getstate__` and `__setstate__`. "Upon unpickling, if the class defines __setstate__(), it is called with the unpickled state."[^1] Example: [`custom-state`](#custom-state).

**r6.** A protocol is a version of the byte format. `dumps` uses `pickle.DEFAULT_PROTOCOL` unless you pass `protocol`, and `pickle.HIGHEST_PROTOCOL` is the newest one. For `loads`, "The protocol version of the pickle is detected automatically, so no protocol argument is needed."[^1]

**r7.** "A “shelf” is a persistent, dictionary-like object." Its values can be "anything that the pickle module can handle". "The keys are ordinary strings."[^2] Example: [`shelf-mutation`](#shelf-mutation).

**r8.** "The copyreg module offers a way to define functions used while pickling specific objects."[^3] It registers a reduction function for a class that you cannot change.

## Examples

```python run id=custom-state
import pickle
import threading

class Counter:
    def __init__(self):
        self.count = 0
        self.lock = threading.Lock()

    def __getstate__(self):
        state = self.__dict__.copy()
        del state["lock"]
        return state

    def __setstate__(self, state):
        self.__dict__.update(state)
        self.lock = threading.Lock()

c = Counter()
c.count = 5
d = pickle.loads(pickle.dumps(c))
print(d.count, type(d.lock).__name__, d.lock is c.lock)
```

```output for=custom-state
5 lock False
```

A lock cannot be pickled, so `__getstate__` leaves it out and `__setstate__` makes a new one (rule r5). The count comes back from the data.

## Versions

> [!since v="3.14"]
> Protocol 5 is the default protocol.[^1]

## Pitfalls

```python wrong id=code-on-load
import pickle

class Payload:
    def __reduce__(self):
        return (print, ("this code ran inside pickle.loads",))

data = pickle.dumps(Payload())
result = pickle.loads(data)
print(result)
```

```output for=code-on-load
this code ran inside pickle.loads
None
```

`__reduce__` tells pickle to call a function on load. This harmless example calls `print`. Data made by an attacker can call a harmful function in the same way (rule r2). "Never unpickle data that could have come from an untrusted source, or that could have been tampered with."[^1] To check that data has not changed, "Consider signing data with hmac".[^1]

```python fails id=lambda expect=exception:PicklingError
import pickle

square = lambda x: x * x
pickle.dumps(square)
```

pickle stores a function by its name, and "all lambda functions share the same name: <lambda>."[^1] Use a `def` at the top level of a module (rule r4).

```python wrong id=shelf-mutation
import os
import shelve
import tempfile

path = os.path.join(tempfile.mkdtemp(), "store")
with shelve.open(path) as db:
    db["scores"] = [1, 2]
    db["scores"].append(3)
with shelve.open(path) as db:
    print(db["scores"])
    scores = db["scores"]
    scores.append(3)
    db["scores"] = scores
with shelve.open(path) as db:
    print(db["scores"], list(db.keys()))
```

```output for=shelf-mutation
[1, 2]
[1, 2, 3] ['scores']
```

`db['scores']` loads a new copy of the list, so `append` changes only the copy. "a shelf cannot know when a mutable persistent-dictionary entry is modified."[^2] Assign the changed value back, or open the shelf with `writeback=True`. Also, "Because the shelve module is backed by pickle, it is insecure to load a shelf from an untrusted source."[^2]

## Comparison

The pickle docs compare it with JSON:[^1]

- "JSON is human-readable, while pickle is not".
- "JSON is interoperable and widely used outside of the Python ecosystem, while pickle is Python-specific".
- JSON "can only represent a subset of the Python built-in types, and no custom classes".
- "Unlike pickle, deserializing untrusted JSON does not in itself create an arbitrary code execution vulnerability."

Use JSON or another text format (see [[python/file-format-modules]]) to exchange data with other programs. Use pickle for Python objects that only your own program reads.
