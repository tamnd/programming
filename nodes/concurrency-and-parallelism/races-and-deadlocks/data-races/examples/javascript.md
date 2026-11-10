```javascript run id=atomics
const { Worker, isMainThread, workerData } = require("node:worker_threads");

if (isMainThread) {
  const shared = new Int32Array(new SharedArrayBuffer(4));
  const workers = [1, 2].map(
    () => new Worker(__filename, { workerData: shared }),
  );
  let running = workers.length;
  for (const worker of workers) {
    worker.on("exit", () => {
      running -= 1;
      if (running === 0) {
        console.log(Atomics.load(shared, 0));
      }
    });
  }
} else {
  for (let i = 0; i < 100_000; i++) {
    Atomics.add(workerData, 0, 1);
  }
}
```

```output for=atomics
200000
```

- Verdict: stdlib. Plain JavaScript values are not shared between threads. A `SharedArrayBuffer` is the only memory that workers share.
- `Atomics.add` on an `Int32Array` over the shared buffer is atomic. A plain `shared[0]++` in both workers would be a data race.
- A data race on shared memory has defined behaviour in ECMAScript. Racing accesses of the same size do not tear, but updates can still be lost.
