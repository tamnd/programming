```javascript run id=class-fields-and-methods
"use strict";

class Account {
  static rate = 0.02; // static fields: one value, stored on the class
  static opened = 0;
  #pin; // a private instance field

  constructor(owner, balance = 0, pin = "0000") {
    this.owner = owner; // public instance fields, one set per object
    this.balance = balance;
    this.#pin = pin;
    Account.opened++;
  }

  deposit(amount) {
    this.balance += amount;
    return this;
  }

  interest() { return this.balance * Account.rate; }

  checkPin(pin) { return pin === this.#pin; }
}

const a = new Account("ana", 100, "1234");
const b = new Account("bo");
a.deposit(50);
console.log(a.balance, b.balance, Account.opened);
console.log(a.interest(), a.checkPin("1234"));
console.log(typeof Account, Object.getPrototypeOf(a) === Account.prototype);
console.log(a.deposit === b.deposit); // methods live once, on the prototype
```

```output for=class-fields-and-methods
150 0 2
3 true
function true
true
```

```javascript wrong id=detached-method
"use strict";

class Counter {
  count = 0;

  increment() { this.count++; }
}

const c = new Counter();
const inc = c.increment; // the method, without its object
try {
  inc(); // this is undefined inside the call
} catch (e) {
  console.log(`${e.name}: ${e.message}`);
}
const bound = c.increment.bind(c);
bound();
console.log(c.count);
```

```output for=detached-method
TypeError: Cannot read properties of undefined (reading 'count')
1
```

- Verdict: native. A `class` declaration creates a constructor function, and its methods go on the function's `prototype` object, so all instances share one copy of each method. Fields declared in the class body belong to each instance, `static` fields belong to the class, and fields whose names start with `#` are private: code outside the class body cannot read them. `typeof Account` is `"function"`, because a class is a function with special rules for calls.
- The wrong block reads a method from an object and calls it alone. The value of `this` depends on the form of the call, not on where the method came from, so the call `inc()` has no object, and `this` is `undefined`. Bind the method with `bind`, or wrap the call in an arrow function, before passing it as a callback.
