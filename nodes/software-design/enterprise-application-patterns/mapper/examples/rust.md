```rust run id=order-warehouse-mapper
// Subsystem 1: the order system. It knows nothing about the warehouse.
struct Order {
    number: u32,
    customer: String,
    lines: Vec<(String, u32)>, // (item, qty)
    status: String,
}

// Subsystem 2: the warehouse. It knows nothing about orders. It works with
// product codes and reference strings, and it cannot be changed.
struct PickRequest {
    reference: String,
    lines: Vec<(String, u32)>, // (code, qty)
}

struct PickReply {
    reference: String,
    state: &'static str,
}

struct Warehouse;

impl Warehouse {
    const CODES: [&'static str; 2] = ["PEN-01", "INK-02"];

    fn pick(&self, request: &PickRequest) -> PickReply {
        let known = request.lines.iter().filter(|(c, _)| Self::CODES.contains(&c.as_str())).count();
        let state = if known == request.lines.len() { "PICKED" } else { "SHORT" };
        PickReply { reference: request.reference.clone(), state }
    }
}

// The mapper knows both subsystems and connects them. Neither of them knows the mapper.
struct OrderWarehouseMapper {
    warehouse: Warehouse,
}

impl OrderWarehouseMapper {
    fn code_of(item: &str) -> &'static str {
        match item {
            "pen" => "PEN-01",
            "ink" => "INK-02",
            _ => "UNKNOWN",
        }
    }

    fn status_of(state: &str) -> &'static str {
        if state == "PICKED" { "picked" } else { "waiting for stock" }
    }

    fn send(&self, order: &mut Order) {
        let request = PickRequest {
            reference: format!("ORD-{}", order.number),
            lines: order.lines.iter().map(|(i, q)| (Self::code_of(i).to_string(), *q)).collect(),
        };
        let lines: Vec<String> = request.lines.iter().map(|(c, q)| format!("{} x{}", c, q)).collect();
        println!("warehouse gets: ref {}, lines {}", request.reference, lines.join(", "));
        let reply = self.warehouse.pick(&request);
        println!("warehouse replies: ref {}, state {}", reply.reference, reply.state);
        order.status = Self::status_of(reply.state).to_string();
    }
}

fn main() {
    let mut order = Order {
        number: 7,
        customer: "ann".to_string(),
        lines: vec![("pen".to_string(), 2), ("ink".to_string(), 1)],
        status: "new".to_string(),
    };
    let lines: Vec<String> = order.lines.iter().map(|(i, q)| format!("{} x {}", q, i)).collect();
    println!("order {} for {}: {}, status {}", order.number, order.customer, lines.join(", "), order.status);
    OrderWarehouseMapper { warehouse: Warehouse }.send(&mut order);
    println!("order {} status: {}", order.number, order.status);
}
```

```output
order 7 for ann: 2 x pen, 1 x ink, status new
warehouse gets: ref ORD-7, lines PEN-01 x2, INK-02 x1
warehouse replies: ref ORD-7, state PICKED
order 7 status: picked
```

- Verdict: idiom. `code_of` and `status_of` are `match` expressions. `send` takes `&mut Order`, so the borrow checker shows that only the mapper changes the order.
