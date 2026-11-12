```javascript run id=order-data
class Order {
  #data; // private field: code outside the class cannot reach it
  constructor(orderId, customer, items) {
    this.#data = Object.freeze({ orderId, customer }); // fixed after construction
    this.items = [...items]; // changes
  }
  get orderId() {
    return this.#data.orderId;
  }
  get customer() {
    return this.#data.customer;
  }
  add(item) {
    this.items.push(item);
  }
  describe() {
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

- Verdict: idiom. `Object.freeze` makes the data object read-only, and the `#data` field keeps it away from callers.
