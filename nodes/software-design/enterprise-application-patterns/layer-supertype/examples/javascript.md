```javascript run id=domain-object-supertype
// The layer supertype: every domain object has an ID, equality by ID and a check.
class DomainObject {
  constructor(id) {
    this.id = id;
  }

  equals(other) {
    return other !== null && other.constructor === this.constructor && other.id === this.id;
  }

  rules() {
    return []; // each subclass gives its own rules
  }

  check() {
    const problems = this.rules().filter(([ok]) => !ok).map(([, message]) => message);
    return problems.length === 0 ? "valid" : "invalid: " + problems.join(", ");
  }
}

class Customer extends DomainObject {
  constructor(id, name) {
    super(id);
    this.name = name;
  }

  rules() {
    return [[this.name !== "", "name is empty"]];
  }
}

class Product extends DomainObject {
  constructor(id, name, priceCents) {
    super(id);
    this.name = name;
    this.priceCents = priceCents;
  }

  rules() {
    return [
      [this.name !== "", "name is empty"],
      [this.priceCents >= 0, "price is negative"],
    ];
  }
}

for (const o of [new Customer(1, "ann"), new Product(2, "pen", 150), new Product(3, "", -5)]) {
  console.log(`${o.constructor.name.toLowerCase()} ${o.id} ${o.name || "(no name)"}: ${o.check()}`);
}
const yesNo = (b) => (b ? "yes" : "no");
console.log("customer 1 equals customer 1 loaded again:", yesNo(new Customer(1, "ann").equals(new Customer(1, "ann"))));
console.log("customer 1 equals product 1:", yesNo(new Customer(1, "ann").equals(new Product(1, "ann", 0))));
```

```output
customer 1 ann: valid
product 2 pen: valid
product 3 (no name): invalid: name is empty, price is negative
customer 1 equals customer 1 loaded again: yes
customer 1 equals product 1: no
```

- Verdict: native. `DomainObject` is a base class with `extends`. JavaScript has no operator overloading, so equality is the method `equals`, and `===` still compares the identity of two objects.
- `other.constructor === this.constructor` keeps a customer and a product with the same ID apart.
