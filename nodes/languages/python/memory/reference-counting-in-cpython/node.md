---
tag: 3QW78B
kind: feature
owner: python
slug: reference-counting-in-cpython
name: Reference counting in CPython
aliases: [refcount, sys.getrefcount, Py_INCREF, Py_DECREF, immortal objects, PEP 683, biased reference counting]
area: languages/python/memory
level: advanced
summary: >-
  Each CPython object holds a count of the references to it. The object is freed when the count falls to zero. Some objects are immortal, and the free-threaded build counts in other ways.
grammar: null
spec: {standard: null, url: "https://docs.python.org/3.15/c-api/refcounting.html", locator: "Reference Counting"}
see_also: [python/memory-management-in-cpython, python/the-cyclic-garbage-collector, python/finalizers-and-del]
sources:
- url: https://docs.python.org/3.15/reference/datamodel.html#objects-values-and-types
  title: The Python Language Reference, 3. Data model
  locator: 3.1. Objects, values and types; 3.3.1. Basic customization, object.__del__
  accessed: 2026-11-16
- url: https://docs.python.org/3.15/library/sys.html#sys.getrefcount
  title: sys, System-specific parameters and functions
  locator: sys.getrefcount; sys._is_immortal
  accessed: 2026-11-16
- url: https://peps.python.org/pep-0683/
  title: "PEP 683: Immortal Objects, Using a Fixed Refcount"
  locator: Abstract; Immortal Global Objects
  accessed: 2026-11-16
- url: https://docs.python.org/3.15/howto/free-threading-python.html
  title: Python support for free threading
  locator: Immortalization; Free-threaded reference counting can cause objects to live longer
  accessed: 2026-11-16
- url: https://peps.python.org/pep-0703/
  title: "PEP 703: Making the Global Interpreter Lock Optional in CPython"
  locator: Reference Counting, Biased Reference Counting
  accessed: 2026-11-16
- url: https://docs.python.org/3.15/whatsnew/3.14.html#optimizations
  title: What's New In Python 3.14
  locator: Optimizations
  accessed: 2026-11-16
- url: https://doc.pypy.org/en/latest/cpython_differences.html
  title: Differences between PyPy and CPython
  locator: Differences related to garbage collection strategies; Missing sys.getrefcount
  accessed: 2026-11-16
- url: https://github.com/swiftlang/swift-book/blob/main/TSPL.docc/LanguageGuide/AutomaticReferenceCounting.md
  title: Automatic Reference Counting, The Swift Programming Language
  locator: How ARC Works; Strong Reference Cycles Between Class Instances
  accessed: 2026-11-16
- url: https://doc.rust-lang.org/std/rc/index.html
  title: Module rc, The Rust Standard Library
  locator: the introduction
  accessed: 2026-11-16
- url: https://docs.python.org/3.15/c-api/refcounting.html
  title: Reference Counting, The Python/C API Reference Manual
  locator: Py_REFCNT; Py_SET_REFCNT
  accessed: 2026-11-16
---

## Synopsis

```text
import sys

sys.getrefcount(obj)    # the count, plus one for the argument itself
sys._is_immortal(obj)   # 3.14 and later, CPython only
```

```text
Py_INCREF(op);           /* add one reference */
Py_DECREF(op);           /* drop one; at zero the object is freed */
Py_ssize_t n = Py_REFCNT(op);
```

Python code never changes a count by hand. Each new reference (a name, a list item, an attribute, an argument) adds one, and each reference that goes away takes one off. C extensions call `Py_INCREF` and `Py_DECREF` themselves.

## Semantics

**r1.** Reference counting is a detail of CPython, not a rule of the language. The reference says that "CPython currently uses a reference-counting scheme with (optional) delayed detection of cyclically linked garbage, which collects most objects as soon as they become unreachable".[^1] Other implementations do not count references (see [Implementation notes](#implementation-notes)).

**r2.** When the count of an object falls to zero, CPython frees the object at once. If its type has a `__del__` method, the method runs first. The reference says that `del x` "decrements the reference count for x by one", and that `x.__del__()` "is only called when x’s reference count reaches zero."[^1] This happens at the statement that drops the last reference: a `del`, a new assignment to the name, or the return of the function that held the local. Example: [`scope`](#scope).

**r3.** `sys.getrefcount()` returns the count. "The count returned is generally one higher than you might expect, because it includes the (temporary) reference as an argument to getrefcount()."[^2] The number is not a reliable count: "do not rely on the returned value to be accurate, other than a value of 0 or 1."[^2] Example: [`count`](#count).

**r4.** Some objects are immortal. PEP 683 says that "such an object’s refcount will never reach 0, and thus the object will never be cleaned up".[^3] The PEP makes immortal the "singletons (None, True, False, Ellipsis, NotImplemented)", all static types, and the static global objects such as small ints.[^3] `getrefcount()` gives these objects "a very high refcount that does not reflect the actual number of references".[^2] Example: [`immortal`](#immortal).

**r5.** A count cannot see a cycle. Two objects that refer to each other keep each count above zero after the last outside reference goes. CPython frees such objects with the [cyclic garbage collector](/python/the-cyclic-garbage-collector/), which "is not guaranteed" for all garbage with circular references.[^1]

**r6.** The free-threaded build uses other kinds of counts. "The free-threaded build uses “biased reference counting”, with a fast-path for objects “owned” by the current thread and a slow path for other objects."[^4] PEP 703 explains: "Reference counting operations from the owning thread use non-atomic instructions to modify a “local” reference count. Other threads use atomic instructions to modify a “shared” reference count."[^5] The how-to warns that in this build "deallocation can be deferred", so an object can live after its last reference goes.[^4]

## Examples

The count goes up by one for each reference and down by one when the reference goes. The example prints the change from a base value, because the absolute value includes the temporary reference of rule r3.

```python run id=count
import sys

data = object()
base = sys.getrefcount(data)
holders = [data, data]
print(sys.getrefcount(data) - base)
holders.pop()
print(sys.getrefcount(data) - base)
del holders
print(sys.getrefcount(data) - base)
```

```output for=count
2
1
0
```

The list holds two references. `pop()` removes one, and `del holders` frees the list, which drops the other.

Rule r2 in three places: the end of a function, a new assignment to a name, and `del`.

```python run id=scope
class Tracer:
    def __init__(self, name):
        self.name = name

    def __del__(self):
        print("del", self.name)


def work():
    t = Tracer("local")
    print("in work", t.name)


work()
print("after work")
x = Tracer("first")
x = Tracer("second")
print("rebound")
del x
print("end")
```

```output for=scope
in work local
del local
after work
del first
rebound
del second
end
```

The singletons and small ints are immortal (rule r4), so their count is very high. A new list and a large int are not.

```python run id=immortal
import sys

for value in (None, True, 7, [], 10**30):
    print(repr(value), sys.getrefcount(value) > 2**30)
```

```output for=immortal
None True
True True
7 True
[] False
1000000000000000000000000000000 False
```

The three examples give the same output on CPython 3.12.3, 3.13.14 and 3.14.6.

## Versions

Immortal objects came with PEP 683, which has "Python-Version: 3.12".[^3] The sys documentation says: "Changed in version 3.12: Immortal objects have very large refcounts that do not match the actual number of references to the object."[^2]

In 3.14, "The interpreter now avoids some reference count modifications internally when it’s safe to do so. This can lead to different values being returned from sys.getrefcount() and Py_REFCNT() compared to previous versions of Python."[^6] The sys documentation gives 3.14 as the version that added `sys._is_immortal()`.[^2]

## Pitfalls

### A test that checks an exact count

A test that asserts `sys.getrefcount(x) == 2` breaks when the version changes (see Versions) or when the object is immortal. Compare two counts of the same object, as in [`count`](#count), or test that an object is freed with a weak reference.

### Code that relies on prompt freeing

Rule r2 is true only for CPython with the GIL. PyPy objects "are not freed instantly when they are no longer reachable",[^7] and the free-threaded build can defer the free (rule r6). Close files and other resources with `with` or an explicit call, not with `__del__`.

## Comparison

**Swift** counts references to class instances with ARC. "when an instance is no longer needed, ARC frees up the memory used by that instance".[^8] Swift asks the programmer to resolve "strong reference cycles by defining some of the relationships between classes as weak or unowned references instead of as strong references".[^8] CPython counts in the same way, but its cyclic collector frees the cycles (rule r5).

**Rust** gives reference counts as a library type. "When the last Rc pointer to a given allocation is destroyed, the value stored in that allocation (often referred to as “inner value”) is also dropped."[^9] As in Swift, "A cycle between Rc pointers will never be deallocated. For this reason, Weak is used to break cycles."[^9]

## Implementation notes

PEP 683 says that "the CPython runtime maintains a small amount of mutable state in the allocated memory of each object"; the count is part of it.[^3] `Py_REFCNT()` reads the count. The C API gives the same warning as `sys`: "do not rely on the returned value to be accurate, other than a value of 0 or 1."[^10] In the free-threaded build, `Py_SET_REFCNT()` makes an object immortal when the new count "is larger than UINT32_MAX".[^10]

PyPy counts no references: "The garbage collectors used or implemented by PyPy are not based on reference counting".[^7] So "PyPy does not implement" `sys.getrefcount()`, and a call raises `AttributeError`.[^7]

## References
