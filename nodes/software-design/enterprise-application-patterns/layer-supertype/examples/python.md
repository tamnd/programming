```python run id=domain-object-supertype
class DomainObject:
    """The layer supertype: every domain object has an ID, equality by ID and a check."""

    def __init__(self, id):
        self.id = id

    def __eq__(self, other):
        return type(self) is type(other) and self.id == other.id

    def __hash__(self):
        return hash((type(self).__name__, self.id))

    def rules(self):  # each subclass gives its own rules
        return []

    def check(self):
        problems = [message for ok, message in self.rules() if not ok]
        return "valid" if not problems else "invalid: " + ", ".join(problems)


class Customer(DomainObject):
    def __init__(self, id, name):
        super().__init__(id)
        self.name = name

    def rules(self):
        return [(self.name != "", "name is empty")]


class Product(DomainObject):
    def __init__(self, id, name, price_cents):
        super().__init__(id)
        self.name, self.price_cents = name, price_cents

    def rules(self):
        return [(self.name != "", "name is empty"), (self.price_cents >= 0, "price is negative")]


for o in [Customer(1, "ann"), Product(2, "pen", 150), Product(3, "", -5)]:
    print(f"{type(o).__name__.lower()} {o.id} {o.name or '(no name)'}: {o.check()}")
yes_no = {True: "yes", False: "no"}
print("customer 1 equals customer 1 loaded again:", yes_no[Customer(1, "ann") == Customer(1, "ann")])
print("customer 1 equals product 1:", yes_no[Customer(1, "ann") == Product(1, "ann", 0)])
```

```output
customer 1 ann: valid
product 2 pen: valid
product 3 (no name): invalid: name is empty, price is negative
customer 1 equals customer 1 loaded again: yes
customer 1 equals product 1: no
```

- Verdict: native. `DomainObject` is a normal base class. It defines `__eq__` and `__hash__` once, so two objects for the same row are equal and fit in one set. `check` calls `rules`, which each subclass overrides.
- `type(self) is type(other)` keeps a customer and a product with the same ID apart.
