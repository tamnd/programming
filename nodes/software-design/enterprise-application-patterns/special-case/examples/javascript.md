```javascript run id=unknown-customer
class Customer {
  constructor(id, name, plan) {
    this.id = id;
    this.name = name;
    this.plan = plan;
  }

  isKnown() {
    return true;
  }

  billLine() {
    return `customer ${this.id}: ${this.name}, plan ${this.plan}`;
  }
}

// The special case: the same interface, with the behavior for a customer with no record.
class UnknownCustomer extends Customer {
  constructor(id) {
    super(id, "current resident", "basic");
  }

  isKnown() {
    return false;
  }
}

const ROWS = new Map([
  [1, ["Ann", "premium"]],
  [2, ["Bob", "basic"]],
]);

function findCustomer(id) {
  const row = ROWS.get(id);
  if (row === undefined) return new UnknownCustomer(id); // not null
  return new Customer(id, ...row);
}

const customers = [1, 2, 9].map(findCustomer);
for (const c of customers) console.log(c.billLine()); // no test for null
console.log("known customers:", customers.filter((c) => c.isKnown()).length);
```

```output
customer 1: Ann, plan premium
customer 2: Bob, plan basic
customer 9: current resident, plan basic
known customers: 2
```

- Verdict: idiom. `UnknownCustomer` extends `Customer` and overrides `isKnown`. The finder returns it in place of `null` or `undefined`.
- Optional chaining `?.` is the other answer to a missing value: it avoids the error, but each caller must still choose what to show.
