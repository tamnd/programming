```javascript run id=three-layers
// Data source layer: the stored data, and a transaction that keeps all or none of a change.
class Store {
  constructor() {
    this.tables = { stock: new Map([["widget", 10]]), orders: [] };
  }

  transaction(change) {
    const saved = structuredClone(this.tables);
    try {
      change(this.tables);
      return true;
    } catch (e) {
      if (!(e instanceof RangeError)) throw e;
      this.tables = saved; // roll back
      return false;
    }
  }
}

// Domain layer: the business rules, with a special deal for one customer.
const PRICE_CENTS = 1000;

function priceCents(customer, quantity) {
  let total = PRICE_CENTS * quantity;
  if (customer === "acme" && quantity >= 3) {
    total = Math.floor((total * 90) / 100); // a deal that sales made with one customer
  }
  return total;
}

function placeOrder(store, customer, item, quantity) {
  return store.transaction((tables) => {
    tables.orders.push({ customer, item, quantity, cents: priceCents(customer, quantity) });
    if (tables.stock.get(item) < quantity) throw new RangeError("not enough stock");
    tables.stock.set(item, tables.stock.get(item) - quantity);
  });
}

function money(cents) {
  return `${Math.floor(cents / 100)}.${String(cents % 100).padStart(2, "0")}`;
}

// Presentation layer: two screens show the same data in different ways.
function orderScreen(store) {
  for (const o of store.tables.orders) {
    console.log(`order screen: ${o.customer} bought ${o.quantity} x ${o.item} for ${money(o.cents)}`);
  }
}

function stockScreen(store) {
  for (const [item, count] of store.tables.stock) {
    console.log(`stock screen: ${item}: ${count} in stock, orders: ${store.tables.orders.length}`);
  }
}

const store = new Store();
console.log("acme orders 3:", placeOrder(store, "acme", "widget", 3) ? "kept" : "rolled back");
console.log("bolt orders 9:", placeOrder(store, "bolt", "widget", 9) ? "kept" : "rolled back");
orderScreen(store);
stockScreen(store);
```

```output
acme orders 3: kept
bolt orders 9: rolled back
order screen: acme bought 3 x widget for 27.00
stock screen: widget: 7 in stock, orders: 1
```

- Verdict: library. Node.js frameworks give the screens and the requests, and a driver or an ORM gives the database. The example uses `structuredClone` to save the tables before a change.
