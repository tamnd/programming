```javascript run flags="--expose-gc" id=cycle
class Item {
  peer = null;
}

async function main() {
  let a = new Item();
  let b = new Item();
  a.peer = b;
  b.peer = a;
  const probe = new WeakRef(a);
  console.log("alive while referenced:", probe.deref() !== undefined);
  a = b = null;
  // A WeakRef target stays alive until the current job ends.
  await new Promise((resolve) => setTimeout(resolve, 0));
  globalThis.gc();
  console.log("collected after gc:", probe.deref() === undefined);
}

main();
```

```output
alive while referenced: true
collected after gc: true
```

- Verdict: native. Every JavaScript engine has a tracing collector, so a cycle of objects is freed when no root reaches it. The specification does not fix the algorithm.
- `globalThis.gc` is not part of the language. Node.js defines it only with the V8 flag `--expose-gc`, which this run uses.
- The `await` on a timer ends the current job. A `WeakRef` keeps its target alive until the job that created it or read it ends, so a `gc()` call in the same job cannot free the target.
- `FinalizationRegistry` can run a callback after a target is collected, but the specification makes this callback optional.
