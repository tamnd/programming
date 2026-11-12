---
tag: YXD2DJ
kind: concept
slug: control-flow
name: Control flow
aliases: [control flow, flow of control, order of execution, control structure, control flow statement, sequence selection iteration, branch, jump, abrupt completion, normal completion]
area: errors-and-control/control-structures
level: core
summary: >-
  Control flow is the order in which a program runs its statements. Sequence, selection, iteration, jumps, calls and exceptions are the ways to change that order.
applies_to: core-languages
viz: optional
formal: none
history:
  year: 1966
  people: [Corrado Böhm, Giuseppe Jacopini]
  note: Böhm and Jacopini showed that sequence, selection and iteration are enough to write any computable function, which became the basis of structured control flow.
see_also: [structured-programming, exception-handling, pattern-matching, short-circuit-evaluation, defer-and-finally, iterators]
same_as:
  wikidata: Q868299
  wikipedia: Control_flow
sources:
- url: https://en.wikipedia.org/wiki/Control_flow
  title: Control flow, Wikipedia
  locator: Introduction
- url: https://en.wikipedia.org/wiki/Structured_program_theorem
  title: Structured program theorem, Wikipedia
  locator: Introduction; Bibliography
- url: https://craftinginterpreters.com/control-flow.html
  title: Control Flow, Crafting Interpreters, Robert Nystrom
  locator: 9.1 Turing Machines (Briefly); 9.2 Conditional Execution
- url: https://en.cppreference.com/w/c/language/statements.html
  title: Statements, C reference, cppreference.com
  locator: Introduction
- url: https://learn.microsoft.com/en-us/dotnet/csharp/language-reference/statements/jump-statements
  title: Jump statements, C# language reference
  locator: Introduction
- url: https://docs.oracle.com/javase/specs/jls/se25/html/jls-14.html
  title: The Java Language Specification, Java SE 25 Edition, Chapter 14
  locator: Introduction; 14.1 Normal and Abrupt Completion of Statements
- url: https://go.dev/ref/spec
  title: The Go Programming Language Specification
  locator: Statements; Terminating statements
- url: https://docs.python.org/3/reference/compound_stmts.html
  title: Compound statements, The Python Language Reference
  locator: Introduction
- url: https://doc.rust-lang.org/book/ch03-05-control-flow.html
  title: Control Flow, The Rust Programming Language
  locator: Introduction
- url: https://www.cs.utexas.edu/~EWD/transcriptions/EWD02xx/EWD215.html
  title: A Case against the GO TO Statement (EWD215), Edsger W. Dijkstra
  locator: Introduction; transcriber's note
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Guide/Control_flow_and_error_handling
  title: Control flow and error handling, MDN Web Docs
  locator: Introduction
---

## Idea

A program is a list of statements, but it does not always run them from top to bottom once. It skips some statements, runs others many times, leaves a loop early, calls a function and comes back, or jumps to an error handler. Control flow is the order in which the statements actually run, and the control structures of a language are the tools that choose that order.

```python run id=idea
def classify(numbers):
    trace = []
    for n in numbers:  # iteration: run the body once for each number
        if n < 0:  # selection: choose one of two paths
            trace.append(f"{n} skip")
            continue  # jump to the next iteration
        if n == 0:
            trace.append("0 stop")
            break  # jump out of the loop
        trace.append(f"{n} keep")
    return trace  # jump back to the caller


print(classify([3, -1, 4, 0, 5]))


def parse(text):
    try:
        value = int(text)
        print("parsed", value)  # skipped when int() raises
        return value
    except ValueError:
        print("not a number:", text)  # the exception jumps here
        return None
    finally:
        print("finally runs on every path")


parse("42")
parse("4x2")
```

```output for=idea
['3 keep', '-1 skip', '4 keep', '0 stop']
parsed 42
finally runs on every path
not a number: 4x2
finally runs on every path
```

`classify` uses the main forms of control flow. The `for` loop runs its body once for each number, the `if` statements choose a path, `continue` jumps to the next number, `break` leaves the loop at `0`, so `5` is never seen, and `return` gives control back to the caller. In `parse`, the call `int("4x2")` raises an exception, so control jumps over the rest of the `try` block to the `except` block. The `finally` block runs on both paths.

## How it works

**Sequence and transfer.** Wikipedia says that "control flow (or flow of control) describes how execution progresses from one command to the next"[^1]. By default, control goes to the next statement, "except when a command transfers control to another point"[^1]. At the machine level, "a control flow instruction often alters the program counter and is either an unconditional branch (a.k.a. jump) or a conditional branch"[^1]. The control structures of a high-level language compile to such branches.

**Three structures are enough.** The structured program theorem states that any computable function can be computed with "only the following three control structures"[^2]: sequence, "Executing one subprogram, and then another subprogram"; selection, "Executing one of two subprograms according to the value of a boolean expression"; and iteration, "Repeatedly executing a subprogram as long as a boolean expression is true"[^2]. Crafting Interpreters says the same in terms of jumps: "Conditional or branching control flow is used to not execute some piece of code", and "Looping control flow executes a chunk of code more than once. It jumps back so that you can do something again"[^3]. With arithmetic, a little control flow and unbounded memory, a language is Turing-complete[^3].

**The forms in real languages.** Languages add more forms to the three, because they make programs shorter and clearer:

- selection: `if`, `else if`, `switch`, `match` and the conditional expression `c ? a : b`,
- iteration: `while`, `do while`, `for`, for-each and `loop`,
- jumps: `break`, `continue`, `return`, `goto` and labeled `break`,
- calls: a function call jumps to the function and a return jumps back,
- exceptions and panics: a `throw` jumps to the nearest matching handler, through any number of calls,
- suspension: `yield` and `await` stop a function and resume it later.

The C reference names five kinds of statements: "compound statements", "expression statements", "selection statements", "iteration statements" and "jump statements"[^4]. In C#, "The jump statements unconditionally transfer control"[^5].

**Normal and abrupt completion.** Java describes control flow with a precise rule. "The sequence of execution of a program is controlled by statements"[^6]. A statement completes normally, or it completes abruptly, and "An abrupt completion always has an associated reason"[^6]: a `break`, a `continue`, a `return`, a `throw` or a `yield`. A `try` statement with `finally` catches every abrupt completion, runs its cleanup and then continues the same completion. Go has a similar rule: "A terminating statement interrupts the regular flow of control in a block"[^7], and the compiler uses it to check that a function always returns.

**Statements and expressions.** In C, Java and Python, most control structures are statements: "The if, while and for statements implement traditional control flow constructs"[^8]. In Rust, `if`, `match` and `loop` are expressions, so they give a value. The Rust book says that "The most common constructs that let you control the flow of execution of Rust code are if expressions and loops"[^9].

## In languages

| Language | Verdict | Mechanism |
|---|---|---|
| Python | native | `if`, `while`, `for`, `match`, `try`, `with`; `else` on loops; generators with `yield` |
| JavaScript | native | `if`, `switch`, loops, labels, `try`; promises and `async` functions for later steps |
| TypeScript | native | the statements of JavaScript; the compiler narrows types along each path |
| Java | native | statements with normal and abrupt completion; `switch` with arrows; checked exceptions |
| Kotlin | native | `if`, `when` and `try` are expressions; labeled `break` and `return` |
| C# | native | `if`, `switch`, loops, `goto case`; no fallthrough; `yield return` iterators |
| Go | native | `if`, `switch`, one `for` loop, `goto`, `defer`, `panic` and `recover` |
| Rust | native | `if`, `match` and `loop` are expressions; `break` with a value; the `?` operator |
| C | native | selection, iteration and jump statements; `goto`; `setjmp` and `longjmp` |
| C++ | native | the statements of C; exceptions that run destructors; `[[fallthrough]]` |
| Bash | native | `if`, `case`, `while`, `for`, `break`, `continue`; the exit status decides each branch |
| PowerShell | native | `if`, `switch`, loops, labeled `break`, `try`, `catch`, `finally` and `trap` |
| SQL | idiom | a query is declarative and has no order of statements; `CASE` chooses a value, and procedural extensions add loops |
| Regex | idiom | the engine tries alternatives and backtracks, and `\|`, `?`, `*` and `+` choose and repeat |
| HTML | n/a | a document has no execution order of statements |
| CSS | n/a | style rules have no control flow; the cascade decides which declaration wins |

All general-purpose languages have sequence, selection and iteration, so the differences are in the details: which structures are expressions, whether a `switch` case falls through, and how a program leaves a block early.

The example files show these details. Python uses `else` on a loop, shows that `finally` runs before a `return` finishes, and shows how a generator gives control back to its caller at each `yield`. JavaScript shows a `switch` with two cases that share one body, and the order of synchronous code, promise callbacks and timers. Java uses a `switch` expression with arrows, an exception that moves control to a `catch` block, `do while`, `break` and `continue`. C# uses `goto case`, a lazy iterator and a `switch` expression. Go shows the last-in, first-out order of `defer`, a `panic` that `recover` stops, `fallthrough` and `goto`. Rust uses `if`, `loop` and `match` as expressions and returns early with `?`. C shows `switch` fallthrough, the `goto` cleanup idiom and a non-local jump with `longjmp`. C++ shows that an exception runs the destructors of each scope that it leaves.

## Variations and related ideas

- **Structured and unstructured flow.** `goto` can jump to any label, so the order of execution can be hard to see from the text. [Structured programming](/structured-programming/) limits a program to blocks with one entry and one exit.
- **Non-local exits.** Exceptions, `longjmp` and labeled `break` leave several levels at once. See [exception handling](/exception-handling/).
- **Control flow in expressions.** `&&`, `||` and `??` decide whether their right operand runs. See [short-circuit evaluation](/short-circuit-evaluation/).
- **Pattern matching.** `match` chooses a branch by the shape of a value. See [pattern matching](/pattern-matching/).
- **Asynchronous flow.** With callbacks, promises and `await`, the next step runs when an event happens, not when the previous statement ends.
- **Control flow graphs.** A compiler represents a function as a graph of basic blocks and the jumps between them, and uses it for optimization and analysis.

## Pitfalls

- **Fallthrough.** In C, C++ and JavaScript, a `switch` case without `break` runs into the next case. C# forbids this, and Go needs the keyword `fallthrough`.
- **Hidden exits.** An exception can leave any call. Code that changes state in several steps must clean up on that path too, with `finally`, `defer` or destructors.
- **`return` in `finally`.** In Java, JavaScript and Python, a `return` in a `finally` block replaces the exception or the value of the `try` block.
- **Deep nesting.** Many levels of `if` make the order hard to follow. Early returns and guard clauses keep the main path flat.
- **Off by one.** A loop that tests `<=` instead of `<` runs one time too many.

## Cost

A branch is cheap, but a processor guesses the direction of each branch to keep its pipeline full, and a wrong guess costs some cycles. Loops over data that give random branch directions are slower than loops with regular branches. Function calls cost a few instructions, and compilers inline small functions to remove them. Exceptions are cheap when no exception is thrown in C++, Java and C#, and expensive when one is.

## History

Machine code had jumps and conditional jumps from the start, and early high-level languages such as FORTRAN had `GO TO` and `IF` statements. In May 1966, Corrado Böhm and Giuseppe Jacopini published "Flow Diagrams, Turing Machines and Languages with Only Two Formation Rules" in the Communications of the ACM[^2], which showed that sequence, selection and iteration are enough. In 1968, Dijkstra wrote: "The go to statement as it stands is just too primitive, it is too much an invitation to make a mess of one's program"[^10]. His letter appeared in Comm. ACM 11 "under the title “Go To Statement Considered Harmful”"[^10]. Languages since then give structured statements first, and keep `goto` or labeled jumps only for special cases. JavaScript, for example, "supports a compact set of statements, specifically control flow statements"[^11].

## References
