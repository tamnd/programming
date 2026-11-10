```typescript run id=checkout
interface Item {
  readonly name: string;
  readonly price: number; // in cents
  readonly quantity: number;
}

type Pricing = (items: readonly Item[]) => number;

const regular: Pricing = (items) =>
  items.reduce((sum, item) => sum + item.price * item.quantity, 0);

const tenPercentOff: Pricing = (items) => Math.trunc((regular(items) * 90) / 100);

// 50 cents off each unit of a line with 5 or more units.
const bulk: Pricing = (items) =>
  items.reduce(
    (sum, item) =>
      sum + (item.quantity >= 5 ? item.price - 50 : item.price) * item.quantity,
    0,
  );

class Checkout {
  pricing: Pricing;

  constructor(pricing: Pricing) {
    this.pricing = pricing;
  }

  total(items: readonly Item[]): number {
    return this.pricing(items);
  }
}

const cart: Item[] = [
  { name: "book", price: 1500, quantity: 2 },
  { name: "pen", price: 200, quantity: 5 },
];
const strategies: [string, Pricing][] = [
  ["regular", regular],
  ["ten percent off", tenPercentOff],
  ["bulk", bulk],
];
const checkout = new Checkout(regular);
for (const [name, pricing] of strategies) {
  checkout.pricing = pricing;
  console.log(`${name}: ${checkout.total(cart)} cents`);
}
```

```output
regular: 4000 cents
ten percent off: 3600 cents
bulk: 3750 cents
```

- Verdict: idiom. The strategy interface is a function type alias, and each strategy is a function value that the compiler checks against it.
- Type compatibility is structural, so any function with a compatible signature is a `Pricing`. It needs no `implements` clause.
- The class declares its field and assigns it in the constructor. A parameter property such as `constructor(public pricing: Pricing)` is shorter, but it makes JavaScript code, so Node.js cannot run it by type stripping alone.
