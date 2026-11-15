```rust run id=logical-and-physical-layers
use std::collections::{BTreeMap, BTreeSet, HashMap};
use std::fmt;

// The error that the application kernel sees, whatever the database says.
#[derive(Debug)]
struct AccessError(String);

impl fmt::Display for AccessError {
    fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result {
        write!(f, "{}", self.0)
    }
}

// ---- Physical access layer: knows the tables. ----

struct LineRow {
    order_id: u32,
    item: &'static str,
    qty: u32,
}

// The example has no database: Database holds the tables in memory.
struct Database {
    customers: HashMap<u32, &'static str>,
    // order id to customer id (normalized) or to customer name (tuned)
    orders_by_customer_id: HashMap<u32, u32>,
    orders_with_name: HashMap<u32, &'static str>,
    lines: Vec<LineRow>,
}

impl Database {
    fn commit(&mut self) {}
}

fn lines() -> Vec<LineRow> {
    vec![LineRow { order_id: 10, item: "bolt", qty: 2 }, LineRow { order_id: 10, item: "nut", qty: 5 }]
}

fn normalized() -> Database {
    Database {
        customers: HashMap::from([(1, "Acme Ltd")]),
        orders_by_customer_id: HashMap::from([(10, 1)]),
        orders_with_name: HashMap::new(),
        lines: lines(),
    }
}

// After tuning: the customer name is copied into the orders table.
fn denormalized() -> Database {
    Database {
        customers: HashMap::new(),
        orders_by_customer_id: HashMap::new(),
        orders_with_name: HashMap::from([(10, "Acme Ltd")]),
        lines: lines(),
    }
}

// The protocol of a physical view of orders: read an order, write a line.
trait OrderView {
    fn db(&mut self) -> &mut Database;
    fn tables(&self) -> &'static str;
    fn read(&mut self, order_id: u32) -> Option<(String, BTreeMap<String, u32>)>;

    fn lines(&mut self, order_id: u32) -> BTreeMap<String, u32> {
        self.db().lines.iter().filter(|r| r.order_id == order_id).map(|r| (r.item.to_string(), r.qty)).collect()
    }

    fn write_line(&mut self, order_id: u32, item: &str, qty: u32) {
        for r in self.db().lines.iter_mut().filter(|r| r.order_id == order_id && r.item == item) {
            r.qty = qty;
        }
    }
}

// Reads an order from the normalized tables.
struct NormalizedOrderView(Database);

impl OrderView for NormalizedOrderView {
    fn db(&mut self) -> &mut Database {
        &mut self.0
    }
    fn tables(&self) -> &'static str {
        "customer, orders, order_line"
    }
    fn read(&mut self, order_id: u32) -> Option<(String, BTreeMap<String, u32>)> {
        let customer_id = *self.0.orders_by_customer_id.get(&order_id)?;
        let name = self.0.customers.get(&customer_id)?.to_string();
        Some((name, self.lines(order_id)))
    }
}

// Reads an order from the tuned tables: one table less to read.
struct DenormalizedOrderView(Database);

impl OrderView for DenormalizedOrderView {
    fn db(&mut self) -> &mut Database {
        &mut self.0
    }
    fn tables(&self) -> &'static str {
        "orders, order_line"
    }
    fn read(&mut self, order_id: u32) -> Option<(String, BTreeMap<String, u32>)> {
        let name = self.0.orders_with_name.get(&order_id)?.to_string();
        Some((name, self.lines(order_id)))
    }
}

// ---- Logical access layer: the interface of the application kernel. ----

// The logical view of an order. It knows nothing of tables.
struct Order {
    id: u32,
    customer: String,
    lines: BTreeMap<String, u32>,
    changed: BTreeSet<String>,
}

impl Order {
    fn set_qty(&mut self, item: &str, qty: u32) {
        self.lines.insert(item.to_string(), qty);
        self.changed.insert(item.to_string());
    }
}

struct AccessLayer {
    physical: Box<dyn OrderView>,
    cache: BTreeMap<u32, Order>,
    reads: u32,
}

impl AccessLayer {
    fn start(&mut self) {
        self.cache.clear();
        self.reads = 0;
    }

    fn get_order(&mut self, order_id: u32) -> Result<&mut Order, AccessError> {
        if !self.cache.contains_key(&order_id) {
            self.reads += 1;
            let (customer, lines) =
                self.physical.read(order_id).ok_or_else(|| AccessError(format!("no order {}", order_id)))?;
            self.cache.insert(order_id, Order { id: order_id, customer, lines, changed: BTreeSet::new() });
        }
        Ok(self.cache.get_mut(&order_id).unwrap())
    }

    fn commit(&mut self) -> Vec<String> {
        let mut written = Vec::new();
        for order in self.cache.values_mut() {
            for item in &order.changed {
                let qty = order.lines[item];
                self.physical.write_line(order.id, item, qty);
                written.push(format!("order {} {} x {}", order.id, item, qty));
            }
            order.changed.clear();
        }
        self.physical.db().commit();
        written
    }
}

// ---- Application kernel: uses only the logical access layer. ----

fn invoice(layer: &mut AccessLayer, order_id: u32) -> Result<String, AccessError> {
    let o = layer.get_order(order_id)?;
    let mut parts = vec![o.customer.clone()];
    for (item, qty) in &o.lines {
        parts.push(format!("{} x {}", item, qty));
    }
    Ok(parts.join(" | "))
}

fn run(name: &str, view: Box<dyn OrderView>) {
    let tables = view.tables();
    let mut layer = AccessLayer { physical: view, cache: BTreeMap::new(), reads: 0 };
    println!("physical layout: {}", name);
    layer.start();
    println!("invoice 10: {}", invoice(&mut layer, 10).unwrap());
    println!("invoice 10: {}", invoice(&mut layer, 10).unwrap());
    println!("physical reads: {}, tables {}", layer.reads, tables);
    layer.get_order(10).unwrap().set_qty("bolt", 3);
    println!("commit: {}", layer.commit().join(", "));
    layer.start();
    println!("invoice 10 in a new transaction: {}", invoice(&mut layer, 10).unwrap());
    if let Err(e) = invoice(&mut layer, 99) {
        println!("error: {}", e);
    }
}

fn main() {
    run("normalized", Box::new(NormalizedOrderView(normalized())));
    run("denormalized", Box::new(DenormalizedOrderView(denormalized())));
}
```

```output
physical layout: normalized
invoice 10: Acme Ltd | bolt x 2 | nut x 5
invoice 10: Acme Ltd | bolt x 2 | nut x 5
physical reads: 1, tables customer, orders, order_line
commit: order 10 bolt x 3
invoice 10 in a new transaction: Acme Ltd | bolt x 3 | nut x 5
error: no order 99
physical layout: denormalized
invoice 10: Acme Ltd | bolt x 2 | nut x 5
invoice 10: Acme Ltd | bolt x 2 | nut x 5
physical reads: 1, tables orders, order_line
commit: order 10 bolt x 3
invoice 10 in a new transaction: Acme Ltd | bolt x 3 | nut x 5
error: no order 99
```

- Verdict: idiom. `OrderView` is a trait with default methods for the shared line access, and the logical layer holds a `Box<dyn OrderView>`. `read` returns an `Option`, and the logical layer turns `None` into an `AccessError`.
- `BTreeMap` and `BTreeSet` keep the lines and the changed items in order, so the output is the same on each run.
