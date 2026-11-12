A program must get memory and other resources, such as files and sockets, and give them back. This area explains the ways that languages do this, from manual calls to garbage collection and ownership, and the bugs that come from each way.

## What it holds

- **Storage and lifetime.** The kinds of storage a program has and how long each object lives in it. These words come before allocation, ownership and safety.
- **Allocation.** How a program asks for memory, the cost of each kind of allocation, and what happens when there is no memory.
- **Pointers and addresses.** Pointers from the view of memory: addresses, arithmetic, aliasing and the rules that compilers rely on.
- **Memory layout.** How values are laid out in memory: size, alignment, padding and the order of fields.
- **Manual memory management.** Asking for memory and giving it back by hand, and the rules the programmer must keep.
- **Garbage collection.** Garbage collection and reference counting from the view of the program that uses them. The inside of collectors is in language-implementation.
- **Ownership and borrowing.** Ownership rules that the compiler checks, as in Rust, and the ideas behind them.
- **Move and copy semantics.** What happens to a value when it is assigned or passed: a copy, a move or a shared reference.
- **RAII and deterministic destruction.** Tying a resource to the lifetime of an object so that the release happens on every path.
- **Smart pointers.** Pointer types that manage the lifetime of their target.
- **Memory safety.** What memory safety means, how languages give it, and the escape hatches.
- **Leaks.** Memory and resource leaks, in manual and in managed languages, and how to find them.
- **Arenas and pools.** Allocating many objects together and freeing them together, and reusing objects instead of allocating them.
- **Resource handles.** Files, sockets, locks and other handles that a program must close, and the rules for them.

## What it does not hold

- How to implement a garbage collector. That is in [Language implementation](/language-implementation/).
- Memory-safety weaknesses as security issues, such as use after free as a CWE entry. They are in [Secure programming](/security/).
- The stack and the heap as a basic model. They are in [Foundations](/foundations/).
- Cleanup statements, such as finally, defer and with. They are in [Errors and control flow](/errors-and-control/).
