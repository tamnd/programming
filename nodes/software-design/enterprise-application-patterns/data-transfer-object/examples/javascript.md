```javascript run id=assembler-and-wire
const cents = (n) => `${Math.floor(n / 100)}.${String(n % 100).padStart(2, "0")}`;

// Domain objects on the server. Some data must never leave the server.
class Line {
  constructor(item, qty, price, costPrice) { Object.assign(this, { item, qty, price, costPrice }); }
}

class Order {
  constructor(number, lines) { this.number = number; this.lines = lines; }
  total() { return this.lines.reduce((s, l) => s + l.qty * l.price, 0); }
}

class Customer {
  constructor(name, email, passwordHash, orders) { Object.assign(this, { name, email, passwordHash, orders }); }
}

// Data transfer objects: only data, shaped for one remote call, and able to serialize themselves.
class CustomerDTO {
  constructor(name, email, orders) { this.name = name; this.email = email; this.orders = orders; }
  toWire() { return JSON.stringify(this); }
  static fromWire(text) {
    const d = JSON.parse(text);
    return new CustomerDTO(d.name, d.email, d.orders.map((o) => ({ number: o.number, total: o.total })));
  }
}

// The assembler moves the data between the domain objects and the DTOs, on the server.
const CustomerAssembler = {
  toDTO(customer) {
    return new CustomerDTO(customer.name, customer.email, customer.orders.map((o) => ({ number: o.number, total: o.total() })));
  },
  updateFrom(dto, customer) {
    customer.name = dto.name; // the orders are read only
    customer.email = dto.email;
  },
};

const ann = new Customer("Ann", "ann@example.com", "pbkdf2:9f86d081", [
  new Order(7, [new Line("lamp", 2, 2500, 1400), new Line("desk", 1, 7500, 5100), new Line("pen", 4, 500, 90)]),
  new Order(8, [new Line("chair", 1, 4000, 2600)]),
]);

// Server: one call returns all the data that the screen needs.
const wire = CustomerAssembler.toDTO(ann).toWire();
console.log("password hash on the wire:", wire.includes(ann.passwordHash) ? "yes" : "no");

// Client: it reads the DTO, and changes its own copy.
const dto = CustomerDTO.fromWire(wire);
console.log(`client: ${dto.name} <${dto.email}>, ${dto.orders.length} orders`);
for (const o of dto.orders) console.log(`  order ${o.number}: ${cents(o.total)}`);
dto.email = "ann@example.org";
console.log("server after a change on the client:", ann.email);

// Client sends the DTO back; the assembler updates the domain object.
CustomerAssembler.updateFrom(CustomerDTO.fromWire(dto.toWire()), ann);
console.log("server after the update call:", ann.email);
```

```output
password hash on the wire: no
client: Ann <ann@example.com>, 2 orders
  order 7: 145.00
  order 8: 40.00
server after a change on the client: ann@example.com
server after the update call: ann@example.org
```

- Verdict: idiom. `JSON.stringify` serializes the own fields of the DTO, so the DTO must hold only the data for the wire. If the server passed the `Customer` object itself, `JSON.stringify` would send the `passwordHash` field too.
- `fromWire` builds new objects from the parsed data, so the client never gets a reference to an object on the server.
