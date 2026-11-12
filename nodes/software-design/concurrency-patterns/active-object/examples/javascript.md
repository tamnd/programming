```javascript run id=active
const { Worker } = require("node:worker_threads");

// The servant and the scheduler run in a worker thread. The worker keeps
// the activation queue and starts to run it when it gets "start".
const schedulerCode = `
const { parentPort } = require("node:worker_threads");
const servant = { total: 0 };
const methods = {
  deposit: (amount) => (servant.total += amount),
  balance: () => servant.total,
};
const activationQueue = [];
let started = false;
const run = () => {
  while (activationQueue.length > 0) {
    const { id, name, args } = activationQueue.shift();
    parentPort.postMessage({ id, result: methods[name](...args) });
  }
};
parentPort.on("message", (message) => {
  if (message === "start") started = true;
  else activationQueue.push(message);
  if (started) run();
});
`;

// The proxy: each method makes a request and returns a future at once.
class AccountProxy {
  worker = new Worker(schedulerCode, { eval: true });
  pending = new Map();
  nextId = 0;

  constructor() {
    this.worker.on("message", ({ id, result }) => {
      this.pending.get(id)(result);
      this.pending.delete(id);
    });
  }

  send(name, ...args) {
    const id = this.nextId++;
    const future = { promise: Promise.resolve(0), done: false };
    future.promise = new Promise((resolve) => {
      this.pending.set(id, (result) => {
        future.done = true;
        resolve(result);
      });
    });
    this.worker.postMessage({ id, name, args });
    return future;
  }

  deposit(amount) {
    return this.send("deposit", amount);
  }

  balance() {
    return this.send("balance");
  }

  start() {
    this.worker.postMessage("start");
  }

  stop() {
    return this.worker.terminate();
  }
}

async function main() {
  const account = new AccountProxy();
  const client = () => Array.from({ length: 250 }, () => account.deposit(1));
  const futures = [client(), client(), client(), client()].flat();
  await new Promise((resolve) => setTimeout(resolve, 100)); // the worker gets the requests
  console.log("deposits queued:", futures.length);
  console.log("deposits done before the scheduler starts:", futures.filter((f) => f.done).length);
  account.start();
  console.log("balance after 1000 deposits:", await account.balance().promise);
  await account.stop();
}

main();
```

```output
deposits queued: 1000
deposits done before the scheduler starts: 0
balance after 1000 deposits: 1000
```

- Verdict: library. `worker_threads` gives the servant its own thread, and a promise is the future. The four clients are functions on the main thread, because the main thread is the only client thread.
