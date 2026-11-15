```python run id=assembler-and-wire
import json
from dataclasses import asdict, dataclass, field


def cents(n):
    return f"{n // 100}.{n % 100:02d}"


# Domain objects on the server. Some data must never leave the server.
class Line:
    def __init__(self, item, qty, price, cost_price):
        self.item, self.qty, self.price, self.cost_price = item, qty, price, cost_price


class Order:
    def __init__(self, number, lines):
        self.number, self.lines = number, lines

    def total(self):
        return sum(l.qty * l.price for l in self.lines)


class Customer:
    def __init__(self, name, email, password_hash, orders):
        self.name, self.email, self.password_hash, self.orders = name, email, password_hash, orders


# Data transfer objects: only data, shaped for one remote call, and able to serialize themselves.
@dataclass
class OrderSummaryDTO:
    number: int
    total: int


@dataclass
class CustomerDTO:
    name: str
    email: str
    orders: list = field(default_factory=list)

    def to_wire(self):
        return json.dumps(asdict(self))

    @staticmethod
    def from_wire(text):
        d = json.loads(text)
        return CustomerDTO(d["name"], d["email"], [OrderSummaryDTO(**o) for o in d["orders"]])


# The assembler moves the data between the domain objects and the DTOs, on the server.
class CustomerAssembler:
    @staticmethod
    def to_dto(customer):
        return CustomerDTO(customer.name, customer.email, [OrderSummaryDTO(o.number, o.total()) for o in customer.orders])

    @staticmethod
    def update_from(dto, customer):
        customer.name, customer.email = dto.name, dto.email  # the orders are read only


ann = Customer("Ann", "ann@example.com", "pbkdf2:9f86d081", [
    Order(7, [Line("lamp", 2, 2500, 1400), Line("desk", 1, 7500, 5100), Line("pen", 4, 500, 90)]),
    Order(8, [Line("chair", 1, 4000, 2600)]),
])

# Server: one call returns all the data that the screen needs.
wire = CustomerAssembler.to_dto(ann).to_wire()
print("password hash on the wire:", "yes" if ann.password_hash in wire else "no")

# Client: it reads the DTO, and changes its own copy.
dto = CustomerDTO.from_wire(wire)
print(f"client: {dto.name} <{dto.email}>, {len(dto.orders)} orders")
for o in dto.orders:
    print(f"  order {o.number}: {cents(o.total)}")
dto.email = "ann@example.org"
print("server after a change on the client:", ann.email)

# Client sends the DTO back; the assembler updates the domain object.
CustomerAssembler.update_from(CustomerDTO.from_wire(dto.to_wire()), ann)
print("server after the update call:", ann.email)
```

```output
password hash on the wire: no
client: Ann <ann@example.com>, 2 orders
  order 7: 145.00
  order 8: 40.00
server after a change on the client: ann@example.com
server after the update call: ann@example.org
```

- Verdict: idiom. The DTOs are dataclasses, so they have fields, a constructor and an equality test, and no behavior of their own. `to_wire` and `from_wire` keep the serialization in one place, with `json` and `dataclasses.asdict`.
- The assembler copies only the fields that the client needs. The password hash and the cost prices stay on the server, and `update_from` copies back only the fields that a client may change.
