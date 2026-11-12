```typescript run id=null-logger
interface Logger {
  log(message: string): void;
}

class ConsoleLogger implements Logger {
  log(message: string): void {
    console.log(`log: ${message}`);
  }
}

class NullLogger implements Logger {
  log(_message: string): void {} // do nothing
}

class Checkout {
  private logger: Logger;

  constructor(logger: Logger) {
    this.logger = logger;
  }

  pay(amount: number): string {
    this.logger.log(`paid ${amount}`);
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

- Verdict: idiom. `NullLogger` implements the `Logger` interface with an empty method.
