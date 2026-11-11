---
tag: XAZRJ8
kind: concept
slug: object-oriented-programming
name: Object-oriented programming
aliases: [OOP, object orientation, objects and classes, classes, inheritance, encapsulation, dynamic dispatch, virtual methods, vtable, prototype-based programming]
area: paradigms/object-oriented
level: core
summary: >-
  Object-oriented programming builds a program from objects that keep private state and answer method calls, so code can ask an object to act without knowing its exact kind.
applies_to: core-languages
viz: optional
formal: none
history:
  year: 1967
  people: [Ole-Johan Dahl, Kristen Nygaard, Alan Kay]
  note: Simula 67 introduced classes, subclasses and virtual procedures, and Smalltalk at Xerox PARC made objects and messages the whole language.
see_also: [interfaces, subtyping, parametric-polymorphism, closures, immutability, strategy-pattern, visitor-pattern, sum-types]
same_as:
  wikidata: Q79872
  wikipedia: Object-oriented_programming
sources:
- url: https://doc.rust-lang.org/1.99.1/book/ch18-01-what-is-oo.html
  title: The Rust Programming Language, Characteristics of Object-Oriented Languages, Rust 1.99.1
  locator: Objects Contain Data and Behavior; Inheritance as a Type System and as Code Sharing
- url: https://docs.python.org/3.14/tutorial/classes.html
  title: The Python Tutorial, 9. Classes, Python 3.14 documentation
  locator: 9. Classes; 9.3.5 Class and Instance Variables
- url: https://docs.oracle.com/javase/specs/jls/se25/html/jls-8.html#jls-8.4.9
  title: The Java Language Specification, Java SE 25 Edition
  locator: 8.4.9 Overloading
- url: https://lwn.net/Articles/444910/
  title: Object-oriented design patterns in the kernel, part 1, Neil Brown, LWN.net, 2011
  locator: Method Dispatch
- url: https://learn.microsoft.com/en-us/dotnet/csharp/fundamentals/tutorials/oop
  title: Object-Oriented programming (C#), Microsoft Learn
  locator: introduction; Polymorphism
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Guide/Inheritance_and_the_prototype_chain
  title: Inheritance and the prototype chain, MDN Web Docs
  locator: Inheritance with the prototype chain
- url: https://www.purl.org/stefan_ram/pub/doc_kay_oop_en
  title: Dr. Alan Kay on the Meaning of Object-Oriented Programming, email to Stefan Ram, 2003
  locator: full email
- url: https://en.cppreference.com/w/cpp/language/virtual
  title: virtual function specifier, cppreference.com
  locator: Explanation; Virtual destructor
- url: https://go.dev/doc/faq#Is_Go_an_object-oriented_language
  title: Frequently Asked Questions (FAQ), The Go Programming Language
  locator: Is Go an object-oriented language?
- url: http://worrydream.com/EarlyHistoryOfSmalltalk/
  title: The Early History of Smalltalk, Alan C. Kay, ACM SIGPLAN Notices 28(3), 1993
  locator: I. 1960-66, Early OOP and other formative ideas of the sixties
---

## Idea

Object-oriented programming organizes a program as a set of objects. An object keeps some state and offers operations, called methods, that read or change that state. Other code asks an object to do something by calling a method, and the object decides how to respond. Two objects of different kinds can answer the same request in different ways, so the calling code does not need to know which kind it has:

```python run id=idea
class Account:
    def __init__(self, owner):
        self.owner = owner
        self._balance = 0

    def deposit(self, amount):
        if amount <= 0:
            raise ValueError("amount must be positive")
        self._balance += amount

    def month_end(self):
        pass

    def report(self):
        return f"{self.owner}: {self._balance}"


class Savings(Account):
    def month_end(self):
        self._balance += self._balance // 100


accounts = [Account("ana"), Savings("bo")]
for a in accounts:
    a.deposit(500)
    a.month_end()
    print(a.report())
```

```output for=idea
ana: 500
bo: 505
```

The loop calls `month_end` on each account and does not ask which class the account has. The `Savings` class inherits `deposit` and `report` from `Account` and replaces `month_end`. The balance is only changed through methods, so the rule that a deposit is positive is in one place.

## How it works

**Objects package state with behavior.** The book Design Patterns by Gamma, Helm, Johnson and Vlissides defines the idea in this way: "Object-oriented programs are made up of objects. An object packages both data and the procedures that operate on that data."[^1] Most languages describe the shape of an object with a class. The Python tutorial states that "Classes provide a means of bundling data and functionality together. Creating a new class creates a new type of object, allowing new instances of that type to be made."[^2]

**Dynamic dispatch selects the method.** A call such as `shape.area()` names an operation, not a function. The language finds the method from the object at run time. The Java specification states that for an instance method, "the actual method to be invoked will be determined at run time, using dynamic method lookup".[^3] Compiled languages usually implement this with a table of function pointers for each class, called a virtual method table or vtable, and each object holds a pointer to the table of its class. A call loads the table pointer, loads the method from a fixed slot, and calls it. C programs build the same structure by hand. An LWN article on the Linux kernel describes "a pure vtable being a structure which contains only function pointers where the first argument of each is a pointer to some other structure (the object type) which itself contains a pointer to this vtable".[^4] Dynamic languages such as Python and Smalltalk look up the method by name in the class and its ancestors, often with a cache.

**Encapsulation hides the state.** If only the methods of an object can change its state, the object can keep its invariants, and its representation can change without changes to the code that uses it. The C# guide describes encapsulation as "Hiding the internal state and functionality of an object and only allowing access through a public set of functions."[^5] Languages enforce this to different degrees. Java, C# and C++ check `private` at compile time, JavaScript enforces `#` fields at run time, and Python relies on a naming convention.

**Inheritance shares code and defines subtypes.** A class can extend another class. It then gets the fields and methods of the parent and can override some of them. The Rust book describes inheritance as a mechanism by which an object can "inherit elements from another object's definition, thus gaining the parent object's data and behavior without you having to define them again".[^1] Inheritance does two jobs at once: it reuses an implementation, and it makes the child a subtype that can be used where the parent is expected. Many later designs separate the two jobs. Interfaces, protocols and traits give the subtype relation without shared state, and composition gives reuse without the subtype relation.

**Classes or prototypes.** In a class-based language, objects are instances of classes, and the class holds the methods. In a prototype-based language such as Self or JavaScript, an object inherits directly from another object. MDN explains that when a property is not on an object, the lookup continues on its prototype, then on the prototype of the prototype, until it "is found or the end of the prototype chain is reached".[^6] The JavaScript `class` keyword is a convenient form for this model, and MDN notes that "classes do not bring a new inheritance pattern."[^6]

**Two views of the same name.** Alan Kay, who led the Smalltalk work at Xerox PARC, put the emphasis on messages and not on classes: "OOP to me means only messaging, local retention and protection and hiding of state-process, and extreme late-binding of all things."[^7] He thought of objects "being like biological cells and/or individual computers on a network, only able to communicate with messages".[^7] The tradition of Simula, C++ and Java puts the emphasis on classes as types, with static checks and inheritance. Both views agree on objects with private state and on dynamic dispatch. They differ in how late the binding is and in how much the type system knows. The Rust book notes that "There is no consensus in the programming community about what features a language must have to be considered object oriented."[^1]

## In languages

| Language | Verdict | Mechanism |
|---|---|---|
| Python | native | classes, multiple inheritance, `super()` with a method resolution order, duck typing; privacy by convention |
| JavaScript | native | prototype chains, `class` syntax, `#private` fields |
| TypeScript | native | JavaScript classes plus interfaces, abstract classes and access modifiers checked at compile time |
| Java | native | classes, single inheritance, interfaces with default methods, virtual by default, `sealed` and `record` |
| Kotlin | native | classes final by default, `open` and `override`, interfaces, data and sealed classes |
| C# | native | classes, single inheritance, interfaces, `virtual`, `abstract`, `override` and `new` |
| Go | native | methods on any named type, implicit interfaces, struct embedding; no classes or inheritance |
| Rust | native | structs with methods, traits, trait objects with `dyn`; no inheritance of data |
| C | idiom | structs of function pointers as hand-built vtables |
| C++ | native | classes, multiple inheritance, opt-in `virtual`, `override` and `final` |
| Bash | absent | no objects; naming conventions only |
| PowerShell | native | `class` with inheritance and interfaces; all values are .NET objects |
| SQL | n/a | relations, not objects |
| Regex, HTML, CSS | n/a | no objects or methods |

Python and JavaScript are dynamic. A method is found by name when the call runs, so any object with a method of the right name can take part. This is duck typing. Python allows more than one base class and resolves the order of lookup with the C3 linearization, which is visible in `__mro__`. The Python tutorial warns that a mutable value in a class attribute, such as a list, is shared by all instances.[^2]

Java and C# are class-based and statically typed. Each class has one parent class and any number of interfaces. In Java, instance methods can be overridden unless they are `final`. In C#, a method must be marked `virtual` or `abstract` before a derived class can override it, and a method marked `new` hides the base method, so the static type of the variable decides which method runs.[^5] Java's `sealed` interfaces and `record` classes, and the equivalent features in C#, move these languages toward the algebraic data types of functional languages.

C++ makes dynamic dispatch opt-in. Only a `virtual` function keeps the overriding behavior when a derived object is used through a pointer or a reference to the base class.[^8] A class with no virtual functions needs no vtable, and its calls cost the same as calls to plain functions. C++ also allows multiple inheritance of classes with state, which brings the problem of a shared base class, solved with `virtual` inheritance.

Go and Rust have no inheritance. The Go FAQ answers the question of whether Go is object-oriented with "Yes and no. Although Go has types and methods and allows an object-oriented style of programming, there is no type hierarchy."[^9] A Go type satisfies an interface when it has the methods, with no declaration. Rust uses traits, and the Rust book explains that it takes "the different approach of using trait objects instead of inheritance to achieve polymorphism at runtime."[^1] Both languages use composition for reuse.

C has no objects, but large C programs use the object-oriented pattern by hand. The Linux kernel has many structures of function pointers, such as `file_operations`, that act as vtables.[^4]

## Variations and related ideas

- **Interfaces and traits.** These define a set of methods without state. They give subtyping without inheritance of an implementation. See [interfaces](/interfaces/) and [subtyping](/subtyping/).
- **Composition over inheritance.** An object holds other objects and forwards calls to them. This is the advice of the Design Patterns book, and the basis of the [decorator](/decorator-pattern/) and [strategy](/strategy-pattern/) patterns.
- **Mixins and multiple inheritance.** A class can take methods from several sources. Python, C++ and Scala allow this. Languages with single inheritance often add default methods in interfaces to get some of the same effect.
- **Multiple dispatch.** CLOS, Julia and Dylan choose a method from the types of all arguments, not only the first. Single dispatch languages use the [visitor pattern](/visitor-pattern/) to get double dispatch.
- **The actor model.** Kay's description of objects as small computers that exchange messages is close to the actor model, in which each actor has a mailbox and runs on its own. Erlang processes are a working example.
- **Closures as objects.** A closure is a function with captured state, and an object is state with functions. Each can model the other. See [closures](/closures/).
- **Expression problem.** In object-oriented code it is easy to add a new kind of object and hard to add a new operation to all kinds. In code that uses [sum types](/sum-types/) and [pattern matching](/pattern-matching/), the opposite is true.

## Pitfalls

- **Deep hierarchies.** Each level of inheritance adds behavior that a reader must find in another file. A change to a base class can break subclasses that depend on its internal call order. This is called the fragile base class problem. The Rust book adds that "Inheritance is often at risk of sharing more code than necessary."[^1]
- **Subclasses that are not subtypes.** A subclass must keep the promises of its parent. A `Square` that extends a mutable `Rectangle` breaks code that sets the width and the height on their own. This is the Liskov substitution principle.
- **Object slicing in C++.** A derived object copied into a variable of the base type loses its derived part, and the copy calls base methods. Pass polymorphic objects by reference or by pointer.
- **Missing virtual destructor.** In C++, deleting a derived object through a pointer to a base class without a virtual destructor is undefined behavior.[^8]
- **Calls to overridable methods from a constructor.** The derived part of the object is not ready yet. Java and C# call the derived override on the half-built object, and C++ calls the base version. Neither result is what most authors expect.
- **Shared mutable class attributes.** In Python, a list defined in the class body is one list for all instances.[^2]
- **Getters and setters for every field.** A class that exposes all its fields through accessors has no real encapsulation. Methods should express what the object does, not mirror its fields.

## Cost

A virtual call loads a table pointer from the object, loads a function pointer from the table, and makes an indirect call. This is a few cycles more than a direct call when the branch predictor guesses well. The larger cost is that the compiler cannot inline a call when it does not know the target, which blocks other optimizations. Compilers remove this cost when they can prove the type, for example with `final` classes or with whole-program analysis, and JIT compilers inline the common target and add a guard. Each object with virtual methods holds one extra pointer. Objects reached through pointers are often spread over the heap, which costs cache misses when a program processes many of them. Data-oriented designs store the fields of many objects in arrays for this reason.

## History

Ole-Johan Dahl and Kristen Nygaard designed Simula at the Norwegian Computing Center in the 1960s to write simulations. Simula 67 introduced classes, objects, subclasses and virtual procedures. Alan Kay studied Simula and Ivan Sutherland's Sketchpad, and in the 1970s led the group at Xerox PARC that built Smalltalk, in which everything is an object and computation is done by sending messages. Kay wrote that "Simula-I had neither classes as objects nor inheritance".[^10] He also wrote that he did not like the way Simula did inheritance, so he "decided to leave out inheritance as a built-in feature until I understood it better."[^7] Bjarne Stroustrup added Simula's classes to C in C with Classes, which became C++ in the 1980s. Objective-C added Smalltalk's messages to C. Java and C# made class-based object-oriented programming the mainstream style of the 1990s and 2000s. Later languages such as Go and Rust kept objects and interfaces and left out inheritance.

## References
