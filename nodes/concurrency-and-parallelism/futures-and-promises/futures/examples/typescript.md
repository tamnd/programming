```typescript run id=typed
function withTimeout<T>(work: Promise<T>, ms: number): Promise<T> {
  const timeout = new Promise<never>((_, reject) =>
    setTimeout(() => reject(new Error(`timed out after ${ms} ms`)), ms),
  );
  return Promise.race([work, timeout]);
}

const slow = new Promise<number>((resolve) => setTimeout(() => resolve(42), 50));
const fast = Promise.resolve(7);

console.log(await withTimeout(fast, 20));
try {
  await withTimeout(slow, 20);
} catch (e) {
  console.log((e as Error).message);
}
```

```output for=typed
7
timed out after 20 ms
```

- Verdict: native. `Promise<T>` carries the type of the value. A rejection has no type, so `catch` gets `unknown` in strict mode.
- `Promise.race` settles with the first input that settles. The slower promise keeps running, because a promise cannot be cancelled. An `AbortSignal` is the usual way to stop the work.
