---
tag: KEXF6B
kind: concept
slug: infinite-data-structures
name: Infinite data structures
aliases: [infinite list, infinite stream, infinite sequence, codata, potentially infinite data, lazy infinite list]
area: functions-and-abstraction/lazy-evaluation
level: advanced
summary: >-
  An infinite data structure, such as the list of all primes, is defined by a rule and computed lazily. A program can use it because it only ever reads a finite part.
applies_to: core-languages
viz: optional
formal: none
history:
  year: 1965
  people: [Peter Landin]
  note: SICP states that Landin first described, in 1965, how delayed evaluation can implement streams, the structure that lets a program define infinitely long sequences.
see_also: [lazy-sequence, laziness, thunk, recursion, iterators]
sources:
- url: https://mitp-content-server.mit.edu/books/content/sectbyfn/books_pres_0/6515/sicp.zip/full-text/book/book-Z-H-24.html
  title: Structure and Interpretation of Computer Programs, H. Abelson and G. J. Sussman, second edition, MIT Press
  locator: 3.5.2 Infinite Streams; Defining streams implicitly; footnotes 58 and 60
- url: https://www.cs.kent.ac.uk/people/staff/dat/miranda/whyfp90.pdf
  title: Why Functional Programming Matters, J. Hughes, in Research Topics in Functional Programming, D. Turner, editor, Addison-Wesley, 1990
  locator: 4.1 Newton-Raphson Square Roots; 5 An Example from Artificial Intelligence
- url: https://en.wikipedia.org/wiki/Corecursion
  title: Corecursion, Wikipedia
  locator: introduction
- url: https://www.haskell.org/onlinereport/haskell2010/haskellch9.html
  title: Haskell 2010 Language Report, Standard Prelude
  locator: 9.1 PreludeList (iterate, repeat, cycle)
- url: https://doc.rust-lang.org/std/iter/index.html
  title: Module std::iter, Rust standard library
  locator: Infinity
- url: https://www.cs.hmc.edu/~oneill/papers/Sieve-JFP.pdf
  title: The Genuine Sieve of Eratosthenes, M. E. O'Neill, Journal of Functional Programming
  locator: Abstract; 1 Introduction; 2 What the Sieve Is and Is Not
- url: https://docs.python.org/3.14/library/itertools.html
  title: itertools, Python 3.14 documentation
  locator: itertools.count; itertools.repeat
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/util/stream/Stream.html
  title: Interface Stream, Java SE 25 API documentation
  locator: iterate; generate
- url: https://kotlinlang.org/docs/sequences.html
  title: Sequences, Kotlin documentation
  locator: Construct, From a function
---

## Idea

A program cannot store an infinite list, but it can store a rule that produces the elements of one. With lazy evaluation, the rule runs only for the elements that the program reads. So a program can define "all natural numbers" or "all powers of two" as a value, pass it around, and take as much of it as it needs:

```python run id=idea
from itertools import count, islice


def iterate(f, x):
    while True:
        yield x
        x = f(x)


powers_of_two = iterate(lambda x: 2 * x, 1)
print(list(islice(powers_of_two, 10)))

evens = (n for n in count(0) if n % 2 == 0)
print(next(evens), next(evens), next(evens))
```

```output for=idea
[1, 2, 4, 8, 16, 32, 64, 128, 256, 512]
0 2 4
```

`iterate` has no end, and `count(0)` has no end. The program stops because `islice` and `next` ask for a finite number of elements. SICP describes the effect: "This is an infinitely long stream, but in any given time we can examine only a finite portion of it. Thus, our programs will never know that the entire infinite stream is not there."[^1]

## How it works

**Potential infinity.** An infinite structure is infinite only in what it can produce. John Hughes writes about an infinite list of approximations: "no more approximations will actually be computed than the rest of the program requires. The infinity is only potential".[^2] At any moment, memory holds a finite prefix and a suspended computation for the rest.

**A head and a delayed tail.** The classic form is a stream cell: a first element and a [thunk](/thunk/) for the rest. SICP defines the positive integers this way, with `(cons-stream n (integers-starting-from (+ n 1)))`, and the rest of the stream is "a promise to produce the integers beginning with 2."[^1] Each force of the tail makes one more cell and one more promise.

**Generators.** An eager language usually expresses an infinite sequence as a loop that never ends, inside a generator or an iterator. The loop pauses after each element, so it runs only when the consumer asks. See [lazy sequence and stream](/lazy-sequence/).

**Corecursion.** Recursion consumes a structure, and corecursion produces one. Wikipedia describes the difference: recursion handles "the topmost layer before descending into its inner parts", while corecursion "produces a data structure by first defining the topmost layer before defining its inner parts."[^3] A corecursive definition is safe when each step produces at least one element before it recurses. The Haskell Prelude defines `iterate` in this way, as `iterate f x = x : iterate f (f x)`, and describes it as "an infinite list of repeated applications of f to x".[^4]

**Self-reference.** A lazy structure can refer to itself. SICP defines the stream of ones as `(define ones (cons-stream 1 ones))`, and then defines the integers from the ones and from the integers: the first element is 1 and the rest "is the sum of ones and integers".[^1] The definition works "because, at any point, enough of the integers stream has been generated so that we can feed it back into the definition to produce the next integer."[^1] In Haskell, `repeat x = xs where xs = x:xs` builds one cell that points to itself, so an infinite list of the same value uses constant memory.[^4]

**Finite consumers.** Only some operations make sense on an infinite structure. `take n`, `takeWhile`, `find`, `first`, indexing and a loop with `break` read a finite prefix and stop. `length`, `sum`, `max`, `sort` and `reverse` must see every element, so they never return. The Rust documentation warns that "methods on infinite iterators, even those for which a result can be determined mathematically in finite time, might not terminate."[^5]

**The prime sieve.** The best-known example is the infinite stream of primes. SICP builds it as a sieve: take the first element of the stream as a prime, filter its multiples out of the rest, and sieve the result. "Thus, not only is the stream infinite, but the signal processor is also infinite, because the sieve contains a sieve within it."[^1] Melissa O'Neill pointed out that this famous program is not the sieve of Eratosthenes: the real sieve crosses off the multiples of each prime "starting from p2", and the stream program instead tests each number against every smaller prime. In her words, "this algorithm is not the Sieve of Eratosthenes!"[^6]

## In languages

| Language | Verdict | Infinite source | Finite consumer |
|---|---|---|---|
| Python | stdlib | `itertools.count`, `cycle`, `repeat`, a generator with `while True` | `islice`, `next`, `takewhile`, `break` |
| JavaScript | native | a generator function with an endless loop | iterator `take`, `find`, `break` |
| TypeScript | native | as JavaScript, `Generator<T>` | as JavaScript |
| Java | stdlib | `Stream.iterate`, `Stream.generate`, `IntStream.iterate` | `limit`, `takeWhile`, `findFirst` |
| Kotlin | stdlib | `generateSequence`, `sequence { }` | `take`, `takeWhile`, `first` |
| C# | stdlib | an iterator method with `yield return` in an endless loop | `Take`, `TakeWhile`, `First` |
| Go | stdlib | an `iter.Seq` function with an endless loop | `for range` with `break` |
| Rust | stdlib | `0..`, `repeat`, `successors`, `from_fn`, `cycle` | `take`, `take_while`, `find`, `nth` |
| C | absent | a struct with a `next` function | a loop with a bound |
| C++ | stdlib | `views::iota(1)`, a coroutine generator | `views::take`, `views::take_while` |
| Bash | idiom | `yes`, an endless `while` loop in a pipeline | `head` |
| Haskell | native | `[1..]`, `iterate`, `repeat`, `cycle`, recursive definitions | `take`, `takeWhile`, `!!` |

**Python.** `itertools.count` makes "an iterator that returns evenly spaced values beginning with start", and `itertools.repeat` "Runs indefinitely unless the times argument is specified."[^7]

**Java.** `Stream.iterate` "Returns an infinite sequential ordered Stream" built from a seed and a function, and `Stream.generate` "Returns an infinite sequential unordered stream" from a `Supplier`.[^8]

**Kotlin.** `generateSequence` builds a sequence from a function, and "The sequence generation stops when the provided function returns null", so a function that never returns null gives an infinite sequence.[^9]

**Rust.** "Iterators do not have to be finite. As an example, an open-ended range is an infinite iterator".[^5]

## Variations and related ideas

**Cyclic structures.** A finite cyclic structure, such as `cycle [1, 2, 3]` in Haskell, behaves like an infinite list but uses a fixed amount of memory. The Prelude defines `cycle xs = xs' where xs' = xs ++ xs'`, and describes it as "the infinite repetition of the original list."[^4]

**Infinite trees.** A game tree, the tree of all moves from a position, can be infinite. Hughes notes that if a game can "go on forever with neither side winning, its game tree is infinite." He builds such a tree lazily and prunes it to a fixed depth before evaluation, so the code that makes moves and the code that limits the search stay separate.[^2]

**Codata.** Wikipedia calls the trees that recursion can consume data, or initial data types, because they are "not infinitely deep". It notes that corecursion "produces codata or final data types, which includes infinitely deep trees."[^3]

**Bounded but huge.** A sequence of all 64-bit integers, or all lines of a log that grows, is finite, but a program treats it like an infinite one: it never builds it in full.

## Pitfalls

**A total operation on an infinite structure.** `len`, `sum`, `max`, `sorted` or `list` on an infinite sequence never returns, and memory can grow until the program fails. Always put a limit, such as `islice`, `take` or `limit`, before such an operation.

**A filter that never finds anything.** `filter` on an infinite sequence is safe only if enough elements pass. A search for the first even prime greater than 2 runs forever.

**Recursive stream code in an eager language.** The SICP sieve, written with nested Python generators, adds two levels of nesting for each prime. The nesting soon passes the recursion limit:

```python wrong id=nested-sieve
from itertools import count, islice


def sieve(numbers):
    p = next(numbers)
    yield p
    yield from sieve(n for n in numbers if n % p != 0)


try:
    print(len(list(islice(sieve(count(2)), 2000))))
except RecursionError as error:
    print("error:", type(error).__name__)
```

```output for=nested-sieve
error: RecursionError
```

The program is also slow. O'Neill describes it as a simple algorithm "known as trial division", and as even worse than the usual form of trial division, because it tests each number against all smaller primes, not only up to the square root. An incremental sieve keeps, for each prime found so far, the next multiple to cross off, and starts each prime at its square:

```python run id=incremental-sieve
from itertools import count, islice


def primes():
    next_composite = {}
    for n in count(2):
        factor = next_composite.pop(n, None)
        if factor is None:
            next_composite[n * n] = n
            yield n
        else:
            m = n + factor
            while m in next_composite:
                m += factor
            next_composite[m] = factor


print(list(islice(primes(), 10)))
print(next(islice(primes(), 1999, None)))
```

```output for=incremental-sieve
[2, 3, 5, 7, 11, 13, 17, 19, 23, 29]
17389
```

**Holding the head.** With a memoized stream, every cell that the program has computed stays in memory while some variable refers to the first cell. A loop over a long prefix then uses memory in proportion to its length. Keep a reference only to the part that is still needed.

**Side effects in the rule.** A rule with side effects runs them when a consumer pulls, in an order that depends on the consumer. An infinite stream that reads input or prints output is hard to reason about.

## Cost

An infinite structure costs only what the program reads. A generator uses constant memory for its state. A memoized stream uses memory for every cell that is computed and still referenced. Each element costs a call into the producer, and for a stream, a thunk allocation and a force. The algorithm inside the rule decides the rest: O'Neill shows that the stream sieve is "asymptotically worse than simple trial division", and that trial division in turn is worse than the true sieve.[^6]

## History

SICP traces the method to the 1960s: delayed evaluation "was inherent in Algol 60's call-by-name parameter-passing method. The use of this mechanism to implement streams was first described by Landin (1965)."[^1] The same footnote states that Friedman and Wise brought delayed streams into Lisp in 1976. O'Neill notes that the sieve "has been cited in introductions to lazy functional programming for more than thirty years (Turner, 1975)", and her paper on the true lazy sieve appeared in the Journal of Functional Programming.[^6] Hughes used infinite lists of approximations and infinite game trees in "Why Functional Programming Matters" to show how laziness separates the producer of values from the code that decides when to stop.[^2] In Haskell, `iterate`, `repeat` and `cycle` are part of the standard Prelude.[^4]

## References
