```rust run id=assembler-and-wire
fn cents(n: i64) -> String {
    format!("{}.{:02}", n / 100, n % 100)
}

// Domain objects on the server. Some data must never leave the server.
#[allow(dead_code)]
struct Line {
    item: &'static str,
    qty: i64,
    price: i64,
    cost_price: i64,
}

struct Order {
    number: u32,
    lines: Vec<Line>,
}

impl Order {
    fn total(&self) -> i64 {
        self.lines.iter().map(|l| l.qty * l.price).sum()
    }
}

struct Customer {
    name: String,
    email: String,
    password_hash: String,
    orders: Vec<Order>,
}

// Data transfer objects: only data, shaped for one remote call, and able to serialize themselves.
struct OrderSummaryDto {
    number: u32,
    total: i64,
}

struct CustomerDto {
    name: String,
    email: String,
    orders: Vec<OrderSummaryDto>,
}

impl CustomerDto {
    // One value on each line: the name, the email, and then one line for each order.
    fn to_wire(&self) -> String {
        let mut s = format!("{}\n{}\n", self.name, self.email);
        for o in &self.orders {
            s.push_str(&format!("{} {}\n", o.number, o.total));
        }
        s
    }

    fn from_wire(text: &str) -> CustomerDto {
        let mut lines = text.lines();
        let name = lines.next().unwrap_or("").to_string();
        let email = lines.next().unwrap_or("").to_string();
        let orders = lines
            .filter_map(|l| {
                let (n, t) = l.split_once(' ')?;
                Some(OrderSummaryDto { number: n.parse().ok()?, total: t.parse().ok()? })
            })
            .collect();
        CustomerDto { name, email, orders }
    }
}

// The assembler moves the data between the domain objects and the DTOs, on the server.
fn to_dto(c: &Customer) -> CustomerDto {
    CustomerDto {
        name: c.name.clone(),
        email: c.email.clone(),
        orders: c.orders.iter().map(|o| OrderSummaryDto { number: o.number, total: o.total() }).collect(),
    }
}

fn update_from(dto: CustomerDto, c: &mut Customer) {
    c.name = dto.name; // the orders are read only
    c.email = dto.email;
}

fn main() {
    let mut ann = Customer {
        name: "Ann".to_string(),
        email: "ann@example.com".to_string(),
        password_hash: "pbkdf2:9f86d081".to_string(),
        orders: vec![
            Order {
                number: 7,
                lines: vec![
                    Line { item: "lamp", qty: 2, price: 2500, cost_price: 1400 },
                    Line { item: "desk", qty: 1, price: 7500, cost_price: 5100 },
                    Line { item: "pen", qty: 4, price: 500, cost_price: 90 },
                ],
            },
            Order { number: 8, lines: vec![Line { item: "chair", qty: 1, price: 4000, cost_price: 2600 }] },
        ],
    };

    // Server: one call returns all the data that the screen needs.
    let wire = to_dto(&ann).to_wire();
    println!("password hash on the wire: {}", if wire.contains(&ann.password_hash) { "yes" } else { "no" });

    // Client: it reads the DTO, and changes its own copy.
    let mut dto = CustomerDto::from_wire(&wire);
    println!("client: {} <{}>, {} orders", dto.name, dto.email, dto.orders.len());
    for o in &dto.orders {
        println!("  order {}: {}", o.number, cents(o.total));
    }
    dto.email = "ann@example.org".to_string();
    println!("server after a change on the client: {}", ann.email);

    // Client sends the DTO back; the assembler updates the domain object.
    update_from(CustomerDto::from_wire(&dto.to_wire()), &mut ann);
    println!("server after the update call: {}", ann.email);
}
```

```output
password hash on the wire: no
client: Ann <ann@example.com>, 2 orders
  order 7: 145.00
  order 8: 40.00
server after a change on the client: ann@example.com
server after the update call: ann@example.org
```

- Verdict: idiom. The DTOs are plain structs. In a real application, the `serde` crate would derive the serialization, but it is a library and not part of the standard library, so `to_wire` writes one value on each line, and a name or an email that has a line break would break this simple format.
- `update_from` takes the DTO by value, so it moves the strings into the domain object and does not copy them.
