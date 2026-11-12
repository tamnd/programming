---
tag: D8XWAX
kind: pattern
slug: marker-interface
name: Marker interface
aliases: [marker interface pattern, tagging interface, tag interface, marker trait]
area: software-design/structural-patterns
level: advanced
summary: >-
  A marker interface is an interface with no methods. A class implements it only to say that it has a property, and code that receives an object tests for the interface.
family: structural-design
polarity: positive
intent: "Attach a property to a class with an interface that declares no members, so that code can test for the property with a type check at run time or with a type bound at compile time."
also_known_as: [Tagging interface]
history:
  year: 2008
  people: [Joshua Bloch]
  note: Cloneable has been in Java since version 1.0, Serializable since 1.1 and RandomAccess since 1.4. Bloch gave the rule Use marker interfaces to define types as Item 37 of the second edition of Effective Java (2008), and compared marker interfaces with marker annotations.
see_also: [annotations, interfaces, runtime-reflection, prototype-pattern]
sources:
  - url: https://en.wikipedia.org/wiki/Marker_interface_pattern
    title: Marker interface pattern, Wikipedia
    locator: Introduction; Example; Critique
  - url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/io/Serializable.html
    title: Serializable, Java SE 25 API
    locator: interface description
  - url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/lang/Cloneable.html
    title: Cloneable, Java SE 25 API
    locator: interface description
  - url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/util/RandomAccess.html
    title: RandomAccess, Java SE 25 API
    locator: interface description
  - url: https://learn.microsoft.com/en-us/dotnet/fundamentals/code-analysis/quality-rules/ca1040
    title: "CA1040: Avoid empty interfaces, Microsoft Learn"
    locator: Rule description; How to fix violations
  - url: https://go.dev/ref/spec#Interface_types
    title: The Go Programming Language Specification
    locator: Interface types (the empty interface)
  - url: https://github.com/golang/go/blob/go1.25.0/src/go/ast/ast.go#L41-L45
    title: Go go1.25.0 source, src/go/ast/ast.go
    locator: the Expr interface and its exprNode method
  - url: https://typescript-eslint.io/rules/no-empty-object-type/
    title: no-empty-object-type, typescript-eslint
    locator: rule description
  - book: isbn:9780134685991
    title: Joshua Bloch, Effective Java, third edition, Addison-Wesley, 2018
    locator: Item 41, Use marker interfaces to define types
  - url: https://doc.rust-lang.org/std/marker/index.html
    title: Module std::marker, The Rust Standard Library
    locator: module description; Traits
  - url: https://github.com/openjdk/jdk/blob/jdk-25%2B36/src/java.base/share/classes/java/util/Collections.java#L213-L219
    title: OpenJDK jdk-25+36 source, java/util/Collections.java
    locator: binarySearch, lines 213 to 219
---

## Intent

Attach a property to a class with an interface that declares no members. The class implements the interface only to say that it has the property, and code that receives an object tests for the interface.[^1]

## Problem

A logger writes each value it receives. Some values are secrets, such as a password or an API key, and the logger must not write them. The logger cannot know every secret class, and the secret classes have nothing in common that the logger could call.

The forces are these:

- The property is a fact about the class, not a behaviour. There is no method to call.
- The code that tests the property is far from the classes that have it.
- The language has types and type checks, but perhaps no other way to attach data to a class.

## Solution

Declare an empty interface, and let each class with the property implement it. The code that needs the property tests whether an object is an instance of the interface.

```text
 interface Sensitive { }              (no members)

 class Password implements Sensitive   class Username
          |                                   |
          v                                   v
 log(value):  value instanceof Sensitive ?  "[redacted]"  :  value
```

The pattern needs a language with type information at run time, or a type system that can check the interface at compile time. It gives a way to associate metadata with a class where the language has no direct support for metadata.[^1]

The Java standard library has three well-known markers:

| Interface | What it marks |
|---|---|
| `java.io.Serializable` | the serialization interface has no methods or fields and serves only to identify the semantics of being serializable[^2] |
| `java.lang.Cloneable` | it is legal for `Object.clone()` to make a field-for-field copy; without it, `clone` throws `CloneNotSupportedException`[^3] |
| `java.util.RandomAccess` | a `List` supports fast, generally constant-time, random access[^4] |

## Example

The example files show one logger in each language. `Password` and `ApiKey` carry the marker, and `Username` does not. The logger writes each value, and writes `[redacted]` for a marked value:

```text
user: ann
password: [redacted]
api key: [redacted]
```

The marker takes a different form in each language:

| Language | Marker | Test |
|---|---|---|
| Python | an empty class | `isinstance` at run time |
| JavaScript | a `Symbol` property on the class | the `in` operator |
| TypeScript | an interface with a `unique symbol` brand | the `in` operator, which also narrows the type |
| Java | an empty interface | `instanceof` |
| C# | a custom attribute, as CA1040 advises[^5] | `GetCustomAttribute` |
| C++ | a type trait specialized for each marked type | `if constexpr` at compile time |
| Go | an interface with one unexported, empty method | a type assertion |
| Rust | a trait with no items | a trait bound at compile time |

Go and TypeScript cannot use a truly empty interface. In Go, all types implement the empty interface, so a marker needs at least one method.[^6] The `go/ast` package uses an unexported method, `exprNode()`, so that only expression nodes can be assigned to an `Expr`.[^7] In TypeScript, the empty object type `{}` means any value that is defined, because of structural typing.[^8] A brand property with a unique symbol fixes this.

## Consequences

A marker interface defines a type. Effective Java names this as its main advantage over a marker annotation: the compiler can then reject a call with an unmarked object, where an annotation is found only at run time.[^9] The test is also cheap, and the marker shows in the documentation of the class.

The costs:

- **No contract.** The interface says nothing about what the property means. `Cloneable` does not contain the `clone` method, so a class that implements it is not cloneable for that reason alone.[^3]
- **A marker cannot be removed.** A subclass inherits the interface, and it cannot unimplement it.[^1] A subclass that adds a field which cannot be serialized still passes the test:

```python wrong id=inherited-marker
import pickle
import threading


class Serializable:
    """Marker: instances can be saved."""


class Cache(Serializable):
    def __init__(self):
        self.items = {}


class LockedCache(Cache):
    def __init__(self):
        super().__init__()
        self.lock = threading.Lock()


def save(obj):
    if not isinstance(obj, Serializable):
        return f"{type(obj).__name__}: not serializable"
    try:
        pickle.dumps(obj)
        return f"{type(obj).__name__}: saved"
    except TypeError as e:
        return f"{type(obj).__name__}: {e}"


for obj in [Cache(), LockedCache()]:
    print(save(obj))
```

```output for=inherited-marker
Cache: saved
LockedCache: cannot pickle '_thread.lock' object
```

The test passed for `LockedCache`, and the save failed later. Java has the same problem: a subclass of a serializable class that must not be serialized has to throw `NotSerializableException` itself.[^1]

## When not to use it

Do not use an empty interface for a property that only code at run time reads, when the language has attributes or annotations. The .NET rule CA1040 says that an empty interface defines no contract. If the identification occurs at run time, the correct way is a custom attribute. If it must occur at compile time, an empty interface is acceptable.[^5]

Do not use a marker when the property needs a method. Declare the method in the interface, so that the type states both the property and the contract.

Do not use an empty interface in a language with structural typing. In TypeScript and Go, an interface with no members matches every value, so it marks nothing.[^8][^6]

## Variants and related patterns

**Marker annotation.** Java annotations and C# attributes attach metadata with no change to the type. Effective Java gives the rule: use a marker interface if you want to define a type, and a marker annotation for other program elements or for a framework that already uses annotations.[^9] See [[annotations]].

**Marker trait.** Rust represents basic properties of types as traits in `std::marker`, such as `Copy`, `Send`, `Sized` and `Sync`.[^10] `Send` marks types that can be transferred across thread boundaries, and the compiler uses it in trait bounds, not at run time.[^10]

**Tag dispatch.** A marker can select an algorithm. The main purpose of `RandomAccess` is to allow generic algorithms to change their behaviour for random or sequential access lists.[^4]

**Hybrid interface.** An interface can act as a marker and also declare methods, but this can confuse its readers if it is used badly.[^1]

## Known uses

- Java: [java/util/Collections.java at jdk-25+36](https://github.com/openjdk/jdk/blob/jdk-25%2B36/src/java.base/share/classes/java/util/Collections.java#L213-L219). `binarySearch` tests `list instanceof RandomAccess`. A marked list, or a small list, gets the indexed search, and other lists get the iterator search.[^11][^4]
- Go: [go/ast/ast.go at go1.25.0](https://github.com/golang/go/blob/go1.25.0/src/go/ast/ast.go#L41-L45) declares `Expr` with the marker method `exprNode()`, which each expression node implements with an empty body.[^7]

## References
