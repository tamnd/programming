Metaprogramming is code that reads, changes or makes code. This area covers the forms that languages give for it, from reflection at run time to macros and evaluation at compile time.

## What it holds

- Reflection and introspection: inspecting and changing types, fields and methods at run time.
- Macros: textual, hygienic and procedural macros.
- Templates and compile-time evaluation: running code in the compiler, such as constexpr and comptime.
- Code generation: programs that write source code for other programs to compile.
- Annotations and attributes: metadata on declarations that tools and the runtime can read.
- Decorators: functions that wrap other functions or classes.
- Eval: running a string as code, and the risks of it.
- Homoiconicity: languages whose code is a data structure of the language.
- Embedded DSLs: small languages built inside a host language.

## What it does not hold

- Parsers and compilers as tools. They are in [Language implementation](/language-implementation/).
- Templates and generics as type system features. They are in [Types](/types/). This area holds only their use for computation at compile time.
- Code injection that comes from eval. It is in [Secure programming](/security/), under injection.
