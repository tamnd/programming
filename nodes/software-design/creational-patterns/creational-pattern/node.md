---
tag: ZFKCA8
kind: concept
slug: creational-pattern
name: Creational pattern
aliases: [creational patterns, creational design pattern, object creation pattern, class creational pattern, object creational pattern]
area: software-design/creational-patterns
level: core
summary: >-
  A creational pattern is a design pattern for the creation of objects. It hides which concrete classes a program uses and how their instances are created and combined.
applies_to: core-languages
viz: optional
formal: none
history:
  year: 1994
  people: [Erich Gamma, Richard Helm, Ralph Johnson, John Vlissides]
  note: Design Patterns (1994) grouped its 23 patterns by purpose, and made the creational group of five patterns, Abstract Factory, Builder, Factory Method, Prototype and Singleton. In 1996 Peter Norvig showed that many of the 23 patterns are invisible or simpler in a dynamic language.
see_also: [constructors, factory-method-pattern, abstract-factory-pattern, builder-pattern, prototype-pattern, singleton-pattern, dependency-injection]
same_as:
  wikipedia: Creational_pattern
sources:
  - url: https://en.wikipedia.org/wiki/Creational_pattern
    title: Creational pattern, Wikipedia
    locator: Introduction; Usage; Examples
  - book: isbn:9780201633610
    title: Erich Gamma, Richard Helm, Ralph Johnson and John Vlissides, Design Patterns, Elements of Reusable Object-Oriented Software, Addison-Wesley, 1994
    locator: Chapter 1, Organizing the Catalog (Table 1.1) and Program to an interface; Chapter 3, Creational Patterns, introduction and discussion
  - book: isbn:9780134685991
    title: Joshua Bloch, Effective Java, third edition, Addison-Wesley, 2018
    locator: Chapter 2, Items 1 to 3
  - url: https://norvig.com/design-patterns/design-patterns.pdf
    title: Peter Norvig, Design Patterns in Dynamic Programming, Object World, 1996
    locator: slides 8 to 11 and 48
  - url: https://docs.python.org/3.14/faq/programming.html#how-do-i-share-global-variables-across-modules
    title: Programming FAQ, Python 3.14 documentation
    locator: How do I share global variables across modules?
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Object/create
    title: Object.create(), MDN Web Docs
    locator: Introduction
  - url: https://kotlinlang.org/docs/object-declarations.html
    title: Object declarations and expressions, Kotlin documentation
    locator: Introduction; Object declarations
  - url: https://learn.microsoft.com/en-us/dotnet/csharp/programming-guide/classes-and-structs/object-and-collection-initializers
    title: Object and Collection Initializers, C# programming guide
    locator: Object initializers
  - url: https://go.dev/doc/effective_go
    title: Effective Go
    locator: Package names; Allocation with new; Constructors and composite literals
  - url: https://doc.rust-lang.org/book/ch05-01-defining-structs.html
    title: Defining and Instantiating Structs, The Rust Programming Language
    locator: Creating Instances with Struct Update Syntax
  - url: https://rust-lang.github.io/api-guidelines/type-safety.html#c-builder
    title: Type safety, Rust API Guidelines
    locator: Builders enable construction of complex values (C-BUILDER)
  - url: https://docs.python.org/3.14/library/copy.html
    title: copy, The Python Standard Library, Python 3.14
    locator: Introduction; Interface summary
---

## Idea

Most code creates objects with a constructor: `Square(2)` or `new Square(2)`. The call names a concrete class. That is fine until the class must change: a different class for each platform, a copy of an object that a user has set up, one shared object in place of many, or an object with ten optional parts. Then each place that names the class must change.

A creational pattern moves the knowledge of the class, and of how to build it, out of the code that uses the object. Wikipedia gives the two main aspects: a creational pattern hides which concrete classes the system uses, and it hides how instances of these classes are created and combined.[^1]

```python run id=idea
import copy


class Circle:
    def __init__(self, size):
        self.size = size


class Square:
    def __init__(self, size):
        self.size = size


# A constructor: the caller names the class.
print("constructor:", type(Square(2)).__name__)

# A factory: the caller names a kind, and one place picks the class.
KINDS = {"circle": Circle, "square": Square}


def make(kind, size):
    return KINDS[kind](size)


print("factory:", [type(make(k, 2)).__name__ for k in ("circle", "square")])

# A prototype: the caller copies an object that it already has.
template = Square(3)
clone = copy.copy(template)
print("prototype:", type(clone).__name__, clone.size, clone is template)

# One shared instance: every caller gets the same object.
_shared = Square(1)


def shared():
    return _shared


print("singleton:", shared() is shared())
```

```output for=idea
constructor: Square
factory: ['Circle', 'Square']
prototype: Square 3 False
singleton: True
```

The constructor call ties the caller to `Square`. The factory lets the caller say what it wants, and one function decides the class. The prototype creates an object from an existing object, so the caller does not need to know the class. The shared instance decides how many objects exist. Each of these is a small form of a creational pattern.

## How it works

**What the patterns share.** A creational pattern aims to separate a system from how its objects are created, composed and represented, and it gives the system flexibility in what is created, who creates it, how and when.[^1] The code that uses an object depends on an interface, and the code that knows the concrete class is in one place. Design Patterns states the link to its principle *program to an interface, not an implementation*: the creational patterns give ways to create concrete objects while the rest of the system is written in terms of interfaces.[^2]

**Class and object creational patterns.** Design Patterns classifies its patterns by purpose (creational, structural, behavioral) and by scope (class or object).[^2] A class creational pattern defers object creation to subclasses. An object creational pattern defers part of the creation to another object.[^1] Factory Method is the only class creational pattern of the book. Abstract Factory, Builder, Prototype and Singleton are object creational.[^2]

| Question | Pattern | Who decides |
|---|---|---|
| Which class? | [[factory-method-pattern]] | a subclass overrides a creation method |
| Which family of classes? | [[abstract-factory-pattern]] | a factory object that the client is given |
| How is a complex object put together? | [[builder-pattern]] | a builder object, step by step |
| Which class, from an example? | [[prototype-pattern]] | an existing object, which is copied |
| How many instances? | [[singleton-pattern]] | the class, which keeps one instance |

Other named patterns answer the same questions. Wikipedia also lists lazy initialization, which delays the creation of an object until the first time it is needed, and the object pool, which recycles objects that are no longer in use.[^1] [[dependency-injection]] moves the creation out of the class: the class accepts the objects it requires from an injector, and does not create them.[^1] A [[static-factory-method]] replaces a constructor with a named method, and a [[multiton-pattern]] keeps one instance for each key.

**When a constructor is enough.** A pattern is worth its cost only when the creation must vary. If the caller knows the class, the class has a few required parameters, and no other class will take its place, a constructor call is the clearest code. Effective Java gives a middle step: a static factory method has a name, does not need to create a new object on each call, and can return an object of a subtype.[^3] Its next items recommend a builder when a constructor has many parameters, and a private constructor or an enum type for a singleton.[^3]

**Languages change the patterns.** Peter Norvig studied the 23 patterns of Design Patterns and found that 16 of them have a qualitatively simpler implementation in Lisp or Dylan than in C++, for at least some uses of each pattern.[^4] For the creational patterns the main reason is first-class types. When classes are objects at run time, a variable can hold a class, and the class itself serves as the factory, so there is no need for a parallel hierarchy of factories and products.[^4] He also noted that a language that can dispatch on constant values needs no singletons.[^4]

## In languages

| Language | Verdict | Mechanism |
|---|---|---|
| Python | idiom | classes are values and act as factories; `copy.copy` and `copy.deepcopy`; keyword arguments with defaults; a module is a singleton |
| JavaScript | idiom | factory functions that return object literals; `Object.create` with a prototype; classes are values; option objects |
| TypeScript | idiom | the forms of JavaScript with types; constructor types such as `new () => T`; `Partial<T>` for options |
| Java | idiom | static factory methods such as `List.of`; constructor references as `Supplier`; an enum for a singleton; builders |
| Kotlin | native | an `object` declaration is a singleton; named and default arguments; companion objects for factories |
| C# | idiom | object initializers and `init` properties; `with` for records; `Func<T>` factories; `Lazy<T>` |
| Go | idiom | a useful zero value; composite literals; constructor functions named `NewT`; `sync.OnceValue` |
| Rust | idiom | `new` by convention; `Default` and struct update syntax; `Clone`; builder types; `OnceLock` |
| C | idiom | functions that allocate and return a pointer to an opaque struct; a table of function pointers |
| C++ | idiom | `std::make_unique` and `std::make_shared`; copy constructors and a virtual `clone`; a function-local `static` |
| Bash | n/a | the shell has no objects; a function can return a name or print a value |
| PowerShell | idiom | classes with static methods; `[pscustomobject]` from a hashtable; `.Clone()` on .NET objects |
| SQL | n/a | `INSERT` creates rows, and default values and sequences fill columns, but there are no objects to create |
| Regex | n/a | a pattern creates no objects |
| HTML | n/a | the author writes each element; `<template>` holds markup that a script can copy |
| CSS | n/a | style rules create no objects |

The patterns matter most in languages where a constructor call must name a class at compile time and where classes are not values: Java, C#, C++. In Python and JavaScript, a class is a value, so a factory is often a dictionary or a parameter. Python's FAQ notes that there is only one instance of each module, so a module is also the basis for the singleton pattern.[^5] JavaScript has prototypes in the language: `Object.create` creates a new object with an existing object as its prototype.[^6] Kotlin has a singleton in the language: an object declaration creates one, and its initialization is thread-safe and done on first access.[^7] Named and default arguments, in Python, Kotlin and C#, remove most of the need for a builder. C# object initializers let a caller assign accessible fields or properties of an object when it creates the object.[^8]

Go and Rust have no constructors in the language. Effective Go advises to design types so that the zero value can be used without further initialization, and names the constructor of the type `Ring` `NewRing`, or `New` when the package exports only that type.[^9] A composite literal creates a new instance each time it is evaluated.[^9] Rust uses an associated function named `new`, struct update syntax to create an instance from most of the values of another instance,[^10] and the Rust API Guidelines recommend a builder type for data structures that need many inputs or optional configuration.[^11]

The example files show these forms. Python and JavaScript use classes as values and copy objects. TypeScript types a constructor parameter and an options object. Java uses `List.of`, `ArrayList::new` and an enum singleton. C# uses an object initializer, `with` and `Lazy<T>`. C++ uses `std::make_unique`, a virtual `clone` and a function-local `static`. Go uses the zero value of `bytes.Buffer`, a composite literal and `NewRequest`. Rust uses `new`, `Default`, struct update syntax and `OnceLock`.

## Variations and related ideas

- **Factory functions and factory objects.** A function that returns a new object is the simplest factory. An object with a creation method can be passed to other code, so the caller decides what they create.
- **Dependency injection.** A container or the main function creates the objects and passes them in. The classes then need no creational pattern of their own.
- **Copy and update.** Records in C# and Java, data classes in Python and Kotlin, and struct update syntax in Rust make a changed copy of a value. For immutable data this does the work of a prototype.
- **Object lifetime patterns.** Object pools, lazy initialization and multitons decide when an object is made and how long it lives, more than which class it has.

## Pitfalls

- **A pattern where a constructor is enough.** A factory for one class, or an abstract factory with one family, adds names and indirection and gives nothing in return.
- **A singleton as hidden global state.** Code that calls a singleton has a dependency that its signature does not show, and tests cannot replace it easily. Pass the object in when you can.
- **A shallow copy as a prototype.** A shallow copy constructs a new compound object and inserts references to the objects found in the original.[^12] A clone that shares a list with its prototype changes the prototype when the list changes. Use a deep copy, or make the shared parts immutable.
- **A builder that allows an incomplete object.** If the builder does not check the required fields in `build`, the program fails later and far from the cause.
- **Creation that is not thread-safe.** A lazy singleton or a registry that checks and then creates can create two objects when two threads race.

## Cost

A creational pattern adds an indirection: a call to a factory in place of a constructor, and often a virtual call or a lookup in a table. The run-time cost is small, but the cost in reading is real. A reader must find the factory to learn which class a call creates. In Rust, a factory that picks the type at run time returns a trait object such as `Box<dyn Trait>`, which is allocated on the heap and calls its methods through dynamic dispatch. A pool or a singleton trades memory for speed, because its objects stay alive.

## History

Design Patterns, by Gamma, Helm, Johnson and Vlissides, appeared in 1994. It classified 23 patterns by purpose and scope, and its creational group has five patterns: Abstract Factory, Builder, Factory Method, Prototype and Singleton.[^2] Factory Method is a class pattern, and the other four are object patterns.[^2]

Peter Norvig first presented *Design Patterns in Dynamic Programming* on 5 May 1996. He argued that 16 of the 23 patterns are either invisible or simpler in Dylan or Lisp, and that first-class types make Abstract Factory and Factory Method invisible or simpler.[^4] Later lists of creational patterns add patterns from other sources, such as dependency injection, lazy initialization and the object pool.[^1] Effective Java opens its chapter on creating and destroying objects with static factory methods, builders and singletons.[^3]

## References
