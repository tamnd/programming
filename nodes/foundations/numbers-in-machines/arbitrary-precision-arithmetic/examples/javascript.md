```javascript run id=bigint
console.log(`2n ** 64n: ${2n ** 64n}`);
console.log(`2n ** 100n: ${2n ** 100n}`);
let f = 1n;
for (let i = 1n; i <= 30n; i++) {
  f *= i;
}
console.log(`30!: ${f}`);
console.log(`-7n / 2n: ${-7n / 2n}`);
console.log(`MAX_SAFE_INTEGER + 2: ${Number.MAX_SAFE_INTEGER + 2}`);
console.log(`BigInt(MAX_SAFE_INTEGER) + 2n: ${BigInt(Number.MAX_SAFE_INTEGER) + 2n}`);
console.log(`Number(2n ** 64n + 1n): ${Number(2n ** 64n + 1n)}`);
try {
  const one = 1;
  console.log(1n + one);
} catch (e) {
  console.log(`1n + 1: ${e.name}`);
}
try {
  console.log(JSON.stringify({ n: 1n }));
} catch (e) {
  console.log(`JSON.stringify({ n: 1n }): ${e.name}`);
}
console.log(`JSON with a string: ${JSON.stringify({ n: (2n ** 64n).toString() })}`);
```

```output for=bigint
2n ** 64n: 18446744073709551616
2n ** 100n: 1267650600228229401496703205376
30!: 265252859812191058636308480000000
-7n / 2n: -3
MAX_SAFE_INTEGER + 2: 9007199254740992
BigInt(MAX_SAFE_INTEGER) + 2n: 9007199254740993
Number(2n ** 64n + 1n): 18446744073709552000
1n + 1: TypeError
JSON.stringify({ n: 1n }): TypeError
JSON with a string: {"n":"18446744073709551616"}
```

- Verdict: native. A `bigint` literal ends in `n`, and the usual operators work on two `bigint` values.
- A `number` above `Number.MAX_SAFE_INTEGER` loses digits, but the `bigint` result is exact. `Number()` on a large `bigint` rounds it.
- `/` on `bigint` values truncates toward zero. A mix of `bigint` and `number` in one operation throws a `TypeError`, and so does `JSON.stringify`.
