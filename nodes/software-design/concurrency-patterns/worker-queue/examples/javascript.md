```javascript run id=worker-queue
const TASKS = 10;
const WORKERS = 3;
const FAILS_ON = 4; // the first worker that takes this task fails before it does the work

// Hands out tasks and keeps each one until a worker acknowledges it.
class WorkQueue {
  constructor() {
    this.ready = [];
    this.unacked = new Map(); // delivery tag -> task
    this.pending = 0; // tasks put and not yet acknowledged
    this.tag = 0;
    this.deliveries = 0;
    this.putBack = 0;
    this.waiters = [];
  }

  wake() {
    for (const w of this.waiters.splice(0)) w();
  }

  put(task) {
    this.ready.push(task);
    this.pending++;
    this.wake();
  }

  // Resolves to [tag, task], or null when every task is acknowledged.
  async get() {
    while (this.ready.length === 0 && this.pending > 0) {
      await new Promise((resolve) => this.waiters.push(resolve));
    }
    if (this.ready.length === 0) return null;
    this.tag++;
    this.deliveries++;
    this.unacked.set(this.tag, this.ready.shift());
    return [this.tag, this.unacked.get(this.tag)];
  }

  ack(tag) {
    this.unacked.delete(tag);
    this.pending--;
    this.wake();
  }

  // The worker went away without an ack, so the task goes back on the queue.
  workerLost(tag) {
    this.ready.push(this.unacked.get(tag));
    this.unacked.delete(tag);
    this.putBack++;
    this.wake();
  }
}

const work = new WorkQueue();
const results = new Map();
let failed = false;

async function worker() {
  let delivery;
  while ((delivery = await work.get()) !== null) {
    const [tag, task] = delivery;
    if (task === FAILS_ON && !failed) {
      failed = true;
      work.workerLost(tag); // this worker stops here
      return;
    }
    await new Promise((resolve) => setTimeout(resolve, 1)); // the work takes time
    results.set(task, task * task);
    work.ack(tag); // only after the work is done
  }
}

async function main() {
  for (let task = 1; task <= TASKS; task++) work.put(task);
  const workers = [];
  for (let i = 0; i < WORKERS; i++) workers.push(worker());
  await Promise.all(workers);

  const all = [...results.keys()].sort((a, b) => a - b).join() === Array.from({ length: TASKS }, (_, i) => i + 1).join();
  console.log("tasks put on the queue: " + TASKS);
  console.log("workers: " + WORKERS);
  console.log("deliveries: " + work.deliveries);
  console.log("tasks put back after a worker failed: " + work.putBack);
  console.log("tasks still not acknowledged: " + work.pending);
  console.log("every task has a result: " + (all ? "True" : "False"));
}

main();
```

```output
tasks put on the queue: 10
workers: 3
deliveries: 11
tasks put back after a worker failed: 1
tasks still not acknowledged: 0
every task has a result: True
```

- Verdict: idiom. The same `WorkQueue` class for async workers on one event loop. It shows the ack and the redelivery, not parallel work.
