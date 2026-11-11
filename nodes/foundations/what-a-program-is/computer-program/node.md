---
tag: NEW-computer-program
kind: concept
slug: computer-program
name: What a program is
aliases: [program, computer program, source code, executable, entry point, main function, script, process, exit status, program state]
area: foundations/what-a-program-is
level: core
summary: >-
  A program is text in a programming language that describes a computation. A run of it reads input, changes state step by step, writes output and ends with an exit status.
applies_to: core-languages
viz: optional
formal: none
history:
  year: 1843
  people: [Ada Lovelace, Charles Babbage, Alan Turing]
  note: Ada Lovelace published a table of operations for the Analytical Engine in 1843, and stored-program computers ran programs from their own memory from 1948.
see_also: [hello-world-program, compilers-and-interpreters, processor-and-memory, imperative-programming, declarative-programming, bytecode, parse-command-line-arguments, read-from-standard-input]
same_as:
  wikidata: Q40056
  wikipedia: Computer_program
sources:
- url: https://mitp-content-server.mit.edu/books/content/sectbyfn/books_pres_0/6515/sicp.zip/full-text/book/book-Z-H-9.html
  title: Harold Abelson and Gerald Jay Sussman, Structure and Interpretation of Computer Programs, second edition, MIT Press, 1996
  locator: Chapter 1, Building Abstractions with Procedures
- url: https://en.cppreference.com/w/cpp/language/basic_concepts.html
  title: Basic concepts, cppreference.com
  locator: Introduction
- url: https://go.dev/ref/spec#Program_execution
  title: The Go Programming Language Specification
  locator: Program execution
- url: https://docs.python.org/3/reference/executionmodel.html
  title: Execution model, The Python Language Reference
  locator: 4.1 Structure of a program
- url: https://www.fourmilab.ch/babbage/sketch.html
  title: L. F. Menabrea, Sketch of the Analytical Engine Invented by Charles Babbage, with notes by the translator, Ada Augusta, Countess of Lovelace, 1843
  locator: Note A and Note G
- url: https://www.cs.utexas.edu/~EWD/transcriptions/EWD03xx/EWD340.html
  title: Edsger W. Dijkstra, The Humble Programmer, ACM Turing Award Lecture, 1972, EWD340
  locator: Main text
---

## Idea

A program is a description of a computation, written as text in a programming language, that a machine can carry out. It names the data that the computation starts from, the steps that change that data, and the results that it gives to the world. When a program runs, it reads input, changes its state one step at a time, writes output, and ends with an exit status that tells the operating system whether it succeeded. The same text can run many times on different input, and each run is a separate process. An algorithm is the method, a program is one exact statement of a method in one language, and a process is one run of a program.

```python run id=idea
import ast

source = """
a, b = 1071, 462
while b != 0:
    a, b = b, a % b
print("gcd", a)
"""

tree = ast.parse(source)  # the text has a structure
print([type(node).__name__ for node in tree.body])

code = compile(tree, "<gcd>", "exec")  # the structure becomes instructions
state = {}
exec(code, state)  # running the instructions changes the state
print("final state:", {k: v for k, v in state.items() if k in "ab"})

edited = source.replace("1071, 462", "48, 18")  # the program is also data
exec(edited, {})
```

```output for=idea
['Assign', 'While', 'Expr']
gcd 21
final state: {'a': 21, 'b': 0}
gcd 6
```

The string `source` holds a program of four lines that computes the greatest common divisor of 1071 and 462 with Euclid's algorithm. Python parses the text into a tree of three statements, an assignment, a loop and a call, then compiles the tree into instructions, and running the instructions changes the state until `b` is 0 and `a` holds the answer, 21. Because the program is a string, the last lines edit it like any other data and run the new program, which gives 6 for 48 and 18. Text, structure, instructions and state are four views of the same program.

## How it works

**A program directs a process.** *Structure and Interpretation of Computer Programs* opens with the distinction between the text and its run: "The evolution of a process is directed by a pattern of rules called a program. People create programs to direct processes."[^1] The program is fixed text; the process is what happens when a machine follows it, and it can differ on each run because the input differs. Most of the work of programming is to write text whose processes do the right thing for every input, including inputs that the author did not try.

**From text to instructions.** A processor cannot run text. A compiler translates the text into machine code ahead of time, an interpreter reads it and carries out each part as it goes, and many systems do both, compiling to bytecode for a virtual machine that interprets it or compiles it again at run time. See [compilers and interpreters](/compilers-and-interpreters/) and [bytecode](/bytecode/). Each language defines what text is a valid program and what it means. The C++ reference states: "A C++ program is a sequence of text files (typically header and source files) that contain declarations. They undergo translation to become an executable program, which is executed when the C++ implementation calls its main function."[^2]

**An entry point and a boundary.** Every language says where a program starts and what it contains. The Go specification says: "A complete program is created by linking a single, unimported package called the main package with all the packages it imports, transitively."[^3] It goes on: "Program execution begins by initializing the program and then invoking the function main in package main. When that function invocation returns, the program exits."[^3] Scripting languages start at the top of a file instead. The Python reference says: "A Python program is constructed from code blocks."[^4] At its boundary a program meets the operating system: its arguments, its environment variables, its standard input and output streams, and the exit status it returns. See [hello, world](/hello-world-program/), [command-line arguments](/parse-command-line-arguments/) and [standard input](/read-from-standard-input/).

**State and steps.** Inside the boundary, a program in the common imperative style is a sequence of steps that change state, the values of its variables and data structures. The meaning of each step depends on the state that the steps before it left. Euclid's algorithm replaces the pair `(a, b)` by `(b, a mod b)`, and the example files show that the order of the updates matters: updating `a` before computing the remainder loses the old value and gives a wrong answer with no error. Declarative and functional programs describe the result in other terms, but they also run as steps on a machine. See [imperative programming](/imperative-programming/) and [declarative programming](/declarative-programming/).

**Programs are data.** A program is text, so other programs can read it, check it, transform it and generate it. Compilers, editors, linters, formatters, test tools and code generators all treat programs as data, and the [stored-program computer](/processor-and-memory/) keeps the translated program in the same memory as its data. Ada Lovelace saw in 1843 that a machine that follows a program is not limited to arithmetic: "the Analytical Engine weaves algebraical patterns just as the Jacquard-loom weaves flowers and leaves."[^5] She also stated its limit: "It can do whatever we know how to order it to perform."[^5]

**Programs are claims.** A program states, in effect, that its processes compute some result. Whether it does is a separate question, and running it on some inputs only samples the answer. Edsger Dijkstra put it in his 1972 Turing Award lecture: "Program testing can be a very effective way to show the presence of bugs, but is hopelessly inadequate for showing their absence."[^6] Specifications, types, tests, reviews and proofs are the ways to make the claim stronger. See [unit testing](/unit-testing/) and [property-based tests](/property-based-tests/).

## In languages

| Language | Verdict | Mechanism |
|---|---|---|
| Python | native | a script runs top to bottom; `if __name__ == "__main__"`; `sys.exit` |
| JavaScript | native | a script or module; `process.exitCode` in Node.js |
| TypeScript | native | as JavaScript, after type checking |
| Java | native | classes; `public static void main(String[] args)` |
| Kotlin | native | a top-level `fun main` |
| C# | native | one entry point; top-level statements or `static Main` |
| Go | native | package `main` and `func main` |
| Rust | native | a crate with `fn main`; `ExitCode` or `Result` |
| C | native | translation units linked into an executable; `int main` |
| C++ | native | as C, with constructors of globals before `main` |
| Bash | native | a script of commands; `exit` |
| PowerShell | native | a script file; `exit` |
| SQL | idiom | scripts and stored procedures; a query states a result |
| Regex | n/a | a pattern runs inside another program |
| HTML | n/a | describes a document, not a computation |
| CSS | n/a | describes presentation, not a computation |

Every example file is a complete program that computes the greatest common divisor of 1071 and 462 and prints the state after each step: `(462, 147)`, `(147, 21)` and `(21, 0)`, then the answer 21. The algorithm is the same in each file; the programs differ in where they start, how they update two variables at once, and how they report the exit status. Python, JavaScript, C#, Go and Rust can assign both variables in one statement. Java and C keep the remainder in a temporary first, and C++ uses `std::exchange`.

Languages fall into two groups at the boundary. In Python, JavaScript, Bash and PowerShell, and in C# with top-level statements, a file of statements is a program, and the runtime runs it from the top. In Java, Go, Rust, C, C++ and Kotlin, a program is a set of declarations, and execution starts at a function called `main` that the language or the linker requires. The Go file shows that a package `main` without a `main` function is not a program at all, and the compiler rejects it.

## Variations and related ideas

- **Scripts and compiled programs.** A script is run from its source by an interpreter; a compiled program is translated to an executable ahead of time. Many languages do both. See [compilers and interpreters](/compilers-and-interpreters/).
- **Libraries.** A library is code without an entry point, which programs link to or import. A program is a library plus a starting point.
- **Processes and threads.** A process is a running program with its own memory; a thread is one sequence of steps inside a process. See [threads](/threads/) and [run a subprocess](/run-a-subprocess/).
- **Interactive programs.** A REPL reads, evaluates and prints one piece of a program at a time, and a server or a graphical program runs a loop that waits for events instead of ending after its output.
- **Specifications.** A specification states what a program must do without saying how. Formal methods prove that a program meets one.
- **Programs that write programs.** Compilers, macros and code generators take programs as input or produce them as output. See [constant evaluation](/constant-evaluation/).
- **The theory of computation.** Turing machines and the lambda calculus define which computations any program can perform, and the halting problem shows that no program can decide, for every program and input, whether it stops.

## Pitfalls

- **Testing only the inputs you thought of.** A program must work for every input that it accepts, including empty input, very large values and malformed text. Fuzzing and property-based tests try inputs that the author did not choose. See [fuzz testing](/fuzz-testing/).
- **Order of updates.** Updating one part of the state before another reads its old value gives wrong results with no error. The wrong blocks of the JavaScript and C files show this. Use multiple assignment or a temporary.
- **Ignoring the exit status.** A program that prints an error and then exits with status 0 tells scripts and build tools that it succeeded. Return a nonzero status on failure.
- **Missing or misnamed entry points.** A misspelled `main`, a `main` in the wrong package, or a Python file whose `main` is never called gives a program that does nothing or does not build. The fails block of the Go file shows one case.
- **Code that runs on import.** Statements at the top level of a Python or JavaScript module run each time another program imports it. Keep work in functions and guard the entry point.
- **Relying on undefined behavior.** In C and C++, a program that breaks a rule of the language, such as reading past the end of an array, has no defined meaning, and it can seem to work until the compiler or the input changes. See [undefined behavior](/undefined-behavior/).

## Cost

A program costs time and memory when it runs, and those costs depend on its input; the analysis of algorithms states them as functions of the input size. It also costs time before it runs. A compiled program pays for translation once, and its start is usually fast; an interpreted program pays to parse and start the interpreter on each run, often tens of milliseconds for Python or Node.js and more for a JVM before it has compiled the hot code. The size of the executable or of the installed runtime is a further cost, from tens of kilobytes for a small C program to tens or hundreds of megabytes for a program that ships with its runtime. Most of the cost of a program over its life is the work of people who read and change it.

## History

Joseph Marie Jacquard's loom of 1804 followed a chain of punched cards, an early sequence of instructions for a machine. In 1843 Ada Lovelace translated Luigi Menabrea's paper on Charles Babbage's Analytical Engine and added notes, and Note G gave a table of operations for the engine to compute Bernoulli numbers, often called the first published computer program.[^5] Alan Turing's paper of 1936 defined computation by a machine that follows a finite table of rules, and the stored-program computers of 1948 and 1949 held their programs in memory. The word program for a plan of operations came into use with the ENIAC project in the mid-1940s.

Programs were first written in machine code and then in assembly language, and FORTRAN, from IBM in 1957, made it practical to write them in a notation close to mathematics. In the 1960s and 1970s, structured programming, which Dijkstra argued for, made the structure of the text follow the structure of the process.[^6] Niklaus Wirth's 1976 book had the title *Algorithms + Data Structures = Programs*, and Harold Abelson and Gerald Jay Sussman's *Structure and Interpretation of Computer Programs*, first published in 1985, taught programs as a way to express ideas about processes.[^1]

## References
