```typescript run id=atomics
import { Worker, isMainThread, workerData } from "node:worker_threads";

function finished(worker: Worker): Promise<number> {
  return new Promise((resolve) => worker.on("exit", resolve));
}

if (isMainThread) {
  const shared = new Int32Array(new SharedArrayBuffer(4));
  const workers = [1, 2].map(
    () => new Worker(new URL(import.meta.url), { workerData: shared }),
  );
  await Promise.all(workers.map(finished));
  console.log(Atomics.load(shared, 0));
} else {
  const shared: Int32Array = workerData;
  for (let i = 0; i < 100_000; i++) {
    Atomics.add(shared, 0, 1);
  }
}
```

```output for=atomics
200000
```

- Verdict: stdlib. TypeScript has the same memory model as JavaScript, and `Atomics` and `SharedArrayBuffer` have types in the standard library declarations.
- The type `Int32Array` of `workerData` is only a claim to the compiler. The structured clone of the message gives a view of the same shared buffer.
- The types do not show which data is shared, so the compiler cannot find a data race.
