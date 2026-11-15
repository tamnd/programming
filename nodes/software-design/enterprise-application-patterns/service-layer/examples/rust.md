```rust run id=one-service-two-clients
use std::collections::BTreeMap;
use std::fmt;

#[derive(Debug)]
struct DomainError(String);

impl fmt::Display for DomainError {
    fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result {
        write!(f, "{}", self.0)
    }
}

fn cents(n: i64) -> String {
    format!("{}.{:02}", n / 100, n % 100)
}

#[derive(Clone)]
struct Customer {
    name: &'static str,
    email: &'static str,
    tier: &'static str,
}

// Domain layer: the objects hold the business rules.
#[derive(Clone)]
struct Order {
    number: usize,
    customer: Customer,
    lines: Vec<(&'static str, i64, i64)>,
}

impl Order {
    fn add(&mut self, item: &'static str, qty: i64, price_cents: i64) -> Result<(), DomainError> {
        if qty <= 0 {
            return Err(DomainError(format!("bad quantity {}", qty)));
        }
        self.lines.push((item, qty, price_cents));
        Ok(())
    }

    fn total(&self) -> i64 {
        let subtotal: i64 = self.lines.iter().map(|&(_, qty, price)| qty * price).sum();
        if self.customer.tier == "gold" && subtotal >= 10000 {
            subtotal - subtotal / 10
        } else {
            subtotal
        }
    }
}

// Data source and other resources, which the service layer coordinates.
struct Store {
    customers: BTreeMap<u32, Customer>,
    prices: BTreeMap<&'static str, i64>,
    orders: BTreeMap<usize, Order>,
    saved: Option<BTreeMap<usize, Order>>,
}

impl Store {
    fn begin(&mut self) {
        self.saved = Some(self.orders.clone()); // a copy of the map of orders
    }
    fn commit(&mut self) {
        self.saved = None;
    }
    fn rollback(&mut self) {
        if let Some(saved) = self.saved.take() {
            self.orders = saved;
        }
    }
}

struct Mailer {
    sent: Vec<String>,
}

// Service layer: the operations of the application, with the transaction and the
// coordination of the responses in one place.
struct OrderService {
    store: Store,
    mailer: Mailer,
}

impl OrderService {
    fn place_order(&mut self, customer_id: u32, items: &[(&'static str, i64)]) -> Result<Order, DomainError> {
        self.store.begin();
        match self.build(customer_id, items) {
            Ok(order) => {
                self.store.commit();
                // Only a committed order gets an email.
                let text = format!("order {}, total {}", order.number, cents(order.total()));
                self.mailer.sent.push(format!("{}: {}", order.customer.email, text));
                Ok(order)
            }
            Err(e) => {
                self.store.rollback();
                Err(e)
            }
        }
    }

    fn build(&mut self, customer_id: u32, items: &[(&'static str, i64)]) -> Result<Order, DomainError> {
        let customer = match self.store.customers.get(&customer_id) {
            Some(c) => c.clone(),
            None => return Err(DomainError(format!("no customer {}", customer_id))),
        };
        let mut order = Order { number: self.store.orders.len() + 1, customer, lines: Vec::new() };
        for &(item, qty) in items {
            let price = *self
                .store
                .prices
                .get(item)
                .ok_or_else(|| DomainError(format!("no product {}", item)))?;
            order.add(item, qty, price)?;
        }
        self.store.orders.insert(order.number, order.clone());
        Ok(order)
    }
}

fn main() {
    let store = Store {
        customers: BTreeMap::from([
            (1, Customer { name: "Ann", email: "ann@example.com", tier: "gold" }),
            (2, Customer { name: "Bob", email: "bob@example.com", tier: "regular" }),
        ]),
        prices: BTreeMap::from([("lamp", 2500), ("desk", 7500), ("pen", 500)]),
        orders: BTreeMap::new(),
        saved: None,
    };
    let mut service = OrderService { store, mailer: Mailer { sent: Vec::new() } };

    // Client 1: a web handler.
    let o = service.place_order(1, &[("lamp", 2), ("desk", 1)]).unwrap();
    println!("web: placed order {} for {}, total {}", o.number, o.customer.name, cents(o.total()));

    // Client 2: a loader that imports orders from a file.
    let rows: [&[(&'static str, i64)]; 2] = [&[("lamp", 1), ("pen", 3)], &[("pen", 1), ("chair", 1)]];
    for (i, items) in rows.iter().enumerate() {
        match service.place_order(2, items) {
            Ok(o) => println!(
                "import line {}: placed order {} for {}, total {}",
                i + 1,
                o.number,
                o.customer.name,
                cents(o.total())
            ),
            Err(e) => println!("import line {}: refused: {}", i + 1, e),
        }
    }

    println!("orders stored: {}", service.store.orders.len());
    for m in &service.mailer.sent {
        println!("email {}", m);
    }
}
```

```output
web: placed order 1 for Ann, total 112.50
import line 1: placed order 2 for Bob, total 40.00
import line 2: refused: no product chair
orders stored: 2
email ann@example.com: order 1, total 112.50
email bob@example.com: order 2, total 40.00
```

- Verdict: idiom. `OrderService` owns the store and the mailer, and `place_order` takes `&mut self`, because it writes. The `match` on the result of `build` is the one place that decides to commit or roll back.
- `build` uses the `?` operator, so the first error ends the operation. The order goes into the store only at the end of `build`, after all the items are correct.
