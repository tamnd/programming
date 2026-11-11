```javascript run id=counting-map
class CountingMap {
  #inner = new Map(); // has-a Map, hidden from callers
  writes = 0;

  constructor(entries = []) {
    for (const [k, v] of entries) this.set(k, v);
  }

  set(k, v) {
    this.writes++;
    this.#inner.set(k, v);
    return this;
  }

  get(k) { return this.#inner.get(k); }

  get size() { return this.#inner.size; }
}

const m = new CountingMap([["a", 1], ["b", 2]]);
m.set("c", 3);
console.log(m.writes, m.size, m.get("b"));

// Build an object from parts, with no class hierarchy.
const canFly = (self) => ({ fly: () => `${self.name} flies` });
const canSwim = (self) => ({ swim: () => `${self.name} swims` });

function duck(name) {
  const self = { name };
  return Object.assign(self, canFly(self), canSwim(self));
}

function penguin(name) {
  const self = { name };
  return Object.assign(self, canSwim(self));
}

console.log(duck("mallard").fly(), "/", penguin("gentoo").swim());
console.log("fly" in penguin("gentoo"));
```

```output for=counting-map
3 3 2
mallard flies / gentoo swims
false
```

```javascript wrong id=extends-map
class CountingMap extends Map {
  writes = 0;

  set(k, v) {
    this.writes++;
    return super.set(k, v);
  }
}

// The Map constructor calls this.set for each entry, before the
// field initializer of the subclass runs and resets writes to 0.
const m = new CountingMap([["a", 1], ["b", 2]]);
m.set("c", 3);
console.log(m.writes, m.size);
```

```output for=extends-map
1 3
```

- Verdict: idiom. `CountingMap` keeps a `Map` in a private field and offers only `set`, `get` and `size`. The second part builds objects from small parts with `Object.assign`: a duck gets the parts that fly and swim, and a penguin only the part that swims, with no class hierarchy at all. Any object with the right methods can be a part, because JavaScript checks methods only when it calls them.
- The wrong block extends `Map` and overrides `set`. The `Map` constructor calls `this.set` for each initial entry, which reaches the override before the field initializer of the subclass runs. The initializer then sets `writes` to 0, so the two initial entries are lost from the count.
