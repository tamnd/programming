```python run id=unknown-customer
class Customer:
    def __init__(self, customer_id, name, plan):
        self.customer_id = customer_id
        self.name = name
        self.plan = plan

    def is_known(self):
        return True

    def bill_line(self):
        return f"customer {self.customer_id}: {self.name}, plan {self.plan}"


class UnknownCustomer(Customer):
    """The special case: the same interface, with the behavior for a customer with no record."""

    def __init__(self, customer_id):
        super().__init__(customer_id, "current resident", "basic")

    def is_known(self):
        return False


ROWS = {1: ("Ann", "premium"), 2: ("Bob", "basic")}


def find_customer(customer_id):
    if customer_id not in ROWS:
        return UnknownCustomer(customer_id)  # not None
    name, plan = ROWS[customer_id]
    return Customer(customer_id, name, plan)


customers = [find_customer(i) for i in (1, 2, 9)]
for c in customers:
    print(c.bill_line())  # no test for None
print("known customers:", sum(1 for c in customers if c.is_known()))
```

```output
customer 1: Ann, plan premium
customer 2: Bob, plan basic
customer 9: current resident, plan basic
known customers: 2
```

- Verdict: idiom. `UnknownCustomer` is a subclass of `Customer` that overrides `is_known`. Python checks the methods only when the program calls them, so the special case must have each method that the callers use.
- `find_customer` never returns `None`, so the loop calls `bill_line` with no test.
