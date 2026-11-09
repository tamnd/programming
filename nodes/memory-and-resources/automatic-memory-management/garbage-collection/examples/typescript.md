```typescript run flags="--expose-gc" id=cycle
// gc() exists only when the engine runs with --expose-gc.
declare function gc(): void;

class Item {
  peer: Item | null = null;
}

async function main(): Promise<void> {
  let a: Item | null = new Item();
  let b: Item | null = new Item();
  a.peer = b;
  b.peer = a;
  const probe = new WeakRef(a);
  console.log("alive while referenced:", probe.deref() !== undefined);
  a = b = null;
  // A WeakRef target stays alive until the current job ends.
  await new Promise((resolve) => setTimeout(resolve, 0));
  gc();
  console.log("collected after gc:", probe.deref() === undefined);
}

main();
```

```output
alive while referenced: true
collected after gc: true
```

- Verdict: native. TypeScript compiles to JavaScript, so the collector is the one of the JavaScript engine.
- `gc` exists only when Node.js starts with `--expose-gc`. The `declare function` line tells the type checker about it.
- `WeakRef<Item>` is generic: `deref()` returns `Item | undefined`.
