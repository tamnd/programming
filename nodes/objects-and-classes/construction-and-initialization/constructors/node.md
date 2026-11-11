---
tag: PV48PE
kind: concept
slug: constructors
name: Constructors and initialization
aliases: [constructor, initializer, object initialization, named constructor, factory function, zero value, init function]
area: objects-and-classes/construction-and-initialization
level: core
summary: >-
  A constructor runs when a program creates an object and puts the object in a valid state, or refuses to create it. Languages without constructors use zero values and creation functions.
applies_to: core-languages
viz: optional
formal: none
history:
  year: 1967
  people: [Ole-Johan Dahl, Kristen Nygaard, Bjarne Stroustrup]
  note: Simula 67 ran the body of a class when it created an object, and C++ made the constructor a named member of the class.
see_also: [encapsulation-and-visibility, class-inheritance, dynamic-dispatch, immutability, resource-acquisition-is-initialization, factory-method-pattern, builder-pattern, exception-handling]
same_as:
  wikidata: Q1049476
  wikipedia: Constructor_(object-oriented_programming)
sources:
- url: https://docs.python.org/3/reference/datamodel.html#object.__new__
  title: Data model, The Python Language Reference
  locator: 3.3.1 Basic customization, object.__new__ and object.__init__
- url: https://docs.oracle.com/javase/specs/jls/se25/html/jls-12.html#jls-12.5
  title: Chapter 12, Execution, The Java Language Specification, Java SE 25 Edition
  locator: 12.5 Creation of New Class Instances
- url: https://en.cppreference.com/cpp/language/initializer_list
  title: Constructors and member initializer lists, cppreference.com
  locator: Initialization order
- url: https://go.dev/doc/effective_go#allocation_new
  title: Effective Go
  locator: Allocation with new; Constructors and composite literals
- url: https://doc.rust-lang.org/book/ch05-03-method-syntax.html
  title: Using Structs to Structure Related Data, Method Syntax, The Rust Programming Language
  locator: Associated Functions
---

## Idea

A constructor is the code that runs when a program creates an object, and its job is to bring the new object into a valid state. Creation has two steps: the runtime allocates memory for the object, and the constructor sets its fields. A constructor that checks its arguments can refuse to finish, so code that holds an object can rely on the rules that the constructor checked, for example that a range never has its low end above its high end. Python, JavaScript, Java, C#, Kotlin, C++ and PowerShell have constructors in the language. Go, Rust and C have none, and use conventions in their place: zero values, struct literals, and ordinary functions with names such as `NewRange`, `new` or `range_init`.

```python run id=idea
class Range:
    """Invariant: lo <= hi."""

    def __new__(cls, *args):  # step 1: allocate a blank object
        print("allocate", cls.__name__)
        return super().__new__(cls)

    def __init__(self, lo, hi):  # step 2: initialize it and check the invariant
        if lo > hi:
            raise ValueError(f"empty range {lo}..{hi}")
        self.lo, self.hi = lo, hi
        print("initialized", self.lo, self.hi)

    @classmethod
    def parse(cls, text):  # a named constructor: another way in, same checks
        lo, hi = text.split("..")
        return cls(int(lo), int(hi))

    def __len__(self):
        return self.hi - self.lo + 1


r = Range(3, 7)
print(len(r))
s = Range.parse("10..12")
print(len(s))

try:
    Range(9, 1)  # the constructor refuses: no broken Range ever exists
except ValueError as e:
    print("refused:", e)
```

```output for=idea
allocate Range
initialized 3 7
5
allocate Range
initialized 10 12
3
allocate Range
refused: empty range 9..1
```

Python shows the two steps as two methods. `__new__` allocates a blank object of the class, and `__init__` checks the arguments and sets the fields. `Range.parse` is a second way to create a `Range`, from text, and it ends by calling the class, so it goes through the same checks. `Range(9, 1)` allocates an object, but `__init__` raises an exception, and the caller never receives a `Range` whose low end is above its high end.

## How it works

**Allocate, then initialize.** Every object starts as raw memory, and the constructor turns it into a value of its class. The Python documentation describes `__new__` as "Called to create a new instance of class cls." and states the split: "Because __new__() and __init__() work together in constructing objects (__new__() to create it, and __init__() to customize it), no non-None value may be returned by __init__()".[^1] Java, C# and JavaScript do the allocation in `new` and give the constructor an object whose fields hold default values. C++ separates the steps in its syntax: `new` allocates and then calls the constructor, and a local object gets its memory on the stack. Go, Rust and C have no constructor step, so the code that allocates must also give the fields their values.

**Invariants start in the constructor.** An invariant is a rule about the fields of an object that every method can assume, such as `lo <= hi` or a buffer length that never exceeds its capacity. If the constructor establishes the invariant and every method keeps it, the invariant holds for the whole life of the object, and no method needs to check it again. This is the reason for private fields: when code outside the class cannot set the fields, the constructor and the methods are the only code that can break the rule. See [encapsulation](/encapsulation-and-visibility/). A constructor that cannot establish the invariant must stop the creation: it throws an exception in Python, Java, C#, JavaScript and C++, and the equivalent function returns an error in Go, Rust and C. See [exception handling](/exception-handling/) and [errors as values](/errors-as-values/).

**The order of initialization.** An object of a subclass has fields from each class in its hierarchy, and the language fixes the order in which they get their values. Java runs the superclass constructor, then the field initializers of the class, then the rest of the constructor body. The Java specification describes the steps that run "Just before a reference to the newly created object is returned as the result".[^2] C++ builds the base classes first and then the members, and cppreference warns that the order in the constructor has no effect: "The order of member initializers in the list is irrelevant", because "non-static data member are initialized in order of declaration in the class definition", and "Finally, the body of the constructor is executed."[^3] The destructor destroys the members in the reverse order, and a fixed order makes that possible. C# runs the field initializers of a class before the base constructor, and JavaScript runs the field initializers of a derived class right after `super()` returns.

**Named constructors and factories.** A language with constructors gives each class one name for creation, the class name, and overloads separate the variants only by the types of their parameters. A static method or a class method gives a variant a name of its own, such as `Range.parse`, `Duration.ofSeconds` or `Money.of`, and can return a cached object, a subclass, or an error in place of a new object. Constructors can also call each other: `this(...)` in Java, `: this(...)` in C# and a delegating constructor in C++ let the simple variants pass their work to the general one. See [the factory method pattern](/factory-method-pattern/) and [the builder pattern](/builder-pattern/) for designs that move creation out of the class.

**Zero values and defaults.** Go gives every variable the zero value of its type, and its documentation asks types to make that value useful: "Since the memory returned by new is zeroed, it's helpful to arrange when designing your data structures that the zero value of each type can be used without further initialization."[^4] When a type needs set-up, a function named by convention `NewX` creates it. Rust requires a struct literal to give every field, so no field is ever unset, and it states that a constructor is only a convention: "Associated functions that aren’t methods are often used for constructors that will return a new instance of the struct. These are often called new, but new isn’t a special name and isn’t built into the language."[^5] Java and C# fill fields with zero, `false` or `null` before the constructor runs, which means that a field can be read before the constructor gives it a value. C leaves the members of a local variable indeterminate unless the program initializes them.

**Partly built objects.** While a constructor runs, the object is not yet valid, and code that sees it at that time can see broken state. The main way this happens is a call from a constructor to a method that a subclass overrides. In Java, C# and JavaScript, the call reaches the override, which runs before the constructor of the subclass has set its own fields. In C++, the call reaches the version of the class whose constructor is running, because the object counts as an object of that class until its constructor returns. Passing `this` to other code from a constructor, for example to register a listener, has the same risk, and in a concurrent program another thread can see the fields before they are set. See [dynamic dispatch](/dynamic-dispatch/).

## In languages

| Language | Verdict | Mechanism |
|---|---|---|
| Python | native | `__new__` and `__init__`; class methods as named constructors |
| JavaScript | native | `constructor`, `super()`, field initializers, static methods |
| TypeScript | native | as JavaScript, with parameter properties |
| Java | native | constructors, `this(...)`, initializer blocks, record compact constructors |
| Kotlin | native | primary constructors, `init` blocks, secondary constructors |
| C# | native | constructors, `: this(...)`, object initializers, `required` members |
| Go | idiom | zero values and functions named `NewX` |
| Rust | idiom | struct literals, `new` functions, the `Default` trait |
| C | idiom | init and create functions, designated initializers |
| C++ | native | constructors with member initializer lists |
| Bash | n/a | no objects |
| PowerShell | native | class constructors, `base()` |
| SQL | idiom | `DEFAULT`, `NOT NULL` and `CHECK` on new rows |
| Regex | n/a | no objects |
| HTML | idiom | custom element constructors and attributes |
| CSS | n/a | no objects |

The table shows two families. Python, JavaScript, Java, C#, Kotlin, C++ and PowerShell have a constructor that the language calls on every creation, and they differ in the order in which the fields of a class hierarchy get their values. Go, Rust and C have no such hook: the code that creates a value must give the fields their values, and a library offers a function that does this and checks the arguments. Go relies on zero values that work without set-up, and Rust relies on the rule that a struct literal names every field, together with private fields that force code in other modules to call `new`.

Each example file shows the checks of a constructor, a second way to create the same type, and one mistake. The mistakes differ: a shared default value in Python, a call to an override from a base constructor in Java and JavaScript, a missing required member in C#, a nil map in a zero value in Go, and a missing field in Rust. The C++ file shows that the members of a class are built in the order of their declaration, and that a constructor that throws stops the creation after the members already built.

## Variations and related ideas

- **Default, copy and move constructors.** C++ creates these for a class unless the class declares its own, and they decide how objects are created without arguments, copied and moved. See [ownership](/ownership/).
- **Primary constructors.** Kotlin, Scala and C# can declare the constructor parameters in the class header, and records in Java and C# derive the constructor from the component list.
- **Object initializers and builders.** C# object initializers and Kotlin named arguments set many properties at creation without a constructor for each combination. A builder collects the settings step by step and checks them in one final call. See [the builder pattern](/builder-pattern/).
- **Two-phase initialization.** Objective-C separates `alloc` and `init` into two messages, and Swift checks in its compiler that an initializer sets every stored property before it uses `self`.
- **Destructors and finalizers.** The opposite of a constructor runs when an object goes away. In C++ and Rust it runs at a known point, which lets constructors acquire resources that destructors release. See [resource acquisition is initialization](/resource-acquisition-is-initialization/).
- **Factories and dependency injection.** A factory or a container creates objects and passes them their dependencies, so the class does not choose its collaborators. See [the factory method pattern](/factory-method-pattern/).
- **Lazy initialization.** A field gets its value on first use, not in the constructor, as `Counter` does with its map in the Go file.
- **Prototypes.** In a language without classes, a new object copies a prototype or delegates to it, and a function sets its fields. See [prototype-based programming](/prototype-based-programming/).
- **Immutable objects.** An object whose fields cannot change after construction needs all its values in the constructor, so the constructor is the only place to check them. See [immutability](/immutability/).

## Pitfalls

- **Calls to overridable methods.** A base constructor that calls an overridden method runs code of the subclass before the subclass has set its fields. The wrong blocks in the Java and JavaScript files print `null` and `undefined`, and the C++ file shows a call that never reaches the override. Call only private or final methods from a constructor.
- **Shared default values.** A default argument that is a mutable object, such as a list in Python, is created once and shared by every object made without that argument. The wrong block in the Python file shows this.
- **Work in constructors.** A constructor that opens network connections, reads files or starts threads is slow, hard to test and can fail in ways that leave resources open. Keep constructors to checks and assignments, and pass in the resources the object needs.
- **Leaking `this`.** Code that stores `this` in a global list or passes it to another thread from inside the constructor exposes an object that is not ready. Register the object after construction.
- **Initialization order across members.** In C++ a member initializer that reads another member reads it before its construction when the other member comes later in the class. Declare members in the order in which they depend on each other, and let the compiler warn about mismatches.
- **Unusable zero values.** In Go, a zero value that holds a nil map panics on the first write, and the failing block in the Go file shows this. Make zero values usable, or make the type unexported and offer a `NewX` function.
- **Half-built objects in C.** A create function that fails part way must free what it already allocated, or it leaks. Free in the reverse order of allocation, as `buffer_create` in the C file does.
- **Null fields.** In Java and C#, a field that the constructor forgets to set stays `null`. Use `final` and `readonly` fields, required members or nullable reference types, so that the compiler finds the omission. See [null references](/null-references/).

## Cost

A constructor costs one call plus the work of its body, and compilers inline small constructors, so creating an object costs about as much as writing its fields. The allocation is a separate cost: a heap allocation and later a collection or a free for objects on the heap, nothing more than a stack adjustment for local objects in C++, Rust, Go and C. Java and C# write the zero values first and then the real values, which writes each field twice unless the compiler removes the first store. Checks in a constructor cost a comparison or two per creation, and they save the cost of checks in every method. Factory functions that return cached objects avoid allocations for common values, as `Integer.valueOf` in Java does for small numbers.

## History

Simula 67 gave each class a body of statements that ran when the program created an object, and this body did the work of a constructor. Smalltalk-80 created objects by sending a message such as `new` to a class, so creation was an ordinary method of the class. Bjarne Stroustrup added constructors and destructors to C in C with Classes, a project that he began in 1979, and C++ named the constructor after its class and added member initializer lists. Objective-C used separate `alloc` and `init` messages.

Java, from 1996, took the C++ form of the constructor and added the rule that every field has a default value before the constructor runs. C#, from 2002, did the same and added object initializers in 2007 and required members in 2022. Python 2.2, in 2001, added `__new__` to the existing `__init__`. Go, from 2009, and Rust, from 2015, left constructors out of the language, Go in favor of zero values and Rust in favor of struct literals and `new` functions by convention. Java 25, in 2025, let a constructor run statements before its call to `super(...)`, so a constructor can check its arguments before the superclass constructor runs.

## References
