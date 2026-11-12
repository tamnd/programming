---
tag: QY41GZ
kind: concept
slug: program-state
name: Program state
aliases: [state, program state, state of a program, stateful, stateless, state variable, mutable state, global state, execution state, machine state]
area: foundations/what-a-program-is
level: core
summary: >-
  The state of a running program is everything it remembers at one moment: the values of its variables and objects, the place it has reached in the code, and its open files.
applies_to: core-languages
viz: optional
formal: none
history:
  year: 1936
  people: [Alan Turing, John von Neumann, Robert Floyd, Tony Hoare, Harold Abelson, Gerald Jay Sussman]
  note: Turing's machine of 1936 has a finite set of states and a tape, and the stored-program computer of the late 1940s kept code and data in one memory. Floyd in 1967 and Hoare in 1969 reasoned about programs through statements about the state at each point.
see_also: [side-effect, computer-program, finite-state-machines, immutability, stack-and-heap, iterators, state-pattern, object-serialization]
same_as:
  wikidata: Q599031
  wikipedia: State_(computer_science)
sources:
- url: https://mitp-content-server.mit.edu/books/content/sectbyfn/books_pres_0/6515/sicp.zip/full-text/book/book-Z-H-20.html
  title: Harold Abelson and Gerald Jay Sussman, Structure and Interpretation of Computer Programs, second edition, MIT Press, 1996
  locator: 3.1 Assignment and Local State
- url: https://docs.python.org/3/library/sys.html
  title: sys, System-specific parameters and functions, Python documentation
  locator: sys.settrace
- url: https://docs.python.org/3/glossary.html
  title: Glossary, Python documentation
  locator: generator iterator
- url: https://en.cppreference.com/w/c/language/storage_class_specifiers.html
  title: Storage-class specifiers, C reference, cppreference.com
  locator: Storage duration, static
- url: https://en.cppreference.com/w/c/string/byte/strtok.html
  title: strtok, strtok_s, cppreference.com
  locator: Notes
- url: https://doc.rust-lang.org/book/ch18-03-oo-design-patterns.html
  title: Implementing an Object-Oriented Design Pattern, The Rust Programming Language
  locator: Encoding States and Behavior as Types
- url: https://learn.microsoft.com/en-us/dotnet/csharp/iterators
  title: Iterators, C# guide
  locator: Produce elements with yield return
---

## Idea

A running program remembers things. At each moment it has values in its variables, objects on the heap, a call stack that records which functions are active, a position in the code that says what runs next, and resources outside memory, such as open files and network connections. All of this is the state of the program. Each step of the run reads the state and changes a part of it, so a run is a sequence of states. The same code gives a different result when it starts from a different state, and most bugs are states that the programmer did not expect.

```python run id=idea
import sys


def digit_sum(n):
    total = 0
    while n > 0:
        total += n % 10
        n //= 10
    return total


def show(frame, event, arg):  # called by Python before each line of digit_sum
    if frame.f_code is digit_sum.__code__:
        if event == "line":
            line = frame.f_lineno - digit_sum.__code__.co_firstlineno
            print(f"before line {line}: {dict(frame.f_locals)}")
        return show
    return None


sys.settrace(show)
result = digit_sum(472)
sys.settrace(None)
print("result:", result)


def counter():  # a generator keeps its local state between calls
    count = 0
    while True:
        count += 1
        yield count


c = counter()
print("next:", next(c), next(c), next(c))
print("suspended frame locals:", c.gi_frame.f_locals)
```

```output for=idea
before line 1: {'n': 472}
before line 2: {'n': 472, 'total': 0}
before line 3: {'n': 472, 'total': 0}
before line 4: {'n': 472, 'total': 2}
before line 2: {'n': 47, 'total': 2}
before line 3: {'n': 47, 'total': 2}
before line 4: {'n': 47, 'total': 9}
before line 2: {'n': 4, 'total': 9}
before line 3: {'n': 4, 'total': 9}
before line 4: {'n': 4, 'total': 13}
before line 2: {'n': 0, 'total': 13}
before line 5: {'n': 0, 'total': 13}
result: 13
next: 1 2 3
suspended frame locals: {'count': 3}
```

`sys.settrace` asks Python to call a function before each line, and the function prints the local variables of `digit_sum` at that moment. The output is the run as a sequence of states: each line reads `n` and `total` and changes one of them, until the loop condition is false. The line number is part of the state too, because the same values at a different line lead to a different next step. The generator `counter` shows state that outlives a call: after each `yield`, the frame of the function stays in memory with `count` in it, and the next call goes on from there.

## How it works

**What the state contains.** At the level of the machine, the state is the contents of the registers, including the program counter, and the contents of memory. At the level of a language, it is the values of all variables that exist, the objects that they reach, the call stack with the return point of each active call, and the position in the current function. The state also includes things outside the process that the program can observe: files, the database, the environment and the clock. A debugger shows the language-level state, and Python's `sys.settrace` gives the same view to a program, which "allows you to implement a Python source code debugger in Python."[^2]

**State changes with time.** In mathematics, a function gives the same value for the same argument. A program with state does not. *Structure and Interpretation of Computer Programs* says that an object has state "if its behavior is influenced by its history", and goes on: "We can characterize an object's state by one or more state variables, which among them maintain enough information about history to determine the object's current behavior."[^1] A bank account does not store all its deposits and withdrawals to answer "can I withdraw 100?"; it stores the balance, which summarizes the history.

**Where state lives.** Each kind of storage gives state a lifetime. Local variables live while their call is active. Objects on the heap live while something refers to them. Global and static variables live for the whole run; in C, static storage duration means "The storage duration is the entire execution of the program, and the value stored in the object is initialized only once, prior to main function."[^4] Closures keep the variables they capture alive after the enclosing call returns. Generators and iterators keep their whole frame: in Python, "Each yield temporarily suspends processing, remembering the execution state (including local variables and pending try-statements)."[^3] C# iterator methods also keep their locals and their position between calls,[^7] and the compiler turns each one into a class whose fields hold them. State that must survive the end of the process goes to files or databases, usually through [serialization](/object-serialization/).

**Who can change the state.** The fewer places that can change a piece of state, the easier it is to know its value. Local variables can change only inside their function. Private fields can change only through the methods of their class, which can keep the state valid: a method rejects a withdrawal that would make the balance negative. Global variables and hidden static variables can change anywhere, and every function that uses them depends on the order of all calls. C's `strtok` is a known case: "Each call to strtok modifies a static variable: is not thread safe."[^5] [Side effects](/side-effect/) are the operations that change state.

**State machines and valid states.** When an object has a few modes, such as idle, running and done, the mode is part of its state, and each event moves it to a new mode or is invalid in the current one. Code can store the mode in an enum and check each transition, or it can give each mode its own type. The Rust book shows how to "encode the states into different types", so that invalid transitions are errors when the program compiles.[^6] A sum type, such as a Rust enum or a C++ `std::variant`, stores only the data that each state needs, so a state such as "ready, but with no data" cannot exist.

**Reasoning about state.** To show that code is correct, a programmer states a condition on the state at a point in the code, such as "`total` is the sum of the digits removed so far". An invariant is a condition that holds every time the program reaches a point, such as the start of a loop or the end of each public method. Robert Floyd and Tony Hoare built proofs of programs on such conditions. Tests use the same idea: set up a state, run the code, and check the new state.

## In languages

| Language | Verdict | Mechanism |
|---|---|---|
| Python | native | variables, objects, closures, generator frames |
| JavaScript | native | variables, objects, closures, generators |
| TypeScript | native | as JavaScript; `readonly` limits writes |
| Java | native | fields, statics, enums for modes |
| Kotlin | native | `var` and `val`, sealed classes for modes |
| C# | native | fields, statics, iterator state machines |
| Go | native | structs with methods, package variables, closures |
| Rust | native | `let mut`, ownership; states as types |
| C | native | automatic, static and allocated storage |
| C++ | native | objects with invariants; `std::variant` for modes |
| Bash | native | shell variables, the environment, files |
| PowerShell | native | variables with scopes, the environment |
| SQL | native | the tables of the database are its state |
| Regex | n/a | the engine has state; the pattern does not |
| HTML | idiom | form controls and the DOM hold state in a page |
| CSS | idiom | pseudo-classes such as `:hover` and `:checked` select by state |

Every general-purpose language has mutable state, so the verdict is native for all of them. They differ in how they limit who changes it. Rust allows a change only through a `mut` binding or a `&mut` reference, and only one such reference at a time. Java, C#, C++ and Kotlin use private fields and methods that keep invariants. Go relies on unexported fields and convention. Python and JavaScript protect state mostly by convention, with closures and private fields as stronger tools. SQL keeps the state in tables and changes it in transactions, which move the database from one valid state to the next. HTML and CSS are idiom: a page has state in the DOM and in its form controls, and CSS can select elements by that state.

The example files show the kinds of state. Python keeps the state of an account valid in a method and saves it to a file. JavaScript keeps state in closures and in the position of a generator. Java checks the transitions of an enum state machine. C# shows that each enumerator of an iterator has its own state. Go copies the state of a struct to make a snapshot. Rust puts the state of a door in its type, so that the compiler rejects a wrong transition. C shows hidden static state in `strtok` breaking a nested loop. C++ holds one of four states in a `std::variant`.

## Variations and related ideas

- **Stateless code.** A pure function or a stateless service keeps nothing between calls, so it gives the same answer for the same input and can be repeated or run in parallel.
- **Immutable state.** Programs in functional style make a new state instead of changing the old one, and keep the old one for undo, history or other threads. See [immutability](/immutability/).
- **Finite-state machines.** A [finite-state machine](/finite-state-machines/) has a fixed set of states and transitions, a model for parsers, protocols and user interfaces.
- **The state pattern.** The [state pattern](/state-pattern/) gives each state an object with its own behavior.
- **Shared state and concurrency.** State that two threads can change needs locks, atomic operations or message passing.
- **Persistent state.** Files, databases and caches keep state across runs; checkpoints and snapshots save the whole state so a run can restart.

## Pitfalls

- **Hidden global state.** Functions such as `strtok`, `rand` and `setlocale` keep state in static variables, so two uses interfere. Use the versions that take the state as an argument, such as `strtok_r`.
- **Invalid combinations.** Separate flags such as `loading`, `loaded` and `error` allow states that make no sense, such as loading and loaded at once. Use one field for the mode, or a sum type.
- **Stale copies.** A copy of the state does not follow later changes, and a reference to it follows all of them. Choose on purpose, as the Go snapshot does.
- **State that tests leak.** A test that changes a global variable, an environment variable or a file can make the next test pass or fail. Reset the state, or avoid it in the code under test.
- **Order dependence.** Code that works only when one function runs before another depends on state that the reader cannot see. Pass the needed values as arguments.
- **State lost at restart.** State in memory is gone when the process ends or crashes. Save what must survive, and design the program to start from the saved state.

## Cost

State costs memory, and each kind of storage has its cost: locals on the stack cost almost nothing, heap objects cost an allocation and later a free or a garbage collection, and state on disk costs I/O. The larger cost is in reasoning. The number of possible states grows with the product of the values of each variable, so ten independent flags give 1,024 states to test. Shared mutable state between threads costs synchronization. Copying state to keep it immutable costs time and memory, which persistent data structures reduce by sharing the unchanged parts.

## History

Alan Turing's machine of 1936 made state part of the definition of computation: the machine has a finite set of internal states, and the tape holds the rest. The stored-program computers of the late 1940s, described in John von Neumann's report on the EDVAC in 1945, kept instructions and data in one memory, and a program changed that memory step by step. Assembly languages and the first high-level languages, such as FORTRAN, were built on assignment to variables.

Robert Floyd in 1967 and Tony Hoare in 1969 gave ways to prove programs correct by stating conditions on the state before and after each statement. Object-oriented languages from Simula in 1967 grouped state with the code that changes it, and Abelson and Sussman's book of 1985 made "assignment and local state" a main topic.[^1] Functional programming, from Lisp to Haskell, has tried to limit mutable state, and modern languages such as Rust make the rules about who changes state part of the type system.

## References
