---
tag: XK6JZT
kind: concept
slug: template-specialization
name: Specialization
aliases: [template specialization, explicit specialization, full specialization, partial specialization, partial template specialization, impl specialization, trait specialization, overlapping implementations]
area: types/generics
level: advanced
summary: >-
  Specialization gives generic code a special version for some type arguments, such as std::vector<bool> in C++, and a rule picks the most specific version that matches.
applies_to: core-languages
viz: optional
formal: none
see_also: [templates-and-generics, bounded-quantification, parametric-polymorphism, overloading, polymorphism, dynamic-dispatch, type-constructor]
same_as:
  wikidata: Q7140390
  wikipedia: Partial_template_specialization
sources:
- url: https://en.cppreference.com/w/cpp/language/template_specialization.html
  title: Explicit (full) template specialization, C++ reference, cppreference.com
  locator: Introduction
- url: https://en.wikipedia.org/wiki/Partial_template_specialization
  title: Partial template specialization, Wikipedia
  locator: Introduction; Templates and specialization
- url: https://en.cppreference.com/w/cpp/language/partial_specialization.html
  title: Partial template specialization, C++ reference, cppreference.com
  locator: Introduction; Partial ordering
- url: https://en.cppreference.com/w/cpp/language/templates.html
  title: Templates, C++ reference, cppreference.com
  locator: Introduction
- url: https://en.cppreference.com/w/cpp/container/vector_bool.html
  title: std::vector<bool>, C++ reference, cppreference.com
  locator: Introduction
- url: https://doc.rust-lang.org/reference/items/implementations.html
  title: Implementations, The Rust Reference
  locator: Trait implementation coherence
- url: https://rust-lang.github.io/rfcs/1210-impl-specialization.html
  title: RFC 1210, impl specialization, The Rust RFC Book
  locator: Summary; Motivation
- url: https://doc.rust-lang.org/unstable-book/language-features/specialization.html
  title: specialization, The Rust Unstable Book
  locator: Whole page
- url: https://doc.rust-lang.org/unstable-book/the-unstable-book.html
  title: The Unstable Book
  locator: Introduction
---

## Idea

A generic function or type uses one piece of code for every type argument. Sometimes one type argument needs different code: a faster way for integers, a smaller layout for `bool`, or a different text for strings. Specialization lets the program write that version beside the generic one. When the program uses the template with that type, the compiler picks the special version.

```cpp run id=idea
#include <iostream>
#include <string>

template <typename T>
std::string describe(const T&) {
    return "some value";
}

// A full specialization for int.
template <>
std::string describe<int>(const int& x) {
    return "the int " + std::to_string(x);
}

int main() {
    std::cout << describe(2.5) << "\n";
    std::cout << describe(7) << "\n";
}
```

```output for=idea
some value
the int 7
```

The primary template works for every type. The line `template <>` starts a full specialization: a version of `describe` for `int` only. The call with `2.5` uses the primary template, and the call with `7` uses the specialization.

## How it works

**Full specialization.** In C++, explicit specialization "Allows customizing the template code for a given set of template arguments."[^1] The program gives every template argument, as in `template <> class KeyValuePair<int, string>`[^2]. "Specialization must be declared before the first use that would cause implicit instantiation"[^1].

**Partial specialization.** A class template can also be specialized for a group of type arguments. Partial specialization customizes class and variable templates "for a given category of template arguments"[^3], such as every pointer type `T*` or every pair whose two types are the same. It "allows the programmer to specialize only some arguments of a class template, as opposed to explicit full specialization, where all the template arguments are provided"[^2]. In C++, "partial specializations are only allowed for class templates and variable templates"[^4], so a function template uses overloading instead.

**Choosing a version.** More than one version can match. "If more than one specialization matches, partial order rules are used to determine which specialization is more specialized. The most specialized specialization is used, if it is unique"[^3]. Informally, "A is more specialized than B" when A accepts a subset of the types that B accepts[^3]. If no specialization matches, the compiler uses the primary template[^3].

**A known case.** The C++ standard library has `std::vector<bool>`, which "is a possibly space-efficient specialization of std::vector for the type bool"[^5]. It can store each element as one bit, so it "Does not necessarily store its elements as a contiguous array"[^5].

**Specialization in Rust.** Rust does not let two trait `impl` blocks apply to one type. "A trait implementation is considered incoherent if either the orphan rules check fails or there are overlapping implementation instances."[^6] RFC 1210 proposes a design for specialization, "which permits multiple impl blocks to apply to the same type/trait"[^7], when one of the blocks is clearly more specific than the other. The feature is in the Unstable Book, with the tracking issue #31844[^8]. A program that uses an unstable feature "must use a flag"[^9], so stable Rust does not have specialization.

## In languages

| Language | Verdict | Mechanism |
|---|---|---|
| Python | absent | no generic code to specialize; `functools.singledispatch` picks a function by the type of the argument when the program runs |
| JavaScript | n/a | the language has no type parameters |
| TypeScript | absent | types are removed before the program runs; overload signatures and type tests are the usual way |
| Java | absent | generics are erased, so a version for one type argument is not possible; overloads take its place |
| Kotlin | absent | generics are erased on the JVM; overloads and `when` on the type take its place |
| C# | absent | no specialization; a generic method can test `typeof(T)` |
| Go | absent | no specialization; a type switch inside a generic function takes its place |
| Rust | absent | specialization is an unstable feature; stable Rust has separate impls for separate types |
| C | n/a | the language has no templates |
| C++ | native | full specialization of templates and partial specialization of class and variable templates |
| Bash | n/a | the language has no types |
| PowerShell | n/a | a script cannot declare its own generic code |
| SQL | n/a | a query has no type parameters |
| Regex | n/a | a pattern has no types |
| HTML | n/a | HTML has no types |
| CSS | n/a | CSS has no types of this kind |

C++ is the only one of these languages with specialization as a stable feature. The others use overloads, a test on the type, or a separate implementation for each type.

The example files show C++ full and partial specialization, and the Rust error for two overlapping `impl` blocks, with the stable way to give each type its own code.

## Variations and related ideas

- **Overloading.** A function template cannot be partially specialized, but an overload with a more specific parameter, such as `f(T*)`, does the same work. See [overloading](/overloading/).
- **Specialization for speed.** Part of the motivation of RFC 1210 is performance: "specialized impls can provide custom high-performance code for particular, concrete cases of an abstraction"[^7].
- **Choosing by a test.** A language without specialization can test the type argument inside the generic code, for example with `typeof(T)` in C# or a type switch in Go. The test runs in the generic code, so the compiler cannot reject a missing case.

## Pitfalls

- **A specialization that changes the behaviour.** A specialization can do something that the primary template does not. `std::vector<bool>` does not store its elements as an array of `bool`[^5], and its elements are reached through a proxy class, `std::vector<bool>::reference`[^5]. Code that takes a `bool&` or a `bool*` to an element works for other vectors and does not compile for `std::vector<bool>`.
- **A specialization declared too late.** The specialization must come before the first use that instantiates the template, in each translation unit[^1]. Put it in the same header as the primary template.
- **Two specializations that match equally.** If no matching specialization is more specialized than the others, "the program cannot be compiled"[^3]. Add a specialization for the overlap.
- **Specialization based on lifetimes in Rust.** RFC 1210 says that the compiler "cannot, and should not allow specialization based on lifetimes"[^7], because lifetimes are erased before the code is made.

## History

Partial specialization was not in every C++ compiler: "Some older compilers allow one only to specialize either all or none of the template's parameters"[^2]. For Rust, RFC 1210 has the start date 2015-06-17[^7], and the feature has a page in the Unstable Book, with its tracking issue[^8].
