---
tag: 8X6Y1E
kind: feature
owner: python
slug: memory-efficiency
name: Memory efficiency
aliases: [memory efficiency, reduce memory, __slots__, generator expression, array module, memoryview, sys.intern, sys.getsizeof, tracemalloc]
area: languages/python/performance
level: advanced
summary: >-
  Each Python object has a header, and a list holds pointers to objects. Save memory with __slots__, generators, array for numbers, memoryview slices and sys.intern for repeated strings.
grammar: null
spec: {standard: null, url: "https://docs.python.org/3/reference/datamodel.html#slots", locator: "Data model, 3.3.2.4. __slots__"}
see_also: [python/object-sizes-and-tracemalloc, python/slots, python/generators, python/memoryview-and-the-buffer-protocol, python/object-identity-and-interning, python/memory-management-in-cpython]
sources:
  - url: https://docs.python.org/3/reference/datamodel.html#slots
    title: Data model
    locator: 3.3.2.4. __slots__
    accessed: 2026-11-16
  - url: https://docs.python.org/3/howto/functional.html
    title: Functional Programming HOWTO
    locator: Generator expressions and list comprehensions
    accessed: 2026-11-16
  - url: https://docs.python.org/3/library/array.html
    title: array, Efficient arrays of numeric values
    locator: introduction; array.itemsize
    accessed: 2026-11-16
  - url: https://docs.python.org/3/library/stdtypes.html#memoryview
    title: Built-in Types
    locator: Memory Views
    accessed: 2026-11-16
  - url: https://docs.python.org/3/library/sys.html
    title: sys, System-specific parameters and functions
    locator: sys.getsizeof; sys.intern
    accessed: 2026-11-16
  - url: https://docs.python.org/3/library/tracemalloc.html
    title: tracemalloc, Trace memory allocations
    locator: introduction; get_traced_memory
    accessed: 2026-11-16
  - url: https://pkg.go.dev/unsafe
    title: Package unsafe
    locator: func Sizeof
    accessed: 2026-11-16
---

## Synopsis

```python run id=slots
import tracemalloc

class WithDict:
    def __init__(self, x, y):
        self.x, self.y = x, y

class WithSlots:
    __slots__ = ('x', 'y')

    def __init__(self, x, y):
        self.x, self.y = x, y

def traced(make):
    tracemalloc.start()
    obj = make()
    size, _ = tracemalloc.get_traced_memory()
    tracemalloc.stop()
    return size

with_dict = traced(lambda: [WithDict(i, i) for i in range(10_000)])
with_slots = traced(lambda: [WithSlots(i, i) for i in range(10_000)])
print('slots use less memory:', with_slots < with_dict)
print('no __dict__:', not hasattr(WithSlots(1, 2), '__dict__'))
```

```output for=slots
slots use less memory: True
no __dict__: True
```

In CPython, each object has a header, each instance of a normal class can have a `__dict__`, and a list or a dict holds pointers to separate objects. When a program keeps millions of small objects, these costs add up. The example measures 10,000 instances of two classes with `tracemalloc`. The class with `__slots__` has no `__dict__`, and its instances use less memory. First measure where the memory goes. Then use the tool that fits: `__slots__`, generators, `array`, `memoryview` or `sys.intern`.

## Semantics

**r1.** "__slots__ allow us to explicitly declare data members (like properties) and deny the creation of __dict__ and __weakref__ (unless explicitly declared in __slots__ or available in a parent.) The space saved over using __dict__ can be significant."[^1] Example: [`slots`](#slots).

**r2.** "Without a __dict__ variable, instances cannot be assigned new variables not listed in the __slots__ definition. Attempts to assign to an unlisted variable name raises AttributeError."[^1] "When inheriting from a class without __slots__, the __dict__ and __weakref__ attribute of the instances will always be accessible."[^1]

**r3.** "Generator expressions return an iterator that computes the values as necessary, not needing to materialize all the values at once."[^2] A generator keeps its state, not its results, so its size does not depend on the number of values. Example: [`lazy`](#lazy).

**r4.** The `array` module "defines an object type which can compactly represent an array of basic values: characters, integers, floating-point numbers, complex numbers."[^3] An array keeps the values in one block of memory, and `itemsize` is "The length in bytes of one array item in the internal representation."[^3] A list of floats keeps a pointer to a separate float object for each item. Example: [`numbers`](#numbers).

**r5.** "memoryview objects allow Python code to access the internal data of an object that supports the buffer protocol without copying."[^4] A slice of a `memoryview` is a view of the same memory. A slice of `bytes` or `bytearray` is a copy. Example: [`views`](#views).

**r6.** `sys.intern` enters a string "in the table of “interned” strings and return the interned string".[^5] Equal strings that a program reads or builds at run time are often separate objects. After `sys.intern`, the program keeps one object for each value. "Interned strings are not immortal; you must keep a reference to the return value of intern() around to benefit from it."[^5] Example: [`intern`](#intern).

**r7.** `sys.getsizeof` returns "the size of an object in bytes".[^5] "Only the memory consumption directly attributed to the object is accounted for, not the memory consumption of objects it refers to."[^5] To measure a structure with all of its parts, use `tracemalloc`, which traces "memory blocks allocated by Python".[^6]

## Examples

A list keeps all the values. A generator expression keeps only its state (rule r3). The sums are the same:

```python run id=lazy
import sys

squares_list = [i * i for i in range(1_000_000)]
squares_gen = (i * i for i in range(1_000_000))
print('list over 1 MB:', sys.getsizeof(squares_list) > 1_000_000)
print('generator under 1 KB:', sys.getsizeof(squares_gen) < 1_000)
print('same sum:', sum(squares_gen) == sum(squares_list))
```

```output for=lazy
list over 1 MB: True
generator under 1 KB: True
same sum: True
```

An `array` of type `'d'` keeps 8 bytes for each float. A list keeps a pointer and a separate float object for each item (rule r4):

```python run id=numbers
import tracemalloc
from array import array

def traced(make):
    tracemalloc.start()
    obj = make()
    size, _ = tracemalloc.get_traced_memory()
    tracemalloc.stop()
    return size

n = 100_000
as_list = traced(lambda: [float(i) for i in range(n)])
as_array = traced(lambda: array('d', (float(i) for i in range(n))))
print('itemsize:', array('d').itemsize)
print('array under half of the list:', as_array * 2 < as_list)
```

```output for=numbers
itemsize: 8
array under half of the list: True
```

A slice of a `memoryview` does not copy the data, and a write through the view changes the original object (rule r5):

```python run id=views
import tracemalloc

data = bytearray(b'abcdefghij' * 100_000)
view = memoryview(data)

def traced(make):
    tracemalloc.start()
    obj = make()
    size, _ = tracemalloc.get_traced_memory()
    tracemalloc.stop()
    return size

print('view slice under 1 KB:', traced(lambda: view[100:900_100]) < 1_000)
print('bytearray slice over 800 KB:', traced(lambda: data[100:900_100]) > 800_000)
view[100:103] = b'XYZ'
print(data[100:103])
```

```output for=views
view slice under 1 KB: True
bytearray slice over 800 KB: True
bytearray(b'XYZ')
```

Strings from `split` are separate objects, even when they are equal. `sys.intern` keeps one object for each value (rule r6):

```python run id=intern
import sys

words = ('red green blue ' * 1000).split()
print(len(words), len({id(w) for w in words}))
words = [sys.intern(w) for w in words]
print(len(words), len({id(w) for w in words}))
```

```output for=intern
3000 3000
3000 3
```

## Pitfalls

Measure before you change the code. `tracemalloc` shows which lines allocate the most memory, and the fix for one line can be different from the fix for another.

`sys.getsizeof` does not count the objects that a container refers to (rule r7). The size of a list does not include the size of its items.

A class with `__slots__` cannot get new attributes at run time, and its instances do not support weak references unless `'__weakref__'` is in `__slots__`.[^1] If a base class has no `__slots__`, the instances still have a `__dict__` (rule r2).

A generator gives its values once. A second loop over the same generator gets no values. Use a list when the program needs the values more than once.

A view can put limits on the original object. The documentation says: "Many objects take special actions when a view is held on them (for example, a bytearray would temporarily forbid resizing); therefore, calling release() is handy to remove these restrictions (and free any dangling resources) as soon as possible."[^4]

## Comparison

Go keeps the fields of a struct in one block of memory, with no dictionary for each value. `unsafe.Sizeof` "takes an expression x of any type and returns the size in bytes of a hypothetical variable v as if v was declared via var v = x. The size does not include any memory possibly referenced by x."[^7] A Go `[]float64` keeps the floats in one array, like `array('d')` in Python (rule r4). A Python list keeps pointers to objects, and a Python instance can have a `__dict__` unless the class uses `__slots__` (rule r1).

## References

[^1]: [Data model](https://docs.python.org/3/reference/datamodel.html#slots)
[^2]: [Functional Programming HOWTO](https://docs.python.org/3/howto/functional.html)
[^3]: [array, Efficient arrays of numeric values](https://docs.python.org/3/library/array.html)
[^4]: [Built-in Types](https://docs.python.org/3/library/stdtypes.html#memoryview)
[^5]: [sys, System-specific parameters and functions](https://docs.python.org/3/library/sys.html)
[^6]: [tracemalloc, Trace memory allocations](https://docs.python.org/3/library/tracemalloc.html)
[^7]: [Package unsafe](https://pkg.go.dev/unsafe)
