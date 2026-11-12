---
tag: CR89SM
kind: concept
slug: hello-world-program
name: Hello, world
aliases: [hello world, hello world program, first program, entry point, main function, standard output, exit status]
area: foundations/what-a-program-is
level: core
summary: >-
  A hello world program prints Hello, world! and exits. It is the smallest complete program, and it shows the entry point, the standard output stream and the exit status of a language.
applies_to: core-languages
viz: optional
formal: none
history:
  year: 1972
  people: [Brian Kernighan, Dennis Ritchie]
  note: Kernighan wrote the first known hello world in a 1972 tutorial on B, and the 1978 book The C Programming Language made it the first program of every language.
see_also: [compilers-and-interpreters, call-stack-and-heap, characters-and-code-points, parse-command-line-arguments, read-from-standard-input, run-a-subprocess]
same_as:
  wikidata: Q131303
  wikipedia: Hello,_world
sources:
- url: https://en.cppreference.com/w/cpp/language/main_function.html
  title: Main function, cppreference.com
  locator: Explanation
- url: https://go.dev/ref/spec#Program_execution
  title: The Go Programming Language Specification
  locator: Program execution
- url: https://doc.rust-lang.org/book/ch01-02-hello-world.html
  title: Hello, World!, The Rust Programming Language
  locator: Anatomy of a Rust Program
- url: https://docs.oracle.com/javase/specs/jls/se25/html/jls-12.html
  title: Chapter 12, Execution, The Java Language Specification, Java SE 25 Edition
  locator: 12.1 Java Virtual Machine Startup
- url: https://docs.python.org/3/library/__main__.html
  title: __main__, Top-level code environment, Python documentation
  locator: What is the top-level code environment?
- url: https://www.nokia.com/bell-labs/about/dennis-m-ritchie/btut.html
  title: Brian W. Kernighan, A Tutorial Introduction to the Language B, Bell Laboratories, 1972
  locator: 2. General Layout of Programs and 7. External Variables
---

## Idea

A hello world program writes the text `Hello, world!` and ends. It is the smallest complete program in a language, so it is the first program that most programmers write, and the first test that a new compiler, toolchain or machine works. The program is short, but it touches every step of running code: the source file, the translation by a compiler or interpreter, the entry point where execution starts, a call to the standard library, the standard output stream, and the exit status that the process gives back to the system.

```python run id=idea
import subprocess
import sys

source = 'print("Hello, world!")'
child = subprocess.run([sys.executable, "-I", "-c", source], capture_output=True)
print("program:    ", source)
print("stdout:     ", child.stdout)
print("bytes:      ", len(child.stdout), "=", child.stdout.hex(" "))
print("stderr:     ", child.stderr)
print("exit status:", child.returncode)

broken = subprocess.run([sys.executable, "-I", "-c", 'print("Hello, world!"'],
                        capture_output=True, text=True)
print("exit status:", broken.returncode, "for", broken.stderr.splitlines()[-1])
```

```output for=idea
program:     print("Hello, world!")
stdout:      b'Hello, world!\n'
bytes:       14 = 48 65 6c 6c 6f 2c 20 77 6f 72 6c 64 21 0a
stderr:      b''
exit status: 0
exit status: 1 for SyntaxError: '(' was never closed
```

The program runs a hello world in a new Python process and looks at what comes out. The output is 14 bytes on standard output: the 13 characters of the text, encoded as ASCII, and a newline, byte `0a`. Nothing goes to standard error, and the exit status is 0, which means success. The second program has a syntax error, so it never runs: the interpreter writes a message to standard error and exits with status 1.

## How it works

**The entry point.** Every program has a place where execution starts. In C, C++, Java, C#, Go and Rust it is a function named `main`. The C++ reference states the rule: a program must "contain a global function named main, which is the designated start of the program in hosted environment."[^1] The Go specification says: "Program execution begins by initializing the program and then invoking the function main in package main. When that function invocation returns, the program exits."[^2] The Rust book says: "The main function is special: It is always the first code that runs in every executable Rust program."[^3] Java looks for a method in a class: "The Java Virtual Machine starts execution by invoking a main method of some specified class or interface."[^4] Scripting languages such as Python, JavaScript, Ruby and the shells have no special function; they run the statements of the file from top to bottom.

**Top-level code.** Without a `main`, a language needs another way to tell a program from a library. Python names the module that starts the program `__main__`: "`__main__` is the name of the environment where top-level code is run."[^5] Code that should run only as a program goes under `if __name__ == "__main__":`, so that importing the file for its functions does not also run it. Some compiled languages also accept top-level code. C# allows top-level statements in one file and generates the `Main` method, and newer Java accepts a `main` method without a class around it. Both changes make the first program shorter for beginners.

**Output.** A process starts with three open streams: standard input, standard output and standard error. `print`, `console.log`, `System.out.println`, `fmt.Println` and `printf` write to standard output, which is the terminal, a file or the input of another program, as the user who starts the program chooses. Diagnostics go to standard error, so that they do not mix with the data. Output is text encoded as bytes, usually UTF-8, and each line ends with a newline. For speed, the standard library buffers output: it collects bytes and writes them in large blocks, at each newline on a terminal, and when the buffer is full or the program exits when the output goes to a pipe or a file. A program that ends without the normal exit path can lose buffered output, as the wrong blocks in the Python and C files show.

**The exit status.** When a program ends, it gives a small integer to the process that started it. Zero means success, and any other value means failure, with meanings that each program chooses. In C and C++, the value that `main` returns is the status, and reaching the end of `main` counts as `return 0`: "if control reaches the end of main without encountering a return statement, the effect is that of executing return 0;".[^1] Shells, build tools and CI systems read the status to decide whether a step worked, so a program that fails must also exit with a status other than 0.

**The toolchain.** The source must become something that the machine runs. For C, C++, Go and Rust, a compiler makes a native executable, which then runs alone. For Java and C#, a compiler makes bytecode, and a virtual machine runs it. Python and JavaScript load the source and run it at once. Modern tools hide the steps: `java Main.java`, `dotnet run`, `go run` and `cargo run` compile and run in one command. See [compilers and interpreters](/compilers-and-interpreters/).

## In languages

| Language | Verdict | Mechanism |
|---|---|---|
| Python | native | top-level `print`; `if __name__ == "__main__"` |
| JavaScript | native | top-level `console.log` |
| TypeScript | native | as JavaScript |
| Java | native | `main` method; `System.out.println` |
| Kotlin | native | top-level `fun main`; `println` |
| C# | native | top-level statements; `Console.WriteLine` |
| Go | native | `package main`, `func main`; `fmt.Println` |
| Rust | native | `fn main`; the `println!` macro |
| C | native | `int main`; `printf` |
| C++ | native | `int main`; `std::cout` |
| Bash | native | `echo` |
| PowerShell | native | a string on its own line; `Write-Output` |
| SQL | native | `SELECT 'Hello, world!'` |
| Regex | n/a | a pattern produces no output |
| HTML | native | the text in the body of a document |
| CSS | idiom | the `content` property of a pseudo-element |

Every example file writes `Hello, world!`, formats a word and a number into a second line, and reports the number of command-line arguments, which is 0 when the examples run. The files show the three shapes of an entry point. Python and JavaScript run a file from the top. Java, Go, C and C++ need a named function in a fixed place, and C# and newer Java accept top-level code and generate that function. Rust makes the output call a macro, which the compiler checks at compile time.

The cost of the first program differs much more than the programs do. A Python, JavaScript or Bash hello world is one line. Java in its classic form needs a class, a method with a fixed signature and a qualified call, which is why the language added a shorter form. C needs an `#include` and a `main`, and the build gives a native executable of tens of kilobytes. Go and Rust give a larger executable, because it holds the runtime or the parts of the standard library that the program uses.

## Variations and related ideas

- **Hello world in other places.** On a microcontroller the first program blinks an LED. A web framework's first program answers an HTTP request with the text, and a graphics API's first program draws a triangle. Each tests the smallest complete path through the toolchain.
- **Command-line arguments.** The next step after a fixed greeting is a greeting by name, from an argument. See [parse command-line arguments](/parse-command-line-arguments/).
- **Reading input.** A program that reads a name from standard input and greets it uses the input stream as well. See [read from standard input](/read-from-standard-input/).
- **The read-eval-print loop.** An interactive interpreter runs `print("Hello, world!")` as soon as the user types it, with no file and no build step.
- **Quines.** A quine is a program that prints its own source, a classic puzzle that grew out of the same interest in the smallest programs.
- **Programs as processes.** The program runs as a process, with its own memory, streams and exit status. See [run a subprocess](/run-a-subprocess/) and [the call stack and heap](/call-stack-and-heap/).

## Pitfalls

- **Lost output at exit.** Ending a process with `os._exit` in Python or `_exit` in C skips the flush of output buffers, so output to a pipe or a file is lost, while output to a terminal looks correct. The wrong blocks in the Python and C files show this. Return from the program normally, or flush first.
- **A missing newline.** Text without a final newline leaves the shell prompt on the same line, and tools that read lines can drop the last one. End each line of output with a newline.
- **Status 0 on failure.** A program that prints an error message and then exits with status 0 tells scripts and CI that it worked. Exit with a status other than 0 on failure.
- **Errors on standard output.** Error messages written to standard output mix with the data, and a pipeline passes them on as if they were data. Write diagnostics to standard error.
- **The wrong name.** `println` without the `!` in Rust, `Main` in the wrong case in Java, and a `main` method with the wrong signature all fail to compile or to start. The fails block in the Rust file shows the first.
- **Encoding.** Text with letters outside ASCII can print as broken characters when the terminal and the program use different encodings. Use UTF-8 for source files and output. See [characters and code points](/characters-and-code-points/).

## Cost

The program itself costs almost nothing: a few system calls to write 14 bytes and to exit. Most of the time goes to starting the program. A native executable from C starts in about a millisecond. A Python or Node.js interpreter takes tens of milliseconds to start, and a JVM or .NET runtime somewhat more, because each loads a runtime and its libraries before the first line runs. Commands that compile and run in one step, such as `java Main.java` and `dotnet run`, add the time of the compile, from a fraction of a second to a few seconds. On disk, a C hello world is tens of kilobytes and links to the shared C library, while a Go or Rust executable, which links its standard library statically, is hundreds of kilobytes to a few megabytes.

## History

Brian Kernighan wrote the first known hello world in *A Tutorial Introduction to the Language B* at Bell Labs in 1972. B could hold only a few characters in each constant, so the program printed the text in three pieces: "a 'hell'; b 'o, w'; c 'orld';".[^6] The same tutorial set out the rule that C kept: "all B programs must have a main. Execution of the program begins at the first statement of main".[^6] Kernighan used the program again in a 1974 Bell Labs memo on C, and the 1978 book *The C Programming Language*, by Kernighan and Dennis Ritchie, opened with it and made it the custom for books on every later language.

The first program became a measure of how much a language asks of a beginner. Java's classic hello world of 1996 needed a class and `public static void main(String[] args)`. Later languages and versions cut this down: C# added top-level statements in 2020, Java 25 made compact source files with a plain `void main()` final in 2025, and Kotlin, Go and Rust started with a short `main` from their first releases.

## References
