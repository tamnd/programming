---
tag: H47ZC9
kind: technique
slug: strictness-annotations
name: Strictness annotations
aliases: [strictness annotation, bang patterns, strict fields, strictness flags, seq, forcing evaluation, force a lazy value]
area: functions-and-abstraction/lazy-evaluation
level: advanced
summary: >-
  Mark a value, an argument or a field so that the program evaluates it at a chosen point and does not keep it as a thunk. Strict languages use the same idea to force a lazy sequence.
see_also: [laziness, thunk, space-leak, strict-evaluation, lazy-sequence, use-lazy-sequences]
sources:
- url: https://downloads.haskell.org/ghc/9.14.1/docs/users_guide/exts/strict.html
  title: Bang patterns and Strict Haskell, Glasgow Haskell Compiler 9.14.1 User's Guide
  locator: 6.14 Bang patterns and Strict Haskell; Strict-by-default data types; Modularity
- url: https://www.haskell.org/onlinereport/haskell2010/haskellch6.html
  title: Predefined Types and Classes, Haskell 2010 Language Report
  locator: 6.2 Strict Evaluation
- url: https://downloads.haskell.org/ghc/9.14.1/docs/users_guide/profiling.html
  title: Profiling, Glasgow Haskell Compiler 9.14.1 User's Guide
  locator: 8.5 Profiling memory usage; Retainer Profiling
- url: https://hackage.haskell.org/package/deepseq-1.5.2.0/docs/Control-DeepSeq.html
  title: Control.DeepSeq, deepseq 1.5.2.0
  locator: deepseq; force; rwhnf
- url: https://www.haskell.org/onlinereport/haskell2010/haskellch4.html
  title: Declarations and Bindings, Haskell 2010 Language Report
  locator: 4.2.1 Algebraic Datatype Declarations, Strictness Flags
- url: https://www.microsoft.com/en-us/research/wp-content/uploads/2016/07/history.pdf
  title: A History of Haskell, Being Lazy With Class, P. Hudak, J. Hughes, S. Peyton Jones and P. Wadler, HOPL III, 2007
  locator: 10.3 Controlling evaluation order
- url: https://docs.python.org/3.13/reference/expressions.html
  title: Expressions, The Python Language Reference 3.13
  locator: 6.2.8 Generator expressions
- url: https://learn.microsoft.com/en-us/dotnet/csharp/linq/get-started/introduction-to-linq-queries
  title: Introduction to LINQ Queries in C#, Microsoft Learn
  locator: Classification of standard query operators by manner of execution
- url: https://docs.oracle.com/en/java/javase/21/docs/api/java.base/java/util/stream/package-summary.html
  title: Package java.util.stream, Java SE 21 API
  locator: Laziness-seeking; Non-interference
---

## Purpose

A lazy language evaluates an expression only when the program needs its value. Until then, the program keeps a [thunk](/thunk/): the expression and the data it refers to. This is often good, but a loop that updates an accumulator lazily builds a chain of thunks, one for each step, and the chain can use much more memory than the value. This is a [space leak](/space-leak/). The GHC User's Guide gives the reason for the technique: "In high-performance Haskell code (e.g. numeric code) eliminating thunks from an inner loop can be a huge win."[^1]

A strictness annotation tells the language to evaluate a value at a chosen point. The GHC User's Guide describes its extensions as a way "to allow the programmer to specify use of strict (call-by-value) evaluation rather than lazy (call-by-need) evaluation."[^1] The Haskell report says the same of its basic tool, `seq`: it "is usually introduced to improve performance by avoiding unneeded laziness."[^2]

The annotation does not change what the program computes when the value is defined. It changes when the program computes it. It can change the result only when the value is undefined, for example an error or a loop that does not stop, because the program then evaluates that value earlier.

## Steps

1. **Find the thunks.** Measure before you annotate. A heap profile shows which data fills the memory. The GHC User's Guide says that heap profiling "is useful for detecting the causes of space leaks, when your program holds on to more memory at run-time that it needs to."[^3] A retainer profile answers the question "why is this data being retained?"[^3]
2. **Choose the depth.** Most annotations evaluate a value to weak head normal form (WHNF): the outer constructor, but not the parts inside it. For an `Int`, this is the whole value. For a pair or a list, it is only the first layer. If the thunks are inside the structure, you need full evaluation, which the deepseq package calls "Normal Form".[^4]
3. **Choose the tool.** Use the smallest annotation that removes the thunks:
   - `seq` and `$!` for one expression. `seq a b` evaluates `a` and then gives `b`. The report defines `f $! x = x `seq` f x` and says that "The operator $! is strict (call-by-value) application".[^2]
   - A bang pattern on a function argument or a `let` binding. "Matching an expression e against a pattern !p is done by first evaluating e (to WHNF) and then matching the result against p."[^1]
   - A strictness flag on a field of a data type. "Whenever a data constructor is applied, each argument to the constructor is evaluated if and only if the corresponding type in the algebraic datatype declaration has a strictness flag, denoted by an exclamation point".[^5]
   - `StrictData` or `Strict` for a whole module. `StrictData` makes "fields of data types defined in the current module strict by default", and `Strict` "switches functions, data types, and bindings to be strict by default, allowing optional laziness by adding ~".[^1]
   - `deepseq` or `force` for full depth. `deepseq` "fully evaluates the first argument, before returning the second", and `force x = x `deepseq` x`.[^4]
4. **Measure again.** Run the same profile and check that the peak memory fell. If it did not, the thunks are somewhere else, so remove the annotation and go back to step 1.

The usual first target is an accumulator. This loop sums a list, and the bang on `acc` makes each step add the number before the next call:

```haskell
{-# LANGUAGE BangPatterns #-}

mean :: [Double] -> Double
mean xs = go 0 0 xs
  where
    go :: Double -> Int -> [Double] -> Double
    go !total !count []       = total / fromIntegral count
    go !total !count (y : ys) = go (total + y) (count + 1) ys
```

Without the two bangs, `total` and `count` each grow to a chain of additions as long as the list. A strict field does the same for a data type. The fields of this pair are never thunks, because the constructor evaluates them:

```haskell
data Sum = Sum !Double !Int
```

The History of Haskell shows the same form of declaration: `data SList a = SNil | SCons !a !(SList a)`.[^6]

## Example

Strict languages have lazy parts too: generators, streams and queries. The value of such an expression is a recipe, and it runs only when the program reads it. The technique is the same: force the value at the point where its inputs are correct. A Python generator expression reads its variables late. The Python reference says: "Variables used in the generator expression are evaluated lazily when the __next__() method is called for the generator object".[^7]

Before: both values are lazy. The first one reads `rate` after the program changes it. The second one reads a file object after the `with` block closes it, so the error appears far from its cause:

```python wrong id=lazy-reads
import io


def read_numbers(text):
    with io.StringIO(text) as source:
        return (int(line) for line in source)


rate = 2
prices = [10, 20, 30]
scaled = (price * rate for price in prices)
rate = 3
print(list(scaled))

numbers = read_numbers("1\n2\n3\n")
try:
    print(sum(numbers))
except ValueError as error:
    print("error:", error)
```

```output for=lazy-reads
[30, 60, 90]
error: I/O operation on closed file
```

After: each value is forced to a list at the point where its inputs are correct. A list comprehension is the strict form of a generator expression:

```python run id=forced-reads
import io


def read_numbers(text):
    with io.StringIO(text) as source:
        return [int(line) for line in source]


rate = 2
prices = [10, 20, 30]
scaled = [price * rate for price in prices]
rate = 3
print(scaled)

numbers = read_numbers("1\n2\n3\n")
print(sum(numbers))
```

```output for=forced-reads
[20, 40, 60]
6
```

The deepseq documentation gives the same use in Haskell: a full force is "useful for forcing pending exceptions, eradicating space leaks, or forcing lazy I/O to happen."[^4] The example files force a Java stream and a C# query in the same way.

## When to use

- An accumulator in a loop, a fold or a recursive function, when a profile shows that thunks fill the heap.
- A field of a record that the program updates many times, such as a counter or a running total. A strict field stops each update from keeping the one before it.
- Data that the program reads from a resource, such as a file or a connection, before the program closes the resource. The deepseq documentation says of `force`: "A typical use is to prevent resource leaks in lazy IO programs".[^4]
- In a strict language: a lazy sequence or query whose source changes or closes before the program reads the result. Microsoft says: "To force immediate execution of any query and cache its results, you can call the ToList or ToArray methods."[^8]

Do not annotate everything. A value that the program may never use, an infinite structure and a value that is expensive to compute are better left lazy.

## Pitfalls

- **WHNF is shallow.** A bang or `seq` evaluates only the outer constructor. The deepseq documentation says that "seq is shallow in the sense that it only evaluates the top level of its argument".[^4] A bang on a pair does not evaluate the two parts, and a strict list field does not make the elements strict. GHC states that "Bang patterns can be nested: f2 (!x, y) = [x,y] Here, f2 is strict in x but not in y."[^1]
- **A full force of an infinite structure never stops.** `deepseq` reduces a value "to normal form (that is, fully evaluate all sub-components)"[^4], so on an infinite list or a cyclic structure it does not return. It also costs a full walk of the structure each time.
- **An earlier error.** A strict value is evaluated even on a path that would not use it. If the value is undefined, the program now fails where a lazy program would succeed. GHC says: "if v is bottom, the match diverges".[^1]
- **Strict by default has limits.** GHC warns that "Strict and StrictData only affects definitions in the module they are used in." and that "Entities defined in other modules might rely on laziness for correctness".[^1] A module with `Strict` can still get lazy values from a library.
- **Annotations do not change the type.** "Bang patterns and strict matching do not affect the type system in any way."[^1] The compiler does not tell you that a value is lazy, so the profile in step 1 is the check.
- **`seq` changes the semantics of the language.** The Haskell report says that `seq` "has important semantic consequences, because it is available at every type", and that "the existence of seq weakens Haskell’s parametricity properties."[^2]
- **A lazy sequence in a strict language reads its source late.** Java says: "For well-behaved stream sources, the source can be modified before the terminal operation commences and those modifications will be reflected in the covered elements."[^9] Force the stream before the source changes if you need the old elements.

## Tools that do it

| Language | One value | Fields | Whole module | Full depth |
|---|---|---|---|---|
| Haskell 2010 | `seq`, `$!` | `!` in a data declaration | none | none |
| GHC | bang patterns (`BangPatterns`) | `!` fields | `StrictData`, `Strict` | `deepseq`, `force`, `$!!` |
| Python | `list(...)`, a comprehension | not applicable | not applicable | `list` at each level |
| Java | a terminal operation such as `toList()` | not applicable | not applicable | collect at each level |
| C# | `ToList`, `ToArray` | not applicable | not applicable | `ToList` at each level |

GHC added `BangPatterns` in version 6.8.1 and `StrictData` and `Strict` in version 8.0.1, and "Bang patterns are available by default as a part of GHC2021".[^1]

The annotations are older than Haskell. A History of Haskell says that `seq` and strict fields came in Haskell 1.3 in 1996, that "seq was primarily introduced to improve the speed of Haskell programs!", and that both were "already present in Miranda for the same reasons (Turner, 1985)".[^6]

## References
