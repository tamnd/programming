```typescript run id=emitter
type Listener<T> = (value: T) => void;

class Emitter<T> {
  #listeners = new Set<Listener<T>>();

  // Returns a function that removes the listener.
  on(listener: Listener<T>, signal?: AbortSignal): () => void {
    this.#listeners.add(listener);
    const off = () => {
      this.#listeners.delete(listener);
    };
    signal?.addEventListener("abort", off, { once: true });
    return off;
  }

  emit(value: T): void {
    for (const listener of this.#listeners) listener(value);
  }

  get size(): number {
    return this.#listeners.size;
  }
}

class Session {
  readonly user: string;
  seen: number[] = [];
  constructor(user: string) {
    this.user = user;
  }
}

const prices = new Emitter<number>(); // lives as long as the program

// Leaks: no session removes its listener.
for (let i = 0; i < 100; i++) {
  const session = new Session(`user-${i}`);
  prices.on((price) => session.seen.push(price));
}
console.log("listeners after 100 leaky sessions:", prices.size);

// Fixed: each session calls the function that on() returned.
const fixed = new Emitter<number>();
for (let i = 0; i < 100; i++) {
  const session = new Session(`user-${i}`);
  const off = fixed.on((price) => session.seen.push(price));
  off();
}
console.log("listeners after unsubscribe:", fixed.size);

// Fixed: one AbortController ends a group of sessions.
const withSignal = new Emitter<number>();
const controller = new AbortController();
for (let i = 0; i < 100; i++) {
  const session = new Session(`user-${i}`);
  withSignal.on((price) => session.seen.push(price), controller.signal);
}
console.log("listeners before abort:", withSignal.size);
controller.abort();
console.log("listeners after abort:", withSignal.size);
```

```output
listeners after 100 leaky sessions: 100
listeners after unsubscribe: 0
listeners before abort: 100
listeners after abort: 0
```

- Verdict: native. TypeScript runs on the JavaScript engine and its collector. The types do not show which object keeps a listener.
- `on` returns its own `off` function, so the caller does not need to keep the listener to remove it.
- An `AbortSignal` removes a group of listeners at one point, for example when a page or a request ends.
