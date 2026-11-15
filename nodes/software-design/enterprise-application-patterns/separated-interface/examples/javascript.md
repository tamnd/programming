```javascript run id=notifier-interface
// ---- module domain: it describes the interface that it needs, and imports nothing below.
// JavaScript has no interface types; the interface is the method send(customer, text).

class OrderService {
  constructor(notifier) {
    if (typeof notifier.send !== "function") throw new TypeError("notifier needs send()");
    this.notifier = notifier;
  }

  confirm(number, customer) {
    this.notifier.send(customer, `order ${number} is confirmed`);
  }
}

// ---- module infrastructure: it implements the interface of the domain.

class EmailNotifier {
  send(customer, text) {
    console.log(`email to ${customer}: ${text}`);
  }
}

// ---- module tests: another implementation of the same interface.

class RecordingNotifier {
  constructor() {
    this.sent = [];
  }

  send(customer, text) {
    this.sent.push(`${customer}: ${text}`);
  }
}

// ---- main: the only code that knows both the domain and the implementations.

new OrderService(new EmailNotifier()).confirm(7, "ann");

const recorder = new RecordingNotifier();
new OrderService(recorder).confirm(8, "bob");
console.log("the test notifier recorded:", recorder.sent.join("; "));
```

```output
email to ann: order 7 is confirmed
the test notifier recorded: bob: order 8 is confirmed
```

- Verdict: idiom. JavaScript has no interface types, so the interface is a method name and its arguments. The constructor of `OrderService` checks that the notifier has a `send` method, so that a wrong object fails at once. TypeScript can write the interface as a type.
