---
tag: 9JNDJF
kind: feature
owner: python
slug: the-cyclic-garbage-collector
name: The cyclic garbage collector
aliases: [gc module, gc.collect, generational garbage collection, reference cycles, gc.freeze, gc.disable, gc thresholds]
area: languages/python/memory
level: advanced
summary: >-
  CPython's gc module finds groups of objects that refer only to each other and frees them. It sorts container objects into generations, and it can be run, tuned, frozen or turned off.
grammar: null
spec: {standard: null, url: "https://docs.python.org/3.15/library/gc.html", locator: "gc, Garbage Collector interface"}
see_also: [python/memory-management-in-cpython, python/reference-counting-in-cpython, python/finalizers-and-del, python/weak-references]
sources:
- url: https://docs.python.org/3.15/reference/datamodel.html#objects-values-and-types
  title: The Python Language Reference, 3. Data model
  locator: 3.1. Objects, values and types
  accessed: 2026-11-16
- url: https://docs.python.org/3.15/library/gc.html
  title: gc, Garbage Collector interface
  locator: the introduction; gc.collect; gc.set_threshold; gc.freeze; gc.callbacks; gc.garbage
  accessed: 2026-11-16
- url: https://github.com/python/cpython/blob/main/InternalDocs/garbage_collector.md
  title: Garbage collector design, CPython InternalDocs
  locator: Identifying reference cycles; Optimization, generations; Collecting the oldest generation
  accessed: 2026-11-16
- url: https://docs.python.org/3.15/whatsnew/3.14.html#garbage-collection
  title: What's New In Python 3.14
  locator: Garbage collection; gc
  accessed: 2026-11-16
- url: https://github.com/swiftlang/swift-book/blob/main/TSPL.docc/LanguageGuide/AutomaticReferenceCounting.md
  title: Automatic Reference Counting, The Swift Programming Language
  locator: Strong Reference Cycles Between Class Instances
  accessed: 2026-11-16
- url: https://go.dev/doc/gc-guide
  title: A Guide to the Go Garbage Collector
  locator: Tracing Garbage Collection
  accessed: 2026-11-16
- url: https://doc.pypy.org/en/latest/gc_info.html
  title: PyPy, Garbage collector documentation and configuration
  locator: Incminimark
  accessed: 2026-11-16
---

## Synopsis

```text
import gc

gc.collect(generation=2)      # run a collection now; returns a count
gc.disable(); gc.enable(); gc.isenabled()
gc.get_threshold(); gc.set_threshold(threshold0, threshold1, threshold2)
gc.get_count()
gc.freeze(); gc.unfreeze(); gc.get_freeze_count()
gc.is_tracked(obj)
gc.get_referrers(*objs); gc.get_referents(*objs)
gc.callbacks                  # a list of callables
gc.garbage                    # objects that could not be freed
gc.set_debug(gc.DEBUG_LEAK)
```

## Semantics

**r1.** Reference counting cannot free a cycle, because each object in it keeps a reference to another. The reference says that CPython's scheme "is not guaranteed to collect garbage containing circular references".[^1] The `gc` module adds a second stage: "Since the collector supplements the reference counting already used in Python, you can disable the collector if you are sure your program does not create reference cycles."[^2] Example: [`cycle`](#cycle).

**r2.** The collector looks only at containers. It "**only focuses** on cleaning container objects (that is, objects that can contain a reference to one or more objects)".[^3] An int or a str cannot be in a cycle, so it is not tracked. `gc.is_tracked()` tells if an object is tracked. Example: [`tracked`](#tracked).

**r3.** To find a cycle, the collector copies the count of each container in the set. Then it "decrements by one the `gc_ref` field of any other object that container is referencing".[^3] "After all the objects have been scanned, only the objects that have references from outside the “objects to scan” list will have `gc_ref > 0`."[^3] Objects that these outside objects can reach are alive, and the rest are unreachable and are freed.[^3]

**r4.** The default build splits the containers into three generations. "Every new object starts in the first generation (generation 0)."[^3] "If an object survives a collection it is moved into the next older generation."[^2] "When the number of allocations minus the number of deallocations exceeds threshold0, collection starts."[^2] Generation 1 is examined when generation 0 "has been examined more than threshold1 times since generation 1 has been examined".[^2] A full collection also needs the ratio `long_lived_pending / long_lived_total` to be "above a given value (hardwired to 25%)", because its cost grows with the number of long-lived objects.[^3]

**r5.** `gc.collect()` runs a collection at once. "With no arguments, run a full collection." "The sum of collected objects and uncollectable objects is returned."[^2]

**r6.** `gc.disable()` stops the automatic collections only. Reference counting goes on, and `gc.collect()` still works. "Setting threshold0 to zero disables collection."[^2]

**r7.** `gc.freeze()` moves all tracked objects "to a permanent generation and ignore them in all the future collections".[^2] It is for programs that fork: "call gc.disable() early in the parent process, gc.freeze() right before fork(), and gc.enable() early in child processes."[^2] Example: [`freeze`](#freeze).

**r8.** `gc.callbacks` is "A list of callbacks that will be invoked by the garbage collector before and after collection."[^2] Each callback gets a phase, `start` or `stop`, and a dict `info` with keys such as `generation`, `collected` and `uncollectable`.[^2] Example: [`callbacks`](#callbacks).

**r9.** `gc.garbage` holds "objects which the collector found to be unreachable but could not be freed (uncollectable objects)". The documentation says that "this list should be empty most of the time, except when using instances of C extension types with a non-NULL tp_del slot."[^2] With `gc.set_debug(gc.DEBUG_LEAK)`, the collector keeps all the unreachable objects in this list, so a program can look at them.[^2]

## Examples

Two nodes that refer to each other stay alive after `del`, because each count is still one. `gc.collect()` frees them. The example turns off the automatic collector so that the collection happens only at the explicit call.

```python run id=cycle
import gc
import weakref


class Node:
    def __init__(self, name):
        self.name = name


gc.disable()
a = Node("a")
b = Node("b")
a.peer = b
b.peer = a
probes = [weakref.ref(a), weakref.ref(b)]
del a, b
print([p() is not None for p in probes])
gc.collect()
print([p() is not None for p in probes])
```

```output for=cycle
[True, True]
[False, False]
```

Atomic values are not tracked, and containers and instances are (rule r2).

```python run id=tracked
import gc


class Node:
    pass


print(gc.is_tracked(0), gc.is_tracked("a"))
print(gc.is_tracked([]), gc.is_tracked(Node()))
```

```output for=tracked
False False
True True
```

A callback sees each collection. A call to `gc.collect()` with no argument collects generation 2, because "generation 2 is the oldest generation".[^2]

```python run id=callbacks
import gc


def report(phase, info):
    print(phase, "generation", info["generation"])


gc.callbacks.append(report)
gc.collect()
gc.callbacks.remove(report)
```

```output for=callbacks
start generation 2
stop generation 2
```

`gc.freeze()` moves every tracked object out of the collector's view, and `gc.unfreeze()` puts them back (rule r7).

```python run id=freeze
import gc

gc.disable()
gc.freeze()
print(gc.get_freeze_count() > 0)
gc.unfreeze()
print(gc.get_freeze_count())
gc.enable()
```

```output for=freeze
True
0
```

The four examples give the same output on CPython 3.12.3, 3.13.14 and 3.14.6.

## Versions

The default thresholds are not the same in all versions. On CPython 3.12.3, `gc.get_threshold()` returned `(700, 10, 10)`, and on 3.13.14 and 3.14.6 it returned `(2000, 10, 10)`. The design notes show the value `(2000, 10, 10)`.[^3] Do not hard-code these numbers.

3.14 had an incremental collector for a short time. The release notes say: "Python 3.14.0-3.14.4 shipped with a new incremental GC. However, due to a number of reports of significant memory pressure in production environments, it has been reverted back to the generational GC from 3.13. This is the GC now used in Python 3.14.5 and later."[^4] The `gc` documentation records the effects: "Changed in version 3.14: generation=1 performs an increment of collection. Changed in version 3.14.5: generation=1 performs collection of the middle generation."[^2] Also "Changed in version 3.14: threshold2 is ignored Changed in version 3.14.5: threshold2 is restored to match Python 3.13 behavior."[^2]

## Pitfalls

### `gc.disable()` in a program that makes cycles

With the collector off, each cycle stays in memory until the program calls `gc.collect()` (rule r6). Many common objects form cycles: the design notes say that "Exceptions contain traceback objects that contain a list of frames that contain the exception itself."[^3] Turn the collector off only for a short, measured part of a program, or call `gc.collect()` at points that you choose.

### Code that depends on the time of a collection

A collection starts when the allocation count passes a threshold (rule r4), so the time of a collection depends on the whole program. A test that waits for an object in a cycle to go must call `gc.collect()` first. Use `with` blocks and explicit `close()` calls for resources, not the collector.

## Comparison

**Swift** has reference counting and no cycle collector. The programmer resolves "strong reference cycles by defining some of the relationships between classes as weak or unowned references instead of as strong references".[^5] In CPython the same cycle is freed by the `gc` module, so weak references are a choice and not a need.

**Go** has only a tracing collector, which "identifies in-use, so-called live, objects by following pointers transitively".[^6] A cycle that no live object reaches is not live, so Go needs no separate step for cycles. CPython traces only the containers and only to find cycles (rule r3); reference counting frees the other objects.

## Implementation notes

The free-threaded build uses the same algorithm on other data structures, and it "pauses other executing threads when performing a collection for thread safety".[^3]

PyPy's default collector, incminimark, is "an incremental, generational moving collector".[^7] It is configured with environment variables such as `PYPY_GC_NURSERY`, and not with the thresholds of rule r4.[^7]

## References
