```javascript run id=threads-and-the-gil
import { Worker } from "node:worker_threads";

// Each worker is a separate thread with its own JavaScript engine instance, so there is no lock
// that all threads share. The workers share only the memory of a SharedArrayBuffer.
const shared = new SharedArrayBuffer(4);
const total = new Int32Array(shared);

const workerCode = `
  const { workerData } = require("node:worker_threads");
  const total = new Int32Array(workerData);
  for (let i = 0; i < 100000; i++) Atomics.add(total, 0, 1);
`;

await Promise.all(
  Array.from({ length: 4 }, () =>
    new Promise((resolve, reject) => {
      const worker = new Worker(workerCode, { eval: true, workerData: shared });
      worker.once("exit", resolve);
      worker.once("error", reject);
    }),
  ),
);
console.log("4 workers added 100000 each with Atomics.add");
console.log("total:", Atomics.load(total, 0));
```

```output for=threads-and-the-gil
4 workers added 100000 each with Atomics.add
total: 400000
```

- Verdict: absent. JavaScript has no GIL. One thread runs the code of one engine instance, and a `Worker` is "an independent JavaScript execution thread". So there is no lock to share.
- Workers can run CPU-bound code in parallel. Node.js says that workers "are useful for performing CPU-intensive JavaScript operations".
- The workers share memory only through a `SharedArrayBuffer`. `Atomics.add` changes the shared total as one indivisible step, so no update is lost.
