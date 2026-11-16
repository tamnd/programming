```javascript run id=scaled
const SHIFT = 16n;
const ONE = 1n << SHIFT;
const toFix = (x) => BigInt(Math.round(x * Number(ONE)));
const mul = (a, b) => (a * b + (ONE >> 1n)) >> SHIFT;
const toNumber = (f) => Number(f) / Number(ONE);

console.log(`1.5 * 2.25: ${toNumber(mul(toFix(1.5), toFix(2.25)))}`);
const q = new Int32Array(1);
q[0] = Number(mul(toFix(300), toFix(300)));
console.log(`300 * 300 in an Int32Array: ${q[0] / 65536}`);

const show = (cents) => `${Math.trunc(cents / 100)}.${String(cents % 100).padStart(2, "0")}`;
console.log(`3 x 19.99: ${show(1999 * 3)}`);
console.log(`0.1 + 0.2 as numbers: ${0.1 + 0.2}`);
console.log(`10 + 20 cents: ${show(10 + 20)}`);
```

```output for=scaled
1.5 * 2.25: 3.375
300 * 300 in an Int32Array: 24464
3 x 19.99: 59.97
0.1 + 0.2 as numbers: 0.30000000000000004
10 + 20 cents: 0.30
```

- Verdict: idiom. JavaScript has no fixed-point type. The block keeps a Q16.16 value in a `bigint` and money as a whole number of cents.
- The store into an `Int32Array` keeps the low 32 bits, so 90000 wraps to 90000 - 65536 = 24464 with no error.
