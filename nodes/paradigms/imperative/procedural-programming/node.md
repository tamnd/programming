---
tag: 7D9E78
kind: concept
slug: procedural-programming
name: Procedural programming
aliases: [procedural style, procedures, subroutines, top-down design, stepwise refinement, functional decomposition]
area: paradigms/imperative
level: core
summary: >-
  Procedural programming organizes an imperative program as named procedures that take parameters and return results, and divides a task into steps until each step is simple.
applies_to: core-languages
viz: optional
formal: none
history:
  year: 1958
  people: [David Wheeler, Niklaus Wirth, David Parnas]
  note: Subroutines came with the first stored-program machines, and Fortran II, ALGOL 60, Pascal and C made the procedure the main unit of design.
see_also: [imperative-programming, structured-programming, recursion, stack-and-heap, parameter-passing, lexical-scope, object-oriented-programming, functional-programming]
same_as:
  wikidata: Q1418502
  wikipedia: Procedural_programming
sources:
- url: https://en.cppreference.com/w/c/language/functions
  title: Functions, C reference, cppreference.com
  locator: introduction
- url: https://docs.python.org/3.14/tutorial/controlflow.html#defining-functions
  title: The Python Tutorial, 4. More Control Flow Tools, Python 3.14 documentation
  locator: 4.8 Defining Functions
- url: https://www.kernel.org/doc/html/latest/process/coding-style.html#functions
  title: Linux kernel coding style
  locator: 6) Functions
- url: https://www.win.tue.nl/~wstomv/edu/2ip30/references/criteria_for_modularization.pdf
  title: On the Criteria To Be Used in Decomposing Systems into Modules, D. L. Parnas, Communications of the ACM 15(12), 1972
  locator: The Criteria
---

## Idea

Procedural programming organizes an imperative program as a set of procedures. A procedure is a named block of statements that takes parameters, may return a result, and can be called from other code. The programmer divides a large task into steps, writes each step as a procedure, and divides the steps again until each one is simple. The main program then reads as a short list of calls:

```python run id=idea
def read_scores(lines):
    return [int(x) for x in lines if x.strip()]


def mean(xs):
    return sum(xs) / len(xs)


def grade(score):
    if score >= 90:
        return "A"
    if score >= 75:
        return "B"
    return "C"


def print_report(scores):
    print(f"mean {mean(scores):.1f}")
    for s in scores:
        print(s, grade(s))


print_report(read_scores(["91", "78", "", "64"]))
```

```output for=idea
mean 77.7
91 A
78 B
64 C
```

Each procedure has one task and a name that says what the task is. `print_report` does not know how a grade is computed, and `grade` does not know where the scores came from. A change to the grading rule touches one procedure.

## How it works

**Procedures as abstraction.** A call to a procedure hides its statements behind a name and a list of parameters. The caller depends only on what the procedure does, not on how. cppreference describes a C function as a construct "that associates a compound statement (the function body) with an identifier (the function name)".[^1] The same procedure can be called from many places, so a fix in one place fixes all the callers.

**The call stack.** Each call gets a new frame on the stack, with its parameters, its local variables and the address to return to. The frame disappears when the procedure returns. This is what lets a procedure call itself, and what keeps the local variables of one call apart from those of another. See [the stack and the heap](/stack-and-heap/) and [recursion](/recursion/).

**Local and global state.** Variables declared inside a procedure are local to one call. Variables declared outside are global and shared by all procedures. Global state is easy to use and hard to reason about, because any procedure can change it. Languages make writes to it visible. The Python tutorial explains that a function cannot assign to a global variable unless the variable is "named in a global statement".[^2] The good procedural style passes data through parameters and return values and keeps global state small.

**Top-down decomposition.** The usual method of design is stepwise refinement: write the top-level procedure in terms of steps that do not exist yet, then write each step in the same way. Niklaus Wirth described the method in 1971, and it became the standard way to teach programming in Pascal. The result is a tree of procedures in which each level is a more detailed version of the level above.

**Short procedures.** Procedural style guides limit the size of a procedure. The Linux kernel coding style says that "Functions should be short and sweet, and do just one thing."[^3] It also limits local variables: "They shouldn't exceed 5-10, or you're doing something wrong. Re-think the function, and split it into smaller pieces."[^3]

**Modules.** As programs grow, procedures are grouped into modules, such as C source files with a header, Pascal units or Go packages. A module exports some procedures and keeps others private. David Parnas showed in 1972 that the choice of what goes into a module matters. A decomposition that makes "each major step in the processing a module" follows the flowchart,[^4] and a change to a data format then touches many modules. A decomposition in which each module hides "a design decision" from the others keeps such changes local.[^4]

## In languages

| Language | Verdict | Mechanism |
|---|---|---|
| Python | native | `def` at module level, modules as files, `global` for writes to shared state |
| JavaScript | native | function declarations, ES modules |
| TypeScript | native | as JavaScript, with typed signatures |
| Java | idiom | `static` methods in a class; no free functions |
| Kotlin | native | top-level functions in packages |
| C# | idiom | `static` methods and classes, local functions in top-level code |
| Go | native | package-level functions, export by capital letter |
| Rust | native | free functions in modules, `pub` |
| C | native | functions, headers, `static` for file scope |
| C++ | native | free functions, namespaces, overloading |
| Bash | native | shell functions, `local` |
| PowerShell | native | functions with `param` blocks, modules |
| SQL | idiom | stored procedures in procedural extensions |
| Regex, HTML, CSS | n/a | no procedures |

C, Go, Rust, Python and C++ let a program consist of free functions with no classes. In C, the procedure is the main unit of design. A source file is a module, `static` makes a function private to its file, and a header declares the functions that other files may call. Go follows the same plan with packages, and a name that starts with a capital letter is exported. Rust uses modules and `pub`.

Java and C# require every method to be in a class. A procedural design there uses classes that have only `static` methods, such as `java.lang.Math`, and C# adds static classes and local functions. The procedural style is still common inside methods, which are often written as a sequence of steps.

Bash and PowerShell functions are procedures too, but their parameters and results are different. A Bash function takes its arguments as strings in `$1`, `$2` and so on, writes its result to standard output, and returns a status code.

## Variations and related ideas

- **Functions and procedures.** Some languages separate procedures, which act, from functions, which return a value. Pascal and Ada have both, and in C a procedure is a function that returns `void`.
- **Structured programming.** Procedures are one of the structured forms, with one entry and one exit. See [structured programming](/structured-programming/).
- **Modular programming.** Procedures grouped behind an interface, with private data. Modula-2 and Ada made modules part of the language.
- **Object-oriented programming.** Procedures move into the data types that they work on and become methods. See [object-oriented programming](/object-oriented-programming/).
- **Functional programming.** Functions with no side effects, so that the order of calls does not matter. See [functional programming](/functional-programming/).
- **Coroutines.** Procedures that can suspend and resume, rather than run to the end on each call.

## Pitfalls

- **Global state.** Procedures that read and write global variables depend on the order in which they are called, and cannot be tested alone. Pass the state as parameters.
- **Long procedures.** A procedure of hundreds of lines does many things, has many local variables, and is hard to test. Split it into named steps.
- **Long parameter lists.** A procedure with many parameters is easy to call with the arguments in the wrong order. Group related parameters into a structure.
- **Output parameters.** Procedures that return results through pointer or reference parameters hide which arguments change. Prefer return values, and return a structure or a tuple when there is more than one result.
- **Decomposition by steps only.** Dividing a program by the steps of the flowchart spreads knowledge of each data format across many procedures. Group the procedures that know a format into one module.
- **Data and code apart.** When many procedures work on the same structure, each one must keep its invariants. A procedure that forgets breaks the others.

## Cost

A procedure call costs a few instructions: the arguments go into registers or onto the stack, the processor saves the return address and jumps, and the procedure may save registers and set up a frame. Compilers inline small procedures, so a short procedure usually costs nothing when the compiler can see its body. Calls across module or library boundaries are harder to inline, and link-time optimization recovers some of them. Deep chains of calls use stack space, and each thread has a fixed amount.

## History

Subroutines are older than high-level languages. The programmers of EDSAC in Cambridge used a library of subroutines in 1949, and David Wheeler devised the method of jumping to a subroutine and back that is now called the Wheeler jump. Fortran II added `SUBROUTINE` and `FUNCTION` in 1958. ALGOL 60 added procedures with local variables, recursion and parameters by name, and Pascal and C made the procedure the main unit of design in the 1970s. Wirth's stepwise refinement in 1971 and Parnas's criteria for modules in 1972 set the methods of procedural design,[^4] and the Linux kernel, written in C, remains one of the largest procedural programs in use.[^3]

## References
