---
tag: 6PMFVG
kind: concept
slug: off-by-one-errors
name: Off-by-one errors
aliases: [off-by-one error, off by one, OBOE, OBOB, OB1, fencepost error, fence post error, picket fence error, half-open interval, inclusive bound, exclusive bound]
area: errors-and-control/loops
level: core
summary: >-
  An off-by-one error is a count or an index that is one too high or one too low, often a loop that runs one pass too many or too few. Half-open ranges prevent many of them.
applies_to: core-languages
viz: optional
formal: none
history:
  year: 1982
  people: [Edsger W. Dijkstra]
  note: In EWD 831, "Why numbering should start at zero", Dijkstra argued for ranges that include the lower bound and exclude the upper bound, and for subscripts that start at 0.
see_also: [for-loop, loop, range, slice-concept, out-of-bounds-read, out-of-bounds-write, array]
same_as:
  wikidata: Q1439356
  wikipedia: Off-by-one_error
sources:
- url: https://en.wikipedia.org/wiki/Off-by-one_error
  title: Off-by-one error, Wikipedia
  locator: Introduction; Fencepost error; Looping over arrays; Security implications
- url: https://www.cs.utexas.edu/~EWD/transcriptions/EWD08xx/EWD831.html
  title: "Edsger W. Dijkstra, Why numbering should start at zero (EWD 831), 1982"
  locator: the whole note
- url: https://docs.python.org/3/library/stdtypes.html
  title: Built-in Types, The Python Standard Library
  locator: Common Sequence Operations, notes 3 and 4; Ranges
- url: https://go.dev/ref/spec
  title: The Go Programming Language Specification
  locator: Slice expressions
- url: https://doc.rust-lang.org/std/ops/struct.Range.html
  title: Struct Range, The Rust Standard Library
  locator: Introduction
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/lang/String.html
  title: String (Java SE 25 and JDK 25)
  locator: substring(int beginIndex, int endIndex)
- url: https://en.wikipedia.org/wiki/Loop_(statement)
  title: Loop (statement), Wikipedia
  locator: Enumeration
---

## Idea

A fence 30 feet long with a post every 3 feet has 10 sections and 11 posts, not 10. Code makes the same mistake when it counts the items from index m to index n as n − m, or when a loop runs one pass too many because it tests `i <= n` and not `i < n`. Such a mistake is an off-by-one error: a number that is one more or one less than the right value.

```python run id=idea
# Fenceposts: a fence of 30 feet with a post every 3 feet.
length, gap = 30, 3
sections = length // gap
print("sections:", sections, "posts:", sections + 1)

# Items 3 to 7, both included: the count is 7 - 3 + 1, not 7 - 3.
items = list(range(3, 8))
print(items, "count:", len(items))

# Half-open ranges: the length is stop - start, and they join with no gap.
a = list(range(10))
print(len(range(2, 6)) == 6 - 2)
print(a[:4] + a[4:] == a)

# The classic bug: <= runs one pass too many and reads past the end.
scores = [70, 85, 90]
i = 0
try:
    while i <= len(scores):
        print("score", scores[i])
        i += 1
except IndexError as e:
    print("IndexError:", e)
```

```output for=idea
sections: 10 posts: 11
[3, 4, 5, 6, 7] count: 5
True
True
score 70
score 85
score 90
IndexError: list index out of range
```

The fence of 10 sections needs 11 posts. The range of items from 3 to 7 has 5 items, one more than 7 − 3. In a half-open range the count is the difference of the bounds, and two ranges that share a bound join with no gap and no overlap. The last loop tests `i <= len(scores)`, so it tries a fourth pass, and Python stops it with an `IndexError`.

## How it works

**The definition.** An off-by-one error "is a logic error that involves a number that differs from its intended value by 1"[^1]. It "often occurs in computer programming when a loop iterates one time too many or too few, usually caused by the use of non-strict inequality (≤) as the terminating condition where strict inequality (<) should have been used, or vice versa"[^1]. The other common cause is "confusion over zero-based numbering"[^1].

**Fenceposts.** "A fencepost error (occasionally called a telegraph pole, lamp-post, or picket fence error) is a specific type of off-by-one error"[^1]. The rule is that "a fence with n sections will have n + 1 posts"[^1]. The same rule holds for the gaps between items, the commas in a list, the edges between nodes in a path, and the years between two dates.

**Inclusive bounds.** A range with both ends included has one more element than the difference of its bounds. Wikipedia asks how many items lie from m to n, both included: "An intuitive answer may be n − m, but that is off by one, exhibiting a fencepost error; the correct answer is n − m + 1"[^1].

**Half-open ranges as the fix.** A half-open range includes its lower bound and excludes its upper bound. Wikipedia notes that for this reason "ranges in computing are often represented by half-open intervals"[^1]. Dijkstra compared the four ways to write the numbers 2 to 12 and chose `2 ≤ i < 13`[^2]. With this form, "the difference between the bounds as mentioned equals the length of the subsequence", and "two subsequences are adjacent means that the upper bound of the one equals the lower bound of the other"[^2]. With subscripts from 0, a sequence of length N has "the nicer range 0 ≤ i < N"[^2].

**Half-open ranges in the languages.** Many standard ranges and slices are half-open.

- In Python, "The slice of s from i to j is defined as the sequence of items with index k such that i <= k < j"[^3], and the values of a range stop before `stop`: "r[i] = start + step*i where i >= 0 and r[i] < stop"[^3].
- In Go, `a[low : high]` "constructs a substring or slice", and "The result has indices starting at 0 and length equal to high - low"[^4].
- In Rust, "The range start..end contains all values with start <= x < end. It is empty if start >= end"[^5].
- In Java, a substring "begins at the specified beginIndex and extends to the character at index endIndex - 1. Thus the length of the substring is endIndex-beginIndex"[^6].

**Loops over collections.** A loop that takes the elements of a collection has no bounds to get wrong. The Wikipedia article on loops notes that enumerations "avoid potential off-by-one errors"[^7].

## In languages

| Language | Verdict | Mechanism |
|---|---|---|
| Python | native | half-open `range` and slices; `IndexError` for an index past the end |
| JavaScript | stdlib | half-open `slice` and `substring`; an index past the end gives `undefined` and no error |
| TypeScript | stdlib | the methods of JavaScript; `noUncheckedIndexedAccess` adds `undefined` to the type of an indexed element |
| Java | native | bounds checks on arrays; half-open `substring`, `subList` and `IntStream.range` |
| Kotlin | native | `..` includes the end; `until` and `..<` exclude it; bounds checks |
| C# | native | bounds checks on arrays; the range `a[1..3]` excludes the end and `^1` is the last element |
| Go | native | half-open slice expressions; `for i := range n`; a run-time panic for an index out of range |
| Rust | native | `a..b` excludes the end and `a..=b` includes it; a panic for an index out of bounds; `get` returns `None` |
| C | idiom | no bounds checks; the convention `for (i = 0; i < n; i++)` and a pointer one past the end |
| C++ | stdlib | iterator ranges from `begin()` to `end()`, one past the last element; `at()` checks the index |
| Bash | idiom | `{1..10}` includes both ends; `${s:offset:length}` takes a length |
| PowerShell | idiom | `1..10` includes both ends |
| SQL | idiom | `BETWEEN` includes both ends; `>= a AND < b` is the half-open form |
| Regex | n/a | no indexes; the quantifier `{m,n}` includes both counts |
| HTML | n/a | no loops or indexes |
| CSS | n/a | no loops; `:nth-child()` counts from 1 |

The languages differ in two ways: the form of their ranges and what happens at a bad index. Python, Go, Rust, Java and C# have half-open ranges in the language or in the standard library, and they stop a program that reads past the end. C has no checks, so an off-by-one index reads or writes the memory next to an array. Bash, PowerShell and SQL have inclusive ranges, so their counts need the extra 1.

The example files show these differences. Python counts fenceposts, uses half-open slices and catches an `IndexError`. JavaScript reads `undefined` past the end of an array and joins words with separators between them. Java uses `substring` and catches an `ArrayIndexOutOfBoundsException`. C# uses the ranges `1..3` and `^1`. Go splits a slice into chunks with half-open slice expressions and recovers from an index panic. Rust compares `0..n` with `0..=n` and uses `windows` for neighbors. C copies a string into a buffer with room for the null character. C++ uses `end()` and `std::views::iota`.

## Variations and related ideas

- **Zero-based and one-based numbering.** "Numbering from 0 is most common, but some languages start array numbering with 1"[^1], and in FORTRAN "subscripts always start at 1"[^2]. Code that moves between them must add or remove 1 in the right places.
- **Buffer overflows.** In C, a loop with `<=` writes one element past the end of an array. Wikipedia describes a common case: `strncat` writes "a terminating null character one byte beyond the maximum length specified"[^1]. See [out-of-bounds write](/out-of-bounds-write/).
- **Separators.** A list of n items has n − 1 separators, so code that adds a comma after each item has one comma too many. `join` functions handle this.
- **Neighbors.** A list of n items has n − 1 pairs of neighbors, so a loop that compares `a[i]` with `a[i + 1]` must stop one step earlier.
- **Ranges.** See [range](/range/) and [slice](/slice-concept/) for the range and slice types of the languages.

## Pitfalls

- **`<=` with a zero-based index.** `for (i = 0; i <= n; i++)` runs n + 1 times. Use `i < n`.
- **The last element.** The last index of a list of length n is n − 1, not n.
- **Inclusive library ranges.** Some functions take an inclusive end or a count, not an exclusive end. Wikipedia gives the example of MATLAB, where `linspace(0,10,5)` gives five values, not five steps[^1]. Read the documentation of each function.
- **Loops with do-while.** Wikipedia notes that an off-by-one error "can occur if a do-while loop is used in place of a while loop (or vice versa)"[^1], because a do-while loop runs at least once.
- **Counts from dates and times.** The days from Monday to Friday, both included, are 5, not 4. Wikipedia notes that the Julian calendar first had a leap year every three years, "due to counting inclusively rather than exclusively"[^1].

## Cost

An off-by-one error costs nothing at run time and much at debug time. In languages with bounds checks, the error usually stops the program with a clear message at the bad index. In C and C++, a read past the end gives garbage and a write past the end damages other data, and Wikipedia notes that such a write "can cause an exploitable condition where an attacker can hijack the local variables for the calling routine"[^1]. Tests at the boundaries, with an empty list, a list of one element and the last element, check the cases where these errors happen.

## History

Wikipedia notes that "An early description of this error appears in the works of Vitruvius"[^1]. In 1982 Dijkstra wrote EWD 831 in favor of half-open ranges and subscripts from 0. He reported the experience with the language Mesa, which had all four forms of intervals: "the use of the other three conventions has been a constant source of clumsiness and mistakes"[^2]. He also noted that "in ALGOL 60 and in PASCAL, convention c) has been adopted"[^2], where convention c) includes both bounds. The slices of Python and Go and the ranges of Rust use the half-open form.

## References
