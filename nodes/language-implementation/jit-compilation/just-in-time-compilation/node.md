---
tag: S005MC
kind: concept
slug: just-in-time-compilation
name: Just-in-time compilation
aliases: [JIT, JIT compilation, dynamic compilation, adaptive optimization, tiered compilation]
area: language-implementation/jit-compilation
level: core
summary: >-
  Just-in-time compilation translates code after the program starts. A runtime interprets first, counts calls, and compiles hot code for the types it saw, with guards that fall back.
applies_to: core-languages
viz: optional
formal: none
history:
  year: 1960
  people: [John McCarthy]
  note: Aycock names McCarthy's LISP paper as probably the earliest published work on JIT compilation, because it describes compiling functions into machine language fast enough that the output need not be saved.
see_also: [bytecode, tree-walking-interpreter, abstract-syntax-tree, closures, sum-types, garbage-collection]
same_as:
  wikidata: Q110546
  wikipedia: Just-in-time_compilation
sources:
  - paper: doi:10.1145/857076.857077
    title: John Aycock, A Brief History of Just-In-Time, ACM Computing Surveys 35(2), June 2003, pages 97 to 113
    locator: section 1, Introduction, pages 97 to 98
  - paper: doi:10.1145/857076.857077
    title: John Aycock, A Brief History of Just-In-Time, ACM Computing Surveys 35(2), June 2003
    locator: section 2.4, Mixed Code, Throw-Away Code, and BASIC, pages 99 to 100
  - paper: doi:10.1145/857076.857077
    title: John Aycock, A Brief History of Just-In-Time, ACM Computing Surveys 35(2), June 2003
    locator: section 2.5, FORTRAN, pages 100 to 101
  - paper: doi:10.1145/857076.857077
    title: John Aycock, A Brief History of Just-In-Time, ACM Computing Surveys 35(2), June 2003
    locator: section 2.7, Self, pages 101 to 103
  - paper: doi:10.1145/857076.857077
    title: John Aycock, A Brief History of Just-In-Time, ACM Computing Surveys 35(2), June 2003
    locator: section 2.14, Java, pages 107 to 108
  - paper: doi:10.1145/857076.857077
    title: John Aycock, A Brief History of Just-In-Time, ACM Computing Surveys 35(2), June 2003
    locator: section 3, Classification of JIT Systems, page 108
  - paper: doi:10.1145/857076.857077
    title: John Aycock, A Brief History of Just-In-Time, ACM Computing Surveys 35(2), June 2003
    locator: section 2.1, Genesis, page 98; section 2.6, Smalltalk, page 101
  - url: https://docs.oracle.com/en/java/javase/25/vm/java-hotspot-virtual-machine-performance-enhancements.html#GUID-85BA7DE7-4AF9-47D9-BFCF-379230C66412
    title: Java Virtual Machine Guide, Java SE 25, Java HotSpot Virtual Machine Performance Enhancements
    locator: Tiered Compilation
  - url: https://docs.oracle.com/en/java/javase/25/docs/specs/man/java.html
    title: The java Command, Java SE 25 tool specifications
    locator: Extra Options for Java, -Xint and -Xmixed
  - url: https://learn.microsoft.com/en-us/dotnet/core/runtime-config/compilation#tiered-compilation
    title: Microsoft Learn, Runtime configuration options for compilation
    locator: Tiered compilation; Quick JIT
  - url: https://v8.dev/blog/ignition-interpreter
    title: Ross McIlroy, Firing up the Ignition interpreter, V8 blog, 23 August 2016
    locator: introduction
  - url: https://v8.dev/blog/launching-ignition-and-turbofan
    title: Launching Ignition and TurboFan, V8 blog, 15 May 2017
    locator: A long journey
  - url: https://v8.dev/blog/sparkplug
    title: Leszek Swirski, Sparkplug, a non-optimizing JavaScript compiler, V8 blog, 27 May 2021
    locator: introduction; A fast compiler; Interpreter-compatible frames
  - url: https://v8.dev/blog/maglev#background
    title: Maglev, V8's Fastest Optimizing JIT, V8 blog, 5 December 2023
    locator: Background
  - url: https://v8.dev/blog/maglev#deoptimization
    title: Maglev, V8's Fastest Optimizing JIT, V8 blog, 5 December 2023
    locator: Deoptimization
  - paper: doi:10.1145/143095.143114
    title: Urs Hölzle, Craig Chambers and David Ungar, Debugging Optimized Code with Dynamic Deoptimization, ACM SIGPLAN '92 Conference on Programming Language Design and Implementation, June 1992
    locator: Abstract; section 3, Deoptimization
  - paper: doi:10.1145/3133876
    title: Edd Barrett, Carl Friedrich Bolz-Tereick, Rebecca Killick, Sarah Mount and Laurence Tratt, Virtual Machine Warmup Blows Hot and Cold, Proceedings of the ACM on Programming Languages 1 (OOPSLA), 2017
    locator: Abstract; section 1, Introduction
  - url: https://peps.python.org/pep-0659/
    title: Mark Shannon, PEP 659, Specializing Adaptive Interpreter
    locator: Abstract; Motivation; Implementation, Overview and Specialization
  - url: https://peps.python.org/pep-0744/
    title: Brandt Bucher and Savannah Ostrowski, PEP 744, JIT Compilation
    locator: Abstract; Motivation; Specification
  - url: https://docs.python.org/3.14/library/dis.html#dis.get_instructions
    title: The Python Standard Library, dis, Disassembler for Python bytecode, Python 3.14
    locator: module introduction (changed in version 3.11); dis.dis and dis.get_instructions, parameter adaptive
  - url: https://docs.oracle.com/en/java/javase/25/docs/api/java.management/java/lang/management/ManagementFactory.html#getCompilationMXBean()
    title: Java SE 25 API, java.lang.management.ManagementFactory
    locator: getCompilationMXBean()
---

## Idea

There are two traditional ways to run a program. A compiler translates it into another language, such as machine code, before it runs. An interpreter performs the same analyses but executes the program immediately. Just-in-time (JIT) compilation is translation that happens after the program begins to execute. It tries to get the benefits of both.[^1]

Each side has something that the other lacks. Compiled programs run faster, especially when the compiler produces code that the hardware executes directly. Interpreted programs are usually smaller and more portable. An interpreter also sees information that exists only at run time: the input, the control flow, the machine it runs on, and the types of the values.[^1] A JIT system starts as an interpreter and collects that information. Then it compiles the parts of the program that run often, and it uses what it saw to make the compiled code faster.

The work of translation now happens while the user waits. Aycock calls this the primary constraint on a JIT system: it must not cause long pauses in the normal execution of the program.[^1] Every design choice in a JIT follows from this constraint. The system compiles only the code that will repay the cost, and it compiles cheaply first and expensively later.

## How it works

### Find the hot code

Data from empirical studies shows that most programs spend most of their time in a small part of their code.[^2] A JIT system finds that part with counters. Hansen's Adaptive FORTRAN of 1974 kept a frequency-of-execution counter for each block of code. When a counter crossed a threshold, the block became a candidate for the next level of optimization.[^3] HotSpot works the same way: with the default option `-Xmixed`, the interpreter executes all bytecode except hot methods, which are compiled to native code.[^9]

The counter answers two questions: which code to compile, and when. The Self team found that the selection of the code mattered more than the trigger. Short methods are common in object-oriented code, so the method whose counter fired is often not the right one to compile. Its caller, with the hot method inlined, can be a better choice.[^4]

### Compile for what was seen

An interpreter for a dynamically typed language checks types on every operation, because `x * x` can multiply two integers, two floats, or two strings. A JIT compiler can specialize. The Self compilers produced a version of each method customized to its context. They had more type information than a static compiler, and the code they produced was much faster.[^4] Later Self systems gathered type information while the program ran, which Aycock calls type feedback, and used it when they recompiled a method.[^4] V8 does the same. During execution it tracks object shapes and types, and the optimizing compiler uses this data and the bytecode to generate speculative machine code.[^14]

### Guard and deoptimize

Specialized code is a bet. The compiler assumes that the types seen so far are the types that will come. A guard at the start of the code checks the assumption. When the guard fails, the system must continue with code that makes no assumption.

Moving from optimized code back to unoptimized code is called deoptimization. Hölzle, Chambers and Ungar described it in 1992 for the debugger of Self. Optimized code can be suspended only at interrupt points. At each of those points the compiler records enough information to rebuild the source-level state, and the system can convert an optimized activation into unoptimized ones on demand.[^16] V8 uses the same idea for speculation. Maglev attaches the abstract state of the interpreter's frame to each node that can deoptimize. The deoptimizer reads the values from the optimized frame and the machine registers and puts them where the interpreter expects them.[^15]

### Tiers

One compiler cannot be both cheap and thorough. Hansen already applied optimizations in stages, so that each crossing of a threshold added one more optimization to a block.[^3] The third-generation Self compiler first compiled methods with a fast, non-optimizing compiler and kept counters to decide when to recompile them.[^4] HotSpot and .NET call this tiered compilation, and V8 has several tiers too:

- **HotSpot.** Tiered compilation was introduced in Java SE 7. Without it, the server VM collects profiling information in the interpreter. With it, the server VM also uses the client compiler to generate compiled methods that collect profiling information about themselves. These run much faster than the interpreter during the profiling phase.[^8]
- **.NET.** Tiered compilation moves methods through two tiers. The first tier generates code quickly without optimizations (quick JIT) or loads pre-compiled ReadyToRun code. The second tier generates optimized code in the background.[^10]
- **V8.** V8 first compiles all JavaScript to Ignition bytecode and interprets it.[^14] Sparkplug compiles bytecode to machine code in a single linear pass, with no intermediate representation, so it can compile almost whenever it wants.[^13] Maglev is a fast optimizing compiler, and TurboFan is the optimizing compiler for peak performance.[^14]

### A model engine

The program below has every part of a JIT, in miniature. The function is `f(x) = x * x + 1`, held as a tree (see [[abstract-syntax-tree]]). Values are boxed with a type tag, as a [[sum-types|sum type]] of `Int` and `Float`. The interpreter walks the tree on each call and checks the tags of both operands on each operation, as a [[tree-walking-interpreter]] does.

The engine counts calls in the interpreter and records the types of the arguments, which is its profile. At the threshold of three calls, it compiles. The model compiles to [[closures]], not to machine code: each node becomes a function that calls the functions of its children, so the tree is no longer walked. If the profile holds only `int`, the compiled code unboxes the argument once and works on raw integers with no tag checks inside. A guard at the entry protects the bet. When the guard fails, the engine discards the code, goes back to the interpreter, and keeps the profile. The next compilation sees both types and produces generic code.

```python run id=engine
from dataclasses import dataclass


# Values carry their type, as boxed values do in a dynamic language.
@dataclass(frozen=True)
class Int:
    n: int
    kind = "int"

    def __str__(self):
        return str(self.n)


@dataclass(frozen=True)
class Float:
    x: float
    kind = "float"

    def __str__(self):
        return str(self.x)


# The body of f(x) = x * x + 1 as a tree.
@dataclass(frozen=True)
class Num:
    value: int


@dataclass(frozen=True)
class Arg:
    pass


@dataclass(frozen=True)
class Bin:
    op: str
    left: object
    right: object


OPS = {"+": lambda a, b: a + b, "*": lambda a, b: a * b}


def arith(op, a, b):
    """Generic arithmetic: check both tags on every operation."""
    if isinstance(a, Int) and isinstance(b, Int):
        return Int(OPS[op](a.n, b.n))
    fa = float(a.n) if isinstance(a, Int) else a.x
    fb = float(b.n) if isinstance(b, Int) else b.x
    return Float(OPS[op](fa, fb))


def interpret(node, arg):
    """The interpreter: walk the tree on every call."""
    match node:
        case Num(value):
            return Int(value)
        case Arg():
            return arg
        case Bin(op, left, right):
            return arith(op, interpret(left, arg), interpret(right, arg))


def compile_int(node):
    """Compiled code for int: closures over raw ints, no tag checks inside."""
    match node:
        case Num(value):
            return lambda n: value
        case Arg():
            return lambda n: n
        case Bin(op, left, right):
            f, l, r = OPS[op], compile_int(left), compile_int(right)
            return lambda n: f(l(n), r(n))


def compile_generic(node):
    """Compiled code for any argument: closures that still check tags."""
    match node:
        case Num(value):
            boxed = Int(value)
            return lambda a: boxed
        case Arg():
            return lambda a: a
        case Bin(op, left, right):
            l, r = compile_generic(left), compile_generic(right)
            return lambda a: arith(op, l(a), r(a))


THRESHOLD = 3


class Function:
    def __init__(self, name, body):
        self.name, self.body = name, body
        self.calls = 0  # calls in the interpreter since the last compilation
        self.seen = set()  # the profile: the kinds of the arguments
        self.code = None  # compiled code, or None
        self.counts = {"interpreter": 0, "compiled": 0, "deoptimizations": 0}

    def compile(self):
        if self.seen == {"int"}:
            raw = compile_int(self.body)

            def code(arg):
                if not isinstance(arg, Int):  # the guard
                    return None
                return Int(raw(arg.n))  # unbox once, compute, box the result

            self.code, self.kind, how = code, "int", "specialized for int"
        else:
            self.code, self.kind, how = compile_generic(self.body), "generic", "generic"
        print(f"* compile {self.name}: arguments seen: {', '.join(sorted(self.seen))}; {how}")

    def __call__(self, arg):
        if self.code is None and self.calls >= THRESHOLD:
            self.compile()
        if self.code is not None:
            result = self.code(arg)
            if result is not None:
                self.counts["compiled"] += 1
                return result, f"compiled, {self.kind}"
            print(f"* deoptimize {self.name}: guard failed, {arg} is not an int")
            self.counts["deoptimizations"] += 1
            self.code, self.calls = None, 0
        self.calls += 1
        self.counts["interpreter"] += 1
        self.seen.add(arg.kind)
        return interpret(self.body, arg), f"interpreter, call {self.calls}"


f = Function("f", Bin("+", Bin("*", Arg(), Arg()), Num(1)))
for arg in [Int(1), Int(2), Int(3), Int(4), Float(2.5), Int(6), Int(7), Int(8)]:
    result, tier = f(arg)
    print(f"f({arg}) = {result} [{tier}]")
print(", ".join(f"{k}: {v}" for k, v in f.counts.items()))
```

```output for=engine
f(1) = 2 [interpreter, call 1]
f(2) = 5 [interpreter, call 2]
f(3) = 10 [interpreter, call 3]
* compile f: arguments seen: int; specialized for int
f(4) = 17 [compiled, int]
* deoptimize f: guard failed, 2.5 is not an int
f(2.5) = 7.25 [interpreter, call 1]
f(6) = 37 [interpreter, call 2]
f(7) = 50 [interpreter, call 3]
* compile f: arguments seen: float, int; generic
f(8) = 65 [compiled, generic]
interpreter: 6, compiled: 2, deoptimizations: 1
```

The log shows the life of a function in a JIT. The first calls run in the interpreter while the profile grows. The compiled code runs `f(4)`. The float argument fails the guard, and that call runs in the interpreter. The profile now holds two types, so the second compilation produces generic code that never deoptimizes for this reason.

The model places its guard at the entry, so the interpreter can always restart the call from the beginning. A real JIT also places guards in the middle of compiled code, for example after an inlined call. There the deoptimizer must rebuild the interpreter's frame from the optimized one, which is the hard part of the technique.[^15][^16]

## In languages

JIT compilation is a property of an implementation, not of a language. The table describes the main implementation of each language. Each example builds the model engine above and prints the same log. The Java example also asks HotSpot for the name of its compilation system, and the Python example shows CPython specializing its own bytecode.

| Language | Verdict | Form |
|---|---|---|
| Java | native | HotSpot interprets bytecode and compiles hot methods; tiered compilation is on by default in the server VM |
| Kotlin | native | on the JVM, the same HotSpot tiers as Java |
| C# | native | the runtime compiles CIL to native code; tiered compilation with quick JIT, then an optimizing JIT in the background |
| JavaScript | native | V8: Ignition bytecode interpreter, Sparkplug, Maglev and TurboFan |
| TypeScript | native | runs as JavaScript on the same engines |
| Python | idiom | CPython specializes bytecode in its interpreter (PEP 659); its JIT compiler is experimental and not in the default build (PEP 744) |
| Go | idiom | compiled ahead of time; the engine is an ordinary program |
| Rust | idiom | compiled ahead of time; an `enum` for values and boxed closures for compiled code |
| C | idiom | compiled ahead of time; a tagged union for values and a tree of nodes for compiled code |
| C++ | idiom | compiled ahead of time; `std::variant` for values and `std::function` for compiled code |
| PowerShell | idiom | an engine written as a script (no example: pwsh is not installed) |
| Bash | idiom | a script can generate the text of a function at run time and define it with `eval` |
| SQL | n/a | whether queries are compiled at run time is a property of the database engine |
| Regex | n/a | the language has no user code to compile; a regex engine may compile patterns at run time |
| HTML/CSS | n/a | markup and style rules are not executed as code |

## Variations and related ideas

- **Invocation, executability and concurrency.** Aycock classifies JIT systems by three properties. A JIT compiler is explicitly invoked if the user must act to cause compilation, and implicitly invoked if compilation is invisible. A system is polyexecutable if it can execute both the source and the target representation, so it can choose when compiling is worth it. A concurrent JIT compiler runs alongside the program, for example in a separate thread, instead of pausing it.[^6] HotSpot is polyexecutable, because it can run a method in the interpreter or as compiled code.[^9] .NET generates its second tier in the background, which makes that part concurrent.[^10]
- **Compile only, no interpreter.** Burke and colleagues built a Java system with a compile-only strategy and no interpreter at all.[^5] A baseline compiler such as Sparkplug plays a similar role inside V8, because it compiles almost immediately.[^13]
- **Mixed code and throw-away compiling.** Mixed code, proposed in 1973, implemented the frequent parts of a program in native code and the rest in interpreted code, to save memory. Throw-away compiling compiled parts of the program on demand and discarded some or all of the compiled code when memory ran out.[^2]
- **Specializing interpreters.** Specialization does not require machine code. CPython replaces generic instructions with specialized ones while it runs, and replaces them back when the inputs change (PEP 659).[^18] This is the same bet and the same deoptimization, at the size of one instruction.
- **On-stack replacement.** A function that is stuck in a hot loop never returns, so the next call never sees the compiled code. V8 replaces the running interpreted function with optimized code for the loop, and it uses the same mechanism in the other direction when optimized code deoptimizes.[^13]
- **Bytecode as the input.** A JIT usually compiles [[bytecode]], not source text. V8 generates optimized code directly from Ignition bytecode, and the bytecode gives a simpler baseline for deoptimization.[^12]

## Pitfalls

### Compiled code without a guard

The specialized code is correct only for the types in the profile. Without the guard, a value of another type reaches code that assumes the wrong layout. In this Python model the mistake raises an exception.

```python fails expect=exception:AttributeError id=no-guard
from dataclasses import dataclass


@dataclass(frozen=True)
class Int:
    n: int


@dataclass(frozen=True)
class Float:
    x: float


def compiled_for_int(arg):
    # No guard: the compiler assumed that every argument is an Int.
    n = arg.n
    return Int(n * n + 1)


print(compiled_for_int(Int(3)))
print(compiled_for_int(Int(4)))
print(compiled_for_int(Float(2.5)))
```

The fix is a guard that checks the assumption and sends other values to code that makes no assumption.

```python run id=no-guard-fix
from dataclasses import dataclass


@dataclass(frozen=True)
class Int:
    n: int


@dataclass(frozen=True)
class Float:
    x: float


def generic(arg):
    # The slow path: check the tag, then compute.
    if isinstance(arg, Int):
        return Int(arg.n * arg.n + 1)
    return Float(arg.x * arg.x + 1)


def compiled_for_int(arg):
    if not isinstance(arg, Int):  # the guard
        return generic(arg)  # deoptimize: fall back to the generic path
    n = arg.n
    return Int(n * n + 1)


print(compiled_for_int(Int(3)))
print(compiled_for_int(Int(4)))
print(compiled_for_int(Float(2.5)))
```

```output for=no-guard-fix
Int(n=10)
Int(n=17)
Float(x=7.25)
```

### Measuring before the warm-up ends

A JIT system runs slowly while it finds and compiles the hot code, and this phase is called warm-up. In the log above, the first three calls ran in the interpreter. A benchmark that measures only those calls measures the interpreter, and a benchmark that mixes them with later calls measures neither tier. Benchmark methods for JIT systems therefore run a benchmark many times in one process and discard the data from before the warm-up.[^17]

That method assumes that the program reaches a steady state of peak performance. Barrett and colleagues tested the assumption on small, deterministic microbenchmarks in a tightly controlled environment. Only 30.0% to 43.5% of the pairs of virtual machine and benchmark consistently reached a steady state of peak performance, and none of the seven virtual machines that they studied did so consistently. Many benchmarks slowed down, never reached a steady state, or behaved differently from one run to the next.[^17] Report all the iterations, not only the fast ones. Repeat the measurement in fresh processes, and check whether a steady state exists before you report one.

The examples on this page print no timings for the same reason. A timing changes on each run, and the log of tiers does not.

### Compiling too early

A low threshold compiles before the profile is stable, and the code deoptimizes when the types change. The V8 team wrote that it could not start optimizing earlier because it would not yet have stable feedback about object shapes.[^13] A compiler that is cheap to run makes a wrong bet cheap. The V8 team deploys Maglev earlier than TurboFan because deoptimizing and recompiling Maglev code costs little if the feedback was not yet stable.[^15] In the model, the threshold of 3 is low on purpose, so that the log shows a deoptimization.

## Cost

- **Compile time at run time.** Every compilation takes time from the program. A JIT must not cause long pauses, so it compiles only hot code and uses cheap tiers first.[^1] In short sessions, much of the work happens before the optimizing compiler has a chance to start.[^13]
- **Memory.** Compiled code occupies memory in addition to the bytecode. The V8 team built Ignition to reduce this overhead, because baseline machine code used memory even for code that ran only once.[^11] HotSpot multiplies the default size of its code cache by 5 when tiered compilation is on, to hold the extra profiled code.[^8]
- **Profiling.** Counters and type feedback cost time in the lower tiers. HotSpot moves the profiling into compiled code of the first tier, which is faster than profiling in the interpreter.[^8]
- **Complexity.** A JIT has two or more execution paths that must behave identically: the interpreter and each compiler.[^2] Deoptimization must be able to rebuild the interpreter's state at any guard.[^16]
- **Predictability.** The same program runs at different speeds at different times, as the tiers change. This makes performance hard to measure.[^17]

## History

- **1960.** John McCarthy's paper on LISP mentioned compiling functions into machine language, fast enough that the output need not be saved. Aycock suggests that this is the earliest published work on JIT compilation.[^7]
- **1968.** Ken Thompson compiled regular expressions into IBM 7094 code, which was then executed to perform the match.[^7]
- **1974.** Hansen's Adaptive FORTRAN kept frequency counters for blocks of code and optimized hot blocks at run time, one stage at a time.[^3]
- **1984.** Deutsch and Schiffman translated Smalltalk virtual machine code into native code on demand, and cached the native code.[^7]
- **1992.** Hölzle, Chambers and Ungar described dynamic deoptimization in Self, to debug optimized code at the source level.[^16]
- **1994.** Hölzle's adaptive optimization for Self compiled methods first with a fast, non-optimizing compiler, recompiled hot ones, and used type feedback gathered at run time.[^4]
- **2003.** Aycock surveyed forty years of JIT work in "A Brief History of Just-In-Time". He noted that Java brought the term just-in-time into common use, and that the pressure for fast Java implementations started a renaissance in JIT research.[^5]
- **2017.** Barrett and colleagues showed that many VMs do not reach a steady state of peak performance, even on small deterministic benchmarks.[^17] V8 launched its pipeline of the Ignition interpreter and the TurboFan compiler.[^12]
- **2021.** V8 added Sparkplug, a non-optimizing compiler between Ignition and TurboFan.[^13] PEP 659 proposed a specializing adaptive interpreter for CPython.[^18]
- **2023.** The V8 team described Maglev, a fast optimizing compiler between Sparkplug and TurboFan.[^14]
- **2024.** PEP 744 described the experimental JIT compiler that had been merged into CPython's main development branch.[^19]

## References
