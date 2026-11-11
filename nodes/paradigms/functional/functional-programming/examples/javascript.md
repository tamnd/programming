```javascript run id=pipeline
const orders = Object.freeze([
  { item: "pen", qty: 3, price: 2 },
  { item: "book", qty: 1, price: 12 },
  { item: "ink", qty: 2, price: 5 },
]);

const total = orders
  .filter((o) => o.qty > 1)
  .map((o) => o.qty * o.price)
  .reduce((sum, x) => sum + x, 0);
console.log(total);

const pipe = (...fs) => (x) => fs.reduce((acc, f) => f(acc), x);
const slug = pipe((s) => s.trim(), (s) => s.toLowerCase(), (s) => s.replaceAll(" ", "-"));
console.log(slug("  Pure Functions  "));

const repriced = orders.map((o) => ({ ...o, price: o.price * 2 }));
console.log(repriced[0].price, orders[0].price);
console.log(orders.toSorted((a, b) => a.price - b.price).map((o) => o.item));
```

```output for=pipeline
16
pure-functions
4 2
[ 'pen', 'ink', 'book' ]
```

- Verdict: native. Functions are values, arrow functions capture their scope, and arrays have `map`, `filter` and `reduce`. The spread syntax builds a changed copy of an object.
- `Object.freeze` is shallow, and array methods such as `sort` and `reverse` change the array in place. The methods `toSorted`, `toReversed` and `with` return new arrays.
