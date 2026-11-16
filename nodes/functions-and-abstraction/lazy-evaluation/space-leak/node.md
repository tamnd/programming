---
tag: K2XY7P
kind: weakness
slug: space-leak
name: Space leak
aliases: [space leaks, thunk leak, lazy memory leak, memory hog, retained memory]
area: functions-and-abstraction/lazy-evaluation
level: advanced
summary: >-
  A space leak is memory that a program keeps much longer than it needs, for example a chain of unevaluated thunks. The memory is freed in the end, but the peak use can be huge.
languages_affected: [haskell, any-with-lazy-values]
history:
  year: 1984
  people: [John Hughes]
  note: A History of Haskell states that space leaks in lazy programs were discussed in Hughes's dissertation in 1984, together with the selective use of strictness to fix them.
see_also: [laziness, thunk, lazy-sequence, strict-evaluation, garbage-collection]
sources:
- url: https://ndmitchell.com/#spaceleak_23_oct_2013
  title: Leaking Space, Eliminating memory hogs, N. Mitchell, ACM Queue 11(9), 2013 (abstract on the author's site)
  locator: abstract
- url: https://en.wikipedia.org/wiki/Memory_leak
  title: Memory leak, Wikipedia
  locator: introduction
- url: https://hackage.haskell.org/package/base/docs/Data-List.html
  title: Data.List, the Haskell base library
  locator: foldl; foldl'
- url: https://wiki.haskell.org/Foldr_Foldl_Foldl'
  title: Foldr Foldl Foldl', HaskellWiki
  locator: Foldl; Foldl'
- url: https://www.microsoft.com/en-us/research/wp-content/uploads/2016/07/history.pdf
  title: A History of Haskell, Being Lazy With Class, P. Hudak, J. Hughes, S. Peyton Jones and P. Wadler, HOPL III, 2007
  locator: 10.2 Space profiling; 10.3 Controlling evaluation order
- url: https://downloads.haskell.org/ghc/9.14.1/docs/users_guide/profiling.html
  title: Profiling, Glasgow Haskell Compiler 9.14.1 User's Guide
  locator: 8.5 Profiling memory usage; 8.5.1 RTS options for heap profiling
- url: https://docs.python.org/3.14/library/itertools.html
  title: itertools, Python 3.14 documentation
  locator: itertools.tee
- url: https://neilmitchell.blogspot.com/2015/09/detecting-space-leaks.html
  title: Detecting Space Leaks, N. Mitchell, blog post, 2015
  locator: Summary; Method; caveats
- url: https://docs.python.org/3.14/library/tracemalloc.html
  title: tracemalloc, Python 3.14 documentation
  locator: introduction; get_traced_memory
- url: https://clojure.org/reference/lazy
  title: Making Clojure Lazier, Clojure reference
  locator: Don't hang (onto) your head
---

## Description

A space leak is memory that a program holds much longer than it needs. Neil Mitchell defines it this way: "A space leak occurs when a computer program uses more memory than necessary. In contrast to memory leaks, where the leaked memory is never released, the memory consumed by a space leak is released, but later than expected."[^1] Wikipedia makes the same distinction: a space leak is "when a program consumes excessive memory but does eventually release it."[^2] The program gives the right answer, but its peak memory can be many times larger than the data it really needs, and it can run out of memory or stack before it ends.

### Thunks that pile up

The classic cause is lazy evaluation. A [thunk](/thunk/) holds an expression and the values that the expression needs. If a program builds thunks faster than it forces them, they form a chain. The Haskell `foldl` is the standard example. Its documentation explains that `foldl` "does not force the inner results" before it applies the operator, and that "This results in a thunk chain O(n) elements long, which then must be evaluated from the outside-in."[^3] For the sum of a list of a million numbers, the chain holds a million suspended additions. The HaskellWiki traces the evaluation and states that these redexes, "instead of being directly reduced, they are allocated on the heap".[^4] When the program finally needs the sum, the evaluation of the chain goes a million levels deep, and the stack overflows.

### Values that live too long

The other cause is a reference that keeps data alive. A lazy structure that is held by its first cell keeps every cell that the program has forced. A thunk that is never forced keeps alive all the data that its expression refers to, even if the result would be small. A History of Haskell explains why lazy programs are prone to this: "By abstracting away from evaluation order, lazy evaluation also abstracts away from object lifetimes, and that is why lazy evaluation contributes to space leaks."[^5]

### Effects

The GHC User's Guide describes the cost: "Space leaks lead to slower execution due to heavy garbage collector activity, and may even cause the program to run out of memory altogether."[^6] The garbage collector cannot help, because the leaked data is still reachable. A space leak is not only a problem of lazy languages. Any program that holds a buffer, a cache or a closure longer than it needs has the same kind of fault, but laziness makes it easy to create one without notice.

### History

A History of Haskell states that the space behaviour of lazy programs "was discussed in Hughes's dissertation in 1984, along with the selective introduction of strictness to partially fix them, but there was no practical way of finding the causes of space leaks in large programs."[^5] Runciman and Wakeling then built a heap profiler that drew the contents of the heap over time, and Runciman and Röjemo extended it with information on object lifetimes and with retainer profiling. In 1996, Haskell 1.3 added `seq` and strict fields in data types, which gave programmers direct control over the evaluation order.[^5] Heap profiles are still the main tool in GHC, and Mitchell later added the small-stack method.[^6][^8]

## Vulnerable code

The function below is a lazy left fold in Python. Like the Haskell `foldl`, it does not compute the running total. It builds one more thunk at each step, and the thunk refers to the thunk before it:

```python wrong id=lazy-fold
def lazy_fold(step, start, items):
    total = lambda: start
    for item in items:
        total = (lambda before, item: lambda: step(before(), item))(total, item)
    return total


total = lazy_fold(lambda a, b: a + b, 0, range(100_000))
try:
    print(total())
except RecursionError as error:
    print("error:", type(error).__name__)
```

```output for=lazy-fold
error: RecursionError
```

Before the force, the program holds 100,000 closures for a result that is one integer. The force then needs one stack frame for each closure, and Python stops it with a `RecursionError`.

A space leak can also come from a lazy structure that two consumers read at different speeds. `itertools.tee` splits one iterator into two. Its documentation warns: "This itertool may require significant auxiliary storage (depending on how much temporary data needs to be stored)."[^7] Here the first consumer reads all the data before the second starts, so `tee` must store every element:

```python wrong id=tee
import tracemalloc
from itertools import tee


def readings():
    for i in range(200_000):
        yield i * 0.5


tracemalloc.start()
first, second = tee(readings())
count = sum(1 for _ in first)
total = sum(second)
peak = tracemalloc.get_traced_memory()[1]
tracemalloc.stop()
print(count, total)
print("peak above 1 MB:", peak > 1_000_000)
```

```output for=tee
200000 9999950000.0
peak above 1 MB: True
```

## Fixed code

The fix for the fold is to compute each step at once, so the accumulator is always a value and not a chain. This is what the Haskell `foldl'` does: it "ensures that each step of the fold is forced to Weak Head Normal Form before being applied, avoiding the collection of thunks that would otherwise occur."[^3]

```python run id=strict-fold
def strict_fold(step, start, items):
    total = start
    for item in items:
        total = step(total, item)
    return total


print(strict_fold(lambda a, b: a + b, 0, range(100_000)))
```

```output for=strict-fold
4999950000
```

The fix for `tee` is to compute both results in one pass, so that no element must wait for the second consumer. The documentation gives the other choice: "if one iterator uses most or all of the data before another iterator starts, it is faster to use list() instead of tee()."[^7]

```python run id=one-pass
import tracemalloc


def readings():
    for i in range(200_000):
        yield i * 0.5


tracemalloc.start()
count = 0
total = 0.0
for value in readings():
    count += 1
    total += value
peak = tracemalloc.get_traced_memory()[1]
tracemalloc.stop()
print(count, total)
print("peak above 1 MB:", peak > 1_000_000)
```

```output for=one-pass
200000 9999950000.0
peak above 1 MB: False
```

### The fix in Haskell

In Haskell the same fixes have names. `foldl'` replaces `foldl`. `seq` forces a value before the program goes on, and the HaskellWiki uses it to define `foldl'`: "seq is a primitive system function that when applied to x and y will first reduce x then return y."[^4] Strict fields in a data type, written with `!`, make a constructor evaluate its fields. A History of Haskell states that with `seq` and strict fields "a programmer can move selected computations earlier, sometimes dramatically shortening the lifetimes of data structures."[^5] See [strictness annotations](/strictness-annotations/).

## How to detect it

**Heap profiles.** A heap profile is a graph of the live heap over time. The GHC User's Guide states that this graph "is useful for detecting the causes of space leaks, when your program holds on to more memory at run-time that it needs to."[^6] GHC breaks the graph down in several ways: `-hT` "by heap closure type", which does not need the profiling runtime, and with `-prof`, `-hc` by the cost-centre stack that produced the data, `-hd` by closure description and `-hy` by type.[^6] A band of thunks that grows for the whole run is the typical sign of a leak.

**A small stack.** Neil Mitchell and Tom Ellis found a simpler method. It is "based around the observation that most space leaks result in an excess use of stack."[^8] The program runs with a small stack limit, for example `+RTS -K1K`, and the stack overflow points to the leak. Mitchell suggests that a test suite keep the small limit, so that a later change that adds a leak fails the tests. He also lists a limit of the method: a leak that holds a large value without a deep chain of thunks does not use much stack, so the method does not find it.[^8]

**Memory tracing in other languages.** In Python, `tracemalloc` traces the memory blocks that Python allocates, and `get_traced_memory` returns "the current size and peak size of memory blocks traced".[^9] The examples above use the peak to show the leak.

## How to prevent it

- Use strict accumulators in loops and folds: `foldl'` instead of `foldl` in Haskell, and a plain variable instead of a chain of closures in a strict language.
- Force a value at the point where its size is known to be small, with `seq`, a bang pattern or a strict field. See [strictness annotations](/strictness-annotations/).
- Do not keep a reference to the head of a long lazy sequence while you read it. The Clojure reference calls this "Don't hang (onto) your head", and Clojure clears local variables on tail calls "to ensure that no lingering references remain on the stack".[^10]
- Read a lazy source in one pass when two results need the same data, instead of splitting it with `tee` or a similar function.
- Keep a test with a small stack or memory limit, so that a leak shows up as a failure.

## Real cases

- **The Haskell base library.** Mitchell's method found "four space leaks so far, including one in the base" library. One was a leak in `maximumBy`, which he found to be "a regression in GHC 7.10" and reported as GHC ticket 10830.[^8]
- **A TCP/IP stack in Haskell.** A History of Haskell describes a TCP/IP stack written by two students of John Hughes. "Their code turned out to contain serious space leaks". In Haskell 1.3, each `seq` on a type variable changed the type signatures of the functions around it, so the students could not fix the leaks in the time of the project. The paper states that this experience "provided ammunition for the eventual removal of class Eval in Haskell 98", which made `seq` a simple polymorphic function.[^5]
- **The clausify program.** With their heap profiler, Colin Runciman and David Wakeling reduced the peak memory of a program for propositional logic "by two orders of magnitude, from 1.3 megabytes to only 10K".[^5]

## References
