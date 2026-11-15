```javascript run id=one-service-two-clients
class DomainError extends Error {}

const cents = (n) => `${Math.floor(n / 100)}.${String(n % 100).padStart(2, "0")}`;

// Domain layer: the objects hold the business rules.
class Order {
  constructor(number, customer) { this.number = number; this.customer = customer; this.lines = []; }

  add(item, qty, priceCents) {
    if (qty <= 0) throw new DomainError(`bad quantity ${qty}`);
    this.lines.push([item, qty, priceCents]);
  }

  total() {
    const subtotal = this.lines.reduce((s, [, qty, price]) => s + qty * price, 0);
    if (this.customer.tier === "gold" && subtotal >= 10000) return subtotal - Math.floor(subtotal / 10);
    return subtotal;
  }
}

// Data source and other resources, which the service layer coordinates.
class Store {
  constructor() {
    this.customers = new Map([
      [1, { name: "Ann", email: "ann@example.com", tier: "gold" }],
      [2, { name: "Bob", email: "bob@example.com", tier: "regular" }],
    ]);
    this.prices = new Map([["lamp", 2500], ["desk", 7500], ["pen", 500]]);
    this.orders = new Map();
    this.saved = null;
  }
  begin() { this.saved = new Map(this.orders); } // a copy of the map of orders
  commit() { this.saved = null; }
  rollback() { this.orders = this.saved; }
}

class Mailer {
  constructor() { this.sent = []; }
  send(to, text) { this.sent.push(`${to}: ${text}`); }
}

// Service layer: the operations of the application, with the transaction and the
// coordination of the responses in one place.
class OrderService {
  constructor(store, mailer) { this.store = store; this.mailer = mailer; }

  placeOrder(customerId, items) {
    this.store.begin();
    let customer, order;
    try {
      customer = this.store.customers.get(customerId);
      if (!customer) throw new DomainError(`no customer ${customerId}`);
      order = new Order(this.store.orders.size + 1, customer);
      this.store.orders.set(order.number, order);
      for (const [item, qty] of items) {
        if (!this.store.prices.has(item)) throw new DomainError(`no product ${item}`);
        order.add(item, qty, this.store.prices.get(item));
      }
      this.store.commit();
    } catch (e) {
      this.store.rollback();
      throw e;
    }
    // Only a committed order gets an email.
    this.mailer.send(customer.email, `order ${order.number}, total ${cents(order.total())}`);
    return [order.number, customer.name, order.total()];
  }
}

const store = new Store(), mailer = new Mailer();
const service = new OrderService(store, mailer);

// Client 1: a web handler.
const [number, name, total] = service.placeOrder(1, [["lamp", 2], ["desk", 1]]);
console.log(`web: placed order ${number} for ${name}, total ${cents(total)}`);

// Client 2: a loader that imports orders from a file.
const rows = [[2, [["lamp", 1], ["pen", 3]]], [2, [["pen", 1], ["chair", 1]]]];
rows.forEach(([customerId, items], i) => {
  try {
    const [n, who, t] = service.placeOrder(customerId, items);
    console.log(`import line ${i + 1}: placed order ${n} for ${who}, total ${cents(t)}`);
  } catch (e) {
    if (!(e instanceof DomainError)) throw e;
    console.log(`import line ${i + 1}: refused: ${e.message}`);
  }
});

console.log("orders stored:", store.orders.size);
for (const message of mailer.sent) console.log("email", message);
```

```output
web: placed order 1 for Ann, total 112.50
import line 1: placed order 2 for Bob, total 40.00
import line 2: refused: no product chair
orders stored: 2
email ann@example.com: order 1, total 112.50
email bob@example.com: order 2, total 40.00
```

- Verdict: idiom. `OrderService` is a class with one method for each operation. The method uses `try` and `catch` to roll back on any error and then throws the error again, so the client still sees it.
- `DomainError` is a subclass of `Error`. The import client uses `instanceof` to report the errors that the user can correct and lets the other errors go up.
