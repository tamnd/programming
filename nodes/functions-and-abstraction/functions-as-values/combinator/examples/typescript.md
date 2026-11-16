```typescript run id=combinator-typescript
const identity = <T>(x: T): T => x;
const constant =
  <A>(x: A) =>
  (_: unknown): A =>
    x;
const flip =
  <A, B, C>(f: (a: A, b: B) => C) =>
  (b: B, a: A): C =>
    f(a, b);
const both =
  <T>(p: (x: T) => boolean, q: (x: T) => boolean) =>
  (x: T): boolean =>
    p(x) && q(x);

const sub = (a: number, b: number): number => a - b;
console.log(identity(5));
console.log(constant(7)(99));
console.log(flip(sub)(2, 10));
const keep = both<number>((x) => x % 2 === 0, (x) => x > 0);
console.log([-2, -1, 0, 1, 2, 3, 4].filter(keep).join(" "));
```

```output for=combinator-typescript
5
7
8
2 4
```

- Verdict: native. Generic types describe each combinator, so `flip(sub)` has the type `(b: number, a: number) => number`.
