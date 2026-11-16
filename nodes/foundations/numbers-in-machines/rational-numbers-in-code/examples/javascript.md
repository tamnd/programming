```javascript run id=pair
function gcd(a, b) {
  a = a < 0n ? -a : a;
  b = b < 0n ? -b : b;
  while (b !== 0n) {
    [a, b] = [b, a % b];
  }
  return a;
}

function rat(n, d) {
  if (d === 0n) {
    throw new RangeError("denominator is 0");
  }
  if (d < 0n) {
    n = -n;
    d = -d;
  }
  const g = gcd(n, d);
  return { n: n / g, d: d / g };
}

const add = (a, b) => rat(a.n * b.d + b.n * a.d, a.d * b.d);
const show = (r) => `${r.n}/${r.d}`;

console.log(`rat(6n, -8n): ${show(rat(6n, -8n))}`);
const s = add(rat(1n, 10n), rat(2n, 10n));
console.log(`1/10 + 2/10: ${show(s)}`);
console.log(`0.1 + 0.2: ${0.1 + 0.2}`);
let h = rat(0n, 1n);
for (let k = 1n; k <= 20n; k++) {
  h = add(h, rat(1n, k));
}
console.log(`1/1 + ... + 1/20: ${show(h)}`);
console.log(`as a number: ${Number(h.n) / Number(h.d)}`);
try {
  rat(1n, 0n);
} catch (e) {
  console.log(`rat(1n, 0n): ${e.name}`);
}
```

```output for=pair
rat(6n, -8n): -3/4
1/10 + 2/10: 3/10
0.1 + 0.2: 0.30000000000000004
1/1 + ... + 1/20: 55835135/15519504
as a number: 3.597739657143682
rat(1n, 0n): RangeError
```

- Verdict: idiom. JavaScript has no rational type, so the block keeps a pair of `bigint` values in lowest terms.
- `bigint` parts do not overflow, and `/` on `bigint` values is exact here, because `g` divides both parts.
