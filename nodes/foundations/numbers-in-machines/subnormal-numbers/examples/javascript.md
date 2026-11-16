```javascript run id=tiny
const minNormal = 2 ** -1022;
console.log("Number.MIN_VALUE:", Number.MIN_VALUE);
console.log("smallest normal: ", minNormal);
console.log("MIN_VALUE / 2:   ", Number.MIN_VALUE / 2);

const x = 1.25 * minNormal;
const y = minNormal;
console.log("x === y:", x === y, "x - y:", x - y);

let v = 1;
let steps = 0;
let firstSubnormal = 0;
while (v > 0) {
  v /= 2;
  steps += 1;
  if (firstSubnormal === 0 && v > 0 && v < minNormal) {
    firstSubnormal = steps;
  }
}
console.log("halvings of 1 to the first subnormal:", firstSubnormal);
console.log("halvings of 1 to zero:", steps);
```

```output for=tiny
Number.MIN_VALUE: 5e-324
smallest normal:  2.2250738585072014e-308
MIN_VALUE / 2:    0
x === y: false x - y: 5.562684646268003e-309
halvings of 1 to the first subnormal: 1023
halvings of 1 to zero: 1075
```

- Verdict: native. JavaScript numbers are IEEE 754 doubles.
- `Number.MIN_VALUE` is the smallest subnormal in the main engines, not the smallest normal value. JavaScript has no constant for the smallest normal value, so the code computes `2 ** -1022`.
- Halving 1 gives normal values for 1022 steps, then subnormals for 52 more steps, and then 0.
