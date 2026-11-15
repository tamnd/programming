```python run id=order-warehouse-mapper
# Subsystem 1: the order system. It knows nothing about the warehouse.
class Order:
    def __init__(self, number, customer, lines):
        self.number, self.customer, self.lines = number, customer, lines  # lines: (item, qty)
        self.status = "new"


# Subsystem 2: the warehouse. It knows nothing about orders. It works with
# product codes and reference strings, and it cannot be changed.
class Warehouse:
    CODES = {"PEN-01", "INK-02"}

    def pick(self, request):
        known = [(code, qty) for code, qty in request["lines"] if code in self.CODES]
        return {"ref": request["ref"], "state": "PICKED" if len(known) == len(request["lines"]) else "SHORT"}


# The mapper knows both subsystems and connects them. Neither of them knows the mapper.
class OrderWarehouseMapper:
    CODE_OF = {"pen": "PEN-01", "ink": "INK-02"}
    STATUS_OF = {"PICKED": "picked", "SHORT": "waiting for stock"}

    def __init__(self, warehouse):
        self.warehouse = warehouse

    def send(self, order):
        request = {"ref": f"ORD-{order.number}",
                   "lines": [(self.CODE_OF[item], qty) for item, qty in order.lines]}
        print("warehouse gets: ref", request["ref"] + ", lines",
              ", ".join(f"{code} x{qty}" for code, qty in request["lines"]))
        reply = self.warehouse.pick(request)
        print("warehouse replies: ref", reply["ref"] + ", state", reply["state"])
        order.status = self.STATUS_OF[reply["state"]]


order = Order(7, "ann", [("pen", 2), ("ink", 1)])
print(f"order {order.number} for {order.customer}:",
      ", ".join(f"{qty} x {item}" for item, qty in order.lines) + f", status {order.status}")
OrderWarehouseMapper(Warehouse()).send(order)
print(f"order {order.number} status: {order.status}")
```

```output
order 7 for ann: 2 x pen, 1 x ink, status new
warehouse gets: ref ORD-7, lines PEN-01 x2, INK-02 x1
warehouse replies: ref ORD-7, state PICKED
order 7 status: picked
```

- Verdict: idiom. The mapper is a normal class with two tables, `CODE_OF` for the item names and `STATUS_OF` for the states. `Order` and `Warehouse` have no import or reference to each other or to the mapper.
- The mapper is the only code that changes when the warehouse gets new product codes.
