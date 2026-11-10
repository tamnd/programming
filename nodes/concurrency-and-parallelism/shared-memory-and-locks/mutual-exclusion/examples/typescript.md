On one thread, a critical section that contains `await` lets other tasks run in the middle of it. Both deposits here read the balance before either one writes.

```typescript wrong id=interleaved
let balance = 100;

async function deposit(amount: number): Promise<void> {
  const seen = balance;
  await new Promise((resolve) => setTimeout(resolve, 10));
  balance = seen + amount;
}

await Promise.all([deposit(50), deposit(50)]);
console.log("balance:", balance);
```

```output
balance: 150
```

An async mutex makes each task wait for the one before it. It suspends the task, not the thread.

```typescript run id=async-mutex
class AsyncMutex {
  private tail: Promise<void> = Promise.resolve();

  lock(): Promise<() => void> {
    let release: () => void = () => {};
    const done = new Promise<void>((resolve) => {
      release = resolve;
    });
    const ready = this.tail.then(() => release);
    this.tail = done;
    return ready;
  }
}

let balance = 100;
const balanceMutex = new AsyncMutex();

async function deposit(amount: number): Promise<void> {
  const unlock = await balanceMutex.lock();
  try {
    const seen = balance;
    await new Promise((resolve) => setTimeout(resolve, 10));
    balance = seen + amount;
  } finally {
    unlock();
  }
}

await Promise.all([deposit(50), deposit(50)]);
console.log("balance:", balance);
```

```output
balance: 200
```

- Verdict: idiom. TypeScript adds no lock type. Between workers, use `Atomics` as in the JavaScript example.
- The mutex keeps a chain of promises. Each `lock` call waits for the promise of the call before it, and `unlock` resolves its own promise.
- `finally` releases the mutex when the critical section throws.
