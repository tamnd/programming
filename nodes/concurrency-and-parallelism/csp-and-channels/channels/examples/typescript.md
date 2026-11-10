```typescript run id=ports
import {
  MessageChannel,
  MessagePort,
  Worker,
  isMainThread,
  receiveMessageOnPort,
  workerData,
} from "node:worker_threads";

type Msg = { value: number } | "done";

if (isMainThread) {
  let total = 0;
  let finished = 0;
  for (const id of [1, 2]) {
    const { port1, port2 } = new MessageChannel();
    new Worker(new URL(import.meta.url), {
      workerData: { id, port: port2 },
      transferList: [port2],
    });
    port1.on("message", (msg: Msg) => {
      if (msg === "done") {
        port1.close();
        finished += 1;
        if (finished === 2) console.log("total:", total);
      } else {
        total += msg.value;
      }
    });
  }
} else {
  const { id, port } = workerData as { id: number; port: MessagePort };
  for (let i = 1; i <= 3; i++) port.postMessage({ value: id * 10 * i } satisfies Msg);
  port.postMessage("done" satisfies Msg);
}
```

```output for=ports
total: 180
```

- Verdict: stdlib. TypeScript uses the same Node.js API as JavaScript.
- The union type `Msg` covers both the data message and the end marker, and `satisfies` checks each sent value against it. Nothing checks a received message at run time, so the type in the handler is a promise of the programmer.
