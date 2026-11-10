```typescript test id=leap
import { test } from "node:test";
import assert from "node:assert/strict";

function isLeap(year: number): boolean {
  return year % 4 === 0 && (year % 100 !== 0 || year % 400 === 0);
}

// The tests in the order of the to-do list. Each one added one clause.
const cases: ReadonlyArray<readonly [year: number, leap: boolean]> = [
  [2024, true],
  [2023, false],
  [1900, false],
  [2000, true],
];

for (const [year, leap] of cases) {
  test(`${year} is ${leap ? "a leap" : "a common"} year`, () => {
    assert.equal(isLeap(year), leap);
  });
}
```

```output
✔ 2024 is a leap year ($TIME)
✔ 2023 is a common year ($TIME)
✔ 1900 is a common year ($TIME)
✔ 2000 is a leap year ($TIME)
ℹ tests 4
ℹ suites 0
ℹ pass 4
ℹ fail 0
ℹ cancelled 0
ℹ skipped 0
ℹ todo 0
ℹ duration_ms $TIME
```

- Verdict: stdlib. TypeScript uses the JavaScript runners. Here `node --test` runs the file. Node.js removes the type annotations before it runs the code, and it does not check them.
- The loop registers one test for each row of the table. Each item that the to-do list gains is one more row.
- The return type `boolean` is part of the test when `tsc` checks the code. It rejects a fake that returns `1`, because `number` is not assignable to `boolean`.

In a project that runs `tsc` before the tests, the first red in TypeScript is a compile error. The checker reports that it cannot find the name `isLeap`, and the tests do not run. This page runs TypeScript the way `node` does, with the types removed and not checked, so the same red shows up at run time as a `ReferenceError`.

```typescript fails expect=exception:ReferenceError id=red
import assert from "node:assert/strict";

// Red: the test comes first, so isLeap does not exist yet.
assert.equal(isLeap(2024), true);
```

- `tsc --noEmit` reports this file with error TS2304, Cannot find name 'isLeap'. A setup that only removes types, as Node.js does, skips that check. Run `tsc` in the cycle to get the red before the code runs.
- To get to a red that is a test failure, write a stub first: `function isLeap(year: number): boolean { return false; }`. Then the test compiles and fails on its check.
