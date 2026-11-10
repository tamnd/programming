```javascript run id=sums
import { Worker } from "node:worker_threads";

const source = `
const { parentPort, workerData } = require("node:worker_threads");
let sum = 0;
for (let n = workerData.start; n < workerData.stop; n++) sum += n;
parentPort.postMessage(sum);
`;

function partialSum(start, stop) {
  return new Promise((resolve, reject) => {
    const worker = new Worker(source, { eval: true, workerData: { start, stop } });
    worker.once("message", resolve);
    worker.once("error", reject);
  });
}

const jobs = [0, 1, 2, 3].map((i) => partialSum(i * 250, (i + 1) * 250));
const results = await Promise.all(jobs);
console.log(results);
console.log("total:", results.reduce((a, b) => a + b, 0));
```

```output for=sums
[ 31125, 93625, 156125, 218625 ]
total: 499500
```

- Verdict: stdlib. The language has no threads. Node.js gives `node:worker_threads`, and browsers give Web Workers.
- Each worker is an independent JavaScript thread with its own event loop. Workers exchange messages, which are copies. A `SharedArrayBuffer` lets them share memory.
- A worker has no `join`. The main thread waits for the `message` event, here wrapped in a promise. `Promise.all` keeps the order of the jobs.
- For waiting on I/O, use [[async-await]] on one thread. Workers are for work that uses the CPU.
