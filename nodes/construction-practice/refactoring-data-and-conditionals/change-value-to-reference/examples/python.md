```python run id=one-object-for-one-thing
class Customer:
    def __init__(self, cid: str, email: str) -> None:
        self.id, self.email = cid, email


class CustomerRepository:
    def __init__(self) -> None:
        self._by_id: dict[str, Customer] = {}

    def register(self, cid: str, email: str) -> Customer:
        return self._by_id.setdefault(cid, Customer(cid, email))

    def get(self, cid: str) -> Customer:
        return self._by_id[cid]


class Order:
    def __init__(self, customer_id: str, repo: CustomerRepository) -> None:
        self.customer = repo.get(customer_id)


repo = CustomerRepository()
repo.register("c1", "old@example.org")
a = Order("c1", repo)
b = Order("c1", repo)
a.customer.email = "changed@example.org"
assert b.customer.email == "changed@example.org"
assert a.customer is b.customer
print("change value to reference: tests pass")
```

```output
change value to reference: tests pass
```
