---
tag: NEW-assertions
kind: concept
slug: assertions
name: Assertion
aliases: [assert, design by contract, precondition, postcondition, class invariant, contract]
area: errors-and-control/assertions-and-contracts
level: core
summary: >-
  An assertion states a condition that must be true at a point in a program. A false assertion at run time shows a bug. Contracts use assertions as preconditions, postconditions and invariants.
applies_to: core-languages
viz: optional
formal: none
history:
  year: 1992
  people: [Bertrand Meyer]
  note: Meyer's article "Applying Design by Contract" described preconditions, postconditions and class invariants as a contract between a routine and its callers, with Eiffel as the notation.
see_also: [panics, errors-as-values, exception-handling, unit-testing, null-references]
same_as:
  wikidata: Q741248
  wikipedia: Assertion_(software_development)
sources:
  - url: https://se.inf.ethz.ch/~meyer/publications/computer/contract.pdf
    title: Bertrand Meyer, Applying "Design by Contract", IEEE Computer 25(10), October 1992, pages 40 to 51, doi:10.1109/2.161279
    locator: page 42 (Figures 1 and 2, the three kinds of assertion); page 43 (the box Further sources); page 44 (assertion violations as bugs, Who should check?); page 45 (Class invariants, Figure 5); page 46 (Monitoring assertions, Why monitor?); page 48 (redeclaration under inheritance); pages 49 and 50 (failure, exceptions, the rescue clause)
  - url: https://docs.python.org/3.14/reference/simple_stmts.html#the-assert-statement
    title: The Python Language Reference, Simple statements, Python 3.14
    locator: 7.3 The assert statement
  - url: https://docs.python.org/3.14/using/cmdline.html#cmdoption-O
    title: Python documentation, Command line and environment, Python 3.14
    locator: the option -O
  - url: https://docs.oracle.com/javase/specs/jls/se25/html/jls-14.html#jls-14.10
    title: The Java Language Specification, Java SE 25 Edition
    locator: 14.10 The assert Statement, including the discussion at the end of the section
  - url: https://docs.oracle.com/en/java/javase/25/docs/specs/man/java.html
    title: The java Command, JDK 25
    locator: -enableassertions (-ea) and -disableassertions (-da)
  - url: https://www.open-std.org/jtc1/sc22/wg14/www/docs/n3220.pdf
    title: ISO/IEC 9899 working draft N3220 (C23)
    locator: 6.7.12 Static assertions; 7.2.1 General (NDEBUG); 7.2.2.1 The assert macro
  - url: https://timsong-cpp.github.io/cppwp/n4950/assertions.assert
    title: Working Draft, Programming Languages, C++ (N4950, the C++23 draft)
    locator: "[assertions.assert]"
  - url: https://learn.microsoft.com/en-us/dotnet/api/system.diagnostics.debug.assert
    title: .NET API reference, Debug.Assert Method
    locator: Remarks ("By default, the Debug.Assert method works only in debug builds") and the attribute ConditionalAttribute
  - url: https://learn.microsoft.com/en-us/dotnet/api/system.diagnostics.conditionalattribute
    title: .NET API reference, ConditionalAttribute Class
    locator: the summary and the Remarks
  - url: https://doc.rust-lang.org/std/macro.assert.html
    title: Rust standard library, macro assert
    locator: the description ("Assertions are always checked in both debug and release builds")
  - url: https://doc.rust-lang.org/std/macro.debug_assert.html
    title: Rust standard library, macro debug_assert
    locator: Uses
  - url: https://nodejs.org/api/assert.html#assertokvalue-message
    title: Node.js documentation, Assert
    locator: assert.ok(value[, message])
  - url: https://jimgray.azurewebsites.net/papers/TandemTR85.7_WhyDoComputersStop.pdf
    title: Jim Gray, Why Do Computers Stop and What Can Be Done About It?, Tandem Technical Report 85.7, June 1985
    locator: page 11, Fault containment through fail-fast software modules
  - url: https://jcp.org/en/jsr/detail?id=41
    title: JSR 41, A Simple Assertion Facility, Java Community Process
    locator: Description and the stage timeline (Final Release, 09 May 2002)
  - url: https://kotlinlang.org/docs/exceptions.html
    title: Kotlin documentation, Exceptions
    locator: Throw exceptions with precondition functions (require, check, error)
  - url: https://go.dev/doc/faq#assertions
    title: Go FAQ
    locator: Why does Go not have assertions?
  - url: https://www.typescriptlang.org/docs/handbook/release-notes/typescript-3-7.html#assertion-functions
    title: TypeScript documentation, TypeScript 3.7 release notes
    locator: Assertion Functions
  - url: https://www.postgresql.org/docs/current/ddl-constraints.html
    title: PostgreSQL documentation, Constraints
    locator: 5.5.1 Check Constraints
  - url: https://timsong-cpp.github.io/cppwp/n4950/structure.specifications
    title: Working Draft, Programming Languages, C++ (N4950, the C++23 draft)
    locator: "[structure.specifications], paragraph 3.3 (Preconditions)"
  - url: https://nodejs.org/api/console.html#consoleassertvalue-message
    title: Node.js documentation, Console
    locator: console.assert(value[, ...message])
  - url: https://devblogs.microsoft.com/typescript/announcing-typescript-3-7/
    title: Daniel Rosenwasser, Announcing TypeScript 3.7, TypeScript blog, 5 November 2019
    locator: Assertion Functions
---

## Idea

A programmer who writes a function makes assumptions. The argument is not negative. The list is sorted. The balance is never below zero. Most of these assumptions stay in the head of the programmer. An assertion writes one of them into the program as a Boolean expression. When the program runs, it can test the expression. If the expression is false, the assumption was wrong, and the program has a bug.

Bertrand Meyer put it this way: "any runtime violation of an assertion is not a special case but always the manifestation of a software bug."[^1] This is the difference between an assertion and an ordinary check. An ordinary check handles a case that can happen, such as a file that is missing. An assertion states a case that must not happen.

In Python, `assert` raises `AssertionError` when its condition is false.

```python run id=idea
def isqrt(n):
    assert n >= 0, "precondition: n >= 0"
    r = 0
    while (r + 1) * (r + 1) <= n:
        r += 1
    assert r * r <= n < (r + 1) * (r + 1), "postcondition"
    return r


print([isqrt(n) for n in [0, 1, 15, 16]])
try:
    isqrt(-4)
except AssertionError as exc:
    print("AssertionError:", exc)
```

```output
[0, 1, 3, 4]
AssertionError: precondition: n >= 0
```

The first assertion states what the caller must give. The second states what the function promises in return. The two together are a small contract.

## How it works

At the level of the machine, an assertion is a comparison and a branch. When the condition holds, the program goes on. When it does not, the program jumps to a failure routine. The languages differ in what that routine does.

| Language | Assertion | On failure | How it is removed |
|---|---|---|---|
| Python | `assert cond, msg` | raises `AssertionError` | the option `-O`[^2][^3] |
| Java | `assert cond : msg;` | throws `AssertionError` | disabled unless the launcher gets `-ea`[^4][^5] |
| C | `assert(cond)` | writes a report to standard error, calls `abort` | define `NDEBUG` before `<assert.h>`[^6] |
| C++ | `assert(cond)` from `<cassert>` | as in C | `NDEBUG`, as in C[^7] |
| C# | `Debug.Assert(cond, msg)` | writes a message and shows the call stack | dropped when the symbol `DEBUG` is not defined[^8][^9] |
| Rust | `assert!(cond, msg)` | panics | never removed[^10] |
| Rust | `debug_assert!(cond, msg)` | panics | not run in optimized builds by default[^11] |
| JavaScript | `assert.ok(value, msg)` from `node:assert` | throws `AssertionError` | never removed[^12] |

### Removal in release builds

Many languages let a build turn the assertions off. The reason is cost: a check that walks a whole data structure can be slower than the work it checks. The ways to turn them off differ.

- **The preprocessor.** In C, if `NDEBUG` is defined where `<assert.h>` is included, `assert` is defined as `((void)0)`.[^6] The expression is gone before the compiler sees it.
- **The compiler.** In Python, `assert` is the same as `if __debug__: if not expression: raise AssertionError`. With `-O`, `__debug__` is `False` and the compiler emits no code for `assert`.[^2] In C#, `Debug.Assert` carries the attribute `Conditional("DEBUG")`.[^8] A call to a method with this attribute is not compiled unless the symbol is defined.[^9]
- **The runtime.** In Java, an assertion is enabled or disabled per class, and a disabled `assert` "does nothing". It does not evaluate its expressions.[^4] Assertions are disabled by default, and the launcher option `-ea` enables them.[^5]
- **The build profile.** Rust keeps `assert!` in every build, and the documentation says that it cannot be disabled.[^10] `debug_assert!` is the version for checks that are too expensive for a release build. An optimized build does not run it unless the compiler gets `-C debug-assertions`.[^11]

The JLS describes the usual practice: assertion checking is enabled during development and testing, and disabled for deployment, to improve performance.[^4] The cost of this practice is that the deployed program does not check the conditions that the tests checked. The pitfalls below show what can go wrong.

### Static assertions

A static assertion is checked by the compiler, not at run time. C has `static_assert(constant-expression, string-literal)`. If the expression is zero, the compiler must produce a diagnostic, which should include the text of the string literal.[^6] A static assertion costs nothing at run time, but it can only test what the compiler knows, such as the size of a type.

### Design by contract

Meyer built a method of software construction on assertions and named it Design by Contract. Its primary source is his 1992 article in IEEE Computer, which uses the language Eiffel.[^1] A routine and its caller are two parties to a contract. The caller is the client, and the routine is the supplier.[^1]

Eiffel attaches the assertions to the routine. The clause `require` holds the precondition, and the clause `ensure` holds the postcondition. In the postcondition, `old child_count` means the value of `child_count` on entry to the call.[^1] A missing precondition is the same as `require True`, the assertion that every state satisfies.[^1]

- **A precondition** is a requirement that each call must satisfy to be correct. It is an obligation of the client.[^1]
- **A postcondition** is a property that the routine ensures in return. It is an obligation of the supplier.[^1]
- **A class invariant** is a property of every instance of a class. Each creation procedure must give an object that satisfies it, and each exported routine must keep it true. In effect, the invariant is added to the precondition and the postcondition of each exported routine. It need not hold in the middle of a routine, only in the states that a client can observe.[^1]

The contract says who has a bug when an assertion fails. "A precondition violation indicates a bug in the client (caller)." "A postcondition violation is a bug in the supplier (routine)."[^1]

#### Who should check

A precondition moves a check to the caller. The routine does not test its own precondition. Meyer calls this "an absolute rule": a condition is either in the `require` clause or in an `if` in the body, "but never in both".[^1] He calls this "the exact opposite of the idea of defensive programming", where each party checks everything.[^1] Gray's fail-fast modules take the other view and are made fail-fast "by defensive programming": they check their inputs, intermediate results, outputs and data structures, and stop when a check fails.[^13] The two views agree that a failed check is a fault. They differ in where the check belongs.

Meyer says that there is "no absolute rule" for how strong a precondition should be. A "demanding" contract has a strong precondition and puts the work on the clients. A "tolerant" one has a weak precondition and puts the work on the routine. Meyer calls the choice "a pragmatic decision about division of labor", and says that "the best solution is the one that achieves the simplest architecture."[^1]

#### Monitoring

In Eiffel, the developer chooses per class how many assertions the program checks at run time: none, preconditions only, preconditions and postconditions, those plus class invariants, or all assertions. Preconditions only is the default. A violation of a monitored assertion raises an exception.[^1] Meyer describes monitoring as "a way to call the developer's bluff", because it checks what the software does against what its author thinks it does.[^1]

#### Contracts and inheritance

A subclass can redeclare a routine. Meyer's rule is that the new version may weaken the precondition and strengthen the postcondition, but not the reverse. Eiffel writes `require else` and `ensure then` in a redeclaration, so the new precondition is the old one or the new one, and the new postcondition is the old one and the new one. The invariant of a class includes the invariants of all its parents, so it is always stronger than or equal to each of them.[^1]

#### Contracts without contract syntax

Most of the tier 1 languages have no `require` or `ensure`. Programs write the contract with the tools that exist. The example files use an account with a `withdraw` method:

- The precondition `0 < amount <= balance` is an `assert` at the start of the method in Python, an exception in Java and C#, and a returned error in Go and Rust.
- The postcondition `balance == old - amount` keeps a copy of the old value in a local variable, because these languages have no `old`.
- The invariant `balance >= 0` is a private method that each public method calls before it returns. In Rust, the type `u32` makes a negative balance impossible, so the invariant needs no check.

## In languages

| Language | Verdict | Form |
|---|---|---|
| Python | native | the statement `assert`; removed by `-O` |
| JavaScript | stdlib | the module `node:assert` in Node.js; `console.assert` only logs |
| TypeScript | idiom | assertion functions with the return type `asserts condition` |
| Java | native | the statement `assert`; disabled unless `-ea` |
| C | stdlib | the macro `assert` and `NDEBUG`; the keyword `static_assert` |
| C++ | stdlib | `assert` from `<cassert>`; the declaration `static_assert` |
| C# | stdlib | `Debug.Assert`, compiled only with `DEBUG`; `Trace.Assert` |
| Go | absent | no assertions; an `if` that returns an error or panics |
| Rust | native | `assert!` in every build, `debug_assert!` in builds without optimization |
| Kotlin | stdlib | `require` and `check` throw exceptions |
| PowerShell | absent | no assertion statement; a script throws when a check fails |
| Bash | absent | no assertion command; a test followed by `exit` |
| SQL | idiom | a `CHECK` constraint states a Boolean condition that each row must satisfy |
| Regex | n/a | a pattern has no assertions in this sense; a lookahead is a different idea |
| HTML/CSS | n/a | markup and style rules state no conditions on a program |

Three groups stand out.

- **A statement in the language.** Python and Java have the keyword `assert`. Both can turn it off, Python at compile time and Java per class at run time.[^2][^4] Java added the statement through JSR 41, "A Simple Assertion Facility", whose final release was on 9 May 2002.[^14]
- **A library function or macro.** C and C++ use a macro, C# a method with a conditional attribute, Rust a macro, and Node.js a module.[^6][^8][^10][^12] Kotlin has the functions `require`, which throws `IllegalArgumentException`, and `check`, which throws `IllegalStateException`.[^15]
- **No assertions on purpose.** The Go FAQ says that Go does not provide assertions. The designers' experience was that programmers use them "as a crutch to avoid thinking about proper error handling and reporting".[^16] A Go program writes an `if` statement and returns an error for a bad argument, or panics for a broken invariant.

TypeScript adds a type-level form. A function whose return type is `asserts condition` tells the checker that the condition is true after the call returns. A return type `asserts value is string` narrows the type of `value`.[^17] The checker trusts the signature and does not check the body (see the TypeScript example).

SQL states conditions on data. A `CHECK` constraint requires that the value in a column satisfy a Boolean expression, and the database rejects a row that does not.[^18]

## Variations and related ideas

- **Assertions in tests.** A unit test calls code and asserts facts about the result. A failed test assertion reports a failed test, not a bug in production ([[unit-testing]]).
- **Assertions as panics.** In Rust and C, a failed assertion is a panic or an abort ([[panics]]). In Python and Java, it is an exception that a handler can catch. The JLS says that `AssertionError` is intended never to be caught.[^4]
- **Rescue and retry.** Eiffel has a `rescue` clause. When a routine fails, the clause can restore a consistent state and execute `retry` to try again, or end without `retry`, and then the routine fails and its caller gets an exception. Meyer calls the second case "organized panic".[^1]
- **Types in place of assertions.** A type can make a bad state impossible. A non-nullable reference removes the need to assert that a pointer is not null ([[null-references]]). An unsigned integer removes the assertion that a count is not negative.
- **Library preconditions in C++.** The C++ standard library specifies preconditions for its functions. A call that violates a precondition has undefined behaviour.[^19] The library does not promise to check it.

## Pitfalls

### Side effects in an assertion

When assertions are removed, their expressions are not evaluated. Code that does work inside an assertion stops doing that work in a release build. The JLS says that the expressions in assertions "should generally be free of side effects".[^4] Here the call to `pop` is inside the `assert`. With `-O`, the call disappears, and the queue keeps its first item.

```python wrong id=side-effect flags="-O"
queue = ["job-1", "job-2"]
assert queue.pop(0) == "job-1", "unexpected first job"
print("next job:", queue[0])
```

```output
next job: job-1
```

The fix is to do the work in a statement of its own and to assert only on the result.

### Assertions that check input

A user, a file or a network can send bad input at any time. Such input is an expected failure, not a bug, so the program must check it in every build. The JLS says that assertions should not check the arguments of public methods, because that check is part of the contract of the method and must hold whether assertions are enabled or not.[^4] The Python example with `-O` shows the result of an assertion used as the only check: the withdrawal goes through and the balance becomes negative. Check input with an ordinary test and an exception or an error value ([[errors-as-values]]).

### An assertion that cannot fail

In Python, `assert (condition, "message")` tests a tuple of two items. A tuple that is not empty is true, so the assertion never fails (see the `tuple` block of the Python example). In JavaScript, `console.assert` only writes a message and "does not otherwise affect execution", so a program that relies on it goes on with bad data.[^20]

### A test of the assertion state

Code that behaves differently when assertions are enabled can pass its tests and fail in production. The tests run with assertions on, and production runs with them off. Run at least some tests with the same settings as the release build.

## Cost

- **Run time.** Each assertion that runs is a comparison and a branch, plus the cost of the expression. An invariant that walks a data structure can turn a constant-time operation into a linear-time one. This is the reason for `debug_assert!` and for removal in release builds.[^11][^4]
- **Removed assertions.** A removed assertion costs nothing at run time. It also catches nothing.
- **Static assertions.** A static assertion costs nothing at run time. The compiler evaluates it once.[^6]
- **Code size.** In C, each failed `assert` writes the text of the expression, the file name, the line and the function name, so these strings are in the program.[^6]

## History

- **1967 and 1969.** Robert Floyd published "Assigning Meanings to Programs" in 1967, and C. A. R. Hoare published "An Axiomatic Basis for Computer Programming" in 1969. Meyer names the work of Floyd, Hoare and Dijkstra on program proving as one of the two primary sources of Design by Contract. He says that the class invariant comes directly from Hoare's data invariants.[^1]
- **1992.** Meyer's article "Applying Design by Contract" in IEEE Computer described preconditions, postconditions, class invariants, assertion monitoring, contracts under inheritance and the `rescue` clause of Eiffel. It says that the use of assertions in an object-oriented language, and the approach to inheritance based on subcontracting, appear original to Eiffel.[^1]
- **2002.** JSR 41 added the `assert` statement to Java. Its final release was on 9 May 2002.[^14]
- **2019.** TypeScript 3.7, announced on 5 November 2019, added assertion functions with the return type `asserts condition`.[^21]

## References
