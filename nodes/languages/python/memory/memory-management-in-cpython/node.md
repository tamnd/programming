---
tag: 699GH8
kind: feature
owner: python
slug: memory-management-in-cpython
name: Memory management in CPython
aliases: [python memory model, private heap, pymalloc, arenas, PYTHONMALLOC, how python frees memory]
area: languages/python/memory
level: core
summary: >-
  Python code never frees memory. An unreachable object may be collected. CPython frees most objects at once by reference counting, finds cycles with a collector, and uses a private heap.
grammar: null
spec: {standard: null, url: "https://docs.python.org/3.15/reference/datamodel.html#objects-values-and-types", locator: "3.1. Objects, values and types"}
see_also: [python/reference-counting-in-cpython, python/the-cyclic-garbage-collector, python/finalizers-and-del, python/object-sizes-and-tracemalloc]
sources:
- url: https://docs.python.org/3.15/reference/datamodel.html#objects-values-and-types
  title: The Python Language Reference, 3. Data model
  locator: 3.1. Objects, values and types
  accessed: 2026-11-16
- url: https://docs.python.org/3.15/reference/executionmodel.html#naming-and-binding
  title: The Python Language Reference, 4. Execution model
  locator: 4.2.1. Binding of names
  accessed: 2026-11-16
- url: https://docs.python.org/3.15/reference/simple_stmts.html#the-del-statement
  title: The Python Language Reference, 7. Simple statements
  locator: 7.5. The del statement
  accessed: 2026-11-16
- url: https://docs.python.org/3.15/library/gc.html
  title: gc, Garbage Collector interface
  locator: the introduction
  accessed: 2026-11-16
- url: https://docs.python.org/3.15/c-api/memory.html
  title: Memory Management, The Python/C API Reference Manual
  locator: Overview; Default Memory Allocators; The pymalloc allocator
  accessed: 2026-11-16
- url: https://docs.python.org/3.15/faq/design.html#how-does-python-manage-memory
  title: Design and History FAQ
  locator: How does Python manage memory?; Why isn't all memory freed when CPython exits?
  accessed: 2026-11-16
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/lang/System.html#gc()
  title: Class System, Java SE 25 API
  locator: gc()
  accessed: 2026-11-16
- url: https://go.dev/doc/gc-guide
  title: A Guide to the Go Garbage Collector
  locator: Introduction; Tracing Garbage Collection
  accessed: 2026-11-16
- url: https://github.com/swiftlang/swift-book/blob/main/TSPL.docc/LanguageGuide/AutomaticReferenceCounting.md
  title: Automatic Reference Counting, The Swift Programming Language
  locator: the introduction; Strong Reference Cycles Between Class Instances
  accessed: 2026-11-16
- url: https://doc.pypy.org/en/latest/cpython_differences.html
  title: Differences between PyPy and CPython
  locator: Differences related to garbage collection strategies
  accessed: 2026-11-16
- url: https://doc.pypy.org/en/latest/gc_info.html
  title: PyPy, Garbage collector documentation and configuration
  locator: Incminimark
  accessed: 2026-11-16
- url: https://docs.micropython.org/en/latest/library/gc.html
  title: gc, control the garbage collector, MicroPython documentation
  locator: Functions
  accessed: 2026-11-16
---

## Synopsis

Python has no statement that frees memory. A program makes objects, binds names to them, and drops the names. The table shows what the language promises and what CPython does on top of it.

| Layer | What it says | Page |
|---|---|---|
| The language | an unreachable object may be collected, at any time or never | this page, rule r1 |
| Names | a name refers to an object; `del` and a new assignment remove a reference | rules r2 and r3 |
| CPython, first stage | reference counting frees an object when its last reference goes | [Reference counting in CPython](/python/reference-counting-in-cpython/) |
| CPython, second stage | the `gc` module finds and frees reference cycles | [The cyclic garbage collector](/python/the-cyclic-garbage-collector/) |
| CPython, allocator | a private heap; pymalloc for objects of up to 512 bytes | rules r5 and r6 |
| Tools | `sys.getsizeof`, `tracemalloc`, `PYTHONMALLOC` | [Object sizes and tracemalloc](/python/object-sizes-and-tracemalloc/) |

```python run id=names
a = [1, 2, 3]
b = a
b.append(4)
print(a)
print(a is b)
del a
print(b)
```

```output for=names
[1, 2, 3, 4]
True
[1, 2, 3, 4]
```

`b = a` copies no list. Both names refer to one object. `del a` removes the name `a`, and the list stays alive because `b` still refers to it.

## Semantics

**r1.** The language reference gives the only rule: "Objects are never explicitly destroyed; however, when they become unreachable they may be garbage-collected. An implementation is allowed to postpone garbage collection or omit it altogether".[^1] So a program can rely on one thing only: no reachable object goes away.

**r2.** "Names refer to objects. Names are introduced by name binding operations."[^2] An assignment, a function parameter, a `for` target and an import each bind a name. A container that holds an object also refers to it. The object stays reachable while one of these references exists.

**r3.** `del x` does not destroy an object. "Deletion of a name removes the binding of that name from the local or global namespace".[^3] Binding the name to another object also drops one reference. Example: [`names`](#names).

**r4.** CPython frees memory in two stages. "CPython currently uses a reference-counting scheme with (optional) delayed detection of cyclically linked garbage, which collects most objects as soon as they become unreachable, but is not guaranteed to collect garbage containing circular references."[^1] The second stage is the `gc` module: "Since the collector supplements the reference counting already used in Python, you can disable the collector if you are sure your program does not create reference cycles."[^4] Examples: [`prompt`](#prompt), [`cycle`](#cycle).

**r5.** All objects live on one heap that the interpreter owns. "Memory management in Python involves a private heap containing all Python objects and data structures. The management of this private heap is ensured internally by the Python memory manager."[^5] "It is important to understand that the management of the Python heap is performed by the interpreter itself and that the user has no control over it".[^5] C extensions take memory through the functions of the C API, and "The APIs used to allocate and free a block of memory must be from the same domain."[^5]

**r6.** "Python has a pymalloc allocator optimized for small objects (smaller or equal to 512 bytes) with a short lifetime. It uses memory mappings called “arenas” with a fixed size of either 256 KiB on 32-bit platforms or 1 MiB on 64-bit platforms."[^5] "It falls back to PyMem_RawMalloc() and PyMem_RawRealloc() for allocations larger than 512 bytes."[^5] The free-threaded build uses mimalloc in place of pymalloc (see Versions).

**r7.** "The PYTHONMALLOC environment variable can be used to configure the memory allocators used by Python."[^5] The C API reference lists the allocators `malloc`, `pymalloc` and `mimalloc`, and a debug form of each that adds "debug hooks on the Python memory allocators".[^5]

**r8.** CPython does not promise to free everything at exit. "Objects referenced from the global namespaces of Python modules are not always deallocated when Python exits. This may happen if there are circular references."[^6]

## Examples

An object with a `__del__` method shows when CPython frees it. Reference counting frees it at the `del` statement, before the next line runs (rule r4).

```python run id=prompt
class Resource:
    def __init__(self, name):
        self.name = name

    def __del__(self):
        print("freed", self.name)


r = Resource("r1")
print("before del")
del r
print("after del")
```

```output for=prompt
before del
freed r1
after del
```

Two objects that refer to each other form a cycle. Reference counting cannot free them, so the cycle stays after `del`. A run of the cyclic collector frees it. The example turns off the automatic collector with `gc.disable()`, so that the collection happens only at the `gc.collect()` call, and a weak reference shows if the object is still alive.

```python run id=cycle
import gc
import weakref

gc.disable()


class Node:
    pass


a = Node()
b = Node()
a.other = b
b.other = a
probe = weakref.ref(a)
del a, b
print("alive after del:", probe() is not None)
gc.collect()
print("alive after collect:", probe() is not None)
```

```output for=cycle
alive after del: True
alive after collect: False
```

## Versions

In the free-threaded build of 3.13 and later, the default allocator is mimalloc and not pymalloc. The C API reference says: "Changed in version 3.13: In the free-threaded build, the default allocator is now mimalloc."[^5]

## Pitfalls

### `del` does not free an object that something else holds

`del` removes one name (rule r3). A cache, a list, a closure or a global can still refer to the object, and then it stays in memory.

```python wrong id=held
import weakref

cache = []


class Image:
    pass


def load():
    image = Image()
    cache.append(image)
    return image


image = load()
probe = weakref.ref(image)
del image
print("freed:", probe() is None)
```

```output for=held
freed: False
```

The list `cache` still holds the image. Remove it from the cache, or keep it in a [weak container](/python/weak-references/).

### Code that relies on prompt freeing is not portable

CPython frees an object as soon as its last reference goes, so a file that is not closed is often closed soon anyway. Other implementations do not do this. The FAQ says that a loop that opens files and does not close them "will probably run out of file descriptors" on such implementations, and that "you should explicitly close the file or use the with statement".[^6] The reference gives the same advice: "Do not depend on immediate finalization of objects when they become unreachable (so you should always close files explicitly)."[^1] Use a `with` statement.

## Comparison

**Java** has `System.gc()`, which only "suggests that the Java Virtual Machine expend effort toward recycling unused objects", and there is "no guarantee that this effort will recycle any particular number of unused objects".[^7] Python's `gc.collect()` is different: "With no arguments, run a full collection."[^4]

**Go** also uses "tracing garbage collection, which identifies in-use, so-called live, objects by following pointers transitively".[^8] The collector finds the live objects by following pointers, so it needs no second stage for cycles.

**Swift** uses reference counting with no cycle collector. Swift resolves "strong reference cycles by defining some of the relationships between classes as weak or unowned references instead of as strong references".[^9] CPython counts references too, and its `gc` module frees the cycles that Swift asks the programmer to break by hand.

## Implementation notes

| Implementation | How it frees memory |
|---|---|
| CPython | reference counting, and the cyclic collector of the `gc` module (rule r4) |
| PyPy | "The garbage collectors used or implemented by PyPy are not based on reference counting, so the objects are not freed instantly when they are no longer reachable."[^10] Its default collector, incminimark, is "an incremental, generational moving collector".[^11] |
| MicroPython | its `gc` module has `gc.collect()`, and also `gc.mem_free()`, which "is MicroPython extension".[^12] |

The FAQ says the same: "The details of Python memory management depend on the implementation."[^6]

## References
