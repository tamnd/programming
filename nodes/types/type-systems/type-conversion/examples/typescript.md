```typescript run id=assertion
interface Order {
  count: number;
}

function readCount(json: string): number {
  const raw: unknown = JSON.parse(json);
  if (typeof raw !== "object" || raw === null || !("count" in raw)) {
    throw new TypeError("no count");
  }
  const value = Number(raw.count);
  if (!Number.isInteger(value)) {
    throw new TypeError(`not an integer: ${String(raw.count)}`);
  }
  return value;
}

const order = JSON.parse('{"count": "42"}') as Order;
console.log(order.count + 1, typeof order.count);
console.log(readCount('{"count": "42"}') + 1);
try {
  readCount('{"count": "4x"}');
} catch (error) {
  console.log((error as Error).message);
}
```

```output for=assertion
421 string
43
not an integer: 4x
```

- Verdict: native. At run time TypeScript has the conversions of JavaScript, because the compiler removes the types.
- `x as T` is a type assertion, not a conversion. It changes what the compiler believes and does nothing at run time. In the first line `order.count` is still the string `"42"`, so `+ 1` concatenates.
- A real conversion is a function such as `Number` or `String`, followed by a check of the result.
