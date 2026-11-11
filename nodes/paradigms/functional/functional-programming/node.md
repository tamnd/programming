---
tag: WYHRGV
kind: concept
slug: functional-programming
name: Functional programming
aliases: [FP, functional style, pure functions, purity, referential transparency, map filter reduce, function composition, functional core imperative shell]
area: paradigms/functional
level: core
summary: >-
  Functional programming builds a program from pure functions and values that do not change, and joins small functions into larger ones with higher-order functions and composition.
applies_to: core-languages
viz: optional
formal: none
history:
  year: 1960
  people: [Alonzo Church, John McCarthy, John Backus]
  note: McCarthy's LISP built programs from recursive functions, and Backus's 1977 Turing Award lecture argued for a functional style free of assignment.
see_also: [higher-order-functions, closures, immutability, recursion, laziness, pattern-matching, sum-types, object-oriented-programming]
same_as:
  wikidata: Q193076
  wikipedia: Functional_programming
sources:
- url: https://docs.python.org/3.14/howto/functional.html
  title: Functional Programming HOWTO, Python 3.14 documentation
  locator: Introduction
- url: https://www.cs.kent.ac.uk/people/staff/dat/miranda/whyfp90.pdf
  title: Why Functional Programming Matters, John Hughes, The Computer Journal 32(2), 1989
  locator: 1 Introduction; 2 An Analogy with Structured Programming
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Array/reduce
  title: Array.prototype.reduce(), MDN Web Docs
  locator: Description
- url: https://www-formal.stanford.edu/jmc/recursive.pdf
  title: Recursive Functions of Symbolic Expressions and Their Computation by Machine, Part I, John McCarthy, Communications of the ACM 3(4), 1960
  locator: 2 Functions and Function Definitions
- url: https://www.haskell.org/onlinereport/haskell2010/haskellch1.html
  title: Haskell 2010 Language Report
  locator: Chapter 1 Introduction
- url: https://learn.microsoft.com/en-us/dotnet/standard/linq/concepts-terminology-functional-transformation
  title: Concepts and terminology (functional transformation), .NET, Microsoft Learn
  locator: Pure functional transformations
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/util/stream/package-summary.html
  title: Package java.util.stream, Java SE 25 and JDK 25 API
  locator: package description; Stateless behaviors
- url: https://doc.rust-lang.org/1.99.1/book/ch13-00-functional-features.html
  title: 'The Rust Programming Language, Functional Language Features: Iterators and Closures, Rust 1.99.1'
  locator: chapter introduction
- url: https://worrydream.com/refs/Backus_1978_-_Can_Programming_Be_Liberated_from_the_von_Neumann_Style.pdf
  title: Can Programming Be Liberated from the von Neumann Style? A Functional Style and Its Algebra of Programs, John Backus, Communications of the ACM 21(8), 1978
  locator: abstract; 11 Functional Programming Systems
---

## Idea

Functional programming builds a program from functions that compute a result from their arguments and do nothing else. A function of this kind does not change a variable, a data structure or the outside world, so a call with the same arguments always gives the same result. Data does not change after it is made. To get a changed value, a program builds a new one. Functions are values too, so they can be passed to other functions, returned from them and joined into new functions:

```python run id=idea
from functools import reduce

orders = (("pen", 3, 2), ("book", 1, 12), ("ink", 2, 5))


def line_total(order):
    _, qty, price = order
    return qty * price


def bulk(order):
    return order[1] > 1


bulk_totals = map(line_total, filter(bulk, orders))
print(reduce(lambda acc, x: acc + x, bulk_totals, 0))
print(tuple(o[0] for o in orders if not bulk(o)))
print(orders[0])
```

```output for=idea
16
('book',)
('pen', 3, 2)
```

The program states what it computes: the totals of the bulk orders, added up. It does not keep a running total in a variable or change the orders. Each function can be tested alone, because its result depends only on its argument.

## How it works

**Pure functions.** A function is pure when its result depends only on its arguments and the call has no other effect. The Python HOWTO states the ideal: "functions only take inputs and produce outputs, and don't have any internal state that affects the output produced for a given input."[^1] John Hughes put the same point as "A function call can have no effect other than to compute its result."[^2] Because a pure call can be replaced by its value, a reader can reason about the code as about algebra. This property is called referential transparency. Hughes noted that in such programs "one can freely replace variables by their values and vice versa".[^2]

**Immutable data.** If values never change, two parts of a program can share a value with no risk, and an old version of a structure stays valid after a new version is made. Functional languages use persistent data structures, such as linked lists and balanced trees, in which a new version shares most of its parts with the old one. A change then costs a small number of new nodes and not a full copy. See [immutability](/immutability/).

**Functions as values.** A function can take functions as arguments and return functions. These are [higher-order functions](/higher-order-functions/). The common ones are `map`, which applies a function to each element, `filter`, which keeps the elements that pass a test, and `fold` or `reduce`, which combines the elements into one value. MDN describes `reduce` as a method that "executes a user-supplied "reducer" callback function on each element of the array, in order, passing in the return value from the calculation on the preceding element."[^3] A function that is returned from another function keeps the variables of its scope, so it is a [closure](/closures/).

**Recursion instead of loops.** A loop needs a variable that changes on each pass. A pure program expresses repetition with [recursion](/recursion/) or with a higher-order function that hides the loop. Functional languages eliminate tail calls, so a recursive call in the last position runs in constant stack space.

**Expressions instead of statements.** In a functional language, almost everything is an expression with a value: a conditional, a block, a `match`. John McCarthy's paper on LISP introduced the conditional expression for this purpose, and he wrote that its use "permits functions to be defined recursively in a new and convenient way."[^4]

**Effects at the edges.** A program must read input and write output at some point. Pure languages such as Haskell, which its report calls "a general purpose, purely functional programming language",[^5] describe effects as values of a type such as `IO` and let the run-time system perform them. Most other languages allow effects anywhere, and the functional style keeps them in a thin outer layer. The Python HOWTO notes that programs in this style "provide a functional-appearing interface but will use non-functional features internally."[^1]

**Why it helps.** Hughes argued that the main gain is modularity, not the absence of assignment. A problem can only be divided into parts as far as the language can join the parts again, and "functional languages provide two new, very important kinds of glue".[^2] The two kinds of glue are higher-order functions and lazy evaluation.[^2] Pure code is also easier to test, because a test needs only arguments and an expected result, and easier to run in parallel, because pure calls do not share mutable state.

## In languages

| Language | Verdict | Mechanism |
|---|---|---|
| Python | native | `lambda`, comprehensions, `map`, `filter`, `functools`, `itertools`, tuples; no tail call elimination |
| JavaScript | native | arrow functions, closures, `map`, `filter`, `reduce`, spread copies, `toSorted` |
| TypeScript | native | as JavaScript, with `readonly` types and discriminated unions |
| Java | native | lambdas, `java.util.function`, streams, records, `List.of` |
| Kotlin | native | lambdas, collection functions, read-only collections, data classes with `copy` |
| C# | native | lambdas, `Func`, LINQ, records with `with`, pattern matching |
| Go | idiom | functions as values and closures; loops are the usual style |
| Rust | native | closures, iterators, immutable by default, enums with `match` |
| C | idiom | function pointers; no closures |
| C++ | native | lambdas, `std::function`, algorithms, range views |
| Bash | idiom | pipelines compose programs; no functions as values |
| PowerShell | idiom | script blocks and the pipeline |
| SQL | n/a | declarative queries; no functions as values |
| Regex, HTML, CSS | n/a | no functions |

Python, JavaScript and C# are multi-paradigm languages that took the tools of functional programming without the rules. They have functions as values, closures and the standard higher-order functions, but their containers are mutable, and nothing checks that a function is pure. The functional style in these languages is a discipline of the programmer. C# added LINQ and later records, and Microsoft's guide says that constructs "such as lambda expressions and type inference make functional programming much easier".[^6]

Java added lambdas and streams. The `java.util.stream` package gives "functional-style operations on streams of elements, such as map-reduce transformations on collections".[^7] The documentation warns that the results of a pipeline "may be nondeterministic or incorrect if the behavioral parameters to the stream operations are stateful".[^7] This is the rule of purity, stated as a requirement for correct parallel streams.

Rust takes the most from functional languages among the systems languages. Its book says that "one significant influence is functional programming".[^8] Variables are immutable unless marked `mut`, enums and `match` come from ML, and iterator chains compile to loops with no extra cost.

C and Go support the style only in part. C has function pointers but no closures, and Go has closures and generics but leaves `map` and `filter` out of its standard library, so the usual Go code is a loop. Bash composes whole programs with pipes, which is the same idea at the level of processes.

## Variations and related ideas

- **Pure and impure languages.** Haskell keeps effects in types and has no mutable variables in ordinary code. The ML family, Scheme, Clojure, F#, Elixir and Scala are functional but allow effects anywhere.
- **Strict and lazy evaluation.** Most functional languages evaluate arguments before a call. Haskell evaluates an expression only when its value is needed. See [laziness](/laziness/).
- **Typed and untyped.** The ML family and Haskell infer types with [Hindley-Milner](/hindley-milner/) and use [sum types](/sum-types/) and [pattern matching](/pattern-matching/). The Lisp family is dynamically typed.
- **Point-free style.** Functions are defined by composition alone, with no named arguments, for example `count = length . filter even`.
- **Function-level programming.** John Backus proposed a style in which programs are built only from functional forms that combine other functions, with no variables at all.[^9]
- **Functional core, imperative shell.** A common design in mainstream languages: the logic is pure, and a thin outer layer does the input, output and state.
- **Dataflow and reactive programming.** A program is a graph of pure transformations over streams of values.

## Pitfalls

- **Hidden mutation.** Many library functions change their argument in place, such as `list.sort` in Python or `Array.prototype.sort` in JavaScript. A function that calls them on its argument is not pure, and the caller sees the change.
- **Shallow immutability.** `Object.freeze` in JavaScript, `final` in Java and `readonly` in TypeScript protect only the top level. The objects inside can still change.
- **Deep recursion.** Python, Java, JavaScript engines and Go do not eliminate tail calls, so a recursive loop over a long list can overflow the stack. Use a loop or a higher-order function in these languages.
- **Copies in a loop.** Building a new array or string on each step of a loop, for example with spread inside `reduce`, costs quadratic time. Use a persistent structure, a builder, or a local mutable value that does not escape.
- **Effects inside a pipeline.** A `map` callback that logs, counts or writes a file makes the result depend on the order and the number of calls. Lazy pipelines and parallel streams can run the callback fewer times, more times or in another order.
- **Unreadable chains.** A long chain of anonymous functions can be harder to read than a loop. Name the steps.

## Cost

Pure code allocates more. Each change makes a new value, and a persistent tree update makes O(log n) new nodes where an in-place update writes one word. The garbage collector must then reclaim the old versions. Closures cost an allocation when they capture variables and a call through a pointer that the compiler may not inline. Good compilers remove much of this. Rust, C++ and the JIT compilers of Java and C# inline iterator chains into plain loops. GHC fuses list pipelines so that no intermediate list is built. In return, pure code needs no locks to share data between threads, and a value can be cached or computed again freely, because a call always gives the same result.

## History

The theory came first. Alonzo Church defined the lambda calculus in the 1930s as a system in which everything is a function. In 1960, John McCarthy described LISP, a language for symbolic computation built on recursive functions, conditional expressions and lists.[^4] Peter Landin showed in the 1960s how to describe ALGOL with lambda expressions and designed ISWIM. In his 1977 Turing Award lecture, published in 1978, John Backus said that conventional languages had a "primitive word-at-a-time style of programming" that came from the design of the von Neumann computer, and he proposed a functional style with an algebra of programs.[^9] Robin Milner's ML in the 1970s added type inference. Lazy languages such as Miranda led to Haskell, whose first report appeared in 1990. Hughes's paper "Why Functional Programming Matters" became the standard argument for the style.[^2] From the 2000s on, mainstream languages took lambdas, map and filter, immutable records and pattern matching from functional languages: C# added LINQ, Java added lambdas and streams, and Rust and Swift made immutable values the default.

## References
