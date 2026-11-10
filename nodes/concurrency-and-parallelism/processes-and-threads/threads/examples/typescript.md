```typescript run id=sums
import { Worker } from "node:worker_threads";

interface Job {
  start: number;
  stop: number;
}

const source = `
const { parentPort, workerData } = require("node:worker_threads");
let sum = 0;
for (let n = workerData.start; n < workerData.stop; n++) sum += n;
parentPort.postMessage(sum);
`;

function partialSum(job: Job): Promise<number> {
  return new Promise((resolve, reject) => {
    const worker = new Worker(source, { eval: true, workerData: job });
    worker.once("message", (sum: number) => resolve(sum));
    worker.once("error", reject);
  });
}

const jobs: Job[] = [0, 1, 2, 3].map((i) => ({ start: i * 250, stop: (i + 1) * 250 }));
const results: number[] = await Promise.all(jobs.map(partialSum));
console.log(results);
console.log("total:", results.reduce((a, b) => a + b, 0));
```

```output for=sums
[ 31125, 93625, 156125, 218625 ]
total: 499500
```

- Verdict: stdlib. TypeScript has the threads of its runtime. In Node.js, these are the workers of `node:worker_threads`.
- The type checker does not see into the worker. The message that comes back has the type `any`, so the code states its type, here `number`.
- The worker code is a string of JavaScript, so it gets no type checking. A worker can also load its own file.
