```javascript run id=account
import assert from "node:assert/strict";

class Account {
  #balance;

  constructor(balance) {
    assert.ok(Number.isInteger(balance) && balance >= 0, "precondition: balance");
    this.#balance = balance;
  }

  get balance() {
    return this.#balance;
  }

  withdraw(amount) {
    assert.ok(amount > 0 && amount <= this.#balance, `precondition: withdraw ${amount} from ${this.#balance}`);
    const old = this.#balance;
    this.#balance -= amount;
    assert.equal(this.#balance, old - amount, "postcondition");
  }
}

const account = new Account(100);
account.withdraw(30);
console.log("balance", account.balance);
try {
  account.withdraw(500);
} catch (err) {
  console.log(err.name, err.code, err.message);
}
```

```output
balance 70
AssertionError ERR_ASSERTION precondition: withdraw 500 from 70
```

- Verdict: stdlib. The Node.js module `node:assert` throws an `AssertionError` when a check fails. `assert.ok(value, message)` tests that the value is truthy.
- The language has no `assert` statement and no flag that removes checks. A check runs in every build.
- `console.assert` is not the same. It only writes a message, and the program goes on.

`console.assert` writes "Assertion failed" to standard error and does not throw. The withdrawal below goes through.

```javascript wrong id=console-assert
let balance = 100;

function withdraw(amount) {
  console.assert(amount <= balance, "precondition: withdraw", amount, "from", balance);
  balance -= amount;
}

withdraw(500);
console.log("balance", balance);
```

```output
balance -400
```
