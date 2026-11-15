---
tag: E8WB8B
kind: concept
slug: infinite-loops
name: Infinite loops
aliases: [infinite loop, endless loop, forever loop, while true, for (;;), loop forever, busy loop, event loop, pseudo-infinite loop, hang]
area: errors-and-control/loops
level: core
summary: >-
  An infinite loop never ends by itself. Servers, event loops and game loops use one on purpose and leave it with break or return, and a bug in a loop condition makes one by mistake.
applies_to: core-languages
viz: optional
formal: none
see_also: [loop, while-loop, for-loop, do-while-loop, loop-and-a-half, off-by-one-errors, recursion, control-flow]
same_as:
  wikidata: Q862372
  wikipedia: Infinite_loop
sources:
- url: https://en.wikipedia.org/wiki/Infinite_loop
  title: Infinite loop, Wikipedia
  locator: Introduction; Intended vs unintended looping; Interruption; Language support; Examples of unintentional infinite loops; Pseudo-infinite loops
- url: https://go.dev/ref/spec
  title: The Go Programming Language Specification
  locator: For statements with single condition; Terminating statements
- url: https://doc.rust-lang.org/reference/expressions/loop-expr.html
  title: Loops and other breakable expressions, The Rust Reference
  locator: Infinite loops
- url: https://learn.microsoft.com/en-us/dotnet/csharp/language-reference/statements/iteration-statements
  title: Iteration statements, C# language reference
  locator: The for statement
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Statements/for
  title: for, MDN Web Docs
  locator: Optional for expressions
- url: https://docs.oracle.com/javase/specs/jls/se25/html/jls-14.html
  title: The Java Language Specification, Java SE 25 Edition, Chapter 14
  locator: 14.22 Unreachable Statements
- url: https://en.cppreference.com/w/cpp/language/for.html
  title: for loop, C++ reference, cppreference.com
  locator: Notes
---

## Idea

Most loops must end, but some must not. A web server waits for the next request, a game draws the next frame, and an operating system waits for the next key press, for as long as the machine runs. These programs use an infinite loop on purpose, and they leave it with `break` or `return`, or they never leave it. In other programs an infinite loop is a bug: a condition that never becomes false makes the program hang.

```python run id=idea
# An intended infinite loop: an event loop that runs until a quit event.
events = iter(["click", "key", "quit", "click"])
while True:
    event = next(events)
    if event == "quit":
        print("quit")
        break
    print("handle", event)

# An unintended one: the float never equals 1.0, so a guard stops the loop.
x, passes = 0.0, 0
while x != 1.0:
    x += 0.1
    passes += 1
    if passes == 100:
        print("gave up after", passes, "passes, x =", round(x, 1))
        break

# Counting with an integer ends after the right number of passes.
passes = 0
for i in range(10):
    passes += 1
print("integer loop:", passes, "passes")
```

```output for=idea
handle click
handle key
quit
gave up after 100 passes, x = 10.0
integer loop: 10 passes
```

The first loop has the condition `True`, so only the `break` can end it, and it ends at the quit event. The second loop adds 0.1 ten times, but the sum is 0.9999999999999999, not 1.0, so the condition `x != 1.0` stays true and the loop would run forever. The guard of 100 passes stops it. The third loop counts with an integer and ends after exactly ten passes.

## How it works

**The definition.** Wikipedia defines an infinite loop as "a sequence of instructions in a computer program which loops endlessly". The cause is a loop with no condition to stop, a condition that "can never be met, or one that causes the loop to start over"[^1]. It notes that "Busy wait loops are also sometimes called "infinite loops""[^1].

**Intended infinite loops.** Some programs must run until the machine stops. "Modern interactive computers require that the computer constantly be monitoring for user input or device activity, so at some fundamental level there is an infinite processing idle loop that must continue until the device is turned off or reset"[^1]. The games of cartridge consoles "typically have no exit condition in their main loop, as there is no operating system for the program to exit to"[^1]. In a program with threads, "some threads can be executing inside infinite loops without causing the entire program to be stuck"[^1], and they wait for input from a socket or a queue.

**The forms.** "Some languages have special constructs for infinite loops, typically by omitting the condition from an indefinite loop"[^1]. Wikipedia names `loop ... end loop` in Ada, `DO ... END DO` in Fortran, `for { ... }` in Go, `loop do ... end` in Ruby and `loop { ... }` in Rust.

- In Go, a `for` with no condition loops forever: "If the condition is absent, it is equivalent to the boolean value true"[^2].
- In Rust, "A loop expression repeats execution of its body continuously"[^3].
- In C, `for (;;)` "is traditional, appearing in the standard reference The C Programming Language, and is often pronounced "forever.""[^1]. "In the descendant languages of C, such as C++, Java, and C#, it is more idiomatic to use while (true)"[^1]. C# documents `for ( ; ; )` as "the infinite for loop"[^4].
- Python and JavaScript use `while True:` and `while (true)`.

**The exits.** A loop with no condition is not always infinite. The presence of `for (;;)` "does not guarantee an infinite loop since the loop can be terminated by breaking using break, returning using return, or jumping using goto"[^1]. An exception also leaves the loop. MDN warns that if the condition of a `for` is left out, "you must make sure to break the loop in the body in order to not create an infinite loop"[^5].

**Compilers know about infinite loops.** The type checkers of Java, Go and Rust treat a loop with no condition as code that does not fall through to the next statement.

- In Java, "A while statement can complete normally iff" it is reachable "and the condition expression is not a constant expression (§15.29) with value true", or "There is a reachable break statement that exits the while statement"[^6]. So a method can end with `while (true)` and no `return` after it.
- In Go, a `for` statement is a terminating statement when "there are no "break" statements referring to the "for" statement, and the loop condition is absent, and the "for" statement does not use a range clause"[^2].
- In Rust, "A loop expression without an associated break expression is diverging and has type !"[^3], so it fits where any type is expected.
- In C++, a loop with no observable behavior, such as output, must end. The behavior of such a loop that does not end is undefined, and "Compilers are permitted to remove such loops"[^7]. A loop that waits for a variable to change with no other work can then vanish from the program.

**Unintended infinite loops.** "Most often, the term is used for those situations when this is not the intended result; that is, when this is a bug"[^1]. The causes are many:

- The body does not change the condition, or it resets the value that the condition tests.
- In C, `if (a = 5)` in the body assigns 5 instead of comparing, "Thus, a will never be able to advance to 10, and this loop cannot terminate"[^1].
- A floating-point test such as `x != 1.1` fails because 0.1 has no exact binary form. "it is safer to use greater-than or less-than tests when dealing with floating-point values"[^1].
- "Improperly formed links can create a reference loop in the data structure, where one node links to another that occurs earlier in the sequence"[^1], and a walk over a linked list then never ends.
- Two programs can loop together. In an email loop, two automatic replies answer each other again and again.

**No tool finds all of them.** "While most infinite loops can be found by close inspection of the code, there is no general method to determine whether a given program will ever halt or will run forever; this is the undecidability of the halting problem"[^1]. Linters catch simple cases, such as a constant condition with no `break`, but no checker can catch all of them.

**Stopping a stuck program.** "As long as the system is responsive, infinite loops can often be interrupted by sending a signal to the process (such as SIGINT in Unix)"[^1], for example with Control-C in a terminal or with the `kill` command.

## In languages

| Language | Verdict | Mechanism |
|---|---|---|
| Python | idiom | `while True:` with `break` or `return`; `itertools.count()` is an endless iterator |
| JavaScript | idiom | `while (true)` or `for (;;)`; a generator with `while (true)` and `yield` |
| TypeScript | idiom | the loops of JavaScript; a function that never returns has the type `never` |
| Java | idiom | `while (true)` or `for (;;)`; a method can end with the loop and no `return` |
| Kotlin | idiom | `while (true)`; a function that never returns has the type `Nothing` |
| C# | idiom | `while (true)` or `for (;;)` |
| Go | native | `for { }` with no condition; it is a terminating statement if it has no `break` |
| Rust | native | `loop { }`; without `break` it has the type `!` |
| C | idiom | `for (;;)`, read "forever", or `while (1)` |
| C++ | idiom | `for (;;)` or `while (true)`; a loop with no observable behavior must end |
| Bash | idiom | `while true; do ...; done` or `while :; do ...; done` |
| PowerShell | idiom | `while ($true) { }` |
| SQL | idiom | no loops in queries; a recursive common table expression with no stop condition runs until a limit |
| Regex | idiom | no loops; nested quantifiers can make the matcher run for a very long time |
| HTML | n/a | no loops |
| CSS | n/a | no loops; `animation-iteration-count: infinite` repeats an animation |

Go and Rust have a statement for an infinite loop, and their compilers use it in the rules for return values. The other languages write the loop with a constant condition. The main difference is what the compiler knows: Java, C#, Go and Rust accept a function that ends in an infinite loop without a `return`, and C++ lets the compiler remove a loop that does nothing observable.

The example files show these differences. Python loops over the endless `itertools.count()` and finds a cycle in a linked list. JavaScript takes values from an endless generator and leaves two endless loops with a labeled `break`. Java ends a method with `while (true)` and no `return`, and shows a `byte` that is always under 200. C# runs jobs until a stop job and turns a wrap-around into an exception with `checked`. Go ends a function with `for` and reads a channel until it closes. Rust ends a function with `loop`, stops a counter with `checked_add` and gets a value from `break`. C uses `for (;;)`, a wrapping counter and a cycle check. C++ bounds an endless range with `take_while` and reads numbers with `for (;;)`.

## Variations and related ideas

- **Event loops.** A GUI, a browser and a server run one endless loop that waits for an event, handles it and waits again.
- **Endless sequences.** Generators, `itertools.count()` in Python and `std::views::iota(1)` in C++ produce values without end. They are safe because they make a value only on request, and the consumer stops.
- **Infinite recursion.** "Infinite loops can also be created using recursion"[^1]. Without tail calls, infinite recursion soon ends with a stack overflow, not a hang. See [recursion](/recursion/).
- **Pseudo-infinite loops.** "A pseudo-infinite loop is a loop that appears infinite but is really just a very long loop"[^1]. `for (unsigned int i = 1; i != 0; i++)` ends when `i` wraps around to 0.
- **Busy waiting.** A loop that tests a flag again and again uses a whole processor while it waits. Blocking calls, condition variables and sleeps are cheaper.

## Pitfalls

- **A loop that never changes its condition.** Check that each pass moves the state toward the exit: a counter steps, a list gets shorter, an input advances.
- **Floating-point equality.** `while (x != 1.0)` can miss 1.0. Test with `<` or `<=`, or count with an integer.
- **Narrow counters.** A `byte` or `unsigned char` counter can never reach 256, so `for (b = 0; b < 256; b++)` never ends.
- **Retry loops with no limit.** A loop that retries a failed request forever can hang a program and overload a server. Count the tries and wait longer after each failure.
- **Cycles in data.** A walk over a graph or a linked list that does not mark the visited nodes can loop forever. Keep a set of visited nodes or use a cycle check.
- **Empty busy loops in C++.** A loop such as `while (!ready) {}` on a plain variable has no observable behavior, so the compiler can assume it ends. Use an atomic variable.

## Cost

An infinite loop that does work is the normal form of a long-running program. A loop that waits by testing a flag again and again is expensive: "infinite loops usually cause the program to consume all available processor time, but can usually be terminated by a user"[^1]. With cooperative multitasking, in older operating systems, "infinite loops normally caused the entire system to become unresponsive"[^1], because the loop never gave control back. Event loops avoid this cost: they block in the operating system until an event comes.

## History

Early programs repeated code with a jump back. Wikipedia shows the BASIC form `10 PRINT "INFINITE LOOP"` and `20 GOTO 10`, and notes that "in unstructured programming this is jump back up (goto), while in structured programming this is an indefinite loop (while loop) set to never end"[^1]. The Apollo Guidance Computer had an endless outer loop in its Exec program, and "if the computer had absolutely no other work to do, it would loop-run a dummy job that would simply turn off the "computer activity" indicator light"[^1]. Later languages added a statement for the case: `loop` in Ada and Rust, and `for { }` in Go.

## References
