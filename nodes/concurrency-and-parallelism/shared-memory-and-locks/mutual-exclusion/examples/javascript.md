```javascript run id=counter
import { Worker } from "node:worker_threads";

const source = `
const { workerData } = require("node:worker_threads");
const cells = new Int32Array(workerData.shared);
const LOCK = 0;
const COUNT = 1;

function lock() {
  while (Atomics.compareExchange(cells, LOCK, 0, 1) !== 0) {
    Atomics.wait(cells, LOCK, 1);
  }
}

function unlock() {
  Atomics.store(cells, LOCK, 0);
  Atomics.notify(cells, LOCK, 1);
}

for (let i = 0; i < workerData.times; i++) {
  lock();
  cells[COUNT] = cells[COUNT] + 1;
  unlock();
}
`;

const shared = new SharedArrayBuffer(2 * Int32Array.BYTES_PER_ELEMENT);

function runWorker(times) {
  return new Promise((resolve, reject) => {
    const worker = new Worker(source, { eval: true, workerData: { shared, times } });
    worker.once("exit", resolve);
    worker.once("error", reject);
  });
}

await Promise.all([1, 2, 3, 4].map(() => runWorker(10_000)));
console.log("total:", new Int32Array(shared)[1]);
```

```output
total: 40000
```

- Verdict: idiom. Code on one thread needs no lock, because nothing else runs between two statements. Workers run in parallel, and a `SharedArrayBuffer` gives them shared memory.
- The language has no mutex type. `Atomics.compareExchange` takes the lock, `Atomics.wait` parks the worker while the cell holds 1, and `Atomics.notify` wakes one waiter after the unlock. The plain read and write of `cells[COUNT]` are safe, because they happen between the atomic operations of the lock and the unlock.
- The main thread of a browser page may not call `Atomics.wait`. Node.js allows it. For critical sections that contain `await` on one thread, see the TypeScript example.
