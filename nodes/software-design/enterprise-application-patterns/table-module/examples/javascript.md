```javascript run id=one-object-for-all-orders
class ModuleError extends Error {}

const cents = (n) => `${Math.floor(n / 100)}.${String(n % 100).padStart(2, "0")}`;

// The record set: an in-memory copy of the rows of several tables. Here the rows are written
// in the code; in an application, a query of the database fills the record set.
function loadRecordSet() {
  return {
    customer: [
      { id: 1, name: "Ann", tier: "gold" },
      { id: 2, name: "Bob", tier: "regular" },
      { id: 3, name: "Cy", tier: "staff" },
    ],
    orders: [{ id: 1, customerId: 1 }, { id: 2, customerId: 2 }, { id: 3, customerId: 3 }],
    orderLine: [
      { orderId: 1, item: "lamp", qty: 2, priceCents: 2500 },
      { orderId: 1, item: "desk", qty: 1, priceCents: 7500 },
      { orderId: 2, item: "lamp", qty: 1, priceCents: 2500 },
      { orderId: 2, item: "pen", qty: 3, priceCents: 500 },
      { orderId: 3, item: "lamp", qty: 1, priceCents: 2500 },
      { orderId: 3, item: "pen", qty: 3, priceCents: 500 },
    ],
  };
}

// One table module for each table. One instance handles all the rows of its table.
class CustomerModule {
  constructor(recordSet) { this.rows = recordSet.customer; }

  row(customerId) {
    const row = this.rows.find((r) => r.id === customerId);
    if (!row) throw new ModuleError(`no customer ${customerId}`);
    return row;
  }

  namesWithTier(tier) { return this.rows.filter((r) => r.tier === tier).map((r) => r.name); }
}

class OrderModule {
  constructor(recordSet, customers) {
    this.rows = recordSet.orders;
    this.lines = recordSet.orderLine;
    this.customers = customers;
  }

  row(orderId) {
    const row = this.rows.find((r) => r.id === orderId);
    if (!row) throw new ModuleError(`no order ${orderId}`);
    return row;
  }

  subtotal(orderId) {
    this.row(orderId);
    return this.lines.filter((l) => l.orderId === orderId).reduce((s, l) => s + l.qty * l.priceCents, 0);
  }

  total(orderId) {
    const subtotal = this.subtotal(orderId);
    const tier = this.customers.row(this.row(orderId).customerId).tier;
    if (tier === "gold" && subtotal >= 10000) return subtotal - Math.floor(subtotal / 10);
    if (tier === "staff") return subtotal - Math.floor(subtotal / 5);
    return subtotal;
  }

  countOver(limit) { return this.rows.filter((r) => this.total(r.id) > limit).length; }
}

const recordSet = loadRecordSet();
const customers = new CustomerModule(recordSet);
const orders = new OrderModule(recordSet, customers);
console.log(`record set: ${recordSet.customer.length} customers, ` +
  `${recordSet.orders.length} orders, ${recordSet.orderLine.length} order lines`);
for (const orderId of [1, 2, 3]) {
  console.log(`order ${orderId}: subtotal ${cents(orders.subtotal(orderId))}, total ${cents(orders.total(orderId))}`);
}
console.log("gold customers:", customers.namesWithTier("gold").join(", "));
console.log("orders over 35.00:", orders.countOver(3500));
try {
  orders.total(9);
} catch (e) {
  if (!(e instanceof ModuleError)) throw e;
  console.log("error:", e.message);
}
```

```output
record set: 3 customers, 3 orders, 6 order lines
order 1: subtotal 125.00, total 112.50
order 2: subtotal 40.00, total 40.00
order 3: subtotal 40.00, total 32.00
gold customers: Ann
orders over 35.00: 2
error: no order 9
```

- Verdict: idiom. The record set is a plain object with an array of rows for each table. Each module keeps a reference to the arrays of its table and finds a row with `find`.
