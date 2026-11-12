```typescript run id=order-data
class OrderData {
  readonly orderId: number;
  readonly customer: string;
  constructor(orderId: number, customer: string) {
    this.orderId = orderId;
    this.customer = customer;
  }
}

class Order {
  readonly #data: OrderData; // fixed after construction
  items: string[]; // changes
  constructor(orderId: number, customer: string, items: string[]) {
    this.#data = new OrderData(orderId, customer);
    this.items = [...items];
  }
  get orderId(): number {
    return this.#data.orderId;
  }
  get customer(): string {
    return this.#data.customer;
  }
  add(item: string): void {
    this.items.push(item);
  }
  describe(): string {
    return `order ${this.orderId} for ${this.customer}: ${this.items.length} items`;
  }
}

const order = new Order(42, "ann", ["pen", "ink"]);
console.log(order.describe());
order.add("paper");
console.log(order.describe());
```

```output
order 42 for ann: 2 items
order 42 for ann: 3 items
```

- Verdict: idiom. `readonly` makes the compiler reject an assignment after the constructor, also inside the methods of `Order`.
