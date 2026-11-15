```javascript run id=logical-and-physical-layers
// The error that the application kernel sees, whatever the database says.
class AccessError extends Error {}

// ---- Physical access layer: knows the tables. ----

// The example has no database: Database holds tables as arrays of rows.
class Database {
  constructor(tables) {
    this.tables = tables;
  }
  rows(table) {
    return this.tables[table];
  }
  commit() {}
}

const normalized = () =>
  new Database({
    customer: [{ id: 1, name: "Acme Ltd" }],
    orders: [{ id: 10, customer_id: 1 }],
    order_line: [{ order_id: 10, item: "bolt", qty: 2 }, { order_id: 10, item: "nut", qty: 5 }],
  });

// After tuning: the customer name is copied into the orders table.
const denormalized = () =>
  new Database({
    orders: [{ id: 10, customer_name: "Acme Ltd" }],
    order_line: [{ order_id: 10, item: "bolt", qty: 2 }, { order_id: 10, item: "nut", qty: 5 }],
  });

// The protocol of a physical view of orders: read an order, write a line.
class OrderView {
  constructor(db) {
    this.db = db;
  }
  lines(orderId) {
    return this.db.rows("order_line").filter((r) => r.order_id === orderId)
      .sort((a, b) => (a.item < b.item ? -1 : 1)).map((r) => [r.item, r.qty]);
  }
  writeLine(orderId, item, qty) {
    for (const r of this.db.rows("order_line")) {
      if (r.order_id === orderId && r.item === item) r.qty = qty;
    }
  }
}

// Reads an order from the normalized tables.
class NormalizedOrderView extends OrderView {
  static tables = "customer, orders, order_line";
  read(orderId) {
    const head = [];
    for (const o of this.db.rows("orders").filter((o) => o.id === orderId)) {
      for (const c of this.db.rows("customer").filter((c) => c.id === o.customer_id)) head.push([c.name]);
    }
    return [head, this.lines(orderId)];
  }
}

// Reads an order from the tuned tables: one table less to read.
class DenormalizedOrderView extends OrderView {
  static tables = "orders, order_line";
  read(orderId) {
    const head = this.db.rows("orders").filter((o) => o.id === orderId).map((o) => [o.customer_name]);
    return [head, this.lines(orderId)];
  }
}

// ---- Logical access layer: the interface of the application kernel. ----

// The logical view of an order. It knows nothing of tables.
class Order {
  constructor(id, customer, lines) {
    Object.assign(this, { id, customer, lines: new Map(lines), changed: new Set() });
  }
  setQty(item, qty) {
    this.lines.set(item, qty);
    this.changed.add(item);
  }
}

class AccessLayer {
  constructor(physical) {
    this.physical = physical;
    this.start();
  }
  start() {
    this.cache = new Map();
    this.reads = 0;
  }
  getOrder(orderId) {
    if (!this.cache.has(orderId)) {
      const [head, lines] = this.physical.read(orderId);
      this.reads += 1;
      if (head.length === 0) throw new AccessError(`no order ${orderId}`);
      this.cache.set(orderId, new Order(orderId, head[0][0], lines));
    }
    return this.cache.get(orderId);
  }
  commit() {
    const written = [];
    for (const order of this.cache.values()) {
      for (const item of [...order.changed].sort()) {
        this.physical.writeLine(order.id, item, order.lines.get(item));
        written.push(`order ${order.id} ${item} x ${order.lines.get(item)}`);
      }
      order.changed.clear();
    }
    this.physical.db.commit();
    return written;
  }
}

// ---- Application kernel: uses only the logical access layer. ----

function invoice(layer, orderId) {
  const o = layer.getOrder(orderId);
  const lines = [...o.lines].sort((a, b) => (a[0] < b[0] ? -1 : 1)).map(([item, qty]) => `${item} x ${qty}`);
  return [o.customer, ...lines].join(" | ");
}

function run(name, db, ViewClass) {
  const layer = new AccessLayer(new ViewClass(db));
  console.log("physical layout:", name);
  layer.start();
  console.log("invoice 10:", invoice(layer, 10));
  console.log("invoice 10:", invoice(layer, 10));
  console.log(`physical reads: ${layer.reads}, tables ${ViewClass.tables}`);
  layer.getOrder(10).setQty("bolt", 3);
  console.log("commit:", layer.commit().join(", "));
  layer.start();
  console.log("invoice 10 in a new transaction:", invoice(layer, 10));
  try {
    invoice(layer, 99);
  } catch (e) {
    if (!(e instanceof AccessError)) throw e;
    console.log("error:", e.message);
  }
}

run("normalized", normalized(), NormalizedOrderView);
run("denormalized", denormalized(), DenormalizedOrderView);
```

```output
physical layout: normalized
invoice 10: Acme Ltd | bolt x 2 | nut x 5
invoice 10: Acme Ltd | bolt x 2 | nut x 5
physical reads: 1, tables customer, orders, order_line
commit: order 10 bolt x 3
invoice 10 in a new transaction: Acme Ltd | bolt x 3 | nut x 5
error: no order 99
physical layout: denormalized
invoice 10: Acme Ltd | bolt x 2 | nut x 5
invoice 10: Acme Ltd | bolt x 2 | nut x 5
physical reads: 1, tables orders, order_line
commit: order 10 bolt x 3
invoice 10 in a new transaction: Acme Ltd | bolt x 3 | nut x 5
error: no order 99
```

- Verdict: idiom. The example has no database, so `Database` holds the tables as arrays of rows, and the physical views do the joins in code. The layers and the output are the same as in the Python file, which runs SQL in SQLite.
- `AccessError` is a subclass of `Error`, and the kernel catches only that class.
