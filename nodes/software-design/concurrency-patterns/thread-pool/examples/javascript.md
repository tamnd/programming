```javascript run id=thread-pool
const { Worker } = require("node:worker_threads");

// Each worker thread runs one task at a time.
const workerCode = `
const { parentPort, threadId } = require("node:worker_threads");
const pause = new Int32Array(new SharedArrayBuffer(4));
parentPort.on("message", (n) => {
  if (n === null) {
    parentPort.close();
    return;
  }
  Atomics.wait(pause, 0, 0, 1); // a short blocking call
  parentPort.postMessage({ threadId, result: n * n });
});
parentPort.postMessage({ threadId, result: null });
`;

const tasks = Array.from({ length: 100 }, (_, n) => n); // the work queue
const idle = [];
const workersUsed = new Set();
let running = 0;
let mostRunning = 0;
const results = [];

function dispatch() {
  while (idle.length > 0) {
    const worker = idle.shift();
    if (tasks.length > 0) {
      running += 1;
      mostRunning = Math.max(mostRunning, running);
      worker.postMessage(tasks.shift());
    } else {
      worker.postMessage(null);
    }
  }
}

const exits = [];
for (let i = 0; i < 4; i++) {
  const worker = new Worker(workerCode, { eval: true });
  worker.on("message", ({ threadId, result }) => {
    if (result !== null) {
      running -= 1;
      workersUsed.add(threadId);
      results.push(result);
    }
    idle.push(worker);
    dispatch();
  });
  exits.push(new Promise((resolve) => worker.on("exit", resolve)));
}

Promise.all(exits).then(() => {
  console.log("tasks run:", results.length);
  console.log("sum of results:", results.reduce((a, b) => a + b, 0));
  console.log("workers used, at most 4:", workersUsed.size <= 4 ? "True" : "False");
  console.log("most tasks running at once, at most 4:", mostRunning <= 4 ? "True" : "False");
});
```

```output
tasks run: 100
sum of results: 328350
workers used, at most 4: True
most tasks running at once, at most 4: True
```

- Verdict: idiom. The main thread keeps the work queue and hands each task to an idle `worker_threads` worker.
