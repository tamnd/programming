---
tag: NEW-decorator-pattern
kind: pattern
slug: decorator-pattern
name: Decorator pattern
aliases: [Decorator, wrapper]
area: software-design/structural-patterns
level: core
summary: >-
  A decorator wraps an object, has the same interface, and adds behaviour before or after it forwards each call. Decorators stack, so features combine at run time without a subclass for each mix.
family: structural-design
polarity: positive
intent: "Add a responsibility to one object at run time by wrapping it in an object with the same interface, as a flexible alternative to subclassing."
also_known_as: [Wrapper]
languages_where_built_in: {kotlin: kotlin/class-delegation}
refactorings: [move-embellishment-to-decorator]
history:
  year: 1994
  people: [Erich Gamma, Richard Helm, Ralph Johnson, John Vlissides]
  note: Gamma, Helm, Johnson and Vlissides named and described the pattern in Design Patterns (1994), with Wrapper as a second name. The Python decorator syntax of PEP 318 took the same name for a different construct.
see_also: [adapter-pattern, strategy-pattern, closures, observer-pattern]
sources:
  - book: isbn:9780201633610
    title: Gamma, Helm, Johnson and Vlissides, Design Patterns, Elements of Reusable Object-Oriented Software, Addison-Wesley, 1994
    locator: Chapter 4, Decorator, page 175 (Intent, Also Known As, Motivation, Participants, Consequences, Implementation, Known Uses, Related Patterns)
  - book: isbn:9780321213358
    title: Joshua Kerievsky, Refactoring to Patterns, Addison-Wesley, 2004
    locator: Chapter 7, Simplification, Move Embellishment to Decorator (problem and solution)
  - url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/io/FilterInputStream.html
    title: Java SE 25 API, java.io.FilterInputStream
    locator: class description
  - url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/io/BufferedInputStream.html
    title: Java SE 25 API, java.io.BufferedInputStream
    locator: class description
  - url: https://docs.python.org/3.14/library/io.html#io.BufferedReader
    title: The Python Standard Library, io, Python 3.14
    locator: io.BufferedReader
  - url: https://pkg.go.dev/net/http#StripPrefix
    title: Go standard library, package net/http
    locator: func StripPrefix
  - url: https://go.dev/ref/spec#Struct_types
    title: The Go Programming Language Specification
    locator: Struct types (embedded fields and promoted methods)
  - url: https://doc.rust-lang.org/std/io/struct.BufReader.html
    title: Rust standard library, std::io::BufReader
    locator: struct description
  - url: https://doc.rust-lang.org/std/iter/index.html#adapters
    title: Rust standard library, module std::iter
    locator: Adapters
  - url: https://learn.microsoft.com/en-us/dotnet/api/system.io.bufferedstream
    title: .NET API reference, System.IO.BufferedStream
    locator: class description
  - url: https://kotlinlang.org/docs/delegation.html
    title: Kotlin documentation, Delegation
    locator: Implementation by delegation; Overriding a member of an interface implemented by delegation
  - url: https://peps.python.org/pep-0318/#on-the-name-decorator
    title: PEP 318, Decorators for Functions and Methods
    locator: On the name 'Decorator'
  - url: https://docs.python.org/3.14/library/functools.html#functools.wraps
    title: The Python Standard Library, functools, Python 3.14
    locator: functools.update_wrapper and functools.wraps
  - url: https://docs.python.org/3.14/library/functools.html#functools.cache
    title: The Python Standard Library, functools, Python 3.14
    locator: functools.cache and functools.lru_cache
  - url: https://github.com/tc39/proposal-decorators
    title: TC39 proposal, Decorators
    locator: README, Detailed design
  - url: https://www.typescriptlang.org/docs/handbook/decorators.html
    title: TypeScript Handbook, Decorators
    locator: Decorators (experimental decorators and the compiler option)
  - url: https://github.com/openjdk/jdk/blob/jdk-25%2B36/src/java.base/share/classes/java/io/FilterInputStream.java#L45-L72
    title: OpenJDK jdk-25+36 source, java/io/FilterInputStream.java
    locator: the field in, the constructor and read, lines 45 to 72
  - url: https://github.com/golang/go/blob/go1.27.3/src/net/http/server.go#L2406-L2425
    title: Go go1.27.3 source, src/net/http/server.go
    locator: StripPrefix, lines 2406 to 2425
  - book: isbn:9780201633610
    title: Gamma, Helm, Johnson and Vlissides, Design Patterns, Elements of Reusable Object-Oriented Software, Addison-Wesley, 1994
    locator: Chapter 4, Proxy, page 207 (Intent, Applicability, Related Patterns)
---

## Intent

Add a responsibility to one object at run time. Wrap the object in another object with the same interface, which does its own work and forwards the call. This is a flexible alternative to a subclass.[^1]

## Problem

A repository loads values by key from a slow store. Some callers need the calls logged. Some need the results cached. Some need both, and a test needs neither. Later someone will ask for retries and for a timer.

The direct design adds a subclass for each feature: `LoggedStore`, `CachedStore`. But the features combine. Logging and caching together need a third class, and with retries and a timer the number of classes grows with each combination. A subclass also fixes the feature when the code is written. It cannot add caching to one store object at run time. Nor can it add caching to a class that the program did not write, such as a class from a library or a `final` class.

The other direct design puts all the features in the store, behind flags. Then every store carries the code for every feature, and the store's main job, loading values, is hard to find. Kerievsky describes this starting point: code adds an embellishment to the core responsibility of a class.[^2]

The forces are these. Features must combine in any order and number. They must be added to single objects, at run time. The code that uses the object must not know which features are there. The core class must not change.

## Solution

Make each feature a class that implements the same interface as the core object and holds a reference to a component of that interface. Each operation does its feature work and forwards the call to the component.[^1]

```text
 Component (Repository)  <----------------------------+
 ----------------------                               |
 get(key)                                             | holds one
 name()                                               |
      ^                                               |
      |                                               |
 +----+--------+---------------------+                |
 |             |                     |                |
 Store    Logged(inner)         Cached(inner) --------+
          get: log, then        get: if in cache, return it;
               inner.get(key)        else inner.get(key), store it
          name: inner.name()    name: inner.name()

 Logged(Cached(Store())).get("a")
   Logged.get --> Cached.get --> Store.get
```

The participants in Design Patterns are the Component interface, the ConcreteComponent, the Decorator, which holds the component and forwards to it, and the ConcreteDecorators.[^1] A client builds a chain at run time: `Logged(Cached(Store()))`. Each decorator sees only the interface of the next object, so any decorator can wrap any component or any other decorator.

The forwarding has two parts. Every operation of the interface must forward, even the ones that the decorator does not change. The decorator can do its work before the call, as with logging or a check. It can work after the call, as when it stores a result. Or it can work in place of the call, as when it returns a cached result.

**Streams are the classic case.** Design Patterns names streams as a known use.[^1] `java.io.FilterInputStream` wraps some other input stream, which it uses as its basic source of data, and passes requests to the wrapped stream. `BufferedInputStream` is one of its subclasses, and adds a buffer to another input stream.[^3][^4] Python `io.BufferedReader` gives buffered access to a raw binary stream. .NET `BufferedStream` adds a buffer to another stream, and Rust `BufReader` adds buffering to any reader.[^5][^10][^8] Go HTTP middleware has the same shape: `http.StripPrefix` takes a handler and returns a handler that changes the request path and calls the first.[^6]

**In languages with first-class functions, a decorator of one operation is a higher-order function.** It takes a function and returns a function with the same signature. Python `functools.cache` is a decorator in this sense: it wraps a function and stores its results.[^14] When the interface has many operations, the class form is still needed, because each operation must forward.

## Example

The example files show one program in each language. The store prints `store: load a` and returns `value-a`. The chain is `Logged(Cached(Store()))`. The program calls `get("a")` twice and `get("b")` once, and then prints `name()`. The second call for `a` is logged but does not reach the store. The decorators do not change `name()`, so they forward it, and it prints `store`.

| Language | Form |
|---|---|
| Python | classes with a `typing.Protocol` for the interface |
| JavaScript | classes that hold the inner object |
| TypeScript | an interface and classes that implement it |
| Java | an interface and final classes, as in `java.io` |
| Kotlin | class delegation with `by`, so only `get` is written by hand |
| C# | an interface and sealed classes |
| Go | a struct that embeds the interface, so `Name` is promoted |
| Rust | generic wrappers over a trait |
| C++ | an abstract class and wrappers that own the inner object through `std::unique_ptr` |

## Consequences

Design Patterns lists these benefits.[^1] Decorators are more flexible than static inheritance: a client can add and remove features at run time, and can use one feature twice. They avoid classes high up in a hierarchy that carry every feature. A program pays only for the features it uses.

The same chapter lists these costs.[^1] A decorator and its component are not the same object, so code that depends on object identity breaks. A design with many decorators has many small objects that look alike, and it is hard to learn and to debug.

**Identity and type checks.** The client holds the outer decorator. A check on the identity or the concrete type of the store fails on it:

```python run id=identity
class Store:
    def get(self, key):
        return f"value-{key}"


class Cached:
    def __init__(self, inner):
        self.inner = inner
        self.cache = {}

    def get(self, key):
        if key not in self.cache:
            self.cache[key] = self.inner.get(key)
        return self.cache[key]


store = Store()
repo = Cached(store)
print(repo.get("a"))
print(f"repo is store: {repo is store}")
print(f"isinstance(repo, Store): {isinstance(repo, Store)}")
```

```output
value-a
repo is store: False
isinstance(repo, Store): False
```

Code that uses a decorated object must use the interface only. A check such as `isinstance(repo, Store)` turns off the decorators without an error.

**The order of the decorators changes the behaviour.** `Logged(Cached(store))` logs every call. `Cached(Logged(store))` logs only the calls that miss the cache, because the cache answers the others before the logger sees them:

```python run id=order
class Store:
    def get(self, key):
        return f"value-{key}"


class Logged:
    def __init__(self, inner):
        self.inner = inner

    def get(self, key):
        print(f"  log: get {key}")
        return self.inner.get(key)


class Cached:
    def __init__(self, inner):
        self.inner = inner
        self.cache = {}

    def get(self, key):
        if key not in self.cache:
            self.cache[key] = self.inner.get(key)
        return self.cache[key]


for name, repo in [("Logged(Cached(store))", Logged(Cached(Store()))),
                   ("Cached(Logged(store))", Cached(Logged(Store())))]:
    print(name)
    for key in ["a", "a", "b"]:
        repo.get(key)
```

```output
Logged(Cached(store))
  log: get a
  log: get a
  log: get b
Cached(Logged(store))
  log: get a
  log: get b
```

Neither order is wrong. Each order means something different, so the code that builds the chain must state the order on purpose.

**Every operation must forward.** A decorator that forgets an operation of the interface breaks the chain. A static type checker finds this in Java, C#, Go, Rust and TypeScript, because the decorator does not implement the interface. In Python the error comes at run time, on the first call:

```python fails expect=exception:AttributeError id=missing-forward
class Store:
    def get(self, key):
        return f"value-{key}"

    def name(self):
        return "store"


class Logged:
    def __init__(self, inner):
        self.inner = inner

    def get(self, key):
        print(f"log: get {key}")
        return self.inner.get(key)


repo = Logged(Store())
print(repo.get("a"))
print(repo.name())
```

The run stops with `AttributeError: 'Logged' object has no attribute 'name'`. A large interface makes each decorator long, because each operation needs one line that forwards. Kotlin writes those lines for the programmer with `by`, and a Go struct that embeds the interface promotes the methods of the inner value.[^11][^7] In Python, a `__getattr__` method that returns `getattr(self.inner, name)` forwards every attribute that the decorator does not define.

**Forwarding is not delegation.** When a method of the inner object calls another of its own methods, it calls it on itself, not on the decorator. The decorator does not see that call. Here `get_many` is a method of the store, and the cache does not apply to the calls that it makes:

```python wrong id=self-call
class Store:
    def get(self, key):
        print(f"store: load {key}")
        return f"value-{key}"

    def get_many(self, keys):
        return [self.get(key) for key in keys]  # self is the store, not the decorator


class Cached:
    def __init__(self, inner):
        self.inner = inner
        self.cache = {}

    def get(self, key):
        if key not in self.cache:
            self.cache[key] = self.inner.get(key)
        return self.cache[key]

    def get_many(self, keys):
        return self.inner.get_many(keys)


repo = Cached(Store())
print(repo.get_many(["a", "a"]))
```

```output
store: load a
store: load a
['value-a', 'value-a']
```

The Kotlin documentation states the same rule for `by`. Members that the delegating class overrides are not called from the members of the delegate object. The delegate can call only its own implementations.[^11] The fix is to put the loop in the decorator, or in a function outside both, so that each `get` goes through the chain.

## When not to use it

Do not use the pattern for a feature that every instance needs. Put it in the class, or in a base class. A decorator that is always present is a cost with no flexibility in return.

Do not use it when the interface is large and the decorator changes one operation. Each decorator must forward every other operation, and each forward is a place for a bug. A smaller interface, or a hook in the component, is often better. Design Patterns notes this: when the component class is heavy, Strategy is a better choice, because the decorators would be too costly.[^1]

Do not use it when code depends on the concrete type or the identity of the object. Examples are equality on objects, a registry keyed by the object, and a check of the concrete type.

Do not build long chains in many places. Build the chain in one place, where the program wires its objects, so a reader can find the order.

## Variants and related patterns

**Function decorators in Python.** The `@name` syntax of Python applies a function to a function or a class at definition time. PEP 318 records that the name was criticized because it is not consistent with its use in the Design Patterns book. It adds that the name probably owes more to its use in the compiler area.[^12] A decorator function that returns a wrapper with the same signature is the pattern applied to one function. `functools.wraps` copies the name, the docstring and other attributes of the wrapped function to the wrapper, and sets `__wrapped__`:[^13]

```python run id=function-decorator
import functools


def logged(function):
    @functools.wraps(function)
    def wrapper(*args):
        print(f"log: {function.__name__}{args}")
        return function(*args)

    return wrapper


@logged
@functools.cache
def load(key):
    print(f"store: load {key}")
    return f"value-{key}"


print(load("a"))
print(load("a"))
print(load.__name__, load.__wrapped__.__wrapped__.__name__)
```

```output
log: load('a',)
store: load a
value-a
log: load('a',)
value-a
load load
```

**Decorators in JavaScript and TypeScript.** The TC39 decorators proposal and the TypeScript decorators are syntax. They run a function on a class, a method or a field when the class is defined.[^15][^16] Like the Python syntax, they can implement the pattern, but they are a language feature, not the pattern itself.

**Adapter.** An adapter changes the interface of an object. A decorator keeps the interface and changes the behaviour.[^1] The [[adapter-pattern]] page shows the contrast. Rust calls its iterator wrappers, such as `map` and `filter`, adapters, and its documentation describes them as a form of the adapter pattern.[^9] By structure they are closer to decorators, because each one takes an iterator and returns an iterator.

**Composite.** Design Patterns describes a decorator as a degenerate composite with one component. A decorator adds responsibilities, and a composite aggregates objects.[^1]

**Strategy.** Design Patterns contrasts the two: a decorator changes the skin of an object, and a strategy changes its guts.[^1] A decorator works from the outside, and the component does not know about it. A [[strategy-pattern|strategy]] is a part that the component knows and calls.

**Proxy.** A proxy has the same structure, an object with the same interface that forwards. Its purpose is to control access to the object, for example to create it late or to reach it over a network. It does not add features that a client chooses.[^19]

**Middleware.** A web server can pass a request through a list of handlers, each of which can act and then call the next. That list is a chain of decorators of one operation.[^6]

## Known uses

- OpenJDK: [FilterInputStream.java at jdk-25+36](https://github.com/openjdk/jdk/blob/jdk-25%2B36/src/java.base/share/classes/java/io/FilterInputStream.java#L45-L72) holds the wrapped stream in the field `in`, sets it in the constructor, and forwards `read()` to `in.read()`. `BufferedInputStream`, `DataInputStream` and the other filter streams extend it.[^17]
- Go: [StripPrefix in server.go at go1.27.3](https://github.com/golang/go/blob/go1.27.3/src/net/http/server.go#L2406-L2425) returns a `HandlerFunc`. It removes a prefix from the path of a copy of the request and calls the wrapped handler.[^18]

## References
