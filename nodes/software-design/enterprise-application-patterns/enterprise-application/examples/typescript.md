```typescript run id=three-layers
// Data source layer: the stored data, and a transaction that keeps all or none of a change.
interface Order {
  customer: string;
  item: string;
  quantity: number;
  cents: number;
}

interface Tables {
  stock: Map<string, number>;
  orders: Order[];
}

class Store {
  tables: Tables = { stock: new Map([["widget", 10]]), orders: [] };

  transaction(change: (tables: Tables) => void): boolean {
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

function priceCents(customer: string, quantity: number): number {
  let total = PRICE_CENTS * quantity;
  if (customer === "acme" && quantity >= 3) {
    total = Math.floor((total * 90) / 100); // a deal that sales made with one customer
  }
  return total;
}

function placeOrder(store: Store, customer: string, item: string, quantity: number): boolean {
  return store.transaction((tables) => {
    tables.orders.push({ customer, item, quantity, cents: priceCents(customer, quantity) });
    const count = tables.stock.get(item) ?? 0;
    if (count < quantity) throw new RangeError("not enough stock");
    tables.stock.set(item, count - quantity);
  });
}

function money(cents: number): string {
  return `${Math.floor(cents / 100)}.${String(cents % 100).padStart(2, "0")}`;
}

// Presentation layer: two screens show the same data in different ways.
function orderScreen(store: Store): void {
  for (const o of store.tables.orders) {
    console.log(`order screen: ${o.customer} bought ${o.quantity} x ${o.item} for ${money(o.cents)}`);
  }
}

function stockScreen(store: Store): void {
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

- Verdict: library. The same frameworks as JavaScript, with interfaces for the rows of each table.
