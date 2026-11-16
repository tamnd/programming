---
tag: QVNFSC
kind: task
slug: use-lazy-sequences
name: Use lazy sequences
aliases: [lazy pipeline, chain lazy operations, process an infinite sequence, generator pipeline, stream pipeline, deferred query]
area: functions-and-abstraction/lazy-evaluation
level: core
summary: Build a pipeline of lazy steps over an infinite sequence, a source, a filter and a take, to get the first n prime numbers without a list of all the candidates.
contract: contract.yaml
category: collections
languages: [python, javascript, typescript, java, c, cpp, csharp, go, rust, bash]
sources:
- url: https://doc.rust-lang.org/std/iter/index.html
  title: Module std::iter, The Rust Standard Library
  locator: Laziness; Infinity
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Iterator
  title: Iterator, MDN Web Docs
  locator: Iterator helper methods
- url: https://docs.python.org/3.14/library/functions.html
  title: Built-in Functions, Python 3.14 documentation
  locator: filter
- url: https://docs.python.org/3.14/library/itertools.html
  title: itertools, Python 3.14 documentation
  locator: count; islice
- url: https://docs.oracle.com/en/java/javase/21/docs/api/java.base/java/util/stream/package-summary.html
  title: Package java.util.stream, Java SE 21 API
  locator: Streams differ from collections; Stream operations and pipelines
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/util/stream/Stream.html
  title: Interface Stream, Java SE 25 API
  locator: class description
- url: https://learn.microsoft.com/en-us/dotnet/csharp/linq/get-started/introduction-to-linq-queries
  title: Introduction to LINQ Queries in C#, Microsoft Learn
  locator: Classification of standard query operators by manner of execution
- url: https://pkg.go.dev/iter@go1.26.5
  title: Package iter, Go 1.26.5 standard library
  locator: Iterators; Seq
- url: https://en.cppreference.com/w/cpp/ranges/take_view.html
  title: std::ranges::take_view, std::ranges::views::take, cppreference.com
  locator: description
- url: https://www.gnu.org/software/coreutils/manual/coreutils.html
  title: GNU Coreutils manual
  locator: 5.1 head
- url: https://docs.python.org/3.14/glossary.html
  title: Glossary, Python 3.14 documentation
  locator: iterator
---

## Task

Get the first `n` prime numbers with a pipeline of lazy steps. The function `first_primes(n)` returns them in increasing order. For example, `first_primes(5)` is `[2, 3, 5, 7, 11]`, and `first_primes(0)` is an empty list.

The program does not know before it starts how many numbers it must test to find `n` primes. So the pipeline starts with the infinite sequence 2, 3, 4, and so on, and has three steps:

1. **Source.** An infinite sequence of the numbers from 2. It makes a number only when the next step asks for one.
2. **Filter.** A step that keeps the numbers that are prime. It tests a number only when the next step asks for one more prime.
3. **Take.** A step that asks for `n` primes and then stops the pipeline.

When the take step stops, the filter stops and the source stops. The pipeline tests the numbers only up to the `n`-th prime. A pipeline of lists cannot do this, because the first list would have no end. Rust says of its iterators: "Iterators (and iterator adapters) are lazy. This means that just creating an iterator doesn’t do a whole lot."[^1] MDN says of an infinite generator: "You cannot convert seq to an array, because it is infinite."[^2]

The test for a prime is trial division: a number `k` of 2 or more is prime if no `d` with `d * d <= k` divides it. The contract allows `n` up to 1000.

## Solutions

| Language | Source | Filter | Take |
|---|---|---|---|
| Python | `count(2)` | `filter(is_prime, ...)` | `islice(..., n)` |
| JavaScript | a generator | `.filter(isPrime)` (iterator helper) | `.take(n).toArray()` |
| TypeScript | a generator | a generator `filter` | a `for` loop with `break` |
| Java | `IntStream.iterate(2, k -> k + 1)` | `.filter(Primes::isPrime)` | `.limit(n).toArray()` |
| C# | a `yield return` method | `.Where(IsPrime)` | `.Take(n).ToArray()` |
| Go | an `iter.Seq[int]` | a function from `iter.Seq[int]` to `iter.Seq[int]` | a `range` loop with `break` |
| Rust | `2..` | `.filter(...)` | `.take(n).collect()` |
| C | a struct that holds the next number | `primes_next` tests until it finds a prime | a loop of `n` calls |
| C++ | `std::views::iota(2)` | `std::views::filter(is_prime)` | `std::views::take(n)` |
| Bash | a loop without end | a `while read` loop | `head -n "$n"` |

- **Python.** `filter` makes "an iterator from those elements of iterable for which function is true"[^3]. `count` makes "an iterator that returns evenly spaced values beginning with start"[^4], and `islice` is an iterator too, so the three steps run one number at a time. A test replaces `count` with a source that records each number it makes, and checks that the pipeline stops at 11 for `n = 5`.
- **JavaScript.** "Iterator helper methods are necessary to work with infinite iterators"[^2]. `filter` and `take` are such methods. TypeScript writes the same steps as generator functions.
- **Java.** "Intermediate operations are always lazy."[^5] `filter` is intermediate, and `limit` is a "short-circuiting stateful intermediate operation"[^6], so the stream stops after `n` primes.
- **C#.** `Where` and `Take` are deferred and streaming: "a streaming operator performs its operation on each source element as it is read and yields the element if appropriate."[^7]
- **Go.** Each step is an `iter.Seq[int]`. A sequence "stops either when the sequence is finished or when yield returns false, indicating to stop the iteration early."[^8] The `break` makes `yield` return false in the last step, and the filter then returns false to the source.
- **Rust.** "Iterators do not have to be finite."[^1] The range `2..` has no end, and `take` makes the pipeline finite.
- **C++.** The views compute nothing until the loop reads them. `take` is a "view of the elements from an underlying sequence, starting at the beginning and ending at a given bound."[^9]
- **C.** C has no lazy sequence in its library. The struct `primes` holds the next number to test, and `primes_next` continues from there. This is the same idea by hand: the state of the pipeline is a small value, not a list.
- **Bash.** The three steps are three processes in a pipeline. `head -n` prints "the first num lines"[^10] and exits, and then the writes of the two other steps fail and they stop.

## Edge cases

The cases are in `cases/basic.jsonl` and `cases/edge.jsonl`.

- `zero`: `n = 0` gives an empty list. The pipeline then does not need to test any number.
- `one`: `first_primes(1)` is `[2]`, the only even prime.
- `two`: `[2, 3]`, two primes with no number between them.
- `past-a-gap`: `n = 31` ends at 127. The numbers 114 to 126 are not prime, so the filter tests 13 numbers before it gives the last prime. The take step must wait for it and not stop early.

## Pitfalls

- **Python an iterator can be read only once.** A generator, `filter` and `islice` are iterators. "Attempting this with an iterator will just return the same exhausted iterator object used in the previous iteration pass, making it appear like an empty container."[^11] The second `list` gets nothing, and there is no error:

```python run id=python-exhausted
from itertools import count, islice

small = islice(filter(lambda k: k % 2 == 1, count(1)), 4)
print(list(small))
print(list(small))
```

```output for=python-exhausted
[1, 3, 5, 7]
[]
```

- **Java a stream can be used only once.** "A stream should be operated on (invoking an intermediate or terminal stream operation) only once."[^6] A second terminal operation throws an exception. Make the stream again from its source:

```java run id=java-stream-once
import java.util.stream.IntStream;

public class Main {
    public static void main(String[] args) {
        IntStream odd = IntStream.iterate(1, k -> k + 2).limit(4);
        System.out.println(odd.sum());
        try {
            System.out.println(odd.sum());
        } catch (IllegalStateException e) {
            System.out.println(e.getClass().getSimpleName());
        }
    }
}
```

```output for=java-stream-once
16
IllegalStateException
```

- **C# a deferred query runs again each time.** The opposite fault: a C# query can be read many times, but it does all the work again each time. "Deferred execution provides the facility of query reuse since the query fetches the updated data from the data source each time query results are iterated."[^7] With a slow filter, call `ToList` once and use the list:

```csharp run id=csharp-query-twice
int tests = 0;
var odd = Enumerable.Range(1, 10).Where(k => { tests++; return k % 2 == 1; });
Console.WriteLine(odd.Count());
Console.WriteLine(odd.Sum());
Console.WriteLine(tests);
```

```output for=csharp-query-twice
5
25
20
```

- **A step that needs all the input never ends on an infinite source.** A sort or a group must read the whole input before it gives the first item. Microsoft says: "Nonstreaming operators must read all the source data before they can yield a result element. Operations such as sorting or grouping fall into this category."[^7] Java says: "Stateful operations may need to process the entire input before producing a result."[^5] Put such a step after the take step, or do not use it on an infinite sequence.
- **Rust an adapter that is not consumed does nothing.** A `map` or `filter` without a `collect`, a `for` loop or another consumer runs no code. The compiler warns: "iterators are lazy and do nothing unless consumed"[^1].

## Related tasks

- [[take-the-first-n-items]]: take the first n items of a list or of an infinite sequence.
- [[filter-a-list]]: keep the items of a list that pass a test.
- [[map-over-a-list]]: apply a function to each item of a list.
- [[iterate-in-chunks]]: split a list into parts of a fixed size.

## References
