```typescript run id=hsha
import { Worker } from "node:worker_threads";

// Sync layer: each worker thread runs one request at a time with a blocking call.
const workerCode = `
const { parentPort } = require("node:worker_threads");
const pause = new Int32Array(new SharedArrayBuffer(4));
parentPort.on("message", (request) => {
  if (request === null) {
    parentPort.close();
    return;
  }
  Atomics.wait(pause, 0, 0, 1); // a blocking call, such as a database query
  parentPort.postMessage(request);
});
parentPort.postMessage("ready");
`;

const requests: number[] = []; // the queueing layer, kept by the main thread
const idle: Worker[] = [];
let gateOpen = false; // lets the demo hold the sync layer
let received = 0;
let done = 0;
let sum = 0;

function dispatch(): void {
  while (gateOpen && idle.length > 0) {
    const worker = idle.shift();
    worker.postMessage(requests.length > 0 ? requests.shift() : null);
  }
}

const workers: Promise<number>[] = [];
for (let i = 0; i < 4; i++) {
  const worker = new Worker(workerCode, { eval: true });
  worker.on("message", (message: number | "ready") => {
    if (message !== "ready") {
      done += 1;
      sum += message;
    }
    idle.push(worker);
    dispatch();
  });
  workers.push(new Promise<number>((resolve) => worker.on("exit", resolve)));
}

// Async layer: the event loop calls a short handler per event.
function onEvent(eventId: number): void {
  received += 1;
  requests.push(eventId); // never blocks the event loop
}

let next = 0;
function eventSource(): void {
  if (next < 100) {
    onEvent(next++);
    setImmediate(eventSource);
    return;
  }
  console.log("events received by the async layer:", received);
  console.log("requests in the queueing layer before the sync layer starts:", requests.length);
  gateOpen = true;
  dispatch();
  Promise.all(workers).then(() => {
    console.log("requests done by the sync layer:", done);
    console.log("sum of request ids:", sum);
  });
}

eventSource();
```

```output
events received by the async layer: 100
requests in the queueing layer before the sync layer starts: 100
requests done by the sync layer: 100
sum of request ids: 4950
```

- Verdict: idiom. The main thread runs the event loop and must not block, so it keeps the queue and hands one request to each idle worker thread. The worker code blocks with `Atomics.wait`.
