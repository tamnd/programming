---
tag: K0A8GB
kind: concept
slug: bounded-quantification
name: Bounded generics
aliases: [bounded quantification, bounded polymorphism, constrained genericity, bounded type parameter, type constraint, trait bound, upper bound, where clause, F-bounded polymorphism]
area: types/generics
level: advanced
summary: >-
  A bound on a type parameter limits the type arguments to types with some operations, such as a compare method, so the generic code can use those operations and the compiler can check each call.
applies_to: core-languages
viz: optional
formal: none
see_also: [parametric-polymorphism, polymorphism, subtyping, interfaces, write-a-generic-function, recursive-type, type-constructor, generic-containers]
same_as:
  wikidata: Q4949985
  wikipedia: Bounded_quantification
sources:
- url: https://docs.oracle.com/javase/tutorial/java/generics/bounded.html
  title: Bounded Type Parameters, The Java Tutorials
  locator: Whole page
- url: https://en.wikipedia.org/wiki/Bounded_quantification
  title: Bounded quantification, Wikipedia
  locator: Introduction; F-bounded quantification
- url: https://learn.microsoft.com/en-us/dotnet/csharp/programming-guide/generics/constraints-on-type-parameters
  title: Constraints on type parameters, C# programming guide
  locator: Introduction
- url: https://www.typescriptlang.org/docs/handbook/2/generics.html
  title: Generics, The TypeScript Handbook
  locator: Generic Constraints
- url: https://kotlinlang.org/docs/generics.html
  title: Generics, Kotlin documentation
  locator: Upper bounds
- url: https://doc.rust-lang.org/book/ch10-02-traits.html
  title: Traits, The Rust Programming Language
  locator: Trait Bound Syntax; Clearer Trait Bounds with where Clauses; Using Trait Bounds to Conditionally Implement Methods
- url: https://go.dev/ref/spec
  title: The Go Programming Language Specification
  locator: Type constraints
- url: https://go.dev/doc/tutorial/generics
  title: "Tutorial: Getting started with generics, Go documentation"
  locator: Add a generic function to handle multiple types
- url: https://en.cppreference.com/w/cpp/language/constraints.html
  title: Constraints and concepts, C++ reference, cppreference.com
  locator: Introduction; Concepts
- url: https://docs.python.org/3.14/library/typing.html
  title: typing, Python 3.14 documentation
  locator: TypeVar
- url: https://docs.oracle.com/javase/tutorial/java/generics/boundedTypeParams.html
  title: Generic Methods and Bounded Type Parameters, The Java Tutorials
  locator: Whole page
- url: https://docs.oracle.com/en/java/javase/21/docs/api/java.base/java/lang/Enum.html
  title: Class Enum, Java SE 21 API
  locator: Class declaration
- url: https://docs.oracle.com/javase/tutorial/java/generics/upperBounded.html
  title: Upper Bounded Wildcards, The Java Tutorials
  locator: Whole page
---

## Idea

A generic function with a plain type parameter `T` knows nothing about its arguments, so it can only store them, pass them on and return them. To compare two items, add them or hash them, the function needs to know that `T` has that operation. A bound on the type parameter says so: `T` can be any type, but only a type that has the operation.

```python run id=idea
from typing import SupportsAbs


def total_abs[T: SupportsAbs[float]](items: list[T]) -> float:
    return sum(abs(x) for x in items)


print(total_abs([-2, 3.5]))
print(total_abs.__type_params__[0].__bound__)

try:
    total_abs(["a"])  # a type checker reports this call
except TypeError as e:
    print(type(e).__name__)
```

```output for=idea
5.5
typing.SupportsAbs[float]
TypeError
```

The bound `SupportsAbs[float]` says that each item must have an `__abs__` method, so the body can call `abs(x)`. A type checker rejects the call with a list of strings. Python itself does not check the bound, so the error comes from `abs` when the program runs.

## How it works

**A bound limits the type arguments.** In Java, "There may be times when you want to restrict the types that can be used as type arguments in a parameterized type"[^1]. The type parameter gets a bound, such as `<T extends Number>`, and the compiler rejects a call whose type argument is not a subtype of the bound. "Bounded quantification is an interaction of parametric polymorphism with subtyping."[^2]

**The body can use the operations of the bound.** "In addition to limiting the types you can use to instantiate a generic type, bounded type parameters allow you to invoke methods defined in the bounds"[^1]. Without a bound, the body can use only the operations of every type. In C#, "Without any constraints, the type argument could be any type. The compiler can only assume the members of System.Object"[^3].

**The forms of bounds.** Each language writes a bound in its own way:

- Java and TypeScript use `extends`: `<T extends Comparable<T>>` in Java and `<Type extends Lengthwise>` in TypeScript[^4]. Kotlin uses a colon, and "The most common type of constraint is an upper bound, which corresponds to Java's extends keyword"[^5].
- C# uses a `where` clause: `where T : IComparable<T>`. "If client code uses a type that doesn't satisfy a constraint, the compiler issues an error."[^3]
- Rust uses trait bounds, `T: PartialOrd`, or a `where` clause after the signature. "We can use trait bounds to specify that a generic type can be any type that has certain behavior."[^6]
- Go uses an interface as the constraint: "A type constraint is an interface that defines the set of permissible type arguments for the respective type parameter and controls the operations supported by values of that type parameter"[^7]. "Each type constraint specifies the permissible type arguments that calling code can use for the respective type parameter."[^8] A Go constraint can also be a union of types, such as `int64 | float64`[^8].
- C++ uses concepts. "Each concept is a predicate, evaluated at compile time, and becomes a part of the interface of a template where it is used as a constraint"[^9].
- Python uses an upper bound on a type variable: "The upper bound of a type variable can be a concrete type, abstract type (ABC or Protocol), or even a union of types"[^10].

**More than one bound.** A type parameter can need two operations. In Java, "A type variable with multiple bounds is a subtype of all the types listed in the bound"[^1], written `<T extends A & B>`. Rust writes `T: Display + Clone`. In Kotlin, "Only one upper bound can be specified inside the angle brackets"[^5], so two bounds go in a `where` clause.

**F-bounded polymorphism.** A bound can mention the type parameter itself, as in `<T extends Comparable<T>>`. Then `T` must be comparable to `T`, and not only to some other type. This is "F-bounded quantification or recursively bounded quantification, introduced in 1989"[^2]. The Java tutorial uses this bound for a method that counts the items greater than a given item[^11]. The class `Enum` in Java has the declaration `class Enum<E extends Enum<E>>` and implements `Comparable<E>`, so "Enum constants are only comparable to other enum constants of the same enum type"[^12].

## In languages

| Language | Verdict | Mechanism |
|---|---|---|
| Python | native | an upper bound in `[T: Bound]` or `TypeVar("T", bound=...)`, for static checkers |
| JavaScript | n/a | the language has no type parameters |
| TypeScript | native | `<T extends Bound>` |
| Java | native | `<T extends Bound>` and `<T extends A & B>`; wildcards `? extends Bound` |
| Kotlin | native | `<T : Bound>` and a `where` clause |
| C# | native | `where T : Bound`, with special constraints such as `struct`, `class` and `new()` |
| Go | native | an interface as the constraint, such as `cmp.Ordered`, with unions of types |
| Rust | native | trait bounds `T: Trait` and `where` clauses |
| C | absent | the language has no type parameters |
| C++ | native | concepts and `requires` clauses on templates |
| Bash | n/a | the language has no types |
| PowerShell | absent | a script cannot declare its own type parameters |
| SQL | n/a | a query has no type parameters |
| Regex | n/a | a pattern has no types |
| HTML | n/a | HTML has no types |
| CSS | n/a | CSS has no types of this kind |

In Java, C#, Kotlin and TypeScript, a bound is a supertype: the type argument must be a subtype of it. In Rust, a bound is a trait, and a type satisfies it when an `impl` of the trait exists. In Go, a bound is an interface that can also list types. In C++, a concept is a test on the type that the compiler evaluates.

The example files show these forms. Python shows a `Protocol` as the bound. TypeScript shows a bound with a `length` property. Java shows `Comparable<T>` and two bounds. C# shows a `where` clause with `IComparable<T>` and `new()`. Go shows `cmp.Ordered` and a constraint with a union. Rust shows trait bounds, a `where` clause and the error for a type without the trait. C++ shows a concept and a `requires` clause.

## Variations and related ideas

- **Bounded wildcards.** Java also puts bounds on the type argument of a variable: "You can use an upper bounded wildcard to relax the restrictions on a variable"[^13]. A `List<? extends Number>` can be a `List<Integer>` or a `List<Double>`. See [subtyping](/subtyping/).
- **Bounds that select the code.** In C++, a constraint "can be used to select the most appropriate function overloads and template specializations"[^9]. In Rust, an `impl` with a bound applies to every type that meets it: "Implementations of a trait on any type that satisfies the trait bounds are called blanket implementations"[^6].
- **Constraints that are not subtypes.** C# has constraints such as `struct`, `class`, `notnull` and `new()`, which ask for a kind of type or a constructor[^3]. Python can constrain a type variable to a fixed list: `TypeVar('A', str, bytes)` "Must be exactly str or bytes"[^10].

## Pitfalls

- **A bound that is too weak.** A bound that does not mention `T` lets the function take two items of different types. With `<S extends Comparable>`, Java accepts a string and an integer in one call, and the program "Throws java.lang.ClassCastException at runtime"[^2]. Use the F-bound `<T extends Comparable<T>>`.
- **Long signatures.** "Using too many trait bounds has its downsides."[^6] With many type parameters, the bounds fill the space between the name of the function and its parameters, and the signature is hard to read. Rust, Kotlin and C# let you move the bounds to a `where` clause after the signature.
- **The order of bounds in Java.** "If one of the bounds is a class, it must be specified first."[^1] If `A` is a class and `B` and `C` are interfaces, `<T extends B & A & C>` is a compile error[^1].
- **Errors inside templates.** Without a concept, a C++ template fails only when the compiler makes its body for one type, and the message can point deep inside the template. With concepts, "Violations of constraints are detected at compile time, early in the template instantiation process, which leads to easy to follow error messages"[^9].
- **Bounds that are not checked.** In Python and TypeScript, only the type checker checks the bounds. A Python program runs a call that breaks a bound, and the error, if any, comes later.

## History

Bounded quantification "has traditionally been studied in the functional setting of System F<:"[^2]. F-bounded quantification was introduced in 1989, in a paper by Canning, Cook, Hill, Mitchell and others[^2]. The Java standard library uses an F-bound in the declaration of `Enum`[^12], and Java, C#, Kotlin, Rust, Go and C++ all have a way to write a bound on a type parameter.
