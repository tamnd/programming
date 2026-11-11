```typescript run id=jobs
function fail(message: string): never {
  throw new Error("invariant broken: " + message);
}

function runJob(job: number, items: readonly number[]): number {
  try {
    const item = items[job] ?? fail(`no item ${job}`);
    return item * 10;
  } finally {
    console.log("cleanup job", job);
  }
}

const items = [1, 2] as const;
for (const job of [0, 5, 1]) {
  try {
    console.log("job", job, "->", runJob(job, items));
  } catch (err) {
    console.log("job", job, "failed:", (err as Error).message);
  }
}
```

```output
cleanup job 0
job 0 -> 10
cleanup job 5
job 5 failed: invariant broken: no item 5
cleanup job 1
job 1 -> 20
```

- Verdict: idiom. TypeScript runs as JavaScript and has no panic. A bug is an ordinary exception.
- The return type `never` marks a function that does not return. The checker then treats `items[job] ?? fail(...)` as a `number`.
- In Node.js, `process.exit` is the hard stop, as in JavaScript.

`process.exit` ends the process before the `finally` block can run.

```typescript fails expect=exit:3 id=process-exit
function stop(code: number): never {
  process.exit(code);
}

try {
  stop(3);
} finally {
  console.log("cleanup");
}
```
