```typescript run id=producer-consumer
class BoundedBuffer {
  capacity: number;
  items: number[] = [];
  waitingPutters: (() => void)[] = []; // resolvers of producers that wait for space
  waitingTakers: (() => void)[] = []; // resolvers of consumers that wait for an item
  closed = false;

  constructor(capacity: number) {
    this.capacity = capacity;
  }

  async put(item: number): Promise<void> {
    while (this.items.length >= this.capacity) {
      await new Promise<void>((resolve) => this.waitingPutters.push(resolve));
    }
    this.items.push(item);
    this.waitingTakers.shift()?.();
  }

  async take(): Promise<number | undefined> {
    while (this.items.length === 0) {
      if (this.closed) return undefined; // the stream has ended
      await new Promise<void>((resolve) => this.waitingTakers.push(resolve));
    }
    const item = this.items.shift();
    this.waitingPutters.shift()?.();
    return item;
  }

  close(): void {
    this.closed = true;
    for (const wake of this.waitingTakers.splice(0)) wake();
  }
}

async function main(): Promise<void> {
  const buffer = new BoundedBuffer(4);
  const consumed: number[] = [];

  async function producer(start: number): Promise<void> {
    for (let item = start; item < start + 500; item++) await buffer.put(item);
  }

  async function consumer(): Promise<void> {
    for (;;) {
      const item = await buffer.take();
      if (item === undefined) return;
      consumed.push(item);
    }
  }

  const consumers = [consumer(), consumer()];
  await Promise.all([producer(0), producer(500)]);
  buffer.close(); // the end of the stream for every consumer
  await Promise.all(consumers);

  consumed.sort((a, b) => a - b);
  const once = consumed.length === 1000 && consumed.every((x, i) => x === i);
  console.log("items consumed:", consumed.length);
  console.log("sum of items:", consumed.reduce((a, b) => a + b, 0));
  console.log("each item consumed once:", once ? "True" : "False");
}

main();
```

```output
items consumed: 1000
sum of items: 499500
each item consumed once: True
```

- Verdict: idiom. The producers and consumers are async functions on one event loop, and `BoundedBuffer` keeps the resolvers of the ones that wait for space or for an item.
