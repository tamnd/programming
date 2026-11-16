```javascript wrong id=lazy-fold
function lazyFold(step, start, items) {
  let total = () => start;
  for (const item of items) {
    const before = total;
    total = () => step(before(), item);
  }
  return total;
}

const numbers = Array.from({ length: 1_000_000 }, (_, i) => i);
const total = lazyFold((a, b) => a + b, 0, numbers);
try {
  console.log(total());
} catch (error) {
  console.log("error:", error.constructor.name);
}
```

```output for=lazy-fold
error: RangeError
```

```javascript run id=strict-fold
function strictFold(step, start, items) {
  let total = start;
  for (const item of items) {
    total = step(total, item);
  }
  return total;
}

const numbers = Array.from({ length: 1_000_000 }, (_, i) => i);
console.log(strictFold((a, b) => a + b, 0, numbers));
```

```output for=strict-fold
499999500000
```

- `lazyFold` keeps a million closures alive until the call `total()`, and each closure keeps the one before it. The force then needs a million nested calls, and the engine stops with a `RangeError`.
- `strictFold` keeps one number. The array method `reduce` works in the same way.
