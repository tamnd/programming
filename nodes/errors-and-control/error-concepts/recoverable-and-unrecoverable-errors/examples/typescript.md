```typescript run id=recoverable-typescript
function portOrDefault(text: string, fallback: number): [number, string] {
  // A bad fallback is a bug in the caller: stop.
  if (!(fallback >= 1 && fallback <= 65535)) {
    throw new RangeError("fallback out of range");
  }
  // Bad text is an expected error: recover.
  const n = Number(text);
  if (!Number.isInteger(n)) {
    return [fallback, " (not a number)"];
  }
  return [n, ""];
}

for (const text of ["8080", "x"]) {
  const [port, note] = portOrDefault(text, 80);
  console.log(`${text} -> ${port}${note}`);
}
```

```output for=recoverable-typescript
8080 -> 8080
x -> 80 (not a number)
```

- Verdict: absent. TypeScript has the exceptions of JavaScript and no second mechanism. The return type `[number, string]` shows the expected error, and the `throw` for the bug does not show in the type.
