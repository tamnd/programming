```rust run id=products-gateway
use std::collections::BTreeMap;

// One row of the products table, as plain data.
#[derive(Clone, Debug)]
struct ProductRow {
    id: u32,
    name: String,
    price: u32,
    stock: u32,
}

// A stand-in for one database table: rows by id and an id counter.
struct Table {
    rows: BTreeMap<u32, ProductRow>,
    next_id: u32,
}

// The gateway holds every operation on the products table.
struct ProductGateway<'a> {
    table: &'a mut Table,
}

impl ProductGateway<'_> {
    fn find(&self, id: u32) -> Option<ProductRow> {
        self.table.rows.get(&id).cloned()
    }

    fn find_all(&self) -> Vec<ProductRow> {
        self.table.rows.values().cloned().collect()
    }

    fn find_low_stock(&self, limit: u32) -> Vec<ProductRow> {
        self.table.rows.values().filter(|r| r.stock < limit).cloned().collect()
    }

    fn insert(&mut self, name: &str, price: u32, stock: u32) -> u32 {
        let id = self.table.next_id;
        self.table.next_id += 1;
        self.table.rows.insert(id, ProductRow { id, name: name.to_string(), price, stock });
        id
    }

    fn update_stock(&mut self, id: u32, stock: u32) {
        if let Some(row) = self.table.rows.get_mut(&id) {
            row.stock = stock;
        }
    }

    fn delete(&mut self, id: u32) {
        self.table.rows.remove(&id);
    }
}

// A transaction script: the rule is here, the data access is in the gateway.
fn restock(products: &mut ProductGateway, limit: u32, amount: u32) -> Vec<String> {
    let mut done = Vec::new();
    for row in products.find_low_stock(limit) {
        products.update_stock(row.id, row.stock + amount);
        done.push(format!("{} {} -> {}", row.name, row.stock, row.stock + amount));
    }
    done
}

fn main() {
    let mut table = Table { rows: BTreeMap::new(), next_id: 1 };
    let mut products = ProductGateway { table: &mut table };
    products.insert("pen", 120, 3);
    let ink = products.insert("ink", 450, 40);
    products.insert("pad", 300, 0);
    println!("restocked: {}", restock(&mut products, 5, 20).join("; "));
    products.delete(ink);
    let rows: Vec<String> = products
        .find_all()
        .iter()
        .map(|r| format!("{} {} {} {}", r.id, r.name, r.price, r.stock))
        .collect();
    println!("rows: {}", rows.join(" | "));
    match products.find(9) {
        Some(row) => println!("find(9): {:?}", row),
        None => println!("find(9): none"),
    }
}
```

```output
restocked: pen 3 -> 23; pad 0 -> 20
rows: 1 pen 120 23 | 3 pad 300 20
find(9): none
```

- Verdict: idiom. The gateway borrows the table mutably, so the borrow checker allows no other access to the table while the gateway exists.
- The finders return cloned rows, so a caller gets data and not a reference into the table.
