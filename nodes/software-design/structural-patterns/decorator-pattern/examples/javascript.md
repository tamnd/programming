```javascript run id=repository
class Store {
  get(key) {
    console.log(`store: load ${key}`);
    return `value-${key}`;
  }

  name() {
    return "store";
  }
}

class Logged {
  #inner;

  constructor(inner) {
    this.#inner = inner;
  }

  get(key) {
    console.log(`log: get ${key}`);
    return this.#inner.get(key);
  }

  name() {
    return this.#inner.name();
  }
}

class Cached {
  #inner;
  #cache = new Map();

  constructor(inner) {
    this.#inner = inner;
  }

  get(key) {
    if (!this.#cache.has(key)) {
      this.#cache.set(key, this.#inner.get(key));
    }
    return this.#cache.get(key);
  }

  name() {
    return this.#inner.name();
  }
}

const repo = new Logged(new Cached(new Store()));
for (const key of ["a", "a", "b"]) {
  console.log(repo.get(key));
}
console.log(`name: ${repo.name()}`);
```

```output
log: get a
store: load a
value-a
log: get a
value-a
log: get b
store: load b
value-b
name: store
```

- Verdict: idiom. The interface is the set of method names that the client calls. Any object with `get` and `name` can be wrapped.
- The private field `#inner` keeps the client from reaching past the decorator to the store.
- The `@decorator` syntax of the TC39 proposal is a different construct. It runs a function on a class or a class element when the class is defined.
- A `Proxy` object with a `get` trap can forward every property without writing each method. The trap then runs for every property access, which makes the code harder to follow.
