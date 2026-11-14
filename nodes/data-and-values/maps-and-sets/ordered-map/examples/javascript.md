```javascript run id=sort-entries
// JavaScript has no ordered map. A Map keeps insertion order, not key order.
const m = new Map([[30, "c"], [10, "a"], [20, "b"]]);
console.log([...m.keys()]);

// The default sort compares strings, so 10 comes before 9.
console.log([10, 9, 1].sort(), [10, 9, 1].sort((a, b) => a - b));

// Sort the entries to get the key order. This copies the map each time.
const sorted = new Map([...m].sort((a, b) => a[0] - b[0]));
console.log([...sorted.keys()]);

// Floor: the largest key <= 25, with a binary search on a sorted key array.
const keys = [...sorted.keys()];
function floor(ks, x) {
  let lo = 0, hi = ks.length;
  while (lo < hi) {
    const mid = (lo + hi) >> 1;
    if (ks[mid] <= x) lo = mid + 1;
    else hi = mid;
  }
  return lo > 0 ? ks[lo - 1] : undefined;
}
console.log(floor(keys, 25), floor(keys, 5));

// Strings in an object: integer-like keys come first in numeric order, then the others in insertion order.
const o = { b: 1, 10: 2, a: 3, 2: 4 };
console.log(Object.keys(o));
```

```output
[ 30, 10, 20 ]
[ 1, 10, 9 ] [ 1, 9, 10 ]
[ 10, 20, 30 ]
20 undefined
[ '2', '10', 'b', 'a' ]
```

- Verdict: idiom. JavaScript has no sorted map. A Map keeps insertion order, so a program sorts its entries into a new Map or an array. The default sort compares strings, so a number sort needs a compare function. A floor search is a hand-written binary search.
