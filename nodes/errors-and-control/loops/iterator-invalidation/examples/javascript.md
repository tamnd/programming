```javascript run id=foreach-push-splice-and-filter
// forEach visits only the indexes below the first length.
const a = [1, 2, 3];
a.forEach((x) => {
  if (x < 3) a.push(x * 10);
});
console.log(a);

// A removal shifts the elements, so the next one is skipped.
const b = [1, 2, 2, 3, 4];
const seen = [];
b.forEach((x, i) => {
  seen.push(x);
  if (x % 2 === 0) b.splice(i, 1);
});
console.log("seen:", seen.join(","), "left:", b.join(","));

// filter builds a new array and leaves the old one alone.
const c = [1, 2, 2, 3, 4];
console.log(c.filter((x) => x % 2 !== 0), c.length);

// A Map iterator sees entries added during the loop.
const m = new Map([["a", 1]]);
for (const [k, v] of m) {
  if (v < 3) m.set(k + "+", v + 1);
}
console.log([...m.keys()].join(" "));
```

```output for=foreach-push-splice-and-filter
[ 1, 2, 3, 10, 20 ]
seen: 1,2,3,4 left: 1,2,3
[ 1, 3 ] 5
a a+ a++
```

- Verdict: native. JavaScript defines the result of each change, but the result is easy to get wrong. `forEach` does not visit the two elements that it adds. The `splice` of the first 2 moves the second 2 to the index that the loop has passed, so the loop never sees it, and it stays in the array. A `Map` iterator visits entries in insertion order and sees the new entries, so the loop adds `a+` and then `a++`.
