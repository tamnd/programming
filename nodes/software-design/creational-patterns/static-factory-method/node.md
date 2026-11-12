---
tag: 1ESRA6
kind: pattern
slug: static-factory-method
name: Static factory method
aliases: [static factory, named constructor, named constructor idiom, factory function, alternative constructor]
area: software-design/creational-patterns
level: core
summary: >-
  A class gives a static method that returns an instance, in place of a public constructor. The method has a name, can return a cached object, a subtype, or no object.
family: creational-design
polarity: positive
intent: "Give a class static methods that return its instances, so that each way to create an object has a name, and the class controls which object the caller gets."
also_known_as: [Named Constructor]
history:
  year: 2001
  people: [Joshua Bloch]
  note: Bloch described the idiom as Item 1 of the first edition of Effective Java (2001), and the third edition (2018) keeps it as Item 1. The C++ FAQ describes the same design as the Named Constructor Idiom.
see_also: [factory-method-pattern, simple-factory, builder-pattern, singleton-pattern, multiton-pattern, flyweight-pattern]
sources:
  - book: isbn:9780134685991
    title: Joshua Bloch, Effective Java, third edition, Addison-Wesley, 2018
    locator: Item 1, Consider static factory methods instead of constructors (advantages, disadvantages, common names)
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Array/of
    title: Array.of(), MDN Web Docs
    locator: Description
  - book: isbn:9780201310054
    title: Joshua Bloch, Effective Java Programming Language Guide, Addison-Wesley, 2001
    locator: Chapter 2, Item 1
  - url: https://rust-lang.github.io/api-guidelines/predictability.html#c-ctor
    title: Rust API Guidelines, Predictability
    locator: C-CTOR, Constructors are static, inherent methods
  - url: https://go.dev/doc/effective_go#composite_literals
    title: Effective Go
    locator: Constructors and composite literals; Package names (ring.New)
  - url: https://docs.python.org/3.14/library/functions.html#classmethod
    title: The Python Standard Library, Built-in Functions, Python 3.14
    locator: classmethod
  - url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/lang/Integer.html#valueOf(int)
    title: Java SE 25 API, java.lang.Integer
    locator: valueOf(int); Constructor Summary (Integer(int) is deprecated)
  - url: https://github.com/openjdk/jdk/blob/jdk-25%2B36/src/java.base/share/classes/java/util/EnumSet.java#L112-L122
    title: OpenJDK jdk-25+36 source, java/util/EnumSet.java
    locator: noneOf, lines 112 to 122
  - book: isbn:9780201633610
    title: Gamma, Helm, Johnson and Vlissides, Design Patterns, Elements of Reusable Object-Oriented Software, Addison-Wesley, 1994
    locator: Chapter 3, Factory Method (Intent, Structure)
  - url: https://kotlinlang.org/docs/coding-conventions.html#factory-functions
    title: Kotlin coding conventions
    locator: Factory functions
  - url: https://docs.python.org/3.14/library/datetime.html#datetime.date.fromtimestamp
    title: The Python Standard Library, datetime, Python 3.14
    locator: date.fromtimestamp
  - url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/util/List.html#unmodifiable
    title: Java SE 25 API, java.util.List
    locator: Unmodifiable Lists
  - url: https://isocpp.org/wiki/faq/ctors#named-ctor-idiom
    title: C++ FAQ, Constructors, Standard C++ Foundation
    locator: What is the Named Constructor Idiom?
  - url: https://learn.microsoft.com/en-us/dotnet/api/system.timespan.fromseconds?view=net-9.0
    title: TimeSpan.FromSeconds Method, .NET API reference
    locator: Overloads
  - url: https://github.com/openjdk/jdk/blob/jdk-25%2B36/src/java.base/share/classes/java/time/Duration.java#L225-L227
    title: OpenJDK jdk-25+36 source, java/time/Duration.java
    locator: ofSeconds, lines 225 to 227; create, lines 518 to 523
---

## Intent

Give a class static methods that return its instances. Each way to create an object then has a name, and the class controls which object the caller gets: a new one, a cached one, or an object of a subclass.[^1]

## Problem

A class `Duration` holds a length of time in seconds. Callers want to create it from seconds, from minutes, and from a text such as `"2m"`. Most durations in a program are zero, so the class could give the same zero object to every caller. A text such as `"2h"` is not valid, and the caller must learn this without a crash.

A constructor meets none of these needs. In Java, C# and C++, every constructor has the name of the class. Two constructors cannot both take one `long`, so "from seconds" and "from minutes" cannot both be constructors. A constructor always creates a new object, and the object is always of exactly that class. A constructor cannot return "no object". It can only throw.

JavaScript shows the cost of an unnamed constructor. `Array.of(7)` creates an array with the single element 7, but `Array(7)` creates an empty array with a length of 7.[^2] The reader of `Array(7)` must know the rule for one argument. The reader of `Array.of(7)` does not.

## Solution

Make the constructor private, or at least not the main way in. Add **static methods** that call it. Each method has a name that says what the arguments mean, and each method decides what to return.[^1]

```text
 Client                         Duration
 ------                         ------------------------------------
 Duration.ofMinutes(2) -------> ofMinutes(m) = ofSeconds(m * 60)
 Duration.ofSeconds(0) -------> ofSeconds(s):
                                    if s == 0: return ZERO   (cached)
                                    return new Duration(s)   (private)
 Duration.parse("2h")  -------> parse(text): Duration or empty
```

There are two participants. The **product** is the class, with a private constructor. The **factory methods** are static methods of the same class, or functions in the same module. The client calls a method by name and gets the product.

Bloch gave this advice as Item 1 of the first edition of Effective Java in 2001,[^3] and the third edition keeps it as Item 1. The third edition gives five advantages over a public constructor:[^1]

1. A factory method has a name. `ofSeconds` and `ofMinutes` take the same parameter type and still both exist.
2. It need not create a new object on each call. It can return a cached instance, as `ofSeconds(0)` does.
3. It can return an object of any subtype of its declared return type, so the class of the result can stay hidden.
4. The class of the result can change from call to call, as a function of the arguments.
5. The class of the result need not exist when the factory method is written. Service provider frameworks use this.

Item 1 also lists the common names: `from` for a conversion, `of` for an aggregation, `valueOf`, `instance` or `getInstance`, `create` or `newInstance`, and the forms `getType`, `newType` and `type` when the method is in another class.[^1] Other languages have their own conventions. In Rust, the main constructor is usually named `new`, other constructors get a suffix such as `_with_foo`, and conversion constructors start with `from_`.[^4] In Go, a function named `New` or `NewT` plays the part, as `ring.New` does.[^5]

## Example

The example files show one program in each language. `Duration` has a factory method from seconds, one from minutes, and one that parses a text. The zero duration is cached, and a bad text gives "no object" in place of a crash:

| Language | Form |
|---|---|
| Python | `classmethod` factories that call `cls(...)`[^6] |
| JavaScript | `static` methods; a private `Symbol` guards the constructor |
| TypeScript | a `private constructor` and `static` methods |
| Java | a private constructor, `static` methods and `Optional` |
| C# | a private constructor, `static` methods and a `TryParse` method |
| C++ | a private constructor, `static` member functions and `std::optional` |
| Go | package-level functions; a parse function that returns an `error` |
| Rust | associated functions with no `self`; `parse` returns a `Result`[^4] |

## Consequences

The benefits are the five advantages of the Solution. Two of them have a cost that a caller must know.

**A cached instance changes the meaning of `==`.** When a factory method returns a cached object, two calls can give the same object, and an identity test is true. When it does not, the test is false. `Integer.valueOf` caches some values. Its documentation says that it "will always cache values in the range -128 to 127, inclusive, and may cache other values outside of this range."[^7] Code that compares with `==` then works for small numbers only:

```java wrong id=valueof-identity
public class Main {
    public static void main(String[] args) {
        Integer a = Integer.valueOf(127);
        Integer b = Integer.valueOf(127);
        Integer c = Integer.valueOf(1000);
        Integer d = Integer.valueOf(1000);
        System.out.println(a == b);      // the same cached object
        System.out.println(c == d);      // two objects with the default settings
        System.out.println(c.equals(d)); // the correct test
    }
}
```

```output for=valueof-identity
true
false
true
```

The result of the second line is not fixed by the API, because a JVM may cache more values. Compare values with `equals`, and treat a cached instance as an optimization that callers must not see.

**The class of the result can vary.** `EnumSet.noneOf` returns a `RegularEnumSet` when the enum has at most 64 constants, and a `JumboEnumSet` when it has more.[^8] Callers see only `EnumSet`, so the library can add a third class later. The same freedom is a trap for a caller that tests the concrete class or casts to it.

The costs, from Item 1:[^1]

- A class with no public or protected constructor cannot be subclassed. Bloch notes that this can be a benefit, because it pushes users to composition.
- A factory method is harder to find than a constructor. API documentation lists constructors in their own section, but a factory method is one static method among many. The naming conventions above reduce this cost.

**A static factory method is not the Factory Method pattern.** The Factory Method of Design Patterns is an instance method that a subclass overrides, so that a base class can create an object whose class it does not know.[^9] A static method cannot be overridden. It chooses the class itself, from its arguments. Bloch makes the same point: a static factory method has no direct equivalent in Design Patterns.[^1] See [[factory-method-pattern]] for the other pattern.

## When not to use it

Do not add a factory method when one public constructor is clear. A record or a data class with two fields gains nothing from `Point.of(x, y)` that `new Point(x, y)` does not give.

Do not give a factory method the name of the class without a reason. The Kotlin conventions ask for a distinct name that makes clear why the factory is special, and allow the class name only when there are no special semantics.[^10]

Do not use many factory methods for many optional parameters. Ten methods for ten combinations are as hard to read as ten constructors. The Rust API Guidelines point to a builder when a type has a multiplicity of construction options.[^4] See [[builder-pattern]].

Do not hide a public constructor that callers already use. A new factory method can sit next to it. Java deprecated the constructor `Integer(int)` and kept it. Its documentation says that the static factory `valueOf(int)` is generally a better choice.[^7]

## Variants and related patterns

**Class methods.** In Python, a `classmethod` receives the class as its first argument. When a subclass calls the method, the subclass is passed.[^6] A factory that builds `cls(...)` therefore works for subclasses with no change. A static method that names the base class does not:

```python run id=classmethod-subclass
class Duration:
    def __init__(self, seconds):
        self.seconds = seconds

    @classmethod
    def of_minutes(cls, minutes):
        return cls(minutes * 60)

    @staticmethod
    def of_hours(hours):
        return Duration(hours * 3600)


class LabeledDuration(Duration):
    def __str__(self):
        return f"{self.seconds} seconds"


print(type(LabeledDuration.of_minutes(2)).__name__)
print(type(LabeledDuration.of_hours(1)).__name__)
```

```output for=classmethod-subclass
LabeledDuration
Duration
```

The Python standard library uses class methods for its alternative constructors, such as `datetime.date.fromtimestamp`.[^11]

**Factory functions.** In a language with functions outside classes, the factory is a plain function of the module. Go has no constructors, and Effective Go shows `NewFile` as a function that builds and returns a `*File`.[^5] Kotlin puts factory functions in a companion object, such as `Point.fromPolar`, or at the top level.[^10]

**Collection factories.** Java's `List.of` and `List.copyOf` are static factory methods that create unmodifiable lists.[^12] The declared return type is the interface `List`, so the caller does not depend on the class that implements it.

**Named constructor idiom.** The C++ FAQ gives the same design this name. All constructors are private or protected, and public static methods return the object, one method for each way to construct it.[^13] .NET has static factory methods next to public constructors, such as `TimeSpan.FromSeconds`.[^14]

**Simple factory.** A [[simple-factory]] is a function that takes a parameter, such as a type name, and returns one of several classes. It is a static factory method whose main job is advantage 4, the choice of the class.

**Singleton and Multiton.** A static method that always returns the same instance is the access point of a [[singleton-pattern]]. One that returns one instance for each key is a [[multiton-pattern]]. Both are static factory methods with a cache.

**Flyweight.** A factory method that returns shared instances of small immutable objects is the factory of the [[flyweight-pattern]]. `Integer.valueOf` is an example.

## Known uses

- OpenJDK: [Duration.ofSeconds at jdk-25+36](https://github.com/openjdk/jdk/blob/jdk-25%2B36/src/java.base/share/classes/java/time/Duration.java#L225-L227) calls a private method `create`, and [create](https://github.com/openjdk/jdk/blob/jdk-25%2B36/src/java.base/share/classes/java/time/Duration.java#L518-L523) returns the constant `ZERO` when both parts are zero. The constructor of `Duration` is private.[^15]
- OpenJDK: [EnumSet.noneOf at jdk-25+36](https://github.com/openjdk/jdk/blob/jdk-25%2B36/src/java.base/share/classes/java/util/EnumSet.java#L112-L122) returns a `RegularEnumSet` for an enum with at most 64 constants and a `JumboEnumSet` for a larger one.[^8]

## References
