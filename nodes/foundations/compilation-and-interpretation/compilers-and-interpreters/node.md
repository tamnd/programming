---
tag: BM299A
kind: concept
slug: compilers-and-interpreters
name: Compilers and interpreters
aliases: [compiler, interpreter, compilation, interpretation, ahead-of-time compilation, AOT, compile time, run time, virtual machine, transpiler]
area: foundations/compilation-and-interpretation
level: core
summary: >-
  A compiler translates a program into another language before it runs. An interpreter carries out a program as it reads it. Most systems mix both, with bytecode and JIT compilers.
applies_to: core-languages
viz: optional
formal: none
history:
  year: 1957
  people: [Grace Hopper, John Backus, John McCarthy]
  note: Hopper's A-0 translated symbolic code in 1952, the FORTRAN compiler of 1957 rivalled hand-written code, and the first Lisp interpreter followed in 1959.
see_also: [bytecode, tree-walking-interpreter, just-in-time-compilation, abstract-syntax-tree, constant-evaluation, recursive-descent-parsing, code-injection]
same_as:
  wikidata: Q47506
  wikipedia: Compiler
sources:
- url: https://doc.rust-lang.org/book/ch01-02-hello-world.html
  title: Hello, World!, The Rust Programming Language
  locator: Compilation and Execution
- url: https://v8.dev/blog/sparkplug
  title: Sparkplug, a non-optimizing JavaScript compiler, V8 blog
  locator: Introduction
- url: https://docs.python.org/3/glossary.html#term-bytecode
  title: Glossary, Python documentation
  locator: bytecode
- url: https://docs.oracle.com/javase/specs/jvms/se25/html/jvms-1.html
  title: Chapter 1, Introduction, The Java Virtual Machine Specification, Java SE 25 Edition
  locator: 1.2 The Java Virtual Machine
- url: https://learn.microsoft.com/en-us/dotnet/standard/managed-execution-process
  title: Managed execution process, .NET documentation, Microsoft Learn
  locator: Compile CIL to native code
- url: https://en.cppreference.com/cpp/language/constexpr
  title: constexpr specifier, cppreference.com
  locator: Explanation
---

## Idea

A program is text, and a machine runs instructions, so something must bridge the two. A compiler translates the whole program, before it runs, into another language, usually machine code or a lower-level code for a virtual machine. An interpreter reads the program and carries out what it says, step by step, while it runs. Most language systems today mix the two: CPython compiles source to bytecode and interprets the bytecode, the JVM and JavaScript engines interpret bytecode and compile the hot parts to machine code while the program runs, and Go, Rust, C and C++ compile to machine code ahead of time.

```python run id=idea
# The program (x * 2 + 1) * x as a tree: (op, left, right), a name or a number.
tree = ("*", ("+", ("*", "x", 2), 1), "x")


def interpret(node, env):  # walk the tree every time the program runs
    if isinstance(node, int):
        return node
    if isinstance(node, str):
        return env[node]
    op, a, b = node
    x, y = interpret(a, env), interpret(b, env)
    return x + y if op == "+" else x * y


def compile_tree(node, code):  # translate the tree once into instructions
    if isinstance(node, int):
        code.append(("push", node))
    elif isinstance(node, str):
        code.append(("load", node))
    else:
        op, a, b = node
        compile_tree(a, code)
        compile_tree(b, code)
        code.append(("add",) if op == "+" else ("mul",))
    return code


def run(code, env):  # a virtual machine: a loop over instructions
    stack = []
    for ins in code:
        if ins[0] == "push":
            stack.append(ins[1])
        elif ins[0] == "load":
            stack.append(env[ins[1]])
        else:
            b, a = stack.pop(), stack.pop()
            stack.append(a + b if ins[0] == "add" else a * b)
    return stack.pop()


code = compile_tree(tree, [])
print(code)
for x in (1, 5, 10):
    print(interpret(tree, {"x": x}), run(code, {"x": x}))
```

```output for=idea
[('load', 'x'), ('push', 2), ('mul',), ('push', 1), ('add',), ('load', 'x'), ('mul',)]
3 3
55 55
210 210
```

The program holds the formula `(x * 2 + 1) * x` as a tree. `interpret` walks the tree each time it runs, deciding at every node what kind of node it is. `compile_tree` walks the tree once and emits a flat list of instructions for a small stack machine, and `run` executes that list in a loop. Both give the same results for every `x`. The compiled form does the analysis of the tree once, so each run only fetches and executes instructions.

## How it works

**The compiler pipeline.** A compiler reads characters and groups them into tokens, parses the tokens into a syntax tree, checks names and types, translates the tree into an intermediate representation, improves that representation, and finally emits code for the target. The Rust book describes the result of compiling ahead of time: "Rust is an ahead-of-time compiled language, meaning you can compile a program and give the executable to someone else, and they can run it even without having Rust installed."[^1] Each phase can reject the program, so a compiler finds syntax and type errors before the program runs at all. See [abstract syntax trees](/abstract-syntax-tree/).

**Interpreters.** A tree-walking interpreter evaluates the syntax tree directly, as `interpret` does. It is simple to write and starts at once, but it repeats the work of inspecting each node every time the node runs. A bytecode interpreter first compiles the tree into compact instructions for a virtual machine, then runs a loop that fetches, decodes and executes them. The V8 team names the cost that remains: "interpreters have inherent overheads that we can’t get rid of; things like bytecode decoding overheads or dispatch overheads that are an intrinsic part of an interpreter’s functionality."[^2] See [tree-walking interpreters](/tree-walking-interpreter/).

**Bytecode and virtual machines.** Many languages compile to an intermediate code and run it on a virtual machine. The Python glossary states: "Python source code is compiled into bytecode, the internal representation of a Python program in the CPython interpreter. The bytecode is also cached in .pyc files so that executing the same file is faster the second time".[^3] The bytecode separates the language from the machine: "The Java Virtual Machine knows nothing of the Java programming language, only of a particular binary format, the class file format."[^4] Kotlin, Scala and Clojure compile to the same class files, and F# and C# compile to the same intermediate language for .NET. See [bytecode](/bytecode/).

**Just-in-time compilation.** A JIT compiler translates code to machine code while the program runs. The .NET documentation describes the step: "At execution time, a just-in-time (JIT) compiler translates the CIL into native code."[^5] Engines such as V8 and HotSpot run new code in an interpreter first, count how often each function runs, and compile the hot functions with optimizing compilers. The optimizer can use what it has seen, such as the types that actually reach a function, and must undo the compiled code, called deoptimization, when that assumption stops holding. See [just-in-time compilation](/just-in-time-compilation/).

**Compile time and run time.** Work that a compiler does happens once, before any user runs the program, and errors that it finds never reach a user. Modern compilers can run parts of the program itself: C++ `constexpr` and Rust `const fn` compute values during compilation. cppreference states: "The constexpr specifier declares that it is possible to evaluate the value of the entities at compile time."[^6] The cost moves from each run of the program to each build, and the rules for code that runs in the compiler are stricter, because it must be deterministic and free of undefined behavior.

**Languages and implementations.** Compiled and interpreted describe implementations, not languages. Python has CPython, a bytecode interpreter, and PyPy, a JIT compiler. C is almost always compiled, but C interpreters exist, and Cling runs C++ interactively. JavaScript was interpreted in its first engines and is JIT-compiled in all major engines today. What a language does fix is how much it lets a compiler know: static types, fixed layouts and no `eval` make good ahead-of-time code easier.

## In languages

| Language | Verdict | Mechanism |
|---|---|---|
| Python | native | CPython: bytecode compiler, `.pyc` cache and a virtual machine |
| JavaScript | native | bytecode interpreter and JIT tiers in the engine |
| TypeScript | native | compiles to JavaScript, which an engine runs |
| Java | native | `javac` to bytecode; the JVM interprets and JIT-compiles it |
| Kotlin | native | JVM bytecode, JavaScript or native code |
| C# | native | CIL in assemblies; JIT at run time or native AOT |
| Go | native | ahead of time to one executable |
| Rust | native | ahead of time through LLVM; `const fn` runs in the compiler |
| C | native | ahead of time to object files and a linker |
| C++ | native | ahead of time; templates and `constexpr` run in the compiler |
| Bash | native | an interpreter that reads and runs command lines |
| PowerShell | native | script blocks compiled to expression trees, interpreted then compiled |
| SQL | native | queries compiled to plans that the database runs |
| Regex | native | patterns compiled to automata or bytecode before matching |
| HTML | n/a | markup is parsed, not run |
| CSS | n/a | rules are parsed, not run |

The core languages fall into three groups. Go, Rust, C and C++ compile ahead of time to machine code, so a program starts at full speed and needs no runtime compiler, but each change needs a build. Java, C# and Kotlin compile ahead of time to bytecode and then JIT-compile it, which keeps one binary for many machines and lets the runtime optimize for the actual workload. Python and JavaScript compile source to bytecode when a program starts, and JavaScript engines add JIT tiers on top.

Each example file shows the language compiling or interpreting code at a boundary that a program can see: Python code objects, the JavaScript `Function` constructor, the Java compiler as a library, C# expression trees, a Go parser used for an interpreter, a bytecode loop in C, and evaluation inside the compiler in Rust and C++. The fails blocks show errors that each system finds before the program runs.

## Variations and related ideas

- **Transpilers.** A source-to-source compiler translates one high-level language to another, such as TypeScript to JavaScript or Nim to C.
- **Ahead-of-time compilation of managed languages.** GraalVM Native Image, .NET Native AOT and the Android runtime compile bytecode to machine code before the program runs, which cuts start-up time.
- **Tiered compilation.** HotSpot, V8 and .NET run code in an interpreter or a fast compiler first, and recompile hot code with a slower optimizing compiler.
- **Tracing JITs.** LuaJIT and PyPy record the operations of a hot loop as it runs and compile that trace.
- **Partial evaluation.** Specializing an interpreter for one program yields a compiled program, an idea known as the Futamura projections. Truffle and PyPy build on it.
- **Read-eval-print loops.** A REPL reads one expression, compiles or interprets it, and prints the result, which suits exploration. Lisp introduced it, and most languages now have one.
- **Compile-time execution.** C++ templates and `constexpr`, Rust `const fn` and macros, and Zig `comptime` run code inside the compiler. See [constant evaluation](/constant-evaluation/).
- **Language implementation.** The parts of a compiler or interpreter, from lexing and parsing to code generation. See [recursive descent parsing](/recursive-descent-parsing/).

## Pitfalls

- **Treating a language as compiled or interpreted.** Speed and behavior depend on the implementation. Measure with the implementation you deploy.
- **Errors found late.** An interpreter finds some errors only when it reaches the faulty line, and a JIT changes nothing about that. Run a type checker or linter, and keep tests that reach each path.
- **Warm-up.** A JIT-compiled program runs slowly until its hot code is compiled, so a benchmark that runs for one second can measure the interpreter. Warm up before measuring, and use a benchmark harness.
- **Stale compiled output.** A cached `.pyc` file, an old class file or a stale object file can make a program run old code. Let the build tool track dependencies, and clean the build when the results make no sense.
- **Running untrusted text as code.** `eval`, `exec` and the `Function` constructor compile any text, so input from a user becomes code. Parse data with a parser for data, not with the language's compiler. See [code injection](/code-injection/).
- **Long builds.** Heavy compile-time work, such as large templates and macro expansion, makes each build slow. Measure build time as you would run time.

## Cost

A tree-walking interpreter often runs 10 to 100 times slower than optimized machine code for the same work, and a bytecode interpreter is often 3 to 30 times slower, the gap coming from dispatch and from values that carry their types. A JIT compiler closes most of that gap for hot code, at the cost of warm-up time, memory for compiled code and a compiler inside the runtime. Ahead-of-time compilation moves all of that cost to the build: compiling a large C++ or Rust program can take minutes, but the result starts at full speed with no compiler at run time.

## History

Grace Hopper's A-0 system of 1952 translated symbolic code into machine code, and John Backus's team at IBM delivered the first FORTRAN compiler in 1957, which showed that compiled code could rival code written by hand. Steve Russell implemented John McCarthy's `eval` in 1958 to 1959 and so made the first Lisp interpreter. UCSD Pascal in the 1970s popularized p-code, a portable bytecode with an interpreter for each machine.

L. Peter Deutsch and Allan Schiffman compiled Smalltalk-80 bytecode to machine code at run time in 1984, and the Self project of the late 1980s and 1990s developed adaptive optimization and deoptimization. These ideas reached the mainstream in Sun's HotSpot JVM in 1999 and Google's V8 JavaScript engine in 2008.[^2] Chris Lattner began LLVM in 2000, and it became the back end of Clang, Rust and Swift. C++11 added `constexpr` in 2011, and later standards and Rust's `const fn` moved more of the program into the compiler.[^6]

## References
