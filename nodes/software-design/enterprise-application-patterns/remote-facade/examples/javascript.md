```javascript run id=one-call-for-an-order
const cents = (n) => `${Math.floor(n / 100)}.${String(n % 100).padStart(2, "0")}`;

// Fine-grained domain objects: small objects with small methods.
class Address {
  #street; #city;
  constructor(street, city) { this.#street = street; this.#city = city; }
  street() { return this.#street; }
  city() { return this.#city; }
  moveTo(street, city) { this.#street = street; this.#city = city; }
}

class Customer {
  #name; #address;
  constructor(name, address) { this.#name = name; this.#address = address; }
  name() { return this.#name; }
  address() { return this.#address; }
}

class Line {
  constructor(item, qty, price) { this.item = item; this.qty = qty; this.price = price; }
  amount() { return this.qty * this.price; }
}

class Order {
  constructor(number, customer, lines) { this.number = number; this.customer = customer; this.lines = lines; }
  total() { return this.lines.reduce((s, l) => s + l.amount(), 0); }
}

// Remote facade: coarse-grained methods over the fine-grained objects, with no domain logic.
class OrderFacade {
  #orders;
  constructor(orders) { this.#orders = orders; }

  getOrder(number) {
    const order = this.#orders.get(number);
    const address = order.customer.address();
    return {
      number: order.number,
      customer: order.customer.name(),
      shipTo: `${address.street()}, ${address.city()}`,
      lines: order.lines.map((l) => ({ item: l.item, qty: l.qty, price: l.price })),
      total: order.total(),
    };
  }

  changeAddress(number, street, city) {
    this.#orders.get(number).customer.address().moveTo(street, city);
  }
}

// The network: each call is counted, and only data crosses it, as JSON text.
class Network {
  constructor(server) { this.server = server; this.calls = 0; }
  call(method, ...args) {
    this.calls++;
    const [name, params] = JSON.parse(JSON.stringify([method, args]));
    const reply = JSON.stringify(this.server[name](...params) ?? null);
    return JSON.parse(reply);
  }
}

const ann = new Customer("Ann", new Address("1 Elm Street", "Leeds"));
const orders = new Map([[7, new Order(7, ann, [new Line("lamp", 2, 2500), new Line("desk", 1, 7500), new Line("pen", 4, 500)])]]);
const net = new Network(new OrderFacade(orders));

// The client: one remote call gets all the data that the screen needs.
const view = net.call("getOrder", 7);
console.log(`order ${view.number} for ${view.customer}: ${view.lines.length} lines, total ${cents(view.total)}`);
for (const line of view.lines) console.log(`  ${line.item} x${line.qty} at ${cents(line.price)}`);
console.log("ship to:", view.shipTo);

net.call("changeAddress", 7, "2 Oak Road", "York");
view.shipTo = "not sent back"; // the client has a copy; this changes nothing on the server
console.log("ship to:", net.call("getOrder", 7).shipTo);
console.log("remote calls:", net.calls);
```

```output
order 7 for Ann: 3 lines, total 145.00
  lamp x2 at 25.00
  desk x1 at 75.00
  pen x4 at 5.00
ship to: 1 Elm Street, Leeds
ship to: 2 Oak Road, York
remote calls: 3
```

- Verdict: idiom. The domain classes use private fields (`#street`), so only their methods can read the data. `OrderFacade` returns a plain object, which `JSON.stringify` can serialize, and has no domain logic.
- `Network.call` changes the request and the reply into JSON text and back. In a real application, the facade would be the handler of an HTTP API, and the client would call it with `fetch`.
