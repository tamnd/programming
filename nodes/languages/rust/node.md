---
tag: CX4NX1
kind: language
slug: rust
name: Rust
area: languages
level: intro
summary: >-
  Rust is a general-purpose systems language with static, strong typing and memory managed through ownership, with no garbage collector. rustc is its reference implementation.
class: programming
tier: 1
paradigms: [imperative, functional, generic, concurrent]
typing: {discipline: static, strength: strong, checking: [static], inference: local}
memory: ownership
execution: [compiled-native]
appeared: 2010
designers: [graydon-hoare]
influenced_by: [standard-ml, ocaml, cpp, ml-kit, cyclone, haskell, newsqueak, alef, limbo, erlang, swift, scheme, csharp, ruby, nil-language, hermes]
standard: null
reference: {url: "https://doc.rust-lang.org/reference/", licence: mit-or-apache-2.0}
licence_of_reference_impl: mit-or-apache-2.0
file_extensions: [rs, rs.in]
hello_world: examples/rust.md#hello
same_as: {wikidata: Q575650, wikipedia: Rust_(programming_language), pldb: rust, linguist: Rust, rosetta: null, hopl: null}
sources:
  - url: https://www.rust-lang.org/
    title: "Rust Programming Language, rust-lang.org"
    locator: the front page; Why Rust?
    accessed: 2026-11-10
  - url: https://doc.rust-lang.org/1.99.1/book/ch00-00-introduction.html#who-rust-is-for
    title: "The Rust Programming Language, Introduction, Rust 1.99.1"
    locator: Who Rust Is For; Teams of Developers; People Who Value Speed and Stability
  - url: https://blog.rust-lang.org/2015/05/15/Rust-1.0/
    title: "Announcing Rust 1.0, Rust Blog"
    locator: the introduction
  - url: https://doc.rust-lang.org/1.99.1/book/ch09-02-recoverable-errors-with-result.html#the--operator-shortcut
    title: "The Rust Programming Language, Recoverable Errors with Result, Rust 1.99.1"
    locator: The ? Operator Shortcut
  - url: https://github.com/github-linguist/linguist/blob/main/lib/linguist/languages.yml
    title: "GitHub Linguist, languages.yml"
    locator: "Rust, extensions"
    accessed: 2026-11-10
  - url: https://doc.rust-lang.org/1.99.1/book/ch03-02-data-types.html#integer-overflow
    title: "The Rust Programming Language, Data Types, Rust 1.99.1"
    locator: Data Types; Integer Overflow
  - url: https://doc.rust-lang.org/1.99.1/book/ch03-03-how-functions-work.html#parameters
    title: "The Rust Programming Language, Functions, Rust 1.99.1"
    locator: Parameters
  - url: https://rustc-dev-guide.rust-lang.org/type-inference.html#type-inference
    title: "Rust Compiler Development Guide, Type inference"
    locator: "Type inference, the introduction"
    accessed: 2026-11-10
  - url: https://doc.rust-lang.org/1.99.1/rust-by-example/types/cast.html#casting
    title: "Rust By Example, Casting, Rust 1.99.1"
    locator: Casting
  - url: https://doc.rust-lang.org/1.99.1/reference/items/traits.html#r-items.traits.intro
    title: "The Rust Reference, Traits, Rust 1.99.1"
    locator: items.traits.intro
  - url: https://doc.rust-lang.org/1.99.1/book/ch10-01-syntax.html#performance-of-code-using-generics
    title: "The Rust Programming Language, Generic Data Types, Rust 1.99.1"
    locator: Performance of Code Using Generics
  - url: https://doc.rust-lang.org/1.99.1/reference/types/enum.html#r-type.enum.intro
    title: "The Rust Reference, Enumerated types, Rust 1.99.1"
    locator: type.enum.intro
  - url: https://doc.rust-lang.org/1.99.1/book/ch06-01-defining-an-enum.html#the-option-enum
    title: "The Rust Programming Language, Defining an Enum, Rust 1.99.1"
    locator: The Option Enum
  - url: https://doc.rust-lang.org/1.99.1/book/ch04-01-what-is-ownership.html#what-is-ownership
    title: "The Rust Programming Language, What Is Ownership?, Rust 1.99.1"
    locator: What Is Ownership?
  - url: https://doc.rust-lang.org/1.99.1/reference/unsafety.html#r-safety.intro
    title: "The Rust Reference, Unsafety, Rust 1.99.1"
    locator: safety.intro; safety.unsafe-ops
  - url: https://doc.rust-lang.org/1.99.1/book/ch20-01-unsafe-rust.html#performing-unsafe-superpowers
    title: "The Rust Programming Language, Unsafe Rust, Rust 1.99.1"
    locator: Performing Unsafe Superpowers
  - url: https://doc.rust-lang.org/1.99.1/rustc/what-is-rustc.html#what-is-rustc
    title: "The rustc book, What is rustc?, Rust 1.99.1"
    locator: What is rustc?
  - url: https://rustc-dev-guide.rust-lang.org/backend/codegen.html#code-generation
    title: "Rust Compiler Development Guide, Code generation"
    locator: "Code generation, the introduction"
    accessed: 2026-11-10
  - url: https://doc.rust-lang.org/1.99.1/rustc/platform-support.html#tier-1-with-host-tools
    title: "The rustc book, Platform Support, Rust 1.99.1"
    locator: Tier 1 with Host Tools; Tier 2 with Host Tools
  - url: https://doc.rust-lang.org/1.99.1/book/ch13-00-functional-features.html#functional-language-features-iterators-and-closures
    title: "The Rust Programming Language, Functional Language Features: Iterators and Closures, Rust 1.99.1"
    locator: the introduction
  - url: https://doc.rust-lang.org/1.99.1/book/ch18-01-what-is-oo.html#objects-contain-data-and-behavior
    title: "The Rust Programming Language, Characteristics of Object-Oriented Languages, Rust 1.99.1"
    locator: Objects Contain Data and Behavior; Inheritance as a Type System and as Code Sharing
  - url: https://doc.rust-lang.org/1.99.1/book/ch09-00-error-handling.html#error-handling
    title: "The Rust Programming Language, Error Handling, Rust 1.99.1"
    locator: "Error Handling, the introduction"
  - url: https://doc.rust-lang.org/1.99.1/reference/expressions/operator-expr.html#r-expr.try.intro
    title: "The Rust Reference, Operator expressions, Rust 1.99.1"
    locator: expr.try.intro
  - url: https://doc.rust-lang.org/1.99.1/book/ch16-04-extensible-concurrency-sync-and-send.html#transferring-ownership-between-threads
    title: "The Rust Programming Language, Extensible Concurrency with Send and Sync, Rust 1.99.1"
    locator: Transferring Ownership Between Threads; Accessing from Multiple Threads
  - url: https://doc.rust-lang.org/1.99.1/nomicon/races.html#data-races-and-race-conditions
    title: "The Rustonomicon, Data Races and Race Conditions, Rust 1.99.1"
    locator: Data Races and Race Conditions
  - url: https://doc.rust-lang.org/1.99.1/book/ch17-00-async-await.html#fundamentals-of-asynchronous-programming-async-await-futures-and-streams
    title: "The Rust Programming Language, Fundamentals of Asynchronous Programming: Async, Await, Futures, and Streams, Rust 1.99.1"
    locator: the introduction
  - url: https://rust-lang.github.io/rustup/concepts/channels.html#channels
    title: "Channels, The rustup book"
    locator: Channels
    accessed: 2026-11-10
  - url: https://doc.rust-lang.org/1.99.1/book/appendix-07-nightly-rust.html#maintenance-time
    title: "The Rust Programming Language, Appendix G, How Rust is Made and \"Nightly Rust\", Rust 1.99.1"
    locator: "Choo, Choo! Release Channels and Riding the Trains; Maintenance time"
  - url: https://www.rust-lang.org/policies/security#security-receiving
    title: "Rust Security Policy, rust-lang.org"
    locator: Receiving security updates
    accessed: 2026-11-10
  - url: https://github.com/rust-lang/rust/blob/master/RELEASES.md
    title: "RELEASES.md, rust-lang/rust"
    locator: Version 1.99.1 (2026-10-15) to Version 1.96.0 (2026-05-28)
    accessed: 2026-11-10
  - url: https://forge.rust-lang.org/#current-release-versions
    title: Rust Forge
    locator: Current Release Versions
    accessed: 2026-11-10
  - url: https://endoflife.date/api/v1/products/rust/
    title: "Rust, endoflife.date API"
    locator: releases 1.96 to 1.99
    accessed: 2026-11-10
  - url: https://doc.rust-lang.org/edition-guide/editions/index.html#what-are-editions
    title: "What are editions?, The Rust Edition Guide"
    locator: What are editions?; Editions do not split the ecosystem
    accessed: 2026-11-10
  - url: https://doc.rust-lang.org/edition-guide/rust-2018/index.html#rust-2018
    title: "Rust 2018, The Rust Edition Guide"
    locator: "Rust 2018, Info, Release version"
    accessed: 2026-11-10
  - url: https://doc.rust-lang.org/edition-guide/rust-2021/index.html#rust-2021
    title: "Rust 2021, The Rust Edition Guide"
    locator: "Rust 2021, Info, Release version"
    accessed: 2026-11-10
  - url: https://doc.rust-lang.org/edition-guide/rust-2024/index.html#rust-2024
    title: "Rust 2024, The Rust Edition Guide"
    locator: "Rust 2024, Info, Release version"
    accessed: 2026-11-10
  - url: https://doc.rust-lang.org/1.99.1/reference/introduction.html#introduction
    title: "The Rust Reference, Introduction, Rust 1.99.1"
    locator: Introduction; What The Reference is not; Conventions
  - url: https://blog.rust-lang.org/2025/03/26/adopting-the-fls/
    title: "Adopting the FLS, Rust Blog"
    locator: "Adopting the FLS; What's this mean for the Rust Reference?"
  - url: https://github.com/rust-lang/rust/blob/1.99.1/README.md#license
    title: "README.md of rust-lang/rust, tag 1.99.1"
    locator: License
  - url: https://ferrocene.dev/blogs/ferrocene-26080/
    title: "Ferrocene 26.08.0 now available, Ferrocene blog"
    locator: What is new in Ferrocene 26.08.0
    accessed: 2026-11-10
  - url: https://github.com/ferrocene/ferrocene/blob/main/README.md
    title: Ferrocene README
    locator: License and trademark
    accessed: 2026-11-10
  - url: https://gcc.gnu.org/gcc-14/changes.html#rust
    title: "GCC 14 Release Series, Changes, New Features, and Fixes"
    locator: Rust
  - url: https://gcc.gnu.org/gcc-15/changes.html#rust
    title: "GCC 15 Release Series, Changes, New Features, and Fixes"
    locator: Rust
  - url: https://gcc.gnu.org/releases.html
    title: GCC Releases
    locator: "the table of releases, GCC 16.2"
    accessed: 2026-11-10
  - url: https://github.com/thepowersgang/mrustc/blob/master/README.md
    title: mrustc README
    locator: Intro; Progress
    accessed: 2026-11-10
  - url: https://doc.rust-lang.org/1.99.1/cargo/index.html#the-cargo-book
    title: "The Cargo Book, Rust 1.99.1"
    locator: "The Cargo Book, the introduction"
  - url: https://rust-lang.github.io/rustup/index.html#introduction
    title: The rustup book
    locator: Introduction
    accessed: 2026-11-10
  - url: https://docs.rs/about
    title: About Docs.rs
    locator: About Docs.rs
    accessed: 2026-11-10
  - url: https://doc.rust-lang.org/1.99.1/cargo/commands/cargo-test.html#description
    title: "cargo test, The Cargo Book, Rust 1.99.1"
    locator: DESCRIPTION
  - url: https://github.com/rust-lang/rustfmt/blob/main/README.md
    title: rustfmt README
    locator: the introduction
    accessed: 2026-11-10
  - url: https://doc.rust-lang.org/1.99.1/clippy/index.html#clippy
    title: "Clippy Documentation, Rust 1.99.1"
    locator: "Clippy, the introduction"
  - url: https://github.com/rust-lang/rust/commit/c01efc669f09508b55eced32d3c88702578a7c3e
    title: "Initial git commit, rust-lang/rust"
    locator: "the commit c01efc6, author and date"
  - url: https://github.com/rust-lang/rust/blob/c01efc669f09508b55eced32d3c88702578a7c3e/LICENSE.txt#L158-L159
    title: LICENSE.txt of rust-lang/rust at the initial commit
    locator: lines 158 to 159
  - url: https://foundation.rust-lang.org/news/2021-02-08-hello-world/
    title: "Hello World!, The Rust Foundation"
    locator: the announcement
  - url: https://blog.rust-lang.org/2018/12/06/Rust-1.31-and-rust-2018/
    title: "Announcing Rust 1.31 and Rust 2018, Rust Blog"
    locator: the introduction
  - url: https://blog.rust-lang.org/2023/06/20/introducing-leadership-council/
    title: "Introducing the Rust Leadership Council, Rust Blog"
    locator: the introduction
  - url: https://blog.rust-lang.org/2025/02/20/Rust-1.85.0/
    title: "Announcing Rust 1.85.0 and Rust 2024, Rust Blog"
    locator: the introduction; Rust 2024
  - url: https://rust-lang.github.io/rfcs/3392-leadership-council.html#summary
    title: "RFC 3392, Leadership Council"
    locator: Summary
  - url: https://www.rust-lang.org/governance/teams/leadership-council
    title: "Leadership council, rust-lang.org"
    locator: Members
    accessed: 2026-11-10
  - url: https://www.rust-lang.org/governance
    title: "Governance, rust-lang.org"
    locator: RFC process; Top-Level Teams
    accessed: 2026-11-10
  - url: https://github.com/rust-lang/rfcs/blob/master/README.md#what-the-process-is
    title: "Rust RFCs, README"
    locator: What the process is
    accessed: 2026-11-10
  - url: https://doc.rust-lang.org/1.99.1/reference/influences.html#influences
    title: "The Rust Reference, Influences, Rust 1.99.1"
    locator: Influences
  - url: https://doc.rust-lang.org/1.99.1/book/ch03-01-variables-and-mutability.html#variables-and-mutability
    title: "The Rust Programming Language, Variables and Mutability, Rust 1.99.1"
    locator: Variables and Mutability
  - url: https://doc.rust-lang.org/1.99.1/book/ch08-02-strings.html#indexing-into-strings
    title: "The Rust Programming Language, Storing UTF-8 Encoded Text with Strings, Rust 1.99.1"
    locator: Indexing into Strings; Internal Representation
---

## Overview

Rust is a general-purpose language for systems programming. Its website states the aim as "A language empowering everyone to build reliable and efficient software." The site names three goals: performance, reliability and productivity. It says that Rust has no runtime and no garbage collector.[^1]

The Rust book describes the compiler as a gatekeeper. It refuses to compile code with subtle bugs, concurrency bugs among them.[^2] Rust tries to make safe code fast through zero-cost abstractions: higher-level features that compile to code as fast as code written by hand. The book states the aim to remove the old trade-offs between safety and productivity, and between speed and ergonomics.[^2]

The announcement of Rust 1.0 said that Rust combines low-level control over performance with high-level safety guarantees. It reaches these goals "without requiring a garbage collector or runtime".[^3]

## A first program

```rust run id=hello
fn main() {
    println!("Hello, world!");
}
```

```output for=hello
Hello, world!
```

The program below reads one shape per line, adds up the areas and collects the errors. It uses an `enum` with data, a function that returns `Result`, the `?` operator and nested patterns in a `match`. The format strings capture variables by name.

```rust run id=shapes
enum Shape {
    Circle(f64),
    Square(f64),
}

fn parse(line: &str) -> Result<Shape, String> {
    let (kind, size) = line.split_once(' ').ok_or("no size")?;
    let size: f64 = size.parse().map_err(|e| format!("{size:?}: {e}"))?;
    match kind {
        "circle" => Ok(Shape::Circle(size)),
        "square" => Ok(Shape::Square(size)),
        _ => Err(format!("unknown shape {kind:?}")),
    }
}

fn main() {
    let input = "circle 1.5\nsquare 2\nhexagon 3\nsquare two\ncircle";
    let mut total = 0.0;
    let mut errors = Vec::new();
    for line in input.lines() {
        match parse(line) {
            Ok(Shape::Circle(r)) => total += std::f64::consts::PI * r * r,
            Ok(Shape::Square(side)) => total += side * side,
            Err(e) => errors.push(e),
        }
    }
    println!("total area {total:.2}");
    println!("{} errors: {}", errors.len(), errors.join("; "));
}
```

```output for=shapes
total area 11.07
3 errors: unknown shape "hexagon"; "two": invalid float literal; no size
```

The last line, `circle`, has no space, so `split_once` gives `None` and `ok_or` makes the error `"no size"`. The `?` operator passes that `&str` through the `From` trait, which converts it to the `String` of the return type.[^4] The `match` must cover every variant of `Result` and `Shape`, or the program does not compile.

## The language at a glance

A source file has the extension `.rs`. GitHub Linguist also counts files with the extension `.rs.in` as Rust.[^5]

**Types.** Rust is statically typed: the compiler must know the type of each variable at compile time. It can usually infer the type from the value and its use.[^6] Each function signature must declare the type of each parameter.[^7] The inference of rustc is based on the Hindley-Milner algorithm, extended for subtyping, regions and higher-ranked types ([[hindley-milner]]).[^8] There is no implicit conversion between primitive types ([[type-conversion]]).[^9] An integer overflow panics in a debug build and wraps in a release build. The `checked_*` and `wrapping_*` methods make the choice explicit ([[integer-overflow]]).[^6]

```rust run id=types
fn main() {
    let names = ["Ada", "Grace"];
    println!("{:?} {:?}", names.first(), names.get(5));
    let n: u8 = 255;
    println!("{:?} {}", n.checked_add(1), n.wrapping_add(1));
    let wide = i64::from(n) + 1;
    println!("{wide}");
}
```

```output for=types
Some("Ada") None
None 0
256
```

A trait describes an abstract interface that types can implement ([[interfaces]]).[^10] The compiler turns generic code into specific code for each concrete type, by monomorphization ([[parametric-polymorphism]]).[^11] An `enum` is a nominal, heterogeneous disjoint union type ([[sum-types]]).[^12] Rust has no null. The `Option<T>` enum stands for a value that can be absent ([[null-references]]).[^13]

**Memory.** Rust manages memory through a system of ownership, with rules that the compiler checks. A program that breaks the rules does not compile, and the checks do not slow the program at run time.[^14] Each value has one owner, and an assignment can move the value to another owner ([[rust/ownership]]). Unsafe operations can violate the memory-safety guarantees of the static semantics. The safe subset of Rust cannot use them, for example the dereference of a raw pointer.[^15] The `unsafe` keyword starts a block that can use them, and the borrow checker still checks references inside it.[^16]

**Execution.** rustc is the compiler of the Rust project. It produces binary code, as a library or as an executable.[^17] It usually generates code through LLVM, and it also supports Cranelift and GCC as back ends.[^18] The Rust project builds official binary releases for each tier 1 target, and it tests them after each change.[^19]

**Paradigms.** Functional programming is one significant influence on the design. Closures and iterators come from it ([[closures]], [[iterators]]).[^20] Under the definition of the Gang of Four book, Rust is object-oriented: structs and enums have data and methods. Rust has no inheritance of structs, but default methods of a trait give a limited form of code reuse.[^21] Rust has no exceptions. A recoverable error is a `Result<T, E>` value, and the `panic!` macro stops the program on an unrecoverable error ([[exception-handling]]).[^22] The `?` operator returns an error value to the caller.[^23] The `Send` trait marks types whose ownership can move between threads, and `Sync` marks types that threads can share by reference ([[threads]]).[^24] Safe Rust guarantees an absence of data races, but it does not prevent general race conditions ([[data-races]]).[^25] The `async` and `await` syntax expresses asynchronous code, and third-party crates supply the runtimes ([[async-await]]).[^26]

## The guide

The guide to Rust has the 25 parts of the language template. Each row says what the part covers for Rust. Two pages exist so far, and the table links them.

| Part | What it covers in Rust |
|---|---|
| Overview and history | this page: the design goals, the versions, the implementations, the history |
| Getting started | installing with rustup, creating a package, `cargo run`, the release channels, the playground |
| Lexical structure and syntax | items, statements and expressions, blocks as expressions, keywords, literals, attributes, the grammar of the Reference |
| Values and types | integer and float types, `bool`, `char`, tuples, arrays, slices, references, the never type, type inference |
| Variables, binding and scope | `let`, `mut`, shadowing, `const` and `static`, patterns in `let`, block scope |
| Control flow | `if` and `match` as expressions, `loop`, `while`, `for`, labelled `break`, `if let`, `let else` |
| Functions | `fn`, methods and `impl`, closures and the `Fn` traits, generic functions, `impl Trait` |
| Built-in collections | `Vec`, `VecDeque`, `HashMap`, `BTreeMap`, `HashSet`, iterators and adapters |
| Strings and text | `String` and `&str` as UTF-8, `char`, byte and char iteration, `format!`, no indexing by position |
| User-defined types | structs, enums with data, traits, generics, associated types, derive |
| Errors and exceptions | `Result` and `Option`, the `?` operator, the `Error` trait, `panic!` and unwinding |
| Memory and resources | [[rust/ownership]], borrowing, lifetimes, `Drop`, `Box`, `Rc` and `Arc`, `unsafe` |
| Modules, packages and builds | modules and `use`, crates, Cargo packages and workspaces, `Cargo.toml`, editions, features |
| Concurrency and parallelism | `std::thread`, `Send` and `Sync`, `Mutex`, channels, atomics, `async` and `await` |
| Metaprogramming | `macro_rules!`, procedural macros, derive macros, attributes, `const fn` |
| The standard library | `core`, `alloc` and `std`, a map of the modules; the api cards hang here |
| I/O, files and the OS | `std::io`, `std::fs`, `std::path`, `std::process`, `std::env`, `std::net` |
| Testing | `#[test]`, `cargo test`, unit, integration and documentation tests, `assert!` macros |
| Tooling | rustup, Cargo, rustfmt, Clippy, rustdoc, Miri, rust-analyzer |
| Ecosystem | crates.io, docs.rs, the Rust Foundation, the Leadership Council and the RFC process, libraries by need |
| Performance | zero-cost abstractions, monomorphization, release profiles, benchmarks, profiling |
| Interoperability | `extern "C"`, the FFI, `#[repr(C)]`, calling Rust from C, WebAssembly |
| Idioms and style | the API guidelines, iterators over index loops, `Option` combinators, newtypes, builders |
| Pitfalls | use after a move ([[rust/errors/E0382]]), borrow conflicts, integer overflow in release builds, deadlocks |
| Versions and migration | the six-week releases, the editions 2015 to 2024, `cargo fix --edition` |

## Versions and support

Rust has a stable release every six weeks, with occasional point releases. The beta channel holds the next stable release, and nightly releases come every night.[^27] When a stable version comes out, the version before it reaches its end of life, so each version is supported for six weeks.[^28] The Rust project gives support and security updates only for the newest stable release and the newest beta and nightly releases.[^29] The pages of this guide state facts for Rust 1.99 and the 2024 edition. A fact that holds only from a later version gives that version.

| Version | Status | First release | Newest release | End of life |
|---|---|---|---|---|
| 1.100 | beta | 2026-11-12 (planned) | none | at the release of Rust 1.101 |
| 1.99 | supported | 2026-10-01 | 1.99.1 (2026-10-15) | at the release of Rust 1.100 |
| 1.98 | end of life | 2026-08-20 | 1.98.1 (2026-09-03) | 2026-10-01 |
| 1.97 | end of life | 2026-07-09 | 1.97.1 (2026-07-16) | 2026-08-20 |
| 1.96 | end of life | 2026-05-28 | 1.96.1 (2026-06-30) | 2026-07-09 |

The release dates come from the release notes of the compiler.[^30] The planned date of Rust 1.100 comes from the release page of the Rust Forge.[^31] The end-of-life dates follow from the policy and the dates of the later releases, and endoflife.date gives the same dates.[^32] Editions are opt-in per crate, and crates of different editions work together.[^33] Rust 2018 came with Rust 1.31.0, Rust 2021 with 1.56.0 and Rust 2024 with 1.85.0.[^34][^35][^36]

## Implementations

The Reference states that it is "the primary reference for the Rust programming language".[^37] It covers only stable Rust, and its rule identifiers are not stable between releases.[^37] In 2025 the Rust project adopted the FLS, a description of Rust that Ferrous Systems and AdaCore assembled. The project says that this does not change the status of the Reference.[^38] The table gives the newest release of each implementation on 2026-11-10.

| Implementation | Runs on | Rust version | Newest release | Licence |
|---|---|---|---|---|
| rustc | the tier 1 and tier 2 targets of the Rust project, among them Linux, macOS and Windows | 1.99, the editions 2015 to 2024 | 1.99.1 (2026-10-15) | MIT OR Apache-2.0 |
| Ferrocene | a downstream of rustc, qualified for safety-critical use on its qualified targets | 1.97 | 26.08.0 (2026-10-16) | MIT OR Apache-2.0 |
| gccrs | a front end of GCC; experimental and not yet able to compile the standard library | a subset; it targets `core` from Rust 1.49 | GCC 16.2 (2026-08-07) | GPL-3.0-or-later |
| mrustc | a C++ program that emits C; its aim is to bootstrap rustc | it bootstraps rustc 1.90.0 | 0.12.0 (tag, 2026-02-17) | MIT |

Sources: rustc[^19][^39], Ferrocene[^40][^41], gccrs[^42][^43][^44] and mrustc[^45]. The release notes of GCC 16 have no Rust entry, so the Rust level of gccrs is the one in the GCC 15 notes. The Cranelift and GCC back ends of rustc are code generators, not separate implementations.[^18] The compat tables of the feature pages have one column, for rustc, with the edition.

## Ecosystem

Cargo is the package manager. It downloads the dependencies of a package, compiles it, and uploads packages to crates.io, the package registry of the community.[^46] rustup installs Rust from the official release channels. It switches between the stable, beta and nightly compilers.[^47] docs.rs builds and hosts the documentation of each library published to crates.io.[^48]

| Need | Tool |
|---|---|
| install and update the toolchain | rustup[^47] |
| build, manage dependencies, publish | Cargo and crates.io[^46] |
| run tests | `cargo test`, for unit, integration and documentation tests[^49] |
| format | rustfmt[^50] |
| lint | Clippy, with over 800 lints[^51] |
| read the documentation of a crate | docs.rs[^48] |

The guide's Ecosystem part lists the main libraries by need, as library cards.

## History and influence

Graydon Hoare made the first commit of the public repository on 2010-06-16.[^52] Its licence file gives the copyright as 2006 to 2010 for Graydon Hoare and 2009 to 2010 for the Mozilla Foundation.[^53] The Rust Foundation calls Mozilla "the original home of the Rust project". Mozilla supported it from its start as a research project in 2010.[^54]

| Date | Event |
|---|---|
| 2010-06-16 | the first commit of the public repository[^52] |
| 2015-05-15 | Rust 1.0[^3] |
| 2018-12-06 | Rust 1.31 and the Rust 2018 edition[^55] |
| 2021-02-08 | the Rust Foundation, with AWS, Huawei, Google, Microsoft and Mozilla as founding members[^54] |
| 2023-06-20 | the Leadership Council starts, and the Core Team ends[^56] |
| 2025-02-20 | Rust 1.85 and the Rust 2024 edition[^57] |

The Leadership Council leads the project as the successor of the core team. Each top-level team sends a representative, and the council delegates most decisions to the teams.[^58] Its page lists seven members: Rémy Rakic, Manish Goregaokar, Jakub Beránek, Pete LeVasseur, Mara Bos, Mark Rousskov and Oli Scherer.[^59] Each major decision starts as a Request for Comments (RFC).[^60] The author opens a pull request on the RFC repository. The relevant team discusses it, and a member proposes a final comment period to merge, close or postpone it.[^61]

The Reference lists the influences. SML and OCaml gave algebraic data types, pattern matching and type inference. C++ gave references, RAII, smart pointers, move semantics and monomorphization. ML Kit and Cyclone gave region-based memory management. Haskell gave typeclasses. Newsqueak, Alef and Limbo gave channels, and Erlang gave message passing. Swift gave optional bindings, Scheme hygienic macros, C# attributes and Ruby the syntax of closures. NIL and Hermes gave typestate.[^62]

## Coming from another language

- An assignment or a call moves a value that does not implement `Copy`. A later use of the old variable is the error E0382. See [[rust/errors/E0382]] and [[rust/ownership]].
- Variables are immutable by default. A variable that changes needs `mut` ([[immutability]]).[^63]
- There is no null. A value that can be absent has the type `Option<T>`, and the compiler makes the code handle `None`.[^13]
- There are no exceptions. A function that can fail returns `Result<T, E>`, and `panic!` is for unrecoverable errors.[^22]
- Integer overflow panics in a debug build and wraps silently in a release build ([[integer-overflow]]).[^6]
- A `String` holds UTF-8 bytes, and Rust does not allow indexing into a string by position. A byte index does not always match a character ([[utf-8]]).[^64]

## References
