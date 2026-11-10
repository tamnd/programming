```javascript run id=orders
class Order {
  constructor(id, customer, total, status = "open") {
    Object.assign(this, { id, customer, total, status });
  }
  ship() {
    if (this.status !== "open") throw new Error(`order ${this.id} is ${this.status}`);
    this.status = "shipped";
  }
}

// Stores plain records and rebuilds an Order on each read, as a database would.
class InMemoryOrderRepository {
  #rows = new Map();

  add(order) {
    if (this.#rows.has(order.id)) throw new Error(`order ${order.id} exists`);
    this.#rows.set(order.id, { ...order });
  }
  get(id) {
    const row = this.#rows.get(id);
    return row ? Object.assign(new Order(), row) : undefined;
  }
  byCustomer(customer) {
    return [...this.#rows.keys()]
      .sort((a, b) => a - b)
      .map((id) => this.get(id))
      .filter((o) => o.customer === customer);
  }
  update(order) {
    if (!this.#rows.has(order.id)) throw new Error(`order ${order.id} does not exist`);
    this.#rows.set(order.id, { ...order });
  }
}

// Application code. It only needs an object with get and update.
function shipOrder(orders, id) {
  const order = orders.get(id);
  if (!order) return `order ${id}: not found`;
  order.ship();
  orders.update(order);
  return `order ${id} is ${order.status}`;
}

const repo = new InMemoryOrderRepository();
[new Order(1, "ana", 30), new Order(2, "bo", 12), new Order(3, "ana", 5)].forEach((o) => repo.add(o));
console.log("ana:", repo.byCustomer("ana").map((o) => `order ${o.id} (${o.total})`).join(", "));
console.log(shipOrder(repo, 1));
console.log(shipOrder(repo, 9));
const stored = repo.get(1);
console.log(`stored: order ${stored.id} is ${stored.status}`);
```

```output
ana: order 1 (30), order 3 (5)
order 1 is shipped
order 9: not found
stored: order 1 is shipped
```

- Verdict: idiom. JavaScript has no interfaces, so the repository is any object with `add`, `get`, `byCustomer` and `update`. `shipOrder` works with an in-memory class, a SQL class or a class over an HTTP API.
- The repository stores a spread copy of the fields and builds a fresh `Order` on each read. Callers cannot change a stored order without `update`.
- `get` returns `undefined` for a missing order, so the caller must check before it calls `ship`.
