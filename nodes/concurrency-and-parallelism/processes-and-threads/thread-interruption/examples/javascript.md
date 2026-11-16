```javascript run id=stop-a-thread
import { Worker } from "node:worker_threads";

// A stop flag in shared memory. The worker checks it and stops by itself.
const flag = new Int32Array(new SharedArrayBuffer(4));
const polite = new Worker(
  `
  const { workerData, parentPort } = require("node:worker_threads");
  const flag = new Int32Array(workerData);
  // Atomics.wait sleeps for up to 10 s, but Atomics.notify wakes it early.
  while (Atomics.load(flag, 0) === 0) Atomics.wait(flag, 0, 0, 10000);
  parentPort.postMessage("worker: stop requested, cleaning up");
  `,
  { eval: true, workerData: flag.buffer },
);
polite.on("message", (m) => console.log(m));
await new Promise((resolve) => setTimeout(resolve, 100));
const start = performance.now();
Atomics.store(flag, 0, 1);
Atomics.notify(flag, 0);
await new Promise((resolve) => polite.once("exit", resolve));
console.log("worker stopped in under 1 s:", performance.now() - start < 1000);

// A worker in an endless loop never checks a flag. terminate() stops it from outside.
const busy = new Worker("for (;;) {}", { eval: true });
await new Promise((resolve) => busy.once("online", resolve));
const code = await busy.terminate();
console.log("busy worker terminated, exit code:", code);
```

```output for=stop-a-thread
worker: stop requested, cleaning up
worker stopped in under 1 s: true
busy worker terminated, exit code: 1
```

- Verdict: stdlib. Node.js has two ways to stop a worker. The worker can check a flag and stop by itself, or the main thread can call `worker.terminate()`.
- The first worker sleeps in `Atomics.wait`. The main thread sets the flag and calls `Atomics.notify`, so the worker wakes at once, posts a message and ends.
- `terminate()` will "Stop all JavaScript execution in the worker thread as soon as possible". It returns "a Promise for the exit code", which is 1 here.
- Workers share memory only "by transferring ArrayBuffer instances or sharing SharedArrayBuffer instances". So a forced stop cannot leave the other objects of the main thread half updated, but data in a `SharedArrayBuffer` can be.
