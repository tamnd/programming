```javascript run id=locking-mapper
class LockError extends Error {}

// Exclusive locks for business transactions, kept until the transaction ends.
class LockManager {
  constructor() {
    this.held = new Map();
  }

  acquire(resource, owner) {
    if (!this.held.has(resource)) this.held.set(resource, owner);
    const holder = this.held.get(resource);
    if (holder !== owner) throw new LockError(`${resource} is locked by ${holder}`);
  }

  releaseAll(owner) {
    for (const [r, o] of [...this.held]) if (o === owner) this.held.delete(r);
  }
}

class CustomerMapper {
  constructor(rows) {
    this.rows = rows;
  }

  find(id) {
    return { id, ...this.rows.get(id) };
  }

  update(customer) {
    this.rows.set(customer.id, { name: customer.name, credit: customer.credit });
  }
}

// Wraps a mapper. Each find takes the lock first, so the application scripts never call the lock manager.
class LockingMapper {
  static taken = 0;

  constructor(mapper, locks, owner) {
    this.mapper = mapper;
    this.locks = locks;
    this.owner = owner;
  }

  find(id) {
    this.locks.acquire(`customer ${id}`, this.owner);
    LockingMapper.taken++;
    return this.mapper.find(id);
  }

  update(customer) {
    this.mapper.update(customer);
  }
}

const rows = new Map([[1, { name: "Acme Ltd", credit: 500 }]]);
const locks = new LockManager();
const plain = new CustomerMapper(rows);

// The framework gives each business transaction a locking mapper and releases its locks at the end.
class BusinessTransaction {
  constructor(owner) {
    this.owner = owner;
    this.customers = new LockingMapper(plain, locks, owner);
  }

  end() {
    locks.releaseAll(this.owner);
  }
}

// The application scripts. Not one of them calls the lock manager.
function openForRename(tx, id) {
  return tx.customers.find(id);
}

function saveRename(tx, customer, name) {
  customer.name = name;
  tx.customers.update(customer);
  tx.end();
}

function raiseCredit(tx, id, amount) {
  const customer = tx.customers.find(id);
  customer.credit += amount;
  tx.customers.update(customer);
  tx.end();
  return customer.credit;
}

function runRaiseCredit(who, tx) {
  try {
    const credit = raiseCredit(tx, 1, 100);
    console.log(`${who} raises the credit of customer 1: ok, credit ${credit}`);
  } catch (e) {
    if (!(e instanceof LockError)) throw e;
    tx.end();
    console.log(`${who} raises the credit of customer 1: refused: ${e.message}`);
  }
}

const ann = new BusinessTransaction("ann");
const bob = new BusinessTransaction("bob");
const customer = openForRename(ann, 1);
console.log("ann opens customer 1 to rename it:", customer.name);
runRaiseCredit("bob", bob);
saveRename(ann, customer, "Acme Group");
console.log("ann saves the new name: Acme Group");
runRaiseCredit("bob", bob);
const last = plain.find(1);
console.log("final:", last.name, "with credit", last.credit);
console.log("locks taken by the mapper:", LockingMapper.taken, "locks held now:", locks.held.size);
```

```output
ann opens customer 1 to rename it: Acme Ltd
bob raises the credit of customer 1: refused: customer 1 is locked by ann
ann saves the new name: Acme Group
bob raises the credit of customer 1: ok, credit 600
final: Acme Group with credit 600
locks taken by the mapper: 2 locks held now: 0
```

- Verdict: idiom. `LockingMapper` has the same methods as `CustomerMapper`, so the scripts use it with no change. JavaScript has no interfaces, and the two classes only need methods with the same names.
- `LockError` extends `Error`, and `runRaiseCredit` checks the class with `instanceof`, so other errors are not caught.
