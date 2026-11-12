```javascript run id=thread-confinement
const { Worker } = require("node:worker_threads");

// Each worker has its own heap, so its counts object is confined to it.
const workerCode = `
const { parentPort, workerData } = require("node:worker_threads");
const counts = {};
for (let r = 0; r < 2000; r++) {
  for (const w of workerData) counts[w] = (counts[w] || 0) + 1;
}
parentPort.postMessage(counts); // the main thread receives a copy
`;

const words = ["a", "b", "c", "d", "e"];
const chunks = 4;
const runs = [];
for (let i = 0; i < chunks; i++) {
  runs.push(
    new Promise((resolve) => {
      new Worker(workerCode, { eval: true, workerData: words }).on("message", resolve);
    }),
  );
}

Promise.all(runs).then((results) => {
  const total = {};
  for (const counts of results) {
    for (const [w, n] of Object.entries(counts)) total[w] = (total[w] || 0) + n;
  }
  console.log("chunks:", chunks);
  console.log("words counted:", Object.values(total).reduce((a, b) => a + b, 0));
  console.log("distinct words:", Object.keys(total).length);
  console.log("count of 'a':", total["a"]);
});
```

```output
chunks: 4
words counted: 40000
distinct words: 5
count of 'a': 8000
```

- Verdict: idiom. Each worker thread has its own `counts` object that no other thread can reach, and `postMessage` gives the main thread a copy.
