```javascript run id=shared
import { Worker } from "node:worker_threads";
import { once } from "node:events";

const shared = new Int32Array(new SharedArrayBuffer(4));
const source = `
const { workerData } = require("node:worker_threads");
for (let i = 0; i < 100000; i++) Atomics.add(workerData, 0, 1);
`;

const workers = [1, 2, 3, 4].map(() => new Worker(source, { eval: true, workerData: shared }));
await Promise.all(workers.map((worker) => once(worker, "exit")));
console.log(Atomics.load(shared, 0));

const old = Atomics.compareExchange(shared, 0, 400000, 0);
console.log(old, Atomics.load(shared, 0));
```

```output for=shared
400000
400000 0
```

- Verdict: native. Each worker has its own heap, and only a `SharedArrayBuffer` is shared. The `Atomics` methods read, write and update its elements atomically, with sequentially consistent order.
- `Atomics.wait` and `Atomics.notify` let a worker sleep until another worker changes a value. Browsers allow shared memory only on pages that are cross-origin isolated.
