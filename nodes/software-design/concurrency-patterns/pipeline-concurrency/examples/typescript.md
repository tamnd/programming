```typescript run id=pipeline-concurrency
// A bounded channel for async tasks: send waits while the buffer is full.
class Channel<T> {
  capacity: number;
  buf: T[];
  closed: boolean;
  waiters: (() => void)[];

  constructor(capacity: number) {
    this.capacity = capacity;
    this.buf = [];
    this.closed = false;
    this.waiters = []; // tasks that wait for a change
  }

  wake(): void {
    const w = this.waiters;
    this.waiters = [];
    w.forEach((r) => r());
  }

  async send(v: T): Promise<void> {
    while (this.buf.length >= this.capacity) await new Promise<void>((r) => this.waiters.push(r));
    this.buf.push(v);
    this.wake();
  }

  async receive(): Promise<T | undefined> {
    // Returns undefined once the channel is closed and drained.
    while (this.buf.length === 0 && !this.closed) await new Promise<void>((r) => this.waiters.push(r));
    if (this.buf.length === 0) return undefined;
    const v = this.buf.shift();
    this.wake();
    return v;
  }

  close(): void {
    this.closed = true;
    this.wake();
  }
}

const ITEMS = 20;
const WORKERS = 3;
const numbers = new Channel<number>(1); // source -> square workers
const squares = new Channel<number>(1); // square workers -> sink
let sinkStarted = false;
let overlap = false;

async function source(): Promise<void> {
  for (let n = 1; n <= ITEMS; n++) await numbers.send(n);
  // Each stage can hold only a few values, so the sink must already be running.
  overlap = sinkStarted;
  numbers.close();
}

async function square(): Promise<void> {
  let n: number | undefined;
  while ((n = await numbers.receive()) !== undefined) await squares.send(n * n);
}

async function main(): Promise<void> {
  const stages: Promise<void>[] = [source()];
  const workers = Array.from({ length: WORKERS }, () => square()); // fan-out
  // The output closes when all workers are done: fan-in.
  stages.push(Promise.all(workers).then(() => squares.close()));

  let received = 0;
  let total = 0;
  let v: number | undefined;
  while ((v = await squares.receive()) !== undefined) {
    sinkStarted = true;
    received++;
    total += v;
  }
  await Promise.all(stages);

  console.log("items from the source:", ITEMS);
  console.log("square workers:", WORKERS);
  console.log("squares that reached the sink:", received);
  console.log("sum of squares:", total);
  console.log("the sink got a value before the source sent its last:", overlap ? "True" : "False");
}

main();
```

```output
items from the source: 20
square workers: 3
squares that reached the sink: 20
sum of squares: 2870
the sink got a value before the source sent its last: True
```

- Verdict: idiom. A small generic `Channel<T>` class for async tasks on one event loop. It shows the stages and the backpressure, not parallel work.
