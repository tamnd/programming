```typescript run id=cents
// Keep money as a whole number of cents, and format it only for output.
type Cents = number & { readonly __brand: "Cents" };

function cents(n: number): Cents {
  if (!Number.isSafeInteger(n)) throw new RangeError(`not a safe integer: ${n}`);
  return n as Cents;
}

function add(a: Cents, b: Cents): Cents {
  return cents(a + b);
}

function format(c: Cents): string {
  const sign = c < 0 ? "-" : "";
  const abs = Math.abs(c);
  return `${sign}${Math.trunc(abs / 100)}.${String(abs % 100).padStart(2, "0")}`;
}

let total = cents(0);
for (let i = 0; i < 10; i++) total = add(total, cents(10));
console.log("ten times 0.10 in cents:", format(total));

let floatTotal = 0;
for (let i = 0; i < 10; i++) floatTotal += 0.1;
console.log("ten times 0.1 as numbers:", floatTotal);

try {
  cents(0.1 * 3 * 100);
} catch (e) {
  console.log((e as Error).message);
}
```

```output
ten times 0.10 in cents: 1.00
ten times 0.1 as numbers: 0.9999999999999999
not a safe integer: 30.000000000000004
```

- Verdict: native. TypeScript uses the `number` of JavaScript, which is IEEE 754 binary64. The type `bigint` holds integers of any size.
- The type checker does not know whether a `number` holds an integer. A branded type such as `Cents` marks the values that the code has checked.
- `0.1 * 3 * 100` is not exactly 30, so the check rejects it.
