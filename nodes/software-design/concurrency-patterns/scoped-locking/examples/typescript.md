```typescript run id=guard
class Lock {
  private tail: Promise<void> = Promise.resolve();
  held = false;
  async withLock<T>(body: () => Promise<T>): Promise<T> {
    const previous = this.tail;
    let release!: () => void;
    this.tail = new Promise<void>((resolve) => (release = resolve));
    await previous;
    this.held = true;
    try {
      return await body();
    } finally {
      this.held = false; // released on every way out
      release();
    }
  }
}

const lock = new Lock();
const table = new Map<string, number>();

function increment(path: string): Promise<number> {
  return lock.withLock(async () => {
    if (!path) return -1;
    const count = table.get(path) ?? 0;
    await null;
    table.set(path, count + 1);
    return 0;
  });
}

async function worker(): Promise<void> {
  for (let i = 0; i < 1000; i++) await increment("/index");
}

async function main(): Promise<void> {
  await Promise.all([worker(), worker(), worker(), worker()]);
  console.log("hits /index:", table.get("/index"));
  console.log("bad path returns", await increment(""));
  console.log("lock free after the early return:", lock.held ? "False" : "True");
  await increment("/index");
  console.log("hits /index:", table.get("/index"));
}

main();
```

```output
hits /index: 4000
bad path returns -1
lock free after the early return: True
hits /index: 4001
```

- Verdict: idiom. The generic `withLock<T>` keeps the type of the value that the critical section returns.
