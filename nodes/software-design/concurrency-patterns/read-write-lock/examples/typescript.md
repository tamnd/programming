```typescript run id=read-write-lock
// One event loop runs these tasks. Each await is a point where another task can run.
class ReadWriteLock {
  readers: number;
  writer: boolean;
  waitingWriters: number;
  waiters: Array<() => void>;

  constructor() {
    this.readers = 0; // tasks that hold the read lock
    this.writer = false; // true while a task holds the write lock
    this.waitingWriters = 0;
    this.waiters = [];
  }
  wait(): Promise<void> {
    return new Promise<void>((resolve) => this.waiters.push(resolve));
  }
  wakeAll(): void {
    const waiters = this.waiters;
    this.waiters = [];
    for (const resolve of waiters) resolve();
  }
  async acquireRead(): Promise<void> {
    while (this.writer || this.waitingWriters > 0) await this.wait();
    this.readers++;
  }
  releaseRead(): void {
    this.readers--;
    if (this.readers === 0) this.wakeAll();
  }
  async acquireWrite(): Promise<void> {
    this.waitingWriters++;
    while (this.writer || this.readers > 0) await this.wait();
    this.waitingWriters--;
    this.writer = true;
  }
  releaseWrite(): void {
    this.writer = false;
    this.wakeAll();
  }
}

const tick = (): Promise<void> => new Promise<void>((resolve) => setImmediate(resolve));
const lock = new ReadWriteLock();
const pair: number[] = [0, 0]; // the writer keeps the two values equal
const counts = { reads: 0, mixed: 0, writes: 0, overlap: 0, inside: 0 };

// Part 1: two readers hold the read lock at the same time.
let atMeeting: number = 0;
async function meet(): Promise<boolean> {
  await lock.acquireRead();
  atMeeting++;
  for (let i = 0; i < 1000 && atMeeting < 2; i++) await tick();
  const met = atMeeting === 2;
  lock.releaseRead();
  return met;
}

// Part 2: readers and a writer share the pair.
async function reader(): Promise<void> {
  for (let n = 0; n < 1000; n++) {
    await lock.acquireRead();
    counts.inside++;
    const x = pair[0];
    await tick(); // give the writer a chance to run here
    const y = pair[1];
    counts.inside--;
    counts.reads++;
    if (x !== y) counts.mixed++;
    lock.releaseRead();
  }
}

async function writer(): Promise<void> {
  for (let i = 1; i <= 100; i++) {
    await lock.acquireWrite();
    if (counts.inside > 0) counts.overlap++;
    pair[0] = i;
    await tick();
    pair[1] = i;
    counts.writes++;
    lock.releaseWrite();
  }
}

async function main(): Promise<void> {
  const met = await Promise.all([meet(), meet()]);
  await Promise.all([reader(), reader(), reader(), reader(), writer()]);
  console.log("two readers inside at once:", met.every((m) => m) ? "True" : "False");
  console.log("reads:", counts.reads);
  console.log("reads that saw a mixed pair:", counts.mixed);
  console.log("writes:", counts.writes);
  console.log("writes that ran with a reader inside:", counts.overlap);
}

main();
```

```output
two readers inside at once: True
reads: 4000
reads that saw a mixed pair: 0
writes: 100
writes that ran with a reader inside: 0
```

- Verdict: idiom. The tasks run on one event loop, and the `ReadWriteLock` class keeps the waiting tasks as promises that it resolves when the lock is released.
