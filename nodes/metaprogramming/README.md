Metaprogramming is code that reads, changes or makes code. This area covers the forms that languages give for it, from reflection at run time to macros and evaluation at compile time.

## What it holds

- **Metaprogramming basics.** What metaprogramming is, its levels and times, and when to use it.
- **Reflection and introspection.** Looking at and changing types, fields and methods while the program runs.
- **Homoiconicity.** Code that is a data structure of its own language, and the quote forms that make code into data.
- **Macros.** Code that runs at compile time to make code: text macros, syntax macros, hygiene and procedural macros.
- **Templates and compile-time evaluation.** Running code in the compiler: constant expressions, compile-time functions and template metaprogramming.
- **Conditional compilation.** Code that the build includes or leaves out by platform, feature or configuration.
- **Code generation.** Programs that write source code for other programs, before or during the build.
- **Staging and multi-stage programming.** Programs that build and run specialized code in stages, for speed or for safety.
- **Annotations and attributes.** Metadata on declarations that the compiler, tools and the runtime read.
- **Decorators.** Functions that wrap functions, methods or classes, and the syntax for them.
- **Eval.** Running a string or a data value as code while the program runs.
- **Runtime code modification.** Changing or loading code while the program runs: hot reload, hooking, instrumentation and self-modifying code.
- **Embedded DSLs.** Small languages built inside a host language with its own syntax and types.

## What it does not hold

- Parsers and compilers as tools. They are in [Language implementation](/language-implementation/).
- Templates and generics as type system features. They are in [Types](/types/). This area holds only their use for computation at compile time.
- Code injection that comes from eval. It is in [Secure programming](/security/), under injection.
