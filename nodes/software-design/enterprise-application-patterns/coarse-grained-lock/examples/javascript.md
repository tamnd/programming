```javascript run id=customer-and-addresses
class ConflictError extends Error {}

// The tables. A customer and its addresses form one group, with one shared version.
const versions = new Map([["customer 1", 1]]);
const customers = new Map([[1, { name: "Acme Ltd", lock: "customer 1" }]]);
const addresses = new Map([
  [1, { customer: 1, street: "1 Elm Street", primary: false }],
  [2, { customer: 1, street: "9 Oak Road", primary: false }],
]);

// A session's copy of the customer and its addresses, with the version of the group.
class Group {
  constructor(customerId) {
    this.customerId = customerId;
    this.lock = customers.get(customerId).lock;
    this.version = versions.get(this.lock);
    this.customer = { ...customers.get(customerId) };
    this.addresses = new Map();
    for (const [a, r] of addresses) if (r.customer === customerId) this.addresses.set(a, { ...r });
  }

  // The rule of the group: at most one primary address for each customer.
  makePrimary(addressId) {
    for (const [a, r] of this.addresses) r.primary = a === addressId;
  }
}

let checks = 0;

function save(group) {
  checks++; // one check for the whole group, however many members changed
  const current = versions.get(group.lock);
  if (current !== group.version) {
    throw new ConflictError(`the group of customer ${group.customerId} changed (version ${current})`);
  }
  versions.set(group.lock, current + 1);
  group.version = current + 1;
  customers.set(group.customerId, { ...group.customer });
  for (const [a, r] of group.addresses) addresses.set(a, { ...r });
}

function trySave(who, group) {
  try {
    save(group);
    console.log(`${who} saves: ok, group version ${group.version}`);
  } catch (e) {
    if (!(e instanceof ConflictError)) throw e;
    console.log(`${who} saves: conflict: ${e.message}`);
  }
}

const ann = new Group(1);
const bob = new Group(1);
console.log(`customer 1: ${ann.customer.name} with ${ann.addresses.size} addresses, group version ${ann.version}`);
ann.makePrimary(1);
console.log("ann makes 1 Elm Street the primary address");
bob.makePrimary(2);
console.log("bob makes 9 Oak Road the primary address");
trySave("ann", ann);
trySave("bob", bob);
const primary = [...addresses.values()].filter((r) => r.primary).map((r) => r.street);
console.log("primary addresses in the database:", primary.join(", "));
console.log("version checks:", checks);
```

```output
customer 1: Acme Ltd with 2 addresses, group version 1
ann makes 1 Elm Street the primary address
bob makes 9 Oak Road the primary address
ann saves: ok, group version 2
bob saves: conflict: the group of customer 1 changed (version 2)
primary addresses in the database: 1 Elm Street
version checks: 2
```

- Verdict: idiom. The tables are `Map` objects, and the group copies each row with the spread syntax, so the copies of the two sessions do not share objects with the tables.
- `save` reads the shared version, compares it with the version that the group read, and then writes the customer and the addresses. There is no `await` in `save`, so no other code can run between the check and the write.
