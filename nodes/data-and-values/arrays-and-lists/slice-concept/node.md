---
tag: YJ2E6V
kind: concept
slug: slice-concept
name: Slice
aliases: [array slice, slicing, subarray, sublist, view, span, Span<T>, std::span, subList, slice expression]
area: data-and-values/arrays-and-lists
level: core
summary: >-
  A slice is a run of consecutive items of an array or a list, chosen by a start and an end index. Some languages copy the items, and others give a view that shares the memory.
applies_to: core-languages
viz: optional
formal: none
see_also: [array, dynamic-array, list, pointers-and-references, ownership, in-place-and-copying-operations, memory-leaks, get-a-substring]
sources:
- url: https://en.wikipedia.org/wiki/Array_slicing
  title: Array slicing, Wikipedia
  locator: Introduction; Details; History; 1991 Python; 1992 Fortran 90
- url: https://docs.python.org/3/library/stdtypes.html
  title: Built-in Types, The Python Standard Library
  locator: Common Sequence Operations, notes 3 to 5
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Array/slice
  title: Array.prototype.slice(), MDN Web Docs
  locator: Description
- url: https://go.dev/ref/spec
  title: The Go Programming Language Specification
  locator: Slice types; Slice expressions; Full slice expressions
- url: https://go.dev/blog/slices-intro
  title: Go Slices, usage and internals, The Go Blog
  locator: Slice internals; A possible gotcha
- url: https://doc.rust-lang.org/std/primitive.slice.html
  title: Primitive Type slice, The Rust Standard Library
  locator: Description
- url: https://doc.rust-lang.org/book/ch04-03-slices.html
  title: The Slice Type, The Rust Programming Language
  locator: 4.3
- url: https://en.cppreference.com/cpp/container/span
  title: std::span, cppreference.com
  locator: Introduction
- url: https://learn.microsoft.com/en-us/dotnet/api/system.span-1?view=net-10.0
  title: Span<T> Struct, .NET API reference
  locator: Definition
- url: https://numpy.org/doc/stable/user/basics.indexing.html
  title: Indexing on ndarrays, NumPy user guide
  locator: Slicing and striding
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/util/List.html
  title: Interface List, Java SE 25 API
  locator: subList
- url: https://learn.microsoft.com/en-us/dotnet/csharp/language-reference/operators/member-access-operators
  title: Member access operators and expressions, C# reference
  locator: Range operator ..
---

## Idea

A slice is a part of a sequence: the items from a start index up to an end index. Wikipedia defines array slicing as "an operation that extracts a subset of elements from an array and packages them as another array, possibly in a different dimension from the original"[^1]. The usual example is a substring, such as `"ell"` in `"hello"`.

Most languages use a half-open range: the start is in the slice and the end is not. So `xs[1:3]` has the items at indices 1 and 2, the length of the slice is `end - start`, and `xs[0:k]` and `xs[k:n]` split the sequence with no gap and no overlap.

The important question is whether the slice copies the items or shares them. Wikipedia notes: "Also depending on the language, the elements of the new array may be aliased to (i.e., share memory with) those of the original array"[^1]. A copy is safe to change, and it costs time and memory in proportion to its length. A view costs almost nothing to make, but a change through the view changes the original.

```python run id=idea
# A slice selects a run of items by a start index and an end index. The end is not included.
xs = [10, 20, 30, 40, 50]
print(xs[1:3], xs[:2], xs[3:], xs[-2:])

# A step picks every k-th item. A negative step walks backward.
print(xs[::2], xs[::-1])

# Bounds out of range are cut to the list, so a slice never raises IndexError.
print(xs[3:99], xs[7:9])

# A list slice is a copy. A change to the copy does not change the list.
part = xs[1:3]
part[0] = 99
print(part, xs)

# A memoryview slice is a view. A change through the view changes the buffer.
buf = bytearray(b"hello")
view = memoryview(buf)[1:4]
view[0] = ord("E")
print(bytes(view), buf)
```

```output for=idea
[20, 30] [10, 20] [40, 50] [40, 50]
[10, 30, 50] [50, 40, 30, 20, 10]
[40, 50] []
[99, 30] [10, 20, 30, 40, 50]
b'Ell' bytearray(b'hEllo')
```

The Python list slice `part` is a new list, so the change to `part[0]` leaves `xs` as it was. The `memoryview` slice is a window on the bytes of `buf`, so the write of `"E"` through the view changes `buf`.

## How it works

**The range of indices.** A slice names a start, an end and sometimes a step. Python gives the rule: "The slice of s from i to j is defined as the sequence of items with index k such that i <= k < j"[^2]. A missing start means 0, and a missing end means the length. Python, Ruby and PowerShell also accept negative indices, which count from the end, and C# uses `^1` for the last item. The step selects every k-th item, and a negative step goes backward.

**Bounds.** Languages differ when an index is out of range. Python cuts the bounds to the sequence: "If i or j is greater than len(s), use len(s)"[^2], so a slice never fails. JavaScript's `slice` does the same. Go, Rust, Java and C# check the bounds and stop the program or throw. Rust also has `get`, which returns `None` for a bad range.

**Copies.** In Python, JavaScript and the C# range on an array, a slice makes a new sequence. MDN says that `slice` "returns a shallow copy of a portion of an array into a new array object", and "The original array will not be modified"[^3]. Wikipedia says that for Python's standard lists "every slice is a copy"[^1]. The copy is shallow: if the items are references, the new sequence refers to the same objects.

**Views.** A view stores a reference to the first item and a length. It does not own the items. Go defines a slice as "a descriptor for a contiguous segment of an underlying array"[^4], and the Go blog explains: "It consists of a pointer to the array, the length of the segment, and its capacity (the maximum length of the segment)"[^5]. Rust says: "Slices are a view into a block of memory represented as a pointer and a length"[^6], and the book adds: "A slice is a kind of reference, so it does not have ownership"[^7]. C++ `std::span` "describes an object that can refer to a contiguous sequence of objects with the first element of the sequence at position zero"[^8]. C# `Span<T>` "Provides a type-safe and memory-safe representation of a contiguous region of arbitrary memory"[^9]. NumPy slices are views too: "All arrays generated by basic slicing are always views of the original array"[^10].

**Views of lists.** Java's `subList` is a view of any list, not only of an array: "The returned list is backed by this list, so non-structural changes in the returned list are reflected in this list, and vice-versa"[^11]. A view of a list can be invalid after the list changes its size, so Java checks for this case.

**Shared memory.** Because a view shares memory, a slice of a slice is a view of the same array. Go states the rule: "A slice therefore shares storage with its array and with other slices of the same array"[^4]. See [in-place and copying operations](/in-place-and-copying-operations/).

## In languages

| Language | Verdict | Mechanism |
|---|---|---|
| Python | native | `s[i:j:k]` copies a list, a tuple or a string; `memoryview` slices are views; slice assignment and `del` change a list |
| JavaScript | native | `slice` copies an array; `subarray` gives a view of a typed array; `splice` removes a range in place |
| TypeScript | native | the JavaScript methods, typed |
| Java | stdlib | `Arrays.copyOfRange` copies; `List.subList` is a view |
| Kotlin | stdlib | `slice` and `copyOfRange` copy; `subList` is a view |
| C# | native | `a[i..j]` copies an array; `Span<T>` and `Memory<T>` slices are views; `List<T>.GetRange` copies |
| Go | native | `a[low:high]` and `a[low:high:max]` make views that share the array |
| Rust | native | `&v[a..b]` and `&mut v[a..b]` borrow a view; `to_vec` copies |
| C | idiom | a pointer to the first item and a count; no bounds check |
| C++ | stdlib | `std::span` with `subspan`, `first` and `last`; `std::string_view` for text |
| Bash | native | `${a[@]:start:count}` copies a range of an indexed array; `${s:start:len}` for a string |
| PowerShell | native | `$a[1..3]` makes a new array; the range includes the end |
| SQL | idiom | `LIMIT` and `OFFSET`, or `OFFSET ... FETCH`, take a range of the sorted rows |
| Regex | n/a | a pattern matches text; it has no slices |
| HTML | n/a | markup; no data operations |
| CSS | n/a | style rules have no slices; `:nth-child` selects elements, not data |

Most languages exclude the end index. C# says: "The left-hand operand is an inclusive start of a range. The right-hand operand is an exclusive end of a range"[^12]. Some older and some scripting languages include it. In Fortran 90, "Both bounds are inclusive and can be omitted"[^1], and the PowerShell range `1..3` has the items 1, 2 and 3. A program that moves code between these languages must change the end index.

Go slices have a capacity as well as a length. A slice can grow into the unused part of its array, so an `append` to one slice can write over items that another slice sees. The full slice expression `a[low : high : max]` limits the capacity: "it controls the resulting slice's capacity by setting it to max - low"[^4]. The next `append` then copies to a new array.

The example files show these forms. Python shows slice objects, `slice.indices`, slice assignment that changes the length, `del` with a step and a shallow copy of nested lists. JavaScript shows a shallow copy, `splice` and the two methods of a typed array. Java shows `Arrays.copyOfRange`, a `subList` view, `subList(...).clear()` and the exception after a structural change. C# shows a range that copies an array, `^` indices, a `Span<T>` view and `GetRange`. Go shows a slice that shares its array, an `append` that writes into the array, the full slice expression and `slices.Clone`. Rust shows shared and mutable slices, `split_at_mut`, `get` with a range and a string slice on character boundaries. C builds a slice from a pointer and a count. C++ shows `std::span` with `subspan`, a fixed extent and a copy built from iterators.

## Variations and related ideas

- **Strides and steps.** A slice with a step skips items. Python and NumPy accept a step, and Fortran 90 writes `lower_bound:upper_bound[:stride]`[^1]. A view with a step stores the step as well as the start and the length.
- **Multidimensional slices.** NumPy and Fortran slice a matrix in each dimension, so `m[:, 0]` is a column. "Basic slicing extends Python’s basic concept of slicing to N dimensions"[^10].
- **String slices.** A substring is a slice of a string. Rust's `&str` and C++ `std::string_view` are views. In Rust, a slice of a UTF-8 string must start and end on a character boundary. See [get a substring](/get-a-substring/).
- **Slice assignment.** In Python, `xs[1:3] = items` replaces a range with a new sequence, and the list can grow or shrink.
- **Iterator slices.** `itertools.islice` in Python and `skip` and `take` in Rust select a range of a stream without indices.
- **Ranges.** A range such as `1..3` or `range(1, 3)` names the indices without the items. Many languages use the same range value to make a slice. A slice is one kind of [sequence](/sequence/): it has a length and integer indices.

## Pitfalls

- **A view that the program thinks is a copy.** A change through a Go slice, a Java `subList`, a NumPy slice or a `Span<T>` changes the original. Copy the slice when the code needs its own data.
- **A copy that the program thinks is a view.** A change to a Python list slice or a JavaScript `slice` result does not reach the original. Use slice assignment, `splice` or an index to change the original.
- **Shallow copies.** A slice copies the references, not the objects. In the Python example, an `append` to an inner list of a sliced grid changes the original grid.
- **A Go append through a shared array.** In the Go example, `append(s, 77)` writes 77 into the array, because `s` has spare capacity. Use `a[low:high:high]` or `slices.Clone` before an `append` that must not touch the array.
- **A small view that keeps a large array.** The Go blog warns: "The full array will be kept in memory until it is no longer referenced"[^5]. NumPy gives the same warning and says: "In such cases an explicit copy() is recommended"[^10]. See [memory leaks](/memory-leaks/).
- **An old view after a change in size.** A Java `subList` throws `ConcurrentModificationException` after the list changes its size. A C++ `std::span` or a C pointer into a `std::vector` becomes invalid when the vector moves its items, and its use is undefined behavior.
- **Bounds in C.** A C slice is only a pointer and a count. The program must check the bounds, or a read past the end is an [out-of-bounds read](/out-of-bounds-read/).
- **Inclusive and exclusive ends.** PowerShell and Fortran include the end index. Most other languages do not.

## Cost

| Operation | Copy | View |
|---|---|---|
| make a slice of `k` items | time and memory in proportion to `k` | constant |
| read or write item `i` of the slice | constant | constant, one addition to the start |
| change the original | no effect on the slice | the slice sees the change |
| memory kept alive | only the `k` items | the whole array or buffer |

A view is cheap to make and to pass to a function, because it is only a pointer and a length. Go says that slicing "creates a new slice value that points to the original array. This makes slice operations as efficient as manipulating array indices"[^5]. Rust and C++ functions take a slice or a span instead of a `Vec` or a `std::vector`, so one function accepts an array, a vector or a part of either. A copy costs more but is independent of the original, so it can outlive the original and is safe to share between threads.

## History

Wikipedia says that "Kenneth Iverson's APL (1957) had very flexible multi-dimensional array slicing, which contributed much to the language's expressive power and popularity" and that "ALGOL 68 (1968) introduced comprehensive multi-dimension array slicing and trimming features"[^1]. PL/I, from 1964, could map a slice of a base array to a new array, and Fortran 66 programmers could pass a row of a matrix to a subroutine only[^1]. Fortran 90 added the `lower:upper:stride` notation[^1].

Python, from 1991, made the half-open slice with optional bounds a common notation. Go, from 2009, and Rust, from 2015, made the slice a view type and the usual way to pass a sequence to a function. C# added ranges and `Span<T>`, and C++20 added `std::span`[^1][^8].

## References
