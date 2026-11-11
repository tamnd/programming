---
tag: 6P6Q7J
kind: concept
slug: structured-programming
name: Structured programming
aliases: [structured programming, structured control flow, goto, go to statement considered harmful, single entry single exit, labeled break]
area: paradigms/structured
level: core
summary: >-
  Structured programming builds control flow from nested sequences, choices, loops and calls, each with one entry and one exit, so that the text of a program shows the order of its execution.
applies_to: core-languages
viz: optional
formal: none
history:
  year: 1968
  people: [Edsger Dijkstra]
  note: Dijkstra's letter on the go to statement argued that free jumps make the process of a program hard to see from its text.
see_also: [recursion, exception-handling, defer-and-finally, resource-acquisition-is-initialization, lexical-scope]
same_as:
  wikidata: Q223335
  wikipedia: Structured_programming
sources:
- url: https://pic.plover.com/knuth-GOTO.pdf
  title: Structured Programming with go to Statements, D. E. Knuth, Computing Surveys 6(4), 1974
  locator: Introduction and Elimination of go to statements
- url: https://www.cs.utexas.edu/~EWD/transcriptions/EWD02xx/EWD215.html
  title: Go To Statement Considered Harmful, E. W. Dijkstra, Communications of the ACM 11(3), 1968, EWD 215
  locator: full letter
- url: https://docs.python.org/3.14/faq/design.html#why-is-there-no-goto
  title: Design and History FAQ, Python 3.14 documentation
  locator: Why is there no goto?
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Statements/label
  title: Labeled statement, MDN Web Docs
  locator: introduction
- url: https://docs.oracle.com/javase/specs/jls/se25/html/jls-14.html#jls-14.15
  title: The Java Language Specification, Java SE 25 Edition
  locator: 14.15 The break Statement
- url: https://doc.rust-lang.org/1.99.1/reference/expressions/loop-expr.html#loop-labels
  title: The Rust Reference, Loops and other breakable expressions, Rust 1.99.1
  locator: Loop labels
- url: https://go.dev/ref/spec#Goto_statements
  title: The Go Programming Language Specification
  locator: Goto statements
- url: https://learn.microsoft.com/en-us/dotnet/csharp/language-reference/statements/jump-statements
  title: Jump statements, C# language reference, Microsoft Learn
  locator: The goto statement
- url: https://www.kernel.org/doc/html/latest/process/coding-style.html#centralized-exiting-of-functions
  title: Linux kernel coding style
  locator: 7) Centralized exiting of functions
---

## Idea

Structured programming builds the control flow of a program from a few nested forms, each with one entry and one exit: a sequence of statements, a choice between alternatives, a loop, and a call to a subroutine. A program that only uses these forms can be read from top to bottom, and the shape of the text shows the shape of the execution. The alternative is free jumps with `goto` to any label, which can make the order of execution impossible to see from the text. Today most code is structured, and most languages do not let a jump go anywhere. The idea still matters when a program must leave a loop early, handle an error, or release resources on every path:

```python run id=idea
def find(grid, target):
    for r, row in enumerate(grid):
        for c, value in enumerate(row):
            if value == target:
                return r, c
    return None


grid = [[3, 1, 4], [1, 5, 9], [2, 6, 5]]
print(find(grid, 9))
print(find(grid, 7))
```

```output for=idea
(1, 2)
None
```

The search has nested loops and two exits, but each exit is a `return` from the function. The reader knows that control leaves the function at that point, and that no other code can jump into the middle of the loops.

## How it works

**Three forms are enough.** In 1966, Corrado Böhm and Giuseppe Jacopini proved that any flowchart can be rewritten with only sequence, selection and iteration. Knuth summarized the result: "Jacopini showed, in effect, that any program given, say, in flowchart form can be transformed systematically into another program, which computes the same results and which is built up from statements in the original program using only the three basic operations of composition, conditional, and iteration".[^1] The construction can need extra Boolean variables, so the result proves that `goto` is not necessary, not that the result is clearer.

**Static text, dynamic process.** Edsger Dijkstra gave the main argument in his 1968 letter. He wrote that "our intellectual powers are rather geared to master static relations and that our powers to visualize processes evolving in time are relatively poorly developed", and that programmers should therefore "shorten the conceptual gap between the static program and the dynamic process".[^2] With nested blocks, the position in the text tells where the program is in its execution. With free jumps, a reader must trace every path that can reach a label. Dijkstra concluded: "The go to statement as it stands is just too primitive, it is too much an invitation to make a mess of one's program."[^2]

**One entry, one exit.** Each structured form can be treated as a single statement. A reader, a proof or a compiler can reason about a block from its start state to its end state, and then forget the inside of the block. This is the base of the stepwise refinement of a program, and of proofs with preconditions and postconditions.

**Structured exits.** A strict rule of one exit per block makes some code worse, for example a search that must stop on the first match. Knuth argued for a balance: "I argue for the elimination of go to's in certain cases, and for their introduction in others."[^1] Modern languages give restricted jumps that keep the structure: `return`, `break` and `continue`, labeled `break` for nested loops, exceptions for errors, and `finally`, `defer` and destructors for cleanup. Each of these only goes outward, to a point that the text makes clear.

## In languages

| Language | Verdict | Mechanism |
|---|---|---|
| Python | native | blocks, `break`, `continue`, `return`, loop `else`, exceptions; no `goto` |
| JavaScript | native | labeled `break` and `continue`; no `goto` |
| TypeScript | native | as JavaScript |
| Java | native | labeled `break` and `continue`, `finally`; `goto` reserved but unused |
| Kotlin | native | labels such as `break@outer`; no `goto` |
| C# | native | `goto` to a label, `goto case`; no labeled `break` |
| Go | native | labeled `break` and `continue`, `defer`, a restricted `goto` |
| Rust | native | labeled loops and blocks, `break` with a value, `?`; no `goto` |
| C | native | `goto` within a function, single-exit cleanup |
| C++ | native | `goto`, destructors for cleanup |
| Bash | native | `break n`, `continue n`; no `goto` |
| PowerShell | native | labeled loops with `break label`; no `goto` |
| SQL | n/a | no control flow in a query |
| Regex, HTML, CSS | n/a | no statements |

**Python** has no `goto`. The design FAQ states that unrestricted `goto` "could lead to messy “spaghetti” code that was hard to understand and revise", and that "one can also use exceptions to provide a “structured goto” that works even across function calls."[^3]

**JavaScript** and **TypeScript** allow a label on a statement: "A labeled statement is any statement that is prefixed with an identifier. You can jump to this label using a break or continue statement nested within the labeled statement."[^4] **Java** has the same rule. A `break` with a label transfers control "to the enclosing labeled statement".[^5]

**Rust** labels loops and blocks. "A loop expression may optionally have a label", and `break` can give the labeled expression a value.[^6]

**Go** has a `goto`, but with limits. "Executing the "goto" statement must not cause any variables to come into scope that were not already in scope at the point of the goto."[^7] **C#** has `goto`, which "transfers control to a statement that is marked by a label", and `goto case` inside a `switch`.[^8]

**C** has an unrestricted `goto` within one function. The Linux kernel coding style uses it for one purpose: "The goto statement comes in handy when a function exits from multiple locations and some common work such as cleanup has to be done."[^9] **C++** has the same `goto`, but destructors do the cleanup on every exit, so the pattern is rarely necessary.

## Variations and related ideas

**Procedural programming.** Structured control flow inside procedures, and procedures as the unit of decomposition, together are the classic style of Pascal and C.

**Exceptions.** An exception is a structured non-local exit: it goes outward through calls to the nearest handler. See [exception handling](/exception-handling/).

**Cleanup on every path.** `finally`, `defer`, `with` and destructors run code on every exit from a scope, which removes the main reason for `goto` in C. See [defer and finally](/defer-and-finally/) and [RAII](/resource-acquisition-is-initialization/).

**Recursion and higher-order functions.** [Recursion](/recursion/) and functions such as `map` and `filter` replace many loops, and they also have one entry and one exit.

**Unstructured control in a structured form.** Coroutines, generators, `async` functions and continuations can suspend and resume a function in the middle. The compiler turns them into state machines with jumps, but the source code keeps a structured shape.

**State machines.** Some programs, such as lexers and protocol handlers, are naturally graphs of states. A `switch` in a loop, a table of states, or tail calls express them without free jumps.

## Pitfalls

**Flag variables.** A strict rule of one exit can force a Boolean flag that each loop must test. The result is often harder to read than a `return` or a labeled `break`.

**Deep nesting.** Code with many nested `if` statements is structured, but hard to read. Early returns for error cases, called guard clauses, keep the main path at a low level of nesting.

**Cleanup that a jump skips.** In C, an early `return` from the middle of a function can skip the release of memory, locks or files. Use one exit with cleanup labels, or the `defer`, `finally` or destructor of the language.

**A `goto` that crosses a declaration.** C allows a `goto` that skips the initialization of a variable, and the variable then has an indeterminate value. C++ and Go reject such jumps.

**Fall-through in `switch`.** In C, C++ and Java, a `case` without `break` runs into the next `case`. This hidden jump causes bugs. C# forbids it, and newer languages, such as Go, Swift and Rust, do not fall through by default.

## Cost

Structured control has no run-time cost. Compilers turn loops, conditionals and `break` into the same jumps that a `goto` would make. A structured program is also easier for a compiler to analyze and optimize, because its control flow graph is reducible. The cost is sometimes extra code: a flag variable, a duplicated statement or a helper function where a single jump would do.

## History

Early Fortran, COBOL and assembly programs used jumps for most control flow. Algol 60 introduced nested blocks and compound statements. In 1966, Böhm and Jacopini proved that sequence, selection and iteration are enough.[^1] In March 1968, Communications of the ACM published Dijkstra's letter, which the editor titled "Go To Statement Considered Harmful". Dijkstra wrote that "the go to statement should be abolished from all "higher level" programming languages".[^2] Dijkstra, Ole-Johan Dahl and Tony Hoare published the book "Structured Programming" in 1972, and Pascal, designed by Niklaus Wirth, was built for this style. In 1974, Knuth answered the debate with "Structured Programming with go to Statements", which proposed structured exits from loops.[^1] Later languages took this advice: most of them have `break`, `continue`, early `return` and exceptions, and Java, Python, JavaScript and Rust have no `goto` at all.

## References
