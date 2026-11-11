---
tag: XAWS3V
kind: concept
slug: imperative-programming
name: Imperative programming
aliases: [imperative style, state and assignment, mutable state, side effects, statements, von Neumann style, assignment statement]
area: paradigms/imperative
level: core
summary: >-
  Imperative programming describes a computation as a sequence of statements that change the values of variables, so the program states the steps that lead to the result.
applies_to: core-languages
viz: optional
formal: none
history:
  year: 1957
  people: [John Backus]
  note: Fortran let programmers write assignments and loops as formulas, and imperative languages have followed the design of the stored-program machine since then.
see_also: [structured-programming, functional-programming, object-oriented-programming, parameter-passing, immutability, data-races, undefined-behavior, order-of-evaluation]
same_as:
  wikidata: Q275596
  wikipedia: Imperative_programming
sources:
- url: https://worrydream.com/refs/Backus_1978_-_Can_Programming_Be_Liberated_from_the_von_Neumann_Style.pdf
  title: Can Programming Be Liberated from the von Neumann Style? A Functional Style and Its Algebra of Programs, John Backus, Communications of the ACM 21(8), 1978
  locator: 3 Von Neumann Computers; 4 Von Neumann Languages
- url: https://go.dev/ref/spec#Assignment_statements
  title: The Go Programming Language Specification
  locator: Assignment statements
- url: https://docs.python.org/3.14/reference/simple_stmts.html#assignment-statements
  title: The Python Language Reference, 7. Simple statements, Python 3.14 documentation
  locator: 7.2 Assignment statements
- url: https://docs.oracle.com/javase/specs/jls/se25/html/jls-14.html
  title: The Java Language Specification, Java SE 25 Edition
  locator: Chapter 14 Blocks, Statements, and Patterns
- url: https://doc.rust-lang.org/1.99.1/book/ch03-01-variables-and-mutability.html
  title: The Rust Programming Language, Variables and Mutability, Rust 1.99.1
  locator: Variables and Mutability
- url: https://en.cppreference.com/w/c/language/statements
  title: Statements, C reference, cppreference.com
  locator: introduction
---

## Idea

Imperative programming describes a computation as a sequence of commands that change the state of the program. The state is the set of current values of the variables. Each statement reads some values, computes, and stores results, and the next statement sees the changed state. The program says how to reach the result step by step, in the order the machine will do the steps:

```python run id=idea
balance = 100
history = []

for amount in [20, -50, 30, -120]:
    if balance + amount < 0:
        history.append(f"refused {amount}")
        continue
    balance += amount
    history.append(f"ok {amount}, balance {balance}")

for line in history:
    print(line)
print(balance)
```

```output for=idea
ok 20, balance 120
ok -50, balance 70
ok 30, balance 100
refused -120
100
```

The variable `balance` holds a different value after each pass of the loop. To know its value at one line, a reader must know which statements ran before that line. This is the main property of imperative code, and the source of both its power and its difficulty.

## How it works

**A model of the machine.** A stored-program computer has a memory of cells and a processor that executes instructions one after another. Each instruction reads cells, computes, writes a cell, or jumps. Imperative languages give names to these operations. John Backus described the match in his Turing Award lecture: "Von Neumann programming languages use variables to imitate the computer's storage cells; control statements elaborate its jump and test instructions; and assignment statements imitate its fetching, storing, and arithmetic."[^1] This is why imperative code maps directly to machine code and why it was the first style of high-level language.

**Variables and assignment.** A variable is a named location that holds a value. Assignment changes the value. The Go specification states that "An assignment replaces the current value stored in a variable with a new value specified by an expression."[^2] Languages differ in what a variable holds. In C, a variable is a block of memory, and assignment copies bytes into it. In Python, Java and JavaScript, a variable of an object type holds a reference, and the Python reference says that assignment statements are used "to (re)bind names to values and to modify attributes or items of mutable objects".[^3] Two names can then refer to one object, and a change through one name is visible through the other. This is aliasing.

**Statements and sequence.** A statement is executed for its effect. The Java specification says that "The sequence of execution of a program is controlled by statements, which are executed for their effect and do not have values."[^4] The order of statements is part of the meaning of the program. Swapping two assignments that touch the same variable changes the result, which is not true of two independent expressions.

**Control flow.** Conditionals select which statements run, loops repeat statements while a condition holds, and calls run a procedure and return. Early languages used jumps to labels for all of these. [Structured programming](/structured-programming/) replaced free jumps with nested blocks, and most imperative code is written in that discipline today.

**Procedures and side effects.** A procedure can change variables that its caller can see: a global variable, an object passed by reference, or a location through a pointer. These changes are side effects. They let a procedure update a large structure in place, but they also mean that a call can change state that the call site does not name. Languages add marks to make this visible, such as `ref` in C#, `&mut` in Rust and pointer arguments in Go and C. See [parameter passing](/parameter-passing/).

**Reasoning about state.** To prove that imperative code is correct, a reader states what must be true before and after each statement. C. A. R. Hoare made this precise in 1969 with rules of the form {P} S {Q}: if P holds before statement S, then Q holds after it. A loop needs an invariant, a condition that is true before and after each pass. Informal versions of the same idea are assertions and the comments that state what a loop keeps true.

## In languages

| Language | Verdict | Mechanism |
|---|---|---|
| Python | native | assignment binds names; mutable lists, dicts and objects; `+=` |
| JavaScript | native | `let` and `const`, loops, destructuring assignment |
| TypeScript | native | as JavaScript, with `readonly` checked at compile time |
| Java | native | statements, assignment, arrays; references to objects |
| Kotlin | native | `var` and `val`, loops |
| C# | native | assignment, loops, `ref` and `out` parameters |
| Go | native | assignment, tuple assignment, one `for` loop, pointers |
| Rust | native | `let mut` to opt in to change, `&mut` references |
| C | native | variables as memory, pointers, statements in sequence |
| C++ | native | the statements of C, references, `const` |
| Bash | native | commands in sequence, global variables by default |
| PowerShell | native | statements, variables, loops |
| SQL | idiom | `UPDATE`, `INSERT`, `DELETE`; procedural extensions |
| Regex, HTML, CSS | n/a | no state or statements |

Almost every general-purpose language is imperative at its base. The differences are in how much change each language allows by default and how visible it is. C and C++ allow any variable to change unless it is marked `const`. Java and C# make every local variable and field mutable unless it is marked `final` or `readonly`. Rust reverses the default. Its book says that "by default, variables are immutable", and a variable that changes must be declared `let mut`.[^5] The compiler also refuses to let two parts of a program change one value at the same time, which removes most of the risks of aliasing.

C is the most direct form. cppreference describes C statements as "fragments of the C program that are executed in sequence",[^6] and a pointer gives code the address of any variable. Python, JavaScript and Java hide addresses but keep shared mutable objects, so aliasing is still possible through references. Go has pointers but no pointer arithmetic, and its slices share their arrays, so a function that receives a slice can change the elements of its caller.

## Variations and related ideas

- **Procedural programming.** Imperative code organized into procedures that each do one task, as in C, Pascal and Fortran.
- **Object-oriented programming.** Imperative code in which the state is divided among objects, and each object changes only its own state. See [object-oriented programming](/object-oriented-programming/).
- **Declarative programming.** The program states what result is wanted, not the steps. SQL queries, regular expressions and HTML are declarative. See also [functional programming](/functional-programming/).
- **Local mutation inside a pure interface.** A function can use loops and assignment inside, but take inputs and return a result with no other effect. Functional languages often allow this, for example with the `ST` monad in Haskell.
- **Concurrent state.** When several threads change shared variables, the order of their statements is no longer fixed. See [data races](/data-races/) and [mutual exclusion](/mutual-exclusion/).

## Pitfalls

- **Aliasing.** Two names for one mutable object make a change in one place appear in another. In Python, `[[0] * 3] * 2` holds the same inner list twice.
- **Hidden side effects.** A function that changes a global variable or an argument makes its call sites hard to understand and its results depend on call order. Prefer return values, and make changes visible at the call site.
- **Uninitialized variables.** In C and C++, reading a local variable before assignment is undefined behavior. See [undefined behavior](/undefined-behavior/).
- **Off-by-one errors.** Loops with index variables can run one time too many or too few. With an unsigned index, `n - 1` for an empty array wraps around to a large number.
- **Changing a collection while iterating over it.** Many languages raise an error or skip elements when a loop adds to or removes from the collection that it walks.
- **Order dependence.** Statements that look independent may share state, so a reordering for clarity can change the result.

## Cost

Imperative code is the closest to the machine, so it usually has the most direct cost model. An assignment to a local variable is often a register move, and an in-place update of an array writes one memory cell and allocates nothing. For large data, an in-place change avoids the copy that an immutable update needs. The costs come in other forms. Shared mutable state needs locks or atomic operations when threads use it, and the compiler must assume that a store through one pointer can change the value behind another pointer, which blocks some optimizations. C's `restrict` and Rust's borrow rules give the compiler the guarantee that two references do not overlap.

## History

The first programs were imperative because the machines were. The stored-program design of the 1940s put code and data in one memory and executed instructions in sequence. Assembly languages named the instructions, and Fortran in 1957 let programmers write assignments and loops as formulas. ALGOL 60 added block structure and recursion, and its descendants Pascal and C carried the style to most programmers. Backus, who led the design of Fortran, later argued that these languages were limited by their match to the machine, and he called the assignment statement "the von Neumann bottleneck of programming languages".[^1] Since then, most new languages have stayed imperative at their base but have added limits on change: `const` and `final`, immutable strings, immutable records, and in Rust, immutable variables by default.

## References
