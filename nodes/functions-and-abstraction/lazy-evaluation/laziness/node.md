---
tag: 2EHE06
kind: concept
slug: laziness
name: Lazy evaluation
aliases: [lazy evaluation, laziness, call by need, non-strict evaluation, deferred execution, lazy initialization, thunk, delayed evaluation]
area: functions-and-abstraction/lazy-evaluation
level: core
summary: >-
  Lazy evaluation computes a value only when a program needs it, and usually stores the result for later use. It allows infinite sequences and avoids work whose result is never used.
applies_to: core-languages
viz: optional
formal: none
history:
  year: 1971
  people: [Christopher Wadsworth]
  note: Wadsworth described call by need, which evaluates an argument at most once and shares the result.
see_also: [parameter-passing, closures, iterators, order-of-evaluation, futures]
same_as:
  wikidata: Q573952
  wikipedia: Lazy_evaluation
sources:
- url: https://www.haskell.org/onlinereport/haskell2010/haskellch6.html
  title: Haskell 2010 Language Report, Predefined Types and Classes
  locator: 6.2 Strict Evaluation
- url: https://mitp-content-server.mit.edu/books/content/sectbyfn/books_pres_0/6515/sicp.zip/full-text/book/book-Z-H-24.html
  title: Structure and Interpretation of Computer Programs, H. Abelson and G. J. Sussman, second edition, MIT Press
  locator: 3.5 Streams
- url: https://www.cs.kent.ac.uk/people/staff/dat/miranda/whyfp90.pdf
  title: Why Functional Programming Matters, J. Hughes, in Research Topics in Functional Programming, D. Turner, editor, Addison-Wesley, 1990
  locator: 4 Gluing Programs Together
- url: https://docs.python.org/3.14/howto/functional.html
  title: Functional Programming HOWTO, Python 3.14 documentation
  locator: Iterators and Generators
- url: https://docs.python.org/3.14/library/functools.html#functools.cached_property
  title: functools, Python 3.14 documentation
  locator: functools.cached_property
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/util/stream/package-summary.html
  title: java.util.stream package summary, Java SE 25 API documentation
  locator: Stream operations and pipelines
- url: https://kotlinlang.org/docs/sequences.html
  title: Sequences, Kotlin documentation
  locator: introduction
- url: https://learn.microsoft.com/en-us/dotnet/csharp/linq/get-started/introduction-to-linq-queries
  title: Introduction to LINQ Queries in C#, Microsoft Learn
  locator: Deferred
- url: https://learn.microsoft.com/en-us/dotnet/framework/performance/lazy-initialization
  title: Lazy Initialization, .NET documentation, Microsoft Learn
  locator: introduction
- url: https://doc.rust-lang.org/1.99.1/book/ch13-02-iterators.html
  title: The Rust Programming Language, 13.2 Processing a Series of Items with Iterators, Rust 1.99.1
  locator: introduction
- url: https://en.cppreference.com/w/cpp/ranges
  title: Ranges library, cppreference.com
  locator: introduction
---

## Idea

Most languages evaluate an expression when the program reaches it. Lazy evaluation waits until the program needs the value. A lazy value is a description of a computation, often called a thunk, and the computation runs only when some code asks for the result. If no code asks, it never runs. If the result is stored after the first run, later requests get the stored value. Laziness lets a program describe a large or infinite sequence and use only the part that it needs:

```python run id=idea
def numbers():
    n = 0
    while True:
        print("computing", n)
        yield n
        n += 1


evens = (n for n in numbers() if n % 2 == 0)
print("nothing computed yet")
print(next(evens))
print(next(evens))
```

```output for=idea
nothing computed yet
computing 0
0
computing 1
computing 2
2
```

The generator describes all natural numbers, but it computes a number only when `next` asks for one. Haskell uses lazy evaluation for every expression. Most other languages are eager by default and give laziness through generators, iterators, streams and lazy values.

## How it works

**Non-strict evaluation.** A function is strict if it always evaluates its argument. The Haskell report states: "Function application in Haskell is non-strict; that is, a function argument is evaluated only when required."[^1] The caller passes an unevaluated expression, and the function evaluates it only if a path through the body uses it. So a function can ignore an argument whose evaluation would fail or never end.

**Thunks and sharing.** An implementation represents an unevaluated expression as a thunk: code to compute the value, and the environment that the code needs, as in a [closure](/closures/). When a program forces the thunk, it runs the code and replaces the thunk with the result. SICP describes this step for streams: "The first time the memoized procedure is run, it saves the computed result. On subsequent evaluations, it simply returns the result." It adds: "The memoizing optimization is also known as call-by-need."[^2] Without memoization, the same mechanism is call by name, which evaluates the expression again on each use. See [parameter passing](/parameter-passing/).

**Demand-driven pipelines.** When each stage of a pipeline is lazy, the last stage drives the work. SICP calls this "demand-driven" programming, "whereby each stage in the stream process is activated only enough to satisfy the next stage."[^2] The stages run interleaved, element by element, and not one after another. This is why `take(3)` on an infinite sequence ends: the consumer stops asking.

**Modularity.** John Hughes argued that laziness is a kind of glue between program parts. It makes it practical "to modularize a program as a generator that constructs a large number of possible answers, and a selector that chooses the appropriate one."[^3] The generator does not need to know when to stop, and the selector does not need to know how the answers are made.

**Forcing.** A lazy language still needs a way to force evaluation, for example to stop a large chain of thunks from building up. Haskell has `seq`, and the report states that "seq is usually introduced to improve performance by avoiding unneeded laziness."[^1]

## In languages

| Language | Verdict | Mechanism |
|---|---|---|
| Python | stdlib | generators, `itertools`, `functools.cached_property` |
| JavaScript | native | generator functions, iterator helpers, self-replacing getters |
| TypeScript | native | as JavaScript, `Generator<T>` |
| Java | stdlib | `Stream` intermediate operations, `Supplier` |
| Kotlin | stdlib | `Sequence`, `by lazy` |
| C# | stdlib | LINQ deferred execution, `yield return`, `Lazy<T>` |
| Go | idiom | `iter.Seq` iterator functions, `sync.OnceValue` |
| Rust | stdlib | iterator adapters, `LazyLock`, `LazyCell` |
| C | absent | a function pointer with a flag, short-circuit operators |
| C++ | stdlib | range adaptors and views, function-local statics |
| Bash | idiom | pipelines, `&&` and `\|\|` |
| PowerShell | idiom | the object pipeline, `Select-Object -First` |
| SQL | n/a | the optimizer decides the order of evaluation |
| Regex, HTML, CSS | n/a | no evaluation of expressions |

All of these languages are eager for function arguments. Each one has a short-circuit form of `and` and `or`, which is the smallest kind of laziness.

**Python** has generator functions and generator expressions. The documentation describes generators as "a special class of functions that simplify the task of writing iterators", and states that an iterator can produce "an infinite stream of data".[^4] `functools.cached_property` computes an attribute "once and then cached as a normal attribute for the life of the instance."[^5]

**JavaScript** and **TypeScript** have generator functions. A getter that replaces itself with a data property is the usual lazy property.

**Java** streams separate intermediate and terminal operations, and "Intermediate operations are always lazy."[^6] **Kotlin** sequences are the lazy form of collections: "actual computing happens only when the result of the whole processing chain is requested."[^7]

**C#** LINQ uses deferred execution, which "means that the operation isn't performed at the point in the code where the query is declared. The operation is performed only when the query variable is enumerated".[^8] `Lazy<T>` gives lazy initialization, in which the creation of an object "is deferred until it is first used."[^9]

**Go** has iterator functions that a `for range` loop drives, and `sync.OnceValue` for lazy initialization.

**Rust** iterators "are lazy, meaning they have no effect until you call methods that consume the iterator to use it up."[^10] `LazyLock` makes a lazy global value.

**C** has no laziness except the short-circuit operators. **C++** range adaptors "are applied to views lazily", and they can be composed into pipelines.[^11]

**Bash** pipelines are demand-driven: each command blocks when the next command does not read, and an exit of the reader stops the writer.

## Variations and related ideas

**Lazy by default or lazy on request.** Haskell and Miranda are lazy by default and offer strictness with `seq`, bang patterns and strict fields. Most languages are eager by default and offer laziness through special types. Scala has `lazy val`, and Swift has `lazy var`.

**Lazy initialization.** A single value that is computed on first use is the most common form in eager languages. When many threads can make the first request, the implementation must make sure that only one of them computes the value.

**Generators and coroutines.** A generator is a function that can pause after each value. It gives lazy sequences in an eager language, and it keeps its state between requests. See [iterators](/iterators/).

**Short-circuit evaluation.** `&&`, `||`, `??` and the conditional operator evaluate the second operand only when the first one does not decide the result. See [order of evaluation](/order-of-evaluation/).

**Lazy loading.** A program or a web page can delay the load of code, data or images until the user needs them. The name is the same, and the idea is the same, at the level of resources.

**Futures.** A [future](/futures/) also stands for a value that is not computed yet. A future usually starts its work at once, in parallel. A lazy value starts its work only when a program asks for the result.

## Pitfalls

**Side effects at unknown times.** With laziness, the time when an effect happens depends on when code asks for the value. Hughes wrote that lazy evaluation and side effects do not combine well, because a programmer would need to know much about the context to predict the order of the effects, or if they occur at all.[^3] Keep lazy code free of side effects, or force it at a known point.

**Sources that change.** A deferred query reads its source when it runs, not when it is defined. If the source changes between the two times, the result includes the change.

**Single-use sequences.** A Python generator, a Java stream and many iterators can be consumed only once. A second pass gets nothing, or an exception.

**Repeated work.** A lazy sequence that is not memoized runs again for each consumer. In C#, two enumerations of the same LINQ query run the query and its side effects twice.

**Space leaks.** In a lazy language, a chain of unevaluated thunks can hold much more memory than the value it describes. The classic example is a sum over a long list with `foldl`, which builds a thunk for each element before it adds any of them. Strict versions, such as `foldl'`, and `seq` prevent this.

**Hidden cost and errors.** An expression that looks cheap can force a large computation, and an error can appear far from the code that caused it, when a thunk is finally forced.

## Cost

A thunk is a heap object with code and an environment, and the force operation checks if the value is ready. In a lazy language, the compiler uses strictness analysis to find arguments that are always needed and evaluates them eagerly, which removes most thunks. In eager languages, lazy iterators often cost less than eager collections, because they do not build the intermediate lists. They can also stop early, so a search through a large or infinite sequence does only the work needed for the result.

## History

The ideas came from call by name in Algol 60. SICP states that "the use of this mechanism to implement streams was first described by Landin (1965). Delayed evaluation for streams was introduced into Lisp by Friedman and Wise (1976). In their implementation, cons always delays evaluating its arguments, so that lists automatically behave as streams."[^2] Christopher Wadsworth described call by need, the memoized form, in his thesis on the lambda calculus in 1971. Peter Henderson and James Morris described a lazy evaluator for a Lisp-like language in 1976. David Turner made laziness the default in SASL, KRC and Miranda, and Haskell, in 1990, followed Miranda. John Hughes argued for the value of laziness in "Why Functional Programming Matters".[^3] Eager languages later took the parts of laziness that work well with side effects: generators, lazy streams, deferred queries and lazy initialization.

## References
