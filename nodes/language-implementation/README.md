This area explains how languages are built and run. It covers each part of a compiler or an interpreter, the runtime that supports a running program, and the tools that work on code as a language server does.

## What it holds

- Lexing and parsing: turning text into tokens and tokens into a tree, as done in real compilers.
- ASTs and IRs: abstract syntax trees and the intermediate representations of compilers.
- Interpreters: tree-walking and bytecode interpreters.
- Compilers: the front end, the middle end and the back end of a compiler.
- JIT compilation: compiling code while the program runs.
- Virtual machines: bytecode virtual machines, such as the JVM and the CPython VM.
- Runtime systems: the support code that runs with a program, such as schedulers and type information.
- Linking and loading: how object files become a program and how the program gets into memory.
- ABIs and calling conventions: the binary rules for calls, data layout and symbol names.
- Garbage collector implementation: how tracing and reference counting collectors work inside.
- WebAssembly as a target: compiling a language to WebAssembly, and the limits of the format.
- Language servers: the Language Server Protocol and the tools built on it.

## What it does not hold

- Parsing algorithms and their analysis, such as LR parsing tables as algorithms. They are on [the algorithm site](https://algorithm.tamnd.com/).
- The use of garbage collection from a program. It is in [Memory and resources](/memory-and-resources/).
- Linkers and object files from the view of a systems programmer. They are in [Systems programming](/systems-programming/). This area holds how a linker works inside.
- WebAssembly in the browser. It is in [Web development](/web-development/).
