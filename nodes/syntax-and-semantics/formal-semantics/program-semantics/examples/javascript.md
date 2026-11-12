```javascript run id=spec-algorithm
// IsLooselyEqual from the ECMAScript specification, for primitive values.
function looselyEqual(x, y) {
  if (typeof x === typeof y) return x === y; // step 1: same type, strict equality
  if (x == null && y == null) return true; // steps 2 and 3: null and undefined
  if (typeof x === "number" && typeof y === "string") return x === Number(y); // step 5
  if (typeof x === "string" && typeof y === "number") return Number(x) === y; // step 6
  if (typeof x === "boolean") return looselyEqual(Number(x), y); // step 9
  if (typeof y === "boolean") return looselyEqual(x, Number(y)); // step 10
  return false; // step 14
}

const values = [null, undefined, 0, 1, "", "0", "1", "a", false, true, NaN];
const show = (v) => (typeof v === "string" ? JSON.stringify(v) : String(v));
let pairs = 0, agree = 0;
const equal = [];
for (const a of values) {
  for (const b of values) {
    pairs++;
    if (looselyEqual(a, b) === (a == b)) agree++;
    if (looselyEqual(a, b) && a !== b && show(a) < show(b)) equal.push(`${show(a)} == ${show(b)}`);
  }
}
console.log(`${agree} of ${pairs} pairs agree with ==`);
console.log(equal.join(", "));
```

```output for=spec-algorithm
121 of 121 pairs agree with ==
null == undefined, 0 == false, 1 == true, "" == 0, "" == false, "0" == 0, "0" == false, "1" == 1, "1" == true
```

- Verdict: absent. The ECMAScript specification defines each operation as a numbered algorithm in precise English, which is an operational semantics in prose, and research groups have turned it into formal and executable semantics. The example writes the steps of the algorithm `IsLooselyEqual` for primitive values as a function and compares it with `==` on all 121 pairs of eleven values. The two agree on every pair, and the list shows the pairs that `==` makes equal even though their types differ.
