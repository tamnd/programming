```javascript run id=dcl
// A lock for async tasks: each body waits for the one before it.
class TaskMutex {
  tail = Promise.resolve();
  async run(body) {
    const previous = this.tail;
    let release;
    this.tail = new Promise((resolve) => (release = resolve));
    await previous;
    try {
      return await body();
    } finally {
      release();
    }
  }
}

class Config {
  static created = 0;
  constructor() {
    Config.created++;
  }
}

let instance;
const lock = new TaskMutex();
let lockedCalls = 0;

async function getInstance() {
  if (instance !== undefined) return instance; // first check, no lock
  return lock.run(async () => {
    lockedCalls++;
    if (instance === undefined) {
      // second check, lock held
      await null; // the set-up is asynchronous
      instance = new Config();
    }
    return instance;
  });
}

async function main() {
  const task = async () => {
    for (let i = 0; i < 1000; i++) await getInstance();
  };
  await Promise.all(Array.from({ length: 8 }, task));
  console.log("instances created:", Config.created);
  console.log("calls:", 8 * 1000);
  console.log("calls that took the lock:", lockedCalls <= 8 ? "8 or fewer" : lockedCalls);
}

main();
```

```output
instances created: 1
calls: 8000
calls that took the lock: 8 or fewer
```

- Verdict: idiom. The tasks share one thread, so the race is between `await` points, not threads. The second check is still needed: without it, each task that waited for the lock would build its own instance.
