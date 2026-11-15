```javascript run id=orders-that-know-their-rules
class DomainError extends Error {}

const cents = (n) => `${Math.floor(n / 100)}.${String(n % 100).padStart(2, "0")}`;

// Each kind of customer knows its own discount rule.
class Customer {
  constructor(name) { this.name = name; }
  get kind() { return "regular"; }
  discount(subtotal) { return 0; }
}

class GoldCustomer extends Customer {
  get kind() { return "gold"; }
  discount(subtotal) { return subtotal >= 10000 ? Math.floor(subtotal / 10) : 0; }
}

class StaffCustomer extends Customer {
  get kind() { return "staff"; }
  discount(subtotal) { return Math.floor(subtotal / 5); }
}

class Product {
  constructor(name, priceCents) { this.name = name; this.priceCents = priceCents; }
}

class OrderLine {
  constructor(product, qty) {
    if (qty <= 0) throw new DomainError(`bad quantity ${qty}`);
    this.product = product;
    this.qty = qty;
  }
  amount() { return this.qty * this.product.priceCents; }
}

class Order {
  #lines = [];
  #placed = false;
  constructor(number, customer) { this.number = number; this.customer = customer; }

  add(product, qty) {
    if (this.#placed) throw new DomainError(`order ${this.number} is placed, so it takes no new lines`);
    this.#lines.push(new OrderLine(product, qty));
  }

  place() {
    if (this.#lines.length === 0) throw new DomainError(`order ${this.number} has no lines`);
    this.#placed = true;
  }

  subtotal() { return this.#lines.reduce((sum, line) => sum + line.amount(), 0); }

  total() {
    const subtotal = this.subtotal();
    return subtotal - this.customer.discount(subtotal);
  }
}

const lamp = new Product("lamp", 2500), desk = new Product("desk", 7500), pen = new Product("pen", 500);
const orders = [new Order(1, new GoldCustomer("Ann")), new Order(2, new Customer("Bob")), new Order(3, new StaffCustomer("Cy"))];
orders[0].add(lamp, 2);
orders[0].add(desk, 1);
for (const order of orders.slice(1)) {
  order.add(lamp, 1);
  order.add(pen, 3);
}
for (const order of orders) {
  order.place();
  console.log(`order ${order.number} for ${order.customer.name} (${order.customer.kind}): ` +
    `subtotal ${cents(order.subtotal())}, total ${cents(order.total())}`);
}
for (const action of [() => orders[0].add(pen, 1), () => new Order(4, new Customer("Di")).place(),
  () => new Order(5, new Customer("Ed")).add(pen, 0)]) {
  try {
    action();
  } catch (e) {
    if (!(e instanceof DomainError)) throw e;
    console.log("error:", e.message);
  }
}
```

```output
order 1 for Ann (gold): subtotal 125.00, total 112.50
order 2 for Bob (regular): subtotal 40.00, total 40.00
order 3 for Cy (staff): subtotal 40.00, total 32.00
error: order 1 is placed, so it takes no new lines
error: order 4 has no lines
error: bad quantity 0
```

- Verdict: idiom. The order keeps its lines and its state in private fields (`#lines`, `#placed`), so only its methods can change them. Each kind of customer is a subclass with its own `discount` and `kind`.
