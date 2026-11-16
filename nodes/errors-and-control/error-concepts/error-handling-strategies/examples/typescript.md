```typescript run id=strategy-typescript
function divide(a: number, b: number): number {
  if (b === 0) {
    throw new RangeError("division by zero");
  }
  return Math.trunc(a / b);
}

for (const [a, b] of [[10, 2], [10, 0]]) {
  try {
    console.log(`${a} / ${b} = ${divide(a, b)}`);
  } catch (e) {
    const message = e instanceof Error ? e.message : String(e);
    console.log(`${a} / ${b}: error: ${message}`);
  }
}
```

```output for=strategy-typescript
10 / 2 = 5
10 / 0: error: division by zero
```

- Verdict: native. Strategy: exceptions, as in JavaScript. The type of `divide` does not show that it can throw. Some TypeScript code returns a union such as `{ ok: true, value: number } | { ok: false, error: string }` instead, so that the type shows the error.
