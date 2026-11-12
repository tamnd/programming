---
tag: 4XE8YW
kind: pattern
slug: lazy-initialization
name: Lazy initialization
aliases: [lazy init, lazy loading, lazy holder, lazy value, deferred initialization, initialize on first use]
area: software-design/creational-patterns
level: core
summary: >-
  Lazy initialization creates an object or computes a value the first time a caller needs it, and stores the result for later calls. It saves work that a program may never need.
family: creational-design
polarity: positive
intent: "Delay the creation of an object or the computation of a value until the first time a caller needs it, and then keep the result for every later use."
also_known_as: [Lazy Load, Initialize on Demand]
history:
  year: 2002
  people: [Martin Fowler, Bill Pugh, Joshua Bloch, Doug Lea]
  note: Fowler named Lazy Load in Patterns of Enterprise Application Architecture (2002). The Double-Checked Locking is Broken declaration, signed by Pugh, Bloch, Lea and others, showed that a common thread-safe form fails in Java without more synchronization, and the memory model of JDK 5 made it work with a volatile field.
see_also: [singleton-pattern, object-pool-pattern, multiton-pattern, proxy-pattern, static-factory-method]
sources:
  - url: https://learn.microsoft.com/en-us/dotnet/api/system.lazy-1?view=net-9.0
    title: Lazy<T> Class, .NET API reference
    locator: Remarks (the purpose of lazy initialization; thread safety; exception caching)
  - url: https://docs.python.org/3.14/library/functools.html#functools.cached_property
    title: The Python Standard Library, functools, Python 3.14
    locator: cached_property; cache
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Functions/get
    title: get, MDN Web Docs
    locator: Smart / self-overwriting / lazy getters
  - url: https://www.cs.umd.edu/~pugh/java/memoryModel/DoubleCheckedLocking.html
    title: The "Double-Checked Locking is Broken" Declaration
    locator: the signers; introduction; Fixing Double-Checked Locking using Volatile; Making it work for static singletons
  - url: https://docs.oracle.com/javase/specs/jls/se25/html/jls-12.html#jls-12.4.1
    title: The Java Language Specification, Java SE 25 Edition
    locator: 12.4.1 When Initialization Occurs
  - url: https://timsong-cpp.github.io/cppwp/n4950/stmt.dcl#3
    title: Working Draft, Programming Languages, C++ (N4950)
    locator: "[stmt.dcl] paragraph 3 (block variables with static storage duration)"
  - url: https://pkg.go.dev/sync#OnceValue
    title: Package sync, the Go standard library
    locator: type Once; func OnceValue
  - url: https://doc.rust-lang.org/std/sync/struct.LazyLock.html
    title: LazyLock, The Rust Standard Library
    locator: Description; Poisoning
  - url: https://doc.rust-lang.org/std/sync/struct.OnceLock.html
    title: OnceLock, The Rust Standard Library
    locator: Description
  - book: isbn:9780134685991
    title: Joshua Bloch, Effective Java, third edition, Addison-Wesley, 2018
    locator: Item 83, Use lazy initialization judiciously
  - book: isbn:9780321127426
    title: Martin Fowler, Patterns of Enterprise Application Architecture, Addison-Wesley, 2002
    locator: Chapter 11, Object-Relational Behavioral Patterns, Lazy Load
  - url: https://github.com/golang/go/blob/go1.25.0/src/mime/type.go#L72-L105
    title: Go go1.25.0 source, src/mime/type.go
    locator: once, initMime and TypeByExtension, lines 72 to 105
---

## Intent

Delay the creation of an object, or the computation of a value, until the first time a caller needs it. Then keep the result, so that every later use gets the same object without the cost.

## Problem

A `Settings` object reads a configuration file. The read is slow, and many runs of the program never look at the configuration. If the constructor reads the file, every run pays for the read, and the start of the program is slower than it needs to be.

The .NET documentation gives the general case: defer the creation of a large or resource-intensive object, or a resource-intensive task, when it might not happen during the life of the program at all.[^1] The forces are these:

- The cost of the value is high, and some runs of the program do not need it.
- The value does not change after it is made, so one copy can serve all callers.
- The callers should not need to know whether the value exists yet.

## Solution

Replace the field with an accessor. The accessor checks whether the value exists. If it does not, the accessor computes it and stores it. Then it returns the stored value.

```text
 Caller                 Settings.config()                       load()
 ------                 -----------------                       ------
 config() -----------> stored? no -> lock -> stored? no -> --->  slow read
                                                  store result <-
            <--------- return stored value
 config() -----------> stored? yes -> return stored value
```

The participants are the **holder**, which owns the stored value and a flag or a lock, and the **initializer**, which computes the value. Most languages now have a holder in the standard library:

| Language | Holder | Thread-safe |
|---|---|---|
| Python | `functools.cached_property`; `functools.cache` for a function[^2] | no, the getter can run more than once[^2] |
| JavaScript | a getter that replaces itself with a data property[^3] | one thread per agent |
| Java | a nested holder class for a static field[^4][^5] | yes, by class initialization |
| C# | `System.Lazy<T>`[^1] | yes, in the default mode[^1] |
| C++ | a function-local `static`[^6] | yes, other threads wait[^6] |
| Go | `sync.Once`, `sync.OnceValue`[^7] | yes |
| Rust | `std::sync::OnceLock`, `std::sync::LazyLock`[^8][^9] | yes; `OnceCell` and `LazyCell` are single-thread |

**Thread safety.** Two threads can both see that the value is missing, and both run the initializer. The result is two loads, or two objects where the program wanted one. A lock around the check and the store fixes this, but a lock on every read is slow. **Double-checked locking** reads without the lock first, and takes the lock only when the value is missing, then checks again.

The Double-Checked Locking is Broken declaration showed that this idiom does not work reliably in Java without more synchronization.[^4] Another thread can see the new reference before it sees the writes that built the object. Under the memory model of JDK 5 and later, a `volatile` field fixes the idiom, because a write to a volatile field cannot be reordered with an earlier read or write.[^4] The Java example uses this form.

For a static value, Java has a simpler form. The language initializes a class just before the first use of one of its static fields, and class initialization is thread-safe.[^5] So a nested class with one static field is a lazy holder with no lock in the code. The declaration gives this as the solution for static singletons.[^4] Effective Java recommends this lazy initialization holder class idiom for a static field, and double-checked locking for an instance field.[^10]

C++ gives the same guarantee for a function-local `static`. If a thread reaches the declaration while another thread is initializing the variable, it waits until the initialization completes.[^6]

## Example

The example files show one program in each language. `Settings` loads its configuration on the first call. The program reads the value twice, and the load runs once. Then eight threads ask a new `Settings` for its value at the same time, and the load still runs once:

| Language | Form |
|---|---|
| Python | a property with double-checked locking and `threading.Lock` |
| JavaScript | a self-overwriting getter, and a cached promise for the async case |
| TypeScript | a generic `lazy<T>` closure, and a cached promise |
| Java | double-checked locking with a `volatile` field |
| C# | `Lazy<T>` in its default thread-safe mode |
| C++ | `std::call_once` with a `std::once_flag` |
| Go | `sync.OnceValue` |
| Rust | `OnceLock::get_or_init` |

In JavaScript, a synchronous value has no race, but an asynchronous load does. A second caller can ask while the first load is still running. The fix is to store the promise, not the result, so that every caller waits for the same load.

## Consequences

The benefits are a faster start and less work. A value that no caller needs is never computed, and a value that many callers need is computed once.

The costs:

- **The first caller pays.** The cost moves from the start of the program to the first use, which can be in the middle of a request that must be fast. If that is a problem, initialize the value at the start.
- **Every access pays a check.** Each read tests a flag, and a thread-safe holder may need a memory barrier. Effective Java warns that lazy initialization lowers the cost of creating the object but raises the cost of each access, and that in most cases normal initialization is better.[^10]
- **Errors come later.** A bad configuration file is found at the first use, not at the start. The holder must also decide what a failed initialization means. `Lazy<T>` with a factory caches the exception: if the factory throws the first time, every later read of `Value` throws the same exception.[^1] Go's `sync.OnceValue` panics with the same value on every call if the function panics.[^7] Rust's `LazyLock` is poisoned if its closure panics, and later accesses panic.[^8]
- **Order becomes hidden.** The program no longer says when the value is made. A lazy value that depends on another lazy value can form a cycle, and in C++ a recursive entry into the initialization of a `static` is undefined behaviour.[^6]

**The marker of "not yet" must not be a valid value.** A hand-written holder often uses `None` or `null` for "not computed". If the initializer can return that value, the holder computes it again on every call:

```python wrong id=none-sentinel
class Settings:
    def __init__(self):
        self._proxy = None
        self.loads = 0

    @property
    def proxy(self):
        if self._proxy is None:  # None means "not loaded", but it is also a result
            self.loads += 1
            self._proxy = None  # no proxy is configured
        return self._proxy


s = Settings()
for _ in range(3):
    s.proxy
print("loads:", s.loads)
```

```output for=none-sentinel
loads: 3
```

Use a separate flag, a private sentinel object, or a holder from the standard library. `functools.cached_property` stores the result as a normal attribute, so it works for `None`:[^2]

```python run id=cached-property
from functools import cached_property


class Settings:
    def __init__(self):
        self.loads = 0

    @cached_property
    def proxy(self):
        self.loads += 1
        return None  # no proxy is configured


s = Settings()
for _ in range(3):
    s.proxy
print("loads:", s.loads)
```

```output for=cached-property
loads: 1
```

## When not to use it

Do not make a value lazy when it is cheap, or when every run needs it. The check on each access is a cost with no gain. Effective Java gives the rule as: do not do it unless you need to.[^10]

Do not use lazy initialization to hide a slow start that should be fixed. If the first request of every user waits for the load, the program is not faster, only less predictable.

Do not write your own thread-safe holder when the language has one. Double-checked locking is easy to get wrong, and the declaration shows that its correctness depends on the memory model of the language.[^4]

## Variants and related patterns

**Lazy Load.** Fowler's Lazy Load is an object that does not contain all of the data you need but knows how to get it.[^11] It applies lazy initialization to data from a database, in four forms: lazy initialization, virtual proxy, value holder and ghost.

**Virtual proxy.** A [[proxy-pattern]] that stands in for a costly object and creates it on the first call. The caller sees the interface of the real object, not a holder.

**Memoization.** `functools.cache` makes a function lazy for each argument: the first call with an argument computes the result, and later calls return the stored result.[^2] Lazy initialization is memoization of a function with no arguments.

**Lazy singleton.** A [[singleton-pattern]] that creates its instance on first access is lazy initialization of a global. The holder class idiom in Java and a function-local `static` in C++ are the safe forms.

**Lazy evaluation.** Some languages delay the evaluation of every expression until its value is needed. Lazy initialization is the same idea for one value, written by hand in a language that evaluates eagerly.

## Known uses

- Go: [mime/type.go at go1.25.0](https://github.com/golang/go/blob/go1.25.0/src/mime/type.go#L72-L105) builds its table of MIME types lazily. A package-level `sync.Once` guards `initMime`, and `TypeByExtension` calls `once.Do(initMime)` before it reads the table, so a program that never asks for a MIME type never builds it.[^12]

## References
