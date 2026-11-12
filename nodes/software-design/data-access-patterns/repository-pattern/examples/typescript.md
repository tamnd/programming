```typescript run id=orders
type Status = "open" | "shipped";

class Order {
  readonly id: number;
  readonly customer: string;
  readonly total: number;
  status: Status;

  constructor(id: number, customer: string, total: number, status: Status = "open") {
    this.id = id;
    this.customer = customer;
    this.total = total;
    this.status = status;
  }

  ship(): void {
    if (this.status !== "open") throw new Error(`order ${this.id} is ${this.status}`);
    this.status = "shipped";
  }
}

// The domain declares the interface. The storage layer implements it.
interface OrderRepository {
  add(order: Order): void;
  get(id: number): Order | undefined;
  byCustomer(customer: string): Order[];
  update(order: Order): void;
}

const copy = (o: Order): Order => new Order(o.id, o.customer, o.total, o.status);

class InMemoryOrderRepository implements OrderRepository {
  readonly #rows = new Map<number, Order>();

  add(order: Order): void {
    if (this.#rows.has(order.id)) throw new Error(`order ${order.id} exists`);
    this.#rows.set(order.id, copy(order));
  }
  get(id: number): Order | undefined {
    const row = this.#rows.get(id);
    return row && copy(row);
  }
  byCustomer(customer: string): Order[] {
    return [...this.#rows.values()]
      .filter((o) => o.customer === customer)
      .sort((a, b) => a.id - b.id)
      .map(copy);
  }
  update(order: Order): void {
    if (!this.#rows.has(order.id)) throw new Error(`order ${order.id} does not exist`);
    this.#rows.set(order.id, copy(order));
  }
}

function shipOrder(orders: OrderRepository, id: number): string {
  const order = orders.get(id);
  if (order === undefined) return `order ${id}: not found`;
  order.ship();
  orders.update(order);
  return `order ${id} is ${order.status}`;
}

const repo: OrderRepository = new InMemoryOrderRepository();
for (const o of [new Order(1, "ana", 30), new Order(2, "bo", 12), new Order(3, "ana", 5)]) repo.add(o);
console.log("ana:", repo.byCustomer("ana").map((o) => `order ${o.id} (${o.total})`).join(", "));
console.log(shipOrder(repo, 1));
console.log(shipOrder(repo, 9));
const stored = repo.get(1)!;
console.log(`stored: order ${stored.id} is ${stored.status}`);
```

```output
ana: order 1 (30), order 3 (5)
order 1 is shipped
order 9: not found
stored: order 1 is shipped
```

- Verdict: idiom. An interface states the collection-like contract in the language of the domain. `shipOrder` depends on the interface only.
- `get` returns `Order | undefined`. With `strictNullChecks`, the compiler rejects `order.ship()` before the check.
- `Status` is a union of two string literals, so the compiler rejects a typo such as `"shiped"` in an assignment to `status`.
