---
tag: NG4EV8
kind: concept
slug: templates-and-generics
name: Templates and generics
aliases: [C++ templates, Java generics, C# generics, template instantiation, monomorphization, reified generics, erased generics, GC shape stenciling, how generics are compiled]
area: types/generics
level: core
summary: >-
  C++ templates make a copy of the code for each type, Java generics are checked and then erased, and C#, Go and Rust each keep a different part of the type at run time.
applies_to: core-languages
viz: optional
formal: none
see_also: [parametric-polymorphism, bounded-quantification, type-erasure, dynamic-dispatch, polymorphism, generic-containers, write-a-generic-function, type-constructor]
same_as:
  wikidata: Q1411845
  wikipedia: Template_(C++)
sources:
- url: https://en.cppreference.com/w/cpp/language/templates.html
  title: Templates, C++ reference, cppreference.com
  locator: Introduction
- url: https://en.cppreference.com/w/cpp/language/constraints.html
  title: Constraints and concepts, C++ reference, cppreference.com
  locator: Introduction
- url: https://docs.oracle.com/javase/tutorial/java/generics/why.html
  title: Why Use Generics?, The Java Tutorials
  locator: Whole page
- url: https://docs.oracle.com/javase/tutorial/java/generics/erasure.html
  title: Type Erasure, The Java Tutorials
  locator: Whole page
- url: https://learn.microsoft.com/en-us/dotnet/csharp/programming-guide/generics/generics-in-the-run-time
  title: Generics in the runtime, C# programming guide
  locator: Whole page
- url: https://doc.rust-lang.org/book/ch10-01-syntax.html
  title: Generic Data Types, The Rust Programming Language
  locator: Performance of Code Using Generics
- url: https://doc.rust-lang.org/reference/types/trait-object.html
  title: Trait objects, The Rust Reference
  locator: Introduction
- url: https://github.com/golang/proposal/blob/master/design/generics-implementation-gcshape.md
  title: Generics implementation, GC Shape Stenciling, Go design document
  locator: Introduction
- url: https://en.wikipedia.org/wiki/Template_(C%2B%2B)
  title: Template (C++), Wikipedia
  locator: Introduction; Technical overview
- url: https://en.wikipedia.org/wiki/Monomorphization
  title: Monomorphization, Wikipedia
  locator: Introduction
- url: https://docs.oracle.com/javase/tutorial/java/generics/restrictions.html
  title: Restrictions on Generics, The Java Tutorials
  locator: Cannot Instantiate Generic Types with Primitive Types; Cannot Create Instances of Type Parameters
- url: https://en.wikipedia.org/wiki/Generic_programming
  title: Generic programming, Wikipedia
  locator: Introduction
---

## Idea

Many languages let one function or class work for many types. They all check the generic code, but they differ in what they make from it. C++ makes a separate copy of a template for each type that uses it. Java checks the types and then erases them, so one copy of the code works for every type. C#, Go and Rust each take a way between these two. The choice decides what the program can ask about a type when it runs, how fast the code is, and how large the program becomes.

```java run id=idea
import java.util.ArrayList;
import java.util.List;

public class Main {
    public static void main(String[] args) {
        List<String> names = new ArrayList<>();
        List<Integer> nums = new ArrayList<>();
        // One class for both lists: the type argument is erased.
        System.out.println(names.getClass() == nums.getClass());
        System.out.println(names.getClass().getName());
    }
}
```

```output for=idea
true
java.util.ArrayList
```

In Java, `List<String>` and `List<Integer>` are different types to the compiler, but the program has one class for both. In C++, `std::vector<std::string>` and `std::vector<int>` are two classes with two copies of the code.

## How it works

**C++ templates make code for each type.** "A template is a C++ entity that defines" a family of classes, functions or variables[^1]. The template arguments "are substituted for the template parameters to obtain a specialization of the template, that is, a specific type or a specific function lvalue"[^1]. When the program uses a specialization, "the template is instantiated (the code for it is actually compiled)"[^1]. The compiler checks the body of a template for each type that it makes. A template that calls `x.size()` compiles for any type that has `size`, and the error for a type without it appears when the template is instantiated. Concepts let the template state its needs, so "Violations of constraints are detected at compile time, early in the template instantiation process"[^2].

**Java generics are checked and then erased.** Java uses the type arguments to check the program: generics give "Stronger type checks at compile time"[^3]. Then the compiler removes them. It applies type erasure to "Replace all type parameters in generic types with their bounds or Object if the type parameters are unbounded"[^4] and to "Insert type casts if necessary to preserve type safety"[^4]. The compiled code "contains only ordinary classes, interfaces, and methods"[^4], so all the parameterized types of one generic class share one class. See [type erasure](/type-erasure/).

**C# generics are kept in the runtime.** In .NET, a compiled generic type "contains metadata that identifies it as having type parameters"[^5]. The runtime makes code for each value type: "Specialized generic types are created one time for each unique value type that is used as a parameter."[^5] For reference types, it shares one copy: "the runtime reuses the previously created specialized version of the generic type. This is possible because all references are the same size."[^5] The program can ask for the type argument when it runs.

**Rust makes a copy for each type.** "Monomorphization is the process of turning generic code into specific code by filling in the concrete types that are used when compiled."[^6] "Because Rust compiles generic code into code that specifies the type in each instance, we pay no runtime cost for using generics."[^6] For one shared copy of the code, Rust has trait objects. "A trait object is an opaque value of another type that implements a set of traits."[^7] A pointer to it includes "a virtual method table, often just called a vtable"[^7].

**Go shares code by shape.** The Go design generates "a single chunk of assembly for each unique GC shape used by any instantiation"[^8]. The GC shape of a type "means how that type appears to the allocator / garbage collector. It is determined by its size, its required alignment, and which parts of the type contain a pointer."[^8] Each chunk takes a dictionary that describes the real type arguments. Types with the same shape, such as all pointer types, share one copy of the code.

## In languages

| Language | Verdict | Mechanism |
|---|---|---|
| Python | native | type parameters for static checkers only; the program runs one copy of the code |
| JavaScript | n/a | the language has no type parameters |
| TypeScript | native | generics checked by the compiler and then removed; the JavaScript output has no types |
| Java | native | generics checked by the compiler and then erased to the bound or `Object` |
| Kotlin | native | erased on the JVM, as in Java; `reified` type parameters in `inline` functions |
| C# | native | generics kept in the runtime; code for each value type, and one shared copy for reference types |
| Go | native | one copy of the code for each GC shape, with a dictionary of the type arguments |
| Rust | native | monomorphization: a copy for each type; `dyn Trait` for one shared copy |
| C | idiom | macros that make code for each type, or `void *` with one copy |
| C++ | native | templates instantiated for each set of template arguments |
| Bash | n/a | the language has no types |
| PowerShell | n/a | a script cannot declare its own generic functions or types |
| SQL | n/a | a query has no type parameters |
| Regex | n/a | a pattern has no types |
| HTML | n/a | HTML has no types |
| CSS | n/a | CSS has no types of this kind |

C++ and Rust make code for each type, so the code is fast and the program is larger. Java, Kotlin, TypeScript and Python keep one copy, so the program cannot ask for a type argument when it runs. C# and Go share some code and copy some.

The example files show what each way keeps. Python and TypeScript show that the type arguments are not checked when the program runs. Java shows the casts that erasure adds, and that `new T()` is not allowed. C# shows that `typeof(T)` gives the real type argument. Go shows a generic function that works for `int` and `string`. Rust shows a generic function and a `dyn Trait` version. C shows a macro that makes a function for each type. C++ shows a template that compiles only for the types that it is used with.

## Variations and related ideas

- **Non-type parameters.** C++ templates "are parameterized by one or more template parameters, of three kinds: type template parameters, non-type template parameters, and template template parameters"[^1]. `std::array<int, 4>` has the number 4 as a template argument. Rust has const generics for the same use.
- **Templates as programs.** Because the compiler runs a template for each argument, C++ code can compute at compile time. "C++ Templates are Turing complete."[^9]
- **Specialization.** "Specializations may also be provided explicitly"[^1]: the program can give a special version of a template for one type.

## Pitfalls

- **Code size.** A copy for each type makes the program larger. Monomorphization "may require more compilation time and storage space due to duplicating the function body"[^10].
- **No primitive types in Java.** Because erasure replaces a type parameter with `Object`, a Java type argument must be a class: "Cannot Instantiate Generic Types with Primitive Types"[^11]. A `List<Integer>` holds boxed numbers.
- **No type at run time in Java.** Java cannot write `new T()`: "Cannot Create Instances of Type Parameters"[^11]. Pass a `Class<T>` or a factory function instead.
- **Templates in headers.** "The definition of a class template must be visible at the point of implicit instantiation, which is why template libraries typically provide all template definitions in the headers"[^1]. A change to a template recompiles every file that includes it.
- **Error messages.** A C++ template without a concept reports an error inside its body, far from the call. Write a concept or a `requires` clause so that the error points to the call.

## History

"Major inspirations for C++ templates were the parameterized modules provided by the language CLU and the generics provided by Ada."[^9] "Generic programming was introduced to the mainstream with Ada in 1977. With templates in C++, generic programming became part of the repertoire of professional library design."[^12] Java and C# took different ways: Java erases the type arguments[^4], and .NET keeps them in the runtime[^5].
