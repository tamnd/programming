```typescript run id=tss
import { Worker } from "node:worker_threads";

// Each worker runs this code in its own global scope, so each has its own lastError.
const workerCode = `
const { parentPort, workerData } = require("node:worker_threads");
let lastError = 0;
function fakeCall(code) {
  lastError = code; // the call sets this thread's error
  return -1;
}
const code = workerData + 1;
let wrongReads = 0;
for (let i = 0; i < 1000; i++) {
  if (fakeCall(code) === -1 && lastError !== code) wrongReads += 1;
}
parentPort.postMessage({ wrongReads, seen: lastError });
`;

type Result = { wrongReads: number; seen: number };

const runs = [0, 1, 2, 3].map(
  (index) =>
    new Promise<Result>((resolve) => {
      new Worker(workerCode, { eval: true, workerData: index }).on("message", resolve);
    }),
);
Promise.all(runs).then((results: Result[]) => {
  console.log("threads:", results.length);
  console.log("calls per thread:", 1000);
  console.log("reads that saw another thread's error:", results.reduce((n, r) => n + r.wrongReads, 0));
  console.log("error code seen by each thread: [" + results.map((r) => r.seen).join(", ") + "]");
});
```

```output
threads: 4
calls per thread: 1000
reads that saw another thread's error: 0
error code seen by each thread: [1, 2, 3, 4]
```

- Verdict: idiom. Each worker thread runs the worker code in its own global scope, so the top-level `lastError` of one worker is not visible to the others.
