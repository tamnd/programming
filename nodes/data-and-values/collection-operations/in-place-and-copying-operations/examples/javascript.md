```javascript run id=sort-returns-same-array
// sort() sorts in place and returns the same array.
const a = [3, 1, 2];
const b = a.sort();
console.log(a, b, a === b);

// toSorted() returns a new array and leaves the original alone.
const c = [3, 1, 2];
const d = c.toSorted();
console.log(c, d, c === d);

// reverse() also mutates; toReversed() copies.
const e = [1, 2, 3];
const f = e.toReversed();
e.reverse();
console.log(e, f);

// with() is the copying form of an index assignment.
const g = [1, 2, 3];
console.log(g.with(0, 9), g);

// The default order compares strings, in place or not.
console.log([10, 9, 1].toSorted(), [10, 9, 1].toSorted((x, y) => x - y));

// The copy is shallow: both arrays hold the same objects.
const people = [{ name: "b" }, { name: "a" }];
const byName = people.toSorted((x, y) => x.name.localeCompare(y.name));
byName[0].name = "changed";
console.log(people.map((p) => p.name));

// A frozen array rejects sort(), but toSorted() works.
const frozen = Object.freeze([2, 1]);
try {
  frozen.sort();
} catch (err) {
  console.log(err.constructor.name);
}
console.log(frozen.toSorted());
```

```output
[ 1, 2, 3 ] [ 1, 2, 3 ] true
[ 3, 1, 2 ] [ 1, 2, 3 ] false
[ 3, 2, 1 ] [ 3, 2, 1 ]
[ 9, 2, 3 ] [ 1, 2, 3 ]
[ 1, 10, 9 ] [ 1, 9, 10 ]
[ 'b', 'changed' ]
TypeError
[ 1, 2 ]
```

- Verdict: native. `sort()` returns the same array, so `a === b` is `true`. `toSorted()`, `toReversed()` and `with()` return new arrays and leave the original alone. Both forms compare strings by default, so a numeric sort needs `(x, y) => x - y`. The copy is shallow, so a change to an object shows in both arrays. `sort()` on a frozen array throws a `TypeError`, but `toSorted()` works.
