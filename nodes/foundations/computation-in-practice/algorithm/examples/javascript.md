```javascript run id=sort-order-and-lower-bound
const nums = [10, 9, 1, 100, 25];
console.log("default sort:", [...nums].sort());
console.log("numeric sort:", [...nums].sort((a, b) => a - b));

// JavaScript has no binary search in the standard library.
function lowerBound(xs, target) {
  let lo = 0;
  let hi = xs.length;
  while (lo < hi) {
    const mid = (lo + hi) >>> 1;
    if (xs[mid] < target) lo = mid + 1;
    else hi = mid;
  }
  return lo;
}

const sorted = [...nums].sort((a, b) => a - b);
for (const t of [25, 26]) {
  const i = lowerBound(sorted, t);
  console.log(t, "index", i, "found", sorted[i] === t);
}
```

```output for=sort-order-and-lower-bound
default sort: [ 1, 10, 100, 25, 9 ]
numeric sort: [ 1, 9, 10, 25, 100 ]
25 index 3 found true
26 index 4 found false
```

- Verdict: stdlib. `Array.prototype.sort` sorts in place and is stable since ECMAScript 2019. With no comparison function it converts the items to strings, so 100 comes before 25 and 9. The fix is a comparison function such as `(a, b) => a - b`, or `toSorted` for a sorted copy. The standard library has no binary search, so the file writes a lower bound with a half-open range; `>>> 1` halves the sum as an unsigned 32-bit value, which works for any array index.
