---
tag: 37GYNK
kind: concept
slug: ownership
name: Ownership and borrowing
aliases: [ownership, borrowing, borrow checker, move semantics, unique ownership, single owner, owning pointer, non-owning reference]
area: memory-and-resources/ownership-and-borrowing
level: core
summary: >-
  Ownership makes one part of a program responsible for releasing each value, and other code borrows the value for a limited time. Rust checks these rules at compile time.
applies_to: core-languages
viz: optional
formal: none
history:
  year: 1990
  people: [Philip Wadler]
  note: Wadler proposed linear types, whose values must be used exactly once, so that they need no reference counting or garbage collection.
see_also: [resource-acquisition-is-initialization, explicit-memory-management, reference-counting, garbage-collection, data-races]
sources:
- url: https://doc.rust-lang.org/1.99.1/book/ch04-01-what-is-ownership.html#ownership-rules
  title: The Rust Programming Language, 4.1 What Is Ownership?, Rust 1.99.1
  locator: Ownership Rules
- url: https://en.cppreference.com/w/cpp/memory/unique_ptr
  title: std::unique_ptr, cppreference.com
  locator: description
- url: https://en.cppreference.com/w/cpp/memory/unique_ptr/unique_ptr
  title: std::unique_ptr<T,Deleter>::unique_ptr, cppreference.com
  locator: overload 5
- url: https://developer.mozilla.org/en-US/docs/Web/API/Web_Workers_API/Transferable_objects
  title: Transferable objects, MDN Web Docs
  locator: introduction
- url: https://doc.rust-lang.org/1.99.1/book/ch04-02-references-and-borrowing.html
  title: The Rust Programming Language, 4.2 References and Borrowing, Rust 1.99.1
  locator: The Rules of References
- url: https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines
  title: C++ Core Guidelines, B. Stroustrup and H. Sutter, editors
  locator: R.3 and R.4
- url: https://learn.microsoft.com/en-us/dotnet/csharp/language-reference/builtin-types/ref-struct
  title: ref structure types, C# language reference, Microsoft Learn
  locator: introduction
- url: https://go.dev/blog/slices-intro
  title: 'Go Slices: usage and internals, The Go Blog'
  locator: Slice internals
- url: https://www.cs.umd.edu/projects/cyclone/papers/cyclone-regions.pdf
  title: Region-Based Memory Management in Cyclone, D. Grossman, G. Morrisett, T. Jim, M. Hicks, Y. Wang and J. Cheney, PLDI 2002
  locator: Abstract
- url: https://github.com/swiftlang/swift-evolution/blob/main/proposals/0377-parameter-ownership-modifiers.md
  title: SE-0377, borrowing and consuming parameter ownership modifiers, Swift Evolution
  locator: Introduction
- url: https://homepages.inf.ed.ac.uk/wadler/topics/linear-logic.html
  title: Linear types can change the world!, P. Wadler, Programming Concepts and Methods, North Holland, 1990
  locator: abstract
---

## Idea

Every piece of memory, every open file and every lock must be released one time, at the right time. Ownership is the rule that says which part of a program is responsible for that release. The owner of a value frees it. Other code can borrow the value, which means that it uses the value for a limited time and does not free it. If two parts of a program both think that they own a value, it is freed twice. If no part owns it, it is never freed. If a borrower keeps the value after the owner frees it, the borrower reads freed memory. Even in a language with a garbage collector, a program needs to know who may change a value. If code keeps a reference to a list that it does not own, a change by the real owner can surprise it:

```python run id=idea
class Playlist:
    def __init__(self, songs):
        self.songs = songs


class OwnedPlaylist:
    def __init__(self, songs):
        self.songs = list(songs)


songs = ["intro", "theme"]
shared = Playlist(songs)
owned = OwnedPlaylist(songs)
songs.clear()
print(shared.songs)
print(owned.songs)
```

```output for=idea
[]
['intro', 'theme']
```

`Playlist` borrows the list of the caller and keeps the borrow. `OwnedPlaylist` makes its own copy and owns it. Most languages leave these rules to documentation and conventions. C++ puts ownership into library types. Rust checks ownership and borrowing in the compiler.

## How it works

**One owner.** The Rust book gives three rules: "Each value in Rust has an owner. There can only be one owner at a time. When the owner goes out of scope, the value will be dropped."[^1] With one owner, the time of the release is known from the program text. It is the end of the scope of the owner, as in [RAII](/resource-acquisition-is-initialization/). No garbage collector and no reference count is necessary.

**Moves.** An owner can give its value to a new owner. In Rust, assignment and a call by value move the value, and the compiler rejects any later use of the old name. In C++, `std::unique_ptr` "owns (is responsible for) and manages another object via a pointer and subsequently disposes of that object when the unique_ptr goes out of scope".[^2] It can be moved but not copied, and a move "transferring ownership from u to *this ... stores the null pointer in u."[^3] The move is the transfer of the duty to free. JavaScript has the same idea for a few resource types: "Transferable objects are objects that own resources that can be transferred from one context to another, ensuring that the resources are only available in one context at a time. Following a transfer, the original object is no longer usable".[^4]

**Borrows.** Code that only needs a value for a short time gets a reference and not the value. The Rust book states: "We call the action of creating a reference borrowing. As in real life, if a person owns something, you can borrow it from them. When you're done, you have to give it back."[^5] In C++, the Core Guidelines give the same meaning to plain pointers and references: "R.3: A raw pointer (a T*) is non-owning" and "R.4: A raw reference (a T&) is non-owning".[^6] A borrower must not free the value, and it must not use the value after the owner frees it.

**Aliasing rules.** Rust adds a second rule for borrows: "At any given time, you can have either one mutable reference or any number of immutable references. References must always be valid."[^5] Many readers or one writer is the same rule as a reader-writer lock, and the compiler checks it before the program runs. The book states that the benefit of this restriction is that "Rust can prevent data races at compile time."[^5] The rule also prevents a common single-threaded bug: a reference into a vector that becomes invalid when the vector grows and moves its elements.

**Lifetimes.** To check that a borrow does not outlive its owner, the compiler gives each reference a lifetime, which is the region of the program where the reference is used. A function signature can name lifetimes to say, for example, that the result borrows from the first argument. Most lifetimes are inferred.

## In languages

| Language | Verdict | Mechanism |
|---|---|---|
| Python | absent | garbage collection, `weakref`, `with` blocks |
| JavaScript | absent | garbage collection, transferable objects |
| TypeScript | absent | as JavaScript, `readonly` types |
| Java | absent | garbage collection, defensive copies |
| Kotlin | absent | garbage collection, `use` |
| C# | absent | garbage collection, `Span<T>` and `ref struct` |
| Go | absent | garbage collection, `slices.Clone` |
| Rust | native | moves, `&` and `&mut` borrows, the borrow checker |
| C | absent | documented conventions, `const` pointers |
| C++ | stdlib | `std::unique_ptr`, move semantics, non-owning raw pointers |
| Bash | n/a | no references to memory |
| PowerShell | absent | garbage collection, `Dispose` |
| SQL | n/a | the database engine owns storage |
| Regex, HTML, CSS | n/a | no memory or references |

**Rust** is the main language with checked ownership. Moves, borrows and lifetimes are part of the type system, and a program that breaks the rules does not compile.[^1]

**C++** has ownership in the library. `std::unique_ptr` is the single owner, `std::shared_ptr` is shared ownership with a reference count, and plain pointers, references and `std::span` are borrows by convention.[^6] The compiler does not check that a borrow lives shorter than its owner.

**C** has no ownership in the language. Each API documents which pointers the caller must free.

**Python**, **JavaScript**, **TypeScript**, **Java**, **Kotlin**, **Go** and **PowerShell** use a garbage collector, so no reference owns memory. Ownership still matters for resources such as files and sockets, and for mutable data that several parts of a program share. These languages use copies, immutable values and blocks such as `with` and `try`-with-resources.

**C#** also uses a garbage collector, but `Span<T>` gives a checked form of borrowing. A span is a `ref struct`, and instances of such a type "can't escape to the managed heap".[^7]

**Go** slices are views of a shared array. "Slicing does not copy the slice's data. It creates a new slice value that points to the original array."[^8] A function that keeps a slice must copy it if the caller can change the array.

## Variations and related ideas

**Shared ownership.** A value can have many owners and be freed when the last owner goes. `std::shared_ptr` in C++, `Rc` and `Arc` in Rust, and Swift classes use [reference counting](/reference-counting/) for this.

**Interior mutability.** Rust types such as `RefCell` and `Mutex` check the aliasing rule at run time, for data structures that the compile-time rule rejects.

**Linear and affine types.** A linear value must be used exactly once, and an affine value at most once. Rust moves are affine: a program can drop a value without using it. Linear types are the theory under ownership.

**Regions.** A region is a block of memory that holds many objects and is freed as a whole. Region systems check that no pointer into a region outlives the region.[^9]

**Ownership modifiers.** Swift adds `borrowing` and `consuming` parameter modifiers, which let a function state if it borrows or takes ownership of an argument.[^10]

**Garbage collection.** A [garbage collector](/garbage-collection/) removes the need to decide which reference owns memory. It does not remove the need to manage other resources and shared mutable state.

## Pitfalls

**Use after move.** In C++, a moved-from object still exists and can be used. A moved-from `std::unique_ptr` is null, and other types are left in a valid but unspecified state. Rust rejects any use after a move at compile time.

**Two owners.** If two owning objects get the same raw pointer, both free it. In C++, make the owner at the point of allocation with `std::make_unique`, and never pass a raw pointer to a second owner.

**A borrow that outlives the owner.** A pointer, reference, iterator or view that the owner invalidates becomes dangling. Common causes are a reference into a vector that grows, a `std::string_view` of a temporary string, and a pointer to a local variable that a function returns. Rust rejects these programs. C and C++ compile them, and the result is [undefined behavior](/undefined-behavior/).

**A hidden shared view.** In garbage-collected languages, a stored reference to a list, an array or a slice is a shared view, and not a copy. If the caller later changes the data, the stored view changes too. Copy the data at the boundary if the code must own it.

**Fights with the borrow checker.** Some correct programs break the Rust rules, for example a graph with parent and child links, or two mutable references into different parts of one structure. The usual answers are indices into a vector, `Rc` with `RefCell`, methods such as `split_at_mut`, or `unsafe` code behind a safe interface.

**Cycles with shared ownership.** Two values that own each other through reference counts are never freed. One of the links must be a weak reference.

## Cost

Single ownership costs nothing at run time. A move copies the bytes of a handle, such as a pointer and a length, and not the data that it points to. A borrow is a plain pointer. The release happens at a known point, so there is no garbage collector pause and no reference count update. The cost is at compile time and for the programmer: some designs need more thought, and code that shares data freely must use shared ownership, which costs a count update for each copy of the handle.

## History

Philip Wadler proposed linear types for functional languages in 1990: "Values belonging to a linear type must be used exactly once: like the world, they cannot be duplicated or destroyed. Such values require no reference counting or garbage collection, and safely admit destructive array update."[^11] Ownership types for object-oriented languages, which control which objects may refer to the internal objects of another object, came from the work of David Clarke, John Potter and James Noble in 1998. Cyclone, a safe dialect of C, used regions and static checks of pointer lifetimes in 2002.[^9] C++ had ownership conventions for a long time, and C++11 added move semantics and `std::unique_ptr`. Rust combined unique ownership, borrowing and lifetimes into one checked system, and Rust 1.0 was released in 2015. Swift, Mojo and other languages have since added ownership features of their own.[^10]

## References
