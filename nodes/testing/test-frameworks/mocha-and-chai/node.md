---
tag: YVQD25
kind: tool
slug: mocha-and-chai
name: Mocha and Chai
aliases: [Mocha, Chai, mocha, chai]
area: testing/test-frameworks
level: core
summary: >-
  Mocha is a JavaScript test runner, and Chai is an assertion library often used with it. This page shows the BDD interface, assertion styles, async tests, hooks, reporters and parallel mode.
role: [test-framework]
licence: mit
repo: https://github.com/mochajs/mocha
homepage: https://mochajs.org/
see_also: [test-framework, node-test, jest, vitest, javascript, unit-testing, test-double, test-driven-development]
sources:
  - url: https://mochajs.org/
    title: Mocha documentation, home page
    locator: the introduction
    accessed: 2026-11-15
  - url: https://www.chaijs.com/guide/installation/
    title: Chai documentation, Installation
    locator: the introduction
    accessed: 2026-11-15
  - url: https://github.com/mochajs/mocha/blob/main/LICENSE
    title: Mocha, LICENSE
    locator: the first lines
    accessed: 2026-11-15
  - url: https://github.com/chaijs/chai/blob/main/LICENSE
    title: Chai, LICENSE
    locator: the first lines
    accessed: 2026-11-15
  - url: https://registry.npmjs.org/mocha
    title: npm registry, metadata of the package mocha
    locator: dist-tags; time; versions 11.0.0, 12.0.0 and 12.0.3
    accessed: 2026-11-15
  - url: https://registry.npmjs.org/chai
    title: npm registry, metadata of the package chai
    locator: dist-tags; time; versions 4.5.0, 5.0.0 and 6.3.0
    accessed: 2026-11-15
  - url: https://mochajs.org/getting-started/
    title: Mocha documentation, Getting Started
    locator: Installation; Test File; Running Mocha
    accessed: 2026-11-15
  - url: https://mochajs.org/interfaces/bdd/
    title: Mocha documentation, BDD interface
    locator: the whole page
    accessed: 2026-11-15
  - url: https://mochajs.org/running/cli/
    title: Mocha documentation, Command-Line Usage
    locator: --bail; --exit; --forbid-only; --grep; --jobs; --parallel; --reporter; --timeout; --ui; --watch
    accessed: 2026-11-15
  - url: https://mochajs.org/interfaces/tdd/
    title: Mocha documentation, TDD interface
    locator: the whole page
    accessed: 2026-11-15
  - url: https://mochajs.org/features/arrow-functions/
    title: Mocha documentation, Arrow Functions
    locator: the whole page
    accessed: 2026-11-15
  - url: https://mochajs.org/features/assertions/
    title: Mocha documentation, Assertions
    locator: the whole page
    accessed: 2026-11-15
  - url: https://www.chaijs.com/guide/styles/
    title: Chai documentation, Assertion Styles
    locator: Assert; BDD; Expect; Should
    accessed: 2026-11-15
  - url: https://www.chaijs.com/api/bdd/
    title: Chai documentation, Expect / Should API
    locator: Language Chains; .deep
    accessed: 2026-11-15
  - url: https://mochajs.org/features/asynchronous-code/
    title: Mocha documentation, Asynchronous Code
    locator: the whole page
    accessed: 2026-11-15
  - url: https://mochajs.org/features/timeouts/
    title: Mocha documentation, Timeouts
    locator: Suite-level; Test-level
    accessed: 2026-11-15
  - url: https://mochajs.org/declaring/pending-tests/
    title: Mocha documentation, Pending Tests
    locator: the whole page
    accessed: 2026-11-15
  - url: https://mochajs.org/declaring/exclusive-tests/
    title: Mocha documentation, Exclusive Tests
    locator: the whole page
    accessed: 2026-11-15
  - url: https://mochajs.org/features/hooks/
    title: Mocha documentation, Hooks
    locator: the introduction
    accessed: 2026-11-15
  - url: https://mochajs.org/explainers/spies/
    title: Mocha documentation, Spies
    locator: the whole page
    accessed: 2026-11-15
  - url: https://mochajs.org/features/parallel-mode/
    title: Mocha documentation, Parallel Mode
    locator: the introduction; Reporter limitations
    accessed: 2026-11-15
  - url: https://mochajs.org/reporters/about/
    title: Mocha documentation, About Reporters
    locator: the whole page
    accessed: 2026-11-15
  - url: https://mochajs.org/reporters/dot/
    title: Mocha documentation, Dot Matrix reporter
    locator: the whole page
    accessed: 2026-11-15
  - url: https://mochajs.org/reporters/tap/
    title: Mocha documentation, TAP reporter
    locator: the whole page
    accessed: 2026-11-15
  - url: https://mochajs.org/explainers/nodejs-native-esm-support/
    title: Mocha documentation, Node.js native ESM support
    locator: the whole page
    accessed: 2026-11-15
  - url: https://mochajs.org/running/configuring/
    title: Mocha documentation, Configuring Mocha
    locator: the whole page
    accessed: 2026-11-15
  - url: https://nodejs.org/en/learn/userland-migrations/mocha-to-node-test-runner
    title: Node.js Learn, Mocha to Node.js Test Runner
    locator: the introduction
    accessed: 2026-11-15
---

## What it is

Mocha is a [[test-framework]] for JavaScript. Its documentation says: "Mocha is a feature-rich JavaScript test framework running on Node.js and in the browser".[^1] Mocha gives the test runner, the functions that declare suites and tests, the hooks and the reporters. It does not give assertions or mocks.

Chai is an assertion library. It is a separate package, and it works with any test framework.[^2] Many projects use the two packages together, so this page shows both. Mocha is under the MIT licence, and so is Chai.[^3][^4]

The first version of Mocha on npm, 0.0.1, is from 22 November 2011.[^5] In November 2026, the latest version is 12.0.3, from 1 October 2026, and the latest version of Chai is 6.3.0, from 30 September 2026.[^5][^6] Mocha 12 needs Node.js 20.19.0 or a later 20 release, or Node.js 22.12.0 or later.[^7]

The documentation says: "Mocha tests run serially, allowing for flexible and accurate reporting, while mapping uncaught exceptions to the correct test cases."[^1]

The examples on this page need the packages `mocha` and `chai` from npm. The site runs code blocks with no npm packages, so it does not run these blocks. The outputs on this page were recorded on 15 November 2026 with Mocha 12.0.3 and Chai 6.3.0 on Node.js 24.10.0. In the outputs, the project directory is written as `/work/shop`.

## Install and first use

Install both packages as development dependencies, and add a script to `package.json`:[^7]

```text
npm install -D mocha chai
```

```text
{
  "type": "module",
  "private": true,
  "scripts": {
    "test": "mocha"
  }
}
```

The documentation puts the test files in a directory `test`, and the command `npx mocha` runs them.[^7] This module `cart.js` is the code under test:

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

This is the test file `test/cart.test.js`. The functions `describe`, `it` and `beforeEach` are globals that Mocha makes. The function `expect` comes from Chai:

```text
import { expect } from "chai";
import { Cart } from "../cart.js";

describe("Cart", function () {
  let cart;

  beforeEach(function () {
    cart = new Cart();
  });

  it("costs nothing when it is new", function () {
    expect(cart.total()).to.equal(0);
  });

  it("multiplies the price by the quantity", function () {
    cart.add("tea", 450, 3);
    expect(cart.total()).to.equal(1350);
  });

  it("keeps the lines in order", function () {
    cart.add("tea", 450);
    cart.add("cup", 1200, 2);
    expect(cart.lines).to.have.lengthOf(2);
    expect(cart.lines[1]).to.deep.equal({ name: "cup", price: 1200, quantity: 2 });
  });
});
```

With the other example files of this page in the directory `test`, the command `npx mocha` printed this report. The failure is explained in the section on assertions:

```text
  Cart
    ✔ costs nothing when it is new
    ✔ multiplies the price by the quantity
    ✔ keeps the lines in order

  priceOf
    ✔ finds a known product
    ✔ rejects an unknown product
    ✔ calls back when the lookup is done
    - gives prices in euros
    - adds the tax

  receipt
    ✔ adds shipping below the limit
    1) gives free shipping at the limit

  the three assertion styles
    ✔ assert
    ✔ expect
    ✔ should


  10 passing (64ms)
  2 pending
  1 failing

  1) receipt
       gives free shipping at the limit:

      AssertionError: expected { subtotal: 5000, shipping: 495, …(1) } to deeply equal { subtotal: 5000, shipping: +0, …(1) }
      + expected - actual

       {
      -  "shipping": 495
      +  "shipping": 0
         "subtotal": 5000
      -  "total": 5495
      +  "total": 5000
       }

      at Context.<anonymous> (file:///work/shop/test/receipt.test.js:17:35)
      at process.processImmediate (node:internal/timers:505:21)
```

## Concepts of the tool

### Interfaces

An interface is the set of functions that declare the suites, the tests and the hooks. "The BDD interface provides describe() , context() , it() , specify() , before() , after() , beforeEach() , and afterEach() ."[^8] BDD is the default.[^9] The TDD interface uses other names: `suite()`, `test()`, `suiteSetup()`, `suiteTeardown()`, `setup()` and `teardown()`.[^10] The option `--ui tdd` selects it.[^9]

The documentation discourages arrow functions as the callbacks of tests and hooks.[^11] An arrow function cannot see the Mocha context `this`, so it cannot call `this.timeout()`.[^11]

### Assertions with Chai

Mocha accepts any assertion library: if it throws an `Error`, it works.[^12] Chai has three assertion styles. The assert style uses functions such as `assert.equal`. The BDD styles are `expect` and `should`, and both use the same chain of words.[^13] This file shows the three styles:

```text
import * as chai from "chai";

const { assert, expect } = chai;
chai.should();

describe("the three assertion styles", function () {
  const line = { name: "tea", price: 450, quantity: 3 };

  it("assert", function () {
    assert.propertyVal(line, "price", 450);
  });

  it("expect", function () {
    expect(line).to.have.property("price", 450);
  });

  it("should", function () {
    line.should.have.property("price", 450);
  });
});
```

The call `chai.should()` extends each object with a property `should`.[^13] Words such as `to`, `be`, `have` and `that` are chainable getters that make the assertion easier to read.[^14] The word `.deep` makes the assertions that follow use deep equality, and not `===`.[^14]

In this file, the code gives free shipping only above the limit, but the rule says from the limit. The second test fails:

```text
import { expect } from "chai";

const FREE_FROM = 5000;

function receipt(subtotal) {
  // The rule is "free from 5000", but the code uses > and not >=.
  const shipping = subtotal > FREE_FROM ? 0 : 495;
  return { subtotal, shipping, total: subtotal + shipping };
}

describe("receipt", function () {
  it("adds shipping below the limit", function () {
    expect(receipt(4999)).to.deep.equal({ subtotal: 4999, shipping: 495, total: 5494 });
  });

  it("gives free shipping at the limit", function () {
    expect(receipt(5000)).to.deep.equal({ subtotal: 5000, shipping: 0, total: 5000 });
  });
});
```

In the report of the first section, the message of the error comes from Chai, and the diff comes from Mocha. In this diff, `+` marks the expected value and `-` marks the actual value. This is the opposite of the diffs of [[node-test]] and [[jest]]. The process ended with the exit code 1.

### Asynchronous tests, timeouts and pending tests

A test can be asynchronous in two ways. It can return a Promise, for example with `async function`, or it can take a callback, usually named `done`.[^15] "This callback accepts both an Error instance or a falsy value".[^15] Each test has a timeout. The default is 2000 milliseconds, and `this.timeout()` changes it for a suite or a test.[^16][^9]

A test with no function is pending: "Pending tests will be included in the test results and marked as pending."[^17] `it.skip` and `describe.skip` also make tests pending, and `it.only` and `describe.only` run only the marked tests.[^17][^18]

```text
import { assert } from "chai";

const prices = new Map([["tea", 450], ["cup", 1200]]);

async function priceOf(name) {
  await new Promise((resolve) => setTimeout(resolve, 10));
  if (!prices.has(name)) {
    throw new RangeError(`no price for ${name}`);
  }
  return prices.get(name);
}

describe("priceOf", function () {
  this.timeout(500);

  it("finds a known product", async function () {
    assert.strictEqual(await priceOf("tea"), 450);
  });

  it("rejects an unknown product", async function () {
    try {
      await priceOf("cake");
      assert.fail("priceOf did not reject");
    } catch (error) {
      assert.instanceOf(error, RangeError);
      assert.strictEqual(error.message, "no price for cake");
    }
  });

  it("calls back when the lookup is done", function (done) {
    priceOf("cup").then((price) => {
      assert.strictEqual(price, 1200);
      done();
    }, done);
  });

  it("gives prices in euros");

  it.skip("adds the tax", async function () {
    assert.strictEqual(await priceOf("tea"), 540);
  });
});
```

In the report of the first section, the spec reporter marks the two pending tests with `-`, and the totals have the line `2 pending`.

### Hooks

The BDD interface has the hooks `before()`, `after()`, `beforeEach()` and `afterEach()`.[^19] "Hooks will run in the order they are defined, as appropriate: all before() hooks run (once), then any beforeEach() hooks, tests, any afterEach() hooks, and finally after() hooks (once)."[^19] A hook can be asynchronous in the same ways as a test.[^19]

### Spies and mocks

The documentation says: "Mocha does not come equipped with spies, though libraries like Sinon provide this behaviour if desired."[^20] So a Mocha project adds a library for [[test-double]] objects, or it writes them by hand.

### Parallel mode

The option `--parallel` runs the test files in a pool of worker processes.[^9] The option `--jobs` sets the maximum number of processes, and the default is the number of CPU cores less 1.[^9] In parallel mode, the reporter output is buffered, and the report of each file comes after the file is completed.[^21] Some reporters do not work in parallel mode, because they must know the number of tests before the run.[^21] Exclusive tests with `.only` do not work in parallel mode.[^18]

## Common tasks

Run only the tests whose name matches a string or a regular expression:[^9]

```text
npx mocha --grep "shipping"
```

Choose a reporter. The default reporter is `spec`.[^9] Mocha turns off the colours when the output is not a terminal.[^22] The reporter `dot` prints one character for each test, a comma for each pending test and `!` for each failed test.[^23] This is the start of its report:

```text
  ......,,.!...

  10 passing (83ms)
  2 pending
  1 failing
```

The reporter `tap` writes lines for a consumer of the Test Anything Protocol.[^24] This is the output of `npx mocha --reporter tap test/cart.test.js`:

```text
ok 1 Cart costs nothing when it is new
ok 2 Cart multiplies the price by the quantity
ok 3 Cart keeps the lines in order
# tests 3
# pass 3
# fail 0
1..3
```

Stop at the first failure with `--bail`.[^9] Run the tests again when a file changes with `--watch`.[^9]

Fail the run in CI when a test has `.only`. Since Mocha 12, the option `--forbid-only` is on by default when the environment variable `CI` is set.[^9]

Make the process end when the tests end. Since Mocha 4, Mocha does not force its process to exit. A test run that does not end often has a server, a socket or a timer that is still open. The option `--exit` forces the exit, but the documentation recommends to find the cause.[^9]

Use ECMAScript modules. Test files can be ES modules with no extra setup: a file with the extension `.mjs`, or a `.js` file in a package with `"type": "module"`.[^25]

## Configuration

Mocha reads a configuration file from the root directory of the project. It can be `.mocharc.js`, `.mocharc.yaml` or `.mocharc.yml`, `.mocharc.json` or `.mocharc.jsonc`, or a property `mocha` in `package.json`.[^26] The property `spec` gives the test files, for example `"spec": "test/**/*.spec.js"`.[^26] This file `.mocharc.yml` sets the timeout and the reporter:

```text
spec: "test/**/*.test.js"
timeout: 5000
reporter: dot
```

## Alternatives

- [[node-test]]: the test runner that is built into Node.js. Its API also has `describe`, `it` and the hooks. The Node.js project gives a codemod that moves Mocha test suites to node:test, and it "adds the required node:test imports for the globals a file uses".[^27]
- [[jest]]: one package with a runner, assertions, mocks and snapshots.
- [[vitest]]: a test framework that uses the Vite build pipeline. Its API is compatible with Jest.

## History and versions

| Package and version | Date | Change |
|---|---|---|
| Mocha 0.0.1 | 22 November 2011 | the first version on npm[^5] |
| Chai 0.0.1 | 7 December 2011 | the first version on npm[^6] |
| Mocha 4.0.0 | 3 October 2017 | Mocha no longer forces its process to exit[^5][^9] |
| Mocha 8.0.0 | 10 June 2020 | `--parallel` and `--jobs`[^5][^9] |
| Chai 5.0.0 | 28 December 2023 | the package is an ECMAScript module[^6] |
| Mocha 11.0.0 | 11 November 2024 | its field `engines` lists Node.js 18.18.0, 20.9.0, and 21.1.0 or later[^5] |
| Chai 6.0.0 | 22 August 2025 | needs Node.js 18 or later[^6] |
| Mocha 12.0.0 | 31 August 2026 | the package is an ECMAScript module; needs Node.js 20.19.0 or a later 20 release, or 22.12.0 or later; `--forbid-only` is on in CI[^5][^7][^9] |

## References
