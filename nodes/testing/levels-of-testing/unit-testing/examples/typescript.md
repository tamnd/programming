```typescript test id=cart
import { test } from "node:test";
import assert from "node:assert/strict";

type Item = readonly [price: number, quantity: number];

function cartTotal(items: readonly Item[]): number {
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

```output
✔ empty cart is zero ($TIME)
✔ sums price times quantity ($TIME)
✔ rejects negative quantity ($TIME)
ℹ tests 3
ℹ suites 0
ℹ pass 3
ℹ fail 0
ℹ cancelled 0
ℹ skipped 0
ℹ todo 0
ℹ duration_ms $TIME
```

- Verdict: stdlib. TypeScript has no test runner of its own. It uses the runners of JavaScript, and the type checker adds types to the tests: here `node:test` and `node:assert`, run by Node.js after it strips the types.
- The type checker catches some mistakes before the tests run. A call such as `cartTotal([[250]])` is a type error, so no test is needed for it.
- The type checker does not check values. A negative quantity is a valid `number`, so the range check still needs a test.
