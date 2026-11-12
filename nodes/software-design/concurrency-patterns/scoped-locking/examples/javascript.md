```javascript run id=guard
// One thread, but awaits interleave the workers, so a lock still matters.
class Lock {
  constructor() {
    this.tail = Promise.resolve();
    this.held = false;
  }
  async withLock(body) {
    const previous = this.tail;
    let release;
    this.tail = new Promise((resolve) => (release = resolve));
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
const table = new Map();

function increment(path) {
  return lock.withLock(async () => {
    if (!path) return -1;
    const count = table.get(path) ?? 0;
    await null; // a pause inside the critical section
    table.set(path, count + 1);
    return 0;
  });
}

async function worker() {
  for (let i = 0; i < 1000; i++) await increment("/index");
}

async function main() {
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

- Verdict: idiom. There is no lock in the language, so the example builds one from promises. `withLock` releases the lock in `finally`, which is the scoped form.
