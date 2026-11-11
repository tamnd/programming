---
tag: ETN9ND
kind: concept
slug: class-inheritance
name: Inheritance
aliases: [inheritance, subclassing, derived class, base class, superclass, implementation inheritance, multiple inheritance, diamond problem]
area: objects-and-classes/inheritance
level: core
summary: >-
  Inheritance defines a class as an extension of another: the subclass gets the fields and methods of its base, adds its own, overrides some, and can usually stand in for the base.
applies_to: core-languages
viz: optional
formal: none
history:
  year: 1967
  people: [Ole-Johan Dahl, Kristen Nygaard, Barbara Liskov]
  note: Simula 67 introduced subclasses, and Liskov's 1987 keynote stated the rule that a subtype must behave as its supertype.
see_also: [dynamic-dispatch, subtyping, interfaces, object-oriented-programming, decorator-pattern, sum-types]
same_as:
  wikidata: Q212542
  wikipedia: Inheritance_(object-oriented_programming)
sources:
- url: https://docs.python.org/3/tutorial/classes.html
  title: The Python Tutorial, Classes
  locator: 9.5 Inheritance
- url: https://doc.rust-lang.org/book/ch18-01-what-is-oo.html
  title: The Rust Programming Language, Characteristics of Object-Oriented Languages
  locator: Inheritance as a Type System and as Code Sharing
- url: https://en.cppreference.com/w/cpp/language/derived_class.html
  title: Derived classes, cppreference.com
  locator: Virtual base classes; Public inheritance
- url: https://docs.python.org/3/howto/mro.html
  title: Michele Simionato, The Python 2.3 Method Resolution Order, Python documentation
  locator: The beginning
- url: https://go.dev/doc/faq
  title: Frequently Asked Questions (FAQ), The Go Programming Language
  locator: Why is there no type inheritance?
---

## Idea

Inheritance defines a new class as an extension of an existing one. The subclass, or derived class, gets the fields and methods of its base class, adds its own, and can replace some inherited methods with its own versions. A `Checking` account that inherits from `Account` needs to write only what is different about checking accounts. Inheritance also makes the subclass a subtype in most languages, so a `Checking` can go wherever an `Account` is expected. Simula 67 introduced it, and Smalltalk, C++, Java, C#, Python and JavaScript made it the main tool of object-oriented design. Go and Rust leave it out and use composition and interfaces instead.

```python run id=idea
class Account:
    def __init__(self, owner, balance=0):
        self.owner, self.balance = owner, balance

    def deposit(self, amount):
        self.balance += amount
        return self

    def describe(self):
        return f"{type(self).__name__} of {self.owner}: {self.balance}"


class Savings(Account):  # inherits __init__, deposit and describe
    rate = 0.02

    def add_interest(self):  # adds a method
        return self.deposit(round(self.balance * self.rate))


class Checking(Account):
    def __init__(self, owner, balance=0, overdraft=100):
        super().__init__(owner, balance)  # reuses the initializer of the base
        self.overdraft = overdraft

    def describe(self):  # overrides and extends a method
        return super().describe() + f" (overdraft {self.overdraft})"


accounts = [Savings("ana", 1000), Checking("bo", 50)]
accounts[0].add_interest()
for a in accounts:
    a.deposit(10)
    print(a.describe(), isinstance(a, Account))
print("deposit" in vars(Savings), "add_interest" in vars(Savings))
print([c.__name__ for c in Checking.__mro__])
```

```output for=idea
Savings of ana: 1030 True
Checking of bo: 60 (overdraft 100) True
False True
['Checking', 'Account', 'object']
```

`Savings` defines one method and inherits the rest: `__init__`, `deposit` and `describe` are not in its own dictionary, `vars(Savings)`, but Python finds them in `Account`. `Checking` replaces `__init__` and `describe`, and each replacement calls the version of the base with `super()`, so it extends the inherited code instead of copying it. Both kinds of account pass `isinstance(a, Account)`, and the loop treats them alike. `__mro__` lists the classes in which Python searches for a name, from the class itself to `object`.

## How it works

**What a subclass gets.** A subclass object contains the fields of its base class, usually as a prefix of its own layout, so code compiled for the base finds them at the same offsets. Methods are found through the class: a dynamic language searches the class and then its bases, and a compiled language copies the method table of the base and changes the entries that the subclass overrides. The Python tutorial states the consequence for methods that call each other: "Derived classes may override methods of their base classes. Because methods have no special privileges when calling other methods of the same object, a method of a base class that calls another method defined in the same base class may end up calling a method of a derived class that overrides it."[^1] See [dynamic dispatch](/dynamic-dispatch/).

**Overriding, extending and constructing.** An overriding method can call the method it replaces, with `super()` in Python, Java and JavaScript and `base` in C#, so that the subclass adds to the behavior instead of copying it. Constructors are not inherited in Java and C#, and a subclass constructor must first call a constructor of its base. The base part of the object is built first and the derived part second, and C++ destroys the parts in the reverse order. Access levels control what a subclass can see: `private` members stay in the base, and `protected` members are visible to subclasses but not to other code.

**Two jobs in one mechanism.** Inheritance does two different things. The Rust Book names them: "You would choose inheritance for two main reasons. One is for reuse of code", and "The other reason to use inheritance relates to the type system: to enable a child type to be used in the same places as the parent type."[^2] The C++ reference describes the second: "Public inheritance models the subtyping relationship of object-oriented programming: the derived class object IS-A base class object."[^3] The two jobs can disagree. A class can reuse the code of another class without being a valid substitute for it, and then code that expects the base breaks. See [subtyping](/subtyping/).

**Multiple inheritance.** C++, Python and Eiffel let a class have several base classes. When two bases share a base of their own, the class hierarchy forms a diamond, and the language must decide how many copies of the shared base the object holds and in which order to search the bases. C++ holds one copy for each path, unless the base is virtual: "For each distinct base class that is specified virtual, the most derived object contains only one base class subobject of that type".[^3] Python orders the bases with the C3 linearization, which, its documentation notes, "was invented by people working on Dylan".[^4] Java, C# and Kotlin allow one base class and any number of interfaces, which gives multiple inheritance of types without multiple inheritance of fields.

**The case against inheritance.** A subclass depends on details of its base: which methods call which, and in what order. A change to the base that keeps its documented behavior can still break a subclass, which is the fragile base class problem. The Rust Book gives the other risk: "Inheritance is often at risk of sharing more code than necessary. Subclasses shouldn't always share all characteristics of their parent class but will do so with inheritance."[^2] The Go FAQ explains why Go has no type inheritance: "Object-oriented programming, at least in the best-known languages, involves too much discussion of the relationships between types, relationships that often could be derived automatically."[^5] The usual alternative is composition: the new class holds an object of the old one and forwards the calls that it wants to expose.

## In languages

| Language | Verdict | Mechanism |
|---|---|---|
| Python | native | `class C(A, B)`; C3 method resolution order; `super()` |
| JavaScript | native | `class ... extends` over the prototype chain |
| TypeScript | native | as JavaScript; `abstract` and `protected` |
| Java | native | one superclass with `extends`; interfaces |
| Kotlin | native | `open` classes; one superclass; interfaces |
| C# | native | one base class; `base(...)`; `sealed` |
| Go | absent | struct embedding promotes fields and methods |
| Rust | absent | default trait methods, supertraits, composition |
| C | idiom | the base struct as the first member |
| C++ | native | multiple inheritance; `virtual` bases; access levels |
| Bash | n/a | no classes |
| PowerShell | native | `class Derived : Base` |
| SQL | n/a | no classes in common engines |
| Regex | n/a | no types |
| HTML | n/a | no classes |
| CSS | n/a | property inheritance is a different idea |

The languages fall into three groups. C++ and Python have multiple inheritance, and their example files show the diamond: two copies of the shared base or one virtual base in C++, and one order of the bases, computed by C3, in Python. Java, C#, Kotlin, TypeScript and JavaScript allow one base class, and add interfaces or mixins for the rest. Go and Rust have no inheritance: Go promotes the fields and methods of an embedded struct without making the outer type a subtype, and Rust shares code through default methods of traits and shares data through composition.

C has no classes, but the layout rule that a struct starts with its first member lets a struct play the role of a derived class, and large C code bases, such as the Linux kernel and CPython, are built on it.

## Variations and related ideas

- **Interface inheritance.** A class can inherit only the declarations of an interface and supply all the code itself, which gives the subtype relation without the reuse of code. See [interfaces](/interfaces/).
- **Mixins and traits.** Small classes that add one feature, such as Python mixins, Ruby modules and Scala traits, are combined into a class without forming a deep hierarchy.
- **Prototype-based inheritance.** In JavaScript, Self and Lua, an object inherits directly from another object, its prototype, and classes are a layer over this.
- **Composition and delegation.** A class holds an object and forwards calls to it, which reuses code without making a subtype. See [decorator pattern](/decorator-pattern/).
- **Abstract classes.** A base class that declares methods without code, so that each subclass must supply them, as in the template method pattern.
- **Sealed and final classes.** A class that no other class can extend, or a hierarchy whose subclasses are all known, which a compiler can check like a sum type. See [sum types](/sum-types/).
- **Embedding.** Go and C put one struct inside another and reuse its fields and functions, without subtyping.
- **Object-oriented programming.** Inheritance is one of the classic features of the paradigm, together with encapsulation and dynamic dispatch. See [object-oriented programming](/object-oriented-programming/).

## Pitfalls

- **The fragile base class.** A subclass that overrides one method can depend on how the base calls its own methods. The wrong block of the Java file counts each element twice for this reason. Prefer composition for classes that are not designed for extension, and seal or finalize such classes.
- **Inheritance for reuse alone.** `java.util.Stack` extends `Vector`, so a stack accepts insertion in the middle, and `Properties` extends `Hashtable`, so it accepts keys that are not strings. Inherit only when the subclass is a valid substitute for the base.
- **Shared class attributes.** In Python a mutable class attribute is inherited as the same object, as the wrong block of the Python file shows.
- **Forgetting the base constructor.** In Python a subclass `__init__` that does not call `super().__init__()` leaves the fields of the base unset. JavaScript rejects `this` before `super()`, as the failing block of the JavaScript file shows.
- **Diamonds.** Two paths to the same base give two copies of it in C++, and an ambiguous name, as the failing block of the C++ file shows. Use virtual bases, or avoid diamonds of classes with data.
- **Deep hierarchies.** Each level adds methods that a reader must find in another file. Keep hierarchies shallow, and put shared code in functions or components.
- **Calls to overridable methods from constructors.** The override can run before the subclass has set its fields. See [dynamic dispatch](/dynamic-dispatch/).

## Cost

Inheritance of fields costs nothing at run time: the fields of the base are part of the object, at fixed offsets. Inheritance of methods costs what dynamic dispatch costs, an indirect call through a table, or a lookup along the chain of classes, which dynamic languages cache. Virtual bases in C++ add an indirection to find the shared base, and multiple inheritance adds a pointer adjustment when a pointer converts from the derived class to its second base. The larger costs are at build time and in maintenance: a change to a base class changes the layout or the behavior of every subclass, so in C++ all of them must be recompiled, and a library cannot add a field to a public base class without breaking binary compatibility.

## History

Ole-Johan Dahl and Kristen Nygaard introduced subclasses in Simula 67, where a class could be prefixed by another class and inherit its attributes. Smalltalk, developed at Xerox PARC in the 1970s, built its whole library as one tree of classes with single inheritance. The Flavors system of the MIT Lisp Machine, around 1980, introduced mixins and multiple inheritance, Bertrand Meyer's Eiffel, from 1986, combined multiple inheritance with renaming, and C++ added multiple inheritance and virtual base classes in 1989.

Barbara Liskov's 1987 keynote "Data Abstraction and Hierarchy" stated the rule that a subtype must behave as its supertype, and the 1994 book *Design Patterns* by Gamma, Helm, Johnson and Vlissides advised to favor object composition over class inheritance. Java, in 1995, allowed one superclass and many interfaces. Kim Barrett and others described the C3 linearization for Dylan in 1996, and Python adopted it in 2003.[^4] Go, in 2009, and Rust, in 2015, left inheritance out of their designs.[^5]

## References
