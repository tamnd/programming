```typescript run id=active
import { Worker } from "node:worker_threads";

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

type Future = { promise: Promise<number>; done: boolean };

// The proxy: each method makes a request and returns a future at once.
class AccountProxy {
  private worker = new Worker(schedulerCode, { eval: true });
  private pending = new Map<number, (result: number) => void>();
  private nextId = 0;

  constructor() {
    this.worker.on("message", ({ id, result }: { id: number; result: number }) => {
      this.pending.get(id)!(result);
      this.pending.delete(id);
    });
  }

  private send(name: string, ...args: number[]): Future {
    const id = this.nextId++;
    const future: Future = { promise: Promise.resolve(0), done: false };
    future.promise = new Promise<number>((resolve) => {
      this.pending.set(id, (result) => {
        future.done = true;
        resolve(result);
      });
    });
    this.worker.postMessage({ id, name, args });
    return future;
  }

  deposit(amount: number): Future {
    return this.send("deposit", amount);
  }

  balance(): Future {
    return this.send("balance");
  }

  start(): void {
    this.worker.postMessage("start");
  }

  stop(): Promise<number> {
    return this.worker.terminate();
  }
}

async function main(): Promise<void> {
  const account = new AccountProxy();
  const client = (): Future[] => Array.from({ length: 250 }, () => account.deposit(1));
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
