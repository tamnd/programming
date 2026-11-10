```typescript run id=checker
// xorshift32 with Marsaglia's seed: the same numbers on every run.
class Rng {
  private state = 2463534242;

  next(): number {
    let y = this.state;
    y ^= y << 13;
    y ^= y >>> 17;
    y ^= y << 5;
    this.state = y >>> 0;
    return this.state;
  }

  between(lo: number, hi: number): number {
    return lo + (this.next() % (hi - lo + 1));
  }
}

type Property<T> = {
  name: string;
  generate: (rng: Rng) => T;
  holds: (value: T) => boolean;
  shrink: (value: T) => T[];
  show: (value: T) => string;
};

// Runs a property on 100 generated cases and shrinks the first failure.
function check<T>(p: Property<T>): void {
  const rng = new Rng();
  for (let trial = 1; trial <= 100; trial++) {
    let c = p.generate(rng);
    if (p.holds(c)) continue;
    console.log(`${p.name}: falsified on test ${trial} by ${p.show(c)}`);
    let steps = 0;
    for (let s = p.shrink(c).find((x) => !p.holds(x)); s !== undefined; s = p.shrink(c).find((x) => !p.holds(x))) {
      c = s;
      steps++;
    }
    console.log(`${p.name}: shrunk in ${steps} steps to ${p.show(c)}`);
    return;
  }
  console.log(`${p.name}: passed 100 tests`);
}

// Gives lo, then x - d for d = (x - lo) / 2, / 4, ..., 1.
function shrinkInt(x: number, lo: number): number[] {
  const out = x > lo ? [lo] : [];
  for (let d = Math.trunc((x - lo) / 2); d > 0; d = Math.trunc(d / 2)) {
    out.push(x - d);
  }
  return out;
}

function pages(items: number, perPage: number): number {
  return Math.trunc(items / perPage); // bug: drops the last page when it is not full
}

check<number>({
  name: "round trip",
  generate: (rng) => rng.between(-(2 ** 31), 2 ** 31 - 1),
  holds: (x) => Number.parseInt(String(x), 10) === x,
  shrink: () => [],
  show: String,
});

check<[items: number, perPage: number]>({
  name: "pages",
  generate: (rng) => [rng.between(0, 1000), rng.between(1, 50)],
  holds: ([items, perPage]) => pages(items, perPage) * perPage >= items,
  shrink: ([items, perPage]) => [
    ...shrinkInt(items, 0).map((n): [number, number] => [n, perPage]),
    ...shrinkInt(perPage, 1).map((p): [number, number] => [items, p]),
  ],
  show: ([items, perPage]) => `(${items}, ${perPage})`,
});
```

```output
round trip: passed 100 tests
pages: falsified on test 1 by (967, 7)
pages: shrunk in 13 steps to (1, 2)
```

- Verdict: library. fast-check is a property testing library for JavaScript and TypeScript.
- `Property<T>` ties the four functions to one type, so a shrinker that returns the wrong kind of value does not type-check. The pair is a labelled tuple `[items: number, perPage: number]`.
- Each call states its type argument, `check<number>` and `check<[items: number, perPage: number]>`. The type annotations are erased before the program runs, so the run behaves as the JavaScript example does.
- The arithmetic is the same as in JavaScript: `>>>` and `>>> 0` keep the state unsigned.
