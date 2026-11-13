---
tag: K46N2K
kind: technique
slug: schwartzian-transform
name: Decorate, sort, undecorate
aliases: [Schwartzian transform, decorate-sort-undecorate, DSU, cached sort key, sort by cached key, Lisp in Perl, map-sort-map]
area: data-and-values/collection-operations
level: advanced
summary: >-
  Compute the sort key of each item once, sort the pairs of key and item, and then remove the keys. The sort then does not compute a slow key again at each comparison.
see_also: [stable-sort, comparator, sort-with-a-custom-key, sort-a-list-of-records, map-over-a-list, higher-order-functions]
sources:
- url: https://perldoc.perl.org/perlfaq4
  title: perlfaq4, Perl documentation
  locator: How do I sort an array by (anything)?
- url: https://docs.python.org/3/howto/sorting.html
  title: Sorting Techniques, Python documentation
  locator: Decorate-Sort-Undecorate
- url: https://doc.rust-lang.org/std/primitive.slice.html
  title: Primitive Type slice, The Rust Standard Library
  locator: sort_by_key; sort_by_cached_key
- url: https://docs.ruby-lang.org/en/master/Enumerable.html
  title: module Enumerable, Ruby documentation
  locator: sort; sort_by
- url: https://hackage.haskell.org/package/base/docs/Data-List.html
  title: Data.List, Haskell base library
  locator: sortOn
- url: https://docs.python.org/3/whatsnew/2.4.html
  title: What's New in Python 2.4, Python documentation
  locator: Other Language Changes
- url: https://en.wikipedia.org/wiki/Schwartzian_transform
  title: Schwartzian transform, Wikipedia
  locator: History
---

## Purpose

A sort compares items many times. A sort of n items makes about n log n comparisons, so each item takes part in about 2 log n of them. When the comparison computes a key from each item, for example the size of a file, a value parsed from a line of text or a lower-case form of a long string, the sort computes the key of each item many times. For a slow key, this repeated work costs more than the sort itself. The Perl FAQ gives the rule: "If you have a complicated function needed to pull out the part you want to sort on, then don't do it inside the sort function. Pull it out first, because the sort BLOCK can be called many times for the same element"[^1].

The technique computes each key once. The Python HOWTO names it: "This idiom is called Decorate-Sort-Undecorate after its three steps"[^2]. Perl programmers call it the Schwartzian transform.

## Steps

1. **Decorate.** Make a new list. For each item, put a pair or a tuple in it: the key first, then the item. The key function runs once for each item, so n times.
2. **Sort.** Sort the new list by the key part only. The comparison now reads a stored key, which is fast.
3. **Undecorate.** Make the result list from the item part of each pair, in the sorted order.

If the sort is not stable, or if the sort compares whole tuples, put the original position between the key and the item. Two positions are never equal, so the sort never compares two items. The Python HOWTO gives the two benefits of the position: "The sort is stable" and "The original items do not have to be comparable because the ordering of the decorated tuples will be determined by at most the first two items"[^2]. See [stable sort](/stable-sort/).

In Perl, the three steps are one expression of `map`, `sort` and `map`, read from the bottom up. This example sorts file names by the size of each file, and calls `-s` once for each name:

```perl
my @by_size =
    map  { $_->[1] }               # 3. undecorate: keep the name
    sort { $a->[0] <=> $b->[0] }   # 2. sort by the stored size
    map  { [ -s $_, $_ ] }         # 1. decorate: [size, name]
    @files;
```

## Example

In Python. The key function `vowels` counts the vowels in a word, and a counter shows how many times it runs. The first sort uses a comparison function that computes two keys at each comparison. The second sort does the three steps by hand. The third sort uses the `key` argument, which does the same steps inside the sort.

```python run id=count-key-calls
from functools import cmp_to_key

calls = 0


def vowels(word):
    """The sort key: the number of vowels. The counter shows how often it runs."""
    global calls
    calls += 1
    return sum(ch in "aeiou" for ch in word)


words = ["banana", "kiwi", "apple", "fig", "cherry", "date", "plum", "mango"]

# Without the technique: the comparison computes two keys at each comparison.
calls = 0
slow = sorted(words, key=cmp_to_key(lambda a, b: vowels(a) - vowels(b)))
print(slow, calls)

# Decorate: put the key and the position in front of each item.
calls = 0
decorated = [(vowels(w), i, w) for i, w in enumerate(words)]
# Sort: the tuples compare by key, then by position, and never by the item.
decorated.sort()
# Undecorate: keep only the items.
fast = [w for _, _, w in decorated]
print(fast, calls)

# The key argument does the three steps inside the sort.
calls = 0
print(sorted(words, key=vowels) == fast, calls)
```

```output for=count-key-calls
['fig', 'cherry', 'plum', 'kiwi', 'apple', 'date', 'mango', 'banana'] 36
['fig', 'cherry', 'plum', 'kiwi', 'apple', 'date', 'mango', 'banana'] 8
True 8
```

All three sorts give the same order. The comparison function calls `vowels` many more times than there are words. The decorated sort and the `key` argument call it exactly 8 times, once for each word. The words with equal counts, for example `kiwi`, `apple`, `date` and `mango`, keep their input order, because of the position in the tuple and because Python sorts are stable.

The example files do the same in JavaScript, Java, Go, Rust and C++. Each file first sorts with a key that is computed at each comparison and shows that the key runs more than 8 times, and then does the three steps and shows 8 calls.

## When to use

Use the technique when the key is slow to compute and the list is long: a key that reads a file, parses text, calls a function over the network or normalizes long strings. The time of the sort then falls from about n log n key calls to n key calls.

Do not use it for a fast key, such as a field of a record. The decorated list needs memory for n pairs and time to build and take apart, and this costs more than the key calls that it saves. Rust says of its cached form: "For simple key functions (e.g., functions that are property accesses or basic operations), sort_by_key is likely to be faster"[^3]. Ruby says that its `sort_by` "generates an array of tuples containing the original collection element and the mapped value. This makes sort_by fairly expensive when the keysets are simple"[^4].

In a language whose sort takes a key function and computes each key once, the technique is built in. The Python HOWTO says: "Now that Python sorting provides key-functions, this technique is not often needed"[^2].

## Pitfalls

- **A key argument is not always cached.** Some library functions take a key but call it at each comparison. Rust `sort_by_key` is "O(m * n * log(n)) worst-case, where the key function is O(m)"[^3], so it calls the key many times. Java `Comparator.comparing`, Go `slices.SortFunc` with a key inside and C++ range projections do the same. Check the documentation, or count the calls as the example does.
- **Comparing the items by accident.** If two keys are equal, a sort of `(key, item)` tuples compares the items. This is slow, it can raise an error when the items cannot be compared, and it changes the order of ties. Put the position between the key and the item, or sort by the key part only.
- **Losing stability.** An unstable sort of decorated pairs can reorder items with equal keys. Use a stable sort or add the position.
- **Keys that change.** The technique computes each key before the sort. If the key depends on data that changes during the sort, for example a file that grows, the sort uses the old value. This is usually what the program wants, but it differs from a comparator that reads the key each time.
- **Memory.** The decorated list holds n keys and n references. For a very large list with large keys, such as the full text of documents, the extra memory can be a problem. Use a smaller key, such as a hash or a prefix, when it gives the same order.

## Tools that do it

| Language | Built in | By hand |
|---|---|---|
| Python | `sort(key=f)`, `sorted(key=f)`, `min` and `max` with `key=` | list of `(key, i, item)` tuples |
| Perl | none | `map`, `sort`, `map` |
| Ruby | `sort_by` | `map`, `sort`, `map` |
| Haskell | `sortOn` | `map`, `sortBy`, `map` |
| Rust | `sort_by_cached_key` | `Vec<(K, T)>` with `sort_by_key` |
| JavaScript | none | `map`, `sort`, `map` |
| Java | none | `stream().map(...).sorted(...).map(...)` with a record |
| C# | none in the documentation | `Select`, `OrderBy`, `Select` |
| Go | none | a slice of structs with `slices.SortStableFunc` |
| C++ | none | a vector of pairs with `std::ranges::stable_sort` |

Ruby says that `sort_by` "implements a Schwartzian transform which is useful when key computation or comparison is expensive"[^4]. Haskell says: "sortOn f is equivalent to sortBy (comparing f), but has the performance advantage of only evaluating f once for each element in the input list. This is called the decorate-sort-undecorate paradigm, or Schwartzian transform"[^5]. Rust says of `sort_by_cached_key`: "During sorting, the key function is called at most once per element, by using temporary storage to remember the results of key evaluation"[^3]. When Python added the `key` argument, its release notes said: "Using key calls lower() method once for each element in the list while using cmp will call it twice for each comparison, so using key saves on invocations of the lower() method"[^6].

The name comes from Randal L. Schwartz. Wikipedia says that "The idiom is named after Randal L. Schwartz, who first demonstrated it in Perl shortly after the release of Perl 5 in 1994", and that it "is a version of a Lisp idiom known as decorate-sort-undecorate"[^7]. The Python HOWTO also gives the name: "Another name for this idiom is Schwartzian transform, after Randal L. Schwartz, who popularized it among Perl programmers"[^2].

## References
