```rust run id=one-script-per-request
use std::collections::BTreeMap;
use std::fmt;

struct Customer {
    name: &'static str,
    tier: &'static str,
}

struct OrderLine {
    order_id: u32,
    product_id: u32,
    qty: i64,
}

// The tables of the database, kept in memory for this example.
struct Db {
    customer: BTreeMap<u32, Customer>,
    product: BTreeMap<u32, (&'static str, i64)>, // name and price in cents
    orders: BTreeMap<u32, u32>,                  // order id to customer id
    order_line: Vec<OrderLine>,
    next_order_id: u32,
}

#[derive(Debug)]
struct ScriptError(String);

impl fmt::Display for ScriptError {
    fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result {
        write!(f, "{}", self.0)
    }
}

fn cents(n: i64) -> String {
    format!("{}.{:02}", n / 100, n % 100)
}

// Transaction script 1: place an order. Each item is (product id, qty).
fn place_order(db: &mut Db, customer_id: u32, items: &[(u32, i64)]) -> Result<(u32, &'static str), ScriptError> {
    let name = match db.customer.get(&customer_id) {
        Some(c) => c.name,
        None => return Err(ScriptError(format!("no customer {}", customer_id))),
    };
    // Check everything first, so that a failed script writes nothing.
    for &(product_id, qty) in items {
        if qty <= 0 {
            return Err(ScriptError(format!("bad quantity {}", qty)));
        }
        if !db.product.contains_key(&product_id) {
            return Err(ScriptError(format!("no product {}", product_id)));
        }
    }
    let order_id = db.next_order_id;
    db.next_order_id += 1;
    db.orders.insert(order_id, customer_id);
    for &(product_id, qty) in items {
        db.order_line.push(OrderLine { order_id, product_id, qty });
    }
    Ok((order_id, name))
}

// Transaction script 2: show the total of an order.
fn order_total(db: &Db, order_id: u32) -> Result<(i64, i64), ScriptError> {
    let customer_id = db
        .orders
        .get(&order_id)
        .ok_or_else(|| ScriptError(format!("no order {}", order_id)))?;
    let tier = db.customer[customer_id].tier;
    let subtotal: i64 = db
        .order_line
        .iter()
        .filter(|l| l.order_id == order_id)
        .map(|l| l.qty * db.product[&l.product_id].1)
        .sum();
    let mut total = subtotal;
    if tier == "gold" && subtotal >= 10000 {
        total = subtotal - subtotal / 10;
    }
    Ok((subtotal, total))
}

fn main() {
    let mut db = Db {
        customer: BTreeMap::from([
            (1, Customer { name: "Ann", tier: "gold" }),
            (2, Customer { name: "Bob", tier: "regular" }),
        ]),
        product: BTreeMap::from([(1, ("lamp", 2500)), (2, ("desk", 7500)), (3, ("pen", 500))]),
        orders: BTreeMap::new(),
        order_line: Vec::new(),
        next_order_id: 1,
    };
    let good: [(u32, &[(u32, i64)]); 2] = [(1, &[(1, 2), (2, 1)]), (2, &[(1, 1), (3, 3)])];
    for (customer_id, items) in good {
        let (order_id, name) = place_order(&mut db, customer_id, items).unwrap();
        println!("placed order {} for {}", order_id, name);
    }
    for order_id in [1, 2] {
        let (subtotal, total) = order_total(&db, order_id).unwrap();
        println!("order {}: subtotal {}, total {}", order_id, cents(subtotal), cents(total));
    }
    let bad: [(u32, &[(u32, i64)]); 2] = [(1, &[(9, 1)]), (2, &[(1, 0)])];
    for (customer_id, items) in bad {
        if let Err(e) = place_order(&mut db, customer_id, items) {
            println!("error: {}", e);
        }
    }
    println!("orders in the database: {}", db.orders.len());
}
```

```output
placed order 1 for Ann
placed order 2 for Bob
order 1: subtotal 125.00, total 112.50
order 2: subtotal 40.00, total 40.00
error: no product 9
error: bad quantity 0
orders in the database: 2
```

- Verdict: idiom. Each script is a function that returns a `Result`. `place_order` takes `&mut Db`, because it writes, and `order_total` takes `&Db`, because it only reads. The compiler checks that no script writes the tables while another script reads them.
