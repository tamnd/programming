```javascript run id=private-fields
"use strict";

class Account {
  #balance = 0;  // a private field: only code inside the class body can name it
  #history = [];

  deposit(amount) {
    if (amount <= 0) throw new RangeError("deposit must be positive");
    this.#history.push(amount);
    this.#balance += amount;
  }

  get balance() { return this.#balance; }  // read-only from outside

  static isAccount(x) { return #balance in x; }  // a brand check
}

const a = new Account();
a.deposit(100);
try {
  a.balance = 5;  // a getter with no setter: a TypeError in strict code
} catch (err) {
  console.log(err.constructor.name);
}
console.log(a.balance, Object.keys(a), JSON.stringify(a));
console.log(Account.isAccount(a), Account.isAccount({ balance: 1 }));

function makeCounter() {  // privacy by closure, the older idiom
  let count = 0;
  return { increment: () => ++count };
}
const c = makeCounter();
c.increment();
console.log(c.increment(), c.count);
```

```output for=private-fields
TypeError
100 [] {}
true false
2 undefined
```

```javascript fails expect=exception:SyntaxError id=outside-access
class Account {
  #balance = 0;
}

const a = new Account();
console.log(a.#balance);  // private names are checked when the code is parsed
```

- Verdict: native. Class fields and methods whose names start with `#` are private: only code in the class body can name them, and the engine enforces this. Private fields do not appear in `Object.keys`, in `JSON.stringify` or through any form of reflection. The test `#balance in x` checks whether an object was made by the class. A getter with no setter makes a property read-only, and assignment to it throws a `TypeError` in strict code. Closures are the older way to hide state, and still a common one.
- The failing block reads `a.#balance` outside the class. The check happens when the engine parses the file, so the program fails with a `SyntaxError` before any line runs.
