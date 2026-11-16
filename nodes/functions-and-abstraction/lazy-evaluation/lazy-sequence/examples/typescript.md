```typescript run id=pipeline
function* naturals(): Generator<number> {
  for (let n = 1; ; n++) yield n;
}

function* map<T, U>(items: Iterable<T>, f: (item: T) => U): Generator<U> {
  for (const item of items) yield f(item);
}

function* filter<T>(items: Iterable<T>, test: (item: T) => boolean): Generator<T> {
  for (const item of items) if (test(item)) yield item;
}

function* take<T>(items: Iterable<T>, count: number): Generator<T> {
  if (count <= 0) return;
  for (const item of items) {
    yield item;
    if (--count === 0) return;
  }
}

const squares = map(naturals(), (n) => {
  console.log("square", n);
  return n * n;
});
console.log([...take(filter(squares, (s) => s % 2 === 1), 2)]);
```

```output for=pipeline
square 1
square 2
square 3
[ 1, 9 ]
```

- Verdict: native. The evaluation rules are those of JavaScript. Generic generator functions give typed lazy stages: each one is a `Generator<T>` that pulls from the stage before it.
- `take` returns right after the last element that it needs, so it does not pull one element too many from the stage before it.
