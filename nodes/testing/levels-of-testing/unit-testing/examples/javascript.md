```javascript run id=cart
import { test } from "node:test";
import assert from "node:assert/strict";

function cartTotal(items) {
  let total = 0;
  for (const [price, quantity] of items) {
    if (quantity < 0) {
      throw new RangeError(`negative quantity: ${quantity}`);
    }
    total += price * quantity;
  }
  return total;
}

test("empty cart is zero", () => {
  assert.equal(cartTotal([]), 0);
});

test("sums price times quantity", () => {
  assert.equal(cartTotal([[250, 2], [100, 1]]), 600);
});

test("rejects negative quantity", () => {
  assert.throws(() => cartTotal([[250, -1]]), RangeError);
});
```

```output for=cart match=regex
✔ empty cart is zero \(\d+\.\d+ms\)
✔ sums price times quantity \(\d+\.\d+ms\)
✔ rejects negative quantity \(\d+\.\d+ms\)
ℹ tests 3
ℹ suites 0
ℹ pass 3
ℹ fail 0
ℹ cancelled 0
ℹ skipped 0
ℹ todo 0
ℹ duration_ms \d+\.\d+
```

- Verdict: stdlib. Node.js has the test runner `node:test` and the assertion module `node:assert` in its standard library. Run the file with `node`, or run `node --test` to find and run all test files.
- Each call to `test` registers one test with a name and a function. The runner reports each test with its duration, then the totals.
- In the strict mode of `node:assert`, `assert.equal` compares with `Object.is`, so `assert.equal("1", 1)` fails.
- The ECMAScript standard has no test runner, and browsers have none. Many projects use a test library such as Jest, Vitest or Mocha.
