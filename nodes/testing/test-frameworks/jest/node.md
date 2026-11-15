---
tag: 47AHGW
kind: tool
slug: jest
name: Jest
aliases: [jest]
area: testing/test-frameworks
level: core
summary: >-
  Jest is a JavaScript test framework from npm with its own runner, expect assertions, mock functions, module mocks, fake timers, snapshots and coverage, shown with output from real runs.
role: [test-framework]
licence: mit
repo: https://github.com/jestjs/jest
homepage: https://jestjs.io/
see_also: [test-framework, node-test, vitest, mocha-and-chai, javascript, typescript, unit-testing, test-double]
sources:
  - url: https://github.com/jestjs/jest/blob/main/LICENSE
    title: Jest, LICENSE
    locator: the first lines
    accessed: 2026-11-15
  - url: https://engineering.fb.com/2022/05/11/open-source/jest-openjs-foundation/
    title: Engineering at Meta, Meta Open Source is transferring Jest to the OpenJS Foundation
    locator: the first four paragraphs
    accessed: 2026-11-15
  - url: https://jestjs.io/blog/2022/05/11/jest-joins-openjs
    title: Jest blog, Jest joins OpenJS Foundation
    locator: the whole post
    accessed: 2026-11-15
  - url: https://registry.npmjs.org/jest
    title: npm registry, metadata of the package jest
    locator: dist-tags; time; versions 30.5.2 engines and license
    accessed: 2026-11-15
  - url: https://jestjs.io/docs/configuration
    title: Jest 30.5 documentation, Configuring Jest
    locator: testMatch; testEnvironment; verbose; coverageProvider; collectCoverage
    accessed: 2026-11-15
  - url: https://jestjs.io/docs/getting-started
    title: Jest 30.5 documentation, Getting Started
    locator: Using Babel; Using TypeScript; Type definitions
    accessed: 2026-11-15
  - url: https://jestjs.io/docs/using-matchers
    title: Jest 30.5 documentation, Using Matchers
    locator: Common Matchers
    accessed: 2026-11-15
  - url: https://jestjs.io/docs/mock-functions
    title: Jest 30.5 documentation, Mock Functions
    locator: the introduction; .mock property; Mocking Modules
    accessed: 2026-11-15
  - url: https://jestjs.io/docs/timer-mocks
    title: Jest 30.5 documentation, Timer Mocks
    locator: Enable Fake Timers; Advance Timers by Time
    accessed: 2026-11-15
  - url: https://jestjs.io/docs/snapshot-testing
    title: Jest 30.5 documentation, Snapshot Testing
    locator: Snapshot Testing with Jest; Inline Snapshots; Best Practices; Frequently Asked Questions
    accessed: 2026-11-15
  - url: https://jestjs.io/docs/cli
    title: Jest 30.5 documentation, Jest CLI Options
    locator: --ci; --coverage; --maxWorkers; --onlyChanged; --runInBand; --shard; --testNamePattern; --updateSnapshot; --watch; --watchAll
    accessed: 2026-11-15
  - url: https://jestjs.io/blog/2025/06/04/jest-30
    title: "Jest blog, Jest 30: Faster, Leaner, Better"
    locator: Breaking Changes; Globals cleanup between test files; New Features
    accessed: 2026-11-15
  - url: https://jestjs.io/docs/ecmascript-modules
    title: Jest 30.5 documentation, ECMAScript Modules
    locator: the whole page
    accessed: 2026-11-15
---

## What it is

Jest is a [[test-framework]] for JavaScript that is installed from npm. One package gives the test runner, the `expect` assertions, mock functions, module mocks, fake timers, snapshot tests and code coverage. Jest is under the MIT licence.[^1]

Jest began at Facebook in 2011, when the chat feature was rewritten in JavaScript, and it became open source in 2014.[^2] In 2016, Christoph Nakazawa overhauled the project with a dedicated team.[^2] In May 2022, Meta moved Jest to the OpenJS Foundation, and the Jest core team leads the project.[^2][^3] In November 2026, the latest version on npm is 30.5.2, from 18 September 2026. Its field `engines` lists Node.js 18.14 and the later 18 releases, 20, 22, and 24 or later.[^4]

By default, Jest runs the tests in a Node.js environment. A web project can choose a browser-like environment, jsdom.[^5]

The examples on this page need the package `jest` from npm. The site runs code blocks with no npm packages, so it does not run these blocks. The outputs on this page were recorded on 15 November 2026 with Jest 30.5.2 on Node.js 24.10.0.

## Install and first use

Install Jest in the project, and add a script to `package.json`:[^6]

```text
npm install --save-dev jest
```

```text
{
  "private": true,
  "scripts": {
    "test": "jest"
  }
}
```

By default, Jest finds the files in folders named `__tests__`, and the files whose names end in `.test.js` or `.spec.js`. The same rule applies to the other JavaScript and TypeScript extensions.[^5] This module `cart.js` is the code under test:

```text
class Cart {
  lines = [];

  add(name, price, quantity = 1) {
    this.lines.push({ name, price, quantity });
  }

  total() {
    return this.lines.reduce((sum, line) => sum + line.price * line.quantity, 0);
  }
}

module.exports = { Cart };
```

This is the test file `cart.test.js`. The functions `test`, `describe`, `beforeEach` and `expect` are globals, so the file does not import them. A TypeScript project can import the same functions from the package `@jest/globals`.[^6]

```text
const { Cart } = require("./cart");

test("a new cart costs nothing", () => {
  expect(new Cart().total()).toBe(0);
});

describe("Cart", () => {
  let cart;

  beforeEach(() => {
    cart = new Cart();
  });

  test("multiplies the price by the quantity", () => {
    cart.add("tea", 450, 3);
    expect(cart.total()).toBe(1350);
  });

  test("adds the lines", () => {
    cart.add("tea", 450);
    cart.add("cup", 1200, 2);
    expect(cart.total()).toBe(2850);
  });
});
```

The command `npx jest cart.test.js` printed this report:

```text
PASS ./cart.test.js
  ✓ a new cart costs nothing (5 ms)
  Cart
    ✓ multiplies the price by the quantity (2 ms)
    ✓ adds the lines (2 ms)

Test Suites: 1 passed, 1 total
Tests:       3 passed, 3 total
Snapshots:   0 total
Time:        1.345 s
Ran all test suites matching cart.test.js.
```

Jest lists each test because the run had only one test file. With more files, the option `verbose` is false by default, and the report gives one line for each file.[^5]

## Concepts of the tool

### Matchers

A test calls `expect(value)` and then a matcher. The documentation says: "toBe uses Object.is to test exact equality. If you want to check the value of an object, use toEqual".[^7] `toEqual` checks every field, but it ignores the properties that are `undefined`. `toStrictEqual` does not ignore them.[^7]

`test.each` runs one test for each row of a table. In this file, the code gives free shipping only above the limit, but the rule says from the limit:

```text
const FREE_FROM = 5000;

function receipt(subtotal) {
  // The rule is "free from 5000", but the code uses > and not >=.
  const shipping = subtotal > FREE_FROM ? 0 : 495;
  return { subtotal, shipping, total: subtotal + shipping };
}

test.each([
  [4999, 495],
  [5000, 0],
  [5001, 0],
])("shipping for %i is %i", (subtotal, shipping) => {
  expect(receipt(subtotal)).toEqual({ subtotal, shipping, total: subtotal + shipping });
});
```

```text
FAIL ./receipt.test.js
  ✓ shipping for 4999 is 495 (15 ms)
  ✕ shipping for 5000 is 0 (11 ms)
  ✓ shipping for 5001 is 0 (1 ms)

  ● shipping for 5000 is 0

    expect(received).toEqual(expected) // deep equality

    - Expected  - 2
    + Received  + 2

      Object {
    -   "shipping": 0,
    +   "shipping": 495,
        "subtotal": 5000,
    -   "total": 5000,
    +   "total": 5495,
      }

      12 |   [5001, 0],
      13 | ])("shipping for %i is %i", (subtotal, shipping) => {
    > 14 |   expect(receipt(subtotal)).toEqual({ subtotal, shipping, total: subtotal + shipping });
         |                             ^
      15 | });
      16 |

      at toEqual (receipt.test.js:14:29)

Test Suites: 1 failed, 1 total
Tests:       1 failed, 2 passed, 3 total
Snapshots:   0 total
Time:        1.822 s
Ran all test suites matching receipt.test.js.
```

The `%i` in the name takes the values of the row. Each line with `-` is from the expected value, and each line with `+` is from the received value. The report also shows the line of the failed assertion. The process ended with the exit code 1.

### Mock functions and module mocks

The documentation says that mock functions erase "the actual implementation of a function", and capture the calls and their parameters.[^8] `jest.fn()` makes a mock function. Each mock function has a property `.mock` that keeps the calls, the results and the value of `this` for each call.[^8] Matchers such as `toHaveBeenCalledWith` read that property.[^8]

`jest.mock(path)` replaces a whole module with an automatic mock.[^8] `jest.useFakeTimers()` replaces `setTimeout()` and the other timer functions, and `jest.advanceTimersByTime()` moves the fake clock.[^9] This module sends reminders:

```text
const mailer = require("./mailer");

const DAY = 24 * 60 * 60 * 1000;

function remindAll(customers) {
  for (const customer of customers) {
    if (customer.owes > 0) {
      mailer.send(customer.name, `You owe ${customer.owes} cents.`);
    }
  }
}

function remindTomorrow(send) {
  setTimeout(send, DAY);
}

module.exports = { DAY, remindAll, remindTomorrow };
```

The real `mailer.send` throws an error if a test calls it. The test file replaces the module `./mailer` and the timers:

```text
jest.mock("./mailer");

const mailer = require("./mailer");
const { DAY, remindAll, remindTomorrow } = require("./reminders");

test("reminds only the customers who owe money", () => {
  remindAll([
    { name: "Ana", owes: 1200 },
    { name: "Bao", owes: 0 },
  ]);

  expect(mailer.send).toHaveBeenCalledTimes(1);
  expect(mailer.send).toHaveBeenCalledWith("Ana", "You owe 1200 cents.");
});

test("sends the reminder after one day", () => {
  jest.useFakeTimers();
  const send = jest.fn();

  remindTomorrow(send);
  jest.advanceTimersByTime(DAY - 1);
  expect(send).not.toHaveBeenCalled();
  jest.advanceTimersByTime(1);
  expect(send).toHaveBeenCalledTimes(1);
});
```

```text
PASS ./reminders.test.js
  ✓ reminds only the customers who owe money (13 ms)
  ✓ sends the reminder after one day (10 ms)

Test Suites: 1 passed, 1 total
Tests:       2 passed, 2 total
Snapshots:   0 total
Time:        1.27 s
Ran all test suites matching reminders.test.js.
```

See [[test-double]] for the kinds of test doubles.

### Snapshots

`toMatchSnapshot()` compares a value with a value that an earlier run saved. On the first run, Jest writes the snapshot file. The documentation says: "The snapshot artifact should be committed alongside code changes, and reviewed as part of your code review process."[^10] This test file was run two times:

```text
function receiptLines(cart) {
  return cart.map((line) => `${line.quantity} x ${line.name}: ${line.price * line.quantity}`);
}

test("receipt lines", () => {
  expect(receiptLines([
    { name: "tea", price: 450, quantity: 3 },
    { name: "cup", price: 1200, quantity: 1 },
  ])).toMatchSnapshot();
});
```

The first run reported `1 snapshot written`, and it made the file `__snapshots__/lines.test.js.snap`:

```text
// Jest Snapshot v1, https://jestjs.io/docs/snapshot-testing

exports[`receipt lines 1`] = `
[
  "3 x tea: 1350",
  "1 x cup: 1200",
]
`;
```

The second run compared the value with the file and reported `Snapshots:   1 passed, 1 total`. When a change is correct, `jest --updateSnapshot` writes the snapshots again.[^10] With the option `--ci`, Jest does not write a new snapshot: the test fails.[^11] `toMatchInlineSnapshot()` writes the snapshot into the test file, as an argument of the call.[^10]

### Test files and workers

Jest isolates the test files. It runs each test file in a separate VM context, so each file gets a fresh global environment.[^12] The files run in a pool of worker processes. In a single run, the default number of workers is the number of cores minus one.[^11] The option `--runInBand` runs all the tests in the current process, "rather than creating a worker pool of child processes that run tests."[^11]

## Common tasks

Run only the tests whose full name matches a regular expression. The full name joins the names of the `describe` blocks and the test name:[^11]

```text
npx jest -t "adds the lines"
```

Run the tests again when a file changes. `--watch` runs the tests that are related to the changed files, and `--watchAll` runs all the tests.[^11] The option `--onlyChanged` works only in a git or hg repository.[^11]

Measure the code coverage with `--coverage`.[^11] This is the report of `npx jest --coverage cart.test.js`, without the list of the tests:

```text
----------|---------|----------|---------|---------|-------------------
File      | % Stmts | % Branch | % Funcs | % Lines | Uncovered Line #s
----------|---------|----------|---------|---------|-------------------
All files |     100 |      100 |     100 |     100 |
 cart.js  |     100 |      100 |     100 |     100 |
----------|---------|----------|---------|---------|-------------------
```

Jest has two coverage providers, `babel`, the default, and `v8`.[^5]

Split a large suite between CI machines, for example with `jest --shard=1/3`, `jest --shard=2/3` and `jest --shard=3/3`.[^11]

Test TypeScript code. Jest supports TypeScript through Babel and the package `@babel/preset-typescript`, or through the package `ts-jest`.[^6] With Babel, "Jest will not type-check your tests as they are run".[^6] See [[typescript]].

Use ECMAScript modules. The documentation says: "Jest ships with experimental support for ECMAScript Modules (ESM)."[^13] The tests run with the Node.js option `--experimental-vm-modules`, for example `NODE_OPTIONS="$NODE_OPTIONS --experimental-vm-modules" npx jest`.[^13] For this reason, the examples on this page use CommonJS modules with `require`.

## Configuration

Jest finds a configuration file automatically if it is named `jest.config.js`, `jest.config.ts`, `jest.config.mjs`, `jest.config.json` or one of the other listed names.[^5] This file sets the environment and asks for coverage on each run:[^5]

```text
module.exports = {
  testEnvironment: "node",
  collectCoverage: true,
  coverageProvider: "v8",
};
```

The command `npm init jest@latest` asks some questions and makes a configuration file.[^6] If a Babel configuration is in the project, the package `babel-jest` transforms the files with it.[^6]

## Alternatives

- [[node-test]]: the test runner that is built into Node.js. It needs no npm package, and its API also has `describe`, `it` and `beforeEach`.
- [[vitest]]: a test framework that uses the Vite build pipeline. Its API is compatible with Jest, and it runs ECMAScript modules and TypeScript with no extra setup.
- [[mocha-and-chai]]: a test runner and an assertion library that are two separate packages.

## History and versions

| Version or date | Change |
|---|---|
| 2011 | Jest is made at Facebook for the rewrite of the chat feature[^2] |
| 2014 | Jest becomes open source[^2] |
| 2016 | Christoph Nakazawa overhauls the project with a dedicated team[^2] |
| May 2022 | Meta moves Jest to the OpenJS Foundation[^2][^3] |
| 30.0.0, 10 June 2025 | drops Node.js 14, 16, 19 and 21; renames `--testPathPattern` to `--testPathPatterns`; adds `using` with spies, `expect.arrayOf` and the `%$` placeholder of `test.each`[^4][^12] |
| 30.5.2, 18 September 2026 | the latest version in November 2026[^4] |

## References
