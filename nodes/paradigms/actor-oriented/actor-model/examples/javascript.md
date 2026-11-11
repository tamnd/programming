```javascript run id=worker
import { Worker, isMainThread, parentPort } from "node:worker_threads";

if (isMainThread) {
  const account = new Worker(new URL(import.meta.url));
  account.on("message", (msg) => {
    console.log(msg);
    if (msg.startsWith("balance")) account.terminate();
  });
  const order = { op: "deposit", amount: 100 };
  account.postMessage(order);
  order.amount = 1_000_000; // changes only the sender's copy
  account.postMessage({ op: "withdraw", amount: 30 });
  account.postMessage({ op: "withdraw", amount: 500 });
  account.postMessage({ op: "balance" });
} else {
  let balance = 0; // only this worker can reach this variable
  parentPort.on("message", ({ op, amount }) => {
    if (op === "deposit") balance += amount;
    else if (op === "withdraw") {
      if (amount > balance) parentPort.postMessage(`refused withdraw ${amount}`);
      else balance -= amount;
    } else parentPort.postMessage(`balance ${balance}`);
  });
}
```

```output for=worker
refused withdraw 500
balance 70
```

- Verdict: stdlib. A worker thread is an actor: it has its own heap and event loop, and the only way to reach it is `postMessage`. Messages are copied with the structured clone algorithm, so the change to `order` after the send does not reach the worker.
