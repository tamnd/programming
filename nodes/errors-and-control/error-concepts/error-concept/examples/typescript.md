```typescript run id=error-typescript
function parseInteger(text: string): number {
  const n = Number(text);
  if (!Number.isInteger(n)) {
    throw new Error(`not an integer: ${text}`);
  }
  return n;
}

for (const text of ["42", "x42"]) {
  try {
    console.log(`${text} -> ${parseInteger(text)}`);
  } catch (e) {
    const name = e instanceof Error ? e.name : "unknown";
    console.log(`${text} -> error (${name})`);
  }
}
```

```output for=error-typescript
42 -> 42
x42 -> error (Error)
```

- Verdict: native. TypeScript has the exceptions of JavaScript. A program can throw any value, not only an `Error`, so the code tests the caught value with `instanceof Error` before it reads `name`. The type of a function does not say which errors it throws.
