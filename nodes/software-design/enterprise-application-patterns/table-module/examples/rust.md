```rust run id=one-object-for-all-orders
use std::fmt;

#[derive(Debug)]
struct ModuleError(String);

impl fmt::Display for ModuleError {
    fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result {
        write!(f, "{}", self.0)
    }
}

fn cents(n: i64) -> String {
    format!("{}.{:02}", n / 100, n % 100)
}

// The rows of the record set.
struct CustomerRow {
    id: u32,
    name: &'static str,
    tier: &'static str,
}
struct OrderRow {
    id: u32,
    customer_id: u32,
}
struct OrderLineRow {
    order_id: u32,
    qty: i64,
    price_cents: i64,
}

// The record set: an in-memory copy of the rows of several tables.
struct RecordSet {
    customer: Vec<CustomerRow>,
    orders: Vec<OrderRow>,
    order_line: Vec<OrderLineRow>,
}

fn load_record_set() -> RecordSet {
    let line = |order_id, qty, price_cents| OrderLineRow { order_id, qty, price_cents };
    RecordSet {
        customer: vec![
            CustomerRow { id: 1, name: "Ann", tier: "gold" },
            CustomerRow { id: 2, name: "Bob", tier: "regular" },
            CustomerRow { id: 3, name: "Cy", tier: "staff" },
        ],
        orders: vec![
            OrderRow { id: 1, customer_id: 1 },
            OrderRow { id: 2, customer_id: 2 },
            OrderRow { id: 3, customer_id: 3 },
        ],
        order_line: vec![
            line(1, 2, 2500), line(1, 1, 7500),
            line(2, 1, 2500), line(2, 3, 500),
            line(3, 1, 2500), line(3, 3, 500),
        ],
    }
}

// One table module for each table. One value handles all the rows of its table.
struct CustomerModule<'a> {
    rows: &'a [CustomerRow],
}

impl<'a> CustomerModule<'a> {
    fn row(&self, id: u32) -> Result<&'a CustomerRow, ModuleError> {
        self.rows.iter().find(|r| r.id == id).ok_or_else(|| ModuleError(format!("no customer {}", id)))
    }

    fn names_with_tier(&self, tier: &str) -> Vec<&'a str> {
        self.rows.iter().filter(|r| r.tier == tier).map(|r| r.name).collect()
    }
}

struct OrderModule<'a> {
    rows: &'a [OrderRow],
    lines: &'a [OrderLineRow],
    customers: &'a CustomerModule<'a>,
}

impl<'a> OrderModule<'a> {
    fn row(&self, id: u32) -> Result<&'a OrderRow, ModuleError> {
        self.rows.iter().find(|r| r.id == id).ok_or_else(|| ModuleError(format!("no order {}", id)))
    }

    fn subtotal(&self, id: u32) -> Result<i64, ModuleError> {
        self.row(id)?;
        Ok(self.lines.iter().filter(|l| l.order_id == id).map(|l| l.qty * l.price_cents).sum())
    }

    fn total(&self, id: u32) -> Result<i64, ModuleError> {
        let subtotal = self.subtotal(id)?;
        let tier = self.customers.row(self.row(id)?.customer_id)?.tier;
        Ok(match tier {
            "gold" if subtotal >= 10000 => subtotal - subtotal / 10,
            "staff" => subtotal - subtotal / 5,
            _ => subtotal,
        })
    }

    fn count_over(&self, limit: i64) -> usize {
        self.rows.iter().filter(|r| self.total(r.id).map_or(false, |t| t > limit)).count()
    }
}

fn main() {
    let rs = load_record_set();
    let customers = CustomerModule { rows: &rs.customer };
    let orders = OrderModule { rows: &rs.orders, lines: &rs.order_line, customers: &customers };
    println!(
        "record set: {} customers, {} orders, {} order lines",
        rs.customer.len(),
        rs.orders.len(),
        rs.order_line.len()
    );
    for id in [1, 2, 3] {
        println!(
            "order {}: subtotal {}, total {}",
            id,
            cents(orders.subtotal(id).unwrap()),
            cents(orders.total(id).unwrap())
        );
    }
    println!("gold customers: {}", customers.names_with_tier("gold").join(", "));
    println!("orders over 35.00: {}", orders.count_over(3500));
    if let Err(e) = orders.total(9) {
        println!("error: {}", e);
    }
}
```

```output
record set: 3 customers, 3 orders, 6 order lines
order 1: subtotal 125.00, total 112.50
order 2: subtotal 40.00, total 40.00
order 3: subtotal 40.00, total 32.00
gold customers: Ann
orders over 35.00: 2
error: no order 9
```

- Verdict: idiom. The modules borrow slices of the record set, so the lifetime `'a` ties each module to the record set that it reads. A module cannot outlive its record set, and it cannot change the rows while another module reads them.
