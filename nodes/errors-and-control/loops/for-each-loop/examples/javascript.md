```javascript run id=for-of-for-in-map-and-code-points
const arr = [10, 20, 30];
arr.extra = "x";
const values = [];
for (const v of arr) values.push(v);
const keys = [];
for (const k in arr) keys.push(k);
console.log("of:", values.join(","));
console.log("in:", keys.join(","));

// A Map gives [key, value] pairs in insertion order.
const m = new Map([["a", 1], ["b", 2]]);
for (const [key, value] of m) console.log(key, value);

// for...of on a string gives code points, not UTF-16 code units.
const s = "a😀";
const chars = [];
for (const ch of s) chars.push(ch);
console.log(s.length, chars.length);
```

```output for=for-of-for-in-map-and-code-points
of: 10,20,30
in: 0,1,2,extra
a 1
b 2
3 2
```

- Verdict: native. `for...of` uses the iterator of the array and gives the three values. `for...in` gives the property names as strings, and it includes the extra property. A `Map` gives pairs that the header takes apart, and a string gives whole code points, so the emoji counts as one element although `length` counts it as two.
