```javascript run id=pool
class Buffer {
  constructor(id) {
    this.id = id; // a real buffer would be costly to create
    this.parts = [];
  }
  write(text) {
    this.parts.push(text);
  }
  text() {
    return this.parts.join(" ");
  }
}

class Pool {
  #idle = [];
  constructor(create, reset, maxIdle) {
    this.create = create;
    this.reset = reset;
    this.maxIdle = maxIdle;
    this.created = 0;
  }
  acquire() {
    if (this.#idle.length > 0) return this.#idle.pop();
    this.created += 1;
    return this.create(this.created);
  }
  release(item) {
    this.reset(item); // clear the state before the next user sees it
    if (this.#idle.length < this.maxIdle) this.#idle.push(item);
  }
  use(fn) {
    const item = this.acquire();
    try {
      return fn(item);
    } finally {
      this.release(item); // runs even if fn throws
    }
  }
  get idle() {
    return this.#idle.length;
  }
}

const pool = new Pool((id) => new Buffer(id), (b) => { b.parts.length = 0; }, 2);
for (let i = 1; i <= 3; i++) {
  pool.use((b) => {
    b.write(`request ${i}`);
    console.log(`request ${i} -> buffer #${b.id}: ${b.text()}`);
  });
}

const held = [pool.acquire(), pool.acquire(), pool.acquire()];
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

- Verdict: idiom. JavaScript has no pool in the language or in the standard library. An array of idle objects is the usual form.
- JavaScript runs one thread per agent, so the pool needs no lock. A pool shared with workers would need messages or `Atomics`.
- Node.js database drivers keep connection pools, and games pool objects to avoid garbage collection pauses during a frame.
