```javascript run id=null-logger
class ConsoleLogger {
  log(message) {
    console.log(`log: ${message}`);
  }
}

class NullLogger {
  log(message) {} // do nothing
}

class Checkout {
  constructor(logger) {
    this.logger = logger;
  }

  pay(amount) {
    this.logger.log(`paid ${amount}`); // no check for null
    return `receipt: ${amount}`;
  }
}

console.log(new Checkout(new ConsoleLogger()).pay(10));
console.log(new Checkout(new NullLogger()).pay(20));
```

```output
log: paid 10
receipt: 10
receipt: 20
```

- Verdict: idiom. Any object with a `log` method can be the logger, so `NullLogger` is a class with an empty method.
