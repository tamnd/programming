```javascript run id=master-worker-pattern
const { Worker } = require("node:worker_threads");

const LIMIT = 20000;
const WORKERS = 4;

// The work of one worker: count the primes in [lo, hi).
function countPrimes(lo, hi) {
  let count = 0;
  for (let n = Math.max(lo, 2); n < hi; n++) {
    let d = 2;
    while (d * d <= n && n % d !== 0) d++;
    if (d * d > n) count++;
  }
  return count;
}

// Each worker thread runs this code with its part and posts back its count.
const workerCode = `
const { parentPort, workerData } = require("node:worker_threads");
${countPrimes.toString()}
parentPort.postMessage(countPrimes(workerData.lo, workerData.hi));
`;

async function master(limit, workers) {
  // 1. Split the work into one part for each worker.
  const bounds = Array.from({ length: workers + 1 }, (_, i) => Math.floor((limit * i) / workers));
  const parts = bounds.slice(0, -1).map((lo, i) => [lo, bounds[i + 1]]);
  // 2. Start the workers, then wait for all of them.
  const results = await Promise.all(
    parts.map(
      ([lo, hi]) =>
        new Promise((resolve, reject) => {
          const w = new Worker(workerCode, { eval: true, workerData: { lo, hi } });
          w.once("message", resolve);
          w.once("error", reject);
        }),
    ),
  );
  // 3. Combine the partial results.
  return [parts, results, results.reduce((a, b) => a + b, 0)];
}

master(LIMIT, WORKERS).then(([parts, results, total]) => {
  console.log("parts: [" + parts.map(([lo, hi]) => `(${lo}, ${hi})`).join(", ") + "]");
  console.log("primes in each part: [" + results.join(", ") + "]");
  console.log(`primes below ${LIMIT} from the master: ${total}`);
  console.log("same as one loop: " + (total === countPrimes(0, LIMIT) ? "True" : "False"));
});
```

```output
parts: [(0, 5000), (5000, 10000), (10000, 15000), (15000, 20000)]
primes in each part: [669, 560, 525, 508]
primes below 20000 from the master: 2262
same as one loop: True
```

- Verdict: stdlib. `node:worker_threads` runs each part in its own thread with `workerData`, and `Promise.all` waits for every `message`.
