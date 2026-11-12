```javascript run id=copy-on-write
class CopyOnWriteList {
  constructor(items) {
    this.items = Object.freeze([...items]); // never changed in place
  }

  add(item) {
    this.items = Object.freeze([...this.items, item]); // copy, then publish
  }

  snapshot() {
    return this.items;
  }
}

const listeners = new CopyOnWriteList(["listener 0"]);
const first = listeners.snapshot();
let iterations = 0;
let errors = 0;
const tick = () => new Promise((resolve) => setImmediate(resolve));

async function reader() {
  for (let r = 0; r < 1000; r++) {
    try {
      for (const name of listeners.snapshot()) {
        if (name === "listener 0") await tick(); // the writer can run mid-loop
      }
    } catch (e) {
      errors += 1;
    }
    iterations += 1;
  }
}

async function writer() {
  for (let n = 1; n <= 100; n++) {
    listeners.add(`listener ${n}`);
    await tick();
  }
}

Promise.all([reader(), reader(), reader(), reader(), writer()]).then(() => {
  console.log("iterations:", iterations);
  console.log("iterations that failed:", errors);
  console.log("listeners at the end:", listeners.snapshot().length);
  console.log("length of the first snapshot:", first.length);
});
```

```output
iterations: 4000
iterations that failed: 0
listeners at the end: 101
length of the first snapshot: 1
```

- Verdict: idiom. `CopyOnWriteList` replaces a frozen array on each `add`, and a reader that awaits inside its loop keeps iterating over the array it started with.
