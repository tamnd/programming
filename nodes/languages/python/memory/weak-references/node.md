---
tag: 0JZ4W0
kind: feature
owner: python
slug: weak-references
name: Weak References
aliases: [weakref, weakref.ref, WeakValueDictionary, WeakKeyDictionary, WeakSet, WeakMethod, weakref.proxy, weak cache]
area: languages/python/memory
level: core
summary: >-
  A weak reference refers to an object but does not keep it alive. The weakref module gives weak references, callbacks, and dicts and sets that drop an entry when its object goes away.
grammar: null
spec: {standard: null, url: "https://docs.python.org/3.15/library/weakref.html", locator: "weakref, Weak references"}
see_also: [python/memory-management-in-cpython, python/finalizers-and-del, python/the-cyclic-garbage-collector]
sources:
- url: https://docs.python.org/3.15/tutorial/stdlib2.html#weak-references
  title: The Python Tutorial, 11. Brief tour of the standard library, part II
  locator: 11.6. Weak references
  accessed: 2026-11-16
- url: https://docs.python.org/3.15/library/weakref.html
  title: weakref, Weak references
  locator: the introduction; weakref.ref; weakref.proxy; WeakKeyDictionary; WeakValueDictionary; WeakSet; WeakMethod
  accessed: 2026-11-16
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/lang/ref/WeakReference.html
  title: Class WeakReference, Java SE 25 API
  locator: the class description
  accessed: 2026-11-16
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/WeakRef
  title: WeakRef, MDN Web Docs JavaScript Reference
  locator: the introduction; Avoid where possible
  accessed: 2026-11-16
- url: https://learn.microsoft.com/en-us/dotnet/api/system.weakreference
  title: WeakReference Class, .NET API
  locator: the introduction
  accessed: 2026-11-16
- url: https://doc.rust-lang.org/std/rc/index.html
  title: Module rc, The Rust Standard Library
  locator: the introduction
  accessed: 2026-11-16
- url: https://docs.python.org/3.15/reference/datamodel.html#slots
  title: The Python Language Reference, 3. Data model
  locator: 3.3.2.4. __slots__
  accessed: 2026-11-16
---

## Synopsis

```text
import weakref

r = weakref.ref(obj)               # r() is obj, or None after obj is gone
r = weakref.ref(obj, callback)     # callback(r) runs when obj is about to go
p = weakref.proxy(obj)             # use p like obj
m = weakref.WeakMethod(obj.method) # m() is a new bound method, or None

weakref.WeakValueDictionary()      # values are weak
weakref.WeakKeyDictionary()        # keys are weak
weakref.WeakSet()                  # elements are weak
weakref.finalize(obj, func, *args) # see Finalizers and __del__
```

## Semantics

**r1.** The tutorial gives the need: "occasionally there is a need to track objects only as long as they are being used by something else. Unfortunately, just tracking them creates a reference that makes them permanent. The weakref module provides tools for tracking objects without creating a reference."[^1]

**r2.** "A weak reference to an object is not enough to keep the object alive: when the only remaining references to a referent are weak references, garbage collection is free to destroy the referent and reuse its memory for something else."[^2] Until then, "the weak reference may return the object even if there are no strong references to it."[^2]

**r3.** `weakref.ref(object)` returns a reference object. "The original object can be retrieved by calling the reference object if the referent is still alive; if the referent is no longer alive, calling the reference object will cause None to be returned."[^2] Example: [`ref`](#ref).

**r4.** A callback runs "when the object is about to be finalized; the weak reference object will be passed as the only parameter to the callback; the referent will no longer be available."[^2] An exception in a callback does not reach the caller; it is handled "in exactly the same way as exceptions raised from an object’s __del__() method."[^2]

**r5.** "A primary use for weak references is to implement caches or mappings holding large objects, where it’s desired that a large object not be kept alive solely because it appears in a cache or mapping."[^2] In a `WeakValueDictionary`, "Entries in the dictionary will be discarded when no strong reference to the value exists any more."[^2] A `WeakKeyDictionary` does the same for keys, and "can be used to associate additional data with an object owned by other parts of an application without adding attributes to those objects."[^2] A `WeakSet` "keeps weak references to its elements."[^2] Examples: [`cache`](#cache), [`keys`](#keys).

**r6.** "Not all objects can be weakly referenced."[^2] "Objects which support weak references include class instances, functions written in Python (but not in C), instance methods, sets, frozensets, some file objects, generators, type objects, sockets, arrays, deques, regular expression pattern objects, and code objects."[^2] "Several built-in types such as list and dict do not directly support weak references but can add support through subclassing".[^2] "Other built-in types such as tuple and int do not support weak references even when subclassed."[^2] A weak reference to an object that does not support it raises `TypeError`. Examples: [`types`](#types), [`int`](#int).

**r7.** A class with `__slots__` has no weak reference support unless it asks for it: "weak reference support is disabled unless a '__weakref__' string is also present in the sequence of strings in the __slots__ declaration."[^2] Example: [`slots`](#slots).

**r8.** "Since a bound method is ephemeral, a standard weak reference cannot keep hold of it. WeakMethod has special code to recreate the bound method until either the object or the original function dies".[^2] Example: [`method`](#method).

## Examples

A reference with a callback. The callback runs when the object goes, and after that the reference returns `None` (rules r3 and r4).

```python run id=ref
import weakref


class Report:
    pass


report = Report()
r = weakref.ref(report, lambda ref: print("callback, dead:", ref() is None))
print(r() is report)
del report
print(r())
```

```output for=ref
True
callback, dead: True
None
```

A cache of images by name. Two calls share one object, and the cache entry goes when the last user lets the image go (rule r5). On CPython this happens at `del`. The tutorial calls `gc.collect()` first, which other implementations need.[^1]

```python run id=cache
import weakref


class Image:
    def __init__(self, name):
        self.name = name


cache = weakref.WeakValueDictionary()


def load(name):
    image = cache.get(name)
    if image is None:
        print("load", name)
        image = Image(name)
        cache[name] = image
    return image


a = load("logo")
b = load("logo")
print(a is b, len(cache))
del a, b
print(len(cache))
load("logo")
```

```output for=cache
load logo
True 1
0
load logo
```

Extra data for objects that the program does not own, and a set of live objects.

```python run id=keys
import weakref


class Widget:
    pass


notes = weakref.WeakKeyDictionary()
live = weakref.WeakSet()
w1, w2 = Widget(), Widget()
notes[w1] = "first"
live.add(w1)
live.add(w2)
print(len(notes), len(live))
del w1
print(len(notes), len(live))
```

```output for=keys
1 2
0 1
```

Which types support weak references (rule r6). A subclass of `list` does, and the base types do not.

```python run id=types
import weakref


class MyList(list):
    pass


print(weakref.ref(MyList([1, 2])) is not None)
for value in (42, "text", [1, 2], {"a": 1}):
    try:
        weakref.ref(value)
    except TypeError as err:
        print(err)
```

```output for=types
True
cannot create weak reference to 'int' object
cannot create weak reference to 'str' object
cannot create weak reference to 'list' object
cannot create weak reference to 'dict' object
```

```python fails id=int expect=exception:TypeError
import weakref

weakref.ref(42)
```

`__slots__` without `__weakref__` turns weak references off (rule r7).

```python run id=slots
import weakref


class Point:
    __slots__ = ("x", "y")


class WeakPoint:
    __slots__ = ("x", "y", "__weakref__")


try:
    weakref.ref(Point())
except TypeError as err:
    print(err)
print(weakref.ref(WeakPoint()) is not None)
```

```output for=slots
cannot create weak reference to 'Point' object
True
```

The examples give the same output on CPython 3.12.3, 3.13.14 and 3.14.6.

## Pitfalls

### A weak reference to a bound method

`button.click` makes a new bound method object each time. Nothing else holds it, so a plain weak reference to it is dead at once (rule r8). `WeakMethod` makes the bound method again on each call.

```python wrong id=method
import weakref


class Button:
    def click(self):
        return "clicked"


button = Button()
r = weakref.ref(button.click)
print(r())
m = weakref.WeakMethod(button.click)
print(m()())
```

```output for=method
None
clicked
```

The first line is the bug: `r()` is `None` while `button` is still alive.

### A weak cache of values that nothing else holds

A `WeakValueDictionary` keeps an entry only while something else holds the value (rule r5). A cache of results that no caller keeps is empty after each call. Use `functools.lru_cache` or a normal dict for that case.

### Code that expects the entry to go at once

On CPython the entry goes when the last strong reference goes. Other implementations free objects later, and an object in a cycle waits for the cyclic collector. The tutorial example calls `gc.collect()` before it checks the cache.[^1]

## Comparison

**Java** has `java.lang.ref.WeakReference`, "Weak reference objects, which do not prevent their referents from being made finalizable, finalized, and then reclaimed."[^3] It names the same use as Python: "Weak references are most often used to implement canonicalizing mappings."[^3]

**JavaScript** has `WeakRef`, which "lets you hold a weak reference to another object, without preventing that object from getting garbage-collected."[^4] MDN warns: "Correct use of WeakRef takes careful thought, and it's best avoided if possible."[^4] MDN also says: "When, how, and whether garbage collection occurs is down to the implementation of any given JavaScript engine."[^4] CPython with reference counting empties a weak reference at a known point (rule r3).

**C#** has `System.WeakReference`, which "references an object while still allowing that object to be reclaimed by garbage collection."[^5]

**Rust** has `std::rc::Weak`. "A Weak pointer can be upgraded to an Rc, but this will return None if the value stored in the allocation has already been dropped."[^6] Rust needs `Weak` to break cycles, because "A cycle between Rc pointers will never be deallocated."[^6] CPython frees cycles with its collector, so in Python a weak reference is for caches and observers, not for cycles.

## Implementation notes

A class instance gets weak reference support from its `__weakref__` attribute. The data model says that `__slots__` "deny the creation of __dict__ and __weakref__ (unless explicitly declared in __slots__ or available in a parent.)"[^7] A type written in C has weak reference support only if its author adds it: "Extension types can easily be made to support weak references".[^2] Some built-in types have no support (rule r6).

## References
