```typescript run id=pool
class Buffer {
  readonly id: number;
  readonly parts: string[] = [];
  constructor(id: number) {
    this.id = id; // a real buffer would be costly to create
  }
  write(text: string): void {
    this.parts.push(text);
  }
  text(): string {
    return this.parts.join(" ");
  }
}

class Pool<T> {
  private readonly idleItems: T[] = [];
  created = 0;
  private readonly create: (n: number) => T;
  private readonly reset: (item: T) => void;
  private readonly maxIdle: number;

  constructor(create: (n: number) => T, reset: (item: T) => void, maxIdle: number) {
    this.create = create;
    this.reset = reset;
    this.maxIdle = maxIdle;
  }

  acquire(): T {
    const item = this.idleItems.pop();
    if (item !== undefined) return item;
    this.created += 1;
    return this.create(this.created);
  }

  release(item: T): void {
    this.reset(item); // clear the state before the next user sees it
    if (this.idleItems.length < this.maxIdle) this.idleItems.push(item);
  }

  use<R>(fn: (item: T) => R): R {
    const item = this.acquire();
    try {
      return fn(item);
    } finally {
      this.release(item); // runs even if fn throws
    }
  }

  get idle(): number {
    return this.idleItems.length;
  }
}

const pool = new Pool<Buffer>((n) => new Buffer(n), (b) => { b.parts.length = 0; }, 2);
for (let i = 1; i <= 3; i++) {
  pool.use((b) => {
    b.write(`request ${i}`);
    console.log(`request ${i} -> buffer #${b.id}: ${b.text()}`);
  });
}

const held: Buffer[] = [pool.acquire(), pool.acquire(), pool.acquire()];
console.log("3 at once -> buffers", held.map((b) => `#${b.id}`).sort().join(" "));
for (const b of held) pool.release(b);
console.log("idle after release:", pool.idle);
console.log("created:", pool.created);
```

```output
request 1 -> buffer #1: request 1
request 2 -> buffer #1: request 2
request 3 -> buffer #1: request 3
3 at once -> buffers #1 #2 #3
idle after release: 2
created: 3
```

- Verdict: idiom. A generic `Pool<T>` takes the create and reset functions, so one class serves every kind of pooled object.
- The type `T` cannot stop a caller from keeping a reference after `release`. That rule is a convention, not a type check.
