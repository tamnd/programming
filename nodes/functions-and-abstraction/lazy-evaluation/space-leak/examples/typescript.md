```typescript wrong id=lazy-fold
function lazyFold<T, A>(step: (a: A, x: T) => A, start: A, items: Iterable<T>): () => A {
  let total: () => A = () => start;
  for (const item of items) {
    const before = total;
    total = () => step(before(), item);
  }
  return total;
}

const numbers = Array.from({ length: 1_000_000 }, (_, i) => i);
const total = lazyFold((a: number, b: number) => a + b, 0, numbers);
try {
  console.log(total());
} catch (error) {
  console.log("error:", (error as Error).constructor.name);
}
```

```output for=lazy-fold
error: RangeError
```

```typescript run id=strict-fold
function strictFold<T, A>(step: (a: A, x: T) => A, start: A, items: Iterable<T>): A {
  let total = start;
  for (const item of items) {
    total = step(total, item);
  }
  return total;
}

const numbers = Array.from({ length: 1_000_000 }, (_, i) => i);
console.log(strictFold((a: number, b: number) => a + b, 0, numbers));
```

```output for=strict-fold
499999500000
```

- The types do not show the leak: `lazyFold` returns `() => A`, a thunk, and the chain behind it has a million links. A return type of `A` instead of `() => A` forces the strict version.
- The run-time behavior is the same as in JavaScript.
