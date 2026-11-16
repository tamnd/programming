```javascript run id=money
let total = 0;
for (let i = 0; i < 10; i++) {
  total += 0.1;
}
console.log(`loop: ${total}`);
console.log(`0.1 to 25 digits: ${(0.1).toPrecision(25)}`);
console.log(`0.1 + 0.2: ${0.1 + 0.2}`);

function kahanSum(values) {
  let sum = 0;
  let c = 0;
  for (const v of values) {
    const y = v - c;
    const t = sum + y;
    c = (t - sum) - y;
    sum = t;
  }
  return sum;
}
console.log(`Kahan: ${kahanSum(Array(10).fill(0.1))}`);

let cents = 0;
for (let i = 0; i < 10; i++) {
  cents += 10;
}
console.log(`cents: ${cents} -> ${(cents / 100).toFixed(2)}`);
console.log(`(1.005).toFixed(2): ${(1.005).toFixed(2)}`);
```

```output for=money
loop: 0.9999999999999999
0.1 to 25 digits: 0.1000000000000000055511151
0.1 + 0.2: 0.30000000000000004
Kahan: 1
cents: 100 -> 1.00
(1.005).toFixed(2): 1.00
```

- Verdict: native. Every JavaScript `number` is a binary64 float.
- `toPrecision(25)` prints digits past the shortest form, so the error becomes visible.
- `(1.005).toFixed(2)` gives `"1.00"` because the stored value is below 1.005.
