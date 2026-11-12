```typescript run id=lazy
// lazy returns a function that runs init once and then returns the stored value.
function lazy<T>(init: () => T): () => T {
  let done = false;
  let value: T | undefined;
  return () => {
    if (!done) {
      value = init();
      done = true;
    }
    return value as T;
  };
}

let loads = 0;

class Settings {
  readonly config = lazy(() => {
    console.log("loading config"); // stands for a slow read of a file
    loads += 1;
    return { timeout: 30 };
  });
}

const s = new Settings();
console.log("settings created, loads =", loads);
console.log("timeout =", s.config().timeout);
console.log("timeout =", s.config().timeout);
console.log("loads =", loads);

// An async value: the stored value is the promise, so callers share one load.
let asyncLoads = 0;
const remote = lazy(
  () =>
    new Promise<{ timeout: number }>((resolve) => {
      asyncLoads += 1;
      setTimeout(() => resolve({ timeout: 30 }), 10);
    }),
);

Promise.all(Array.from({ length: 8 }, () => remote())).then(() =>
  console.log("8 callers: loads =", asyncLoads),
);
```

```output
settings created, loads = 0
loading config
timeout = 30
timeout = 30
loads = 1
8 callers: loads = 1
```

- Verdict: idiom. A closure over a flag and a value is a typed lazy holder. The flag, not a test for `undefined`, records that the value exists, so `init` can return `undefined` and still run once.
- The same `lazy` works for a promise, which gives a shared async load.
