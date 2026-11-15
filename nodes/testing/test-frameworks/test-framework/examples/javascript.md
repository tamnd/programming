```javascript test id=account
import { describe, it, beforeEach } from "node:test";
import assert from "node:assert/strict";

class InsufficientFunds extends Error {}

class Account {
  constructor(balance) {
    this.balance = balance;
  }

  deposit(amount) {
    this.balance += amount;
  }

  withdraw(amount) {
    if (amount > this.balance) {
      throw new InsufficientFunds(`balance ${this.balance}, asked ${amount}`);
    }
    this.balance -= amount;
  }
}

describe("Account", () => {
  let account;

  // The fixture: the runner calls this hook before each test in the suite.
  beforeEach(() => {
    account = new Account(100);
  });

  it("adds a deposit to the balance", () => {
    account.deposit(50);
    assert.equal(account.balance, 150);
  });

  for (const [amount, rest] of [[30, 70], [100, 0]]) {
    it(`leaves ${rest} after a withdrawal of ${amount}`, () => {
      account.withdraw(amount);
      assert.equal(account.balance, rest);
    });
  }

  it("rejects a withdrawal larger than the balance", () => {
    assert.throws(() => account.withdraw(150), InsufficientFunds);
    assert.equal(account.balance, 100);
  });
});
```

```output
▶ Account
  ✔ adds a deposit to the balance ($TIME)
  ✔ leaves 70 after a withdrawal of 30 ($TIME)
  ✔ leaves 0 after a withdrawal of 100 ($TIME)
  ✔ rejects a withdrawal larger than the balance ($TIME)
✔ Account ($TIME)
ℹ tests 4
ℹ suites 1
ℹ pass 4
ℹ fail 0
ℹ cancelled 0
ℹ skipped 0
ℹ todo 0
ℹ duration_ms $TIME
```

- Verdict: stdlib. `node:test` and `node:assert` are built into Node.js. `node --test` finds the test files and runs them.
- This file uses the spec style: `describe` makes a suite and `it` makes a test. They are aliases of `suite` and `test`, so the two styles give the same report.
- `beforeEach` is the fixture. It runs before each test of the suite, so each test gets a new account.
- The table of withdrawals is a plain loop that registers one test for each row. The name of each test comes from a template string, so the report names the row.
- Jest, Vitest and Mocha use the same `describe`, `it` and `beforeEach` names. Jest and Vitest also add `it.each` for tables.
