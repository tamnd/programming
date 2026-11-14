```javascript run id=set-object
// A Set keeps each value once, in insertion order.
const s = new Set([3, 1, 3, 2, 1]);
console.log(s.size, [...s], s.has(2), s.has(5));

// add returns the set, so calls chain. A value that is there already is ignored.
s.add(2).add(4);
console.log([...s]);

// Values are compared with SameValueZero: NaN is one value, and 0 and -0 are one value.
const n = new Set([NaN, NaN, 0, -0]);
console.log(n.size, n.has(NaN));

// Objects are compared by identity, so two equal-looking arrays are two elements.
const arrays = new Set([[1, 2], [1, 2]]);
console.log(arrays.size);

// Set methods for set algebra (ES2025).
const a = new Set([1, 2, 3]);
const b = new Set([2, 3, 4]);
console.log([...a.union(b)], [...a.intersection(b)], [...a.difference(b)], [...a.symmetricDifference(b)]);
console.log(new Set([2, 3]).isSubsetOf(a), a.isDisjointFrom(new Set([7, 8])));

// A Set removes duplicates from an array and keeps the first order.
console.log([...new Set(["b", "a", "b", "c", "a"])]);
```

```output
3 [ 3, 1, 2 ] true false
[ 3, 1, 2, 4 ]
2 true
2
[ 1, 2, 3, 4 ] [ 2, 3 ] [ 1 ] [ 1, 4 ]
true true
[ 'b', 'a', 'c' ]
```

- Verdict: native. A Set keeps each value once, in insertion order, and compares values with SameValueZero, so NaN is one value. Arrays and objects are elements by identity. ES2025 adds union, intersection, difference, symmetricDifference, isSubsetOf and isDisjointFrom.
