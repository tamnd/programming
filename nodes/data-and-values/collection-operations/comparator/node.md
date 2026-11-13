---
tag: 2ZWRWS
kind: concept
slug: comparator
name: Comparator
aliases: [comparison function, compare function, three-way comparison, key function, sort key, Comparator interface, cmp function, ordering function, collation function]
area: data-and-values/collection-operations
level: core
summary: >-
  A comparator is a function that gets two values and tells which one comes first. A key function gets one value and returns the value to compare. Sorts, sorted maps and heaps use them.
applies_to: core-languages
viz: optional
formal: none
see_also: [sort-with-a-custom-key, sort-a-list-of-records, sort-a-list, compare-strings, in-place-and-copying-operations, higher-order-functions, value-equality-and-identity]
sources:
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/util/Comparator.html
  title: Interface Comparator, Java SE 25 API
  locator: Description; compare; comparing; thenComparing; nullsFirst
- url: https://docs.python.org/3/library/functools.html#functools.cmp_to_key
  title: functools, Python documentation
  locator: cmp_to_key
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Array/sort
  title: Array.prototype.sort(), MDN Web Docs
  locator: Parameters; Description
- url: https://en.cppreference.com/w/c/algorithm/qsort.html
  title: qsort, qsort_s, C reference, cppreference.com
  locator: Parameters
- url: https://pkg.go.dev/cmp
  title: Package cmp, Go standard library
  locator: Compare; Or
- url: https://learn.microsoft.com/en-us/dotnet/api/system.collections.generic.icomparer-1
  title: IComparer<T> Interface, .NET API reference
  locator: Definition
- url: https://learn.microsoft.com/en-us/dotnet/api/system.comparison-1
  title: Comparison<T> Delegate, .NET API reference
  locator: Definition
- url: https://doc.rust-lang.org/std/cmp/enum.Ordering.html
  title: Enum Ordering, The Rust Standard Library
  locator: Description; then
- url: https://hackage.haskell.org/package/base/docs/Data-Ord.html
  title: Data.Ord, Haskell base library
  locator: Ord; Ordering; comparing
- url: https://en.cppreference.com/w/cpp/language/operator_comparison.html
  title: Comparison operators, C++ reference, cppreference.com
  locator: Three-way comparison
- url: https://en.cppreference.com/w/cpp/named_req/Compare.html
  title: C++ named requirements, Compare, cppreference.com
  locator: Requirements
- url: https://doc.rust-lang.org/std/primitive.slice.html
  title: Primitive Type slice, The Rust Standard Library
  locator: sort_by; sort_by_key
- url: https://docs.python.org/3/glossary.html#term-key-function
  title: Glossary, Python documentation
  locator: key function
- url: https://docs.python.org/3/howto/sorting.html
  title: Sorting Techniques, Python documentation
  locator: Comparison Functions
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/lang/Comparable.html
  title: Interface Comparable, Java SE 25 API
  locator: Description
- url: https://doc.rust-lang.org/std/cmp/trait.Ord.html
  title: Trait Ord, The Rust Standard Library
  locator: Description
- url: https://hackage.haskell.org/package/base/docs/Data-List.html
  title: Data.List, Haskell base library
  locator: sortBy; sortOn
- url: https://learn.microsoft.com/en-us/dotnet/api/system.collections.generic.comparer-1.create
  title: Comparer<T>.Create(Comparison<T>) Method, .NET API reference
  locator: Definition
- url: https://pkg.go.dev/slices
  title: Package slices, Go standard library
  locator: SortFunc
- url: https://docs.python.org/3/whatsnew/3.0.html
  title: What's New In Python 3.0, Python documentation
  locator: Ordering Comparisons
---

## Idea

A sort must know which of two items comes first. For numbers and text, the language has a default order. For other items, or for another order, the program gives the sort a function. There are two kinds of such functions. A comparator, also called a comparison function, gets two items and returns their order. A key function gets one item and returns a value, the sort key, and the sort compares the keys with the default order.

```python run id=idea
from functools import cmp_to_key

words = ["pear", "fig", "apple", "kiwi"]

# A key function maps one item to a value that Python compares.
print(sorted(words, key=len))


# A comparison function gets two items and returns a negative number, zero or a positive number.
def by_length_then_text(a, b):
    if len(a) != len(b):
        return len(a) - len(b)
    return (a > b) - (a < b)


print(sorted(words, key=cmp_to_key(by_length_then_text)))

# The same order with a key: a tuple compares item by item.
print(sorted(words, key=lambda w: (len(w), w)))
```

```output for=idea
['fig', 'pear', 'kiwi', 'apple']
['fig', 'kiwi', 'pear', 'apple']
['fig', 'kiwi', 'pear', 'apple']
```

The key `len` sorts the words by length. `pear` and `kiwi` have the same length, so they keep their old order. The comparison function `by_length_then_text` compares the lengths first and the text second, so `kiwi` comes before `pear`. `cmp_to_key` changes the comparison function into a key, because Python `sorted()` takes only a key. The tuple key `(len(w), w)` gives the same order as the comparison function, with less code.

## How it works

**The three-way contract.** Most comparators return an integer whose sign is the answer. The Java `compare` method "Returns a negative integer, zero, or a positive integer as the first argument is less than, equal to, or greater than the second"[^1]. Python uses the same rule: "A comparison function is any callable that accepts two arguments, compares them, and returns a negative number for less-than, zero for equality, or a positive number for greater-than"[^2]. JavaScript, C and Go use it too[^3][^4][^5]. In C#, the interface `IComparer<T>` "Defines a method that a type implements to compare two objects"[^6], and the delegate `Comparison<T>` "Represents the method that compares two objects of the same type"[^7]. Both return an `int` with the same rule. Some languages return a special type instead of an integer. Rust returns an `Ordering`, and "An Ordering is the result of a comparison between two values"[^8]. Haskell has the same type with the values `LT`, `EQ` and `GT`[^9]. C++ `<=>` returns an ordering object, and "(a <=> b) < 0 if a < b"[^10].

**The two-way contract.** C++ uses a comparator that returns `bool`. The comparator "yields true if the first argument of the call appears before the second in the strict weak ordering relation induced by this type, and false otherwise"[^11]. The sort calls it twice to find that two items are equal: when `comp(a, b)` and `comp(b, a)` are both false, the items are equivalent.

**The rules of an order.** A comparator must describe a consistent order, or the sort can give a wrong result. MDN lists the rules for a JavaScript compare function[^3]:

| Rule | Meaning |
|---|---|
| pure | the comparator does not change its arguments and has no side effects |
| reflexive | an item compared with itself gives zero |
| anti-symmetric | if `a` comes before `b`, then `b` comes after `a` |
| transitive | if `a` comes before `b` and `b` comes before `c`, then `a` comes before `c` |

Java states the anti-symmetric rule as an equation: "The implementor must ensure that signum(compare(x, y)) == -signum(compare(y, x)) for all x and y"[^1]. For C++ `bool` comparators, the rules are "For all a, comp(a, a) == false. If comp(a, b) == true then comp(b, a) == false"[^11]. A comparator that breaks a rule does not always cause an error. Rust says that if the function "does not implement a total order, the function may panic; even if the function exits normally, the resulting order of elements in the slice is unspecified"[^12].

**Key functions.** A key function is shorter and harder to get wrong. The Python glossary says: "A key function or collation function is a callable that returns a value used for sorting or ordering"[^13]. The key must have a default order, for example a number, a string or a tuple. The Python HOWTO gives the difference between the two kinds: "Unlike key functions that return an absolute value for sorting, a comparison function computes the relative ordering for two inputs"[^14]. Rust `sort_by_key` "Sorts the slice in ascending order with a key extraction function"[^12], and Java `Comparator.comparing` "Accepts a function that extracts a Comparable sort key from a type T, and returns a Comparator<T> that compares by that sort key"[^1].

**Natural order.** A type can have one built-in order. In Java, the interface `Comparable` "imposes a total ordering on the objects of each class that implements it. This ordering is referred to as the class's natural ordering"[^15]. A `Comparator` is a separate object that gives another order, for example by name instead of by date. Rust has the same split: the trait `Ord` is for "types that form a total order"[^16], and `sort_by` takes any other order. Haskell has the class `Ord`, which "is used for totally ordered datatypes"[^9], and `sortBy`, which takes a function.

**Composition.** Libraries have functions that build a comparator from smaller parts:

| Operation | Java | Rust | Go | Python | Haskell |
|---|---|---|---|---|---|
| order by a key | `comparing(f)` | `sort_by_key(f)` | `cmp.Compare(f(a), f(b))` | `key=f` | `comparing f` |
| then by another key | `thenComparing(g)` | `then_with(g)` | `cmp.Or(c1, c2)` | tuple key | `<>` on `Ordering` |
| reverse the order | `reversed()` | `Reverse(k)` or `.reverse()` | swap `a` and `b` | `reverse=True` | `Down k` |
| put nulls first | `nullsFirst(c)` | `Option` order | not in the library | not in the library | `Maybe` order |

Java `thenComparing` "Returns a lexicographic-order comparator with another comparator"[^1]. Rust `then` "Chains two orderings. Returns self when it’s not Equal. Otherwise returns other"[^8]. Go `cmp.Or` "returns the first of its arguments that is not equal to the zero value"[^5], so a zero from the first comparison passes to the second. Haskell defines `comparing p x y = compare (p x) (p y)`[^9], the comparator of a key function in one line.

## In languages

| Language | Verdict | Mechanism |
|---|---|---|
| Python | native | `key=` on `sort`, `sorted`, `min`, `max`; `functools.cmp_to_key` changes a comparison function into a key |
| JavaScript | native | a compare function `(a, b) => number` for `sort` and `toSorted`; `Intl.Collator` and `localeCompare` for text |
| TypeScript | native | the JavaScript compare function, with the type `(a: T, b: T) => number` |
| Java | native | `Comparator<T>` and `Comparable<T>`; `comparing`, `thenComparing`, `reversed`, `nullsFirst` |
| Kotlin | native | `Comparator<T>` and `Comparable<T>`; `compareBy`, `thenBy`, `sortedWith` and `sortedBy` |
| C# | native | `IComparer<T>`, `Comparison<T>` and `IComparable<T>`; LINQ `OrderBy` and `ThenBy` take keys |
| Go | native | `func(a, b T) int` for `slices.SortFunc`; `cmp.Compare` and `cmp.Or` |
| Rust | native | the traits `Ord` and `PartialOrd`; `Ordering`; `sort_by`, `sort_by_key`, `then_with`, `Reverse` |
| C | native | a function pointer `int (*)(const void *, const void *)` for `qsort` and `bsearch` |
| C++ | native | a `bool` comparator for a strict weak ordering; `<=>` returns an ordering object; ranges take projections |
| Bash | idiom | the `sort` command takes keys and options (`-k`, `-n`, `-r`, `-t`), not a function |
| PowerShell | native | `Sort-Object` takes properties or script blocks as keys; `[Array]::Sort` takes a .NET `Comparison` |
| SQL | native | `ORDER BY` takes key expressions with `ASC`, `DESC` and `NULLS FIRST`; `COLLATE` selects a text order |
| Regex | n/a | a pattern matches text and does not order values |
| HTML | n/a | markup does not order values |
| CSS | n/a | the style rules do not order values; the `order` property takes a number, not a function |

Every general-purpose language in the table accepts a function that orders two values. The languages differ in the return type (an integer, a `bool` or an ordering type), in the support for key functions, and in the helpers that build a comparator from keys.

The example files show these differences. Python shows tuple keys, a minus sign that reverses one key, a comparator that returns `bool` and gives a wrong order, and the error for the old `cmp` argument. JavaScript shows the default text order, chained keys with `||` and `Intl.Collator`. Java shows `comparingInt`, `thenComparing`, `reversed`, `nullsFirst` and a comparator `a - b` that overflows. C# shows `Comparison<T>`, `ThenBy` and `Comparer<T>.Create` for a `SortedSet`. Go shows `cmp.Compare`, `cmp.Or` and the order of `NaN`. Rust shows `then_with`, `Reverse`, `total_cmp` for floats and `partial_cmp`. C shows the three-way idiom for `qsort` and a comparator for strings. C++ shows a `bool` comparator, `std::tie`, a projection and a defaulted `<=>`.

## Variations and related ideas

- **Key functions as comparators.** Every key function gives a comparator: compare the keys of the two items. The opposite is not always simple, and Python `cmp_to_key` does it with a wrapper object: it will "Transform an old-style comparison function to a key function"[^2].
- **Decorate, sort, undecorate.** A sort calls a comparator many times for each item, so a key that is slow to compute costs much. A program can compute each key once and sort the pairs of key and item. Haskell `sortOn` does this, and its documentation says that it has "the performance advantage of only evaluating f once for each element in the input list"[^17].
- **Collation.** Text order depends on the language of the text. A collator, for example `Intl.Collator` in JavaScript or `java.text.Collator` in Java, is a comparator for the text of one locale. See [compare strings](/compare-strings/).
- **Ordered collections.** A sorted map, a sorted set and a priority queue keep their items in the order of a comparator. C# `Comparer<T>.Create` "Creates a comparer by using the specified comparison"[^18] for such a collection.
- **Consistency with equality.** Java says: "It is strongly recommended (though not required) that natural orderings be consistent with equals"[^15]. If `compare(a, b)` is zero but `a.equals(b)` is false, a sorted set keeps only one of the two items. See [value equality and identity](/value-equality-and-identity/).
- **Partial orders.** Some values have no order. Rust `partial_cmp` returns `None` for `NaN`, and a sort of floats must use `total_cmp` or handle `NaN` in another way.

## Pitfalls

- **Subtraction overflows.** `(a, b) -> a - b` in Java or C# gives a wrong sign when the difference does not fit in an `int`. In C, signed overflow is undefined behavior. Use `Integer.compare`, `CompareTo`, `cmp.Compare` or `(x > y) - (x < y)`. In JavaScript, MDN says that "(a, b) => a - b sorts numbers in ascending order"[^3], because JavaScript numbers are floating point and do not overflow in this way.
- **A comparator that returns a bool.** A JavaScript or Python comparator that returns `a > b` gives `true` or `false`, which convert to 1 or 0, never to a negative number. The sort then gives a wrong order and no error. C++ is the opposite: its comparators must return `bool`, and a comparator that uses `<=` breaks the rules.
- **No compare function in JavaScript.** Without a compare function, `sort()` converts the items to strings, so `[10, 9, 1].sort()` gives `[1, 10, 9]`.
- **Inconsistent comparators.** A comparator that uses random numbers, a clock or changing state breaks the rules. The Java sorts can throw `IllegalArgumentException` when they find such a comparator, and other sorts give a wrong order with no error.
- **NaN.** `NaN` is not less than, equal to or greater than any number. A comparator that uses `<` on floats is not transitive when the list holds `NaN`. Go `cmp.Compare` puts `NaN` first: "a NaN is considered less than any non-NaN"[^5].
- **Null values.** A Java comparator that calls a method on its argument throws `NullPointerException` for a `null` item. Java `nullsFirst` "Returns a null-friendly comparator that considers null to be less than non-null"[^1].
- **Unstable sorts.** A comparator that returns zero for two items does not fix their order. Go says of `SortFunc`: "This sort is not guaranteed to be stable"[^19]. Use a stable sort or add a key that tells the items apart.

## Cost

A comparison sort of n items calls the comparator about n log n times, so the cost of one call is multiplied by that number. A key function is called once for each item when the library computes the keys first. Python does this, and its sort is fast "because the key function is called exactly once for each input record"[^14]. A comparator that computes keys inside itself computes each key about 2 log n times. A comparator that calls other comparators in a chain stops at the first non-zero result, so the later keys cost time only for items that are equal on the earlier keys. A call through a function pointer, as in C `qsort`, is not inlined, and C++ `std::sort` with a lambda is often faster for that reason.

## History

The C library `qsort` takes "comparison function which returns" a negative, zero or positive integer[^4], and later libraries kept the three-way contract. Java added the interfaces `Comparator` and `Comparable` together with its collections framework[^1][^15].

Python first had both a `cmp` argument and a `key` argument on its sorts. The Python 3.0 release notes say: "sorted() and list.sort() no longer accept the cmp argument providing a comparison function. Use the key argument instead"[^20]. The helper `functools.cmp_to_key` converts old comparison functions[^2]. Newer libraries also prefer keys and composition: Rust has `sort_by_key` and `then_with`[^12][^8], Go has `cmp.Or`[^5], and C++ has the three-way operator `<=>` and range projections[^10].

## References
