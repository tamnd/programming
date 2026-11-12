---
tag: QR2VX8
kind: concept
slug: side-effect
name: Side effect
aliases: [side effect, side effects, side-effect, effect, pure function, purity, impure function, referential transparency, observable effect]
area: foundations/computation-in-practice
level: core
summary: >-
  A side effect is a change that a function makes beyond its return value, such as a write to a variable, a file or the screen. It makes the order and the number of calls matter.
applies_to: core-languages
viz: optional
formal: none
history:
  year: 1958
  people: [John McCarthy, Christopher Strachey, Peter Landin, John Hughes, Philip Wadler, Simon Peyton Jones]
  note: Lisp, from 1958, mixed pure functions with assignment. Christopher Strachey described referential transparency in 1967, and Haskell made effects part of the type of a function with monadic I/O in Haskell 1.3, in 1996.
see_also: [immutability, functional-programming, imperative-programming, order-of-evaluation, parameter-passing, laziness, pointers-and-references]
same_as:
  wikidata: Q1144241
  wikipedia: Side_effect_(computer_science)
sources:
- url: https://en.cppreference.com/w/c/language/eval_order.html
  title: Order of evaluation, C reference, cppreference.com
  locator: Definitions, side effects
- url: https://gcc.gnu.org/onlinedocs/gcc-15.2.0/gcc/Common-Function-Attributes.html
  title: Common Function Attributes, Using the GNU Compiler Collection, GCC 15.2
  locator: pure and const
- url: https://www.haskell.org/tutorial/io.html
  title: A Gentle Introduction to Haskell, Input/Output
  locator: Introduction
- url: https://docs.python.org/3/library/stdtypes.html
  title: Built-in Types, Python documentation
  locator: list.sort
- url: https://docs.python.org/3/reference/compound_stmts.html
  title: Compound statements, The Python Language Reference
  locator: Function definitions
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Array/toSorted
  title: Array.prototype.toSorted(), MDN Web Docs
  locator: Description
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/util/stream/package-summary.html
  title: Package java.util.stream, Java SE 25 API documentation
  locator: Side-effects
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/util/stream/Stream.html
  title: Interface Stream, Java SE 25 API documentation
  locator: count()
- url: https://learn.microsoft.com/en-us/dotnet/api/system.diagnostics.contracts.pureattribute
  title: PureAttribute Class, .NET API documentation
  locator: Definition
- url: https://en.cppreference.com/w/cpp/language/eval_order.html
  title: Order of evaluation, C++ reference, cppreference.com
  locator: Rules
---

## Idea

A function has a side effect when calling it does something more than compute a value: it changes a variable outside itself, changes an object that the caller passed in, writes to a file, the screen or the network, or reads input that changes from call to call. A function with no side effects, whose result depends only on its arguments, is pure. A call to a pure function can be replaced by its value, called twice or not called at all, and the program means the same thing. A call with side effects cannot: the number of calls and their order change what the program does.

```python run id=idea
total = 0


def add(a, b):  # no side effect: the result depends only on the arguments
    return a + b


def add_and_count(a, b):  # side effect: it changes a global variable
    global total
    total += a + b
    return total


print("add:", add(2, 3), add(2, 3))
print("add_and_count:", add_and_count(2, 3), add_and_count(2, 3))


def shout(words):  # side effect: it changes the caller's list
    words.append("!")
    return " ".join(words)


w = ["hello", "world"]
print(shout(w))
print(shout(w))
print("caller's list:", w)

log = []


def f(x):  # side effect: it records each call
    log.append(x)
    return x


print("value:", f(1) + f(2) * f(3), "calls in order:", log)

total = 0
y = add_and_count(2, 3)
print("y + y =", y + y, end="; ")
total = 0
print("two calls give", add_and_count(2, 3) + add_and_count(2, 3))
```

```output for=idea
add: 5 5
add_and_count: 5 10
hello world !
hello world ! !
caller's list: ['hello', 'world', '!', '!']
value: 7 calls in order: [1, 2, 3]
y + y = 10; two calls give 15
```

`add` gives 5 every time. `add_and_count` gives 5 and then 10 for the same arguments, because each call changes `total`. `shout` changes the list that the caller owns, so the second call adds a second `"!"`. The function `f` records each call, which makes the order of evaluation visible: Python evaluates the operands from left to right. The last two lines show that a call with a side effect cannot be replaced by its value: storing the result once and adding it to itself gives 10, but two calls give 15.

## How it works

**What counts as a side effect.** The C standard gives an exact list. A side effect is "access (read or write) to an object designated by a volatile lvalue, modification (writing) to an object", atomic synchronization, a change to a file or to the floating-point environment, "or calling a function that does any of those operations."[^1] In general terms, an expression has a side effect when it changes state that some other part of the program, or the world outside it, can observe. Reading changeable state, such as the clock, a global variable or input, is not a side effect in the C sense, but it makes a function impure in the same way, because the result no longer depends only on the arguments.

**Pure functions and referential transparency.** An expression is referentially transparent when it can be replaced by its value without a change in the meaning of the program. Pure functions give this property, and it allows a compiler and a reader to reason about code locally. GCC's `pure` attribute states the condition for C: "Calls to functions that have no observable effects on the state of the program other than to return a value may lend themselves to optimizations such as common subexpression elimination."[^2] .NET has a `PureAttribute` that "Indicates that a type or method is pure, that is, it does not make any visible state changes."[^9] Neither compiler checks that the claim is true.

**Order and sequencing.** Side effects make the order of evaluation part of the meaning. Each language states which effects happen before others. C and C++ define sequence points or the relation "sequenced before", and two unsequenced writes to one object are undefined behavior; C++ has rules such as "The value computations (but not the side effects) of the operands to any operator are sequenced before the value computation of the result of the operator".[^10] Java, C#, JavaScript and Python evaluate operands from left to right. A program that depends on the order of two calls should put them in separate statements.

**Effects through parameters.** A function can change its caller's data when it receives a reference to it: an object in Python, Java, JavaScript or C#, a pointer in C and Go, a slice or a map in Go, a reference in C++, or a `&mut` reference in Rust. In most languages, the signature does not show whether the function changes the argument. Rust does: a `&` reference allows only reads, and a change through it is a compile error. Libraries name the convention. Python's `list.sort` returns `None`: "To remind users that it operates by side effect, it does not return the sorted sequence (use sorted() to explicitly request a new sorted list instance)."[^4] JavaScript added copying methods in ECMAScript 2023: "The toSorted() method of Array instances is the copying version of the sort() method."[^6]

**Effects in lazy code.** Lazy sequences and queries run their functions when a consumer asks for items, not when the code that builds them runs. A side effect in such a function happens at a time, and a number of times, that the reader does not see. Java warns: "Side-effects in behavioral parameters to stream operations are, in general, discouraged, as they can often lead to unwitting violations of the statelessness requirement, as well as other thread-safety hazards."[^7] `Stream.count` can skip the pipeline: "An implementation may choose to not execute the stream pipeline (either sequentially or in parallel) if it is capable of computing the count directly from the stream source."[^8] A LINQ query in C# runs again each time the program enumerates it.

**Controlling effects.** Most languages allow effects anywhere and rely on conventions: immutable values, functions that return new values, and effects at the edges of the program, around a core of pure functions. Haskell makes the separation part of the type system. Its tutorial says that I/O actions "are cleanly separated from the purely functional core of the language", and "A function such as f :: Int -> Int -> Int absolutely cannot do any I/O since IO does not appear in the returned type."[^3] Effect systems in Koka and the effect handlers of OCaml 5 track other kinds of effects in the same way.

## In languages

| Language | Verdict | Mechanism |
|---|---|---|
| Python | native | effects anywhere; `sorted` against `list.sort`; tuples and frozen dataclasses |
| JavaScript | native | effects anywhere; copying methods such as `toSorted`; `Object.freeze` |
| TypeScript | native | as JavaScript; `readonly` types prevent some writes |
| Java | native | effects anywhere; immutable collections; stream rules |
| Kotlin | native | `val`, read-only collection interfaces |
| C# | native | `readonly`, records with `with`, `[Pure]` with no check |
| Go | native | value parameters copy; pointers, slices and maps share |
| Rust | native | `&` and `&mut` show effects on arguments in the signature |
| C | native | `const` pointers; GCC `pure` and `const` attributes |
| C++ | native | `const` references and methods; `mutable` for hidden caches |
| Bash | native | commands act on files and the environment; subshells isolate changes |
| PowerShell | native | cmdlets with effects support `-WhatIf` |
| SQL | native | `SELECT` reads; `INSERT`, `UPDATE` and `DELETE` change data |
| Regex | n/a | a pattern has no side effects |
| HTML | n/a | markup has no evaluation |
| CSS | n/a | rules have no evaluation |

Every general-purpose language on this list allows side effects anywhere in a function, so the verdict is native for each. They differ in how much the code shows. Rust shows effects on arguments in the types, C and C++ show them with the absence of `const`, and Go shows them with pointers, although slices and maps share data with no pointer in sight. Python, Java, JavaScript and C# show nothing in the signature, and the convention of the library tells the reader, as in `sort` against `sorted`. SQL separates queries from statements that change data, and a query is close to a pure function of the tables.

The example files show these points. Python shows `list.sort` against `sorted` and the shared default list. JavaScript shows `toSorted` and the shallow `Object.freeze`. Java shows a read-only view that still changes, and lazy streams that run a `peek` late or never. C# shows a deferred query that runs its lambda each time. Go shows which parameters share data. Rust shows the compile error for a change through `&`. C shows a static counter and a macro that runs its argument twice. C++ shows a `const` method that changes a `mutable` cache.

## Variations and related ideas

- **Benign effects.** A cache, a log line or a counter of calls changes state but not the results that callers see. C++ marks such members `mutable`, and functional programmers still call a memoized function pure.
- **Idempotence.** An operation is idempotent when doing it twice has the same effect as doing it once, such as setting a value or an HTTP `PUT`. Idempotent effects make retries safe.
- **Command-query separation.** Bertrand Meyer's rule says that a method should either return a value or change state, not both.
- **Effect systems.** Languages such as Koka track the effects of each function in its type, and Haskell tracks them through monads such as `IO` and `State`.
- **Immutability.** [Immutable](/immutability/) values cannot change, so code that shares them has fewer effects to track.
- **State.** Side effects are the way a program changes its state: the values of its variables, objects and files at a point in the run.

## Pitfalls

- **Hidden changes to arguments.** A function that sorts, appends to or edits an argument surprises a caller that still uses it. Return a new value, or name the function so that the change is clear.
- **Shared default values.** In Python, a default value such as `[]` is made once when `def` runs. The documentation says: "the expression is evaluated once, when the function is defined, and that the same “pre-computed” value is used for each call."[^5]
- **Macros that repeat arguments.** A C macro such as `MAX(a, b)` evaluates an argument twice, so an argument with a side effect runs twice.
- **Unspecified order.** The order in which C and C++ evaluate the arguments of a function call is unspecified, so `printf("%d %d", next(), next())` can print the two values in either order.
- **Effects in assertions and logs.** Code with an effect inside `assert`, or inside a log call that is turned off, stops running in the release build.
- **Effects in lazy pipelines.** A lambda with an effect in a stream, a generator or a query runs late, more than once or not at all.
- **Shallow protection.** `Object.freeze`, `readonly` fields, `final` and `const` pointers protect one level. The objects that they refer to can still change.

## Cost

Side effects are how a program does useful work, and they cost nothing to write. The cost is in reasoning and in optimization. A compiler cannot remove, reorder or merge calls with effects, so a call in a loop condition runs on every pass. Code with many shared effects needs locks or other synchronization when threads run it. Pure code is easier to test, cache, run in parallel and run again after a failure. Avoiding effects also has a cost: a function that returns a new collection instead of changing one allocates and copies, unless the language uses persistent data structures that share most of their structure.

## History

Early languages, from FORTRAN to Algol, were built on assignment, and a function could change global variables freely. Lisp, from 1958, had pure functions in its mathematical core but also assignment with `setq` and changes to lists with `rplaca`. Christopher Strachey's lecture notes of 1967, *Fundamental Concepts in Programming Languages*, used the term referential transparency for expressions that can be replaced by their values. Peter Landin's ISWIM of 1966 and the language ML kept assignment but made it explicit through reference cells.

The lazy functional languages needed another way, because a lazy program evaluates expressions in an order that the programmer does not control. Philip Wadler and Simon Peyton Jones applied monads, from the work of Eugenio Moggi, to I/O, and Haskell 1.3 adopted monadic I/O in 1996. Later work made effects part of more type systems, from the effect systems of the late 1980s to Koka and the effect handlers of OCaml 5 in 2022. Mainstream languages added copying methods, immutable collections and readonly types, which make code with few effects easier to write.

## References
