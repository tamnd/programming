```typescript run id=countdown
class Countdown implements Iterable<number> {
  readonly start: number;

  constructor(start: number) {
    this.start = start;
  }

  [Symbol.iterator](): Iterator<number> {
    let current = this.start;
    return {
      next(): IteratorResult<number> {
        if (current <= 0) {
          return { done: true, value: undefined };
        }
        return { done: false, value: current-- };
      },
    };
  }
}

for (const n of new Countdown(3)) {
  console.log(n);
}

const tens: number[] = Iterator.from(new Countdown(5))
  .filter((n) => n % 2 === 1)
  .map((n) => n * 10)
  .toArray();
console.log(tens);
```

```output for=countdown
3
2
1
[ 50, 30, 10 ]
```

- Verdict: native. TypeScript uses the JavaScript protocol and adds the types `Iterable<T>`, `Iterator<T>` and `IteratorResult<T>`.
- `Iterator.from` wraps an iterable so that the iterator helper methods (`filter`, `map`, `take`, `toArray`) are available. The helpers are lazy: each one pulls one value at a time from the one before it.
- The type checker checks only the shapes. It does not check that `next()` keeps returning `done: true` after the end.
