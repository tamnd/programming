---
tag: 8WZW5P
kind: concept
slug: polymorphism
name: Polymorphism
aliases: [polymorphic, subtype polymorphism, inclusion polymorphism, ad hoc polymorphism, coercion polymorphism, static polymorphism, dynamic polymorphism, many forms]
area: types/generics
level: intro
summary: >-
  Polymorphism lets one name or one piece of code work with values of many types. The main kinds are subtype, parametric and ad hoc polymorphism, and some authors add coercion.
applies_to: core-languages
viz: optional
formal: none
see_also: [parametric-polymorphism, subtyping, dynamic-dispatch, overloading, operator-overloading, type-conversion, interfaces, row-polymorphism]
same_as:
  wikidata: Q3240252
  wikipedia: Polymorphism_(computer_science)
sources:
- url: https://en.wikipedia.org/wiki/Polymorphism_(computer_science)
  title: Polymorphism (computer science), Wikipedia
  locator: Introduction; History; Forms; Static and dynamic polymorphism
- url: https://docs.oracle.com/javase/tutorial/java/IandI/polymorphism.html
  title: Polymorphism, The Java Tutorials
  locator: Whole page
- url: https://learn.microsoft.com/en-us/dotnet/csharp/fundamentals/object-oriented/polymorphism
  title: Polymorphism, C# fundamentals
  locator: Introduction
- url: https://docs.python.org/3.14/library/functools.html
  title: functools, Python 3.14 documentation
  locator: functools.singledispatch
- url: https://en.cppreference.com/w/c/language/generic.html
  title: Generic selection, C reference, cppreference.com
  locator: Syntax; Notes
- url: https://developer.mozilla.org/en-US/docs/Glossary/Type_coercion
  title: Type coercion, MDN Web Docs Glossary
  locator: Whole entry
- url: https://docs.python.org/3.14/glossary.html
  title: Glossary, Python 3.14 documentation
  locator: duck-typing; generic function
- url: https://go.dev/ref/spec
  title: The Go Programming Language Specification
  locator: Interface types; Type parameter declarations
- url: https://doc.rust-lang.org/book/ch18-02-trait-objects.html
  title: Using Trait Objects to Abstract over Shared Behavior, The Rust Programming Language
  locator: Performing Dynamic Dispatch
---

## Idea

Code often does the same thing to values of different types: it computes the area of a square or a circle, takes the first item of a list of numbers or of strings, or prints an integer or a string. Polymorphism is the name for the ways a language lets one name or one piece of code serve all these types. Without it, a program needs one function for each type, with a different name for each. Python shows the main kinds in a few lines.

```python run id=idea
from functools import singledispatch


# Subtype polymorphism: one call, and the class of the object picks the method.
class Shape:
    def area(self) -> float:
        raise NotImplementedError


class Square(Shape):
    def __init__(self, side: float) -> None:
        self.side = side

    def area(self) -> float:
        return self.side * self.side


class Circle(Shape):
    def __init__(self, r: float) -> None:
        self.r = r

    def area(self) -> float:
        return round(3.14159 * self.r * self.r, 2)


print([s.area() for s in [Square(2), Circle(1)]])


# Parametric polymorphism: one body of code for every element type.
def first[T](items: list[T]) -> T:
    return items[0]


print(first([3, 4]), first(["a", "b"]))


# Ad hoc polymorphism: a separate body of code for each type.
@singledispatch
def show(x: object) -> str:
    return "?"


@show.register
def _(x: int) -> str:
    return f"int {x}"


@show.register
def _(x: str) -> str:
    return f"str {x!r}"


print(show(1), show("a"), show(2.5))

# Operators are ad hoc too, and coercion turns the int into a float.
print("a" * 3, 2 * 3, 1 + 2.5)
```

```output for=idea
[4, 3.14]
3 a
int 1 str 'a' ?
aaa 6 3.5
```

The call `s.area()` runs the method of `Square` or of `Circle`, chosen by the object. `first` has one body that works for a list of any element type, because it does nothing with the items except return one. `show` has a separate body for each type, and the type of the argument picks one. The operator `*` repeats a string and multiplies numbers, and `1 + 2.5` converts the integer to a float before it adds.

## How it works

**One interface, many types.** "In programming language theory and type theory, polymorphism allows a value or variable to have more than one type and allows a given operation to be performed on values of more than one type"[^1]. "In object-oriented programming, polymorphism is the provision of one interface to entities of different data types"[^1]. The Java tutorial takes the word from biology, where "an organism or species can have many different forms or stages"[^2].

**Subtype polymorphism.** A function written for a type `T` also accepts a value of a subtype `S` of `T`. "Subtyping (also called subtype polymorphism or inclusion polymorphism): when a name denotes instances of many different classes related by a common superclass"[^1]. When the code calls a method, the class of the object picks the code: "At run-time, when client code calls the method, the CLR looks up the run-time type of the object, and invokes that override of the virtual method"[^3]. In Java, the JVM "does not call the method that is defined by the variable's type"[^2]. See [subtyping](/subtyping/) and [dynamic dispatch](/dynamic-dispatch/).

**Parametric polymorphism.** "Parametric polymorphism allows a function or a data type to be written generically, so that it can handle values uniformly without depending on their type"[^1]. The code takes a type parameter, such as `T`, and works the same way for every type argument. "Any parametrically polymorphic function is necessarily restricted in what it can do, working on the shape of the data instead of its value"[^1]. In C# and Java this is called generics, and in C++ it is done with templates[^1]. See [parametric polymorphism](/parametric-polymorphism/).

**Ad hoc polymorphism.** "Ad hoc polymorphism: defines a common interface for an arbitrary set of individually specified types"[^1]. Overloaded functions and operators are the usual form: they "behave differently depending on the type of the argument to which they are applied"[^1], and each version is a separate function to the compiler[^1]. In Python, `functools.singledispatch` makes "a single-dispatch generic function"[^4], and "the dispatch happens on the type of the first argument"[^4]. In C, `_Generic` provides "a way to choose one of several expressions at compile time, based on a type of a controlling expression"[^5]. See [overloading](/overloading/).

**Coercion.** Implicit type conversion "has also been defined as a form of polymorphism"[^1], called coercion polymorphism. An operator that is defined for floats also accepts an integer, because the language converts the integer first. In JavaScript, "Type coercion is the automatic or implicit conversion of values from one data type to another (such as strings to numbers)"[^6]. See [type conversion](/type-conversion/).

**Static and dynamic polymorphism.** "Polymorphism can be distinguished by when the implementation is selected: statically (at compile time) or dynamically (at run time, typically via a virtual function)"[^1]. "Static polymorphism typically occurs in ad hoc polymorphism and parametric polymorphism, whereas dynamic polymorphism is usual for subtype polymorphism"[^1].

## In languages

| Language | Verdict | Mechanism |
|---|---|---|
| Python | native | methods chosen by class; duck typing; `functools.singledispatch`; type parameters for static checkers |
| JavaScript | native | methods found through the prototype chain; duck typing; implicit coercion |
| TypeScript | native | structural subtyping; generics; overload signatures |
| Java | native | virtual methods on classes and interfaces; generics; overloaded methods |
| Kotlin | native | open classes and interfaces; generics; overloaded functions and operators |
| C# | native | virtual methods and interfaces; generics; overloaded methods and operators |
| Go | native | interfaces that a type meets by its methods; type parameters; no overloading |
| Rust | native | traits, with static dispatch for generics and dynamic dispatch for `dyn Trait`; no overloading |
| C | idiom | function pointers in a struct; `_Generic` for overloads; arithmetic conversions |
| C++ | native | virtual functions; templates; overloaded functions and operators |
| Bash | n/a | every value is a string, so there are no types to choose between |
| PowerShell | native | .NET classes and interfaces; overloaded .NET methods; conversion of arguments |
| SQL | n/a | built-in functions take several types, but a query defines no types or subtypes |
| Regex | n/a | a pattern has no types |
| HTML | n/a | HTML has no types |
| CSS | n/a | CSS has no types in this sense |

Java, C#, Kotlin and C++ have all three main kinds in the language. Go and Rust have subtype-like polymorphism through interfaces and traits, and parametric polymorphism through type parameters, but no overloading by type. Python and JavaScript choose a method by the object when the program runs, so any object with the right method works. C has only `_Generic` and function pointers, which the programmer must build into each design.

The example files show these differences. Python shows duck typing and a `Protocol`. JavaScript shows a call on objects that share no class, and the coercion of `'5' + 9`. TypeScript shows structural subtyping, a generic function and overload signatures. Java shows that an overload is chosen by the declared type, while an override is chosen by the object. C# shows a virtual method and an overloaded method. Go shows an interface and a function with a type parameter. Rust shows the same trait with static and with dynamic dispatch. C shows `_Generic` and a table of function pointers. C++ shows a virtual function, a template and an overload.

## Variations and related ideas

- **Duck typing.** A dynamic language can skip the types and call the method: with duck typing, "the method or attribute is simply called or used"[^7], and the code does not test the type first. Go makes a similar check when the program compiles: "A variable of interface type can store a value of any type that is in the type set of the interface"[^8], and a type is in that set when it has the methods.
- **Bounded polymorphism.** "The interaction between parametric polymorphism and subtyping leads to the concepts of type variance and bounded quantification"[^1]. A type parameter can require a subtype of an interface, such as `T extends Comparable<T>`.
- **Multiple dispatch.** Most object systems choose a method by one object only, but some, "such as Common Lisp Object System, provide multiple dispatch, under which method calls are polymorphic in all arguments"[^1].
- **Row polymorphism.** Row polymorphism "is a similar, but distinct concept from subtyping"[^1]: a function accepts any record that has certain fields and keeps the types of the other fields. See [row polymorphism](/row-polymorphism/).

## Pitfalls

- **An overload is chosen by the declared type.** With overloading, "one of several functions is chosen at compile time based on the types of the arguments"[^5], but an override is chosen by the object when the program runs[^2]. In Java, `describe(Object o)` and `describe(String s)` with an argument declared as `Object` call the `Object` version even when the object is a string. The Java example shows this.
- **Unwanted coercion.** In JavaScript, the string `'5'` plus the number `9` gives the string `'59'`, because "JavaScript has coerced the 9 from a number into a string"[^6]. Convert explicitly, for example with `Number()`[^6].
- **A method on a value that lacks it.** With duck typing, a call on an object without the method fails only when the program runs. A static checker with `Protocol` in Python, or an interface in Go and TypeScript, finds the error before the run.
- **Changing the meaning of an operator.** An overloaded operator can do work that the reader does not expect, such as `*` on a string. Overload an operator only when its meaning for the type is clear. See [operator overloading](/operator-overloading/).

## Cost

Static polymorphism costs nothing when the program runs: the compiler picks the code for each call. For generics, "The compiler generates nongeneric implementations of functions and methods for each concrete type"[^9], as C++ does for templates[^1], so the program grows with each type it uses. Dynamic polymorphism adds a lookup to each call. In Rust, "This lookup incurs a runtime cost"[^9] that static dispatch does not have, and dynamic dispatch "prevents the compiler from choosing to inline a method"[^9]. "Static polymorphism executes faster, because there is no dynamic dispatch overhead, but requires additional compiler support"[^1].

## History

Christopher Strachey described ad hoc polymorphism and parametric polymorphism in *Fundamental Concepts in Programming Languages*, where they are "the two main classes" of polymorphism[^1]. Ad hoc polymorphism was a feature of ALGOL 68, and parametric polymorphism was the core of the type system of ML[^1]. In 1985, Peter Wegner and Luca Cardelli introduced the term inclusion polymorphism for subtypes and inheritance, and named Simula as the first language with it[^1].

## References
