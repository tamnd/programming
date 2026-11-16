```javascript run id=traps
const values = [3, NaN, 1, 2];
console.log(`NaN === NaN: ${NaN === NaN}`);
console.log(`Number.isNaN(NaN): ${Number.isNaN(NaN)}`);
console.log(`isNaN("abc"): ${isNaN("abc")}, Number.isNaN("abc"): ${Number.isNaN("abc")}`);
console.log(`indexOf(NaN): ${values.indexOf(NaN)}, includes(NaN): ${values.includes(NaN)}`);

const set = new Set([NaN, NaN, 0, -0]);
console.log(`Set size: ${set.size}`);
console.log(`Object.is(NaN, NaN): ${Object.is(NaN, NaN)}, Object.is(0, -0): ${Object.is(0, -0)}`);
console.log(`1 / -0: ${1 / -0}`);
console.log(`Math.max(1, NaN): ${Math.max(1, NaN)}`);
console.log(`JSON: ${JSON.stringify([NaN, Infinity, -0])}`);
console.log(`sorted: ${[...values].sort((a, b) => a - b)}`);
```

```output for=traps
NaN === NaN: false
Number.isNaN(NaN): true
isNaN("abc"): true, Number.isNaN("abc"): false
indexOf(NaN): -1, includes(NaN): true
Set size: 2
Object.is(NaN, NaN): true, Object.is(0, -0): false
1 / -0: -Infinity
Math.max(1, NaN): NaN
JSON: [null,null,0]
sorted: 3,NaN,1,2
```

- Verdict: native. JavaScript numbers are binary64, with `NaN`, `Infinity` and `-0`.
- The global `isNaN` converts its argument first, so a string that is not a number gives true. `Number.isNaN` does not convert.
- `Set` and `includes` use the SameValueZero rule, so they find NaN and treat `0` and `-0` as one value.
- The comparator `a - b` returns NaN for the NaN element, and the sort leaves the array out of order.
