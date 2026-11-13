```rust run id=product-record
use std::cell::RefCell;
use std::collections::BTreeMap;
use std::fmt;

// What the table stores for one product.
#[derive(Clone)]
struct Row {
    name: String,
    price: u32,
    stock: i32,
}

// A stand-in for the products table: rows by id, with an id counter.
// The Rust standard library has no database driver.
struct Table {
    rows: BTreeMap<u32, Row>,
    next_id: u32,
}

thread_local! {
    static PRODUCTS: RefCell<Table> = RefCell::new(Table { rows: BTreeMap::new(), next_id: 1 });
}

// An Active Record: one value per row, with the data access and the domain logic together.
struct Product {
    id: Option<u32>,
    name: String,
    price: u32,
    stock: i32,
}

impl Product {
    fn new(name: &str, price: u32, stock: i32) -> Self {
        Product { id: None, name: name.to_string(), price, stock }
    }

    // Data access: the finders are associated functions, and saving is a method.
    fn find(id: u32) -> Option<Product> {
        PRODUCTS.with_borrow(|t| t.rows.get(&id).map(|r| Product { id: Some(id), name: r.name.clone(), price: r.price, stock: r.stock }))
    }

    fn all() -> Vec<Product> {
        let ids: Vec<u32> = PRODUCTS.with_borrow(|t| t.rows.keys().copied().collect());
        ids.into_iter().filter_map(Product::find).collect()
    }

    fn validate(&self) -> Result<(), String> {
        if self.stock < 0 {
            return Err(format!("{}: stock cannot be negative", self.name));
        }
        Ok(())
    }

    fn save(&mut self) -> Result<(), String> {
        self.validate()?;
        let row = Row { name: self.name.clone(), price: self.price, stock: self.stock };
        PRODUCTS.with_borrow_mut(|t| {
            let id = *self.id.get_or_insert_with(|| {
                t.next_id += 1;
                t.next_id - 1
            });
            t.rows.insert(id, row);
        });
        Ok(())
    }

    #[allow(dead_code)]
    fn delete(&self) {
        if let Some(id) = self.id {
            PRODUCTS.with_borrow_mut(|t| t.rows.remove(&id));
        }
    }

    // The domain logic is on the same type.
    fn needs_restock(&self) -> bool {
        self.stock < 5
    }

    fn sell(&mut self, quantity: i32) -> Result<u32, String> {
        if quantity > self.stock {
            return Err(format!("{}: only {} in stock", self.name, self.stock));
        }
        self.stock -= quantity;
        Ok(quantity as u32 * self.price)
    }
}

impl fmt::Display for Product {
    fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result {
        write!(f, "{} {} {}", self.id.unwrap_or(0), self.name, self.stock)
    }
}

fn main() -> Result<(), String> {
    for (name, price, stock) in [("pen", 120, 3), ("ink", 450, 40), ("pad", 300, 0)] {
        Product::new(name, price, stock).save()?;
    }

    let mut pen = Product::find(1).ok_or("no pen")?;
    println!("sold 2 pens for {}", pen.sell(2)?);
    pen.save()?;
    let mut restocked = Vec::new();
    for mut product in Product::all() {
        if product.needs_restock() {
            product.stock += 20;
            product.save()?;
            restocked.push(product.name);
        }
    }
    println!("restocked: {}", restocked.join(", "));
    let rows: Vec<String> = Product::all().iter().map(|p| p.to_string()).collect();
    println!("rows: {}", rows.join(" | "));
    let mut ink = Product::find(2).ok_or("no ink")?;
    ink.stock -= 45;
    if let Err(e) = ink.save() {
        println!("not saved: {e}");
    }
    match Product::find(9) {
        Some(p) => println!("find(9): {p}"),
        None => println!("find(9): none"),
    }
    Ok(())
}
```

```output
sold 2 pens for 240
restocked: pen, pad
rows: 1 pen 21 | 2 ink 40 | 3 pad 20
not saved: ink: stock cannot be negative
find(9): none
```

- Verdict: idiom. The finders are associated functions, and `save` returns `Result<(), String>`, so a failed validation is an error value.
- A `thread_local` `RefCell` holds the stand-in table, so the records can reach a shared table without passing it to each call, as an Active Record reaches its connection.
