```rust run id=one-call-for-an-order
use std::collections::HashMap;

fn cents(n: i64) -> String {
    format!("{}.{:02}", n / 100, n % 100)
}

// Fine-grained domain objects: small objects with small methods.
struct Address {
    street: String,
    city: String,
}

impl Address {
    fn street(&self) -> &str {
        &self.street
    }
    fn city(&self) -> &str {
        &self.city
    }
    fn move_to(&mut self, street: &str, city: &str) {
        self.street = street.to_string();
        self.city = city.to_string();
    }
}

struct Customer {
    name: String,
    address: Address,
}

struct Line {
    item: &'static str,
    qty: i64,
    price: i64,
}

impl Line {
    fn amount(&self) -> i64 {
        self.qty * self.price
    }
}

struct Order {
    number: u32,
    customer: Customer,
    lines: Vec<Line>,
}

impl Order {
    fn total(&self) -> i64 {
        self.lines.iter().map(Line::amount).sum()
    }
}

// The data that crosses the network: owned values, with no references into the server.
#[derive(Clone)]
struct LineView {
    item: String,
    qty: i64,
    price: i64,
}

#[derive(Clone)]
struct OrderView {
    number: u32,
    customer: String,
    ship_to: String,
    lines: Vec<LineView>,
    total: i64,
}

// Remote facade: coarse-grained methods over the fine-grained objects, with no domain logic.
struct OrderFacade {
    orders: HashMap<u32, Order>,
}

impl OrderFacade {
    fn get_order(&self, number: u32) -> OrderView {
        let o = &self.orders[&number];
        let a = &o.customer.address;
        OrderView {
            number: o.number,
            customer: o.customer.name.clone(),
            ship_to: format!("{}, {}", a.street(), a.city()),
            lines: o.lines.iter().map(|l| LineView { item: l.item.to_string(), qty: l.qty, price: l.price }).collect(),
            total: o.total(),
        }
    }

    fn change_address(&mut self, number: u32, street: &str, city: &str) {
        if let Some(o) = self.orders.get_mut(&number) {
            o.customer.address.move_to(street, city);
        }
    }
}

// The network: each call is counted. A real remote call would also serialize the values.
struct Network {
    server: OrderFacade,
    calls: u32,
}

impl Network {
    fn get_order(&mut self, number: u32) -> OrderView {
        self.calls += 1;
        self.server.get_order(number)
    }
    fn change_address(&mut self, number: u32, street: &str, city: &str) {
        self.calls += 1;
        self.server.change_address(number, street, city);
    }
}

fn main() {
    let ann = Customer {
        name: "Ann".to_string(),
        address: Address { street: "1 Elm Street".to_string(), city: "Leeds".to_string() },
    };
    let lines = vec![
        Line { item: "lamp", qty: 2, price: 2500 },
        Line { item: "desk", qty: 1, price: 7500 },
        Line { item: "pen", qty: 4, price: 500 },
    ];
    let orders = HashMap::from([(7, Order { number: 7, customer: ann, lines })]);
    let mut net = Network { server: OrderFacade { orders }, calls: 0 };

    // The client: one remote call gets all the data that the screen needs.
    let mut view = net.get_order(7);
    println!("order {} for {}: {} lines, total {}", view.number, view.customer, view.lines.len(), cents(view.total));
    for l in &view.lines {
        println!("  {} x{} at {}", l.item, l.qty, cents(l.price));
    }
    println!("ship to: {}", view.ship_to);

    net.change_address(7, "2 Oak Road", "York");
    view.ship_to = "not sent back".to_string(); // the client has a copy; this changes nothing on the server
    println!("ship to: {}", net.get_order(7).ship_to);
    println!("remote calls: {}", net.calls);
}
```

```output
order 7 for Ann: 3 lines, total 145.00
  lamp x2 at 25.00
  desk x1 at 75.00
  pen x4 at 5.00
ship to: 1 Elm Street, Leeds
ship to: 2 Oak Road, York
remote calls: 3
```

- Verdict: idiom. `OrderView` and `LineView` own their strings, so they have no references into the server, and the client can keep `view` while the server changes the order. If `get_order` returned a reference to the order, the borrow checker would refuse the later call to `change_address`.
- `Network` only counts the calls. A real remote call would also serialize the values, for example with the `serde` crate, which is a library and not part of the standard library.
