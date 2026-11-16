---
tag: R3S50M
kind: concept
slug: opaque-type
name: Opaque type
aliases: [opaque data type, abstract type, opaque pointer, incomplete type, opaque return type, opaque type alias, impl Trait, some Protocol, pImpl]
area: types/forms-of-types
level: advanced
summary: >-
  An opaque type hides its representation from the code that uses it, so that code can work with the values only through the operations that the defining module gives.
applies_to: core-languages
viz: optional
formal: none
see_also: [encapsulation-and-visibility, interfaces, pointers-and-references, parametric-polymorphism, program-to-an-interface, deep-modules, module-pattern, composite-type]
same_as:
  wikidata: Q7095588
  wikipedia: Opaque_data_type
sources:
- url: https://en.wikipedia.org/wiki/Opaque_data_type
  title: Opaque data type, Wikipedia
  locator: Introduction; Uses in various languages
- url: https://en.cppreference.com/w/c/language/type.html
  title: Type, C reference, cppreference.com
  locator: Incomplete types
- url: https://ocaml.org/manual/5.3/moduleexamples.html
  title: The module system, The OCaml Manual 5.3
  locator: 2 Signatures
- url: https://doc.rust-lang.org/reference/types/impl-trait.html
  title: Impl trait, The Rust Reference
  locator: Abstract return types; Differences between generics and impl Trait in return position
- url: https://raw.githubusercontent.com/swiftlang/swift-book/main/TSPL.docc/LanguageGuide/OpaqueTypes.md
  title: Opaque and Boxed Protocol Types, The Swift Programming Language
  locator: Introduction; Returning an Opaque Type
- url: https://docs.scala-lang.org/scala3/book/types-opaque-types.html
  title: Opaque Types, Scala 3 Book
  locator: Introduction; Opaque Types; Summary of Opaque Types
- url: https://doc.rust-lang.org/reference/visibility-and-privacy.html
  title: Visibility and privacy, The Rust Reference
  locator: Default visibility
- url: https://learn.microsoft.com/en-us/dotnet/csharp/programming-guide/classes-and-structs/access-modifiers
  title: Access Modifiers, C# programming guide
  locator: Introduction
- url: https://go.dev/ref/spec
  title: The Go Programming Language Specification
  locator: Exported identifiers
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Classes/Private_elements
  title: Private elements, MDN Web Docs
  locator: Introduction
- url: https://docs.python.org/3.14/tutorial/classes.html
  title: Classes, The Python Tutorial 3.14
  locator: 9.6 Private Variables
- url: https://en.cppreference.com/w/cpp/language/pimpl.html
  title: PImpl, C++ reference, cppreference.com
  locator: Introduction; Explanation
---

## Idea

A module often needs a type whose inside it may change later, or whose values must obey a rule, such as "a temperature is never below absolute zero". If other code can see and build the representation, it can break the rule, and it can come to depend on details that the module wants to change. An opaque type solves this: the code outside the module knows that the type exists and knows its operations, but it cannot see what the type is made of. In Rust, a field is private to its module by default, so a struct with a private field is opaque to the code outside.

```rust run id=idea
mod temperature {
    pub struct Celsius(f64); // the field is private to this module

    impl Celsius {
        pub fn from_degrees(c: f64) -> Option<Celsius> {
            if c >= -273.15 { Some(Celsius(c)) } else { None }
        }

        pub fn fahrenheit(&self) -> f64 {
            self.0 * 9.0 / 5.0 + 32.0
        }
    }
}

use temperature::Celsius;

fn main() {
    let t = Celsius::from_degrees(100.0).unwrap();
    println!("{}", t.fahrenheit());
    println!("{}", Celsius::from_degrees(-300.0).is_none());
    println!("{}", std::mem::size_of::<Celsius>());
    // let bad = Celsius(-300.0); // error: the field is private
    // println!("{}", t.0);      // error: the field is private
}
```

```output for=idea
212
true
8
```

`main` is outside the module `temperature`, so it cannot build a `Celsius` from a number or read its field. It must call `from_degrees`, which rejects a value below absolute zero, and `fahrenheit`. The type still has the size of one `f64`, so the hiding costs nothing at run time.

## How it works

**The representation is not in the interface.** "In computer science, an opaque data type is a data type whose concrete data structure is not defined in an interface. This enforces information hiding, since its values can only be manipulated by calling subroutines that have access to the missing information"[^1]. "A data type whose representation is visible is called transparent"[^1]. The operations of the module form the interface of the type, so an opaque type is a common way to build an abstract data type[^1].

**Incomplete types in C.** C hides a struct by declaring it without its members. "An incomplete type is an object type that lacks sufficient information to determine the size of the objects of that type"[^2], and a structure type "of unknown content" is one kind[^2]. "The only thing that the client can do with an object of such a type is to take its memory address, to produce an opaque pointer"[^1]. The standard library works this way: its file functions take and return pointers to `FILE`, and "the concrete implementation of the type FILE is not specified"[^1].

**Abstract types in ML modules.** In OCaml, a signature lists the parts of a module that other code may use. "It can be used to hide some components of a structure (e.g. local function definitions) or export some components with a restricted type"[^3]. A signature that declares `type 'a queue` without a definition "makes the queue type abstract (by not providing its actual representation as a concrete type)"[^3], and "This ensures that users of the Fifo module cannot violate data structure invariants that operations rely on"[^3].

**Opaque return types.** Rust and Swift let a function hide the concrete type of the value it returns and show only a trait or a protocol. In Rust, "Functions can use impl Trait to return an abstract return type. These types stand in for another concrete type where the caller may only use the methods declared by the specified Trait"[^4]. In Swift, "A function or method that returns an opaque type hides its return value's type information"[^5]. Swift describes this as "the reverse of a generic type"[^5]: with a [type parameter](/parametric-polymorphism/) the caller chooses the type, but with `impl Trait` "the function chooses the return type, and the caller cannot choose the return type"[^4].

**Opaque type aliases.** Scala 3 can declare a type alias that is a plain alias inside its scope and an abstract type outside it. For `opaque type Logarithm = Double`, "The fact that Logarithm is the same as Double is only known in the scope where Logarithm is defined"[^6]. Outside that scope, "To users of Logarithm it is not possible to discover that Logarithm is actually implemented as a Double"[^6]. "Opaque type aliases provide type abstraction without any overhead"[^6].

**Access control.** In languages with classes, a private field gives a partly opaque type. "Some languages allow partially opaque types, e.g. a record which has some public fields, known and accessible to all clients, and some hidden fields which are not revealed in the interface. Such types play a fundamental role in object-oriented programming"[^1]. In Rust, "By default, everything is private"[^7], with a few exceptions. In C#, a `private` member is one that "Only code declared in the same class or struct can access"[^8]. In Go, "An identifier may be exported to permit access to it from another package"[^9], and only an identifier whose first letter is uppercase is exported[^9]. In JavaScript, private class elements "cannot be legally referenced outside of the class", and "The privacy encapsulation of these class elements is enforced by JavaScript itself"[^10]. See [encapsulation and visibility](/encapsulation-and-visibility/).

## In languages

| Language | Verdict | Mechanism |
|---|---|---|
| Python | idiom | a leading underscore marks a name as not public; nothing stops access |
| JavaScript | idiom | private class elements with `#`, or values that a closure keeps |
| TypeScript | idiom | `#` and `private` members; an exported interface without the class |
| Java | idiom | `private` fields; a public interface with a class that the package does not export |
| Kotlin | idiom | `private` and `internal` members; an exported interface without the class |
| C# | idiom | `private` and `internal` members; a public interface with an internal class |
| Go | idiom | identifiers that start with a lowercase letter are not exported from the package |
| Rust | native | items are private to their module by default; `impl Trait` hides a return type |
| C | native | a struct declared without its members is an incomplete type; code uses pointers to it |
| C++ | native | incomplete class types behind pointers, as in the pImpl technique; `private` members |
| Bash | n/a | Bash has no user-defined types |
| PowerShell | idiom | .NET access control, as in C# |
| SQL | n/a | a query sees the columns of the tables it can read; there are no user types with hidden parts |
| Regex | n/a | a pattern has no types |
| HTML | n/a | HTML has no types |
| CSS | n/a | CSS has no types in this sense |

C, C++ and Rust can hide a type itself, so code outside the module does not know the size or the fields. Java, C#, Kotlin and Go hide the fields of a type, or let a factory return an interface so that the class stays unnamed. Python and JavaScript have no static types, so they hide data by convention, by private class elements, or in closures.

The example files show these differences. Python shows that a name with two underscores is renamed but can still be read. JavaScript shows a private `#` field and a value that a closure hides. Java shows a factory that returns an interface while the class stays private. C# shows an interface with a private nested class. Go shows a constructor that returns an interface. Rust shows `impl Trait` in return position. C shows an incomplete struct that the code uses only through pointers. C++ shows the pImpl technique.

## Variations and related ideas

- **Opaque pointers.** "An opaque pointer is a special case of an opaque data type, a datatype that is declared to be a pointer to a record or data structure of some unspecified data type"[^1]. Handles from an operating system are often opaque, for example the POSIX thread types[^1]. "In some languages, such as Java, the only kind of opaque type provided is the opaque pointer"[^1].
- **Opaque types versus boxed types.** Swift contrasts opaque types with boxed protocol types: "Opaque types preserve type identity"[^5]: "the compiler has access to the type information, but clients of the module don't"[^5]. A boxed type, like a Rust trait object or a Java interface value, can hold values of different concrete types, and the code finds the type only when it runs[^5].
- **Opaque types and abstract data types.** "Opaque data types are frequently used to implement abstract data types"[^1]. The type and its operations form the abstract data type, and the hidden representation lets the module change it.
- **Friend modules.** "The information which is missing in the interface may be declared in its implementation"[^1], or in a second interface that only some modules can see[^1]. Rust `pub(crate)` and C# `internal` share hidden parts among the modules of one crate or one assembly[^7][^8].

## Pitfalls

- **Privacy by convention.** The Python tutorial says that private "instance variables that cannot be accessed except from inside an object" do not exist in Python[^11]. A name with a leading underscore "should be treated as a non-public part of the API"[^11], and name mangling is "designed mostly to avoid accidents; it still is possible to access or modify a variable that is considered private"[^11]. Do not use it for security.
- **Exposed fields in a header.** A C++ class with `private` members still puts those members in its header, and "any change to those implementation details requires recompilation of all users of the class"[^12]. Use the pImpl technique, or a C-style incomplete type, when the layout must stay out of the header.
- **No values on the stack.** In C, code outside the module cannot declare a variable of an incomplete struct type, because the size is unknown[^2]. The module must allocate each value and give a destroy function, so the user must remember to call it. The C example shows this.
- **One concrete type per function.** For `impl Trait` in return position, "Each possible return value from the function must resolve to the same concrete type"[^4]. To return different types, return a trait object such as `Box<dyn Trait>`.
- **Zero-cost and checked are different.** A Scala opaque alias or a Rust tuple struct has no cost at run time, but the check that a value obeys the rule happens only in the constructor. A module that gives a public field, or a function that converts from the raw type without a check, removes the protection.

## Cost

An opaque type that the compiler sees through, such as a Scala opaque alias, a Rust private field or an `impl Trait` return, costs nothing at run time[^6][^4]. "impl Trait in return position allows a function to return an unboxed abstract type"[^4]. An opaque pointer in C, or the pImpl technique in C++, costs an allocation for each value and an indirection for each access. In return, "changes to the implementation do not cause recompilation"[^12], and a library can change the representation without breaking code that it already linked with.

## History

The C standard library uses an opaque `FILE` type for file streams, and the POSIX threads interface uses opaque types for threads, mutexes and condition variables[^1]. OCaml hides types with module signatures[^3]. Rust has `impl Trait`[^4], Swift has opaque return types with `some`[^5], and Scala 3 has opaque type aliases[^6].

## References
