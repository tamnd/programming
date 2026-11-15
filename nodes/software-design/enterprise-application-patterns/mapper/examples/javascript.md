```javascript run id=order-warehouse-mapper
// Subsystem 1: the order system. It knows nothing about the warehouse.
class Order {
  constructor(number, customer, lines) {
    this.number = number;
    this.customer = customer;
    this.lines = lines; // [item, qty]
    this.status = "new";
  }
}

// Subsystem 2: the warehouse. It knows nothing about orders. It works with
// product codes and reference strings, and it cannot be changed.
class Warehouse {
  static CODES = new Set(["PEN-01", "INK-02"]);

  pick(request) {
    const known = request.lines.filter(([code]) => Warehouse.CODES.has(code));
    return { ref: request.ref, state: known.length === request.lines.length ? "PICKED" : "SHORT" };
  }
}

// The mapper knows both subsystems and connects them. Neither of them knows the mapper.
class OrderWarehouseMapper {
  static CODE_OF = { pen: "PEN-01", ink: "INK-02" };
  static STATUS_OF = { PICKED: "picked", SHORT: "waiting for stock" };

  constructor(warehouse) {
    this.warehouse = warehouse;
  }

  send(order) {
    const request = {
      ref: `ORD-${order.number}`,
      lines: order.lines.map(([item, qty]) => [OrderWarehouseMapper.CODE_OF[item], qty]),
    };
    const lines = request.lines.map(([code, qty]) => `${code} x${qty}`).join(", ");
    console.log(`warehouse gets: ref ${request.ref}, lines ${lines}`);
    const reply = this.warehouse.pick(request);
    console.log(`warehouse replies: ref ${reply.ref}, state ${reply.state}`);
    order.status = OrderWarehouseMapper.STATUS_OF[reply.state];
  }
}

const order = new Order(7, "ann", [["pen", 2], ["ink", 1]]);
const lines = order.lines.map(([item, qty]) => `${qty} x ${item}`).join(", ");
console.log(`order ${order.number} for ${order.customer}: ${lines}, status ${order.status}`);
new OrderWarehouseMapper(new Warehouse()).send(order);
console.log(`order ${order.number} status: ${order.status}`);
```

```output
order 7 for ann: 2 x pen, 1 x ink, status new
warehouse gets: ref ORD-7, lines PEN-01 x2, INK-02 x1
warehouse replies: ref ORD-7, state PICKED
order 7 status: picked
```

- Verdict: idiom. The tables of the mapper are static fields. The request and the reply are plain objects, the format of the warehouse.
