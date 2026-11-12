```javascript run id=immutable-object
"use strict"; // in strict mode, a write to a frozen object throws

function makeSettings(port) {
  return Object.freeze({ port, tag: `port ${port}` });
}

const first = makeSettings(8000);
let current = first; // the writer replaces the reference
let reads = 0;
let mixed = 0;
const tick = () => new Promise((resolve) => setImmediate(resolve));

async function reader() {
  for (let r = 0; r < 1000; r++) {
    const s = current; // keep one snapshot across the await
    await tick(); // the writer can run here
    if (s.tag !== `port ${s.port}`) mixed += 1;
    reads += 1;
  }
}

async function writer() {
  for (let port = 8001; port <= 8100; port++) {
    current = makeSettings(port); // a new object, never a change
    await tick();
  }
}

Promise.all([reader(), reader(), reader(), reader(), writer()]).then(() => {
  const unchanged = first.port === 8000 && first.tag === "port 8000";
  console.log("reads:", reads);
  console.log("reads that saw a mixed pair:", mixed);
  console.log("settings published:", 1 + current.port - first.port);
  console.log("first settings unchanged:", unchanged ? "True" : "False");
});
```

```output
reads: 4000
reads that saw a mixed pair: 0
settings published: 101
first settings unchanged: True
```

- Verdict: stdlib. `Object.freeze` stops changes to the `Settings` objects, and the readers share them across `await` points on one event loop.
