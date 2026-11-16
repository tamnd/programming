---
tag: 0D7NXD
kind: feature
owner: python
slug: finalizers-and-del
name: Finalizers and __del__
aliases: [__del__, destructor, finalizer, object resurrection, PEP 442, weakref.finalize, unraisable exceptions]
area: languages/python/memory
level: advanced
summary: >-
  __del__ runs when an object is about to be destroyed. When it runs depends on the implementation, so weakref.finalize and with blocks are the safer ways to clean up.
grammar: null
spec: {standard: null, url: "https://docs.python.org/3.15/reference/datamodel.html#object.__del__", locator: "3.3.1. Basic customization, object.__del__"}
see_also: [python/memory-management-in-cpython, python/reference-counting-in-cpython, python/the-cyclic-garbage-collector, python/weak-references]
sources:
- url: https://docs.python.org/3.15/reference/datamodel.html#object.__del__
  title: The Python Language Reference, 3. Data model
  locator: 3.3.1. Basic customization, object.__del__
  accessed: 2026-11-16
- url: https://peps.python.org/pep-0442/
  title: "PEP 442: Safe object finalization"
  locator: Impact; Predictability
  accessed: 2026-11-16
- url: https://docs.python.org/3.15/library/sys.html#sys.unraisablehook
  title: sys, System-specific parameters and functions
  locator: sys.unraisablehook
  accessed: 2026-11-16
- url: https://docs.python.org/3.15/library/weakref.html#weakref.finalize
  title: weakref, Weak references
  locator: weakref.finalize; Comparing finalizers with __del__() methods
  accessed: 2026-11-16
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/lang/Object.html#finalize()
  title: Class Object, Java SE 25 API
  locator: finalize()
  accessed: 2026-11-16
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/lang/ref/Cleaner.html
  title: Class Cleaner, Java SE 25 API
  locator: the class description
  accessed: 2026-11-16
- url: https://learn.microsoft.com/en-us/dotnet/csharp/programming-guide/classes-and-structs/finalizers
  title: Finalizers, C# programming guide
  locator: the introduction; Remarks
  accessed: 2026-11-16
- url: https://pkg.go.dev/runtime#AddCleanup
  title: Package runtime, The Go Programming Language
  locator: func AddCleanup
  accessed: 2026-11-16
---

## Synopsis

```text
class Resource:
    def __del__(self):          # the finalizer of the instance
        ...

import weakref
f = weakref.finalize(obj, func, *args, **kwargs)
f()          # run it now, at most once
f.alive      # True until it has run
f.detach()   # cancel it and get (obj, func, args, kwargs)
f.atexit     # run it at exit if still alive (default True)
```

## Semantics

**r1.** `__del__` is "Called when the instance is about to be destroyed. This is also called a finalizer or (improperly) a destructor."[^1] It does not run at `del x`: "del x doesn’t directly call x.__del__() — the former decrements the reference count for x by one, and the latter is only called when x’s reference count reaches zero."[^1]

**r2.** A subclass must call the finalizer of its base: "If a base class has a __del__() method, the derived class’s __del__() method, if any, must explicitly call it to ensure proper deletion of the base class part of the instance."[^1]

**r3.** An object with `__del__` in a cycle is freed by the cyclic collector, and its finalizer runs then. PEP 442 says that such objects "can now be reclaimed when they are part of a reference cycle".[^2] The PEP has "Python-Version: 3.4".[^2] In a cycle, "the order in which finalizers are called (step 2 above) is undefined."[^2] Example: [`cycle`](#cycle).

**r4.** A finalizer can save its object. "It is possible (though not recommended!) for the __del__() method to postpone destruction of the instance by creating a new reference to it. This is called object resurrection. It is implementation-dependent whether __del__() is called a second time when a resurrected object is about to be destroyed; the current CPython implementation only calls it once."[^1] Example: [`resurrect`](#resurrect).

**r5.** An exception in `__del__` does not reach the caller: "exceptions that occur during their execution are ignored, and a warning is printed to sys.stderr instead."[^1] The warning goes through `sys.unraisablehook`, which is "Called when an exception has occurred but there is no way for Python to handle it. For example, when a destructor raises an exception".[^3] Example: [`ignored`](#ignored).

**r6.** A finalizer can run at a bad time. "__del__() can be invoked when arbitrary code is being executed, including from any arbitrary thread." "__del__() can be executed during interpreter shutdown."[^1] "It is not guaranteed that __del__() methods are called for objects that still exist when the interpreter exits."[^1]

**r7.** `weakref.finalize(obj, func, *args, **kwargs)` "Return a callable finalizer object which will be called when obj is garbage collected."[^4] "A finalizer is considered alive until it is called (either explicitly or at garbage collection), and after that it is dead."[^4] At exit, "each remaining live finalizer is called unless its atexit attribute has been set to false."[^4] Examples: [`finalize`](#finalize), [`remove`](#remove).

**r8.** The function and the arguments of `weakref.finalize` must not refer to the object. "It is important to ensure that func, args and kwargs do not own any references to obj, either directly or indirectly, since otherwise obj will never be garbage collected. In particular, func should not be a bound method of obj."[^4] Example: [`bound`](#bound).

## Examples

A finalizer in a cycle runs when the collector frees the cycle (rule r3). The list collects the names, and `sorted` hides the order, which is undefined.

```python run id=cycle
import gc

finalized = []


class Node:
    def __init__(self, name):
        self.name = name

    def __del__(self):
        finalized.append(self.name)


gc.disable()
a = Node("a")
b = Node("b")
a.peer = b
b.peer = a
del a, b
print(finalized)
gc.collect()
print(sorted(finalized))
```

```output for=cycle
[]
['a', 'b']
```

Resurrection: `__del__` puts the object in a list, so it lives on. When the list lets it go, CPython does not call `__del__` again (rule r4).

```python run id=resurrect
saved = []


class Phoenix:
    def __del__(self):
        print("__del__ runs")
        saved.append(self)


p = Phoenix()
del p
print(len(saved))
saved.clear()
print("cleared")
```

```output for=resurrect
__del__ runs
1
cleared
```

The exception in `__del__` goes to `sys.unraisablehook`, and the program goes on (rule r5). The example sets its own hook to print one short line.

```python run id=ignored
import sys


def hook(unraisable):
    print("ignored:", type(unraisable.exc_value).__name__)


sys.unraisablehook = hook


class Bad:
    def __del__(self):
        raise ValueError("boom")


b = Bad()
del b
print("still running")
```

```output for=ignored
ignored: ValueError
still running
```

`weakref.finalize` with a plain function. The finalizer is alive until the object goes (rule r7).

```python run id=finalize
import weakref


class Connection:
    pass


conn = Connection()
done = weakref.finalize(conn, print, "connection closed")
print(done.alive)
del conn
print(done.alive)
```

```output for=finalize
True
connection closed
False
```

The pattern of the weakref documentation: a `remove()` method that calls the finalizer. The cleanup runs once, at the first of three events: the call, the collection of the object, or the exit.[^4] Here the path is a plain string, and nothing is deleted.

```python run id=remove
import weakref


class TempDir:
    def __init__(self, name):
        self.name = name
        self._finalizer = weakref.finalize(self, print, "remove", name)

    def remove(self):
        self._finalizer()


d = TempDir("/tmp/x1")
d.remove()
d.remove()
del d
print("end")
```

```output for=remove
remove /tmp/x1
end
```

The examples give the same output on CPython 3.12.3, 3.13.14 and 3.14.6.

## Pitfalls

### A bound method as the finalizer function

`self.close` holds a reference to `self`, so the object can never be collected (rule r8). The finalizer runs only at exit, after the last line of the program.

```python wrong id=bound
import weakref


class Handle:
    def __init__(self):
        self.finalizer = weakref.finalize(self, self.close)

    def close(self):
        print("closed")


h = Handle()
del h
print("after del")
```

```output for=bound
after del
closed
```

Pass a function and the data it needs, as in [`remove`](#remove): `weakref.finalize(self, os.close, self.fd)`.

### `__del__` as the only way to release a resource

The time of the call depends on the implementation (rule r6). The weakref documentation says that "handling of __del__() methods is notoriously implementation specific, since it depends on internal details of the interpreter’s garbage collector implementation."[^4] Give the class a `close()` method and support `with`, and keep a finalizer only as a last line of defence.

### A lock or a global in `__del__`

"If __del__() needs to take a lock or invoke any other blocking resource, it may deadlock as the resource may already be taken by the code that gets interrupted to execute __del__()."[^1] At shutdown, "the global variables it needs to access (including other modules) may already have been deleted or set to None."[^1] Keep `__del__` short, and do not take locks in it.

## Comparison

**Java** has `Object.finalize()`, and "Finalization is deprecated and subject to removal in a future release."[^5] Its replacement is `java.lang.ref.Cleaner`: "Cleaning actions are registered to run after the cleaner is notified that the object has become phantom reachable."[^6] This is close to `weakref.finalize`, which also runs a separate function after the object is gone.

**C#** has finalizers, written `~ClassName()`. "The programmer has no control over when the finalizer is called; the garbage collector decides when to call it."[^7] The guide adds that "In most cases, you can avoid writing a finalizer by using the System.Runtime.InteropServices.SafeHandle or derived classes".[^7]

**Go** has `runtime.AddCleanup`: "Some time after ptr is no longer reachable, the runtime will call cleanup(arg) in a separate goroutine."[^8] It has the same trap as rule r8: "If ptr is reachable from cleanup or arg, ptr will never be collected and the cleanup will never run."[^8]

## Implementation notes

On CPython, reference counting runs `__del__` at the moment the count falls to zero, and the cyclic collector runs it for objects in cycles (rules r1 and r3). PEP 442 describes the CPython design: "an object’s finalizer is always called exactly once, even if it was resurrected afterwards."[^2]

## References
