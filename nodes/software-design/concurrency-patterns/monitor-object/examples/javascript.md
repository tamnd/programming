```javascript run id=monitor
// One thread runs every job to completion, so the methods need no lock.
// A method that cannot go on awaits a promise, which plays the condition.
class Condition {
  constructor() {
    this.waiters = [];
  }
  wait() {
    return new Promise((resolve) => this.waiters.push(resolve));
  }
  notify() {
    const resolve = this.waiters.shift();
    if (resolve) resolve();
  }
}

class MessageQueue {
  constructor(capacity) {
    this.capacity = capacity;
    this.items = [];
    this.notFull = new Condition();
    this.notEmpty = new Condition();
    this.puts = 0;
    this.gets = 0;
    this.largest = 0;
  }

  async put(message) {
    while (this.items.length === this.capacity) {
      await this.notFull.wait();
    }
    this.items.push(message);
    this.puts += 1;
    this.largest = Math.max(this.largest, this.items.length);
    this.notEmpty.notify();
  }

  async get() {
    while (this.items.length === 0) {
      await this.notEmpty.wait();
    }
    const message = this.items.shift();
    this.gets += 1;
    this.notFull.notify();
    return message;
  }
}

async function main() {
  const q = new MessageQueue(2);
  let sum = 0;
  const producer = async (p) => {
    for (let i = 0; i < 250; i++) await q.put(p * 250 + i);
  };
  const consumer = async () => {
    for (let i = 0; i < 500; i++) {
      const message = await q.get();
      sum += message;
    }
  };
  await Promise.all([0, 1, 2, 3].map(producer).concat([consumer(), consumer()]));
  console.log("messages put:", q.puts);
  console.log("messages got:", q.gets);
  console.log("sum of messages got:", sum);
  console.log("queue never held more than 2:", q.largest <= 2 ? "True" : "False");
}

main();
```

```output
messages put: 1000
messages got: 1000
sum of messages got: 499500
queue never held more than 2: True
```

- Verdict: idiom. The language has no monitor construct here. Single-threaded jobs give the mutual exclusion, and the `Condition` class is a list of promise resolvers.
