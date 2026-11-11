```javascript run id=jobs
function runJob(job, items) {
  try {
    return items[job].value * 10;
  } finally {
    console.log("cleanup job", job);
  }
}

const items = [{ value: 1 }, { value: 2 }];
for (const job of [0, 5, 1]) {
  try {
    console.log("job", job, "->", runJob(job, items));
  } catch (err) {
    console.log("job", job, "failed:", err instanceof TypeError, err.message);
  }
}
```

```output
cleanup job 0
job 0 -> 10
cleanup job 5
job 5 failed: true Cannot read properties of undefined (reading 'value')
cleanup job 1
job 1 -> 20
```

- Verdict: idiom. JavaScript has no panic. A bug raises an ordinary exception, here a `TypeError` from reading a property of `undefined`, and a boundary catches it.
- An exception that no code catches ends a Node.js process with exit code 1.
- `process.exit` and `process.abort` are the hard stops in Node.js.

`process.exit` ends the process synchronously. The `finally` block does not run.

```javascript fails expect=exit:3 id=process-exit
try {
  process.exit(3);
} finally {
  console.log("cleanup");
}
```
