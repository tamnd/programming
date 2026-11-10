---
tag: NEW-cpp
kind: language
slug: cpp
name: C++
area: languages
level: intro
summary: >-
  C++ is a general-purpose language based on C, with static typing, classes, templates and exceptions. The standard ISO/IEC 14882 defines it, and compilers such as GCC, Clang and MSVC implement it.
class: programming
tier: 1
paradigms: [imperative, procedural, object-oriented, generic]
typing: {discipline: static, strength: weak, checking: [static], inference: local}
memory: manual
execution: [compiled-native]
appeared: 1985
designers: [bjarne-stroustrup]
influenced_by: [c, simula, algol-68, bcpl, ada, clu, ml]
standard: iso-iec-14882
reference: {url: "https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2023/n4950.pdf", licence: iso-iec-copyright}
licence_of_reference_impl: null
file_extensions: [cpp, c++, cc, cp, cppm, cxx, h, h++, hh, hpp, hxx, inc, inl, ino, ipp, ixx, re, tcc, tpp, txx]
hello_world: examples/cpp.md#hello
same_as: {wikidata: Q2407, wikipedia: C++, pldb: cpp, linguist: C++, rosetta: null, hopl: null}
sources:
  - url: https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2023/n4950.pdf
    title: N4950, Working Draft, Standard for Programming Language C++ (2023-05-10)
    locator: "1 Scope [intro.scope], paragraph 2"
  - url: https://www.open-std.org/jtc1/sc22/wg21/
    title: ISO/IEC JTC1/SC22/WG21, The C++ Standards Committee
    locator: the home page
    accessed: 2026-11-10
  - url: https://www.stroustrup.com/bs_faq.html#invention
    title: "Bjarne Stroustrup's FAQ"
    locator: When was C++ invented?; Why did you invent C++?
    accessed: 2026-11-10
  - url: https://www.stroustrup.com/hopl-almost-final.pdf
    title: "Bjarne Stroustrup, Evolving a language in and for the real world: C++ 1991-2006, HOPL-III (2007)"
    locator: "2 Background: C++ 1979-1991, the rules of thumb; 2.1 The Birth of C with Classes; 2.5 Chronology"
  - url: https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines
    title: C++ Core Guidelines, edited by Bjarne Stroustrup and Herb Sutter
    locator: "the front matter; R.1 (#rr-raii); R.11 (#rr-newdelete)"
    accessed: 2026-11-10
  - url: https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2023/n4951.html
    title: "N4951, Editors' Report, Programming Languages, C++ (2023-05-10)"
    locator: Introduction
  - url: https://www.iso.org/standard/83626.html
    title: "ISO/IEC 14882:2024, Programming languages, C++"
    locator: Status, Publication date, Edition, Stage
    accessed: 2026-11-10
  - url: https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2023/n4950.pdf
    title: N4950, Working Draft, Standard for Programming Language C++ (2023-05-10)
    locator: "6.9.3.1 main function [basic.start.main], paragraphs 1 and 5; 31.7.10 Print functions [print.fun], paragraphs 3 and 4"
  - url: https://libcxx.llvm.org/Status/Cxx23.html
    title: libc++ C++23 Status
    locator: Overview; Paper Status, P2093R14 Formatted output
    accessed: 2026-11-10
  - url: https://gcc.gnu.org/onlinedocs/libstdc++/manual/status.html
    title: libstdc++ Implementation Status
    locator: C++ 2023, the introduction and the row for P2093R14
    accessed: 2026-11-10
  - url: https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2023/n4950.pdf
    title: N4950, Working Draft, Standard for Programming Language C++ (2023-05-10)
    locator: "11.4.7 Destructors [class.dtor], paragraph 14; 20.3.1.1 [unique.ptr.general], paragraphs 1 and 4"
  - url: https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2023/n4950.pdf
    title: N4950, Working Draft, Standard for Programming Language C++ (2023-05-10)
    locator: "13.1 Preamble [temp.pre], paragraph 1; 13.7.9 Concept definitions [temp.concept], paragraph 1; 18.4.7 Arithmetic concepts [concepts.arithmetic]"
  - url: https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2023/n4950.pdf
    title: N4950, Working Draft, Standard for Programming Language C++ (2023-05-10)
    locator: "7.5.5.2 Closure types [expr.prim.lambda.closure], paragraph 1; 7.5.5.3 Captures [expr.prim.lambda.capture], paragraph 10; 27.8.2 Sorting [alg.sort]; 27.6.11 Count [alg.count]"
  - url: https://github.com/github-linguist/linguist/blob/main/lib/linguist/languages.yml
    title: GitHub Linguist, languages.yml
    locator: C++, extensions
    accessed: 2026-11-10
  - url: https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2023/n4950.pdf
    title: N4950, Working Draft, Standard for Programming Language C++ (2023-05-10)
    locator: "6.8.2 Fundamental types [basic.fundamental], paragraphs 1 and 2 and Note 2; 7.1 Preamble [expr.pre], paragraph 4; 7.3.9 Integral conversions [conv.integral]"
  - url: https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2023/n4950.pdf
    title: N4950, Working Draft, Standard for Programming Language C++ (2023-05-10)
    locator: "9.3.4.2 Pointers [dcl.ptr]; 9.3.4.3 References [dcl.ref]; 9.2.9.6 Placeholder type specifiers [dcl.spec.auto]; 22.5.1 [optional.general], paragraph 1"
  - url: https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2023/n4950.pdf
    title: N4950, Working Draft, Standard for Programming Language C++ (2023-05-10)
    locator: "3.65 undefined behavior [defns.undefined]; 4.1.1 [intro.compliance.general], paragraph 1"
  - url: https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2023/n4950.pdf
    title: N4950, Working Draft, Standard for Programming Language C++ (2023-05-10)
    locator: "6.9.2.2 Data races [intro.races], paragraph 21; 33.4 Threads [thread.threads]"
  - url: https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2023/n4950.pdf
    title: N4950, Working Draft, Standard for Programming Language C++ (2023-05-10)
    locator: "6.7.5.4 Automatic storage duration [basic.stc.auto], paragraph 1; 6.7.5.5.1 [basic.stc.dynamic.general], paragraph 1; 6.7.3 Lifetime [basic.life], paragraphs 6 and 7"
  - url: https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2023/n4950.pdf
    title: N4950, Working Draft, Standard for Programming Language C++ (2023-05-10)
    locator: "4.1.2 Abstract machine [intro.abstract], paragraph 1; 5.2 Phases of translation [lex.phases]; 15.3 Source file inclusion [cpp.include]; 10.1 Module units and purviews [module.unit]; 16.4.2.4 Modules [std.modules]"
  - url: https://gcc.gnu.org/projects/cxx-status.html
    title: C++ Standards Support in GCC
    locator: C++23 Support in GCC; C++26 Support in GCC; C++20 Support in GCC
    accessed: 2026-11-10
  - url: https://github.com/microsoft/STL/wiki/Changelog
    title: microsoft/STL wiki, Changelog
    locator: the table of contents; MSVC Build Tools 14.52 Preview; MSVC Build Tools 14.51
    accessed: 2026-11-10
  - url: https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2023/n4950.pdf
    title: N4950, Working Draft, Standard for Programming Language C++ (2023-05-10)
    locator: "14.3 Constructors and destructors [except.ctor], paragraphs 1 and 2"
  - url: https://www.open-std.org/jtc1/sc22/wg21/docs/standards
    title: WG21, C++ Standards
    locator: the editions and their publication dates; the working drafts
    accessed: 2026-11-10
  - url: https://www.iso.org/standard/79358.html
    title: "ISO/IEC 14882:2020, and the pages of the editions of 2017 (68564), 2014 (64029), 2011 (50372), 2003 (38110) and 1998 (25845)"
    locator: Status, Publication date, Edition, Stage
    accessed: 2026-11-10
  - url: https://www.iso.org/standard/91179.html
    title: ISO/IEC DIS 14882, Programming languages, C++
    locator: Status, Stage, Edition
    accessed: 2026-11-10
  - url: https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2026/n5051.html
    title: "N5051, Editors' Report, Programming Languages, C++ (2026-06-01)"
    locator: Introduction
  - url: https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2026/n5056.pdf
    title: "N5056, WG21 convener's report (2026-07-28)"
    locator: 1.2.1; 1.2.2; 3.5
  - url: https://gcc.gnu.org/releases.html
    title: GCC Releases
    locator: the table of releases
    accessed: 2026-11-10
  - url: https://gcc.gnu.org/git/?p=gcc.git;a=blob;f=gcc/cp/parser.cc;hb=releases/gcc-16.2.0
    title: gcc/cp/parser.cc at the tag releases/gcc-16.2.0
    locator: the licence header
  - url: https://clang.llvm.org/cxx_status.html
    title: C++ Support in Clang
    locator: the table of standards; C++2c implementation status
    accessed: 2026-11-10
  - url: https://github.com/llvm/llvm-project/releases/tag/llvmorg-23.1.5
    title: LLVM 23.1.5, GitHub release
    locator: the release date
  - url: https://github.com/llvm/llvm-project/blob/llvmorg-23.1.5/LICENSE.TXT
    title: llvm-project LICENSE.TXT at llvmorg-23.1.5
    locator: the first lines; libcxx/LICENSE.TXT
  - url: https://learn.microsoft.com/en-us/cpp/overview/visual-cpp-language-conformance
    title: Microsoft C/C++ language conformance by Visual Studio version
    locator: C++23 Core language features; notes 23 and 24
    accessed: 2026-11-10
  - url: https://learn.microsoft.com/en-us/cpp/build/reference/std-specify-language-standard-version
    title: /std (Specify Language Standard Version), Microsoft Learn
    locator: "/std:c++14; /std:c++23preview"
    accessed: 2026-11-10
  - url: https://learn.microsoft.com/en-us/visualstudio/releases/2026/release-notes
    title: Visual Studio 2026 Release Notes
    locator: Version 18.11.3; May Update 18.6.0, MSVC Build Tools v14.51
    accessed: 2026-11-10
  - url: https://visualstudio.microsoft.com/license-terms/
    title: Visual Studio License Terms
    locator: Visual Studio 2026
    accessed: 2026-11-10
  - url: https://gcc.gnu.org/onlinedocs/libstdc++/manual/license.html
    title: libstdc++ manual, License
    locator: "The Code: GPL"
    accessed: 2026-11-10
  - url: https://libcxx.llvm.org/Status/Cxx20.html
    title: libc++ C++20 Status
    locator: Paper Status
    accessed: 2026-11-10
  - url: https://github.com/microsoft/STL/blob/main/LICENSE.txt
    title: microsoft/STL LICENSE.txt
    locator: the first line
    accessed: 2026-11-10
  - url: https://learn.microsoft.com/en-us/vcpkg/get_started/overview
    title: vcpkg overview, Microsoft Learn
    locator: What is vcpkg?
    accessed: 2026-11-10
  - url: https://docs.conan.io/2/introduction.html
    title: Conan 2 documentation, Introduction
    locator: the introduction
    accessed: 2026-11-10
  - url: https://cmake.org/cmake/help/latest/
    title: CMake Reference Documentation
    locator: the introduction
    accessed: 2026-11-10
  - url: https://google.github.io/googletest/
    title: "GoogleTest User's Guide"
    locator: "Welcome to GoogleTest!"
    accessed: 2026-11-10
  - url: https://clang.llvm.org/docs/ClangFormat.html
    title: ClangFormat, Clang documentation
    locator: the introduction
    accessed: 2026-11-10
  - url: https://clang.llvm.org/extra/clang-tidy/
    title: Clang-Tidy, Extra Clang Tools documentation
    locator: the introduction
    accessed: 2026-11-10
  - url: https://clang.llvm.org/docs/AddressSanitizer.html
    title: AddressSanitizer, Clang documentation
    locator: Introduction
    accessed: 2026-11-10
  - url: https://clang.llvm.org/docs/UndefinedBehaviorSanitizer.html
    title: UndefinedBehaviorSanitizer, Clang documentation
    locator: Introduction
    accessed: 2026-11-10
  - url: https://www.stroustrup.com/bs_faq.html#name
    title: "Bjarne Stroustrup's FAQ"
    locator: "Where did the name \"C++\" come from?; Which language did you use to write C++?"
    accessed: 2026-11-10
  - url: https://www.stroustrup.com/hopl2.pdf
    title: "Bjarne Stroustrup, A History of C++: 1979-1991, HOPL-II (1993)"
    locator: 1 Introduction; 2.4.4 Why C?; 3.5 Tools for Language Design
  - url: https://www.stroustrup.com/dne.html
    title: Bjarne Stroustrup, The Design and Evolution of C++
    locator: the book page
    accessed: 2026-11-10
  - url: https://www.wikidata.org/wiki/Q2407
    title: C++, Wikidata
    locator: inception (P571); designed by (P287)
    accessed: 2026-11-10
  - url: https://pldb.io/concepts/cpp.html
    title: C++, PLDB
    locator: the summary
    accessed: 2026-11-10
---

## Overview

C++ is a general-purpose programming language based on C. Its standard says that C++ adds data types, classes, templates, exceptions, namespaces, operator and function name overloading, references, free store management operators and library facilities to C.[^1] The international standard ISO/IEC 14882 defines the language. The ISO/IEC committee JTC1/SC22/WG21, the C++ Standards Committee, develops it.[^2]

Bjarne Stroustrup started the work at Bell Labs in 1979.[^3][^4] He wanted to write efficient systems programs in the styles of Simula67, so he added better type checking, data abstraction and object-oriented programming to C.[^3] His rules of thumb for the design include "What you don't use, you don't pay for (zero-overhead rule)" and "Leave no room for a lower-level language below C++ (except assembler)".[^4] The C++ Core Guidelines, which Stroustrup and Herb Sutter edit, give rules for the use of the language. Rule R.1 is "Manage resources automatically using resource handles and RAII".[^5]

This guide cites N4950, a public working draft of 2023-05-10. Its editors' report calls N4950 "the current and final working draft for C++23" and says that it forms the basis of the Draft International Standard for C++23.[^6] ISO published that edition as ISO/IEC 14882:2024 in October 2024.[^7] The guide gives each section of N4950 with its number and its stable name, for example 6.7.3 [basic.life].

## A first program

```cpp run id=hello
#include <print>

int main() {
    std::println("Hello, world!");
}
```

```output for=hello
Hello, world!
```

A program contains exactly one function `main` in the global scope. If control flows off the end of `main`, the effect is the same as `return 0`.[^8] `std::println` formats its arguments, writes them to the standard output and adds a new line.[^8] It comes from the C++23 paper P2093, which libc++ marks as complete.[^9] libstdc++ has it from GCC 14.1, with `-std=c++23`.[^10]

The program below shows how C++ ties the life of a resource to the life of an object. The class `Guard` prints a line in its constructor and another in its destructor.

```cpp run id=raii
#include <algorithm>
#include <concepts>
#include <memory>
#include <print>
#include <string>
#include <utility>
#include <vector>

class Guard {
public:
    explicit Guard(std::string name) : name_(std::move(name)) {
        std::println("open {}", name_);
    }
    ~Guard() { std::println("close {}", name_); }

private:
    std::string name_;
};

template <typename T>
concept Number = std::integral<T> || std::floating_point<T>;

template <Number T>
T total(const std::vector<T>& values) {
    T sum{};
    for (const T& v : values) {
        sum += v;
    }
    return sum;
}

int main() {
    Guard outer("outer");
    std::vector<int> scores{42, 7, 19, 88, 3};
    std::ranges::sort(scores);
    int limit = 10;
    auto above = [limit](int s) { return s > limit; };
    std::println("sorted {}", scores);
    std::println("above {}: {}", limit, std::ranges::count_if(scores, above));
    std::println("total {}", total(scores));
    std::println("mean {}", total(std::vector<double>{1.5, 2.5, 4.0}) / 3);
    {
        auto inner = std::make_unique<Guard>("inner");
        std::println("in block");
    }
    std::println("after block");
}
```

```output for=raii
open outer
sorted [3, 7, 19, 42, 88]
above 10: 3
total 159
mean 2.6666666666666665
open inner
in block
close inner
after block
close outer
```

The destructor of an automatic object runs when the block in which the object was created exits. A `std::unique_ptr` owns another object and disposes of it when the `unique_ptr` itself is destroyed. A `unique_ptr` can move but cannot be copied.[^11] So `inner` closes at the end of its block, and `outer` closes last, at the end of `main`. The template `total` is a family of functions, one for each type `T`. The concept `Number` constrains `T` to the integral and floating-point types of the concepts library.[^12] The lambda expression makes a closure object, and `[limit]` captures `limit` by copy. `std::ranges::sort` and `std::ranges::count_if` are algorithms of the standard library.[^13]

## The language at a glance

Source files often have the extensions `.cpp` and `.hpp`. GitHub Linguist counts files with the extensions `.cpp`, `.c++`, `.cc`, `.cp`, `.cppm`, `.cxx`, `.h`, `.h++`, `.hh`, `.hpp`, `.hxx`, `.inc`, `.inl`, `.ino`, `.ipp`, `.ixx`, `.re`, `.tcc`, `.tpp` and `.txx` as C++.[^14]

**Types.** C++ is statically typed, and one of Stroustrup's rules is "No implicit violations of the static type system".[^4] The fundamental types include the signed and unsigned integer types, `bool`, the character types and the floating-point types. Unsigned arithmetic is modulo 2^N and does not overflow. Signed overflow has undefined behavior ([[integer-overflow]]).[^15] A conversion to an integer type gives the value that is congruent to the source modulo 2^N, so it can change the value ([[type-conversion]]).[^15] Pointers and references are separate declarator forms ([[pointers-and-references]]). `auto` lets the compiler deduce the type of a variable from its initializer. `std::optional` holds a value or nothing.[^16]

```cpp run id=types
#include <cstdint>
#include <optional>
#include <print>
#include <vector>

std::optional<int> digit(char c) {
    if (c >= '0' && c <= '9') {
        return c - '0';
    }
    return std::nullopt;
}

int main() {
    std::uint32_t count = 0;
    count -= 1;
    std::println("{}", count);

    std::vector<int> a{1, 2, 3};
    std::vector<int> b = a;
    int& first = a[0];
    first = 10;
    std::println("{} {}", a, b);

    auto seven = digit('7');
    auto none = digit('x');
    std::println("{} {}", seven.value_or(-1), none.has_value());
}
```

```output for=types
4294967295
[10, 2, 3] [1, 2, 3]
7 false
```

The unsigned subtraction wraps around. `b` is a copy of `a`, so a change through the reference `first` changes only `a`.

**Undefined behavior.** The standard defines undefined behavior as "behavior for which this document imposes no requirements". An implementation does not have to diagnose it.[^17] Signed overflow, a data race and access to an object after its lifetime has ended are examples.[^15][^18][^19] A program on this site never shows undefined behavior as working code.

**Memory.** The storage of an automatic object lasts until its block exits. A `new` expression creates an object with dynamic storage duration, and a `delete` expression destroys it.[^19] A pointer to an object whose lifetime has ended can be used only in limited ways. A use outside those ways has undefined behavior ([[use-after-free]]).[^19] Core Guideline R.11 says to avoid calling `new` and `delete` explicitly, and to give the pointer to a resource handle.[^5]

**Execution.** The standard describes a parameterized nondeterministic abstract machine. An implementation needs to emulate only its observable behavior. The standard defines the phases of translation. A source file can include headers, and a program can also have module units.[^20] Modules are a C++20 feature, and GCC calls its support for them experimental.[^21] From C++23, the standard library has the named modules `std` and `std.compat`.[^20][^22]

**Paradigms.** Classes give data abstraction and object-oriented programming.[^3] Templates and concepts give generic code ([[parametric-polymorphism]]).[^12] Lambda expressions make closure objects ([[closures]]).[^13] The library has `std::thread` ([[threads]]). A data race between two conflicting actions has undefined behavior ([[data-races]]).[^18] An exception unwinds the stack and destroys the automatic objects on the way ([[exception-handling]]).[^23]

## The guide

The guide to C++ has the 25 parts of the language template. Only this page exists so far, so the table has no links yet.

| Part | What it covers in C++ |
|---|---|
| Overview and history | this page: design goals, editions of the standard, implementations, history |
| Getting started | installing GCC, Clang or MSVC, `-std=c++23`, compiling and running one file |
| Lexical structure and syntax | phases of translation, the preprocessor, keywords, literals, declarators |
| Values and types | fundamental types, pointers, references, `auto`, `const`, conversions |
| Variables, binding and scope | declarations, storage duration, block and namespace scope, lifetime |
| Control flow | `if`, `switch`, `for`, range-based `for`, `while`, `goto` |
| Functions | overloading, default arguments, lambda expressions, `constexpr` and `consteval` |
| Built-in collections | arrays, `std::vector`, `std::array`, `std::map`, `std::unordered_map`, `std::span` |
| Strings and text | `std::string`, `std::string_view`, `std::format`, character types and encodings |
| User-defined types | classes, constructors and destructors, operator overloading, inheritance, `enum class` |
| Errors and exceptions | `throw`, `try` and `catch`, `noexcept`, `std::expected`, error codes |
| Memory and resources | storage duration, RAII, `new` and `delete`, `std::unique_ptr` and `std::shared_ptr` |
| Modules, packages and builds | headers, the one-definition rule, modules, `import std`, CMake, vcpkg and Conan |
| Concurrency and parallelism | `std::thread`, `std::jthread`, mutexes, atomics, the memory model, coroutines |
| Metaprogramming | templates, concepts, `constexpr` evaluation, type traits |
| The standard library | a map of the library clauses; the api cards hang here |
| I/O, files and the OS | iostreams, `<print>`, `<filesystem>`, `<chrono>` |
| Testing | GoogleTest, `static_assert`, the sanitizers |
| Tooling | compilers and their flags, clang-format, clang-tidy, debuggers, sanitizers |
| Ecosystem | package managers, build systems, libraries by need |
| Performance | the zero-overhead rule, move semantics, inlining, profiling |
| Interoperability | `extern "C"`, the C library headers, ABIs |
| Idioms and style | the C++ Core Guidelines, RAII, the rule of zero, value semantics |
| Pitfalls | undefined behavior, dangling references, signed overflow, implicit conversions |
| Versions and migration | the editions C++98 to C++26, feature-test macros, compiler `-std` flags |

## Versions and support

The versions of C++ are the editions of ISO/IEC 14882. ISO marks each edition before the 2024 edition as withdrawn. A standard has no end of life, so the last column does not apply. The editors' report N4951 calls N4950 the final working draft for C++23.[^6] WG21 lists the editions from 1998 to 2020 with their dates and working drafts.[^24]

| Version | Status | First release | Newest release | End of life |
|---|---|---|---|---|
| C++26 | draft; ISO/IEC DIS 14882, stage 40.60 (close of voting) | none | none | not applicable |
| C++23 | published; ISO stage 90.92 (to be revised) | 2024-10 | ISO/IEC 14882:2024, edition 7 | not applicable |
| C++20 | withdrawn | 2020-12 | ISO/IEC 14882:2020, edition 6 | not applicable |
| C++17 | withdrawn | 2017-12 | ISO/IEC 14882:2017, edition 5 | not applicable |
| C++14 | withdrawn | 2014-12 | ISO/IEC 14882:2014, edition 4 | not applicable |
| C++11 | withdrawn | 2011-09 | ISO/IEC 14882:2011, edition 3 | not applicable |
| C++03 | withdrawn | 2003-10 | ISO/IEC 14882:2003, edition 2 | not applicable |
| C++98 | withdrawn | 1998-09 | ISO/IEC 14882:1998, edition 1 | not applicable |

The statuses, months and edition numbers come from the ISO pages.[^7][^25] The ISO page of the 2024 edition says that the DIS is expected to replace it.[^7] The ISO page of the DIS gives stage 40.60 and edition 8.[^26] The editors' report N5051 says that the working draft N5050 forms the basis of the DIS for C++26.[^27] The convener's report N5056 says that the next edition "is now in DIS ballot on schedule to be completed in 2026".[^28] The guide states facts for C++23. A fact that holds only from a later edition gives that edition. The examples compile with `-std=c++23 -Wall`.

## Implementations

The standard has no reference implementation. Compilers and standard libraries implement it, and each states its own level of support. The table gives the newest release of each on 2026-11-10.

| Implementation | Runs on | C++ version | Newest release | Licence |
|---|---|---|---|---|
| GCC (`g++`) | the GNU Compiler Collection | C++20 by default; C++23 and C++26 are experimental | 16.2 (2026-08-07) | GPL-3.0-or-later |
| Clang | the C and C++ front end of LLVM | C++17 by default; C++20, C++23 and C++2c are partial | 23.1.5 (2026-11-03) | Apache-2.0 WITH LLVM-exception |
| MSVC | Windows, with Visual Studio 2026 | C++14 by default; C++20 with `/std:c++20`; C++23 features with `/std:c++23preview` or `/std:c++latest` | Visual Studio 2026 18.11.3 (2026-10-29), MSVC Build Tools 14.51 | Microsoft Software License Terms |
| libstdc++ | the standard library of GCC | C++17 not experimental from GCC 9.1; C++20 and C++23 feature by feature | GCC 16.2 (2026-08-07) | GPL-3.0-or-later WITH GCC-exception-3.1 |
| libc++ | the standard library of LLVM | most C++20 and C++23 papers complete, some partial | 23.1.5 (2026-11-03) | Apache-2.0 WITH LLVM-exception |
| MSVC STL | the standard library of MSVC | C++20; C++23 feature complete in the 14.52 Preview | MSVC Build Tools 14.51 (shipped in Visual Studio 2026 18.6, 2026-05-12) | Apache-2.0 WITH LLVM-exception |

Sources: GCC[^21][^29][^30], Clang[^31][^32][^33], MSVC[^34][^35][^36][^37], libstdc++[^10][^38], libc++[^39][^9][^33] and the MSVC STL[^34][^22][^40]. GCC 13.5 came out on 2026-09-11, later than 16.2, as a release of an older series.[^29] The GCC page says that the ABI of C++20 features was not stable until GCC 16.[^21] The compat tables of the feature pages have a column for the standard and columns for GCC, Clang, MSVC, libstdc++, libc++ and the MSVC STL.

## Ecosystem

The standard does not define a package manager or a build system. vcpkg is a free and open-source C and C++ package manager from Microsoft and the C++ community.[^41] Conan is a free and open-source dependency and package manager for C and C++, under the MIT licence.[^42] CMake manages the build of source code. It generates build systems such as Ninja and project files for Visual Studio and Xcode.[^43]

| Need | Tool |
|---|---|
| get libraries | vcpkg[^41]; Conan[^42] |
| build a project | CMake[^43] |
| run tests | GoogleTest, Google's testing and mocking framework[^44] |
| format | clang-format[^45] |
| lint | clang-tidy, a clang-based "linter" tool[^46] |
| find memory errors | AddressSanitizer, for out-of-bounds accesses, use after free and double free[^47] |
| find undefined behavior | UndefinedBehaviorSanitizer, for example for signed integer overflow[^48] |

The guide's Ecosystem part lists the main libraries by need, as library cards.

## History and influence

Stroustrup's FAQ gives the early dates. The first version of C++ was used inside AT&T in August 1983, and the name C++ came into use late that year. The first commercial implementation came out in October 1985, with the first edition of The C++ Programming Language.[^3] Rick Mascitti suggested the name. Stroustrup wrote the first version of the first compiler, Cfront, in C with Classes. Cfront generated C.[^49]

| Date | Event |
|---|---|
| April 1979 | work on C with Classes starts at Bell Labs in Murray Hill[^4] |
| October 1979 | Cpre, a preprocessor that adds Simula-like classes to C[^4] |
| December 1983 | the name C++ is first used, in the final copies of two papers[^49] |
| October 1985 | Cfront Release 1.0, the first commercial release[^3][^4] |
| 1989 | Cfront Release 2.0, The Annotated C++ Reference Manual, the ANSI committee J16[^4] |
| 1991 | the first meeting of ISO WG21, in Lund[^4] |
| 1998-09 | ISO/IEC 14882:1998, the first edition of the standard[^24] |
| 2011-09 | ISO/IEC 14882:2011, the third edition[^24] |
| 2024-10 | ISO/IEC 14882:2024, the edition for C++23[^7] |

The chronology of the HOPL-III paper puts "C++ named" in 1984.[^4] The HOPL-II paper says that C++ was designed to give Simula's facilities for program organization together with C's efficiency and flexibility for systems programming. Only C, Simula, Algol68 and, in one case, BCPL left noticeable traces in C++ as released in 1985. Simula gave classes, and Algol68 gave operator overloading, references and declarations anywhere in a block. BCPL gave `//` comments. Later C++ shows the influence of Ada, Clu and ML.[^50] The Design and Evolution of C++ (Addison-Wesley) tells the history of the design in full.[^51] Wikidata gives 1983 as the inception of C++, and PLDB gives 1985 as the year it was created. Both name Stroustrup.[^52][^53]

## Coming from another language

- From [[c]]: C++ is based on C as ISO/IEC 9899:2018 describes it.[^1] Use destructors and `std::unique_ptr` for cleanup, not paired calls.[^11][^5]
- From [[java]]: an object can live in a local variable with no `new`, and it is destroyed when its block exits.[^19] The initialization of one `std::vector` from another copies the elements, as the `types` example shows. An object from `new` lives until a `delete` expression destroys it ([[garbage-collection]]).[^19]
- From [[rust]]: `std::unique_ptr` has a single owner and can move but not copy, like an owned value in [[rust/ownership]].[^11] A use of an object after its lifetime ends has undefined behavior, and the standard does not require a diagnostic for it.[^19][^17]
- Signed integer overflow has undefined behavior, so a program cannot rely on wrapping or on an error ([[integer-overflow]]).[^15]
- During stack unwinding, the destructors of automatic objects run in the reverse order of the completion of their construction ([[exception-handling]]).[^23]

## References
