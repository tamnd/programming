```javascript run id=unref
import { Worker } from "node:worker_threads";

const source = `
const { parentPort } = require("node:worker_threads");
parentPort.postMessage(6 * 7);
`;

const worker = new Worker(source, { eval: true });
let answer;
worker.on("message", (value) => {
  answer = value;
});
worker.on("exit", (code) => {
  console.log(`worker exit code: ${code}, answer: ${answer}`);

  const forever = new Worker("setInterval(() => {}, 1000);", { eval: true });
  forever.unref();
  console.log("main returns; the unref'd worker does not keep Node.js alive");
});
```

```output for=unref
worker exit code: 0, answer: 42
main returns; the unref'd worker does not keep Node.js alive
```

- Verdict: stdlib. A Node.js `Worker` has no blocking join. The `'exit'` event plays the part of a join: it fires after the worker has stopped, with its exit code.
- The worker sends its result with `postMessage`, and the main thread keeps it until the `'exit'` event.
- The second worker runs a timer, so it never ends by itself. `unref()` lets the process exit while it runs, as a detach would.
- `worker.terminate()` stops a worker from the main thread.
