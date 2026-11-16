```javascript run id=private-and-closure
class Account {
  #balance; // a private element

  constructor(balance) {
    this.#balance = balance;
  }

  deposit(amount) {
    if (amount <= 0) throw new RangeError("amount must be positive");
    this.#balance += amount;
  }

  get balance() {
    return this.#balance;
  }
}

const a = new Account(10);
a.deposit(5);
console.log(a.balance, Object.keys(a).length, JSON.stringify(a));
// a.#balance; // SyntaxError outside the class

function makeCounter() {
  let count = 0; // only the returned functions can see this
  return {
    next: () => ++count,
    reset: () => { count = 0; },
  };
}

const c = makeCounter();
c.next();
console.log(c.next(), c.count, Object.keys(c).join(","));
```

```output for=private-and-closure
15 0 {}
2 undefined next,reset
```

- Verdict: idiom. A `#` field is private to the class body: other code cannot read it, and it does not show in `Object.keys` or in JSON. A closure gives the same hiding without a class: the variable `count` lives in the scope of `makeCounter`, and only the two returned functions can reach it.
