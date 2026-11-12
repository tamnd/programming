```javascript run id=multiton
const DECIMALS = new Map([["USD", 2], ["EUR", 2], ["JPY", 0]]); // the valid keys

class Currency {
  static #instances = new Map();
  static #creating = false;

  constructor(code, decimals) {
    if (!Currency.#creating) throw new TypeError("use Currency.of(code)");
    this.code = code;
    this.decimals = decimals;
    Object.freeze(this);
  }

  static of(code) {
    let instance = Currency.#instances.get(code);
    if (instance === undefined) {
      if (!DECIMALS.has(code)) throw new RangeError(code);
      Currency.#creating = true;
      try {
        instance = new Currency(code, DECIMALS.get(code));
      } finally {
        Currency.#creating = false;
      }
      Currency.#instances.set(code, instance); // one instance per key
    }
    return instance;
  }

  static get count() {
    return Currency.#instances.size;
  }
}

console.log("USD has", Currency.of("USD").decimals, "decimals");
console.log("JPY has", Currency.of("JPY").decimals, "decimals");
console.log("same USD object:", Currency.of("USD") === Currency.of("USD"));
console.log("instances:", Currency.count);
try {
  Currency.of("XYZ");
} catch (e) {
  console.log(`${e.message}: unknown currency`);
}
const seen = new Set(Array.from({ length: 8 }, () => Currency.of("EUR")));
console.log("8 calls ask for EUR:", seen.size, "object");
console.log("instances:", Currency.count);
```

```output
USD has 2 decimals
JPY has 0 decimals
same USD object: true
instances: 2
XYZ: unknown currency
8 calls ask for EUR: 1 object
instances: 3
```

- Verdict: idiom. A static private `Map` holds the instances. JavaScript runs one thread per agent, so the check and the insert need no lock.
- `Symbol.for(key)` is a multiton in the language: it returns the same symbol from the global symbol registry for the same key.
- A `WeakRef` in the map, with a `FinalizationRegistry` to remove dead entries, keeps the registry from growing for ever.
