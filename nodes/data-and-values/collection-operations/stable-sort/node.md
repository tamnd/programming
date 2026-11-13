---
tag: AN08PB
kind: concept
slug: stable-sort
name: Stable sort
aliases: [stable sorting, sort stability, stable sorting algorithm, unstable sort, order-preserving sort, multi-pass sort, sort by several keys]
area: data-and-values/collection-operations
level: core
summary: >-
  A stable sort keeps items with equal keys in the order that they had before the sort. Stability lets a program sort by several keys in several passes, and it makes the result predictable.
applies_to: core-languages
viz: optional
formal: none
see_also: [comparator, sort-a-list, sort-a-list-of-records, sort-with-a-custom-key, in-place-and-copying-operations, group-items-by-a-key]
sources:
- url: https://docs.python.org/3/library/stdtypes.html#list.sort
  title: Built-in Types, Python documentation
  locator: Lists; sort
- url: https://docs.python.org/3/howto/sorting.html
  title: Sorting Techniques, Python documentation
  locator: Sort Stability and Complex Sorts; Ascending and Descending
- url: https://en.cppreference.com/w/c/algorithm/qsort.html
  title: qsort, qsort_s, C reference, cppreference.com
  locator: Description
- url: https://raw.githubusercontent.com/python/cpython/main/Objects/listsort.txt
  title: listsort.txt, CPython source
  locator: Intro
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/util/Arrays.html
  title: Class Arrays, Java SE 25 API
  locator: sort(Object[]); sort(int[])
- url: https://tc39.es/ecma262/multipage/indexed-collections.html
  title: ECMAScript Language Specification, Indexed Collections
  locator: SortIndexedProperties
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Array/sort
  title: Array.prototype.sort(), MDN Web Docs
  locator: Sort stability
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/util/List.html
  title: Interface List, Java SE 25 API
  locator: sort
- url: https://learn.microsoft.com/en-us/dotnet/api/system.collections.generic.list-1.sort
  title: List<T>.Sort Method, .NET API reference
  locator: Remarks
- url: https://learn.microsoft.com/en-us/dotnet/api/system.array.sort
  title: Array.Sort Method, .NET API reference
  locator: Remarks
- url: https://learn.microsoft.com/en-us/dotnet/api/system.linq.enumerable.orderby
  title: Enumerable.OrderBy Method, .NET API reference
  locator: Remarks
- url: https://pkg.go.dev/sort
  title: Package sort, Go standard library
  locator: Slice; SliceStable; Sort; Stable
- url: https://pkg.go.dev/slices
  title: Package slices, Go standard library
  locator: SortFunc; SortStableFunc
- url: https://doc.rust-lang.org/std/primitive.slice.html
  title: Primitive Type slice, The Rust Standard Library
  locator: sort; sort_unstable
- url: https://en.cppreference.com/w/cpp/algorithm/stable_sort.html
  title: std::stable_sort, C++ reference, cppreference.com
  locator: Description
- url: https://en.cppreference.com/w/cpp/algorithm/sort.html
  title: std::sort, C++ reference, cppreference.com
  locator: Description
- url: https://kotlinlang.org/api/core/kotlin-stdlib/kotlin.collections/sort-with.html
  title: sortWith, Kotlin standard library
  locator: Description
- url: https://kotlinlang.org/api/core/kotlin-stdlib/kotlin.collections/sorted-with.html
  title: sortedWith, Kotlin standard library
  locator: Description
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/util/stream/Stream.html
  title: Interface Stream, Java SE 25 API
  locator: sorted
- url: https://en.cppreference.com/w/cpp/container/list/sort.html
  title: std::list::sort, C++ reference, cppreference.com
  locator: Description
- url: https://hackage.haskell.org/package/base/docs/Data-List.html
  title: Data.List, Haskell base library
  locator: sort
- url: https://learn.microsoft.com/en-us/powershell/module/microsoft.powershell.utility/sort-object
  title: Sort-Object, PowerShell documentation
  locator: Syntax; Example 11
- url: https://www.postgresql.org/docs/current/queries-order.html
  title: Sorting Rows (ORDER BY), PostgreSQL documentation
  locator: 7.5
---

## Idea

Two items can have the same sort key but still be different items. Two people can be in the same team, and two orders can have the same date. A sort must put such items next to each other, but it can put them in either order. A stable sort keeps them in the order that they had in the input. An unstable sort can put them in any order. The Python documentation says: "A sort is stable if it guarantees not to change the relative order of elements that compare equal"[^1].

```python run id=idea
# The people are in order of name.
people = [("ann", "ops"), ("bob", "dev"), ("cy", "ops"), ("di", "dev"), ("ed", "ops")]

# A stable sort by team keeps the name order inside each team.
by_team = sorted(people, key=lambda p: p[1])
for name, team in by_team:
    print(team, name)
```

```output for=idea
dev bob
dev di
ops ann
ops cy
ops ed
```

The input is in order of name. The sort compares only the team, so `ann`, `cy` and `ed` are equal for the sort, and `bob` and `di` are equal too. Python sorts are stable, so each team keeps the name order of the input. An unstable sort could give `ops ed` before `ops ann`.

## How it works

**What stability means.** A sort compares keys, not whole items. When the keys of two items are equal, the sort has no reason to put one item first. A stable sort uses the input position as the last tie-breaker. The Python HOWTO says: "Sorts are guaranteed to be stable. That means that when multiple records have the same key, their original order is preserved"[^2]. Stability matters only when equal keys can belong to items that the program can tell apart. A sort of plain integers gives the same result in both cases, because two equal integers are the same value.

**Sorting by several keys in passes.** A stable sort lets a program sort by several keys, one key at a time. Sort by the least important key first, and by the most important key last. Each stable pass keeps the order of the passes before it inside each group of equal keys. The Python HOWTO says: "This wonderful property lets you build complex sorts in a series of sorting steps"[^2]. The passes can have different directions, for example a descending grade inside an ascending team. One pass with a compound key, for example a tuple or a chained comparator, gives the same order. See [comparator](/comparator/).

**Making any sort stable.** An unstable sort gives a stable result when no two keys are equal. Add the original position of each item as the last part of the key: two items never have the same position, so the sort has no ties. C `qsort` does not promise stability: "If comp indicates two elements as equivalent, their order in the resulting sorted array is unspecified"[^3]. A C program can store the position in each record and compare it last.

**Algorithms.** Merge sort, insertion sort and Timsort are stable when they are written in the usual way. Quicksort, heapsort and introsort are not stable. A stable merge needs a buffer, and this is the main cost of stability. CPython uses Timsort, which its author describes as "an adaptive, stable, natural mergesort"[^4]. Java sorts arrays of objects with a stable merge sort, and it sorts arrays of primitive values with a quicksort, because stability does not matter for them: "The sorting algorithm is a Dual-Pivot Quicksort by Vladimir Yaroslavskiy, Jon Bentley, and Joshua Bloch"[^5].

**What the libraries promise.** The documentation of each sort states whether it is stable. A sort that does not promise stability can still give a stable result on one input, so a test does not prove that a sort is stable.

| Library | Stable | Not stable |
|---|---|---|
| Python | `list.sort`, `sorted` | none |
| JavaScript | `sort`, `toSorted` | none |
| Java | `List.sort`, `Collections.sort`, `Arrays.sort` on objects, `Stream.sorted` on an ordered stream | `Arrays.sort` on primitives, where it does not matter |
| C# | LINQ `OrderBy`, `ThenBy` | `List<T>.Sort`, `Array.Sort` |
| Go | `slices.SortStableFunc`, `sort.Stable`, `sort.SliceStable` | `slices.Sort`, `slices.SortFunc`, `sort.Sort`, `sort.Slice` |
| Rust | `sort`, `sort_by`, `sort_by_key` | `sort_unstable`, `sort_unstable_by`, `sort_unstable_by_key` |
| C++ | `std::stable_sort`, `std::list::sort` | `std::sort` |
| C | none | `qsort` |
| Kotlin | `sortWith`, `sortedBy`, `sortedWith` | none |
| Haskell | `sort`, `sortBy`, `sortOn` | none |

Python says that "The sort() method is guaranteed to be stable"[^1]. The ECMAScript specification defines the result of `sort` as a permutation and ends with "i.e., the sort is stable"[^6], and MDN says that "the specification dictates that Array.prototype.sort is stable"[^7]. Java `Arrays.sort` on objects says: "This sort is guaranteed to be stable: equal elements will not be reordered as a result of the sort"[^5], and `List.sort` says: "The sort is stable: this method must not reorder equal elements"[^8]. C# `List<T>.Sort` "performs an unstable sort; that is, if two elements are equal, their order might not be preserved"[^9], and `Array.Sort` has the same remark[^10]. LINQ `OrderBy` "performs a stable sort; that is, if the keys of two elements are equal, the order of the elements is preserved"[^11]. Go `sort.Slice` says: "The sort is not guaranteed to be stable: equal elements may be reversed from their original order. For a stable sort, use SliceStable"[^12], and `slices.SortStableFunc` "sorts the slice x while keeping the original order of equal elements"[^13]. Rust `sort` is "stable (i.e., does not reorder equal elements)", and `sort_unstable` is "unstable (i.e., may reorder equal elements), in-place (i.e., does not allocate)"[^14]. For C++ `std::stable_sort`, "The order of equivalent elements is guaranteed to be preserved"[^15], and for `std::sort`, "The order of equivalent elements is not guaranteed to be preserved"[^16]. Kotlin says of `sortWith` and `sortedWith`: "The sort is stable. It means that equal elements preserve their order relative to each other after sorting"[^17][^18]. A Java stream sort is stable only on an ordered stream: "For ordered streams, the sort is stable"[^19]. C++ `std::list::sort` "preserves the order of equivalent elements"[^20]. Haskell says: "The sort function implements a stable sorting algorithm"[^21].

## In languages

| Language | Verdict | Mechanism |
|---|---|---|
| Python | native | `list.sort` and `sorted` are always stable, also with `reverse=True` |
| JavaScript | native | `sort` and `toSorted` are stable by the language specification |
| TypeScript | native | the JavaScript methods |
| Java | native | `List.sort`, `Collections.sort` and `Arrays.sort` on objects are stable |
| Kotlin | native | `sortWith`, `sortedBy` and `sortedWith` are stable |
| C# | native | LINQ `OrderBy` and `ThenBy` are stable; `List<T>.Sort` and `Array.Sort` are not |
| Go | native | `slices.SortStableFunc` and `sort.SliceStable` are stable; `slices.Sort` and `sort.Slice` are not |
| Rust | native | `sort`, `sort_by` and `sort_by_key` are stable; the `sort_unstable` methods are not |
| C | idiom | `qsort` is not stable; a program adds the original position as the last key |
| C++ | native | `std::stable_sort` is stable; `std::sort` is not |
| Bash | native | the `sort` command has the option `-s` for a stable sort |
| PowerShell | native | `Sort-Object` has the switch `-Stable`[^22] |
| SQL | idiom | `ORDER BY` has no stable form; a query adds a unique column as the last key |
| Regex | n/a | a pattern does not sort values |
| HTML | n/a | markup does not sort values |
| CSS | n/a | the style rules do not sort values |

Most languages have a stable sort in the library, and some of them also have a faster unstable sort. C and SQL have no stable sort, so a program makes the order of equal keys explicit with one more key.

The example files show these differences. Python shows two passes, a tuple key, the stability of `reverse=True`, and `reversed()`, which also reverses the equal items. JavaScript shows two passes and a chained compare function. Java shows `List.sort`, two passes, a stream sort and a reversed comparator on an array. C# shows the stable LINQ `OrderBy` and an index that makes `List<T>.Sort` stable. Go shows `SortStableFunc`, `SliceStable` and a full key for `SortFunc`. Rust shows `sort_by_key` and an index key for `sort_unstable_by_key`. C shows an index key for `qsort`. C++ shows `std::ranges::stable_sort` with projections and a full key for `std::ranges::sort`.

## Variations and related ideas

- **Reverse order and stability.** A descending stable sort keeps equal items in their input order. The Python HOWTO says: "The reverse parameter still maintains sort stability (so that records with equal keys retain the original order)"[^2]. An ascending sort followed by a reversal of the list gives the same keys in descending order, but it also reverses the equal items.
- **Partial sorts and selection.** Algorithms that find the k smallest items, for example `std::partial_sort` or a heap, are usually not stable. Add a position key when the order of ties matters.
- **Grouping.** A stable sort by a key puts the items of each group together and keeps their input order inside the group. Some grouping functions depend on this. See [group items by a key](/group-items-by-a-key/).
- **Sorting in a database.** A table has no input order. PostgreSQL says that without a sort step "the rows will be returned in an unspecified order" and that the order "must not be relied on"[^23]. Rows with equal `ORDER BY` keys have the same problem, so a query that pages through results must add a unique column as the last key.
- **Runs in the input.** A stable adaptive sort, for example Timsort, finds runs that are already sorted. The Python HOWTO says that Timsort "does multiple sorts efficiently because it can take advantage of any ordering already present in a dataset"[^2], so a second pass over the result of a first pass is fast.

## Pitfalls

- **Testing for stability.** A sort that does not promise stability can keep the order of ties on a small input, for example when it uses insertion sort for short arrays. A test with five items then passes, and a production input with many items gives another order. Read the documentation of the sort.
- **Passes in the wrong order.** In a multi-pass sort, the last pass is the main key. A program that sorts by team first and by grade second gets a list sorted by grade, with the teams in order only inside each grade.
- **A reversed list is not a descending stable sort.** `reversed(sorted(xs, key=k))` puts the equal items in the reverse of their input order. Use `reverse=True`, a descending comparator or a negated key.
- **An unstable sort in a multi-pass sort.** Every pass after the first must be stable. In C#, `List<T>.Sort` in a second pass can destroy the order of the first pass. Use `OrderBy` and `ThenBy`, or one compound key.
- **A comparator that adds hidden ties.** A comparator that compares only a part of the key, for example the first letter of a name, makes many items equal. A stable sort keeps their input order, which can look like a bug in the comparator.
- **Database pages.** `ORDER BY created_at LIMIT 10 OFFSET 10` can show one row twice and skip another when several rows have the same `created_at`. Add the primary key to the `ORDER BY`.

## Cost

A stable comparison sort has the same O(n log n) bound for comparisons as an unstable one. The usual cost of stability is memory: a merge sort needs a buffer of up to n items, while quicksort and heapsort need only a small stack. Rust says that its unstable sort is in place and "does not allocate"[^14]. Stable in-place merge sorts exist, but they are more complex and slower. An unstable sort with a position key needs O(n) memory for the positions, so it has no memory advantage. For arrays of primitive values, where stability has no visible effect, an unstable sort is often a little faster.

## History

Stability became a common promise of library sorts over time. C `qsort` does not promise it[^3], and C++ has the separate function `std::stable_sort`[^15]. Tim Peters wrote Timsort for CPython and described it as "an adaptive, stable, natural mergesort, modestly called timsort"[^4]. His notes also say: "It turns out that Perl is moving to a stable mergesort"[^4]. Java uses an adaptation of Timsort for its object sorts[^5].

JavaScript engines did not all have a stable `sort`, and a program could get a different order of ties in each browser. The ECMAScript specification now requires a stable sort[^6][^7]. Newer libraries often give both forms and name the unstable one: Rust has `sort` and `sort_unstable`[^14], and Go has `SortFunc` and `SortStableFunc`[^13].

## References
