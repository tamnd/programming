```javascript run id=at-and-undefined
const a = ["a", "b", "c", "d", "e"];

// Indices start at 0. at() accepts a negative index and counts from the end.
console.log(a[0], a[a.length - 1], a.at(-1), a.at(-2));

// Brackets do not count from the end: a[-1] looks up a property named "-1".
console.log(a[-1], a.at(-6));

// An index past the end gives undefined. It does not throw.
console.log(a[5], a[99]);

// An index is converted to a string property key, so a["2"] is a[2].
console.log(a["2"], Object.keys(a).slice(0, 3));

// A write far past the end makes a sparse array with holes.
const b = [1, 2];
b[5] = 6;
console.log(b.length, b, 3 in b);

// A typed array ignores a write out of bounds.
const t = new Int32Array(3);
t[3] = 7;
console.log(t.length, t[3], t);
```

```output
a e e d
undefined undefined
undefined undefined
c [ '0', '1', '2' ]
6 [ 1, 2, <3 empty items>, 6 ] false
3 undefined Int32Array(3) [ 0, 0, 0 ]
```

- Verdict: native. Indices start at 0, and at() counts from the end with a negative index. a[-1] looks up a property named "-1" and gives undefined. An index past the end gives undefined and does not throw. Index keys are strings. A write far past the end makes a sparse array, and a typed array ignores a write out of bounds.
