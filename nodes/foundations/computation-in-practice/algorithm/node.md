---
tag: 78SNNV
kind: concept
slug: algorithm
name: Algorithm
aliases: [algorithm, algorithms, procedure, method of computation, effective procedure, computational method, step-by-step procedure]
area: foundations/computation-in-practice
level: core
summary: >-
  An algorithm is a finite list of exact steps that takes an input and gives the answer to every instance of a problem. A program is one way to write an algorithm down for a machine.
applies_to: core-languages
viz: optional
formal: none
history:
  year: 1936
  people: [Euclid, Muhammad ibn Musa al-Khwarizmi, Alan Turing, Alonzo Church, Donald Knuth, Jon Bentley, Joshua Bloch]
  note: Euclid wrote down his method for the greatest common divisor in about 300 BC, and the word comes from the name of al-Khwarizmi. Church and Turing gave exact definitions of an effective procedure in 1936.
see_also: [computer-program, recursion, finite-state-machines, integer-overflow, sort-a-list-of-records, remove-duplicates-from-a-list]
same_as:
  wikidata: Q8366
  wikipedia: Algorithm
sources:
- url: https://xlinux.nist.gov/dads/HTML/algorithm.html
  title: algorithm, Dictionary of Algorithms and Data Structures, NIST
  locator: Definition
- url: https://docs.python.org/3/library/bisect.html
  title: bisect, Array bisection algorithm, Python documentation
  locator: Introduction
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/util/Arrays.html
  title: Class Arrays, Java SE 25 API documentation
  locator: binarySearch(long[] a, long key)
- url: https://pkg.go.dev/slices
  title: Package slices, Go standard library documentation
  locator: func BinarySearch
- url: https://doc.rust-lang.org/std/primitive.slice.html
  title: Primitive type slice, Rust standard library documentation
  locator: method binary_search
- url: https://en.cppreference.com/w/c/algorithm/bsearch.html
  title: bsearch, bsearch_s, cppreference.com
  locator: Description
- url: https://learn.microsoft.com/en-us/dotnet/api/system.array.binarysearch
  title: Array.BinarySearch Method, .NET API documentation
  locator: Returns and remarks
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Array/sort
  title: Array.prototype.sort(), MDN Web Docs
  locator: Description
- url: https://research.google/blog/extra-extra-read-all-about-it-nearly-all-binary-searches-and-mergesorts-are-broken/
  title: Joshua Bloch, Extra, Extra, Read All About It, Nearly All Binary Searches and Mergesorts are Broken, Google Research blog, 2006
  locator: Main text
- url: https://en.cppreference.com/w/cpp/algorithm/lower_bound.html
  title: std::lower_bound, cppreference.com
  locator: Description
---

## Idea

An algorithm is a method to solve a problem, written as steps so exact that a machine or a careful person can follow them with no judgment. It takes an input, does a finite number of steps and gives an output, and it must work for every input of the problem, not only for the examples that the author tried. A program is an algorithm written in a programming language, together with the details that the language and the machine need. One problem usually has many algorithms, and they can differ greatly in the number of steps they take.

```python run id=idea
def linear_search(xs, target):  # look at each item in turn
    steps = 0
    for i, x in enumerate(xs):
        steps += 1
        if x == target:
            return i, steps
    return -1, steps


def binary_search(xs, target):  # xs must be sorted
    lo, hi, steps = 0, len(xs), 0
    while lo < hi:  # the target, if present, is in xs[lo:hi]
        steps += 1
        mid = (lo + hi) // 2
        if xs[mid] < target:
            lo = mid + 1
        else:
            hi = mid
    found = lo < len(xs) and xs[lo] == target
    return (lo if found else -1), steps


xs = list(range(0, 2_000_000, 2))  # one million even numbers, sorted
for t in (10, 1_999_998, 7):
    print(f"target {t:>9}: linear {linear_search(xs, t)}, binary {binary_search(xs, t)}")

print("unsorted input:", binary_search([3, 1, 2], 3))


def collatz_steps(n):  # exact steps, but nobody has proved that it stops for every n
    steps = 0
    while n != 1:
        n = n // 2 if n % 2 == 0 else 3 * n + 1
        steps += 1
    return steps


print("collatz steps for 27:", collatz_steps(27))
```

```output for=idea
target        10: linear (5, 6), binary (5, 20)
target   1999998: linear (999999, 1000000), binary (999999, 20)
target         7: linear (-1, 1000000), binary (-1, 20)
unsorted input: (-1, 2)
collatz steps for 27: 111
```

The program solves one problem, find a number in a sorted list, with two algorithms. Linear search looks at each item in turn, so in the worst case it looks at all one million items. Binary search compares the target with the middle item and discards half of the list at each step, so it never needs more than 20 steps, because 2 to the power 20 is more than one million. Binary search is faster only because it uses a fact about the input: the list is sorted. With an unsorted list, it gives a wrong answer and no error. The last function follows exact steps, but nobody has proved that it stops for every starting number, so it is not known to be an algorithm.

## How it works

**What makes a procedure an algorithm.** The NIST dictionary defines an algorithm as "A computable set of steps to achieve a desired result."[^1] Donald Knuth gave five properties in *The Art of Computer Programming*. Finiteness: it stops after a finite number of steps for every input. Definiteness: each step is exact and has one meaning. Input: it takes zero or more values. Output: it gives one or more values that relate to the input. Effectiveness: each step is basic enough that a person with pencil and paper could do it in a finite time. A procedure that can run forever, such as a server loop or the Collatz function above, is a computation but not an algorithm in this strict sense.

**Problem, algorithm and program.** A problem states what the answer must be for each input, for example "the index of the target in a sorted list, or a sign that it is not there". An algorithm states how to get the answer. A program states the algorithm in one language, and adds choices that the algorithm leaves open: the types of the values, what happens at the limits of the machine, how errors are reported and what the function returns when the target is absent. The algorithm is the same in each language of this page; the contracts of the library functions differ.

**Preconditions and correctness.** An algorithm is correct when it gives the right output for every input that meets its precondition. Binary search has the precondition that the input is sorted. The library functions state it in their documentation: Java's `Arrays.binarySearch` says "The array must be sorted (as by the sort(long[]) method) prior to making this call. If it is not sorted, the results are undefined."[^3] Rust says that "If the slice is not sorted, the returned result is unspecified and meaningless."[^5] The usual way to show that a loop is correct is an invariant, a statement that is true before and after each pass. For binary search it is "if the target is in the list, it is between `lo` and `hi`". Each pass makes the range smaller, so the loop stops, and when it stops the invariant gives the answer.

**Cost.** Two algorithms for one problem are compared by how the number of steps grows with the size of the input, written in big-O notation. Linear search takes O(n) steps and binary search O(log n) steps. For small inputs the difference is small, and a simple algorithm with a small constant can be faster. For large inputs the growth rate decides. The analysis of algorithms is a field of its own, and [the algorithm site](https://algorithm.tamnd.com/) covers it.

**Algorithms in the standard library.** Programs seldom write the classic algorithms again. Each language ships sorting and searching, and the documentation gives the contract. Python's `bisect` module keeps "a list in sorted order without having to sort the list after each insertion".[^2] Go's `slices.BinarySearch` "returns the earliest position where target is found, or the position where target would appear in the sort order; it also returns a bool saying whether the target is really found in the slice."[^4] C's `bsearch` takes a comparison function and needs an array in which "all the elements that compare less than must appear before all the elements that compare equal to, and those must appear before all the elements that compare greater than the key object."[^6] C++'s `std::lower_bound` "Searches for the first element in the partitioned source range [first, last) which is not ordered before value".[^10] .NET returns "the bitwise complement of the index of the first element that is larger than value" when the value is absent.[^7] JavaScript has no binary search, and its sort has a surprise: "The default sort order is ascending, built upon converting the elements into strings".[^8]

**Algorithms and data structures.** An algorithm works on data in some layout, and the layout decides which algorithms are possible. Binary search needs random access to the middle item, so it works on an array but not on a linked list. A hash table finds a key in constant time on average and has no order, so it cannot answer "the first key greater than 50". Niklaus Wirth gave a book the title *Algorithms + Data Structures = Programs* in 1976 for this reason.

## In languages

| Language | Verdict | Mechanism |
|---|---|---|
| Python | stdlib | `list.sort`, `sorted`, the `bisect` module |
| JavaScript | stdlib | `Array.prototype.sort`; no binary search |
| TypeScript | stdlib | as JavaScript |
| Java | stdlib | `Arrays.sort`, `Arrays.binarySearch`, `Collections` |
| Kotlin | stdlib | `sorted`, `sortedBy`, `binarySearch` on lists |
| C# | stdlib | `Array.Sort`, `Array.BinarySearch`, LINQ `OrderBy` |
| Go | stdlib | `slices.Sort`, `slices.BinarySearch`, `sort.Search` |
| Rust | stdlib | `sort`, `sort_unstable`, `binary_search`, `partition_point` |
| C | stdlib | `qsort` and `bsearch` with a comparison function |
| C++ | stdlib | `<algorithm>`: `std::ranges::sort`, `lower_bound` |
| Bash | idiom | the `sort` command; loops for the rest |
| PowerShell | stdlib | `Sort-Object`; .NET methods |
| SQL | native | `ORDER BY`; the engine chooses the algorithm |
| Regex | n/a | the engine runs a matching algorithm |
| HTML | n/a | markup, not computation |
| CSS | n/a | the layout algorithm belongs to the browser |

Every general-purpose language ships the common algorithms, and the differences are in the contracts. A search that finds nothing returns the insertion point with no flag in Python's `bisect`, a negative number in Java and C#, a pair in Go, a `Result` in Rust, a null pointer in C and an iterator in C++. Sorts differ in stability: Python's `sort`, Java's object sort, Rust's `sort` and C#'s LINQ `OrderBy` keep equal items in their order, while C#'s `Array.Sort`, Go's `slices.Sort`, Rust's `sort_unstable` and C++'s `std::sort` do not. SQL is the clearest case of an algorithm that the programmer does not write: a query states the result, and the planner chooses the sort, the join and the index to use.

The example files show the same contracts. Python shows `bisect_left`, `insort` and the `key` argument, and a wrong block searches an unsorted list. JavaScript shows the string order of the default sort. Java prints the overflow that Joshua Bloch found in the textbook binary search. Go shows `sort.Search` on a predicate, Rust shows `partition_point`, C shows a comparison function that cannot overflow, and C++ shows `lower_bound` and a sort by a projection.

## Variations and related ideas

- **Kinds of algorithm.** Deterministic algorithms give the same steps for the same input. Randomized algorithms use random choices, such as quicksort with a random pivot. Approximation algorithms give an answer close to the best one when the best is too costly to find. Online algorithms handle the input one item at a time.
- **Heuristics.** A heuristic is a rule that usually gives a good answer quickly but has no proof that it is correct or best.
- **Computational method.** Knuth's name for a procedure that has all the properties of an algorithm except finiteness, such as an operating system or a server.
- **Algorithm design techniques.** Divide and conquer, [recursion](/recursion/), dynamic programming, greedy choice and backtracking are general ways to find an algorithm.
- **State machines.** A [finite-state machine](/finite-state-machines/) is a restricted algorithm with a fixed amount of memory.
- **Declarative programs.** In SQL and in logic programming, the program states the problem and the system chooses the algorithm.

## Pitfalls

- **Forgetting the precondition.** Binary search on unsorted data, or with a comparison that does not match the sort order, gives wrong answers and no error.
- **Off-by-one errors.** Algorithms on ranges must choose closed or half-open ranges and keep the choice. The half-open form `[lo, hi)` of this page makes the empty range `lo == hi`.
- **Overflow inside the algorithm.** `(lo + hi) / 2` overflows when the indexes are large, a bug that stayed in the JDK for about nine years. Bloch writes that it "fails if the sum of low and high is greater than the maximum positive int value".[^9] Use `lo + (hi - lo) / 2`. A C comparison function that returns `a - b` overflows in the same way.
- **The default sort order.** JavaScript sorts numbers as strings by default, so `[10, 9, 1].sort()` gives `[1, 10, 9]`.
- **Unstable sorts.** Sorting records by a second key after the first one keeps the first order only when the sort is stable.
- **Proof by examples.** An algorithm that passes the tests can fail on the empty input, one item, all equal items or the largest input. Test the edges, or test properties over random inputs.
- **Cost that grows hidden.** A loop that calls a linear search, or `list.insert` at the front, inside another loop is quadratic. The program is fast on test data and slow on real data.

## Cost

The cost of an algorithm is the number of basic steps and the memory it uses, as a function of the size of the input. The worst case gives a guarantee, the average case describes typical inputs, and the amortized cost averages over a sequence of operations. Big-O notation hides constants, so cache behavior, the cost of a comparison and the size of the input decide which of two algorithms is faster in practice. Binary search takes about log2(n) comparisons: 20 for a million items and 30 for a billion. A comparison sort needs about n log2(n) comparisons in the worst case, and no comparison sort can do better.

## History

Euclid described his method for the greatest common divisor in the *Elements* in about 300 BC, and it is still in use. The word algorithm comes from the Latin form of the name of Muhammad ibn Musa al-Khwarizmi, a scholar in Baghdad in the ninth century whose book on calculation with Hindu-Arabic numerals reached Europe in translation. For centuries "algorism" meant arithmetic with these numerals.

The question of what a mechanical procedure is became exact in 1936. Alonzo Church defined it with the lambda calculus, Alan Turing defined it with his machines, and Turing showed in 1937 that the two definitions are equivalent. Turing also showed that no algorithm can decide for every program whether it stops. Donald Knuth's *The Art of Computer Programming*, from 1968, made the analysis of algorithms a field and gave the five properties above.

Binary search shows how hard exact algorithms are. Jon Bentley found that most professional programmers could not write it correctly, and wrote about it in *Programming Pearls* in 1986. In 2006 Joshua Bloch reported that the version in the JDK, and many others, overflowed on arrays of more than about a billion items, a bug that was in the version he wrote for the JDK about nine years before.[^9]

## References
