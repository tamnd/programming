```javascript run id=checker
// xorshift32 with Marsaglia's seed: the same numbers on every run.
class Rng {
  #state = 2463534242;

  next() {
    let y = this.#state;
    y ^= y << 13; // the bit operators work on 32-bit signed integers
    y ^= y >>> 17;
    y ^= y << 5;
    this.#state = y >>> 0; // back to an unsigned value
    return this.#state;
  }

  between(lo, hi) {
    return lo + (this.next() % (hi - lo + 1));
  }
}

// Runs a property on 100 generated cases and shrinks the first failure.
function check(name, generate, holds, shrink) {
  const rng = new Rng();
  for (let trial = 1; trial <= 100; trial++) {
    let c = generate(rng);
    if (holds(c)) continue;
    console.log(`${name}: falsified on test ${trial} by (${c.join(", ")})`);
    let steps = 0;
    for (let smaller; (smaller = shrink(c).find((s) => !holds(s))) !== undefined; steps++) {
      c = smaller;
    }
    console.log(`${name}: shrunk in ${steps} steps to (${c.join(", ")})`);
    return;
  }
  console.log(`${name}: passed 100 tests`);
}

// Gives lo, then x - d for d = (x - lo) / 2, / 4, ..., 1.
function shrinkInt(x, lo) {
  const out = x > lo ? [lo] : [];
  for (let d = Math.trunc((x - lo) / 2); d > 0; d = Math.trunc(d / 2)) {
    out.push(x - d);
  }
  return out;
}

function pages(items, perPage) {
  return Math.trunc(items / perPage); // bug: drops the last page when it is not full
}

check(
  "round trip",
  (rng) => [rng.between(-(2 ** 31), 2 ** 31 - 1)],
  ([x]) => Number.parseInt(String(x), 10) === x,
  () => [],
);

check(
  "pages",
  (rng) => [rng.between(0, 1000), rng.between(1, 50)],
  ([items, perPage]) => pages(items, perPage) * perPage >= items,
  ([items, perPage]) => [
    ...shrinkInt(items, 0).map((n) => [n, perPage]),
    ...shrinkInt(perPage, 1).map((p) => [items, p]),
  ],
);
```

```output
round trip: passed 100 tests
pages: falsified on test 1 by (967, 7)
pages: shrunk in 13 steps to (1, 2)
```

- Verdict: library. fast-check is a property testing library for JavaScript and TypeScript.
- JavaScript numbers are 64-bit floats, and the bit operators convert them to 32-bit integers. `<<` and `^` give a signed result, so `>>>` is the only shift that treats the state as unsigned, and `>>> 0` turns the result back into a value from 0 to 2^32 - 1.
- `check` takes four functions: a generator, the property, a shrinker and a printer. Closures over `per_page` and `items` build the shrink candidates for a pair.
- `Math.trunc` gives integer division for non-negative numbers. The round trip works on the full 32-bit range because a double holds every 32-bit integer exactly.

The classic mistake in a port of xorshift32 to JavaScript is the signed shift `>>` and a state that stays signed. The remainder of a negative number is negative, so the generator for page sizes gives values below 1.

```javascript wrong id=signed-shift
// The same xorshift32, but with >> and no >>> 0 at the end.
let state = 2463534242;
function next() {
  state ^= state << 13; // ^ turns state into a signed 32-bit integer
  state ^= state >> 17; // >> copies the sign bit
  state ^= state << 5;
  return state;
}

const perPage = [];
for (let i = 0; i < 8; i++) {
  next(); // skip the value for items
  perPage.push(1 + (next() % 50));
}
console.log(perPage.join(" "));
```

```output for=signed-shift
-37 38 -26 8 26 49 -36 -6
```

With a page size of -37, the property divides by a negative number, and every result after it means nothing. Write `>>>` for the right shift and end with `>>> 0`, as in the checker above.
