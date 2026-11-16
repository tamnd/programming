```javascript run id=per-thread-values
import { AsyncLocalStorage } from "node:async_hooks";
import { Worker } from "node:worker_threads";

// Each thread has its own global object. A worker does not see the globals of the main thread.
function bump(times) {
  globalThis.counter ??= 0;
  for (let i = 0; i < times; i++) globalThis.counter += 1;
  return globalThis.counter;
}

const mainCount = bump(3); // the main thread counts first

const workerCode = `
  const { parentPort, workerData } = require("node:worker_threads");
  ${bump.toString()}
  parentPort.postMessage(bump(workerData));
`;

function runWorker(times) {
  return new Promise((resolve, reject) => {
    const worker = new Worker(workerCode, { eval: true, workerData: times });
    worker.once("message", resolve);
    worker.once("error", reject);
  });
}

const [one, two] = await Promise.all([runWorker(1), runWorker(2)]);
console.log(`main: counter = ${mainCount}`);
console.log(`worker 1: counter = ${one}`);
console.log(`worker 2: counter = ${two}`);

// On one thread, AsyncLocalStorage keeps a value for one chain of calls and callbacks.
const storage = new AsyncLocalStorage();
const show = (text) => console.log(text, storage.getStore()?.user ?? "undefined");
await storage.run({ user: "alice" }, async () => {
  await Promise.resolve();
  show("the task with the value sees user:");
});
show("a later task on the same thread sees user:");
```

```output for=per-thread-values
main: counter = 3
worker 1: counter = 1
worker 2: counter = 2
the task with the value sees user: alice
a later task on the same thread sees user: undefined
```

- Verdict: absent. JavaScript has no thread-local variables, because threads do not share variables at all. Each worker thread has its own global object, so the workers do not see the counter of the main thread.
- The worker code gets the same `bump` function as source text. The workers count from 0, though the main thread counted to 3 first.
- On one thread, Node.js has `AsyncLocalStorage`, which keeps a value for one chain of calls, callbacks and promises. It is "similar to thread-local storage in other languages".
- The value set by `run` is still there after `await`, but code outside `run` does not see it.
