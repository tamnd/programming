```javascript run id=producer-consumer
class BoundedBuffer {
  constructor(capacity) {
    this.capacity = capacity;
    this.items = [];
    this.waitingPutters = []; // resolvers of producers that wait for space
    this.waitingTakers = []; // resolvers of consumers that wait for an item
    this.closed = false;
  }

  async put(item) {
    while (this.items.length >= this.capacity) {
      await new Promise((resolve) => this.waitingPutters.push(resolve));
    }
    this.items.push(item);
    this.waitingTakers.shift()?.();
  }

  async take() {
    while (this.items.length === 0) {
      if (this.closed) return undefined; // the stream has ended
      await new Promise((resolve) => this.waitingTakers.push(resolve));
    }
    const item = this.items.shift();
    this.waitingPutters.shift()?.();
    return item;
  }

  close() {
    this.closed = true;
    for (const wake of this.waitingTakers.splice(0)) wake();
  }
}

async function main() {
  const buffer = new BoundedBuffer(4);
  const consumed = [];

  async function producer(start) {
    for (let item = start; item < start + 500; item++) await buffer.put(item);
  }

  async function consumer() {
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
