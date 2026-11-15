```rust run id=orders-that-know-their-rules
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

// Each kind of customer knows its own discount rule.
trait Customer {
    fn name(&self) -> &str;
    fn kind(&self) -> &str {
        "regular"
    }
    fn discount(&self, _subtotal: i64) -> i64 {
        0
    }
}

struct Regular(String);
struct Gold(String);
struct Staff(String);

impl Customer for Regular {
    fn name(&self) -> &str {
        &self.0
    }
}

impl Customer for Gold {
    fn name(&self) -> &str {
        &self.0
    }
    fn kind(&self) -> &str {
        "gold"
    }
    fn discount(&self, subtotal: i64) -> i64 {
        if subtotal >= 10000 {
            subtotal / 10
        } else {
            0
        }
    }
}

impl Customer for Staff {
    fn name(&self) -> &str {
        &self.0
    }
    fn kind(&self) -> &str {
        "staff"
    }
    fn discount(&self, subtotal: i64) -> i64 {
        subtotal / 5
    }
}

#[derive(Clone, Copy)]
struct Product {
    price_cents: i64,
}

struct OrderLine {
    product: Product,
    qty: i64,
}

// The fields are private to the module, so only the methods can change an order.
struct Order {
    number: u32,
    customer: Box<dyn Customer>,
    lines: Vec<OrderLine>,
    placed: bool,
}

impl Order {
    fn new(number: u32, customer: Box<dyn Customer>) -> Order {
        Order { number, customer, lines: Vec::new(), placed: false }
    }

    fn add(&mut self, product: Product, qty: i64) -> Result<(), DomainError> {
        if self.placed {
            return Err(DomainError(format!("order {} is placed, so it takes no new lines", self.number)));
        }
        if qty <= 0 {
            return Err(DomainError(format!("bad quantity {}", qty)));
        }
        self.lines.push(OrderLine { product, qty });
        Ok(())
    }

    fn place(&mut self) -> Result<(), DomainError> {
        if self.lines.is_empty() {
            return Err(DomainError(format!("order {} has no lines", self.number)));
        }
        self.placed = true;
        Ok(())
    }

    fn subtotal(&self) -> i64 {
        self.lines.iter().map(|l| l.qty * l.product.price_cents).sum()
    }

    fn total(&self) -> i64 {
        let subtotal = self.subtotal();
        subtotal - self.customer.discount(subtotal)
    }
}

fn main() {
    let (lamp, desk, pen) = (Product { price_cents: 2500 }, Product { price_cents: 7500 }, Product { price_cents: 500 });
    let mut orders = vec![
        Order::new(1, Box::new(Gold("Ann".into()))),
        Order::new(2, Box::new(Regular("Bob".into()))),
        Order::new(3, Box::new(Staff("Cy".into()))),
    ];
    orders[0].add(lamp, 2).unwrap();
    orders[0].add(desk, 1).unwrap();
    for order in orders.iter_mut().skip(1) {
        order.add(lamp, 1).unwrap();
        order.add(pen, 3).unwrap();
    }
    for order in orders.iter_mut() {
        order.place().unwrap();
        println!(
            "order {} for {} ({}): subtotal {}, total {}",
            order.number,
            order.customer.name(),
            order.customer.kind(),
            cents(order.subtotal()),
            cents(order.total())
        );
    }
    let results = [
        orders[0].add(pen, 1),
        Order::new(4, Box::new(Regular("Di".into()))).place(),
        Order::new(5, Box::new(Regular("Ed".into()))).add(pen, 0),
    ];
    for result in results {
        if let Err(e) = result {
            println!("error: {}", e);
        }
    }
}
```

```output
order 1 for Ann (gold): subtotal 125.00, total 112.50
order 2 for Bob (regular): subtotal 40.00, total 40.00
order 3 for Cy (staff): subtotal 40.00, total 32.00
error: order 1 is placed, so it takes no new lines
error: order 4 has no lines
error: bad quantity 0
```

- Verdict: idiom. `Customer` is a trait with default methods for the regular customer, and `Gold` and `Staff` override them. The order holds a `Box<dyn Customer>`. In a library, `Order` would be in its own module, and its fields would be private there, so other modules would have to use `add` and `place`.
