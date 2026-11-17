---
tag: EG09QP
kind: concept
slug: variadic-generics
name: Variadic generics
aliases: [variadic templates, parameter packs, template parameter pack, type parameter pack, TypeVarTuple, variadic tuple types, fold expression, sizeof...]
area: types/generics
level: research
summary: >-
  Variadic generics let a generic function or type take any number of type parameters, as C++ parameter packs do, instead of one overload for each count of types.
applies_to: core-languages
viz: optional
formal: none
see_also: [templates-and-generics, parametric-polymorphism, tuple, function-type, template-specialization, bounded-quantification, type-constructor, syntactic-macros]
same_as:
  wikidata: Q17152756
  wikipedia: Variadic_template
sources:
- url: https://en.cppreference.com/w/cpp/language/pack.html
  title: Pack, C++ reference, cppreference.com
  locator: Introduction; Pack expansion; The sizeof... operator
- url: https://en.wikipedia.org/wiki/Variadic_template
  title: Variadic template, Wikipedia
  locator: Introduction; C++
- url: https://en.cppreference.com/w/cpp/language/fold.html
  title: Fold expressions, C++ reference, cppreference.com
  locator: Introduction
- url: https://peps.python.org/pep-0646/
  title: PEP 646, Variadic Generics
  locator: Abstract
- url: https://docs.python.org/3.14/library/typing.html
  title: typing, Python 3.14 documentation
  locator: TypeVarTuple
- url: https://www.typescriptlang.org/docs/handbook/release-notes/typescript-4-0.html
  title: TypeScript 4.0, release notes
  locator: Variadic Tuple Types
- url: https://github.com/swiftlang/swift-evolution/blob/main/proposals/0393-parameter-packs.md
  title: SE-0393, Value and Type Parameter Packs, Swift Evolution
  locator: Introduction
- url: https://learn.microsoft.com/en-us/dotnet/api/system.func-17
  title: Func<T1,...,T16,TResult> Delegate, .NET API
  locator: Definition
- url: https://doc.rust-lang.org/std/primitive.tuple.html
  title: Primitive Type tuple, Rust standard library
  locator: Trait implementations
---

## Idea

Some generic code takes a list of types whose length is not fixed: a tuple of any size, a function with any number of parameters, a `zip` of any number of lists. With one type parameter for each type, a library needs one declaration for each count. Variadic generics let one declaration take a list of type parameters of any length.

```cpp run id=idea
#include <iostream>
#include <string>
#include <tuple>

// Ts is a pack: zero or more types.
template <typename... Ts>
std::size_t count_types(const Ts&...) {
    return sizeof...(Ts);
}

int main() {
    std::cout << count_types() << " " << count_types(1, "two", 3.0) << "\n";
    std::tuple<int, std::string, double> t{1, "two", 3.0};
    std::cout << std::tuple_size_v<decltype(t)> << "\n";
}
```

```output for=idea
0 3
3
```

`typename... Ts` declares a template parameter pack. One template covers calls with zero, one or many arguments, each of its own type. `std::tuple` is a variadic class template, so `std::tuple<int, std::string, double>` has three type arguments.

## How it works

**Parameter packs.** In C++, "A template parameter pack is a template parameter that accepts zero or more template arguments (non-types, types, or templates)."[^1] "A function parameter pack is a function parameter that accepts zero or more function arguments."[^1] "A template with at least one parameter pack is called a variadic template."[^1] Variadic templates "take a variable number of type parameters"[^2].

**Using a pack.** The program expands a pack: `f(xs...)` passes every element, and `sizeof...(Ts)` gives the number of elements[^1]. A fold expression "Reduces (folds) a pack over a binary operator."[^3] `(xs + ...)` adds all the elements. A template can also handle the first element and call itself with the rest.

**Other languages.** Python added type variable tuples for static checkers. PEP 646 introduces `TypeVarTuple`, "enabling parameterisation with an arbitrary number of types"[^4], and the typing documentation calls it "A specialized form of type variable that enables variadic generics"[^5]. TypeScript has variadic tuple types: "spreads in tuple type syntax can now be generic"[^6], so a type such as `[...T, ...U]` joins two tuple types. Swift added type parameter packs and value parameter packs "to enable abstracting over the number of types and values with distinct type"[^7].

**The workaround: one declaration for each count.** A language without variadic generics writes the same declaration many times. .NET has a `Func` delegate for each number of parameters up to 16; the largest one "Encapsulates a method that has 16 parameters"[^8]. In Rust, some traits "are only implemented on tuples of arity 12 or less"[^9], and the code for each size is written by a macro. The Swift proposal describes these "ad-hoc variadic APIs with an arbitrary upper bound, typically achieved with overloads that each have a different fixed number of type parameters and corresponding arguments"[^7].

## In languages

| Language | Verdict | Mechanism |
|---|---|---|
| Python | native | `TypeVarTuple` and `[*Ts]`, for static checkers |
| JavaScript | n/a | the language has no type parameters |
| TypeScript | native | variadic tuple types, such as `[...T, ...U]` |
| Java | absent | varargs `T...` take many values of one type; no list of type parameters |
| Kotlin | absent | `vararg` takes many values of one type; no list of type parameters |
| C# | absent | `params` takes many values of one type; `Func` and `Action` have one overload for each count |
| Go | absent | variadic parameters `...T` take many values of one type |
| Rust | absent | tuples and macros that write an `impl` for each tuple size |
| C | n/a | the language has no generics; `va_list` functions have no types for their arguments |
| C++ | native | template parameter packs, pack expansion, `sizeof...` and fold expressions |
| Bash | n/a | the language has no types |
| PowerShell | n/a | a script cannot declare its own generic code |
| SQL | n/a | a query has no type parameters |
| Regex | n/a | a pattern has no types |
| HTML | n/a | HTML has no types |
| CSS | n/a | CSS has no types of this kind |

"While other languages, such as Java or C# offer support for type-safe variadic functions, they do not offer the variadic generics support like that of C++ and D."[^2] A varargs parameter takes any number of values of one type. Variadic generics take any number of types.

The example files show C++ packs and fold expressions, Python `TypeVarTuple`, TypeScript variadic tuple types, and the Rust macro that writes one `impl` for each tuple size.

## Variations and related ideas

- **Packs of values.** C++ packs can hold values, such as `template <int... Ns>`, and template templates as well as types[^1].
- **Tuples as the workaround.** A function that takes one tuple of any type can stand for a function with many parameters. The code still needs one declaration for each tuple size, so libraries stop at a fixed size, such as 12 in Rust[^9] or 16 in .NET[^8].
- **Variadic functions.** A variadic function, such as `printf` in C, takes any number of arguments with no types. Before variadic templates, C++ "variadic functions could only be implemented using va_list or variadic macros from C, which lack type safety"[^2].

## Pitfalls

- **A limit on the size.** Code that uses overloads or macros for each size has a largest size. A Rust tuple with 13 elements does not implement `Debug`, because the trait is implemented only up to 12[^9].
- **Recursion in the compiler.** A C++ pack processed by recursion makes one instantiation for each element. Use a fold expression or a pack expansion where you can.
- **A pack is not a value.** In C++, a pack is not a tuple. The program cannot store it in a variable; it expands it or puts it in a `std::tuple`.
- **Checks only in the checker.** In Python and TypeScript, the types of a variadic generic are checked only by the type checker.

## History

The variadic template feature of C++ "was designed by Douglas Gregor and Jaakko Järvi"[^2], and before it, "templates (classes and functions) could only take a fixed number of arguments"[^2]. PEP 646 was created on 16 September 2020[^4]. The Swift proposal calls packs "the first step toward variadic generics in Swift"[^7].
