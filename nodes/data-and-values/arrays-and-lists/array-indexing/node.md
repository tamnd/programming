---
tag: RAT491
kind: concept
slug: array-indexing
name: Array indexing
aliases: [array index, subscript, index origin, zero-based indexing, one-based indexing, negative index, bounds check, index out of range]
area: data-and-values/arrays-and-lists
level: intro
summary: >-
  Array indexing selects an item of an array by its position. Most languages count from 0, some count from 1, and many check that the index is inside the array.
applies_to: core-languages
viz: optional
formal: none
see_also: [array, range, slice-concept, string-indexing, out-of-bounds-read, out-of-bounds-write, memory-address, undefined-behavior]
sources:
- url: https://en.wikipedia.org/wiki/Zero-based_numbering
  title: Zero-based numbering, Wikipedia
  locator: Introduction; Origin; Usage in programming languages
- url: https://en.cppreference.com/c/language/operator_member_access
  title: Member access operators, C, cppreference.com
  locator: Subscript
- url: https://www.cs.utexas.edu/~EWD/transcriptions/EWD08xx/EWD831.html
  title: Edsger W. Dijkstra, Why numbering should start at zero (EWD 831), 1982
  locator: Full text
- url: https://en.wikipedia.org/wiki/Array_(data_type)
  title: Array (data type), Wikipedia
  locator: Index origin
- url: https://www.lua.org/pil/11.1.html
  title: Arrays, Programming in Lua (first edition)
  locator: Section 11.1
- url: https://docs.python.org/3/library/stdtypes.html
  title: Built-in Types, The Python Standard Library
  locator: Common Sequence Operations, notes 3 and 4
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Array/at
  title: Array.prototype.at(), MDN Web Docs
  locator: Description
- url: https://learn.microsoft.com/en-us/dotnet/csharp/tutorials/ranges-indexes
  title: Indices and ranges, C# documentation
  locator: Language support for indices and ranges
- url: https://en.wikipedia.org/wiki/Bounds_checking
  title: Bounds checking, Wikipedia
  locator: Introduction; Index checking
- url: https://docs.oracle.com/javase/specs/jls/se25/html/jls-10.html
  title: Chapter 10, Arrays, The Java Language Specification, Java SE 25
  locator: 10.4 Array Access
- url: https://go.dev/ref/spec
  title: The Go Programming Language Specification
  locator: Index expressions
- url: https://doc.rust-lang.org/book/ch03-02-data-types.html
  title: Data Types, The Rust Programming Language
  locator: Invalid Array Element Access
---

## Idea

An array keeps its items in order, and an index is the number of an item's position. `a[i]` reads the item at index `i`. The language decides three things about indices:

- the **origin**, which is the index of the first item: 0 in most languages, 1 in some;
- what a **negative index** means: an error, or a position counted from the end;
- what happens when the index is **out of bounds**: an exception, a special value, or undefined behavior.

Wikipedia defines the most common origin: "Zero-based numbering is a way of numbering in which the initial element of a sequence is assigned the index 0, rather than the index 1 as is typical in everyday non-mathematical or non-programming circumstances"[^1]. With origin 0, an array of length `n` has the indices 0 to `n - 1`.

```python run id=idea
a = ["a", "b", "c", "d"]

# The first item has index 0 and the last has index len(a) - 1.
print(a[0], a[3], a[len(a) - 1], len(a))

# Item i starts i * size bytes after the start of the array.
base, size = 1000, 4
print([base + size * i for i in range(len(a))])

# With indices from 1, the same address needs an extra subtraction.
print([base + size * (i - 1) for i in range(1, len(a) + 1)])

# An index outside 0 to len(a) - 1 is an error.
try:
    a[4]
except IndexError as e:
    print("IndexError:", e)
```

```output for=idea
a d d 4
[1000, 1004, 1008, 1012]
[1000, 1004, 1008, 1012]
IndexError: list index out of range
```

The two address lists are the same, but the second formula has one more subtraction. Index 4 is past the end of a list of length 4, so Python raises `IndexError`.

## How it works

**An index is an offset.** The items of an array are next to each other in memory, so the address of an item is the start of the array plus the index times the size of an item. Wikipedia gives the two formulas: with indices from 1, "the desired address is computed by this expression: a+s×(i−1), where s is the size of each element. In contrast, if the index numbers count from 0, the expression becomes a+s×i. This simpler expression is more efficient to compute at run time"[^1]. C defines indexing as this arithmetic: "By definition, the subscript operator E1[E2] is exactly identical to *((E1)+(E2))"[^2]. So in C, `a[2]`, `*(a + 2)` and even `2[a]` are the same item. In the C example, `&a[3] - &a[0]` is 3 items and 12 bytes.

**Why 0.** Dijkstra argued from ranges. With the half-open convention `start ≤ i < end`, indices from 1 give the range `1 ≤ i < N+1`, but "starting with 0, however, gives the nicer range 0 ≤ i < N". He concluded that "an element's ordinal (subscript) equals the number of elements preceding it in the sequence"[^3]. The index of an item is how many items come before it. See [range](/range/).

**Why 1.** Indices from 1 match the way people count and the notation of mathematics. Wikipedia says that one-based arrays are "the traditional convention in mathematics for matrices and mathematical sequences"[^4], and that "Fortran or COBOL, have array subscripts starting with one, because they were meant as high-level programming languages"[^1]. Lua tables can use any index, but "it is customary in Lua to start arrays with index 1. The Lua libraries adhere to this convention"[^5]. Some languages let the program choose: "A few languages, such as Pascal and Lua, support n-based array types, whose minimum legal indices are chosen by the programmer"[^4].

**Negative indices.** Some languages read a negative index from the end. In Python, "If i or j is negative, the index is relative to the end of sequence s: len(s) + i or len(s) + j is substituted. But note that -0 is still 0"[^6]. So `a[-1]` is the last item. JavaScript brackets do not do this, but the `at` method does: "Negative integers count back from the last item in the array"[^7]. C# has a separate operator: "For any number n, the index ^n is the same as sequence.Length - n"[^8], so the index `^1` is the last item and `^0` is one past the end. Java, Go, Rust, C and C++ have no negative indices.

**Bounds checks.** A bounds check tests that `0 ≤ i < length` before the access. Wikipedia describes it as a test "that a variable being used as an array index is within the bounds of the array (index checking). A failed bounds check usually results in the generation of some sort of exception signal"[^9]. Java checks every access: "All array accesses are checked at run time; an attempt to use an index that is less than zero or greater than or equal to the length of the array causes an ArrayIndexOutOfBoundsException to be thrown"[^10]. In Go, "the index x is in range if 0 <= x < len(a), otherwise it is out of range", and "if x is out of range at run time, a run-time panic occurs"[^11]. In Rust, "If the index is greater than or equal to the length, Rust will panic"[^12]. C does not check, and an access out of bounds is undefined behavior. See [out-of-bounds read](/out-of-bounds-read/) and [out-of-bounds write](/out-of-bounds-write/).

## In languages

| Language | Verdict | Mechanism |
|---|---|---|
| Python | native | origin 0; negative indices count from the end; `IndexError` out of bounds; slices cut their bounds |
| JavaScript | native | origin 0; `at(-1)` counts from the end; an index out of bounds gives `undefined` |
| TypeScript | native | the JavaScript rules; `noUncheckedIndexedAccess` adds `undefined` to the type of `a[i]` |
| Java | native | origin 0, `int` indices; `ArrayIndexOutOfBoundsException`; `List.get` throws `IndexOutOfBoundsException` |
| Kotlin | native | origin 0; `getOrNull` and `getOrElse`; `lastIndex`; an exception out of bounds |
| C# | native | origin 0; `^n` counts from the end; `IndexOutOfRangeException` on arrays |
| Go | native | origin 0; a run-time panic out of bounds; a constant index out of range is a compile error |
| Rust | native | origin 0, `usize` indices; a panic out of bounds; `get` returns an `Option` |
| C | native | origin 0; `a[i]` is `*(a + i)`; no bounds check, and an access out of bounds is undefined behavior |
| C++ | native | origin 0; `operator[]` does not check; `at()` throws `std::out_of_range` |
| Bash | native | origin 0 for indexed arrays; a negative index counts from the end; an unset index gives an empty string |
| PowerShell | native | origin 0; `$a[-1]` is the last item; an index out of bounds gives `$null` |
| SQL | native | PostgreSQL arrays start at 1 by default and give NULL out of bounds; `ROW_NUMBER()` numbers rows from 1 |
| Regex | n/a | a pattern has no array indices; capture groups are numbered from 1, and group 0 is the whole match |
| HTML | n/a | markup; `:nth-child(1)` in CSS selects the first child, so selectors count from 1 |
| CSS | n/a | style rules have no arrays; `nth-child` counts from 1 |

The languages give four answers to an index out of bounds. Java, C#, Python and C++ `at()` throw an exception. Go and Rust stop the program with a panic. JavaScript, PowerShell and PostgreSQL return a special value such as `undefined` or NULL. C and C++ `operator[]` do not check, so the behavior is undefined.

The example files show these rules. Python uses negative indices, `IndexError` on both sides, slices that cut their bounds, `enumerate` from 1 and `i % n` to wrap an index. JavaScript shows `at(-1)`, `a[-1]` as a property lookup, `undefined` past the end, a sparse array and a typed array that ignores a write out of bounds. Java shows the exception messages for arrays, a `List` and `Objects.checkIndex`. C# shows `^n`, the `Index` type, `^0` and the two exception types. Go recovers from the panics and shows that a string index is a byte. Rust uses `get`, `first`, `last`, `try_from` for a negative value, `checked_sub` and byte indices into a string. C shows that `a[i]`, `*(a + i)` and `i[a]` are the same and writes its own bounds check. C++ compares `operator[]` with `at()`, counts down with `std::ssize`, and uses `std::get` on a `std::array`.

## Variations and related ideas

- **Arbitrary lower bounds.** Wikipedia notes that "Ada, ALGOL 60, PL/I, allow an arbitrary lower bound for each index"[^1], and Pascal allows any ordinal type as the index type[^1], so an array can be indexed by the days of the week.
- **A run-time origin.** "APL allows setting the index origin to 0 or 1 during runtime programmatically"[^1].
- **Index types.** Rust uses the unsigned `usize`, so a negative index cannot exist, and a signed value must be converted first. Java uses `int`, so the length of an array must fit in an `int`.
- **Multidimensional indices.** `m[i][j]` selects a row and then an item. A true matrix computes one offset from both indices. See [multidimensional array](/multidimensional-array/).
- **Strings.** A string index can count bytes, code units or characters, and each choice gives a different item for a non-ASCII string. In the Go and Rust examples, `"héllo"` has 6 bytes. See [string indexing](/string-indexing/).
- **Safe access.** `get` in Rust, `getOrNull` in Kotlin and `at` in JavaScript return a value that marks a missing item instead of an error.

## Pitfalls

- **Off by one.** The last index is `length - 1`, not `length`. A loop with `i <= n` reads one item too far. Wikipedia notes that zero-based indexing "can avoid off-by-one or fencepost errors"[^4] when the code uses half-open ranges, but a mix of 0-based and 1-based code causes them.
- **Mixed origins.** Data from people, SQL and Lua is often 1-based, and the program is 0-based. Convert at the edge of the program, in one place.
- **Negative indices by mistake.** In Python, an index computed as `i - 1` is -1 when `i` is 0, and it reads the last item without an error. In JavaScript, `a[-1]` gives `undefined`, not the last item.
- **Silent results.** JavaScript returns `undefined` and does not throw, so a wrong index can travel far before it causes an error. A write far past the end of a JavaScript array makes a sparse array with holes.
- **Unsigned arithmetic.** In C, C++ and Rust, `len - 1` on an empty array wraps around to the largest value, or panics in a Rust debug build. Test for an empty array first, or use `checked_sub` or a signed size.
- **No check in C.** An index out of bounds in C or with C++ `operator[]` reads or writes other memory. The program can continue with wrong data, and an attacker can use it. Use a checked access when the index comes from input.

## Cost

| Operation | Time | Notes |
|---|---|---|
| read or write `a[i]` | constant | one multiply and one add for the address |
| bounds check | constant | one compare and a branch, often removed by the compiler |
| negative index | constant | one more add of the length |
| index into a linked list | linear in `i` | the list must follow `i` links |

Indexing is constant time only for contiguous storage. The cost of a bounds check is small, and compilers remove it when they can prove the index is in range, for example in a loop from 0 to the length. Wikipedia calls this "Bounds-checking elimination"[^9].

## History

Early high-level languages used origin 1. FORTRAN counted from 1, and ALGOL 60 and Pascal let the program give both bounds. Dijkstra wrote that "In FORTRAN subscripts always start at 1; in ALGOL 60 and in PASCAL, convention c) has been adopted"[^3], where convention c) includes both bounds. Index checking also came early: Wikipedia lists ALGOL 60, ALGOL 68 and Pascal among the "Early compiled programming languages with index checking ability"[^9]. In his 1980 Turing Award lecture, C. A. R. Hoare said of ALGOL 60 that "every occurrence of every subscript of every subscripted variable was on every occasion checked at run time against both the upper and the lower declared bounds of the array"[^9].

Origin 0 came from systems languages. Martin Richards, the creator of BCPL, "designed arrays initiating at 0 as the natural position to start accessing the array contents in the language, since the value of a pointer p used as an address accesses the position p + 0 in memory"[^1]. BCPL was a precursor of C[^1], and C kept arrays that start at 0. In 1982, Dijkstra gave the argument from half-open ranges in EWD 831[^3]. Newer languages kept origin 0 and added safer access: C# has `^n`, JavaScript has `at`, and Rust and Go check every index.

## References
