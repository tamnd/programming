```javascript run id=workers-and-structured-clone
const { Worker, MessageChannel } = require("node:worker_threads");

// The worker runs in its own thread with its own memory. It answers each message.
const code = `
const { parentPort } = require("node:worker_threads");
parentPort.on("message", (msg) => {
  msg.items.push("added by worker");
  const sum = msg.items.filter((x) => typeof x === "number").reduce((a, b) => a + b, 0);
  parentPort.postMessage({ sum, items: msg.items });
});
`;

function ask(worker, msg) {
  return new Promise((resolve) => {
    worker.once("message", resolve);
    worker.postMessage(msg);
  });
}

async function main() {
  const worker = new Worker(code, { eval: true });
  const items = [1, 2, 3];
  const reply = await ask(worker, { items });
  // The message is a structured clone, so the worker changed a copy.
  console.log(reply.sum, reply.items.join(","), "|", items.join(","));
  await worker.terminate();

  // A Map and a Date survive the clone, but a function does not.
  const copy = structuredClone({ when: new Date(0), tags: new Map([["a", 1]]) });
  console.log(copy.when.toISOString(), copy.tags.get("a"));
  try {
    structuredClone({ f: () => 1 });
  } catch (e) {
    console.log(e.name);
  }

  // A MessageChannel has two ports: a message sent on one arrives on the other.
  const { port1, port2 } = new MessageChannel();
  const got = new Promise((resolve) => port2.once("message", resolve));
  port1.postMessage("hello");
  console.log(await got);
  port1.close();
}

main();
```

```output for=workers-and-structured-clone
6 1,2,3,added by worker | 1,2,3
1970-01-01T00:00:00.000Z 1
DataCloneError
hello
```

- Verdict: native. `postMessage` sends an array to a worker, and the worker adds an item to its copy, so the array of the main thread is not changed. `structuredClone` keeps a `Date` and a `Map` but throws `DataCloneError` for a function. A message sent on one port of a `MessageChannel` arrives on the other port.
