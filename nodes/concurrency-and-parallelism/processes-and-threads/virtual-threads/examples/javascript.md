```javascript run id=async-limit
const TASKS = 1_000;
const LIMIT = 10;

let active = 0;
let peak = 0;
const waiting = [];

// A small semaphore: acquire resolves when a permit is free.
function acquire() {
  if (active < LIMIT) {
    active++;
    return Promise.resolve();
  }
  return new Promise((resolve) => waiting.push(resolve));
}

function release() {
  const next = waiting.shift();
  if (next) {
    next(); // pass the permit on; active stays the same
  } else {
    active--;
  }
}

async function work(n) {
  await acquire();
  try {
    peak = Math.max(peak, active);
    await new Promise((resolve) => setTimeout(resolve, 10));
    return n;
  } finally {
    release();
  }
}

const results = await Promise.all(Array.from({ length: TASKS }, (_, n) => work(n)));
console.log(`tasks: ${results.length}, sum: ${results.reduce((a, b) => a + b, 0)}`);
console.log(`never more than 10 at once: ${peak <= LIMIT}`);
```

```output for=async-limit
tasks: 1000, sum: 499500
never more than 10 at once: true
```

- Verdict: absent. JavaScript has no virtual threads. `async` functions on the event loop are the alternative: one thread runs all of them, and each `await` lets another function run.
- The standard library has no semaphore, so the example writes a small one. `acquire` waits when ten tasks are active, and `release` passes the permit to the next waiting task.
- All tasks run on one thread, so the counters need no lock.
- `Promise.all` waits for all tasks and keeps the results in order.
