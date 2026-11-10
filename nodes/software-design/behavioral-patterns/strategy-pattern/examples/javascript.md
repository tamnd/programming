```javascript run id=checkout
const regular = (items) =>
  items.reduce((sum, item) => sum + item.price * item.quantity, 0);

const tenPercentOff = (items) => Math.trunc((regular(items) * 90) / 100);

// 50 cents off each unit of a line with 5 or more units.
const bulk = (items) =>
  items.reduce(
    (sum, item) =>
      sum + (item.quantity >= 5 ? item.price - 50 : item.price) * item.quantity,
    0,
  );

class Checkout {
  constructor(pricing) {
    this.pricing = pricing;
  }

  total(items) {
    return this.pricing(items);
  }
}

const cart = [
  { name: "book", price: 1500, quantity: 2 },
  { name: "pen", price: 200, quantity: 5 },
];
const checkout = new Checkout(regular);
for (const [name, pricing] of [
  ["regular", regular],
  ["ten percent off", tenPercentOff],
  ["bulk", bulk],
]) {
  checkout.pricing = pricing;
  console.log(`${name}: ${checkout.total(cart)} cents`);
}
```

```output
regular: 4000 cents
ten percent off: 3600 cents
bulk: 3750 cents
```

- Verdict: idiom. JavaScript functions are values, so a strategy is a function in a field of the context.
- `this.pricing(items)` calls the function as a method of the checkout, so `this` inside a `function` strategy would be the checkout. Arrow functions ignore that `this`.
- `Math.trunc` keeps the integer cents. Numbers are doubles, so integer cents stay exact up to `Number.MAX_SAFE_INTEGER`.
- `Array.prototype.sort(comparator)` takes its comparison as a strategy in the same way.
