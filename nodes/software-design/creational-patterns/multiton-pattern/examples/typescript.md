```typescript run id=multiton
type Code = "USD" | "EUR" | "JPY";
const DECIMALS: Record<Code, number> = { USD: 2, EUR: 2, JPY: 0 }; // the valid keys

class Currency {
  private static readonly instances = new Map<Code, Currency>();
  readonly code: Code;
  readonly decimals: number;

  private constructor(code: Code) {
    this.code = code;
    this.decimals = DECIMALS[code];
  }

  static of(code: Code): Currency {
    let instance = Currency.instances.get(code);
    if (instance === undefined) {
      instance = new Currency(code);
      Currency.instances.set(code, instance); // one instance per key
    }
    return instance;
  }

  static parse(text: string): Currency | undefined {
    return Object.hasOwn(DECIMALS, text) ? Currency.of(text as Code) : undefined;
  }

  static get count(): number {
    return Currency.instances.size;
  }
}

console.log("USD has", Currency.of("USD").decimals, "decimals");
console.log("JPY has", Currency.of("JPY").decimals, "decimals");
console.log("same USD object:", Currency.of("USD") === Currency.of("USD"));
console.log("instances:", Currency.count);
console.log(`XYZ: ${Currency.parse("XYZ") === undefined ? "unknown currency" : "found"}`);
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

- Verdict: idiom. A union type of the keys lets the compiler reject `Currency.of("XYZ")`. Text from outside goes through `parse`, which checks the key at run time.
- When the set of keys is fixed and known at compile time, a frozen object of instances is simpler than a registry: the instances exist from the start, and no code creates them.
