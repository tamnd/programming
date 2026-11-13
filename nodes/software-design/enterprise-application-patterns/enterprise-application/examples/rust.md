```rust run id=three-layers
use std::collections::BTreeMap;

#[derive(Clone)]
struct Order {
    customer: String,
    item: String,
    quantity: u32,
    cents: u32,
}

#[derive(Clone)]
struct Tables {
    stock: BTreeMap<String, u32>,
    orders: Vec<Order>,
}

// Data source layer: the stored data, and a transaction that keeps all or none of a change.
struct Store {
    tables: Tables,
}

impl Store {
    fn transaction(&mut self, change: impl FnOnce(&mut Tables) -> Result<(), String>) -> bool {
        let saved = self.tables.clone();
        match change(&mut self.tables) {
            Ok(()) => true,
            Err(_) => {
                self.tables = saved; // roll back
                false
            }
        }
    }
}

// Domain layer: the business rules, with a special deal for one customer.
const PRICE_CENTS: u32 = 1000;

fn price(customer: &str, quantity: u32) -> u32 {
    let mut total = PRICE_CENTS * quantity;
    if customer == "acme" && quantity >= 3 {
        total = total * 90 / 100; // a deal that sales made with one customer
    }
    total
}

fn place_order(store: &mut Store, customer: &str, item: &str, quantity: u32) -> bool {
    store.transaction(|t| {
        t.orders.push(Order {
            customer: customer.to_string(),
            item: item.to_string(),
            quantity,
            cents: price(customer, quantity),
        });
        let count = t.stock.get_mut(item).ok_or("no such item")?;
        if *count < quantity {
            return Err("not enough stock".to_string());
        }
        *count -= quantity;
        Ok(())
    })
}

fn money(cents: u32) -> String {
    format!("{}.{:02}", cents / 100, cents % 100)
}

// Presentation layer: two screens show the same data in different ways.
fn order_screen(store: &Store) {
    for o in &store.tables.orders {
        println!("order screen: {} bought {} x {} for {}", o.customer, o.quantity, o.item, money(o.cents));
    }
}

fn stock_screen(store: &Store) {
    for (item, count) in &store.tables.stock {
        println!("stock screen: {}: {} in stock, orders: {}", item, count, store.tables.orders.len());
    }
}

fn result(ok: bool) -> &'static str {
    if ok { "kept" } else { "rolled back" }
}

fn main() {
    let mut store = Store {
        tables: Tables { stock: BTreeMap::from([("widget".to_string(), 10)]), orders: Vec::new() },
    };
    println!("acme orders 3: {}", result(place_order(&mut store, "acme", "widget", 3)));
    println!("bolt orders 9: {}", result(place_order(&mut store, "bolt", "widget", 9)));
    order_screen(&store);
    stock_screen(&store);
}
```

```output
acme orders 3: kept
bolt orders 9: rolled back
order screen: acme bought 3 x widget for 27.00
stock screen: widget: 7 in stock, orders: 1
```

- Verdict: library. Web frameworks and database crates give the requests and the data. The transaction takes a closure that returns a `Result`, and an error restores the clone of the tables.
