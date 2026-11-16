```javascript run id=async-turns
import { isMainThread } from "node:worker_threads";

async function worker(name, steps) {
  for (let i = 0; i < steps; i++) {
    console.log(`${name} step ${i}`);
    await null; // give the thread back until the next microtask
  }
}

await Promise.all([worker("a", 3), worker("b", 2)]);
console.log(`all steps ran on the main thread: ${isMainThread}`);
```

```output for=async-turns
a step 0
b step 0
a step 1
b step 1
a step 2
all steps ran on the main thread: true
```

- Verdict: absent. JavaScript runs each event loop on one thread, and it has no green threads. `async` functions are the alternative: each `await` suspends the function, and the event loop resumes it later.
- Each call to `worker` runs until its first `await`. Then the next call starts. After that, the two functions resume in turn.
- A Node.js `Worker` is an OS thread with its own event loop, not a green thread.
- A loop without `await` keeps the thread, so no other function can run until the loop ends.
