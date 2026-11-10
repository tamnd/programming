```javascript test id=leap
import { test } from "node:test";
import assert from "node:assert/strict";

function isLeap(year) {
  return year % 4 === 0 && (year % 100 !== 0 || year % 400 === 0);
}

// The tests in the order of the to-do list. Each one added one clause.
test("a year divisible by 4 is a leap year", () => {
  assert.equal(isLeap(2024), true);
});

test("another year is a common year", () => {
  assert.equal(isLeap(2023), false);
});

test("a century is a common year", () => {
  assert.equal(isLeap(1900), false);
});

test("a century divisible by 400 is a leap year", () => {
  assert.equal(isLeap(2000), true);
});
```

```output
✔ a year divisible by 4 is a leap year ($TIME)
✔ another year is a common year ($TIME)
✔ a century is a common year ($TIME)
✔ a century divisible by 400 is a leap year ($TIME)
ℹ tests 4
ℹ suites 0
ℹ pass 4
ℹ fail 0
ℹ cancelled 0
ℹ skipped 0
ℹ todo 0
ℹ duration_ms $TIME
```

- Verdict: stdlib. Node.js has the test runner `node:test` and the module `node:assert` in its runtime. The ECMAScript standard has no test runner.
- The runner runs the tests in the order of the calls to `test`, so the report follows the to-do list.
- `assert.equal` from `node:assert/strict` compares with `Object.is`. A fake that returns `1` in place of `true` fails the test.

The first red in JavaScript happens at run time. The module loads, and the call to `isLeap` throws a `ReferenceError` because no such name exists.

```javascript fails expect=exception:ReferenceError id=red
import assert from "node:assert/strict";

// Red: the test comes first, so isLeap does not exist yet.
assert.equal(isLeap(2024), true);
```

- No tool stops this program before it runs. A linter can report the undefined name, but the language itself does not. TypeScript reports it at compile time; see the TypeScript example.
