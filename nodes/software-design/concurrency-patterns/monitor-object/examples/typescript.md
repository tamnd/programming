```typescript run id=monitor
// One thread runs every job to completion, so the methods need no lock.
// A method that cannot go on awaits a promise, which plays the condition.
class Condition {
  private waiters: Array<() => void> = [];

  wait(): Promise<void> {
    return new Promise((resolve) => this.waiters.push(resolve));
  }
  notify(): void {
    const resolve = this.waiters.shift();
    if (resolve) resolve();
  }
}

class MessageQueue {
  capacity: number;
  items: number[] = [];
  notFull = new Condition();
  notEmpty = new Condition();
  puts = 0;
  gets = 0;
  largest = 0;

  constructor(capacity: number) {
    this.capacity = capacity;
  }

  async put(message: number): Promise<void> {
    while (this.items.length === this.capacity) {
      await this.notFull.wait();
    }
    this.items.push(message);
    this.puts += 1;
    this.largest = Math.max(this.largest, this.items.length);
    this.notEmpty.notify();
  }

  async get(): Promise<number> {
    while (this.items.length === 0) {
      await this.notEmpty.wait();
    }
    const message = this.items.shift()!;
    this.gets += 1;
    this.notFull.notify();
    return message;
  }
}

async function main(): Promise<void> {
  const q = new MessageQueue(2);
  let sum = 0;
  const producer = async (p: number) => {
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

- Verdict: idiom. The language has no monitor construct here. Single-threaded jobs give the mutual exclusion, and the typed `Condition` class is a list of promise resolvers.
