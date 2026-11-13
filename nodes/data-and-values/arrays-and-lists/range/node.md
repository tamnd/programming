---
tag: 9E82V0
kind: concept
slug: range
name: Range
aliases: [range type, integer range, half-open range, interval, range(), "start..end", Range<T>, IntStream.range, Enumerable.Range, iota_view, System.Range]
area: data-and-values/arrays-and-lists
level: intro
summary: >-
  A range is a sequence of numbers or positions given by a start, an end and sometimes a step. Most languages include the start and exclude the end.
applies_to: core-languages
viz: optional
formal: none
see_also: [sequence, slice-concept, array, iterators, laziness, integer-overflow, out-of-bounds-read, string-indexing]
sources:
- url: https://en.wikipedia.org/wiki/Range_(computer_programming)
  title: Range (computer programming), Wikipedia
  locator: Introduction; Range of an array; Range as an alternative to iterator
- url: https://www.cs.utexas.edu/~EWD/transcriptions/EWD08xx/EWD831.html
  title: Edsger W. Dijkstra, Why numbering should start at zero (EWD 831), 1982
  locator: Full text
- url: https://docs.python.org/3/library/stdtypes.html
  title: Built-in Types, The Python Standard Library
  locator: Ranges
- url: https://doc.rust-lang.org/std/ops/struct.Range.html
  title: Struct Range, The Rust Standard Library
  locator: Description
- url: https://doc.rust-lang.org/std/ops/struct.RangeInclusive.html
  title: Struct RangeInclusive, The Rust Standard Library
  locator: Description
- url: https://learn.microsoft.com/en-us/dotnet/csharp/tutorials/ranges-indexes
  title: Indices and ranges, C# documentation
  locator: Language support for indices and ranges
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/util/stream/IntStream.html
  title: Interface IntStream, Java SE 25 API
  locator: range; rangeClosed
- url: https://kotlinlang.org/docs/ranges.html
  title: Ranges and progressions, Kotlin documentation
  locator: Ranges; Progression
- url: https://learn.microsoft.com/en-us/dotnet/api/system.linq.enumerable.range
  title: Enumerable.Range method, .NET API reference
  locator: Definition; Exceptions
- url: https://go.dev/ref/spec
  title: The Go Programming Language Specification
  locator: For statements with range clause
- url: https://en.cppreference.com/cpp/ranges/iota_view
  title: std::ranges::views::iota, std::ranges::iota_view, cppreference.com
  locator: Description
- url: https://github.com/tc39/proposal-iterator.range
  title: Iterator.range proposal, TC39
  locator: README
- url: https://www.postgresql.org/docs/current/rangetypes.html
  title: Range Types, PostgreSQL documentation
  locator: Built-in Range and Multirange Types; Inclusive and Exclusive Bounds; Discrete Range Types
---

## Idea

A range names a run of numbers by its ends instead of by its items. `range(2, 13)` in Python means the numbers 2, 3 and so on up to 12. A range usually has three parts: a start, an end and a step, which is 1 when the code does not give one. A program uses a range to count the passes of a loop, to name a part of an array, or to test whether a value is between two bounds.

Wikipedia lists three meanings of the word in programming: "The possible values that may be stored in a variable. The upper and lower bounds of an array. An alternative to iterator"[^1]. This page is about the second and third meanings: a range as a value that stands for a sequence of numbers or positions.

The most important choice is whether each end is part of the range. Most languages use a half-open range: the start is in the range and the end is not. Dijkstra gave the reasons for this choice in 1982[^2].

```python run id=idea
# range(start, stop) gives start, start + 1, ..., stop - 1.
r = range(2, 13)
print(list(r))
print(len(r), 13 - 2, 2 in r, 13 in r)

# Split at 7: the halves share the bound 7, and each number is in exactly one half.
lo, hi = range(2, 7), range(7, 13)
print(list(lo), list(hi), len(lo) + len(hi))

# A range with a step, a range that counts down, and an empty range.
print(list(range(0, 10, 3)), list(range(5, 0, -1)), list(range(5, 5)))
```

```output for=idea
[2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12]
11 11 True False
[2, 3, 4, 5, 6] [7, 8, 9, 10, 11, 12] 11
[0, 3, 6, 9] [5, 4, 3, 2, 1] []
```

The range from 2 to 13 has 11 items, and 11 is 13 minus 2. The two halves at 7 use the same number as the end of one and the start of the other, and together they have all 11 items. The range from 5 to 5 is empty, and no special case is necessary for it.

## How it works

**Four conventions.** To name the numbers 2 to 12, Dijkstra lists "four conventions are open to us a) 2 ≤ i < 13 b) 1 < i ≤ 12 c) 2 ≤ i ≤ 12 d) 1 < i < 13"[^2]. With a) and b), "the difference between the bounds as mentioned equals the length of the subsequence"[^2], and two ranges are adjacent when "the upper bound of the one equals the lower bound of the other"[^2]. To choose between them, he looks at ranges that start at 0. An excluded lower bound would then have to be -1, which is not a natural number, and an included upper bound would have to be -1 for the empty range. "That is ugly, so for the upper bound we prefer < as in a) and d). We conclude that convention a) is to be preferred"[^2].

Convention a) is the half-open range `[start, end)`. It has these properties:

- the length is `end - start`;
- the range is empty when `start >= end`, so a loop over it runs zero times;
- the ranges `[a, m)` and `[m, b)` split `[a, b)` with no gap and no overlap;
- the indices of an array of length `n` are the range `[0, n)`.

**A range is small.** A range stores its bounds and not its items. Python says: "The advantage of the range type over a regular list or tuple is that a range object will always take the same (small) amount of memory, no matter the size of the range it represents (as it only stores the start, stop and step values, calculating individual items and subranges as needed)"[^3]. So `len`, `in` and indexing on a range are arithmetic. In the Python example, `range(0, 10**18, 2)` has 500000000000000000 items, and `10**17 in big` is fast.

**A range as a type.** In Python, Rust and C#, a range is a value that the program can store and pass. Rust defines `Range` as "A (half-open) range bounded inclusively below and exclusively above (start..end)", and "The range start..end contains all values with start <= x < end. It is empty if start >= end"[^4]. Rust has a second type for an included end: "The RangeInclusive start..=end contains all values with x >= start and x <= end"[^5]. In other languages a range is only a loop form or a function that gives a stream of numbers.

**Ranges and slices.** A range of positions selects a part of an array or a string. In C#, "The start of the range is inclusive, but the end of the range is exclusive"[^6], and `^n` counts from the end. In Go and Python, `a[1:3]` uses the same half-open rule. See [slice](/slice-concept/).

**Ranges of iterators.** C++ and D use the word for a pair of iterators. Wikipedia describes this meaning as "a pair of begin/end iterators packed together"[^1]. A C++ `std::vector` is a range in this sense, and `std::views::iota` is a range that makes its numbers as the loop asks for them. See [iterators](/iterators/).

## In languages

| Language | Verdict | Mechanism |
|---|---|---|
| Python | native | `range(stop)`, `range(start, stop, step)`, an immutable sequence with `len`, `in` and slicing |
| JavaScript | idiom | no range type; `Array.from({ length: n }, (_, i) => i)`, `keys()` or a generator; `Iterator.range` is a proposal |
| TypeScript | idiom | the JavaScript idioms with the type `number[]` or `Generator<number>` |
| Java | stdlib | the `for` loop; `IntStream.range` and `IntStream.rangeClosed` |
| Kotlin | native | `a..b` and `a..<b` with `step` and `downTo`; the types `IntRange` and `IntProgression` |
| C# | native | `a..b` and `^n` (`System.Range` and `System.Index`) for indices; `Enumerable.Range(start, count)` for numbers |
| Go | native | `for i := range n` (Go 1.22); `a[low:high]` for slices; an `iter.Seq` function for other ranges |
| Rust | native | `a..b`, `a..=b`, `a..` and `..b`, with `step_by`, `rev` and `contains` |
| C | idiom | `for (i = start; i < end; i++)`; a struct with two bounds |
| C++ | stdlib | `std::views::iota` (C++20) with `stride` (C++23) and `reverse`; any begin and end pair is a range |
| Bash | native | brace expansion `{1..5}` and `{0..10..2}`; the arithmetic `for ((i = 0; i < n; i++))` |
| PowerShell | native | the range operator `1..5`, which includes both ends |
| SQL | native | in PostgreSQL, range types such as `int4range` and `tsrange`; `generate_series` for rows |
| Regex | idiom | the character range `[a-z]` and the count range `{2,5}`, both with included ends |
| HTML | n/a | markup; `<input type="range">` is a slider control, not a range value |
| CSS | n/a | style rules have no range values; media queries can test a range of widths |

The languages do not agree on the end. Python, Go, Rust `..`, C# `..`, Kotlin `..<` and Java `IntStream.range` exclude the end. Kotlin `..`, Rust `..=`, Java `rangeClosed`, PowerShell and Bash include it. Java documents both: `range` goes "from startInclusive (inclusive) to endExclusive (exclusive) by an incremental step of 1", and `rangeClosed` goes "from startInclusive (inclusive) to endInclusive (inclusive) by an incremental step of 1"[^7]. Kotlin says that `1..4` "represents the numbers 1, 2, 3, and 4"[^8]. C# `Enumerable.Range` takes a count, not an end: it "Generates a sequence of integral numbers within a specified range"[^9], so `Enumerable.Range(2, 5)` gives 2 to 6.

Go added a range over an integer in Go 1.22. "For an integer value n, where n is of integer type or an untyped integer constant, the iteration values 0 through n-1 are produced in increasing order"[^10], and "If n <= 0, the loop does not run any iterations"[^10]. C++20 added `std::views::iota`, "A range factory that generates a sequence of elements by repeatedly incrementing an initial value. Can be either bounded or unbounded (infinite)"[^11]. JavaScript has no range yet. The TC39 proposal for `Iterator.range` gives its stage as 2, and it notes that one Stack Overflow question has at least 20 different implementations of a range function[^12].

The example files show these forms. Python shows `len`, `in`, `index` and slicing on ranges, a huge range that takes no memory, and the error for a step of 0. JavaScript builds ranges with `Array.from` and a generator with a step. Java uses `range`, `rangeClosed`, `iterate` with a condition and the `for` loop. C# shows that `Enumerable.Range` takes a count, `System.Range` with `^` on an array, and `GetOffsetAndLength`. Go uses `range` over an integer, a negative count, an `iter.Seq` with a step and slice bounds. Rust shows `..` and `..=`, `contains`, `step_by`, `rev`, a range of all `u8` values and ranges as slice indices. C writes a range struct and splits it in two. C++ uses `iota`, `take`, `stride`, `reverse` and `subrange`.

## Variations and related ideas

- **Inclusive ranges.** A closed range `[a, b]` has `b - a + 1` items. It is natural for ranges of values such as dates or the letters `a` to `z`, and it is the only way to name a range that ends at the largest value of a type. Rust notes that `Range<u8>` "cannot represent the range that covers all of u8"[^4]. In the Rust example, `0u8..=255` has 256 items.
- **Steps.** A range with a step, such as `range(0, 10, 3)`, gives every third number. Kotlin calls this a progression, and `0..8 step 2` gives `02468`[^8]. A negative step counts down.
- **Open-ended ranges.** Rust `a..` and C++ `iota(a)` have no end. They are infinite lazy sequences, and the program must stop them, for example with `take`. See [laziness](/laziness/).
- **Ranges of indices.** C# `1..^1` and Python `slice(1, -1)` name a part of any array, and the actual positions depend on the length. C# `GetOffsetAndLength` turns the range into an offset and a length for a given length.
- **Ranges as data.** PostgreSQL stores ranges in columns. "Range types are data types representing a range of values of some element type"[^13]. A range value can be empty or unbounded, and its text form shows each bound: "an inclusive lower bound is represented by “[” while an exclusive lower bound is represented by “(”"[^13]. For a discrete type, where "A discrete range is one whose element type has a well-defined “step”"[^13], the built-in types "int4range, int8range, and daterange all use a canonical form that includes the lower bound and excludes the upper bound"[^13].
- **Ranges of a type.** A type such as a 16-bit integer has a range of values. Wikipedia's example: "the range of a signed 16-bit integer variable is all the integers from −32,768 to +32,767"[^1]. See [integer overflow](/integer-overflow/).

## Pitfalls

- **Off by one.** The most common range error is a wrong end. `IntStream.range(1, 100).sum()` is 4950, not the sum of 1 to 100, because the end is excluded; `rangeClosed(1, 100)` gives 5050. Read the rule of each language: Kotlin `..` and PowerShell `..` include the end, and Rust `..` and C# `..` exclude it.
- **Count or end.** `Enumerable.Range(2, 5)` in C# ends at 6, because the second argument is a count. The same call in Python, `range(2, 5)`, ends at 4.
- **Unsigned count down.** In C, `for (size_t i = n - 1; i >= 0; i--)` never ends, because an unsigned `i` is always 0 or more and wraps around after 0. The C example uses `for (size_t i = n; i-- > 0;)`, which tests before it subtracts.
- **Overflow at the end.** A loop such as `for (unsigned char i = 0; i <= 255; i++)` never stops, because `i` wraps from 255 to 0. Rust's `0u8..=255` and Python's arbitrary-size integers avoid this.
- **Empty and backward ranges.** `range(5, 2)` and Rust `5..2` are empty, not a count down. A count down needs a negative step or `rev`. A step of 0 is an error in Python and does not end in a hand-written loop.
- **Out of bounds.** A range that goes past the end of an array panics in Rust, throws in C# and Java, and reads memory outside the array in C. Rust's `get(3..9)` returns `None` instead. See [out-of-bounds read](/out-of-bounds-read/).
- **Floating-point ranges.** A range with a fractional step gathers rounding errors, so the last item can be missing or extra. Compute each item as `start + i * step` from an integer range instead.

## Cost

| Operation | Range value (Python, Rust, C++ `iota`) | Materialized list |
|---|---|---|
| make | constant | linear in the length |
| memory | the bounds and the step | all items |
| length | constant (arithmetic) | constant |
| item `i` | constant (arithmetic) | constant |
| membership test `x in r` | constant (arithmetic) | linear |
| loop over all items | linear, no allocation | linear |

A loop over a range needs only a counter, as in `for (i = a; i < b; i++)`, so it costs the same as a counting loop with an index. A range that the program turns into a list, such as `list(range(n))` or `IntStream.range(0, n).boxed().toList()`, pays for memory for each item.

## History

Dijkstra wrote "Why numbering should start at zero" (EWD 831) on 11 August 1982[^2]. He gave the half-open convention and noted the experience with the Mesa language at Xerox PARC, which had notations for all four conventions: "Extensive experience with Mesa has shown that the use of the other three conventions has been a constant source of clumsiness and mistakes"[^2]. The note is also the usual argument for [indices from 0](/array-indexing/), because the indices of an array of length `n` are then `0 ≤ i < n`.

Later languages made half-open ranges the default. Python's `range`, the C++ iterator pair `[begin, end)`, Go slice expressions and Rust `..` all exclude the end. Newer additions are the C++20 `iota_view`[^11], the Go 1.22 range over integers[^10] and the C# index and range operators `^` and `..`[^6]. The TC39 proposal for `Iterator.range` is at stage 2[^12].

## References
