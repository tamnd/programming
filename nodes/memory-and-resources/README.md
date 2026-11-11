A program must get memory and other resources, such as files and sockets, and give them back. This area explains the ways that languages do this, from manual calls to garbage collection and ownership, and the bugs that come from each way.

## What it holds

- Allocation: how a program asks for memory, and the cost of each kind of allocation.
- Manual memory management: malloc and free, new and delete, and the rules that the programmer must follow.
- Garbage collection: garbage collection and reference counting from the view of the program that uses them.
- Ownership and borrowing: ownership rules that the compiler checks, as in Rust.
- RAII and deterministic destruction: tying a resource to the lifetime of an object.
- Smart pointers: pointer types that manage the lifetime of their target.
- Memory safety: what memory safety means, and which languages give it.
- Leaks: memory and resource leaks, and how to find them.
- Arenas and pools: allocating many objects together and freeing them together.
- Resource handles: files, sockets, locks and other handles that a program must close.

## What it does not hold

- How to implement a garbage collector. That is in [Language implementation](/language-implementation/).
- Memory-safety weaknesses as security issues, such as use after free as a CWE entry. They are in [Secure programming](/security/).
- The stack and the heap as a basic model. They are in [Foundations](/foundations/).
- Cleanup statements, such as finally, defer and with. They are in [Errors and control flow](/errors-and-control/).
