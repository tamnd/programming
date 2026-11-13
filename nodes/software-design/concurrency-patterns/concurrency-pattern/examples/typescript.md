```typescript run id=three-ways
// JavaScript runs these tasks on one thread, so they can interleave only at an await.
const TASKS = 4;
const STEPS = 1000;
const tick = (): Promise<void> => new Promise((resolve) => setImmediate(resolve));

// A lock for async tasks: lock() waits until no other task holds it.
class Mutex {
  private held = false;
  private waiters: (() => void)[] = [];

  async lock(): Promise<void> {
    while (this.held) await new Promise<void>((resolve) => this.waiters.push(resolve));
    this.held = true;
  }
  unlock(): void {
    this.held = false;
    const next = this.waiters.shift();
    if (next) next();
  }
}

// A channel with no bound: receive() waits until a value is there.
class Channel<T> {
  private values: T[] = [];
  private waiters: ((value: T) => void)[] = [];

  send(value: T): void {
    const next = this.waiters.shift();
    if (next) next(value);
    else this.values.push(value);
  }
  receive(): Promise<T> {
    if (this.values.length > 0) return Promise.resolve(this.values.shift()!);
    return new Promise<T>((resolve) => this.waiters.push(resolve));
  }
}

const runTasks = (task: (i: number) => Promise<void>): Promise<void[]> => Promise.all(Array.from({ length: TASKS }, (_, i) => task(i)));

async function main(): Promise<void> {
  // 1. Synchronization: a lock guards the shared counter across the await.
  let count = 0;
  const mutex = new Mutex();
  await runTasks(async () => {
    for (let n = 0; n < STEPS; n++) {
      await mutex.lock();
      const value = count;
      await tick(); // other tasks run here, but they wait for the lock
      count = value + 1;
      mutex.unlock();
    }
  });
  console.log("lock around a shared counter: " + count);

  // 2. Confinement: one owner task keeps the counter; the others send it messages.
  const inbox = new Channel<number>();
  const owner = (async () => {
    let total = 0;
    for (let n = 0; n < TASKS * STEPS; n++) total += await inbox.receive();
    return total;
  })();
  await runTasks(async () => {
    for (let n = 0; n < STEPS; n++) {
      inbox.send(1);
      await tick();
    }
  });
  console.log("one owner thread, the others send messages: " + (await owner));

  // 3. No sharing: each task makes its own result, and the results are combined at the end.
  const parts: number[] = new Array(TASKS).fill(0);
  await runTasks(async (i) => {
    for (let n = 0; n < STEPS; n++) {
      parts[i] += 1; // only task i writes slot i
      if (n % 100 === 0) await tick();
    }
  });
  console.log("own result for each thread, combined after the join: " + parts.reduce((a, b) => a + b, 0));
}

main();
```

```output
lock around a shared counter: 4000
one owner thread, the others send messages: 4000
own result for each thread, combined after the join: 4000
```

- Verdict: native. The same program as JavaScript, with types for the mutex, the channel and the tasks.
