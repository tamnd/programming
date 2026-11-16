```typescript run id=infinite
function* naturalsFrom(n: number): Generator<number> {
  for (;;) {
    yield n;
    n += 1;
  }
}

function* iterate<T>(f: (x: T) => T, x: T): Generator<T> {
  for (;;) {
    yield x;
    x = f(x);
  }
}

function* filter<T>(items: Iterable<T>, test: (x: T) => boolean): Generator<T> {
  for (const item of items) {
    if (test(item)) yield item;
  }
}

function take<T>(items: Iterable<T>, count: number): T[] {
  const result: T[] = [];
  if (count <= 0) return result;
  for (const item of items) {
    result.push(item);
    if (result.length === count) break;
  }
  return result;
}

function isPrime(n: number): boolean {
  for (let d = 2; d * d <= n; d += 1) {
    if (n % d === 0) return false;
  }
  return true;
}

console.log(take(filter(naturalsFrom(2), isPrime), 10).join(" "));
console.log(take(iterate((x: number) => 2 * x, 1), 8).join(" "));
```

```output for=infinite
2 3 5 7 11 13 17 19 23 29
1 2 4 8 16 32 64 128
```

- Verdict: native. The generators are the same as in JavaScript, and the type `Generator<number>` records that they produce numbers. Nothing in the type says that a generator is infinite.
- `take` checks the count before it asks for the next element. If it checked after, it would ask the infinite source for one element too many, which matters when an element is costly.
