---
tag: MZ951T
kind: concept
slug: dynamic-dispatch
name: Dynamic dispatch
aliases: [virtual method call, virtual function, late binding, method overriding, vtable, virtual method table, single dispatch, multiple dispatch]
area: objects-and-classes/polymorphism-and-dispatch
level: core
summary: >-
  Dynamic dispatch chooses the code for a call at run time from the type of a value, usually the receiver, which lets one call site run the overriding method of each subclass.
applies_to: core-languages
viz: optional
formal: none
history:
  year: 1967
  people: [Ole-Johan Dahl, Kristen Nygaard, Alan Kay, Bjarne Stroustrup]
  note: Simula 67 introduced virtual procedures that a subclass could redefine, and Smalltalk made every call a message looked up at run time.
see_also: [object-oriented-programming, interfaces, subtyping, parametric-polymorphism, sum-types, pattern-matching, just-in-time-compilation, decorator-pattern]
same_as:
  wikidata: Q1268939
  wikipedia: Dynamic_dispatch
sources:
- url: https://doc.rust-lang.org/book/ch18-02-trait-objects.html
  title: The Rust Programming Language, Using Trait Objects to Abstract over Shared Behavior
  locator: Performing Dynamic Dispatch
- url: https://en.cppreference.com/w/cpp/language/virtual.html
  title: virtual function specifier, cppreference.com
  locator: Explanation; During construction and destruction
- url: https://docs.oracle.com/javase/specs/jls/se25/html/jls-15.html
  title: The Java Language Specification, Java SE 25 Edition, Chapter 15
  locator: 15.12.4.4 Locate Method to Invoke
- url: https://docs.python.org/3/library/functools.html
  title: 'Python documentation: functools, Higher-order functions and operations on callable objects'
  locator: functools.singledispatch
---

## Idea

Dynamic dispatch chooses the code for a call at run time, from the type of a value, usually the object that receives the call. One call site such as `s.area()` runs the area method of a circle for one object and of a square for the next, and the code at the call site does not know which. Static dispatch makes the choice at compile time instead, from the types that the compiler sees. Dynamic dispatch is the mechanism behind virtual methods, interfaces, trait objects and overriding in object-oriented languages.

```python run id=idea
def make_class(name, base=None, **methods):  # a class: a name, a base and a method table
    return {"name": name, "base": base, "methods": methods}


def send(obj, selector, *args):  # dynamic dispatch: find the code at run time
    cls = obj["class"]
    while cls is not None:
        if selector in cls["methods"]:
            return cls["methods"][selector](obj, *args)
        cls = cls["base"]
    raise AttributeError(f"{obj['class']['name']} does not understand {selector}")


Shape = make_class(
    "Shape",
    kind=lambda self: "shape",
    describe=lambda self: f"{send(self, 'kind')} with area {send(self, 'area'):.2f}",
)
Circle = make_class("Circle", Shape,
                    kind=lambda self: "circle",
                    area=lambda self: 3.14159 * self["r"] ** 2)
Square = make_class("Square", Shape, area=lambda self: self["side"] ** 2)

shapes = [{"class": Circle, "r": 1.0}, {"class": Square, "side": 2}]
for s in shapes:
    print(send(s, "describe"))  # one call site, two different area methods

try:
    send(shapes[1], "perimeter")
except AttributeError as e:
    print(e)
```

```output for=idea
circle with area 3.14
shape with area 4.00
Square does not understand perimeter
```

A class here is a table of methods with a link to its base class. `send` finds the method at the time of the call: it looks in the class of the object, then in each base class in turn, and calls the first method with the right name. `describe` is defined once, in `Shape`, and it sends `area` and `kind` back to the same object, so a circle runs its own `area` and `kind`, while a square runs its own `area` and the default `kind` of `Shape`. A name that no class in the chain defines is an error at run time.

## How it works

**Static and dynamic dispatch.** A call names an operation, and something must choose the code that runs. The Rust Book states the difference: static dispatch "is when the compiler knows what method you're calling at compile time. This is opposed to dynamic dispatch, which is when the compiler can't tell at compile time which method you're calling."[^1] Calls to plain functions, to overloaded functions and to generic functions that the compiler copies for each type are static. Calls through a base class, an interface or a trait object are dynamic, because the compiler sees only the declared type and the real type is known only at run time.

**Overriding.** A subclass can replace a method of its base class with its own version. The C++ reference describes virtual functions as "member functions whose behavior can be overridden in derived classes. As opposed to non-virtual functions, the overriding behavior is preserved even if there is no compile-time information about the actual type of the class."[^2] Java makes every instance method virtual unless it is `static`, `private` or `final`; the Java specification notes that "a subclass may override a specific method already declared in a parent class so that this overriding method is invoked".[^3] C#, C++ and Kotlin make methods static by default and dispatch only on methods marked `virtual`, `abstract` or `open`.

**Virtual method tables.** Compiled languages implement dispatch with a table. Each class has a table of pointers to the code of its virtual methods, and each object holds a hidden pointer to the table of its class. A call loads the table pointer from the object, loads the entry for the method at a fixed slot, and calls the address that it finds. A subclass copies the table of its base, replaces the entries of the methods it overrides and adds entries for new methods, so a slot number is valid for the whole hierarchy. Rust and Go keep the table pointer outside the object: a trait object or an interface value is a pair of a data pointer and a table pointer, and the same value can have a different table for each trait. The C example file builds such a table by hand.

**Lookup by name.** Python, Ruby, Smalltalk and JavaScript find a method by its name at the time of the call. Python searches the classes of the method resolution order, and JavaScript searches the prototype chain. Because the lookup happens on each call, a program can add or replace a method while it runs. A search for every call would be slow, so implementations cache the result: a method cache stores the method for each pair of class and name, and an inline cache stores at each call site the class that it saw last and the method that it found.

**Late binding of self.** A method of a base class that calls another method on `self` or `this` also dispatches, so the base class can call code that a subclass supplies later. `describe` in the example calls `area`, which only the subclasses define. This is called open recursion, and the template method pattern depends on it. It is also a hazard: a change to the calls that a base class makes to its own methods can break a subclass that overrides one of them, which is the fragile base class problem. Constructors are a special case. In C++, during construction and destruction "the function called is the final overrider in the constructor's or destructor's class and not one overriding it in a more-derived class",[^2] while Java, C# and JavaScript dispatch to the override before the fields of the subclass are set, as the wrong block of the JavaScript file shows.

**Single, double and multiple dispatch.** In most object-oriented languages only the receiver selects the method: the other arguments choose an overload at compile time, from their static types. The Java example file shows one object that selects two different overloads. To choose on the types of two arguments, a program can dispatch twice, which is the visitor pattern, or use a language with multiple dispatch, such as Common Lisp, Julia or Dylan, where a generic function selects a method from the types of all its arguments. Python's `functools.singledispatch` dispatches a plain function on one argument. Its documentation says that the decorator will "Transform a function into a single-dispatch generic function", and that "the dispatch happens on the type of the first argument".[^4]

## In languages

| Language | Verdict | Mechanism |
|---|---|---|
| Python | native | lookup by name along `__mro__`; `functools.singledispatch` |
| JavaScript | native | lookup along the prototype chain |
| TypeScript | native | as JavaScript; `override` keyword |
| Java | native | instance methods are virtual unless `static`, `private` or `final` |
| Kotlin | native | `open` and `override`; interfaces |
| C# | native | `virtual`, `abstract` and `override`; interfaces; `dynamic` |
| Go | native | calls through interface values |
| Rust | native | trait objects with `dyn`; generics dispatch statically |
| C | idiom | structs of function pointers |
| C++ | native | `virtual` functions and vtables |
| Bash | idiom | a function name built from a string |
| PowerShell | native | class methods on .NET |
| SQL | n/a | no objects with methods |
| Regex | n/a | no methods |
| HTML | n/a | no methods |
| CSS | n/a | no methods |

Every core language with objects or interfaces has dynamic dispatch as a native feature, and they differ in what dispatches by default. In Python, JavaScript and Java nearly every method call dispatches. In C++, C# and Kotlin a method dispatches only when its declaration says so. In Go and Rust only calls through an interface value or a trait object dispatch, and a call on a concrete type, or through a generic parameter, is static.

Each example file builds the same shapes, with a `describe` method that calls `area`, and shows a pitfall of dispatch in that language: a misspelled override in Python, a call from a constructor in JavaScript, an overload that looks like an override in Java, a hidden method in C#, embedding that does not dispatch in Go, a trait that cannot be a trait object in Rust, and object slicing in C++.

## Variations and related ideas

- **Interfaces and protocols.** An interface declares the methods that a call through it can dispatch to. See [interfaces](/interfaces/) and [subtyping](/subtyping/).
- **Multiple dispatch.** Common Lisp's CLOS, Julia and Dylan choose a method from the types of all arguments. C# can do this at run time through `dynamic`.
- **The visitor pattern.** Two single dispatches in a row choose code by the types of two objects, in languages that dispatch only on the receiver.
- **Overloading.** Overloaded functions share a name and the compiler chooses one from the static types of the arguments. This is static dispatch, often called ad hoc polymorphism. See [parametric polymorphism](/parametric-polymorphism/).
- **Pattern matching.** A `match` over the cases of a sum type is the other way to choose code by the kind of a value: it keeps all the cases of one operation in one place, while dispatch keeps all the operations of one case in one class. See [sum types](/sum-types/) and [pattern matching](/pattern-matching/).
- **Type classes and traits.** Haskell type classes and Rust traits choose an implementation from types, mostly at compile time, and use dictionaries or vtables when the type is known only at run time.
- **Inline caching and devirtualization.** JIT compilers cache the target of each call site and inline it, and compilers turn virtual calls into direct calls when they can prove the type. See [just-in-time compilation](/just-in-time-compilation/).
- **Object-oriented programming.** Dispatch on the receiver is one of the central mechanisms of the paradigm. See [object-oriented programming](/object-oriented-programming/).

## Pitfalls

- **An override that overrides nothing.** A misspelled name or a different parameter type creates a new method, and the base method still runs. Use `@Override` in Java, `override` in C++, C#, Kotlin and TypeScript, and `typing.override` in Python. The wrong blocks of the Python and Java files show this.
- **Hiding instead of overriding.** A method with the same name in a subclass, when the base method is not virtual, is chosen by the static type of the reference. The wrong block of the C# file shows this.
- **Calls from constructors.** A constructor that calls an overridable method runs the override on an object whose fields are not set yet, in Java, C# and JavaScript, and runs the base version in C++. Do not call overridable methods from constructors.
- **Object slicing.** In C++, copying a derived object into a base object keeps only the base part and its vtable. Pass polymorphic objects by reference or pointer.
- **Fragile base classes.** A subclass that overrides one method can depend on how the base class calls its own methods, so a safe change to the base breaks it. Prefer composition, or document which methods call which.
- **Embedding is not inheritance.** In Go, a method of an embedded struct calls the methods of the embedded type, not of the outer type. The wrong block of the Go file shows this.
- **Missing virtual destructor.** In C++, deleting a derived object through a base pointer without a virtual destructor is undefined behavior, and in practice skips the destructor of the derived class.

## Cost

A virtual call costs two dependent loads, the table pointer and the slot, and an indirect branch. When the branch predictor guesses the target, the call costs a few cycles more than a direct call; when the target changes often at one site, a misprediction costs tens of cycles. The larger cost is lost optimization: the Rust Book notes that "Dynamic dispatch also prevents the compiler from choosing to inline a method's code, which in turn prevents some optimizations".[^1] Each object pays one pointer of memory for its vtable pointer, or each reference pays it in Rust and Go. Lookup by name costs a hash table search on a cache miss and a comparison of the class on a hit. JIT compilers and devirtualization in ahead-of-time compilers remove most of the cost at call sites where one or two types appear.

## History

Simula 67, by Ole-Johan Dahl and Kristen Nygaard, introduced classes, subclasses and virtual procedures, which a subclass could redefine. Smalltalk, developed by Alan Kay, Dan Ingalls and others at Xerox PARC in the 1970s, made every operation a message that the receiving object looks up by name at run time. L. Peter Deutsch and Allan Schiffman described the inline cache in their 1984 paper on the Smalltalk-80 implementation, and Urs Hölzle, Craig Chambers and David Ungar added polymorphic inline caches for the Self language in 1991.

Bjarne Stroustrup added virtual functions in 1983, as C with Classes became C++, with the vtable layout that most compiled languages still use. The Common Lisp Object System, adopted into the ANSI standard of 1994, made multiple dispatch on generic functions part of Lisp. Java, in 1995, made instance methods virtual by default, while C#, in 2000, required `virtual` and `override`. Go, in 2009, and Rust, in 2015, separated the table from the object and dispatched only through interface values and trait objects. Julia, released in 2012, built its whole design on multiple dispatch.

## References
