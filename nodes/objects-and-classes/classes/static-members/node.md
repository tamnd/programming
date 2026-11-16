---
tag: FVBDQB
kind: concept
slug: static-members
name: Static members
aliases: [static member, static field, static method, class variable, class method, class attribute, static property, companion object, associated function, associated constant]
area: objects-and-classes/classes
level: core
summary: >-
  A static member belongs to a class, not to its instances: one shared variable, or a function that is called on the class and has no receiver object.
applies_to: core-languages
viz: optional
history:
  year: 1980
  people: [Alan Kay, Dan Ingalls, Adele Goldberg]
  note: Smalltalk-80 gave each class its own class variables and class methods, which live on the metaclass. C++ and Java called the same idea static members.
see_also: [classes-and-instances, method-concept, field-and-property, use-a-singleton, static-factory-method, singleton-pattern, global-data, define-a-class]
same_as:
  wikipedia: Static_variable
sources:
  - url: https://docs.oracle.com/javase/tutorial/java/javaOO/classvars.html
    title: Understanding Class Members, The Java Tutorials
    locator: Class Variables; Class Methods; Constants
  - url: https://learn.microsoft.com/en-us/dotnet/csharp/programming-guide/classes-and-structs/static-classes-and-static-class-members
    title: Static Classes and Static Class Members, C# programming guide
    locator: Static Members
  - url: https://en.cppreference.com/cpp/language/static
    title: static members, cppreference.com
    locator: Explanation; Static member functions; Static data members
  - url: https://docs.python.org/3/library/functions.html
    title: Built-in Functions, The Python Standard Library
    locator: classmethod; staticmethod
  - url: https://en.wikipedia.org/wiki/Smalltalk
    title: Smalltalk, Wikipedia
    locator: Reflection
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Classes/static
    title: static, JavaScript reference, MDN
    locator: Description
  - url: https://doc.rust-lang.org/book/ch05-03-method-syntax.html
    title: Methods, The Rust Programming Language
    locator: Associated Functions
  - url: https://kotlinlang.org/docs/object-declarations.html
    title: Object declarations and expressions, Kotlin documentation
    locator: Companion objects
  - url: https://docs.python.org/3/tutorial/classes.html
    title: 9. Classes, The Python Tutorial
    locator: 9.3.5 Class and Instance Variables
---

## Idea

Most members of a class belong to each instance: each order has its own `id` and its own `amount`. A static member belongs to the class itself. A static field is one variable that all instances share, and a static method is a function that is called on the class and has no instance to work on. The Java tutorial says: "Fields that have the static modifier in their declaration are called static fields or class variables",[^1] and "Every instance of the class shares a class variable, which is in one fixed location in memory."

The usual uses are a counter of the instances, a constant such as a tax rate, a cache that all instances use, and functions that make instances, such as `Order.fromText("50")`. In Python:

```python run id=class-and-instance-members
class Order:
    created = 0  # a class variable: one value for the class, shared by all orders
    TAX_RATE = 0.2  # a constant, by the upper-case naming convention

    def __init__(self, amount):
        Order.created += 1  # change it through the class
        self.id = Order.created  # an instance variable: one value per order
        self.amount = amount

    @classmethod
    def from_text(cls, text):  # gets the class, so a subclass gets its own type
        return cls(int(text))

    @staticmethod
    def is_valid(amount):  # gets neither the class nor an instance
        return amount > 0

    def total(self):
        return self.amount * (1 + Order.TAX_RATE)


class RushOrder(Order):
    pass


a = Order(100)
b = RushOrder.from_text("50")
print(a.id, b.id, Order.created, type(b).__name__)
print(Order.is_valid(-5), a.total())
print(a.created, "created" in vars(a))  # read through the instance, stored on the class
```

```output for=class-and-instance-members
1 2 2 RushOrder
False 120.0
2 False
```

`created` and `TAX_RATE` are on the class, so every order sees the same values. `id` and `amount` are on each order. `from_text` is a class method: it gets the class as `cls`, so `RushOrder.from_text` makes a `RushOrder`. `is_valid` is a static method: it gets neither the class nor an order.

## How it works

- **Storage.** A static field has one place in memory for the whole program, or for each class when a language keeps one per subclass. C# says: "Only one copy of a static member exists, regardless of how many instances of the class are created."[^2] An instance field has one place in each object.
- **No receiver.** A static method gets no object, so it cannot read instance fields unless it gets an object as a parameter. Java says: "class methods cannot use the this keyword as there is no instance for this to refer to."[^1] C++ says: "Static member functions are not associated with any object."[^3] A call such as `Order.fromText("50")` compiles to a plain function call with the class name as a namespace.
- **Class methods with a class receiver.** Python and Smalltalk have a third kind. "A class method receives the class as an implicit first argument, just like an instance method receives the instance."[^4] It is not the same as a Java static method: "Class methods are different than C++ or Java static methods." In Smalltalk, a class is itself an object: "Each class is an instance of the metaclass of that class",[^5] and class methods are the methods of that metaclass.
- **Initialization.** A static field gets its value once, before its first use. Java and C# run the initializers of a class when the class is first used; C++ initializes static data members before `main`, or on first use for a function-local `static`. [Use a singleton](/use-a-singleton/) depends on these rules.
- **Access.** Most languages reach a static member through the class name: `Order.created`, `Order::created`. Java and C++ also allow `a.created` through an instance, C# does not, and in JavaScript "Static properties cannot be directly accessed on instances of the class."[^6]

## In languages

| Language | Verdict | Mechanism |
|---|---|---|
| Python | native | class attributes; `@classmethod` with `cls`; `@staticmethod` |
| JavaScript | native | `static` fields and methods on the class object; `this` is the class in a static method |
| TypeScript | native | as JavaScript, with `private`, `protected` and `readonly` static members |
| Java | native | `static` fields, methods, nested classes and initializer blocks |
| Kotlin | native | `companion object` members; `object` declarations; no `static` keyword |
| C# | native | `static` fields, properties, methods and constructors; `static class`; `const` |
| Go | idiom | package-level variables, constants and functions; no static members |
| Rust | native | associated functions and associated constants in `impl`; `static` items at module level |
| C | idiom | `static` variables at file scope; functions named with a prefix |
| C++ | native | `static` data members and member functions; `static inline` and `static constexpr` |
| Bash | n/a | global variables and functions, no types |
| PowerShell | native | `static` properties and methods in `class` definitions, called with `[Type]::Name` |
| SQL | n/a | a shared value is a row or a sequence, not a member |
| Regex | n/a | no types or members |
| HTML | n/a | no types or members |
| CSS | n/a | no types or members |

- **Rust** has no static fields in a type. An `impl` block holds functions and constants: "All functions defined within an impl block are called associated functions",[^7] and an associated function without `self` plays the part of a static method, as in `String::from`. A shared variable is a `static` item in the module.
- **Kotlin** has no `static` keyword. "Companion objects allow you to define class-level functions and properties tied to a class",[^8] and code calls them on the class name, as in `Order.fromText("50")`.
- **Go** has no classes. The package is the unit that owns shared state, so a package-level variable plays the part of a static field, and a function such as `NewOrder` plays the part of a static factory method.

## Variations and related ideas

- **Static classes and modules.** A C# `static class` holds only static members, and "a static class can't be instantiated".[^2] `System.Math` is one. A Python or JavaScript module, a Go package and a Rust module do the same job without a class.
- **Static factory methods.** A static method that returns an instance, such as `Order.fromText` or Rust `Order::new`, can have a name, can return a cached instance and can return a subclass. See [static factory method](/static-factory-method/).
- **Constants.** A Java `static final` field, a C# `const`, a C++ `static constexpr` member and a Rust associated `const` are static members whose value cannot change. They are the safest kind, because shared state that does not change needs no lock.
- **Per-thread statics.** C# `[ThreadStatic]`, Java `ThreadLocal`, C++ `thread_local` and Rust `thread_local!` give each thread its own copy: "Static fields marked with ThreadStaticAttribute have one copy per thread."[^2]
- **Singletons and global data.** A static field that holds an object is the base of [the singleton pattern](/singleton-pattern/). A changing static field is [global data](/global-data/) with a class name in front of it.

## Pitfalls

- **Assignment through an instance in Python.** `self.count += 1` reads the class variable, but it stores the sum in an attribute of the instance, so the class variable does not change. A mutable class attribute has the opposite problem: `self.names.append(x)` changes the one list that all instances share. The Python tutorial warns about such a list: it is a "mistaken use of a class variable",[^9] because "a single list would be shared by all Dog instances".

```python run id=instance-hides-class-variable
class Counter:
    count = 0
    names = []  # one list for the class and every instance

    def __init__(self, name):
        self.count += 1  # reads Counter.count, then stores the sum on the instance
        self.names.append(name)  # changes the shared list


a = Counter("a")
b = Counter("b")
print(Counter.count, a.count, b.count)
print(Counter.names, a.names is b.names)
```

```output for=instance-hides-class-variable
0 1 1
['a', 'b'] True
```

- **`cls` in a Python class method is the subclass.** "If a class method is called for a derived class, the derived class object is passed as the implied first argument."[^4] So `cls.created += 1` makes a separate counter on each subclass. Write the class name, `Order.created += 1`, for one counter.
- **A static method does not override.** A Java or C# static method with the same name in a subclass hides the method of the base class. A call is bound by the declared type at compile time, not by the object at run time.
- **Shared state between threads.** Two threads that do `created++` on one static field can lose an update. Use an atomic counter or a lock, or keep the field constant. Rust makes this a compile error: a `static` must be safe to share, so a counter is an atomic type.
- **Hidden dependencies and tests.** A method that reads a static field depends on a value that does not show in its parameters, and that keeps its value from one test to the next. See the pitfalls of [Use a singleton](/use-a-singleton/).
- **Initialization order in C++.** Static data members in different source files are initialized in an order that the standard does not fix. One that uses another in its initializer can see it before it has its value.

## Cost

A static field is at a fixed address, so a read or a write costs the same as for a global variable, with no object to load first. A static method call is a direct call with no receiver and no dynamic dispatch, so the compiler can inline it. The first use of a class in Java or C# can run its static initializer, and a C++ function-local `static` has a guard check at each call; after the first call, these checks cost little. In Python and JavaScript, a class attribute is found by a lookup at run time, as for other attributes. Memory: a static field takes its space once, not once per instance.

## History

Smalltalk-80 made classes objects: "Smalltalk-80 added metaclasses",[^5] so a class could have its own variables and methods, which Smalltalk calls class variables and class methods. C++ used the keyword `static` for the same idea, because a static data member has static storage duration, like a `static` variable in C. cppreference puts it this way: "Static members of a class are not associated with the objects of the class".[^3] A static data member is an independent variable, and a static member function is a regular function in the scope of the class. Java and C# kept the C++ keyword, and Python added `classmethod` and `staticmethod` as built-in functions. JavaScript added static methods with classes, and later static fields; MDN gives the usual jobs: "Static methods are often utility functions, such as functions to create or clone objects, whereas static properties are useful for caches, fixed-configuration, or any other data you don't need to be replicated across instances."[^6] Kotlin, Go and Rust dropped the keyword and use companion objects, packages and modules instead.

## References
