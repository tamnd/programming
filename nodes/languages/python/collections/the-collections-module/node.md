---
tag: ZJFBZ6
kind: feature
owner: python
slug: the-collections-module
name: The collections module
aliases: [collections, namedtuple, deque, Counter, defaultdict, OrderedDict, ChainMap, UserDict, multiset, bag]
area: languages/python/collections
level: core
summary: >-
  The collections module adds container types to the built-in ones: namedtuple, deque, ChainMap, Counter, OrderedDict, defaultdict, and the UserDict, UserList and UserString wrappers.
grammar: null
spec: {standard: null, url: "https://docs.python.org/3.15/library/collections.html", locator: "collections: Container datatypes"}
see_also: [python/dictionaries, python/lists-as-queues, python/tuples-and-sequences, python/collections-abc, python/dict-order-and-views, python/emulating-container-types]
sources:
  - url: https://docs.python.org/3.15/library/collections.html
    title: "collections: Container datatypes, Python 3.15"
    locator: Introduction; ChainMap; Counter; defaultdict; namedtuple(); OrderedDict
    accessed: 2026-11-16
  - url: https://go.dev/ref/spec#Struct_types
    title: The Go Programming Language Specification
    locator: Struct types
    accessed: 2026-11-16
---

## Synopsis

```python run id=tour
from collections import Counter, defaultdict, namedtuple, deque

words = "red blue red green blue blue".split()
print(Counter(words).most_common(2))

by_len = defaultdict(list)
for w in words:
    by_len[len(w)].append(w)
print(dict(by_len))

Point = namedtuple("Point", ["x", "y"])
p = Point(11, y=22)
print(p.x + p.y, p)

d = deque([1, 2, 3], maxlen=3)
d.append(4)
print(d)
```

```output for=tour
[('blue', 3), ('red', 2)]
{3: ['red', 'red'], 4: ['blue', 'blue', 'blue'], 5: ['green']}
33 Point(x=11, y=22)
deque([2, 3, 4], maxlen=3)
```

Each type solves one common task with less code than a plain dict, list or tuple.

## Semantics

**r1.** "This module implements specialized container datatypes providing alternatives to Python’s general purpose built-in containers, dict, list, set, and tuple."[^1]

**r2.** "A Counter is a dict subclass for counting hashable objects. It is a collection where elements are stored as dictionary keys and their counts are stored as dictionary values."[^1] "Counter objects have a dictionary interface except that they return a zero count for missing items instead of raising a KeyError".[^1] Example: [`counter`](#counter).

**r3.** For a defaultdict, when a key is missing and default_factory is not None, "it is called without arguments to provide a default value for the given key, this value is inserted in the dictionary for the key, and returned."[^1] "Note that __missing__() is not called for any operations besides __getitem__()."[^1] Example: [`lookup-inserts`](#lookup-inserts).

**r4.** namedtuple() "Returns a new tuple subclass named typename. The new subclass is used to create tuple-like objects that have fields accessible by attribute lookup as well as being indexable and iterable."[^1] Example: [`namedtuple`](#namedtuple).

**r5.** deque is a "list-like container with fast appends and pops on either end".[^1]

**r6.** "A ChainMap groups multiple dicts or other mappings together to create a single, updateable view."[^1] Example: [`chainmap`](#chainmap).

**r7.** OrderedDict is a "dict subclass that has methods specialized for rearranging dictionary order."[^1] "Equality tests between OrderedDict objects are order-sensitive and are roughly equivalent to list(od1.items())==list(od2.items())."[^1] Example: [`ordered`](#ordered).

**r8.** UserDict, UserList and UserString are wrappers "for easier dict subclassing", list subclassing and string subclassing.[^1]

## Examples

```python run id=counter
from collections import Counter

c = Counter("abracadabra")
print(c["a"], c["z"], c.most_common(3))
c.update("aaz")
print(c["a"], c["z"])
print(Counter(a=3, b=1) - Counter(a=1, b=2))
```

```output for=counter
5 0 [('a', 5), ('b', 2), ('r', 2)]
7 1
Counter({'a': 2})
```

A missing key has the count 0 (rule r2). Subtraction keeps only positive counts.

```python run id=namedtuple
from collections import namedtuple

Point = namedtuple("Point", ["x", "y"])
p = Point(3, 4)
x, y = p
print(p[0], x, p.y)
print(p._replace(x=10), p._asdict())
print(isinstance(p, tuple))
```

```output for=namedtuple
3 3 4
Point(x=10, y=4) {'x': 3, 'y': 4}
True
```

A named tuple is still a tuple, so indexing and unpacking work (rule r4).

```python run id=chainmap
from collections import ChainMap

defaults = {"color": "red", "user": "guest"}
args = {"user": "ann"}
settings = ChainMap(args, defaults)
print(settings["user"], settings["color"])
settings["color"] = "blue"
print(args, defaults)
```

```output for=chainmap
ann red
{'user': 'ann', 'color': 'blue'} {'color': 'red', 'user': 'guest'}
```

A lookup searches the mappings in order. A write goes to the first mapping only.

```python run id=ordered
from collections import OrderedDict

od = OrderedDict.fromkeys("abcde")
od.move_to_end("b")
print("".join(od))
od.move_to_end("b", last=False)
print("".join(od))
print(OrderedDict(a=1, b=2) == OrderedDict(b=2, a=1), dict(a=1, b=2) == dict(b=2, a=1))
```

```output for=ordered
acdeb
bacde
False True
```

## Pitfalls

Reading a missing key from a defaultdict adds the key:

```python wrong id=lookup-inserts
from collections import defaultdict

stock = defaultdict(int)
if stock["apples"] == 0:
    print("none left")
print(dict(stock))
```

```output for=lookup-inserts
none left
{'apples': 0}
```

Use `"apples" in stock` or `stock.get("apples", 0)` to read without a change (rule r3).

The fields of a named tuple cannot be assigned:

```python fails id=frozen-fields expect=exception:AttributeError
from collections import namedtuple
Point = namedtuple("Point", ["x", "y"])
p = Point(1, 2)
p.x = 5
```

Use `p._replace(x=5)` to get a new tuple.

## Comparison

A Python named tuple is close to a Go struct: "A struct is a sequence of named elements, called fields, each of which has a name and a type."[^2] A Go struct field can be assigned. A named tuple field cannot, and a named tuple can also be indexed and unpacked like a tuple (rule r4).
