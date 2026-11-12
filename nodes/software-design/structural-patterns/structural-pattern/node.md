---
tag: 4NCWB4
kind: concept
slug: structural-pattern
name: Structural pattern
aliases: [structural patterns, structural design pattern, class structural pattern, object structural pattern, wrapper pattern]
area: software-design/structural-patterns
level: core
summary: >-
  A structural pattern is a design pattern about how classes and objects are composed into larger structures. Most of them wrap one object in another and differ in what the wrapper does.
applies_to: core-languages
viz: optional
formal: none
history:
  year: 1994
  people: [Erich Gamma, Richard Helm, Ralph Johnson, John Vlissides]
  note: Design Patterns (1994) made a structural group of seven patterns, Adapter, Bridge, Composite, Decorator, Facade, Flyweight and Proxy. In 1996 Peter Norvig showed that Flyweight, Proxy and Facade are invisible or simpler in Dylan or Lisp.
see_also: [adapter-pattern, bridge-pattern, composite-pattern, decorator-pattern, facade-pattern, flyweight-pattern, proxy-pattern, composition-over-inheritance]
same_as:
  wikipedia: Structural_pattern
sources:
  - url: https://en.wikipedia.org/wiki/Structural_pattern
    title: Structural pattern, Wikipedia
    locator: Introduction; Examples
  - book: isbn:9780201633610
    title: Erich Gamma, Richard Helm, Ralph Johnson and John Vlissides, Design Patterns, Elements of Reusable Object-Oriented Software, Addison-Wesley, 1994
    locator: Chapter 1, Organizing the Catalog (Table 1.1); Chapter 4, Structural Patterns, introduction and Discussion of Structural Patterns
  - url: https://norvig.com/design-patterns/design-patterns.pdf
    title: Peter Norvig, Design Patterns in Dynamic Programming, Object World, May 5, 1996
    locator: slides 9 and 28
  - url: https://go.dev/ref/spec#Struct_types
    title: The Go Programming Language Specification, Struct types
    locator: embedded fields and promoted methods
  - url: https://kotlinlang.org/docs/delegation.html
    title: Delegation, Kotlin documentation
    locator: Implementation by delegation
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Proxy
    title: Proxy, MDN Web Docs
    locator: Introduction
  - url: https://docs.python.org/3.14/reference/datamodel.html#object.__getattr__
    title: Data model, The Python Language Reference, Python 3.14
    locator: 3.3.2.1 object.__getattr__; 3.3.13 Special method lookup
  - url: https://doc.rust-lang.org/std/ops/trait.Deref.html
    title: Trait Deref, Rust standard library
    locator: Deref coercion; When to implement Deref or DerefMut
---

## Idea

A program is rarely one class. Classes and objects are put together into larger structures, and the way they are put together decides what can change later. Wikipedia describes a structural pattern as a design pattern that encapsulates relationships between entities.[^1] Design Patterns says that structural patterns are concerned with how classes and objects are composed to form larger structures.[^2]

Many structural patterns have the same shape: one object holds another and forwards calls to it. They differ in the interface that the outer object gives and in what it does with each call. Here are three wrappers around one sensor:

```python run id=idea
class Sensor:
    def __init__(self):
        self.reads = 0

    def read(self):
        self.reads += 1
        return 20  # degrees Celsius


class FahrenheitAdapter:  # a different interface
    def __init__(self, sensor):
        self.sensor = sensor

    def fahrenheit(self):
        return self.sensor.read() * 9 // 5 + 32


class Calibrated:  # the same interface, and one more responsibility
    def __init__(self, sensor, offset):
        self.sensor = sensor
        self.offset = offset

    def read(self):
        return self.sensor.read() + self.offset


class CachingProxy:  # the same interface, and control of access
    def __init__(self, sensor):
        self.sensor = sensor
        self.value = None

    def read(self):
        if self.value is None:
            self.value = self.sensor.read()
        return self.value


print(f"adapter: {FahrenheitAdapter(Sensor()).fahrenheit()} F")
print(f"decorator: {Calibrated(Sensor(), 1).read()}")
real = Sensor()
proxy = CachingProxy(real)
print(f"proxy: {proxy.read()} {proxy.read()}, real reads: {real.reads}")
```

```output for=idea
adapter: 68 F
decorator: 21
proxy: 20 20, real reads: 1
```

The adapter changes the interface. The decorator keeps the interface and adds a calibration. The proxy keeps the interface and decides when the real sensor is read. The code that each wrapper contains is almost the same, so the name of the pattern is mostly a statement of intent.

## How it works

**Class and object structural patterns.** Design Patterns classifies its patterns by purpose and by scope.[^2] A structural class pattern uses inheritance to compose interfaces or implementations. A structural object pattern composes objects at run time to get new functionality, so the composition can change while the program runs. Adapter is the only structural pattern of the book with both forms: a class adapter inherits from the adaptee, and an object adapter holds it. Bridge, Composite, Decorator, Facade, Flyweight and Proxy are object patterns.[^2]

**Wrappers compared.** Most of the seven patterns hold a reference to another object. The table shows what each one changes:[^2]

| Pattern | Interface to the client | What the pattern does |
|---|---|---|
| [[adapter-pattern]] | the one the client expects, not the one of the wrapped object | makes existing classes work together after they are designed |
| [[bridge-pattern]] | an abstraction with its own hierarchy | separates an abstraction from its implementation up front, so that both can vary |
| [[decorator-pattern]] | the same as the wrapped object | adds responsibilities to one object, and wrappers can be stacked |
| [[proxy-pattern]] | the same as the wrapped object | a placeholder that controls access to the real object |
| [[composite-pattern]] | the same for a leaf and for a group | puts objects in a tree, so that clients treat one object and a group in the same way |
| [[facade-pattern]] | a new, simpler interface | gives one interface to a set of classes in a subsystem |
| [[flyweight-pattern]] | the same as an unshared object | shares objects to support large numbers of fine-grained objects |

Decorator and Proxy have the same structure, so their difference is the purpose. A decorator adds behaviour, and it is often stacked. A proxy is a stand-in for an object that is inconvenient or undesirable to access directly.[^2]

**Other structural patterns.** Wikipedia also lists the marker interface, the opaque pointer and pipes and filters as structural patterns.[^1] This area adds patterns from other sources: [[delegation-pattern]], [[mixin-pattern]], [[module-pattern]], [[private-class-data]], [[marker-interface]], [[twin-pattern]], [[whole-part]] and [[extension-object]].

**Languages change the patterns.** Peter Norvig found that 16 of the 23 patterns of Design Patterns are invisible or simpler in Dylan or Lisp. Among the structural patterns, first-class types make Flyweight and Proxy simpler, and modules make Facade invisible: a module exports only the names that make up the interface.[^3] Some languages also generate the forwarding code of a wrapper, which is the part that is the same in all of them.

## In languages

| Language | Verdict | Mechanism |
|---|---|---|
| Python | idiom | duck typing, so a wrapper needs only the methods that are called; `__getattr__` to forward the rest; modules as facades |
| JavaScript | native | the `Proxy` object intercepts the operations on another object; duck typing; modules |
| TypeScript | idiom | interfaces for the wrapped and the wrapper type; `Proxy` from JavaScript |
| Java | idiom | interfaces and object composition; `java.lang.reflect.Proxy` for dynamic proxies; stream classes that wrap other streams |
| Kotlin | native | class delegation with `by`, where the compiler generates the forwarding methods |
| C# | idiom | interfaces and object composition; stream classes that wrap other streams |
| Go | native | struct embedding promotes the methods of the embedded field; small interfaces such as `io.Reader` |
| Rust | idiom | traits and wrapper structs; `Deref` for smart pointers |
| C | idiom | opaque pointers; structs of function pointers as interfaces |
| C++ | idiom | virtual functions and composition; multiple inheritance for a class adapter; smart pointers as proxies |
| Bash | idiom | a pipeline joins filters; a function wraps a command |
| PowerShell | idiom | the pipeline; classes that implement interfaces and wrap .NET objects |
| SQL | idiom | a view gives a simpler or a changed interface to tables |
| Regex | n/a | a pattern has no objects to compose |
| HTML | n/a | elements nest, but markup has no objects or interfaces |
| CSS | n/a | style rules have no objects or interfaces |

Three languages generate the forwarding. In Go, a field or method of an embedded field is promoted, and the method set of the struct includes the promoted methods.[^4] A struct that embeds a value and declares one method of its own is a decorator with no forwarding code. In Kotlin, the by-clause in the supertype list "indicates that b will be stored internally in objects of Derived and the compiler will generate all the methods of Base that forward to b."[^5] In JavaScript, a `Proxy` object can intercept and redefine fundamental operations for another object.[^6]

Python forwards by a hook: `__getattr__` is called when the default attribute access fails.[^7] Rust uses `Deref` for types that act like a value of another type, which are often called smart pointers. A type that implements `Deref<Target = U>` implicitly implements all the methods of `U` that take `&self`.[^8] The documentation advises against `Deref` for a type with methods that are likely to collide with methods of the target type, so it is not a general tool for decorators.[^8]

The example files show the three wrappers of the Idea section in each language. Python forwards with `__getattr__`. JavaScript uses a `Proxy` for the proxy. TypeScript, Java, C# and C++ use an interface or an abstract class. Go embeds the sensor in the decorator. Rust uses a trait.

## Variations and related ideas

- **Class versus object composition.** A class pattern fixes the structure when the program is compiled. An object pattern can change it at run time, which is one reason for the advice of Design Patterns to favor object composition over class inheritance.[^2] See [[composition-over-inheritance]].
- **Generated wrappers.** Dynamic proxies, `Proxy` in JavaScript and Kotlin's `by` remove the forwarding code, and the wrapper holds only the behaviour that differs.
- **Structure without wrappers.** A facade can be a module, a flyweight can be a cache of shared values, and a composite can be a recursive data type.
- **Creational and behavioral patterns.** A structural pattern is about how parts fit together. A [[creational-pattern]] is about how objects are created, and a behavioral pattern is about algorithms and the assignment of responsibilities between objects.[^2]

## Pitfalls

- **Identity.** A wrapper is a different object. A test of identity or a key in a map that holds the wrapped object does not find the wrapper.
- **Partial forwarding.** A hand-written wrapper forwards only the methods that it declares. In Python, `__getattr__` forwards normal attribute access, but implicit invocations of special methods use the type, not the instance, so `len` does not go through the hook:[^7]

```python wrong id=special-methods
class Logged:
    def __init__(self, inner):
        self._inner = inner

    def __getattr__(self, name):  # called only when normal lookup fails
        return getattr(self._inner, name)


items = Logged([3, 1, 2, 1])
print("count:", items.count(1))
try:
    print("len:", len(items))
except TypeError as e:
    print("len:", e)
```

```output for=special-methods
count: 2
len: object of type 'Logged' has no len()
```

  The fix is to define each special method that the wrapper must support on the wrapper class, for example `def __len__(self): return len(self._inner)`.
- **Deep stacks.** Each layer of decorators or proxies is one more object to read through in a stack trace or a debugger.
- **The wrong pattern name.** An adapter that also adds behaviour, or a proxy that changes the interface, confuses readers who know the names. Name the wrapper by what it does.

## Cost

Each wrapper adds one call and one object. In most programs this is small next to the work of the wrapped object. A proxy or a flyweight can save more than it costs: a proxy can avoid the creation or the use of an expensive object, and a flyweight shares one object among many uses. In Rust and C++, a wrapper that the compiler sees in full can be inlined, while a wrapper behind a trait object or a virtual function is called through dynamic dispatch.

## History

Design Patterns, by Gamma, Helm, Johnson and Vlissides, appeared in 1994. It classified 23 patterns by purpose and scope. Its structural group has seven patterns: Adapter, Bridge, Composite, Decorator, Facade, Flyweight and Proxy.[^2]

Peter Norvig presented *Design Patterns in Dynamic Programming* at Object World on May 5, 1996. He listed Flyweight and Proxy among the six patterns that first-class types make invisible or simpler, and Facade as the pattern that modules make invisible.[^3] Later lists of structural patterns add patterns from other sources, such as the marker interface and the opaque pointer.[^1]

## References
