---
tag: NEW-adapter-pattern
kind: pattern
slug: adapter-pattern
name: Adapter pattern
aliases: [Adapter, wrapper]
area: software-design/structural-patterns
level: core
summary: >-
  An adapter converts the interface of an existing class into the interface that a client expects. The client and the class stay as they are, and the adapter translates each call.
family: structural-design
polarity: positive
intent: "Convert the interface of a class into another interface that clients expect, so that classes with interfaces that do not match can work together."
also_known_as: [Wrapper]
refactorings: [unify-interfaces-with-adapter, extract-adapter]
history:
  year: 1994
  people: [Erich Gamma, Richard Helm, Ralph Johnson, John Vlissides]
  note: Gamma, Helm, Johnson and Vlissides named and described the pattern in Design Patterns (1994), with Wrapper as a second name, in a class form and an object form.
see_also: [decorator-pattern, strategy-pattern, factory-method-pattern, closures]
sources:
  - book: isbn:9780201633610
    title: Gamma, Helm, Johnson and Vlissides, Design Patterns, Elements of Reusable Object-Oriented Software, Addison-Wesley, 1994
    locator: Chapter 4, Adapter (Intent, Also Known As, Motivation, Structure, Consequences, Implementation, Related Patterns)
  - book: isbn:9780321213358
    title: Joshua Kerievsky, Refactoring to Patterns, Addison-Wesley, 2004
    locator: Chapter 8, Generalization, Unify Interfaces with Adapter and Extract Adapter (problem and solution)
  - url: https://pkg.go.dev/net/http#HandlerFunc
    title: Go standard library, package net/http
    locator: type HandlerFunc
  - url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/io/InputStreamReader.html
    title: Java SE 25 API, java.io.InputStreamReader
    locator: class description
  - url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/util/Arrays.html#asList(T...)
    title: Java SE 25 API, java.util.Arrays
    locator: asList(T...)
  - url: https://docs.python.org/3.14/library/contextlib.html#contextlib.closing
    title: The Python Standard Library, contextlib, Python 3.14
    locator: contextlib.closing
  - url: https://docs.python.org/3.14/library/io.html#io.TextIOWrapper
    title: The Python Standard Library, io, Python 3.14
    locator: io.TextIOWrapper
  - url: https://nodejs.org/api/util.html#utilpromisifyoriginal
    title: Node.js documentation, Util
    locator: util.promisify(original)
  - url: https://learn.microsoft.com/en-us/dotnet/api/system.io.streamreader
    title: .NET API reference, System.IO.StreamReader
    locator: class description
  - url: https://doc.rust-lang.org/reference/items/implementations.html#orphan-rules
    title: The Rust Reference, Implementations
    locator: Orphan rules
  - url: https://doc.rust-lang.org/book/ch20-02-advanced-traits.html#implementing-external-traits-with-the-newtype-pattern
    title: The Rust Programming Language, Advanced Traits
    locator: Implementing External Traits with the Newtype Pattern
  - url: https://timsong-cpp.github.io/cppwp/n4950/container.adaptors
    title: Working Draft, Standard for Programming Language C++, N4950
    locator: "[container.adaptors] Container adaptors"
  - url: https://www.typescriptlang.org/docs/handbook/type-compatibility.html
    title: TypeScript Handbook, Type Compatibility
    locator: introduction (structural subtyping)
  - url: https://doc.rust-lang.org/std/iter/index.html#adapters
    title: Rust standard library, module std::iter
    locator: Adapters
  - book: isbn:9780201633610
    title: Gamma, Helm, Johnson and Vlissides, Design Patterns, Elements of Reusable Object-Oriented Software, Addison-Wesley, 1994
    locator: Chapter 4, Bridge (Intent, Related Patterns)
  - book: isbn:9780201633610
    title: Gamma, Helm, Johnson and Vlissides, Design Patterns, Elements of Reusable Object-Oriented Software, Addison-Wesley, 1994
    locator: Chapter 4, Facade (Intent)
  - url: https://github.com/golang/go/blob/go1.27.3/src/net/http/server.go#L2350-L2359
    title: Go go1.27.3 source, src/net/http/server.go
    locator: type HandlerFunc and its ServeHTTP method, lines 2350 to 2359
  - url: https://github.com/openjdk/jdk/blob/jdk-25%2B36/src/java.base/share/classes/java/io/InputStreamReader.java#L81-L85
    title: OpenJDK jdk-25+36 source, java/io/InputStreamReader.java
    locator: the constructor InputStreamReader(InputStream), lines 81 to 85, and read(char[], int, int), lines 182 to 184
  - url: https://github.com/openjdk/jdk/blob/jdk-25%2B36/src/java.base/share/classes/java/util/Arrays.java#L4180-L4233
    title: OpenJDK jdk-25+36 source, java/util/Arrays.java
    locator: asList and the private class ArrayList, lines 4180 to 4233
  - url: https://kotlinlang.org/docs/fun-interfaces.html
    title: Kotlin documentation, Functional (SAM) interfaces
    locator: SAM conversions
---

## Intent

Convert the interface of a class into the interface that a client expects. Then classes that could not work together, because their interfaces do not match, can work together.[^1]

## Problem

A dashboard shows temperatures. Its code calls `celsius()` on each sensor and prints the result. The sensors that the team wrote have that method. Then the team must add an old thermometer from a vendor library. Its class has no `celsius()`. It has `fahrenheit_tenths()`, which returns an integer in tenths of a degree Fahrenheit, so 98.6 F is `986`.

The two direct fixes are both bad. The team can change the dashboard to test for each kind of sensor and convert. Then the dashboard knows every vendor, and each added sensor changes it again. Or the team can change the vendor class. Often they cannot, because it is in a library, and when they can, every update of the library undoes the change.

Kerievsky describes the same starting point in two forms. Clients use two classes, and one of the two has the preferred interface. Or one class adapts several versions of a component, library or API, and the code for each version is mixed together.[^2]

The forces are these. The client must keep one interface. The existing class must not change. The translation between the two must live in one place, where a test can reach it.

## Solution

Write a class that implements the interface of the client (the Target) and holds an instance of the existing class (the Adaptee). Each method of the Target calls the methods of the Adaptee and converts the arguments, the results and the errors.[^1]

```text
 Client (dashboard)
   calls celsius()
        |
        v
 Target: Thermometer                 Adaptee: LegacyThermometer
 ------------------                  --------------------------
 celsius() -> float                  fahrenheit_tenths() -> int
        ^                                     ^
        |  implements                         |  holds one
        |                                     |
 Adapter: LegacyAdapter(legacy) --------------+
   celsius():
     tenths = legacy.fahrenheit_tenths()
     return (tenths / 10 - 32) * 5 / 9
```

Design Patterns gives two forms.[^1] An **object adapter** holds the adaptee and forwards to it, as above. It can adapt the class and all its subclasses, and the adaptee can be chosen at run time. A **class adapter** inherits from the adaptee and implements the target, so it needs multiple inheritance or an interface. It can override behaviour of the adaptee, but it adapts one concrete class only. Most code uses the object form, because composition works in every language that has references.

**A function type can be the adapter.** Go `net/http` defines `HandlerFunc`, a function type with a `ServeHTTP` method that calls the function. Its documentation calls it an adapter that allows the use of ordinary functions as HTTP handlers.[^3] Here the target is an interface with one method, and the adaptee is a plain function. With first-class functions, an adapter of one function is a function. It takes the old function and returns one with the signature of the target. Node `util.promisify` takes a function that uses an error-first callback and returns a version that returns promises.[^8]

**Standard libraries adapt streams.** `java.io.InputStreamReader` is a bridge from byte streams to character streams: it reads bytes and decodes them into characters with a charset.[^4] .NET `StreamReader` implements a `TextReader` that reads characters from a byte stream in an encoding.[^9] Python `io.TextIOWrapper` gives a text interface over a buffered binary stream.[^7] Python `contextlib.closing` adapts any object with a `close()` method to a context manager, which closes it at the end of a `with` block.[^6]

## Example

The example files show one program in each language. The dashboard function takes a `Thermometer` and prints its temperature in Celsius with one decimal. The program shows a native sensor at 21.5 C, then the legacy thermometer through the adapter at readings 986 and 704. The output is `21.5 C`, `37.0 C` and `21.3 C`.

| Language | Form |
|---|---|
| Python | a class with `celsius()` that holds the legacy object |
| JavaScript | a class that holds the legacy object |
| TypeScript | a class that implements the interface; structural types also accept an object literal |
| Java | an interface and a final class that holds the legacy object |
| Kotlin | a `fun interface`, so a lambda that reads the legacy object is the adapter |
| C# | an interface and a sealed class with a primary constructor |
| Go | a function type with a method, in the form of `http.HandlerFunc` |
| Rust | a newtype struct that wraps the legacy type and implements the trait |
| C++ | an abstract class and an object adapter that holds a reference |

## Consequences

The client and the adaptee stay as they are. The translation is in one class, so a test can check it with known readings. Another vendor needs another adapter and no change to the client. The client depends on the target interface only, which is what lets the [[strategy-pattern]] and the [[decorator-pattern]] wrap or swap the adapted object later.

The costs are these.

**An adapter can hide a mismatch in meaning.** An interface that matches by name and type does not match by meaning. Units, ranges, error codes and time zones must all be translated, and each one is a place for a bug. Here the legacy thermometer returns `-9999` when it is offline. An adapter that converts only the units turns that code into a temperature:

```python wrong id=sentinel
class LegacyThermometer:
    OFFLINE = -9999

    def __init__(self, tenths):
        self.tenths = tenths

    def fahrenheit_tenths(self):
        return self.tenths


class LegacyAdapter:
    def __init__(self, legacy):
        self.legacy = legacy

    def celsius(self):
        return (self.legacy.fahrenheit_tenths() / 10 - 32) * 5 / 9


for tenths in [986, LegacyThermometer.OFFLINE]:
    sensor = LegacyAdapter(LegacyThermometer(tenths))
    print(f"{sensor.celsius():.1f} C")
```

```output
37.0 C
-573.3 C
```

The second line, -573.3 C, is below absolute zero. The adapter must also translate the error convention. The adaptee signals an error with a value, and the client expects an exception, so the adapter turns the one into the other:

```python run id=sentinel-fixed
class SensorOffline(Exception):
    pass


class LegacyThermometer:
    OFFLINE = -9999

    def __init__(self, tenths):
        self.tenths = tenths

    def fahrenheit_tenths(self):
        return self.tenths


class LegacyAdapter:
    def __init__(self, legacy):
        self.legacy = legacy

    def celsius(self):
        tenths = self.legacy.fahrenheit_tenths()
        if tenths == LegacyThermometer.OFFLINE:
            raise SensorOffline("legacy thermometer is offline")
        return (tenths / 10 - 32) * 5 / 9


for tenths in [986, LegacyThermometer.OFFLINE]:
    sensor = LegacyAdapter(LegacyThermometer(tenths))
    try:
        print(f"{sensor.celsius():.1f} C")
    except SensorOffline as error:
        print(f"offline: {error}")
```

```output
37.0 C
offline: legacy thermometer is offline
```

**An adapter can hide a difference in behaviour.** The adapted object can keep rules of the adaptee that the target interface does not show. Java `Arrays.asList` adapts an array to the `List` interface. Its documentation states that the list is fixed size and backed by the array. Changes to the list are visible in the array. Methods that would change the size throw `UnsupportedOperationException`.[^5] The type `List<String>` does not say this:

```java run id=as-list
import java.util.Arrays;
import java.util.List;

public class Main {
    public static void main(String[] args) {
        String[] names = {"pen", "book"};
        List<String> list = Arrays.asList(names);
        list.set(0, "ink");
        System.out.println("array after set: " + names[0]);
        try {
            list.add("cup");
        } catch (UnsupportedOperationException error) {
            System.out.println("add: " + error.getClass().getSimpleName());
        }
    }
}
```

```output
array after set: ink
add: UnsupportedOperationException
```

A method that takes a `List` and adds to it fails at run time when a caller passes this list. Copy the list with `new ArrayList<>(Arrays.asList(names))` when the receiver needs a list that it owns.

**An adapter can hide a cost.** A call through an adapter looks as cheap as any call on the target. If the adapter converts a large value, opens a connection, or copies a collection on each call, the client cannot see it. State the cost in the documentation of the adapter.

**A class adapter couples to one class.** Design Patterns notes that a class adapter cannot adapt the subclasses of the adaptee, while an object adapter can. In return, a class adapter can override methods of the adaptee, and for an object adapter this is harder.[^1]

## When not to use it

Do not write an adapter when you own both sides and can change one of them. Change the class so that it has the interface the client needs. An adapter between two classes of the same program is often a sign that one interface is not needed.

Do not write an adapter for a mismatch that is only a name. A function reference, a lambda or a method reference adapts one call in one line. The Kotlin, Go and JavaScript examples show such forms.[^20][^3][^8]

Do not let one adapter grow to cover a whole subsystem. When the client needs a smaller, simpler interface to many classes, that is a facade, which has a different purpose.[^16]

Do not adapt in many places. Kerievsky's Extract Adapter starts from one class that adapts several versions of a component, with the code for each version mixed together. The fix is one adapter for each version.[^2]

## Variants and related patterns

**Two-way and pluggable adapters.** Design Patterns describes two more forms. A two-way adapter implements both interfaces, so that each side can use it. A pluggable adapter builds the adaptation into the class that needs it, with abstract operations or delegates.[^1] In languages with first-class functions, a pluggable adapter is often a function parameter.

**Newtypes in Rust.** Rust has an orphan rule. It allows a trait implementation only if the trait or at least one of the types is defined in the current crate.[^10] The Rust book shows how to implement an external trait on an external type. It wraps the type in a tuple struct with one field, a newtype, and implements the trait on the wrapper. The book states that the wrapper has no run-time cost.[^11] This is an object adapter that the compiler removes.

**Structural types.** TypeScript relates types by their members only.[^13] A class with the right methods is already a valid target, with no declaration. An adapter is needed only when the names or the types differ.

**Container adaptors in C++.** The standard library calls `stack`, `queue` and `priority_queue` container adaptors. Each one takes the type of an underlying container as a template parameter and gives it a smaller interface.[^12] These are class-level adapters fixed at compile time.

**Rust iterator adapters.** The Rust documentation calls functions that take an iterator and return another iterator iterator adapters, as a form of the adapter pattern.[^14] They keep the `Iterator` interface and change the values, so by the definitions of Design Patterns they are closer to decorators.[^1]

**Decorator.** A decorator keeps the interface and adds behaviour. An adapter changes the interface and keeps the behaviour.[^1]

**Bridge.** Bridge has a structure like an object adapter, but a different purpose. Bridge separates an abstraction from its implementation when the design starts, so that both can vary. An adapter makes two existing classes work together after the fact.[^15]

**Facade.** A facade provides one interface to a set of interfaces in a subsystem.[^16] An adapter converts one interface into another that already exists. A facade defines an interface that did not exist before.

**Factory Method.** Code that must choose an adapter for each vendor at run time often uses a [[factory-method-pattern|factory method]] or a table of constructors. Then the client never names an adapter class.

## Known uses

- Go: [HandlerFunc in server.go at go1.27.3](https://github.com/golang/go/blob/go1.27.3/src/net/http/server.go#L2350-L2359) is a function type whose `ServeHTTP` method calls the function. The comment above it calls it an adapter.[^17]
- OpenJDK: [InputStreamReader.java at jdk-25+36](https://github.com/openjdk/jdk/blob/jdk-25%2B36/src/java.base/share/classes/java/io/InputStreamReader.java#L81-L85) builds a `StreamDecoder` over the byte stream in its constructor, and `read(char[], int, int)` forwards to the decoder.[^18]
- OpenJDK: [Arrays.java at jdk-25+36](https://github.com/openjdk/jdk/blob/jdk-25%2B36/src/java.base/share/classes/java/util/Arrays.java#L4180-L4233) shows `asList` returning a private `ArrayList` class that holds the array itself. Its `set` method writes to the array.[^19]

## References
