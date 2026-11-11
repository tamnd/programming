```javascript run id=object-literals-and-messages
const account = {
  owner: "ana",
  balance: 100,
  deposit(amount) {
    this.balance += amount;
    return this;
  },
};
account.deposit(50).deposit(25);
console.log(account.owner, account.balance);

account.close = function () {
  return `${this.owner} closed`;
};
console.log(account.close(), Object.keys(account).join(","));

const message = "deposit";
account[message](5); // a message chosen at run time
console.log(account.balance);

const logged = new Proxy(account, {
  get(target, name) {
    if (!(name in target)) return () => `does not understand: ${String(name)}`;
    return target[name];
  },
});
console.log(logged.withdraw());

console.log(typeof 5, typeof account, (5).toFixed(1));
```

```output for=object-literals-and-messages
ana 175
ana closed owner,balance,deposit,close
180
does not understand: withdraw
number object 5.0
```

```javascript wrong id=property-on-primitive
let label = "total";
label.unit = "EUR"; // sets a property on a temporary wrapper object
console.log(label.unit);
const boxed = new String("total");
boxed.unit = "EUR";
console.log(boxed.unit);
```

```output for=property-on-primitive
undefined
EUR
```

- Verdict: native. A JavaScript object is a collection of properties, and an object literal creates one directly, with no class. A method is a property whose value is a function, and `this` names the object through which it was called. A program can add properties at any time, and `account[message]` sends a message whose name is in a variable. A `Proxy` can catch every property access, including names that the object lacks.
- Numbers, strings and booleans are primitives, not objects. When code reads a property of a primitive, JavaScript wraps it in a temporary object, which is why `(5).toFixed(1)` works.
- The wrong block sets a property on a string. The assignment goes to a temporary wrapper that is thrown away at once, so the next read finds nothing. In sloppy code the assignment fails with no error, and in strict mode and modules it throws a `TypeError`. Store extra data in an object, not on a primitive.
