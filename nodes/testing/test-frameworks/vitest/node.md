---
tag: 43S2S7
kind: tool
slug: vitest
name: Vitest
aliases: [vitest]
area: testing/test-frameworks
level: core
summary: >-
  Vitest is a JavaScript and TypeScript test framework that uses the Vite pipeline and has a Jest-compatible API. This page shows tests, mocks, fake timers, snapshots, watch mode and browser mode.
role: [test-framework]
licence: mit
repo: https://github.com/vitest-dev/vitest
homepage: https://vitest.dev/
see_also: [test-framework, jest, node-test, mocha-and-chai, javascript, typescript, unit-testing, test-double]
sources:
  - url: https://vitest.dev/guide/
    title: Vitest 5.1 documentation, Getting Started
    locator: Adding Vitest to Your Project; Writing Tests; Configuring Vitest
    accessed: 2026-11-15
  - url: https://vitest.dev/guide/why
    title: Vitest 5.1 documentation, Why Vitest
    locator: the whole page
    accessed: 2026-11-15
  - url: https://github.com/vitest-dev/vitest/blob/main/LICENSE
    title: Vitest, LICENSE
    locator: the first lines
    accessed: 2026-11-15
  - url: https://vitest.dev/guide/features
    title: Vitest 5.1 documentation, Features
    locator: Smart and instant watch mode; Threads; Running Tests Concurrently; Coverage; In-Source Testing; Type Testing; Sharding
    accessed: 2026-11-15
  - url: https://registry.npmjs.org/vitest
    title: npm registry, metadata of the package vitest
    locator: dist-tags; time; versions 5.1.0 engines and license
    accessed: 2026-11-15
  - url: https://vitest.dev/blog/vitest-5
    title: Vitest blog, Vitest 5.0 is out
    locator: Performance Improvements; Trace View; vi.when; Breaking Changes
    accessed: 2026-11-15
  - url: https://vitest.dev/guide/migration/jest
    title: Vitest 5.1 documentation, Migrating from Jest
    locator: Globals as a Default; Module Mocks; Test Names; Done Callback
    accessed: 2026-11-15
  - url: https://vitest.dev/config/globals
    title: Vitest 5.1 documentation, Config, globals
    locator: the whole page
    accessed: 2026-11-15
  - url: https://vitest.dev/guide/mocking/modules
    title: Vitest 5.1 documentation, Mocking Modules
    locator: Mocking a Module; Automocking
    accessed: 2026-11-15
  - url: https://vitest.dev/guide/mocking/timers
    title: Vitest 5.1 documentation, Mocking Timers
    locator: the introduction
    accessed: 2026-11-15
  - url: https://vitest.dev/guide/snapshot
    title: Vitest 5.1 documentation, Snapshot
    locator: Use Snapshots; Inline Snapshots; Updating Snapshots; File Snapshots
    accessed: 2026-11-15
  - url: https://vitest.dev/config/pool
    title: Vitest 5.1 documentation, Config, pool
    locator: threads; forks
    accessed: 2026-11-15
  - url: https://vitest.dev/config/environment
    title: Vitest 5.1 documentation, Config, environment
    locator: the whole page
    accessed: 2026-11-15
  - url: https://vitest.dev/guide/cli
    title: Vitest 5.1 documentation, Command Line Interface
    locator: vitest run; vitest watch; vitest related; vitest list; vitest doctor; --shard
    accessed: 2026-11-15
  - url: https://vitest.dev/blog/vitest-5-1
    title: Vitest blog, Vitest 5.1 is out
    locator: the first sections
    accessed: 2026-11-15
  - url: https://vitest.dev/guide/coverage
    title: Vitest 5.1 documentation, Coverage
    locator: Coverage Providers
    accessed: 2026-11-15
  - url: https://vitest.dev/guide/browser/
    title: Vitest 5.1 documentation, Browser Mode, Getting Started
    locator: Installation
    accessed: 2026-11-15
  - url: https://vitest.dev/blog/vitest-4
    title: Vitest blog, Vitest 4.0 is out
    locator: Browser Mode is Stable; Visual Regression Testing
    accessed: 2026-11-15
  - url: https://vitest.dev/config/
    title: Vitest 5.1 documentation, Configuring Vitest
    locator: the introduction
    accessed: 2026-11-15
---

## What it is

Vitest is a [[test-framework]] for JavaScript and TypeScript that is installed from npm. The documentation calls Vitest "a next generation testing framework powered by Vite."[^1] Vitest uses the same configuration and the same transformation pipeline as a Vite application. So the plugins of the application also apply to the tests.[^2] Vitest is under the MIT licence.[^3]

The API is compatible with [[jest]]. The documentation says that Vitest can be used "as a drop-in replacement in most projects".[^2] It has `describe`, `test`, `it`, `expect` and the hooks, mock functions with `vi`, snapshots and coverage.[^2][^4] ECMAScript modules, TypeScript and JSX work with no extra setup.[^4]

In November 2026, the latest version on npm is 5.1.0, from 2 November 2026.[^5] Vitest 5 needs Vite 6.4.0 or later and Node.js 22.12.0 or later.[^1][^6]

The examples on this page need the package `vitest` from npm. The site runs code blocks with no npm packages, so it does not run these blocks. The outputs on this page were recorded on 15 November 2026 with Vitest 5.1.0 on Node.js 24.10.0. In the outputs, the project directory is written as `/work/shop`.

## Install and first use

Install Vitest in the project, and add a script to `package.json`:[^1]

```text
npm install -D vitest
```

```text
{
  "type": "module",
  "private": true,
  "scripts": {
    "test": "vitest"
  }
}
```

"By default, tests must contain .test. or .spec. in their file name."[^1] This module `cart.js` is the code under test:

```text
export class Cart {
  lines = [];

  add(name, price, quantity = 1) {
    this.lines.push({ name, price, quantity });
  }

  total() {
    return this.lines.reduce((sum, line) => sum + line.price * line.quantity, 0);
  }
}
```

This is the test file `cart.test.js`. Unlike Jest, Vitest does not put the test functions in the global scope by default, so the file imports them from `vitest`.[^7][^8]

```text
import { describe, test, expect, beforeEach } from "vitest";
import { Cart } from "./cart.js";

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

The command `vitest` starts in watch mode, and `vitest run` runs the tests one time.[^1][^4] The command `npx vitest run cart.test.js` printed this report:

```text
 RUN  v5.1.0 /work/shop

 ✓ cart.test.js (3 tests) 11ms

 Test Files  1 passed (1)
      Tests  3 passed (3)
   Start at  15:43:28
   Duration  856ms (transform 71%, import 19%, tests 5%, worker 5%)
```

The default reporter gives one line for each passed file. With `--reporter=verbose`, it gives one line for each test, and the name joins the file, the suites and the test with `>`. These are the test lines of `npx vitest run --reporter=verbose cart.test.js`:

```text
 ✓ cart.test.js > a new cart costs nothing 10ms
 ✓ cart.test.js > Cart > multiplies the price by the quantity 6ms
 ✓ cart.test.js > Cart > adds the lines 3ms
```

## Concepts of the tool

### Assertions

`expect` has the matchers of Jest, such as `toBe`, `toEqual` and `toStrictEqual`. The documentation says: "Chai is built-in for assertions with Jest expect -compatible APIs."[^4] In this file, the code gives free shipping only above the limit, but the rule says from the limit:

```text
import { test, expect } from "vitest";

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
 RUN  v5.1.0 /work/shop

 ❯ receipt.test.js (3 tests | 1 failed) 53ms
   ✓ shipping for 4999 is 495 9ms
   × shipping for 5000 is 0 38ms
   ✓ shipping for 5001 is 0 1ms

⎯⎯⎯⎯⎯⎯⎯ Failed Tests 1 ⎯⎯⎯⎯⎯⎯⎯

 FAIL  receipt.test.js > shipping for 5000 is 0
AssertionError: expected { subtotal: 5000, shipping: 495, …(1) } to deeply equal { subtotal: 5000, shipping: +0, …(1) }

- Expected
+ Received

  {
-   "shipping": 0,
+   "shipping": 495,
    "subtotal": 5000,
-   "total": 5000,
+   "total": 5495,
  }

 ❯ receipt.test.js:16:29
     14|   [5001, 0],
     15| ])("shipping for %i is %i", (subtotal, shipping) => {
     16|   expect(receipt(subtotal)).toEqual({ subtotal, shipping, total: subto…
       |                             ^
     17| });
     18|

⎯⎯⎯⎯⎯⎯⎯⎯⎯⎯⎯⎯⎯⎯⎯⎯⎯⎯⎯⎯⎯⎯⎯⎯[1/1]⎯


 Test Files  1 failed (1)
      Tests  1 failed | 2 passed (3)
   Start at  15:43:33
   Duration  824ms (import 36%, transform 34%, tests 22%, worker 8%)
```

The first line of the error comes from Chai, and the diff is in the format of Jest. The process ended with the exit code 1.

### Mocks and fake timers

The object `vi` makes test doubles. `vi.fn()` makes a mock function, and `vi.spyOn()` records the calls of a method.[^9] `vi.mock(path)` replaces a module. With no factory and no file in `__mocks__`, Vitest makes an automatic mock in which "All methods will return undefined".[^9] Vitest moves each `vi.mock` call to the top of the file, so it runs before the imports.[^9] `vi.useFakeTimers()` replaces `setTimeout` and `setInterval`, and the test moves the clock with `vi.advanceTimersByTime()`.[^10]

This module sends reminders:

```text
import { send } from "./mailer.js";

export const DAY = 24 * 60 * 60 * 1000;

export function remindAll(customers) {
  for (const customer of customers) {
    if (customer.owes > 0) {
      send(customer.name, `You owe ${customer.owes} cents.`);
    }
  }
}

export function remindTomorrow(callback) {
  setTimeout(callback, DAY);
}
```

The real `send` throws an error if a test calls it. The test file replaces the module `./mailer.js` and the timers:

```text
import { test, expect, vi, afterEach } from "vitest";
import { send } from "./mailer.js";
import { DAY, remindAll, remindTomorrow } from "./reminders.js";

vi.mock("./mailer.js");

afterEach(() => {
  vi.useRealTimers();
});

test("reminds only the customers who owe money", () => {
  remindAll([
    { name: "Ana", owes: 1200 },
    { name: "Bao", owes: 0 },
  ]);

  expect(send).toHaveBeenCalledTimes(1);
  expect(send).toHaveBeenCalledWith("Ana", "You owe 1200 cents.");
});

test("sends the reminder after one day", () => {
  vi.useFakeTimers();
  const callback = vi.fn();

  remindTomorrow(callback);
  vi.advanceTimersByTime(DAY - 1);
  expect(callback).not.toHaveBeenCalled();
  vi.advanceTimersByTime(1);
  expect(callback).toHaveBeenCalledTimes(1);
});
```

```text
 RUN  v5.1.0 /work/shop

 ✓ reminders.test.js (2 tests) 22ms

 Test Files  1 passed (1)
      Tests  2 passed (2)
   Start at  15:43:38
   Duration  1.48s (transform 57%, import 39%, tests 2%, worker 1%)
```

The module mocks of Vitest are not equal to those of Jest. For example, a factory must return an object with each export, and a file in `__mocks__` is used only when the test calls `vi.mock()`.[^7] See [[test-double]] for the kinds of test doubles.

### Snapshots

The documentation says: "The snapshot artifact should be committed alongside code changes, and reviewed as part of your code review process."[^11] `toMatchSnapshot()` keeps the snapshot in a file in `__snapshots__`. `toMatchInlineSnapshot()` writes it into the test file.[^11] Before the first run, this test had no argument in the call:

```text
import { test, expect } from "vitest";

function receiptLines(cart) {
  return cart.map((line) => `${line.quantity} x ${line.name}: ${line.price * line.quantity}`);
}

test("receipt lines", () => {
  expect(receiptLines([
    { name: "tea", price: 450, quantity: 3 },
    { name: "cup", price: 1200, quantity: 1 },
  ])).toMatchInlineSnapshot();
});
```

The run reported `Snapshots  1 written`, and Vitest changed the end of the test to this:

```text
  ])).toMatchInlineSnapshot(`
    [
      "3 x tea: 1350",
      "1 x cup: 1200",
    ]
  `);
```

When a change is correct, `vitest -u` writes the snapshots again.[^11] `toMatchFileSnapshot()` compares a value with a file that has any extension, for example an HTML file.[^11]

### Test files and workers

"By default Vitest runs test files in multiple processes using node:child_process , allowing tests to run simultaneously."[^4] The default pool is `forks`, and the pool `threads` uses worker threads.[^12] Vitest isolates the environment of each test file, and `--no-isolate` turns the isolation off.[^4] Inside one file, `test.concurrent` starts tests at the same time.[^4]

The default environment is Node.js. A web project can choose `jsdom` or `happy-dom`, which are separate packages.[^13]

## Common tasks

Run only the tests whose name matches a pattern. The name joins the suites and the test with `>`, so `-t adds` matches `Cart > adds the lines`:[^7]

```text
npx vitest run -t adds
```

Run a TypeScript test file. Vitest transforms TypeScript with no extra setup.[^4] This test file `total.test.ts` passed:

```text
import { test, expect } from "vitest";

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
  expect(total(lines)).toBe(2100);
});
```

For tests of the types, Vitest has `expectTypeOf` from the package expect-type.[^4] See [[typescript]].

Run the tests that a change affects. In watch mode, Vitest uses the module graph and runs again only the tests that are related to the changed files.[^4] `vitest related` takes a list of source files.[^14] Vitest 5.1 adds the experimental option `--since-last-run`.[^15]

Measure the code coverage with `--coverage`. Vitest supports V8 coverage and istanbul coverage, and the default provider is `v8`.[^16] Each provider is a separate package, for example `@vitest/coverage-v8`.[^5][^16]

Run the tests in a real browser. Browser mode needs a provider: `preview`, `playwright` or `webdriverio`.[^17] It became stable in Vitest 4.[^18] See [[playwright]].

Put the tests in the source file. With in-source testing, a block `if (import.meta.vitest)` at the end of a module holds tests that can use the private parts of the module.[^4]

Split a large suite between CI machines, for example with `vitest run --shard=1/3`.[^14]

## Configuration

"Vitest reads your vite.config.* by default".[^1] A file `vitest.config.ts` or `vitest.config.js` has a higher priority than the Vite configuration.[^19] The settings go into the property `test`:[^19]

```text
import { defineConfig } from "vitest/config";

export default defineConfig({
  test: {
    environment: "node",
    globals: false,
  },
});
```

With `globals: true`, or the option `--globals`, Vitest puts `test`, `expect` and the other functions in the global scope, as Jest does.[^8]

## Alternatives

- [[jest]]: the framework whose API Vitest follows.
- [[node-test]]: the test runner that is built into Node.js. It needs no npm package.
- [[mocha-and-chai]]: an older test runner from npm, and the assertion library that Vitest also uses inside `expect`.

## History and versions

| Version | Date | Change |
|---|---|---|
| 0.0.1 | 3 December 2021 | the first version on npm[^5] |
| 1.0.0 | 4 December 2023 | the first major version[^5] |
| 4.0.0 | 22 October 2025 | browser mode is no longer experimental; visual regression tests in browser mode[^5][^18] |
| 5.0.0 | 3 September 2026 | faster runs; Trace View for browser mode; `vi.when`; needs Vite 6.4.0 and Node.js 22.12.0 or later[^5][^6] |
| 5.1.0 | 2 November 2026 | `--since-last-run`; an interactive review of failed snapshots[^5][^15] |

## References
