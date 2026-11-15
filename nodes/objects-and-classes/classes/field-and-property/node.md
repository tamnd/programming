---
tag: 01BRCM
kind: concept
slug: field-and-property
name: Field and property
aliases: [field, property, data member, instance variable, attribute, member variable, computed property, backing field, accessor, getter and setter, auto-property, stored property]
area: objects-and-classes/classes
level: intro
summary: >-
  A field is a variable stored in each object. A property looks like a field to the code that uses it, but reads and writes run accessor code, which can check, compute or log the value.
applies_to: core-languages
viz: optional
formal: none
history:
  year: 1995
  people: [Anders Hejlsberg]
  note: Delphi's Object Pascal had property declarations with read and write accessors, and C#, also led by Hejlsberg, made properties a central part of the language in 2002.
see_also: [classes-and-instances, method-concept, define-a-class, define-a-method-on-a-type, encapsulation-and-visibility, immutability]
same_as:
  wikipedia: Property_(programming)
sources:
  - url: https://docs.oracle.com/javase/specs/jls/se25/html/jls-8.html
    title: Chapter 8, Classes, The Java Language Specification, Java SE 25 Edition
    locator: 8.3 Field Declarations
  - url: https://go.dev/ref/spec
    title: The Go Programming Language Specification
    locator: Struct types
  - url: https://en.wikipedia.org/wiki/Property_(programming)
    title: Property (programming), Wikipedia
    locator: Introduction; Support in languages
  - url: https://learn.microsoft.com/en-us/dotnet/csharp/programming-guide/classes-and-structs/properties
    title: Properties, C# programming guide
    locator: Introduction; Field backed properties; Auto-implemented properties
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Functions/get
    title: get, MDN Web Docs
    locator: Description
  - url: https://docs.python.org/3/howto/descriptor.html
    title: Descriptor Guide, Python HOWTOs
    locator: Properties; Managed attributes
  - url: https://docs.swift.org/swift-book/documentation/the-swift-programming-language/properties/
    title: Properties, The Swift Programming Language
    locator: Stored Properties; Computed Properties; Lazy Stored Properties; Property Observers
  - url: https://kotlinlang.org/docs/properties.html
    title: Properties, Kotlin documentation
    locator: Declaring properties; Backing fields
---

## Idea

A field is a variable that each object stores: a rectangle has a `width` field and a `height` field, and each rectangle holds its own values. A property is a member that the calling code reads and assigns like a field, but each read or assignment runs a function, an accessor. The function can return a stored value, compute a value, check a value before it stores it, or record the change. The value that a property stores is held in a field, its backing field. Python, C#, Kotlin, Swift and JavaScript have properties. Java, C++, Go and Rust have only fields, and code writes accessor methods by hand:

```python run id=idea
class Temperature:
    def __init__(self, celsius):
        self.celsius = celsius  # this assignment goes through the setter

    @property
    def celsius(self):  # a property: code runs on each read
        return self._celsius  # the backing field

    @celsius.setter
    def celsius(self, value):
        if value < -273.15:
            raise ValueError("below absolute zero")
        self._celsius = value

    @property
    def fahrenheit(self):  # a computed property: no storage of its own
        return self._celsius * 9 / 5 + 32


t = Temperature(20)
print(t.celsius, t.fahrenheit)
t.celsius = 30  # reads like a field assignment
print(t.celsius, t.fahrenheit)
try:
    t.celsius = -300
except ValueError as e:
    print("ValueError:", e)
print(vars(t))  # the instance stores only the backing field
```

```output for=idea
20 68.0
30 86.0
ValueError: below absolute zero
{'_celsius': 30}
```

`celsius` is a property with a check, and `_celsius` is its backing field. `fahrenheit` is a computed property: it has no storage and is computed from `_celsius` on each read. The calling code uses `t.celsius` and `t.fahrenheit` as if they were fields, and `vars(t)` shows that the instance stores one value.

## How it works

**A field is storage in the object.** The JLS states it for Java: "The variables of a class are introduced by field declarations."[^1] Go describes a struct in the same terms: "A struct is a sequence of named elements, called fields, each of which has a name and a type."[^2] A field read is a load from a fixed offset in the object, and a field write is a store, with no code of the class in between. A field can be an instance field, with one value in each object, or a static field, with one value for the class. C++ calls fields data members, and Python calls them attributes.

**A property runs code on access.** Wikipedia describes a property as "a special sort of class member, intermediate in functionality between a field (or data member) and a method".[^3] The C# guide gives the purpose: "A property is a member that provides a flexible mechanism to read, write, or compute the value of a data field."[^4] The compiler, or the runtime in Python and JavaScript, turns `t.celsius` into a call of the getter and `t.celsius = 30` into a call of the setter. In JavaScript, "The get syntax binds an object property to a function that will be called when that property is looked up."[^5] In Python, "Calling property() is a succinct way of building a data descriptor that triggers a function call upon access to an attribute."[^6]

**Stored and computed properties.** Swift names the two kinds: "Stored properties store constant and variable values as part of an instance, whereas computed properties calculate (rather than store) a value."[^7] A computed property, such as an area or a full name, has a getter and often no setter, and it never gets out of date, because nothing stores it.

**Backing fields.** A stored property needs a field to hold the value. C# and Kotlin generate it when the accessors are the default ones. For a C# auto-implemented property, "The compiler generates a hidden backing field for the property."[^4] Kotlin does the same: "The compiler automatically generates backing fields for properties when a value needs to be stored in memory", and a custom accessor reaches the generated field with the keyword `field`.[^8] C# 14 adds the same keyword: "The field keyword accesses the compiler synthesized backing field for a property."[^4] Without such a keyword, the code declares the backing field by hand, often with a name such as `_celsius`.

**Uniform access.** Because a property and a field look the same to the calling code, a class can start with a public field and change it to a property later, when it needs a check or a computed value, without changes to the code that uses it. This holds at the source level in Python, JavaScript, Kotlin and Swift. In C# it does not hold for compiled code: a field and a property compile to different instructions, so the change breaks binary compatibility, and C# code makes public members properties from the start. In Java, C++, Go and Rust, a field and a method call have different syntax, so code that may need a check later hides the field behind accessor methods from the start. See [encapsulation and visibility](/encapsulation-and-visibility/).

## In languages

| Language | Verdict | Mechanism |
|---|---|---|
| Python | native | attributes as fields; `@property` with a getter, a setter and a deleter; `__slots__` |
| JavaScript | native | public and `#private` fields; `get` and `set` accessors |
| TypeScript | native | as JavaScript, with typed fields, `readonly` and access modifiers |
| Java | idiom | fields; `getX` and `setX` methods; record accessors |
| Kotlin | native | every `val` and `var` member is a property; `field` in custom accessors |
| C# | native | fields; properties with `get`, `set` and `init`; auto-implemented properties |
| Go | idiom | struct fields; exported names; getter methods without a `Get` prefix |
| Rust | idiom | struct fields; `pub` for visibility; accessor methods |
| C | idiom | struct members; functions that read and write them |
| C++ | idiom | data members; accessor member functions |
| Bash | n/a | variables only |
| PowerShell | native | class properties; `Add-Member` script properties |
| SQL | idiom | columns of a table; computed or generated columns |
| Regex | n/a | no data members |
| HTML | idiom | attributes of elements, and DOM properties that reflect them |
| CSS | idiom | properties that style elements, a different idea; custom properties |

The languages differ in whether property syntax exists and in what a field declaration means. In Kotlin and Swift every member variable is a property, and the compiler adds the field only when needed. In C#, fields and properties are separate kinds of member. In Python and JavaScript, an instance can gain fields at run time by assignment, while a property is defined once, on the class or the prototype. Java, C++, Go and Rust have only fields, and code that needs a check writes methods.

Each example file defines a rectangle with a `width`, a `height` and an `area`, makes `area` a computed value, and checks the width where the language allows it.

## Variations and related ideas

- **Read-only and init-only properties.** A property with a getter only, a Kotlin `val`, a C# `init` accessor and a Java record component allow a value to be set once, at construction. See [immutability](/immutability/).
- **Lazy properties.** "A lazy stored property is a property whose initial value isn't calculated until the first time it's used."[^7] Kotlin has `by lazy`, and Python has `functools.cached_property`.
- **Property observers.** Swift's `willSet` and `didSet` run code before and after a stored property changes, and data binding in user interface frameworks uses notifications such as `INotifyPropertyChanged` in .NET.
- **Getters and setters.** In languages without property syntax, a pair of methods plays the part of a property. Wikipedia notes that "Some object-oriented languages, such as Java and C++, do not support properties, requiring the programmer to define a pair of accessor and mutator methods instead."[^3]
- **Indexers.** C# indexers and Python's `__getitem__` let an object be read like an array, with the same idea of accessor code behind the syntax.
- **Static fields and properties.** A field or property can belong to the class and have one value for all instances. See [classes and instances](/classes-and-instances/).

## Pitfalls

- **A setter that calls itself.** In a Python property named `celsius`, the setter must store into `self._celsius`, not `self.celsius`, or each assignment calls the setter again until the recursion limit. The same mistake in C# makes a setter `set { Width = value; }` recurse into a stack overflow.
- **Expensive or surprising getters.** Code that reads a property expects a cheap read with no side effects. A getter that runs a query, allocates a large object or changes state belongs in a method with a name that says what it does.
- **Public mutable fields.** A public field lets any code break the invariants of the object, such as a negative width. Make it private and offer methods or a property with a check.
- **A field and a property with the same data.** A class that stores both the area and the sides can get them out of step. Compute the derived value in a property instead.
- **Getters and setters for every field.** A getter and a setter that pass every value through expose the representation as much as a public field does. Offer operations that keep the invariants of the class.
- **Assigning to a read-only property.** In JavaScript, assigning a property that has a getter and no setter throws a `TypeError` in strict code and is ignored in sloppy code. The JavaScript example shows the error.

## Cost

A field read or write is one memory access at a fixed offset. A property access is a function call, but compilers and JIT compilers inline small accessors in C#, Java, Kotlin, Swift, C++, Go and Rust, so a trivial getter usually costs the same as a field read. In Python, a property read calls a Python function, which costs more than a plain attribute read. Per object, a field costs its size plus any padding for alignment, a computed property costs no memory, and Python instances without `__slots__` also pay for an attribute dictionary.

## History

Simula 67 and Smalltalk objects held instance variables, and Smalltalk code read them from outside only through methods. Object Pascal in Delphi, released in 1995, declared properties with `read` and `write` accessors, and Anders Hejlsberg, the chief architect of Delphi, later led the design of C#, which made properties a basic kind of member. Python added `property` in version 2.2, together with descriptors, Kotlin and Swift made every member variable a property, and JavaScript added `get` and `set` in ECMAScript 5 and class fields in ECMAScript 2022.

## References
