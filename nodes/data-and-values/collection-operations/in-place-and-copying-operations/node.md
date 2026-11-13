---
tag: GRNRV1
kind: concept
slug: in-place-and-copying-operations
name: In-place and copying operations
aliases: [in place, in-place, mutating method, non-mutating method, destructive operation, non-destructive operation, sort versus sorted, copying method, sort and sorted, bang method]
area: data-and-values/collection-operations
level: core
summary: >-
  An in-place operation changes the collection that it gets. A copying operation leaves it alone and returns a new collection. The name and the return value tell which one a function is.
applies_to: core-languages
viz: optional
formal: none
see_also: [sort-a-list, immutability, copy-on-write, value-equality-and-identity, iterators, map-over-a-list]
sources:
- url: https://docs.python.org/3/library/stdtypes.html#list.sort
  title: Built-in Types, Python documentation
  locator: Lists; sort
- url: https://docs.python.org/3/library/functions.html#sorted
  title: Built-in Functions, Python documentation
  locator: sorted
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Array/toSorted
  title: Array.prototype.toSorted(), MDN Web Docs
  locator: Introduction
- url: https://docs.python.org/3/faq/design.html#why-doesn-t-list-sort-return-the-sorted-list
  title: Design and History FAQ, Python documentation
  locator: Why doesn't list.sort() return the sorted list?
- url: https://pkg.go.dev/slices
  title: Package slices, Go standard library
  locator: Clone; Reverse; Sort; Sorted
- url: https://en.cppreference.com/w/c/algorithm/qsort.html
  title: qsort, qsort_s, C reference, cppreference.com
  locator: Description
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Array/sort
  title: Array.prototype.sort(), MDN Web Docs
  locator: Introduction
- url: https://www.swift.org/documentation/api-design-guidelines/
  title: API Design Guidelines, Swift.org
  locator: Strive for Fluent Usage; Name Mutating/nonmutating method pairs consistently
- url: https://kotlinlang.org/docs/list-operations.html
  title: List-specific operations, Kotlin documentation
  locator: Sorting
- url: https://docs.ruby-lang.org/en/master/syntax/methods_rdoc.html
  title: Methods, Ruby documentation
  locator: Method Names
- url: https://conservatory.scheme.org/schemers/Documents/Standards/R5RS/HTML/r5rs-Z-H-4.html
  title: Revised5 Report on the Algorithmic Language Scheme
  locator: 1.3.5 Naming conventions
- url: https://en.cppreference.com/w/cpp/algorithm.html
  title: Algorithms library, C++ reference, cppreference.com
  locator: Modifying sequence operations; Order-changing operations
- url: https://doc.rust-lang.org/std/primitive.slice.html
  title: Primitive Type slice, The Rust Standard Library
  locator: sort; to_vec
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Array
  title: Array, MDN Web Docs
  locator: Copying methods and mutating methods
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/util/List.html
  title: Interface List, Java SE 25 API
  locator: Unmodifiable Lists; sort
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/util/SequencedCollection.html
  title: Interface SequencedCollection, Java SE 25 API
  locator: reversed
- url: https://learn.microsoft.com/en-us/dotnet/api/system.linq.enumerable.orderby
  title: Enumerable.OrderBy Method, .NET API reference
  locator: Remarks
- url: https://www.typescriptlang.org/docs/handbook/2/objects.html
  title: Object Types, The TypeScript Handbook
  locator: The ReadonlyArray Type
- url: https://docs.python.org/3/howto/sorting.html
  title: Sorting Techniques, Python documentation
  locator: Sorting Basics
- url: https://docs.ruby-lang.org/en/master/Array.html
  title: class Array, Ruby documentation
  locator: uniq!
- url: http://www.lispworks.com/documentation/HyperSpec/Body/f_sort_.htm
  title: Function SORT, STABLE-SORT, Common Lisp HyperSpec
  locator: Description
- url: http://www.lispworks.com/documentation/HyperSpec/Body/f_revers.htm
  title: Function REVERSE, NREVERSE, Common Lisp HyperSpec
  locator: Description
- url: https://docs.python.org/3/whatsnew/2.4.html
  title: What's New in Python 2.4, Python documentation
  locator: Other Language Changes
- url: https://github.com/tc39/proposal-change-array-by-copy
  title: Change Array by copy, TC39 proposal
  locator: Introduction; Overview
---

## Idea

A function that sorts, reverses or filters a collection can give its result in two ways. It can change the collection that the caller gave it, or it can leave that collection alone and return a new one. The first way is an in-place operation, also called a mutating or destructive operation. The second way is a copying operation, also called a non-mutating or non-destructive operation. Many libraries have both forms of the same operation, and the caller must know which form a name means.

```python run id=idea
# sorted() returns a new list and leaves nums alone.
nums = [3, 1, 2]
fresh = sorted(nums)
print(nums, fresh)

# list.sort() changes nums itself and returns None.
result = nums.sort()
print(nums, result)

# Two names for one list both see an in-place change.
a = [3, 1, 2]
b = a
a.sort()
print(b)

# A copying operation gives a new list to one name only.
a = [3, 1, 2]
b = a
a = sorted(a)
print(a, b)
```

```output for=idea
[3, 1, 2] [1, 2, 3]
[1, 2, 3] None
[1, 2, 3]
[1, 2, 3] [3, 1, 2]
```

`sorted(nums)` makes a new list, so `nums` keeps its order. `nums.sort()` puts the items of `nums` in order and returns `None`. When two names refer to one list, an in-place sort through one name changes what the other name sees. After `a = sorted(a)`, `a` refers to a new list, and `b` still refers to the old list in the old order.

## How it works

**In place.** An in-place operation writes its result into the storage of the collection that it gets. It needs little or no extra memory, and the old content is gone after the call. Python says that `list.sort()` "modifies the sequence in place for economy of space when sorting a large sequence"[^1].

**Copying.** A copying operation reads the collection and builds a new one. The caller then has two collections: the original, which does not change, and the result. Python `sorted()` will "Return a new sorted list from the items in iterable"[^2]. In JavaScript, "The toSorted() method of Array instances is the copying version of the sort() method"[^3].

**The return value is a signal.** Many in-place operations return nothing, so that the caller cannot mistake them for copies. The Python list method "does not return the sorted sequence (use sorted() to explicitly request a new sorted list instance)"[^1]. The Python FAQ gives the reason: "This way, you won’t be fooled into accidentally overwriting a list when you need a sorted copy but also need to keep the unsorted version around"[^4]. Java `List.sort` returns `void`, Rust `sort` returns `()`, and Go `slices.Sort`[^5] and C `qsort`[^6] return nothing. JavaScript takes the other choice: `sort()` "sorts the elements of an array in place and returns the reference to the same array, now sorted"[^7]. So in JavaScript, `const b = a.sort()` gives a second name for `a`, not a sorted copy.

**The name is a signal.** Some libraries pair the two forms by a rule of names:

| Rule | In place | Copy | Where |
|---|---|---|---|
| verb and past participle | `sort()`, `reverse()` | `sorted()`, `reversed()` | Python, Swift, Kotlin |
| prefix `to` | `sort()`, `reverse()`, `splice()` | `toSorted()`, `toReversed()`, `toSpliced()` | JavaScript |
| suffix `!` | `sort!`, `uniq!` | `sort`, `uniq` | Ruby, Scheme |
| suffix `_copy` | `reverse`, `remove`, `rotate` | `reverse_copy`, `remove_copy`, `rotate_copy` | C++ |
| `&mut self` in the signature | `sort`, `reverse`, `retain` | `to_vec`, `iter().rev().collect()` | Rust |

The Swift guidelines state the rule of the first row: "use the verb's imperative for the mutating method and apply the “ed” or “ing” suffix to name its nonmutating counterpart"[^8]. Kotlin says that the in-place sorting functions have similar names to the copying ones, "but without the ed/d suffix"[^9]. Ruby says that a bang method "permanently modifies its receiver"[^10], and Scheme says that the names of procedures that "store values into previously allocated locations" end in `!` by convention[^11]. The C++ algorithms library lists `reverse` next to `reverse_copy` and `rotate` next to `rotate_copy`[^12]. In Rust, the type of the receiver tells: `sort` takes `&mut self`, and `to_vec` "Copies self into a new Vec"[^13]. The task [reverse a list](/reverse-a-list/) shows both forms of `reverse` in ten languages.

**Copies are shallow.** A copying operation copies the container, not the items. MDN says of the copying array methods: "The copy always happens shallowly"[^14]. Go `slices.Clone` copies the elements "using assignment, so this is a shallow clone"[^5]. So when the items are objects, the original and the copy hold the same objects, and a change to an object shows in both.

**Read-only collections force a copy.** An in-place operation needs a collection that the program can change. Java `List.of` and `List.copyOf` make lists that are unmodifiable, and "Calling any mutator method on the List will always cause UnsupportedOperationException to be thrown"[^15]. A program that has such a list must use the copying form.

## In languages

| Language | Verdict | Mechanism |
|---|---|---|
| Python | native | `list.sort()` and `list.reverse()` return `None`; `sorted()` returns a new list; `reversed()` returns an iterator |
| JavaScript | native | `sort()`, `reverse()` and `splice()` mutate; `toSorted()`, `toReversed()`, `toSpliced()` and `with()` copy |
| TypeScript | native | the JavaScript methods; `ReadonlyArray` has no mutating methods |
| Java | native | `List.sort` and `Collections.sort` mutate; `stream().sorted().toList()` copies; `reversed()` is a view |
| Kotlin | native | `sort()` and `reverse()` on a `MutableList`; `sorted()` and `reversed()` return a new list |
| C# | native | `List<T>.Sort()`, `List<T>.Reverse()` and `Array.Sort` mutate; LINQ `OrderBy` and `Reverse` return new sequences |
| Go | native | `slices.Sort` and `slices.Reverse` mutate; `slices.Clone` and `slices.Sorted` give new slices |
| Rust | native | `sort`, `reverse` and `retain` take `&mut self`; `clone`, `to_vec` and iterator chains make new vectors |
| C | native | `qsort` and `strtok` work in place; a copy is a `memcpy` into new storage |
| C++ | native | `std::sort` and `std::reverse` mutate; `reverse_copy`, `remove_copy` and the other `_copy` algorithms write to another range |
| Bash | idiom | the `sort` command writes a new output and does not change its input file unless `-o` names that file |
| PowerShell | native | `Sort-Object` sends new output down the pipeline; `[Array]::Sort` sorts an array in place |
| SQL | native | `ORDER BY` in a query returns a sorted result and does not change the table; `UPDATE` changes rows in place |
| Regex | n/a | a pattern does not change a collection |
| HTML | n/a | markup has no operations on collections |
| CSS | n/a | the `order` property changes the visual order of flex and grid items, not the document |

Every general-purpose language in the table has both forms. They differ in the default form, in the return value of the in-place form, and in the rule that names the two forms.

The example files show these differences. Python shows the `None` that `sort()`, `shuffle()` and `reverse()` return, the iterator of `reversed()`, the difference between `+=` and `a = a + b`, and a shallow copy. JavaScript shows that `sort()` returns the same array, the copying methods `toSorted()`, `toReversed()` and `with()`, and a frozen array. Java shows an unmodifiable list, the write-through of `Arrays.asList` and the view of `reversed()`. C# shows the deferred `OrderBy`, the in-place `List<T>.Reverse()` and a copying call whose result is lost. Go shows `slices.Sorted`, `slices.Clone` and a subslice that shares its array. Rust shows `&mut self`, `retain` and lazy iterator adapters. C copies an array with `memcpy` before `qsort`, and shows that `strtok` changes its input. C++ shows the `_copy` algorithms and `std::remove`, which does not change the size.

## Variations and related ideas

- **Views.** A view is neither a copy nor an in-place change. It is a new object that reads the original. Java `reversed()` "Returns a reverse-ordered view of this collection"[^16], and Python `reversed()` returns an iterator over the list. A later change to the original shows in the view.
- **Lazy copies.** C# LINQ `OrderBy` "is implemented by using deferred execution"[^17], so the sort happens when the program enumerates the result, not when it calls `OrderBy`. Iterator chains in Rust and Java streams also wait for a terminal operation.
- **Read-only types.** A type can remove the mutating methods. In TypeScript, "The ReadonlyArray is a special type that describes arrays that shouldn’t be changed"[^18], so `sort()` on a `ReadonlyArray` does not compile. See [immutability](/immutability/).
- **Copy-on-write.** A copy can share storage with its original until one of them changes. See [copy-on-write](/copy-on-write/).
- **In-place algorithms.** In the study of algorithms, an in-place algorithm uses a small, fixed amount of extra memory. A library sort that is in place for the caller can still use temporary storage inside.

## Pitfalls

- **Assigning the result of an in-place call.** `scores = scores.sort()` in Python sets `scores` to `None`. Python returns `None` from such methods "to avoid confusion"[^19], but the assignment still loses the list.
- **Two names for one array.** `const b = a.sort()` in JavaScript does not make a copy, so a later change to `b` changes `a`. Use `toSorted()` or `[...a].sort()`.
- **Dropping the result of a copying call.** A C# statement `list.OrderBy(n => n);` or a Python statement `sorted(nums)` changes nothing, because the program does not use the new collection.
- **`nil` from a bang method.** Ruby `uniq!` "returns self if any elements removed, nil otherwise"[^20], so `a.uniq!.size` fails on an array that has no duplicates.
- **Shared storage.** A Go subslice shares its array, so `sort.Ints(all[:2])` changes `all`, and `append` on a short subslice can overwrite the next item of the original.
- **Shallow copies.** A sorted copy of a list of lists holds the same inner lists, so a change to an inner list shows in the original.
- **Hidden in-place work.** C `strtok` writes null characters into its argument, and C++ `std::remove` moves items but keeps the size, so the program must call `erase`.

## Cost

An in-place operation needs no new collection, so its extra memory is small. Some in-place operations still need temporary memory inside: a merge sort, for example, uses a buffer. A copying operation allocates a new collection of n items and copies n references or values, so it costs O(n) time and memory on top of the work of the operation. For a sort, this extra O(n) is small next to the O(n log n) comparisons, and the Python Sorting HOWTO says that `list.sort()` is "slightly more efficient"[^19] than `sorted()` when the program does not need the original. The copy also removes a class of errors: no other part of the program sees a change that it did not expect.

## History

Lisp calls an in-place operation destructive. The Common Lisp standard says that "sort and stable-sort destructively sort sequences"[^21], and it pairs `reverse` with `nreverse`: "reverse always creates and returns a new sequence, whereas nreverse might modify and return the given sequence"[^22]. Scheme marks a procedure that changes storage with `!`[^11], and Ruby uses the `!` in the same way for its bang methods[^10].

Python had only the in-place `list.sort()` until Python 2.4. That release added "a new built-in function sorted(iterable) that works like the in-place list.sort() method but can be used in expressions"[^23]. JavaScript had only the mutating `sort()` and `reverse()` until the Change Array by copy proposal, which "Provides additional methods on Array.prototype and TypedArray.prototype to enable changes on the array by returning a new copy of it with the change"[^24]. MDN says that `toSorted()` has been available across browsers since July 2023[^3].

## References
