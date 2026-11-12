```javascript run id=typed-arrays
const a = Float64Array.from([3, 1, 4, 1, 5]);
const b = Float64Array.from([2, 7, 1, 8, 2]);

const sum = a.map((x, i) => x + b[i]);           // element-wise
const dot = a.reduce((acc, x, i) => acc + x * b[i], 0);
const scaled = a.map((x) => x * 10);              // scalar extension
const big = a.filter((x) => x > 2);               // compress by a test

console.log(Array.from(sum), dot, Array.from(scaled), Array.from(big));
```

```output for=typed-arrays
[ 5, 8, 5, 9, 7 ] 35 [ 30, 10, 40, 10, 50 ] [ 3, 4, 5 ]
```

```javascript wrong id=plus-on-arrays
const a = [1, 2, 3];
const b = [10, 20, 30];
console.log(a + b);
console.log(a * 2);
```

```output for=plus-on-arrays
1,2,310,20,30
NaN
```

- Verdict: idiom. Arrays and typed arrays have `map`, `filter` and `reduce`, which take a function and apply it to each element, but the arithmetic operators do not work on arrays. A `Float64Array` stores the numbers in one block of memory, which is the layout that array libraries and WebGPU use.
- The wrong block uses `+` and `*` on arrays. JavaScript converts each array to a string, so `+` joins two strings and `*` gives `NaN`.
