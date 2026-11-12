```typescript run id=leader-followers
import { Worker } from "node:worker_threads";

// Shared state: the leader flag, the next event (the handle set) and counters.
const LEADER = 0, NEXT = 1, WAITING = 2, MOST = 3, PROCESSED = 4, SUM = 5;

const workerCode = `
const { workerData } = require("node:worker_threads");
const s = new Int32Array(workerData);
const [LEADER, NEXT, WAITING, MOST, PROCESSED, SUM] = [0, 1, 2, 3, 4, 5];
const pause = new Int32Array(new SharedArrayBuffer(4));
while (true) {
  // Follower role: wait for the turn to lead.
  while (Atomics.compareExchange(s, LEADER, 0, 1) !== 0) Atomics.wait(s, LEADER, 1);
  const waiting = Atomics.add(s, WAITING, 1) + 1;
  if (waiting > Atomics.load(s, MOST)) Atomics.store(s, MOST, waiting);
  const event = Atomics.add(s, NEXT, 1); // leader role: take the next event
  // promote_new_leader
  Atomics.sub(s, WAITING, 1);
  Atomics.store(s, LEADER, 0);
  Atomics.notify(s, LEADER, 1);
  if (event >= 100) break;
  Atomics.wait(pause, 0, 0, 1); // processing role: a 1 ms blocking call in this thread
  Atomics.add(s, PROCESSED, 1);
  Atomics.add(s, SUM, event);
}
`;

const shared = new SharedArrayBuffer(6 * Int32Array.BYTES_PER_ELEMENT);
const s: Int32Array = new Int32Array(shared);
const exits: Promise<number>[] = [];
for (let i = 0; i < 4; i++) {
  const worker = new Worker(workerCode, { eval: true, workerData: shared });
  exits.push(new Promise<number>((resolve) => worker.on("exit", resolve)));
}
Promise.all(exits).then(() => {
  console.log("events processed:", s[PROCESSED]);
  console.log("sum of event ids:", s[SUM]);
  console.log("most threads waiting on the handle set at once:", s[MOST]);
});
```

```output
events processed: 100
sum of event ids: 4950
most threads waiting on the handle set at once: 1
```

- Verdict: idiom. Worker threads share a `SharedArrayBuffer`. `Atomics.compareExchange` takes the leader role, `Atomics.wait` holds the followers, and `Atomics.notify` promotes one of them.
