```typescript run id=typed
function* fibonacci(): Generator<number, never, unknown> {
  let [a, b] = [0, 1];
  while (true) {
    yield a;
    [a, b] = [b, a + b];
  }
}

function firstWhere<T>(items: Iterable<T>, test: (item: T) => boolean): T | undefined {
  for (const item of items) {
    if (test(item)) return item;
  }
  return undefined;
}

console.log(firstWhere(fibonacci(), (n) => n > 100));
```

```output for=typed
144
```

- Verdict: native. As JavaScript. The type `Generator<T, TReturn, TNext>` describes the values that a generator yields and returns.
