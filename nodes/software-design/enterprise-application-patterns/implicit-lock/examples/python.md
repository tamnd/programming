```python run id=locking-mapper
class LockError(Exception):
    pass


class LockManager:
    """Exclusive locks for business transactions, kept until the transaction ends."""

    def __init__(self):
        self.held = {}

    def acquire(self, resource, owner):
        holder = self.held.setdefault(resource, owner)
        if holder != owner:
            raise LockError(f"{resource} is locked by {holder}")

    def release_all(self, owner):
        for resource in [r for r, o in self.held.items() if o == owner]:
            del self.held[resource]


class CustomerMapper:
    def __init__(self, rows):
        self.rows = rows

    def find(self, customer_id):
        return dict(self.rows[customer_id], id=customer_id)

    def update(self, customer):
        self.rows[customer["id"]] = {"name": customer["name"], "credit": customer["credit"]}


class LockingMapper:
    """Wraps a mapper. Each find takes the lock first, so the application scripts never call the lock manager."""

    taken = 0

    def __init__(self, mapper, locks, owner):
        self.mapper, self.locks, self.owner = mapper, locks, owner

    def find(self, customer_id):
        self.locks.acquire(f"customer {customer_id}", self.owner)
        LockingMapper.taken += 1
        return self.mapper.find(customer_id)

    def update(self, customer):
        self.mapper.update(customer)


rows = {1: {"name": "Acme Ltd", "credit": 500}}
locks = LockManager()
plain = CustomerMapper(rows)


class BusinessTransaction:
    """The framework gives each business transaction a locking mapper and releases its locks at the end."""

    def __init__(self, owner):
        self.owner = owner
        self.customers = LockingMapper(plain, locks, owner)

    def end(self):
        locks.release_all(self.owner)


# The application scripts. Not one of them calls the lock manager.
def open_for_rename(tx, customer_id):
    return tx.customers.find(customer_id)


def save_rename(tx, customer, name):
    customer["name"] = name
    tx.customers.update(customer)
    tx.end()


def raise_credit(tx, customer_id, amount):
    customer = tx.customers.find(customer_id)
    customer["credit"] += amount
    tx.customers.update(customer)
    tx.end()
    return customer["credit"]


def run_raise_credit(who, tx):
    try:
        credit = raise_credit(tx, 1, 100)
        print(f"{who} raises the credit of customer 1: ok, credit {credit}")
    except LockError as e:
        tx.end()
        print(f"{who} raises the credit of customer 1: refused: {e}")


ann, bob = BusinessTransaction("ann"), BusinessTransaction("bob")
customer = open_for_rename(ann, 1)
print("ann opens customer 1 to rename it:", customer["name"])
run_raise_credit("bob", bob)
save_rename(ann, customer, "Acme Group")
print("ann saves the new name: Acme Group")
run_raise_credit("bob", bob)
print("final:", plain.find(1)["name"], "with credit", plain.find(1)["credit"])
print("locks taken by the mapper:", LockingMapper.taken, "locks held now:", len(locks.held))
```

```output
ann opens customer 1 to rename it: Acme Ltd
bob raises the credit of customer 1: refused: customer 1 is locked by ann
ann saves the new name: Acme Group
bob raises the credit of customer 1: ok, credit 600
final: Acme Group with credit 600
locks taken by the mapper: 2 locks held now: 0
```

- Verdict: idiom. `LockingMapper` wraps the plain mapper and has the same methods, so the application scripts do not know that a lock is taken. `BusinessTransaction` gives each business transaction its locking mapper, and `end` releases its locks.
- `run_raise_credit` also calls `end` when the lock is refused, so a business transaction that fails keeps no locks.
