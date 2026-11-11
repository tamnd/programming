```javascript run id=number
const x = 0.1 + 0.2;
console.log("0.1 + 0.2 =", x);
console.log("=== 0.3   :", x === 0.3);
console.log("near 0.3  :", Math.abs(x - 0.3) < Number.EPSILON);
console.log("0.1 to 20 digits:", (0.1).toPrecision(20));
console.log("EPSILON   :", Number.EPSILON);
console.log("MAX_SAFE_INTEGER:", Number.MAX_SAFE_INTEGER);
console.log("2**53 + 1 :", 2 ** 53 + 1);
console.log("(1.005).toFixed(2):", (1.005).toFixed(2));
console.log("NaN === NaN:", NaN === NaN, " Object.is(NaN, NaN):", Object.is(NaN, NaN));
console.log("0 === -0  :", 0 === -0, " Object.is(0, -0):", Object.is(0, -0));
```

```output
0.1 + 0.2 = 0.30000000000000004
=== 0.3   : false
near 0.3  : true
0.1 to 20 digits: 0.10000000000000000555
EPSILON   : 2.220446049250313e-16
MAX_SAFE_INTEGER: 9007199254740991
2**53 + 1 : 9007199254740992
(1.005).toFixed(2): 1.00
NaN === NaN: false  Object.is(NaN, NaN): true
0 === -0  : true  Object.is(0, -0): false
```

- Verdict: native. Every JavaScript number is an IEEE 754 binary64 value, and the language has no separate integer type for `number`.
- An integer above `Number.MAX_SAFE_INTEGER`, which is 2^53 - 1, may not have an exact representation. `2 ** 53 + 1` rounds to `2 ** 53`.
- `(1.005).toFixed(2)` gives `"1.00"`, because the stored value is a little below 1.005.
- `===` follows IEEE 754: `NaN` is not equal to itself, and `0` equals `-0`. `Object.is` tells them apart.
- A fixed comparison with `Number.EPSILON` works only near 1. For large numbers the gap between floats is much larger than `Number.EPSILON`.

An integer type with no size limit, `BigInt`, is exact for large integers.

```javascript run id=bigint
const big = 2n ** 53n + 1n;
console.log(big.toString());
console.log(Number(big));
```

```output
9007199254740993
9007199254740992
```
