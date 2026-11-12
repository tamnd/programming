---
tag: Z2NCVX
kind: pattern
slug: multiton-pattern
name: Multiton
aliases: [multiton pattern, registry of singletons, keyed singleton, instance per key]
area: software-design/creational-patterns
level: advanced
summary: >-
  A multiton keeps one instance of a class for each key. A static method returns the instance for a key, creates it on the first request and returns the same object later.
family: creational-design
polarity: positive
intent: "Ensure that a class has one instance for each key, and give a global point of access that returns the instance for a key."
also_known_as: [Registry of Singletons]
history:
  year: 1994
  people: [Erich Gamma, Richard Helm, Ralph Johnson, John Vlissides]
  note: Design Patterns (1994) does not name the multiton, but its Singleton pattern describes a registry of singletons that maps names to instances, so that a program can select a subclass of a singleton by name.
see_also: [singleton-pattern, static-factory-method, flyweight-pattern, object-pool-pattern, lazy-initialization]
sources:
  - url: https://en.wikipedia.org/wiki/Multiton_pattern
    title: Multiton pattern, Wikipedia
    locator: Introduction; Description; Drawbacks; Implementations
  - book: isbn:9780201633610
    title: Erich Gamma, Richard Helm, Ralph Johnson and John Vlissides, Design Patterns, Elements of Reusable Object-Oriented Software, Addison-Wesley, 1994
    locator: Singleton, Implementation (subclassing the Singleton class, a registry of singletons) and Consequences
  - url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/util/concurrent/ConcurrentHashMap.html
    title: ConcurrentHashMap, Java SE 25 API
    locator: computeIfAbsent
  - url: https://learn.microsoft.com/en-us/dotnet/api/system.collections.concurrent.concurrentdictionary-2.getoradd
    title: ConcurrentDictionary<TKey,TValue>.GetOrAdd Method, .NET API reference
    locator: Remarks
  - url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/util/Currency.html
    title: Currency, Java SE 25 API
    locator: Class description; getInstance(String)
  - url: https://docs.python.org/3.14/library/weakref.html#weakref.WeakValueDictionary
    title: weakref, The Python Standard Library, Python 3.14
    locator: Introduction; WeakValueDictionary
  - url: https://docs.oracle.com/javase/specs/jls/se25/html/jls-8.html#jls-8.9
    title: The Java Language Specification, Java SE 25 Edition
    locator: 8.9 Enum Classes
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Symbol/for
    title: Symbol.for(), MDN Web Docs
    locator: Introduction; Description
  - url: https://github.com/openjdk/jdk/blob/jdk-25%2B36/src/java.base/share/classes/java/util/Currency.java#L303-L355
    title: OpenJDK jdk-25+36 source, java/util/Currency.java
    locator: getInstance(String, int, int), lines 303 to 355; the instances map, line 144
  - url: https://github.com/python/cpython/blob/v3.14.0/Lib/logging/__init__.py#L1362-L1391
    title: CPython v3.14.0 source, Lib/logging/__init__.py
    locator: Manager.getLogger, lines 1362 to 1391
  - url: https://docs.python.org/3.14/library/logging.html#logging.getLogger
    title: logging, The Python Standard Library, Python 3.14
    locator: logging.getLogger
---

## Intent

Ensure that a class has one instance for each key, and give a global point of access that returns the instance for a key. The multiton generalizes the [[singleton-pattern]]: a singleton has one instance for the program, and a multiton has one instance for each key.[^1]

## Problem

A program works with money in several currencies. Each currency has a code and a number of decimal places: two for the US dollar, none for the yen. The program compares currencies often, and it wants `usd == usd` to be a test of identity, not of fields. It also wants to check the code once, when the currency is made, and not at each use.

If each caller creates its own `Currency` object, the program has many objects for the dollar, and identity means nothing. A single global object does not help, because there are many currencies. The forces are these:

- The class has many instances, but at most one for each key.
- The callers ask by key, often from many places and many threads.
- The set of keys is not known in full when the program starts, or it is too large to create in advance.

## Solution

Make the constructor private. Keep a static map from key to instance, and give the class a static method that takes a key. The method looks up the key. If an instance is there, it returns it. If not, it checks the key, creates the instance, stores it and returns it.[^1]

```text
 Caller                    Currency.of(code)                 registry (map)
 ------                    -----------------                 --------------
 of("USD") ------------->  lock; look up "USD" ----------->  absent
                           valid code? create, store ---->  "USD" -> #1
           <-------------  return #1
 of("USD") ------------->  lock; look up "USD" ----------->  "USD" -> #1
           <-------------  return #1 (the same object)
 of("XYZ") ------------->  not a valid code -> error
```

A multiton differs from a plain map in two ways. Clients cannot add entries, and a request for a key never returns an empty result: the multiton creates the instance on the first request, and returns the same instance for later requests with that key.[^1]

Design Patterns does not describe the multiton as a pattern of its own.[^1] Its Singleton pattern comes close in two places. A registry of singletons maps names to instances, so that the program can pick a subclass by name. And one of the consequences of Singleton is that it permits a variable number of instances, because the access method controls how many exist.[^2]

**Thread safety.** Two threads can both find the key absent, and both create an instance. The check and the insert must be one step. A lock around both is the simple form. A concurrent map can do it without a lock in the code, but the details differ by library. Java's `ConcurrentHashMap.computeIfAbsent` performs the whole call atomically, and calls the function once if the key is absent.[^3] .NET's `ConcurrentDictionary.GetOrAdd` adds only one pair for a key, but its factory may be called more than once when threads race,[^4] so the factory must not have side effects, or the value must be a `Lazy<T>`.

## Example

The example files show one `Currency` class in each language. `of(code)` returns the one instance for a code. The program asks for the dollar twice and gets the same object, asks for an unknown code and gets an error, and then eight threads ask for the euro at the same time and get one object:

| Language | Registry | Thread safety |
|---|---|---|
| Python | a class-level `dict` and a `classmethod` | `threading.Lock` |
| JavaScript | a static private `Map` | one thread per agent, so no lock |
| TypeScript | a private static `Map`, keys as a union type | one thread per agent, so no lock |
| Java | a `ConcurrentHashMap` | `computeIfAbsent`[^3] |
| C# | a `ConcurrentDictionary` of `Lazy<Currency>` | `GetOrAdd`[^4] |
| C++ | a static `std::map` of `std::unique_ptr` | `std::mutex` |
| Go | a package-level map of pointers | `sync.Mutex` |
| Rust | `LazyLock<Mutex<HashMap<_, Arc<_>>>>` | `Mutex` |

## Consequences

The benefits:

- **Identity per key.** Callers can compare instances with `==` or `is`, and two parts of the program that ask for the same key share one object and its state.
- **Validation in one place.** The access method checks the key once. Java's `Currency` has no public constructor, and `getInstance` throws `IllegalArgumentException` if the code is not a supported ISO 4217 code.[^5]
- **Lazy creation.** An instance is created only when a caller first asks for its key.

The costs are those of Singleton, once for each key. A multiton is global state, and global state makes unit tests harder: one test can change an instance that the next test gets.[^1]

**The registry holds every instance for ever.** In a language with garbage collection, the registry keeps a strong reference to each instance, so no instance is ever collected.[^1] This is harmless when the keys are a small fixed set, such as currencies. It is a leak when the keys come from input. Here each request uses a new key, and the registry keeps all of them:

```python wrong id=unbounded
class Tag:
    _instances = {}

    @classmethod
    def of(cls, name):
        if name not in cls._instances:
            cls._instances[name] = cls()
        return cls._instances[name]


for i in range(100_000):
    Tag.of(f"request-{i}")  # a new key for each request
print("instances kept:", len(Tag._instances))
```

```output for=unbounded
instances kept: 100000
```

One fix is to validate the key against a fixed set, as the `Currency` example does. Another is to hold the instances weakly. Python's `WeakValueDictionary` discards an entry when no strong reference to the value exists any more.[^6] The weakref documentation gives this use: a cache or mapping that should not keep a large object alive only because the object is in the mapping.[^6]

```python run id=weak-registry
import weakref


class Tag:
    _instances = weakref.WeakValueDictionary()

    @classmethod
    def of(cls, name):
        tag = cls._instances.get(name)
        if tag is None:
            tag = cls()
            cls._instances[name] = tag
        return tag


for i in range(100_000):
    Tag.of(f"request-{i}")
print("instances kept:", len(Tag._instances))
admin = Tag.of("admin")
print("same object while in use:", admin is Tag.of("admin"))
print("instances kept:", len(Tag._instances))
```

```output for=weak-registry
instances kept: 0
same object while in use: True
instances kept: 1
```

A weak registry changes the guarantee. There is one instance for each key while some code holds it. After the last holder lets go, the next request creates a new object, so any state in the old object is lost.

## When not to use it

Do not use a multiton when the instances are values with no identity. If two `Currency` objects with the same code are equal by their fields, a [[static-factory-method]] that returns a new or cached object, with an equality method, is enough.

Do not use it when the set of keys is fixed and known at compile time. An enum is simpler. In Java, an enum class has no instances other than those defined by its enum constants,[^7] so the language gives one instance per key with no registry.[^1]

Do not use it to reach shared services from everywhere. Pass the instance that a component needs to the component. The registry then belongs to the code that builds the program, and tests can give each component its own instance.

## Variants and related patterns

**Singleton.** A multiton with one key is a [[singleton-pattern]]. An enum with one constant is the same in Java.[^1]

**Registry of singletons.** In Design Patterns, the registry maps a name to the one instance of a subclass, so that the program can choose the class of its singleton by name, for example from an environment variable.[^2]

**Interning.** The registry of a multiton can be in the language. `Symbol.for(key)` in JavaScript searches a runtime-wide symbol registry for the key, returns the symbol if it is there, and otherwise creates a new symbol in the registry.[^8]

**Flyweight.** A [[flyweight-pattern]] factory also returns one shared object for each key. The difference is the purpose. A flyweight is immutable and saves memory when there are many equal objects. A multiton instance can have state, and its purpose is one identity for each key.

**Object pool.** An [[object-pool-pattern]] gives a client any free object, and takes it back after use. A multiton gives every client the same object for a key, and never takes it back.

## Known uses

- Java: [java/util/Currency.java at jdk-25+36](https://github.com/openjdk/jdk/blob/jdk-25%2B36/src/java.base/share/classes/java/util/Currency.java#L303-L355). The class keeps a static `ConcurrentMap` named `instances`. `getInstance` returns the entry for the code if there is one. If not, it checks the code, builds a new `Currency` and stores it with `putIfAbsent`, so that two threads that race get the same object.[^9] The API documentation says that there is never more than one `Currency` instance for any given currency.[^5]
- Python: [Lib/logging/__init__.py at v3.14.0](https://github.com/python/cpython/blob/v3.14.0/Lib/logging/__init__.py#L1362-L1391). `Manager.getLogger` takes the module lock, looks up the name in `loggerDict`, and creates and stores a new `Logger` if the name is not there.[^10] The documentation says that multiple calls to `getLogger()` with the same name will always return a reference to the same `Logger` object.[^11]

## References
