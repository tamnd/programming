```javascript run id=proxy
const account = {
  balance: 100,
  deposit(n) { this.balance += n; },
  withdraw(n) { this.balance -= n; },
  report() { return `balance ${this.balance}`; },
};

const pointcut = /^(deposit|withdraw)$/;
const advised = new Proxy(account, {
  get(target, name, receiver) {
    const value = Reflect.get(target, name, receiver);
    if (typeof value !== "function" || !pointcut.test(name)) return value;
    return function (...args) {
      console.log(`before ${name}(${args})`);
      const result = value.apply(this, args);
      console.log(`after ${name}: balance ${target.balance}`);
      return result;
    };
  },
});

advised.deposit(50);
advised.withdraw(30);
console.log(advised.report());
```

```output for=proxy
before deposit(50)
after deposit: balance 150
before withdraw(30)
after withdraw: balance 120
balance 120
```

- Verdict: stdlib. A `Proxy` intercepts every property access on an object, so the `get` trap can wrap the methods that a pattern selects. `Reflect.get` performs the normal access. Code that holds a reference to the original object, and not to the proxy, does not see the advice.
