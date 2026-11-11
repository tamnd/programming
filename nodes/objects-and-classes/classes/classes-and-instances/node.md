---
tag: BNR9SD
kind: concept
slug: classes-and-instances
name: Classes and instances
aliases: [class, instance, object type, class member, instance variable, class variable, static member, metaclass, struct with methods]
area: objects-and-classes/classes
level: core
summary: >-
  A class describes the fields and methods of a kind of object. An instance is one object made from it, with its own field values, sharing the methods and the class data.
applies_to: core-languages
viz: optional
formal: none
history:
  year: 1967
  people: [Ole-Johan Dahl, Kristen Nygaard]
  note: Simula 67 introduced classes and objects, and Smalltalk made every value an instance of a class, including the classes themselves.
see_also: [object-oriented-programming, constructors, encapsulation-and-visibility, class-inheritance, interfaces, dynamic-dispatch, value-equality-and-identity, prototype-based-programming]
same_as:
  wikidata: Q4479242
  wikipedia: Class_(programming)
sources:
- url: https://docs.python.org/3/tutorial/classes.html
  title: Classes, The Python Tutorial
  locator: 9. Classes; 9.3.5 Class and Instance Variables
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Classes
  title: Classes, MDN Web Docs
  locator: Introduction
- url: https://docs.oracle.com/javase/specs/jls/se25/html/jls-8.html
  title: Chapter 8, Classes, The Java Language Specification, Java SE 25 Edition
  locator: Chapter 8, introduction
- url: https://en.cppreference.com/cpp/language/classes
  title: Classes, cppreference.com
  locator: Introduction
- url: https://learn.microsoft.com/en-us/dotnet/standard/design-guidelines/choosing-between-class-and-struct
  title: Choosing Between Class and Struct, .NET Framework design guidelines
  locator: Introduction
- url: https://go.dev/doc/faq
  title: Frequently Asked Questions (FAQ), The Go Programming Language
  locator: Is Go an object-oriented language?
---

## Idea

A class describes a kind of object: the fields that each object of the kind holds, and the methods that work on them. An instance is one object made from the class, with its own values for the fields. The class exists once, and every instance links to it, so all instances share one copy of the methods and of any data that belongs to the class itself. A class is also a type: code can state that a variable holds an `Account`, and the compiler or the runtime checks it. Python, JavaScript, Java, C#, Kotlin, C++ and PowerShell have classes. Go and Rust put methods on structs and other types without classes, and C writes the pattern by hand with structs and functions.

```python run id=idea
def deposit(self, amount):
    self["balance"] += amount


def interest(self):
    return self["balance"] * self["class"]["rate"]


# A class: a name, data shared by all instances, and a table of methods.
Account = {"name": "Account", "rate": 0.02,
           "methods": {"deposit": deposit, "interest": interest}}


def new(cls, **fields):  # an instance: its own fields and a link to its class
    return {"class": cls, **fields}


def send(obj, name, *args):  # a method call: find the method in the class
    return obj["class"]["methods"][name](obj, *args)


a = new(Account, owner="ana", balance=100)
b = new(Account, owner="bo", balance=500)
send(a, "deposit", 50)
print(a["balance"], b["balance"])  # each instance has its own fields
print(send(a, "interest"), send(b, "interest"))
Account["rate"] = 0.05  # one change in the class reaches every instance
print(send(a, "interest"), send(b, "interest"))
print(a["class"] is b["class"])  # the methods exist once, in the class
```

```output for=idea
150 500
3.0 10.0
7.5 25.0
True
```

The program builds a class with plain dictionaries. `Account` holds a shared value, `rate`, and a table of methods. `new` makes an instance: a dictionary with its own fields and a link to its class. `send` calls a method by looking up its name in the class of the object and passing the object as the first argument. Each instance keeps its own balance, but a change to `rate` in the class reaches both instances, because they read it through their link. Every language with classes does this work, with faster lookups and with checks at compile time.

## How it works

**A class bundles data and behavior.** The Python tutorial gives the definition: "Classes provide a means of bundling data and functionality together. Creating a new class creates a new type of object, allowing new instances of that type to be made."[^1] MDN describes JavaScript classes in the same terms: "Classes are a template for creating objects. They encapsulate data with code to work on that data."[^2] The Java Language Specification lists the parts: "The body of a class declares members (fields, methods, classes, and interfaces), instance and static initializers, and constructors".[^3] Access modifiers such as `private` decide which code can touch each member. See [encapsulation](/encapsulation-and-visibility/) and [constructors](/constructors/).

**Instance members and class members.** An instance field has one value in each object, and a class field, called static in Java, C#, C++ and JavaScript, has one value for the whole class. The Python tutorial states the rule: "Generally speaking, instance variables are for data unique to each instance and class variables are for attributes and methods shared by all instances of the class".[^1] A static method belongs to the class and has no instance, so it can reach only the static fields, unless it receives an instance as an argument. Factory methods, counters and constants are the usual static members.

**Methods receive the instance.** A method is a function with one hidden parameter, the instance on which it is called. Python and Rust make the parameter visible as `self`, and Go names it in the receiver. Java, C#, C++ and JavaScript call it `this` and pass it without a declaration. A call such as `a.deposit(50)` does two things: it finds the method through the class of `a`, and it passes `a` as the receiver. In languages that look up methods at run time, the first step is [dynamic dispatch](/dynamic-dispatch/), which lets a subclass replace a method. See [class inheritance](/class-inheritance/).

**Reference types and value types.** In Python, JavaScript and Java, a variable of a class type holds a reference, and assigning it copies the reference, so two variables can name one object. In C++, "A class is a user-defined type",[^4] and its objects are values: assignment copies the fields, and a program shares an object through a pointer or a reference. C# and Swift offer both kinds. The C# design guidelines state the choice: "One of the basic design decisions every framework designer faces is whether to design a type as a class (a reference type) or as a struct (a value type)."[^5] Value types avoid a heap allocation and copy on assignment, which suits small values such as points and money. See [value equality and identity](/value-equality-and-identity/).

**Classes at run time.** In Python, JavaScript, Smalltalk and Ruby, a class is an object that a program can pass around, inspect and change, and a program can create new classes while it runs. Java and C# keep a run-time object for each class, which reflection uses to list fields and call methods, but the set of members is fixed when the class is compiled. In C++, a class exists only at compile time, apart from the table of virtual functions and the type information for `typeid` and `dynamic_cast`.

**Methods without classes.** Go and Rust separate the data from the methods. A struct declares the fields, and methods are declared elsewhere, for the type. The Go FAQ answers the question whether Go is object-oriented: "Yes and no. Although Go has types and methods and allows an object-oriented style of programming, there is no type hierarchy."[^6] Methods in Go can belong to any named type, even an integer type, and shared behavior comes from interfaces. Rust adds methods in `impl` blocks and shared behavior through traits. See [interfaces](/interfaces/) and [composition over inheritance](/composition-over-inheritance/).

## In languages

| Language | Verdict | Mechanism |
|---|---|---|
| Python | native | `class`; class objects of type `type`; `self` |
| JavaScript | native | `class` over prototypes; `static` and `#private` fields |
| TypeScript | native | as JavaScript, with access modifiers and typed fields |
| Java | native | classes, `static` members, records |
| Kotlin | native | classes, data classes, companion objects |
| C# | native | classes, structs, records |
| Go | idiom | struct types with methods; no classes |
| Rust | idiom | `struct` and `impl`; traits; no classes |
| C | idiom | `struct` and functions that take a pointer |
| C++ | native | `class` and `struct`, static and `const` members |
| Bash | n/a | no classes |
| PowerShell | native | `class` keyword for .NET classes |
| SQL | idiom | table definitions; user-defined types in some databases |
| Regex | n/a | no types; character classes are a different idea |
| HTML | idiom | custom elements defined by classes |
| CSS | n/a | CSS classes are labels for selectors |

The languages with classes differ in three ways. The first is whether objects are references or values: Python, JavaScript, Java and Kotlin use references for every class, C++ uses values, and C# lets each type choose. The second is how much of a class exists at run time: Python and JavaScript classes are ordinary objects that a program can change, while Java and C# classes are fixed after compilation. The third is how static data is held: Kotlin replaces static members with a companion object, Python and JavaScript store them on the class object, and Go and C use variables at package or file level.

Each example file defines an account with an owner, a balance, a deposit method and data that all accounts share, and makes two instances. The wrong blocks show a mistake that comes from the model of each language: a mutable class attribute that all Python instances share, a JavaScript method that loses its `this`, a C# struct that a list returns as a copy, a Go method that changes a copy of its receiver, and a C++ object that loses its derived part when it is copied into its base class.

## Variations and related ideas

- **Records and data classes.** Java records, C# records, Kotlin data classes and Python dataclasses generate the constructor, equality and string form from a list of fields, for classes that are mainly data. See [immutability](/immutability/).
- **Prototypes.** JavaScript classes are syntax over prototypes, and in Self and early JavaScript, objects inherit directly from other objects without classes. See [prototype-based programming](/prototype-based-programming/).
- **Metaclasses.** In Smalltalk and Python, a class is an instance of a metaclass, which controls how the class is created and how it behaves. Python's `type` is the default metaclass.
- **Open classes.** Ruby, Smalltalk, Python and JavaScript let a program add methods to an existing class at run time. C# extension methods and Kotlin extension functions give a similar syntax without changing the class.
- **Abstract and sealed classes.** An abstract class cannot have instances and leaves some methods to its subclasses; a sealed or final class cannot have subclasses, or only those that it lists.
- **Nested and inner classes.** Java, C# and C++ let a class contain other classes. A Java inner class holds a reference to an instance of the outer class.
- **Singletons and objects without classes.** Kotlin and Scala `object` declarations create one instance with no separate class name, and JavaScript object literals create objects directly.
- **Classes in object-oriented design.** A class is the main unit of design in most object-oriented languages. See [object-oriented programming](/object-oriented-programming/).

## Pitfalls

- **Shared mutable class data.** A list or map stored in a class attribute or a static field is shared by every instance and every thread. The wrong block in the Python file shows a cart whose items appear in every cart.
- **Lost receiver.** A method read from an object and called later can lose its object. The wrong block in the JavaScript file shows `this` as `undefined`.
- **Copies of values.** Changing a copy of a struct, or a method with a value receiver, changes nothing that the caller sees. The wrong blocks in the C# and Go files show both.
- **Slicing.** Copying a derived object into a variable of its base class type in C++ drops the derived part and its overrides. The wrong block in the C++ file shows this.
- **Classes that do too much.** A class with many unrelated fields and methods is hard to change and test. Split it by responsibility, and prefer small classes that hold other objects.
- **Static state.** Static fields act as global variables: they make tests depend on each other and can race between threads. Pass the state in, or hold it in an object that the program creates.
- **Getters and setters for every field.** Public accessors for all fields expose the representation as much as public fields do. Offer operations that keep the invariants of the class, such as `deposit`, rather than `setBalance`.

## Cost

An instance costs the memory of its fields plus a header. In Java, C# and JavaScript, the header holds a pointer to the class or its hidden layout, and in C++ an object of a class with virtual functions holds a pointer to the table of virtual functions; a C++ class without them, a C struct, a Go struct and a Rust struct have no header. Python instances store their attributes in a dictionary, which costs more memory per object, unless the class declares `__slots__`. A method call costs a direct call when the compiler knows the class, which it can then inline, and an indirect call or a cached lookup when it does not. Instances of reference types need a heap allocation, which value types avoid.

## History

Ole-Johan Dahl and Kristen Nygaard introduced classes and objects in Simula 67, for simulation programs in which each object of a class had its own data and its own procedures. Alan Kay and his group at Xerox PARC built Smalltalk in the 1970s, in which everything is an object and every object is an instance of a class, including the classes themselves. Bjarne Stroustrup added classes to C in C with Classes from 1979, which became C++, and Brad Cox and Tom Love created Objective-C in the early 1980s.

Python had classes from its first release in 1991, and Java 1.0, released in 1996, required every method to belong to a class. C# added structs as value types beside its classes in 2002. JavaScript added the `class` syntax over its prototypes in ECMAScript 2015 and private fields in ECMAScript 2022.[^2] Go in 2009 and Rust in 2015 left classes out and put methods on types directly.[^6] C# added records in 2020 and Java in 2021, which made classes for plain data short to write.

## References
