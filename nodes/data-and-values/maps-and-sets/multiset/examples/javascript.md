```javascript run id=map-of-counts
// JavaScript has no multiset. A Map from item to count is the usual form.
function count(items) {
  const m = new Map();
  for (const x of items) m.set(x, (m.get(x) ?? 0) + 1);
  return m;
}

const c = count("the cat and the hat and the bat".split(" "));
console.log([...c].map(([k, v]) => `${k}=${v}`).join(" "));
console.log(c.get("the"), c.get("dog") ?? 0);

// The most common items: sort the entries by count, then by name.
const top = [...c].sort((a, b) => b[1] - a[1] || a[0].localeCompare(b[0])).slice(0, 2);
console.log(top);

// Two multisets are equal when they have the same keys with the same counts.
function equal(a, b) {
  if (a.size !== b.size) return false;
  for (const [k, v] of a) if (b.get(k) !== v) return false;
  return true;
}
console.log(equal(count("listen"), count("silent")), equal(count("aab"), count("ab")));

// Remove one copy: lower the count, and delete the key at zero.
function removeOne(m, x) {
  const n = m.get(x) ?? 0;
  if (n <= 1) m.delete(x);
  else m.set(x, n - 1);
}
removeOne(c, "and");
removeOne(c, "cat");
console.log(c.get("and"), c.has("cat"), c.size);
```

```output
the=3 cat=1 and=2 hat=1 bat=1
3 0
[ [ 'the', 3 ], [ 'and', 2 ] ]
true false
1 false 4
```

- Verdict: idiom. JavaScript has no multiset. A Map from item to count, updated with (m.get(x) ?? 0) + 1. Equality, the most common items and the removal of one copy are short functions.
