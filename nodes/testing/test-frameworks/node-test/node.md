---
tag: 9E783P
kind: tool
slug: node-test
name: "node:test"
aliases: [Node.js test runner, node --test]
area: testing/test-frameworks
level: core
summary: >-
  node:test is the test runner that is built into Node.js. This page shows test, describe and it, subtests, skip and todo, mocks and mock timers, snapshots, coverage and the reporters.
role: [test-framework]
licence: mit
repo: https://github.com/nodejs/node
homepage: https://nodejs.org/api/test.html
see_also: [test-framework, jest, vitest, mocha-and-chai, javascript, typescript, unit-testing, test-double]
sources:
  - url: https://nodejs.org/api/test.html
    title: Node.js v26.13.1 documentation, Test runner
    locator: Test runner; Subtests; describe() and it() aliases; Skipping tests; TODO tests; Running tests from the command line; Test runner execution model; Collecting code coverage; Mocking; Timers; Snapshot testing; Test reporters; before(); beforeEach(); timers.enable()
    accessed: 2026-11-15
  - url: https://nodejs.org/api/assert.html
    title: Node.js v26.13.1 documentation, Assert
    locator: Strict assertion mode
    accessed: 2026-11-15
  - url: https://github.com/nodejs/node/blob/main/LICENSE
    title: Node.js, LICENSE
    locator: the first paragraph
    accessed: 2026-11-15
  - url: https://nodejs.org/en/about/previous-releases
    title: Node.js, Node.js Releases
    locator: Release Schedule
    accessed: 2026-11-15
  - url: https://nodejs.org/api/cli.html
    title: Node.js v26.13.1 documentation, Command-line API
    locator: --config-file; --experimental-test-module-mocks; --test; --test-concurrency; --test-coverage-lines; --test-isolation; --test-only; --test-shard; --test-update-snapshots
    accessed: 2026-11-15
  - url: https://nodejs.org/en/learn/test-runner/mocking
    title: Node.js Learn, Mocking in tests
    locator: When and not to mock
    accessed: 2026-11-15
  - url: https://nodejs.org/api/typescript.html
    title: Node.js v26.13.1 documentation, Modules, TypeScript
    locator: Type stripping
    accessed: 2026-11-15
  - url: https://nodejs.org/en/learn/userland-migrations/mocha-to-node-test-runner
    title: Node.js Learn, Mocha to Node.js Test Runner
    locator: the introduction; done callbacks
    accessed: 2026-11-15
---

## What it is

node:test is the [[test-framework]] that comes with Node.js. Its documentation says: "The node:test module facilitates the creation of JavaScript tests."[^1] It needs no package from npm. The command `node --test` finds the test files and runs them, and the module `node:assert` gives the assertions.[^1][^2] Node.js is under the MIT licence.[^3]

A test is one function. The runner treats it in one of three ways:[^1]

- a synchronous function fails when it throws an exception, and passes when it does not;
- a function that returns a Promise fails when the Promise rejects;
- a function that takes a callback fails when the callback gets a truthy first argument.

"If any tests fail, the process exit code is set to 1 ."[^1] The module is "only available under the node: scheme", so the import is `node:test` and not `test`.[^1]

The test runner was added in Node.js 18.0.0 and 16.17.0, and it became stable in 20.0.0.[^1] In November 2026, the release lines with long-term support (LTS) are 26, 24 and 22.[^4] The examples on this page run with Node.js 24.

## Install and first use

There is nothing to install. Any Node.js with the test runner can run a test file:[^1]

```text
node --version
node --test
```

With no file names, `node --test` runs all the files that match patterns such as `**/*.test.{cjs,mjs,js}`, `**/*_test.{cjs,mjs,js}` and `**/test/**/*.{cjs,mjs,js}`. Unless the option `--no-strip-types` is given, it also runs TypeScript files such as `**/*.test.ts`.[^1] The usual entry in `package.json` is a script, so that `npm test` runs the tests:

```text
{
  "type": "module",
  "scripts": {
    "test": "node --test"
  }
}
```

A test file imports `test`, or the spec style names `describe` and `it`, from `node:test`. The documentation says that "describe() is an alias for suite() , and it() is an alias for test() ".[^1] This file has one top-level test and a suite with two tests. The hook `beforeEach` makes a new cart before each test of the suite:

```javascript test id=first-use
import { test, describe, it, beforeEach } from "node:test";
import assert from "node:assert/strict";

class Cart {
  lines = [];

  add(name, price, quantity = 1) {
    this.lines.push({ name, price, quantity });
  }

  total() {
    return this.lines.reduce((sum, line) => sum + line.price * line.quantity, 0);
  }
}

test("a new cart costs nothing", () => {
  assert.equal(new Cart().total(), 0);
});

describe("Cart", () => {
  let cart;

  beforeEach(() => {
    cart = new Cart();
  });

  it("multiplies the price by the quantity", () => {
    cart.add("tea", 450, 3);
    assert.equal(cart.total(), 1350);
  });

  it("adds the lines", () => {
    cart.add("tea", 450);
    cart.add("cup", 1200, 2);
    assert.equal(cart.total(), 2850);
  });
});
```

```output
✔ a new cart costs nothing ($TIME)
▶ Cart
  ✔ multiplies the price by the quantity ($TIME)
  ✔ adds the lines ($TIME)
✔ Cart ($TIME)
ℹ tests 3
ℹ suites 1
ℹ pass 3
ℹ fail 0
ℹ cancelled 0
ℹ skipped 0
ℹ todo 0
ℹ duration_ms $TIME
```

The spec reporter prints a tree of the suites and the tests, and then the totals. The examples on this page run with `node --test --test-reporter=spec main.test.js`.

## Concepts of the tool

### Assertions

node:test has no assertion functions of its own. Tests use the module `node:assert`, and most tests import `node:assert/strict`. In the strict assertion mode, `assert.deepEqual()` behaves like `assert.deepStrictEqual()`, and "error messages for objects display a diff".[^2] In this file, the code gives free shipping only above the limit, but the rule says from the limit. The second test fails:

```javascript test fails expect=test-failure id=failure
import { test } from "node:test";
import assert from "node:assert/strict";

const FREE_FROM = 5000;

function receipt(subtotal) {
  // The rule is "free from 5000", but the code uses > and not >=.
  const shipping = subtotal > FREE_FROM ? 0 : 495;
  return { subtotal, shipping, total: subtotal + shipping };
}

test("shipping costs 495 below the limit", () => {
  assert.deepEqual(receipt(4999), { subtotal: 4999, shipping: 495, total: 5494 });
});

test("shipping is free at the limit", () => {
  assert.deepEqual(receipt(5000), { subtotal: 5000, shipping: 0, total: 5000 });
});
```

```output
✔ shipping costs 495 below the limit ($TIME)
✖ shipping is free at the limit ($TIME)
ℹ tests 2
ℹ suites 0
ℹ pass 1
ℹ fail 1
ℹ cancelled 0
ℹ skipped 0
ℹ todo 0
ℹ duration_ms $TIME

✖ failing tests:

test at main.test.js:16:1
✖ shipping is free at the limit ($TIME)
  AssertionError [ERR_ASSERTION]: Expected values to be strictly deep-equal:
  + actual - expected

    {
  +   shipping: 495,
  -   shipping: 0,
      subtotal: 5000,
  +   total: 5495
  -   total: 5000
    }

      at TestContext.<anonymous> (file://$WORK/main.test.js:17:10) {
    generatedMessage: true,
    code: 'ERR_ASSERTION',
    actual: { subtotal: 5000, shipping: 495, total: 5495 },
    expected: { subtotal: 5000, shipping: 0, total: 5000 },
    operator: 'deepStrictEqual',
    diff: 'simple'
  }
```

The report gives the place of the failed test, the diff of the two objects, and the stack. Each line with `+` is from the actual value, and each line with `-` is from the expected value. The process ends with the exit code 1.

### Subtests, asynchronous tests, skip and todo

A test function gets a test context, usually named `t`. The method `t.test()` makes a subtest. The parent must `await` its subtests, "because tests do not wait for their subtests to complete, unlike tests created within suites."[^1] An `async` test fails when its Promise rejects, and `assert.rejects` checks that a Promise rejects with the expected error.[^1][^2]

The option `skip` makes the runner skip a test, and a string gives the reason. The option `todo` marks a test that is not finished. The documentation says: "TODO tests are executed, but are not treated as test failures, and therefore do not affect the process exit code."[^1]

```javascript test id=subtests
import { test } from "node:test";
import assert from "node:assert/strict";

const prices = new Map([["tea", 450], ["cup", 1200]]);

async function priceOf(name) {
  await new Promise((resolve) => setImmediate(resolve));
  if (!prices.has(name)) {
    throw new RangeError(`no price for ${name}`);
  }
  return prices.get(name);
}

test("price lookup", async (t) => {
  await t.test("finds a known product", async () => {
    assert.equal(await priceOf("tea"), 450);
  });

  await t.test("rejects an unknown product", async () => {
    await assert.rejects(priceOf("cake"), {
      name: "RangeError",
      message: "no price for cake",
    });
  });
});

test("prices in euros", { skip: "the currency service is not ready" }, () => {
  assert.fail("this code does not run");
});

test("price with tax", { todo: "the tax rules are not decided" }, async () => {
  assert.equal(await priceOf("tea"), 540);
});
```

```output
▶ price lookup
  ✔ finds a known product ($TIME)
  ✔ rejects an unknown product ($TIME)
✔ price lookup ($TIME)
﹣ prices in euros ($TIME) # the currency service is not ready
✖ price with tax ($TIME) # the tax rules are not decided
ℹ tests 5
ℹ suites 0
ℹ pass 3
ℹ fail 0
ℹ cancelled 0
ℹ skipped 1
ℹ todo 1
ℹ duration_ms $TIME

✖ failing tests:

test at main.test.js:31:1
✖ price with tax ($TIME) # the tax rules are not decided
  AssertionError [ERR_ASSERTION]: Expected values to be strictly equal:

  450 !== 540

      at TestContext.<anonymous> (file://$WORK/main.test.js:32:10) {
    generatedMessage: true,
    code: 'ERR_ASSERTION',
    actual: 450,
    expected: 540,
    operator: 'strictEqual',
    diff: 'simple'
  }
```

The todo test ran and its assertion failed. The report lists the failure, but the count of failed tests is 0, so the run passes. The skipped test did not run, so `assert.fail` was not called.

### Hooks

The hooks are `before`, `after`, `beforeEach` and `afterEach`. `before` "creates a hook that runs before executing a suite", and `beforeEach` "creates a hook that runs before each test in the current suite".[^1] The `after` hook "is guaranteed to run, even if tests within the suite fail."[^1] A hook can be `async`, and it can have a `timeout`.[^1]

### Mocks and mock timers

The object `mock`, and the property `t.mock` of each test context, make test doubles. `mock.fn()` makes a spy function, and `mock.method()` replaces a method of an object and records its calls.[^1] The documentation gives the reason to use `t.mock`: "the test runner will automatically restore all mocked functionality once the test finishes."[^1]

`mock.timers` replaces `setTimeout`, `setInterval`, `setImmediate` and `Date`, so that a test can move the time forward with `tick()` and not wait.[^1] In this file, the first test replaces the method that sends mail, and the second test moves the clock by one day:

```javascript test id=mocks
import { test } from "node:test";
import assert from "node:assert/strict";

const DAY = 24 * 60 * 60 * 1000;

class Mailer {
  send(to, text) {
    throw new Error(`a test must not send mail to ${to}: ${text}`);
  }
}

function remindAll(mailer, customers) {
  for (const customer of customers) {
    if (customer.owes > 0) {
      mailer.send(customer.name, `You owe ${customer.owes} cents.`);
    }
  }
}

function remindTomorrow(send) {
  setTimeout(send, DAY);
}

test("reminds only the customers who owe money", (t) => {
  const mailer = new Mailer();
  t.mock.method(mailer, "send", () => {});

  remindAll(mailer, [
    { name: "Ana", owes: 1200 },
    { name: "Bao", owes: 0 },
  ]);

  assert.equal(mailer.send.mock.callCount(), 1);
  assert.deepEqual(mailer.send.mock.calls[0].arguments, ["Ana", "You owe 1200 cents."]);
});

test("sends the reminder after one day", (t) => {
  t.mock.timers.enable({ apis: ["setTimeout"] });
  const send = t.mock.fn();

  remindTomorrow(send);
  t.mock.timers.tick(DAY - 1);
  assert.equal(send.mock.callCount(), 0);
  t.mock.timers.tick(1);
  assert.equal(send.mock.callCount(), 1);
});
```

```output
✔ reminds only the customers who owe money ($TIME)
✔ sends the reminder after one day ($TIME)
ℹ tests 2
ℹ suites 0
ℹ pass 2
ℹ fail 0
ℹ cancelled 0
ℹ skipped 0
ℹ todo 0
ℹ duration_ms $TIME
```

Each call in `mock.calls` has the `arguments`, the `result`, the `error` and the `this` of the call.[^1] The method `mock.module()` replaces a whole module, but it is in early development and needs the option `--experimental-test-module-mocks`.[^1][^5] The Node.js guide on mocking warns that mocking code of the project itself "can be more trouble than worth, especially when foo is simple, well-tested, and rarely updated."[^6] See [[test-double]] for the kinds of test doubles.

### Snapshots

A snapshot assertion compares a value with a value that an earlier run saved in a file. The documentation says: "Snapshot files are generated by starting Node.js with the --test-update-snapshots command-line flag." It also says that the first run of a snapshot test "will fail because the corresponding snapshot file does not exist."[^1] Snapshot testing was added in 22.3.0 and was no longer experimental from 23.4.0.[^1] This test file was run on Node.js 24.10.0, first with the option and then without it:

```text
import { test } from "node:test";

function receiptLines(cart) {
  return cart.map((line) => `${line.quantity} x ${line.name}: ${line.price * line.quantity}`);
}

test("receipt lines", (t) => {
  t.assert.snapshot(receiptLines([
    { name: "tea", price: 450, quantity: 3 },
    { name: "cup", price: 1200, quantity: 1 },
  ]));
});
```

```text
node --test --test-update-snapshots main.test.js
node --test main.test.js
```

The first command wrote the file `main.test.js.snapshot` next to the test file, and both commands reported `ℹ pass 1`. This is the file:

```text
exports[`receipt lines 1`] = `
[
  "3 x tea: 1350",
  "1 x cup: 1200"
]
`;
```

Each snapshot is an export whose name is the full name of the test and a counter.[^1] By default, the runner makes the string with `JSON.stringify(value, null, 2)`.[^1] The snapshot file goes into version control with the test file.[^1] A change of the output then fails the test, and a person checks the change before running with `--test-update-snapshots` again.

### Test files and processes

"When process-level test isolation is enabled, each matching test file is executed in a separate child process."[^1] This is the default: the default isolation mode is `process`.[^5] The tests of one file run in one thread of that process.[^1] The option `--test-concurrency` sets how many test files run at the same time, and its default is `os.availableParallelism() - 1`.[^5] With `--test-isolation=none`, all the test files run in the process of the runner, so tests of two files can change the same global state.[^1][^5]

## Common tasks

Run only the tests whose name matches a regular expression. With the file of the first example, this command runs the test `adds the lines` and its suite:[^1]

```text
node --test --test-name-pattern="adds" main.test.js
```

Skip the tests whose name matches a pattern with `--test-skip-pattern`.[^1] Run only the tests that have the option `only`, or that `describe.only` and `it.only` mark, with `--test-only`.[^1][^5]

Run the tests again each time a file changes. Watch mode is experimental:[^1]

```text
node --test --watch
```

Measure the code coverage. Coverage is experimental, and the spec reporter prints a table at the end of the report.[^1] In this run, on Node.js 24.10.0, one branch of `cart.js` had no test, and the option `--test-coverage-lines` asked for 95 percent of the lines:

```text
node --test --experimental-test-coverage --test-coverage-lines=95
```

```text
ℹ Error: 92.31% line coverage does not meet threshold of 95%.
ℹ start of coverage report
ℹ ----------------------------------------------------------
ℹ file      | line % | branch % | funcs % | uncovered lines
ℹ ----------------------------------------------------------
ℹ cart.js   |  92.31 |    80.00 |  100.00 | 12
ℹ ----------------------------------------------------------
ℹ all files |  92.31 |    80.00 |  100.00 |
ℹ ----------------------------------------------------------
ℹ end of coverage report
```

The process ended with the exit code 1. The documentation says that if the coverage "does not reach the threshold specified, the process will exit with code 1 ."[^5] By default, the test files and the files in `node_modules/` are not in the report.[^1] The comment `/* node:coverage ignore next */` removes the next line from the report.[^1]

Write the report in another format. The built-in reporters are `spec`, `tap`, `dot`, `junit` and `lcov`, and `spec` is the default.[^1] Two reporters can write to two places at the same time:[^1]

```text
node --test --test-reporter=spec --test-reporter=junit --test-reporter-destination=stdout --test-reporter-destination=junit.xml
```

Run a TypeScript test file. By default, Node.js runs a TypeScript file that has only erasable TypeScript syntax: it replaces the types with whitespace.[^7] So a `.test.ts` file needs no build step:[^1]

```typescript test id=typescript
import { test } from "node:test";
import assert from "node:assert/strict";

interface Line {
  name: string;
  price: number;
  quantity: number;
}

function total(lines: readonly Line[]): number {
  return lines.reduce((sum, line) => sum + line.price * line.quantity, 0);
}

test("total of typed lines", () => {
  const lines: Line[] = [
    { name: "tea", price: 450, quantity: 2 },
    { name: "cup", price: 1200, quantity: 1 },
  ];
  assert.equal(total(lines), 2100);
});
```

```output
✔ total of typed lines ($TIME)
ℹ tests 1
ℹ suites 0
ℹ pass 1
ℹ fail 0
ℹ cancelled 0
ℹ skipped 0
ℹ todo 0
ℹ duration_ms $TIME
```

The documentation says that "no type checking is performed."[^7] A project that wants the type checks also runs `tsc --noEmit`. See [[typescript]].

Split a large suite between several CI machines with `--test-shard`. Each machine runs one part, for example `node --test --test-shard=1/3`.[^5]

## Configuration

Most settings are options on the command line. The documentation lists them with the `--test` prefix, for example `--test-timeout`, `--test-concurrency` and `--test-isolation`.[^5]

A file `node.config.json` can hold the options. Node.js reads it with the option `--config-file`. A namespace `test` in the file "automatically enables the corresponding flag" `--test`.[^5] In Node.js 26.12.0, the option was renamed from `--experimental-config-file` to `--config-file` and became stable.[^5] This file is equal to `node --test --test-isolation=process`:[^5]

```text
{
  "test": {
    "test-isolation": "process"
  }
}
```

The option `--test-global-setup` names a module whose functions `globalSetup` and `globalTeardown` run once before and after all the tests. It was added in 24.0.0, and it is in early development.[^1]

## Alternatives

- [[jest]]: a test framework from npm with its own `expect` assertions, module mocks and snapshots. Its API uses the same names `describe`, `it` and `beforeEach`.
- [[vitest]]: a test framework that uses the Vite build pipeline. Its API is compatible with Jest.
- [[mocha-and-chai]]: an older test runner from npm, and an assertion library. The Node.js project gives a codemod that moves Mocha test suites to node:test. It "adds the required node:test imports for the globals a file uses", and it changes `done` callbacks to the `(t, done)` signature.[^8]

## History and versions

| Version | Change |
|---|---|
| 16.17.0 and 18.0.0 | the module `node:test` is added[^1] |
| 16.17.0 and 18.1.0 | the option `--test` of the command line[^5] |
| 16.17.0 and 18.6.0 | `it()`[^1] |
| 16.18.0 and 18.8.0 | the hooks `before`, `after`, `beforeEach` and `afterEach`[^1] |
| 18.13.0 and 19.1.0 | mocks with `mock.fn()` and `mock.method()`[^1] |
| 19.2.0 | watch mode with `--watch`, experimental[^1] |
| 18.19.0 and 20.4.0 | mock timers with `mock.timers.enable()`[^1] |
| 20.0.0 | the test runner becomes stable[^1] |
| 22.3.0 | snapshot tests and `--test-update-snapshots`[^1][^5] |
| 23.0.0 | the default reporter is `spec` also when the output is not a terminal; before, it was `tap`[^1] |
| 23.4.0 | snapshot tests are no longer experimental[^1] |
| 24.0.0 | `--test-global-setup` for global setup and teardown[^1] |

The version history in the documentation shows that some new features also went into the older LTS lines. For example, the hooks were added in 18.8.0 and in 16.18.0.[^1]

## References
