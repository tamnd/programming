---
tag: XB0HCF
kind: feature
owner: javascript
slug: copying-array-methods
name: Copying array methods
aliases: [toSorted, toReversed, toSpliced, Array.prototype.with, change array by copy, non-mutating array methods]
area: languages/javascript/collections
level: core
summary: >-
  toSorted, toReversed, toSpliced and with are the copying versions of sort, reverse, splice and an index write. Each returns a new array and leaves the original as it was.
grammar: null
spec: {standard: "ECMA-262, 17th edition (ECMAScript 2026)", url: "https://tc39.es/ecma262/2026/multipage/indexed-collections.html#sec-array.prototype.tosorted", locator: "23.1.3.33 to 23.1.3.35 and 23.1.3.39"}
see_also: [javascript/arrays, javascript/array-methods, javascript/sorting-arrays, in-place-and-copying-operations]
sources:
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Array/toSorted
  title: MDN Web Docs, JavaScript Reference, Array.prototype.toSorted()
  locator: the introduction; Description
  accessed: 2026-11-18
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Array/toReversed
  title: MDN Web Docs, JavaScript Reference, Array.prototype.toReversed()
  locator: the introduction; Description
  accessed: 2026-11-18
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Array/toSpliced
  title: MDN Web Docs, JavaScript Reference, Array.prototype.toSpliced()
  locator: the introduction; Description
  accessed: 2026-11-18
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Array/with
  title: MDN Web Docs, JavaScript Reference, Array.prototype.with()
  locator: the introduction; Exceptions; Description
  accessed: 2026-11-18
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Array
  title: MDN Web Docs, JavaScript Reference, Array
  locator: Copying methods and mutating methods
  accessed: 2026-11-18
- url: https://tc39.es/ecma262/2026/multipage/indexed-collections.html
  title: ECMAScript 2026 Language Specification (ECMA-262, 17th edition)
  locator: 23.1.3.39 Array.prototype.with
  accessed: 2026-11-18
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Array/sort
  title: MDN Web Docs, JavaScript Reference, Array.prototype.sort()
  locator: the introduction; Return value
  accessed: 2026-11-18
- url: https://github.com/tc39/proposals/blob/main/finished-proposals.md
  title: TC39, Finished Proposals
  locator: the row Change Array by Copy
  accessed: 2026-11-18
- url: https://docs.python.org/3.14/library/functions.html#sorted
  title: The Python Standard Library, Python 3.14, Built-in Functions
  locator: sorted()
  accessed: 2026-11-18
- url: https://docs.python.org/3.14/library/stdtypes.html#list.sort
  title: The Python Standard Library, Python 3.14, Built-in Types
  locator: Lists, list.sort
  accessed: 2026-11-18
- url: https://pkg.go.dev/slices
  title: Go standard library, package slices
  locator: func Clone; func Reverse
  accessed: 2026-11-18
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/util/stream/Stream.html
  title: Java SE 25 API, Interface Stream
  locator: sorted()
  accessed: 2026-11-18
---

## Synopsis

```javascript run id=pairs
const scores = [30, 10, 20];

const sorted = scores.toSorted((a, b) => a - b);
const reversed = scores.toReversed();
const spliced = scores.toSpliced(1, 1, 15, 16);
const changed = scores.with(0, 99);

console.log(scores);
console.log(sorted, reversed);
console.log(spliced, changed);
```

```output for=pairs
[ 30, 10, 20 ]
[ 10, 20, 30 ] [ 20, 10, 30 ]
[ 30, 15, 16, 20 ] [ 99, 10, 20 ]
```

| Mutating form | Copying form | The copy returns |
|---|---|---|
| `a.sort(cmp)` | `a.toSorted(cmp)` | the elements in sorted order |
| `a.reverse()` | `a.toReversed()` | the elements in reverse order |
| `a.splice(i, n, ...items)` | `a.toSpliced(i, n, ...items)` | the array with `n` elements at `i` replaced by `items` |
| `a[i] = v` | `a.with(i, v)` | the array with the element at `i` replaced by `v` |

## Semantics

**r1.** `toSorted` "is the copying version of the sort() method. It returns a new array with the elements sorted in ascending order."[^1] `toReversed` "is the copying counterpart of the reverse() method."[^2] `toSpliced` "is the copying version of the splice() method."[^3] `with` "is the copying version of using the bracket notation to change the value of a given index."[^4] Example: [`pairs`](#pairs).

**r2.** `toSorted` takes the same comparator as `sort`.[^1] The rules for the comparator are on the page [[javascript/sorting-arrays]].

**r3.** `toSpliced` takes the arguments of `splice`. Like `splice`, "it removes the given number of elements from the array, starting at a given index, and then inserts the given elements at the same index."[^3] The difference is the result: "However, it returns a new array instead of modifying the original array."[^3] `splice` returns "the array of removed elements", and `toSpliced` returns the whole new array.[^5] Example: [`splice`](#splice).

**r4.** `with(index, value)` accepts a negative index, which counts from the end: the specification adds it to the length.[^6] Unlike a write with brackets, it never grows the array. The specification says: "If actualIndex ≥ len or actualIndex < 0, throw a RangeError exception."[^6] Example: [`with-range`](#with-range).

**r5.** The copying methods never return a sparse array. Each one reads an empty slot as `undefined`: for `toSorted`, "When used on sparse arrays, the toSorted() method iterates empty slots as if they have the value undefined".[^1] `toSpliced` and `with` say the same: "the empty slots will be replaced with undefined in the new array."[^3][^4]

**r6.** The copy is shallow. "The copy always happens shallowly", so an object in the copy is the same object as in the original.[^5] Example: [`shallow`](#shallow).

**r7.** The mutating methods change the array and return it. `sort` "sorts the elements of an array in place and returns the reference to the same array, now sorted."[^7] Before the copying methods, a program made a copy first: "An easy way to change a mutating method into a non-mutating alternative is to use the spread syntax or slice() to create a copy first".[^5]

## Examples

`splice` against `toSpliced` (rule r3).

```javascript run id=splice
const months = ["Jan", "Mar", "Apr"];
const inserted = months.toSpliced(1, 0, "Feb");
console.log(inserted, months);

const removed = months.splice(1, 1);
console.log(removed, months);
```

```output for=splice
[ 'Jan', 'Feb', 'Mar', 'Apr' ] [ 'Jan', 'Mar', 'Apr' ]
[ 'Mar' ] [ 'Jan', 'Apr' ]
```

A chain of copying methods leaves the input as it was (rules r1 and r4).

```javascript run id=chain
const ranks = ["b", "c", "a"];
const top = ranks.toSorted().with(-1, "z").toReversed();
console.log(top, ranks);
```

```output for=chain
[ 'z', 'b', 'a' ] [ 'b', 'c', 'a' ]
```

## Versions

The four methods come from the TC39 proposal Change Array by Copy. The TC39 list of finished proposals gives 2023 as its publication year, so the methods are in ECMAScript 2023 and later.[^8] Node v24 has them.

## Pitfalls

### `with` does not grow the array

A write with brackets past the end makes the array longer. `with` throws instead (rule r4).

```javascript wrong id=with-range
const slots = ["a", "b"];
try {
  slots.with(2, "c");
} catch (e) {
  console.log(e.name);
}
slots[2] = "c";
console.log(slots);
```

```output for=with-range
RangeError
[ 'a', 'b', 'c' ]
```

Use `toSpliced(a.length, 0, v)` or a spread, `[...a, v]`, to add an element in a copy.

### A copy that shares its objects

`toSorted` gives a new array, but the elements are the same objects (rule r6).

```javascript wrong id=shallow
const users = [{ name: "Bo", age: 40 }, { name: "Al", age: 30 }];
const byAge = users.toSorted((a, b) => a.age - b.age);
byAge[0].age = 31;
console.log(users[1].age, byAge[0] === users[1]);
```

```output for=shallow
31 true
```

Copy the objects too when the copy must change them, for example with `users.map((u) => ({ ...u }))`.

## Comparison

Python has the same pair for sorting: `sorted` "Return a new sorted list from the items in iterable", and `list.sort` "sorts the list in place".[^9][^10] Go's `slices` package has in-place functions such as `Reverse`, which "reverses the elements of the slice in place", and `Clone` makes the copy first; "this is a shallow clone."[^11] Java sorts a stream into a new stream: `sorted()` "Returns a stream consisting of the elements of this stream, sorted according to natural order."[^12] See [[in-place-and-copying-operations]].

## References
