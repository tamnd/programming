```python run id=customer-and-addresses
import copy


class ConflictError(Exception):
    pass


# The tables. A customer and its addresses form one group, with one shared version.
versions = {"customer 1": 1}
customers = {1: {"name": "Acme Ltd", "lock": "customer 1"}}
addresses = {
    1: {"customer": 1, "street": "1 Elm Street", "primary": False},
    2: {"customer": 1, "street": "9 Oak Road", "primary": False},
}


class Group:
    """A session's copy of the customer and its addresses, with the version of the group."""

    def __init__(self, customer_id):
        self.customer_id = customer_id
        self.lock = customers[customer_id]["lock"]
        self.version = versions[self.lock]
        self.customer = copy.deepcopy(customers[customer_id])
        self.addresses = {a: copy.deepcopy(r) for a, r in addresses.items() if r["customer"] == customer_id}

    def make_primary(self, address_id):
        # The rule of the group: at most one primary address for each customer.
        for a, r in self.addresses.items():
            r["primary"] = a == address_id

    def primary(self):
        return [r["street"] for r in self.addresses.values() if r["primary"]]


checks = 0


def save(group):
    global checks
    checks += 1  # one check for the whole group, however many members changed
    if versions[group.lock] != group.version:
        raise ConflictError(f"the group of customer {group.customer_id} changed (version {versions[group.lock]})")
    versions[group.lock] += 1
    group.version = versions[group.lock]
    customers[group.customer_id] = copy.deepcopy(group.customer)
    for a, r in group.addresses.items():
        addresses[a] = copy.deepcopy(r)


def try_save(who, group):
    try:
        save(group)
        print(f"{who} saves: ok, group version {group.version}")
    except ConflictError as e:
        print(f"{who} saves: conflict: {e}")


ann, bob = Group(1), Group(1)
print("customer 1:", ann.customer["name"], "with", len(ann.addresses), "addresses, group version", ann.version)
ann.make_primary(1)
print("ann makes 1 Elm Street the primary address")
bob.make_primary(2)
print("bob makes 9 Oak Road the primary address")
try_save("ann", ann)
try_save("bob", bob)
print("primary addresses in the database:", ", ".join(r["street"] for r in addresses.values() if r["primary"]))
print("version checks:", checks)
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

- Verdict: idiom. The customer row holds the name of its lock, `customer 1`, and the `versions` table holds one version for that name. Each address belongs to the customer, so a save of the group checks and increases that one version.
- `Group` loads the customer and all its addresses together, and `save` writes them together after one check. The `checks` counter shows that the group needs one check for each save, however many members changed.
