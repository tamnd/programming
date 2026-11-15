```javascript run id=undefined-past-the-end-and-slice
const scores = [70, 85, 90];
let total = 0;
for (let i = 0; i <= scores.length; i++) {
  total += scores[i]; // the last pass adds undefined
}
console.log("total with <=:", total);

total = 0;
for (let i = 0; i < scores.length; i++) {
  total += scores[i];
}
console.log("total with <:", total);

// slice and substring exclude the end index.
const s = "hamburger";
console.log(s.slice(4, 8), s.substring(4, 8).length);
console.log(s.slice(0, 3) + s.slice(3) === s);

// The last element is at length - 1. at(-1) counts from the end.
console.log(scores[scores.length], scores[scores.length - 1], scores.at(-1));

// Separators go between the items.
console.log(["a", "b", "c"].join("-"));
```

```output for=undefined-past-the-end-and-slice
total with <=: NaN
total with <: 245
urge 4
true
undefined 90 90
a-b-c
```

- Verdict: stdlib. JavaScript does not stop a read past the end of an array: `scores[3]` is `undefined`, and `245 + undefined` is `NaN`, so the bug shows up only in the result. `slice(4, 8)` and `substring(4, 8)` take the characters at 4, 5, 6 and 7, and two slices that share the index 3 give the whole string back.
