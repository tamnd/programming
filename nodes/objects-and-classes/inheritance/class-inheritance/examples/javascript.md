```javascript run id=extends
class Account {
  #balance = 0; // private: a subclass cannot read it
  constructor(owner) { this.owner = owner; }
  deposit(n) { this.#balance += n; return this; }
  get balance() { return this.#balance; }
  describe() { return `${this.constructor.name} of ${this.owner}: ${this.balance}`; }
  static open(owner) { return new this(owner); } // static methods are inherited too
}

class Checking extends Account {
  constructor(owner, overdraft = 100) {
    super(owner); // must run before the constructor uses this
    this.overdraft = overdraft;
  }
  describe() { return `${super.describe()} (overdraft ${this.overdraft})`; }
}

const c = Checking.open("bo").deposit(50);
console.log(c.describe());
console.log(c instanceof Account,
  Object.getPrototypeOf(Checking.prototype) === Account.prototype,
  Object.getPrototypeOf(Checking) === Account);

class Stack extends Array { // a built-in class as the base
  peek() { return this.at(-1); }
}
const s = Stack.from([1, 2, 3]);
console.log(s.peek(), s.map((x) => x * 2) instanceof Stack);
```

```output for=extends
Checking of bo: 50 (overdraft 100)
true true true
3 true
```

```javascript fails expect=exception:ReferenceError id=this-before-super
class Account {
  constructor(owner) { this.owner = owner; }
}

class Checking extends Account {
  constructor(owner) {
    this.overdraft = 100; // this does not exist before super() runs
    super(owner);
  }
}

console.log(new Checking("bo"));
```

- Verdict: native. `class Checking extends Account` sets the prototype of `Checking.prototype` to `Account.prototype`, so instances find the methods of the base along the prototype chain, and it sets the prototype of `Checking` to `Account`, so static methods are inherited as well. `new this(owner)` in `open` creates an object of the class on which it is called. Built-in classes such as `Array` can be extended, and `map` on a `Stack` returns a `Stack`.
- Private fields with `#` belong to the class that declares them, so `Checking` cannot read `#balance` and uses the getter. In the failing block, a derived constructor uses `this` before it calls `super()`, which is a `ReferenceError`, because the base constructor creates the object.
