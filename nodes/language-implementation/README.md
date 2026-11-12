This area explains how languages are built and run. It covers each part of a compiler or an interpreter, the runtime that supports a running program, and the tools that work on code as a language server does.

## What it holds

- **Implementation strategies.** The ways to run a language: interpret it, compile it ahead of time, compile it to another language, or mix the methods. The pages give the words that the other sub-areas use.
- **Lexing and parsing.** How real compilers turn text into tokens and tokens into trees. The theory of grammars is in syntax-and-semantics, and the analysis of parsing algorithms is on algorithm.tamnd.com.
- **Semantic analysis.** The passes between the parser and the IR: names, types, desugaring and diagnostics.
- **ASTs and IRs.** The trees and the intermediate forms that a compiler builds and changes.
- **Interpreters.** Programs that run other programs directly: tree-walking and bytecode interpreters, and the ideas behind them.
- **Virtual machines.** Language virtual machines: their instruction sets, their verification and loading, and the main machines in use.
- **Compilers.** The structure of a compiler as a program: its phases, its driver, and how it compiles programs of many files.
- **Optimization passes.** How each optimizing transformation works inside the compiler. The view of the programmer is in performance (compiler-optimizations).
- **Native code generation.** The back end: from a low-level IR to machine code for a real processor.
- **Compiling language features.** How compilers implement closures, generics, objects, pattern matching, exceptions, coroutines and laziness.
- **JIT compilation.** Compiling code while the program runs, with feedback from the running program.
- **Runtime systems.** The support code that runs with every program: startup, object layout, type information, stacks and schedulers.
- **Garbage collector implementation.** How collectors and allocators work inside. The use of GC from a program is in memory-and-resources.
- **Linking and loading.** How object files become a program, and how the program and its libraries get into memory. The systems view is in systems-programming.
- **ABIs and calling conventions.** The binary rules that let separately compiled code work together: calls, data layout, symbol names and foreign calls.
- **WebAssembly as a target.** WebAssembly as a compilation target: its module format, its memory model, and the ways languages compile to it. Wasm in the browser is in web-development.
- **Language servers.** Tools that understand code for editors: the Language Server Protocol, the servers, and the parsing and analysis that they need.
- **Compiler infrastructure.** Frameworks and generators that people use to build compilers and interpreters.
- **Language implementations.** The main compilers, interpreters and runtimes of the tier 1 languages, as tool pages. Each page covers the architecture of the implementation, not only its command line.
- **Compiler correctness.** How compilers are tested and verified, and what happens when they are wrong.

## What it does not hold

- Parsing algorithms and their analysis, such as LR parsing tables as algorithms. They are on [the algorithm site](https://algorithm.tamnd.com/).
- The use of garbage collection from a program. It is in [Memory and resources](/memory-and-resources/).
- Linkers and object files from the view of a systems programmer. They are in [Systems programming](/systems-programming/). This area holds how a linker works inside.
- WebAssembly in the browser. It is in [Web development](/web-development/).
