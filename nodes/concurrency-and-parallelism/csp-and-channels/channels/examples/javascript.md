```javascript run id=ports
import {
  MessageChannel,
  Worker,
  isMainThread,
  receiveMessageOnPort,
  workerData,
} from "node:worker_threads";

if (isMainThread) {
  let total = 0;
  let finished = 0;
  for (const id of [1, 2]) {
    const { port1, port2 } = new MessageChannel();
    new Worker(new URL(import.meta.url), {
      workerData: { id, port: port2 },
      transferList: [port2], // the worker owns port2 now
    });
    port1.on("message", (msg) => {
      if (msg === "done") {
        port1.close();
        finished += 1;
        if (finished === 2) console.log("total:", total);
      } else {
        total += msg.value;
      }
    });
  }

  const { port1, port2 } = new MessageChannel();
  const job = { value: 1 };
  port1.postMessage(job);
  job.value = 2; // changes the sender's object only
  console.log(receiveMessageOnPort(port2));
  console.log(receiveMessageOnPort(port2));
  port1.close();
} else {
  const { id, port } = workerData;
  for (let i = 1; i <= 3; i++) port.postMessage({ value: id * 10 * i });
  port.postMessage("done");
}
```

```output for=ports
{ message: { value: 1 } }
undefined
total: 180
```

- Verdict: stdlib. The language has no channel. `MessageChannel` in `node:worker_threads` gives two connected `MessagePort` objects, and each worker thread has its own event loop.
- `postMessage` is asynchronous: it does not wait for the receiver, so there is no rendezvous. The receiver gets a structured clone of the value, so a later change by the sender does not reach it.
- `close` disables further sending on both ports. `receiveMessageOnPort` receives at once and returns `undefined` when no message waits.
