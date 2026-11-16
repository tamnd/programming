---
tag: V6PDED
kind: concept
slug: lazy-sequence
name: Lazy sequence and stream
aliases: [lazy sequence, stream, lazy stream, lazy list, deferred sequence, lazy iterator, sequence (Kotlin), LazyList]
area: functions-and-abstraction/lazy-evaluation
level: core
summary: >-
  A lazy sequence produces its elements one at a time, only when a consumer asks for them. A pipeline of lazy stages does the work for one element at a time and can stop early.
applies_to: core-languages
viz: optional
formal: none
history:
  year: 1965
  people: [Peter Landin]
  note: SICP states that Landin first described, in 1965, the use of delayed evaluation in the manner of Algol 60 call by name to implement streams.
see_also: [laziness, thunk, infinite-data-structures, iterators, collection-pipeline, strict-evaluation]
same_as:
  wikidata: Q6659657
  wikipedia: Stream_(abstract_data_type)
sources:
- url: https://mitp-content-server.mit.edu/books/content/sectbyfn/books_pres_0/6515/sicp.zip/full-text/book/book-Z-H-24.html
  title: Structure and Interpretation of Computer Programs, H. Abelson and G. J. Sussman, second edition, MIT Press
  locator: 3.5 Streams; 3.5.1 Streams Are Delayed Lists; footnote 58
- url: https://www.cs.kent.ac.uk/people/staff/dat/miranda/whyfp90.pdf
  title: Why Functional Programming Matters, J. Hughes, in Research Topics in Functional Programming, D. Turner, editor, Addison-Wesley, 1990
  locator: 4 Gluing Programs Together
- url: https://pkg.go.dev/iter
  title: iter package, Go standard library
  locator: Iterators
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/util/stream/package-summary.html
  title: java.util.stream package summary, Java SE 25 API documentation
  locator: Streams differ from collections in several ways
- url: https://kotlinlang.org/docs/sequences.html
  title: Sequences, Kotlin documentation
  locator: introduction; Sequence operations
- url: https://learn.microsoft.com/en-us/dotnet/standard/linq/deferred-execution-lazy-evaluation
  title: Deferred execution and lazy evaluation (LINQ to XML), .NET documentation
  locator: Deferred execution; Eager vs. lazy evaluation
- url: https://www.scala-lang.org/api/current/scala/collection/immutable/LazyList.html
  title: LazyList, Scala standard library API
  locator: class description
- url: https://docs.python.org/3.14/howto/functional.html
  title: Functional Programming HOWTO, Python 3.14 documentation
  locator: Iterators
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Iterator
  title: Iterator, MDN Web Docs
  locator: Iterator helper methods
- url: https://learn.microsoft.com/en-us/dotnet/api/system.linq.enumerable.take
  title: Enumerable.Take Method, .NET API documentation
  locator: Remarks
- url: https://doc.rust-lang.org/book/ch13-02-iterators.html
  title: Processing a Series of Items with Iterators, The Rust Programming Language
  locator: 13.2, introduction
- url: https://en.cppreference.com/w/cpp/ranges
  title: Ranges library, cppreference.com
  locator: introduction
---

## Idea

A list holds all of its elements in memory. A lazy sequence holds a recipe for its elements and produces each one only when a consumer asks for it. When the stages of a pipeline are lazy, the consumer pulls one element through all the stages, then the next one, and the pipeline stops as soon as the consumer stops asking:

```python run id=idea
def numbers():
    for n in range(1, 6):
        print("produce", n)
        yield n


squares = (n * n for n in numbers())
big = (s for s in squares if s > 5)
print("pipeline built")
print(next(big))
```

```output for=idea
pipeline built
produce 1
produce 2
produce 3
9
```

Building the pipeline does no work. The call to `next` pulls numbers through the square stage and the filter stage until one passes, and `4` and `5` are never produced. The word stream has the same meaning in SICP and in the Java and Scala libraries; it is not the same thing as a byte stream for files and sockets.

## How it works

**A sequence with a delayed rest.** SICP introduces streams as lists whose rest is not computed yet. "From an abstract point of view, a stream is simply a sequence", and delayed evaluation "enables us to represent very large (even infinite) sequences as streams."[^1] A stream cell holds the first element and a [thunk](/thunk/) for the rest of the stream. Taking the rest forces the thunk, which makes the next cell.

**Demand drives the work.** SICP calls this "demand-driven" programming, "whereby each stage in the stream process is activated only enough to satisfy the next stage." The program is written as if the streams existed all at once, while the computation "is performed incrementally".[^1] John Hughes describes the same idea for whole programs: a producer `f` "is started only when g tries to read some input, and runs only for long enough to deliver the output g is trying to read." He adds that this "allows termination conditions to be separated from loop bodies".[^2]

**Pull and push.** Most lazy sequences use a pull protocol: the consumer calls a method such as `next`, and each stage calls `next` on the stage before it. Go uses a push protocol for its iterators. An iterator "is a function that passes successive elements of a sequence to a callback function, conventionally named yield. The function stops either when the sequence is finished or when yield returns false".[^3] In both protocols the consumer decides how much work is done.

**Intermediate and terminal operations.** A lazy library splits its operations into two groups. An intermediate operation, such as `map` or `filter`, returns another lazy sequence and does no work. A terminal operation, such as `toList`, `sum` or a `for` loop, pulls elements and starts the work. The Java documentation states: "Intermediate operations are always lazy."[^4] Kotlin uses the same split: if an operation returns another sequence, "it's called intermediate. Otherwise, the operation is terminal."[^5]

**Element by element, not stage by stage.** An eager pipeline runs each stage over the whole collection and builds a full intermediate collection. Kotlin describes the difference: a `Sequence` "performs all the processing steps one-by-one for every single element", while an eager `Iterable` "completes each step for the whole collection and then proceeds to the next step."[^5]

**Stateless and stateful stages.** A stage such as `map`, `filter` or `take` needs only the current element. A stage such as `sorted` or `distinct` needs to remember many elements. Kotlin notes that stateful operations "require a significant amount of state, usually proportional to the number of elements in a sequence."[^5] .NET calls a stateful stage eager inside a deferred query: "the OrderBy method has to sort the entire collection before it returns the first element."[^6]

**Two kinds of lazy sequence.** An iterator or a generator is a cursor: it has a position, it moves forward, and it can be read one time. A memoized stream is a value: each cell, once computed, stays in memory, and any number of readers can walk it from the start. Scala's `LazyList` is the second kind. Its documentation states: "This class implements an immutable linked list", and the list "computes its elements only when they are needed."[^7] Haskell lists and SICP streams with memoized `delay` are also of this kind.

## In languages

| Language | Verdict | Lazy sequence | Kind |
|---|---|---|---|
| Python | native | generators, generator expressions, `map`, `filter`, `itertools` | single-pass iterator |
| JavaScript | native | generator functions, iterator helpers | single-pass iterator |
| TypeScript | native | as JavaScript, typed `Generator<T>` and `IterableIterator<T>` | single-pass iterator |
| Java | stdlib | `java.util.stream.Stream` | single-use pipeline |
| Kotlin | stdlib | `Sequence<T>`, `sequence { }` | re-runnable recipe |
| C# | stdlib | LINQ over `IEnumerable<T>`, `yield return` | re-runnable recipe |
| Go | stdlib | `iter.Seq[T]` and `iter.Seq2[K, V]` | push iterator |
| Rust | stdlib | the `Iterator` trait and its adapters | single-pass iterator |
| C | absent | a struct with a `next` function | hand-written iterator |
| C++ | stdlib | range views and adaptors | re-runnable view |
| Bash | idiom | a pipeline of processes | single-pass stream of lines |
| Haskell | native | every list | memoized stream |

**Python.** "An iterator is an object representing a stream of data", and an iterator can produce "an infinite stream of data."[^8] Generator functions and generator expressions make iterators, and `itertools` gives the lazy stages.

**JavaScript.** Iterator helpers, such as `map`, `filter`, `take` and `drop` on `Iterator.prototype`, are lazy. MDN notes that this style "only iterates the iterator once, without memorizing any intermediate values", and that "Iterator helper methods are necessary to work with infinite iterators".[^9]

**Java.** A stream is lazy and "Consumable. The elements of a stream are only visited once during the life of a stream."[^4] A stream is also "Possibly unbounded", and "Short-circuiting operations such as limit(n) or findFirst() can allow computations on infinite streams to complete in finite time."[^4]

**C#.** LINQ operators use deferred execution, which "means that the evaluation of an expression is delayed until its realized value is actually required."[^6] `Enumerable.Take`, for example, "is implemented by using deferred execution."[^10]

**Rust.** "In Rust, iterators are lazy, meaning they have no effect until you call methods that consume the iterator to use it up."[^11]

**C++.** Range adaptors "are applied to views lazily. Adaptors can be composed into pipelines, so that their actions take place as the view is iterated."[^12]

**Bash.** The processes of a pipeline run at the same time and pass lines through pipes. A reader that exits early, such as `head`, stops the writer.

## Variations and related ideas

**Generators.** A generator function is the easiest way to write a lazy sequence in an eager language. It keeps its local variables between elements, so a loop with `yield` describes the whole sequence. See [iterators](/iterators/).

**Collection pipelines.** A chain of `map`, `filter` and `reduce` is a [collection pipeline](/collection-pipeline/). The chain can be eager, as with Kotlin lists, or lazy, as with Kotlin sequences and Java streams. The code looks the same, and the order of the work differs.

**Infinite sequences.** A lazy sequence can describe all natural numbers or all primes, because no code ever asks for all of them. See [infinite data structures](/infinite-data-structures/).

**Lazy in the source, eager in a stage.** A deferred query can still contain an eager stage. .NET separates the two ideas: an operator is deferred if it does no work until enumeration, and, once it runs, it is lazy if "a single element of the source collection is processed during each call to the iterator".[^6]

**Async and reactive streams.** An asynchronous sequence, such as an `async` generator, also produces elements on demand, but the consumer waits for each element. Reactive streams push elements to subscribers as they arrive.

## Pitfalls

**A lazy sequence outlives its source.** A lazy sequence reads its source when the consumer pulls, not when the code builds it. If the source is closed by then, the read fails:

```python wrong id=closed-source
import io


def read_lines(text):
    with io.StringIO(text) as source:
        return (line.strip() for line in source)


lines = read_lines("a\nb\n")
try:
    print(list(lines))
except ValueError as error:
    print("error:", type(error).__name__)
```

```output for=closed-source
error: ValueError
```

The `with` block closes the source when `read_lines` returns, before any line is read. Make the function a generator, so that the source stays open while the consumer reads:

```python run id=open-source
import io


def read_lines(text):
    with io.StringIO(text) as source:
        for line in source:
            yield line.strip()


print(list(read_lines("a\nb\n")))
```

```output for=open-source
['a', 'b']
```

**Reading a single-pass sequence twice.** A Python generator, a Java stream and a Rust iterator are used up by one pass. A second pass finds nothing, or fails.

**Running a re-runnable recipe twice.** A Kotlin sequence or a LINQ query runs its whole pipeline again for each terminal operation. If a stage is slow or has side effects, the work and the effects happen again. Store the result in a list when more than one consumer needs it.

**Side effects at surprising times.** The stages run interleaved, so a `print` in a `map` stage appears between the effects of other stages, and an effect in a stage that the consumer never reaches does not happen. Hughes wrote that the power of lazy evaluation depends on "the programmer giving up any direct control over the order in which the parts of a program are executed".[^2]

**A stateful stage on an infinite sequence.** `sorted`, `distinct` over an unbounded range, `count` and `reverse` must see every element. On an infinite sequence they never return.

## Cost

A lazy pipeline does not build intermediate collections, so it uses memory for one element per stage instead of one collection per stage. It can stop early, so a search does only the work needed to find the result. The price is overhead for each element: a call through each stage, and often a small object for the iterator state of each stage. Kotlin warns that "the lazy nature of sequences adds some overhead which may be significant when processing smaller collections or doing simpler computations."[^5] A memoized stream also keeps each computed cell while any code holds a reference to an earlier cell, so a reference to the head of a long stream can hold the whole stream in memory.

## History

SICP traces lazy streams to Algol 60. Its footnote states that delayed evaluation "was inherent in Algol 60's call-by-name parameter-passing method. The use of this mechanism to implement streams was first described by Landin (1965). Delayed evaluation for streams was introduced into Lisp by Friedman and Wise (1976)."[^1] SICP made streams a main tool of its chapter 3, and Hughes's paper "Why Functional Programming Matters", in 1990, argued that lazy evaluation is "perhaps the most powerful tool for modularization in the functional programmer's repertoire."[^2] Eager languages took the idea in the form of generators, iterators and stream libraries, such as Python generators, Java streams, LINQ, Kotlin sequences, Rust iterators and C++ ranges.

## References
