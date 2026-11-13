```rust run id=product-mapper
use std::collections::BTreeMap;
use std::fmt;

// A domain object. It has no storage code and no idea that a database exists.
struct Product {
    id: Option<u32>,
    name: String,
    price: u32,
    stock: u32,
    tags: Vec<String>,
}

impl Product {
    fn new(name: &str, price: u32, stock: u32, tags: &[&str]) -> Self {
        Product { id: None, name: name.into(), price, stock, tags: tags.iter().map(|t| t.to_string()).collect() }
    }

    fn sell(&mut self, quantity: u32) -> Result<u32, String> {
        if quantity > self.stock {
            return Err(format!("{}: only {} in stock", self.name, self.stock));
        }
        self.stock -= quantity;
        Ok(quantity * self.price)
    }
}

impl fmt::Display for Product {
    fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result {
        write!(f, "{} {} {} {} [{}]", self.id.unwrap_or(0), self.name, self.price, self.stock, self.tags.join(", "))
    }
}

// A stand-in for the database, because the standard library has no driver.
// The two tables have their own column names.
struct ProductRow {
    title: String,
    price_cents: u32,
    qty: u32,
}

struct TagRow {
    product_id: u32,
    tag: String,
}

#[derive(Default)]
struct Database {
    products: BTreeMap<u32, ProductRow>,
    product_tags: Vec<TagRow>,
    next_id: u32,
}

// The mapper moves products between objects and the two tables.
struct ProductMapper<'a> {
    db: &'a mut Database,
}

impl ProductMapper<'_> {
    fn find(&self, id: u32) -> Option<Product> {
        let row = self.db.products.get(&id)?;
        let mut tags: Vec<String> = self.db.product_tags.iter().filter(|t| t.product_id == id).map(|t| t.tag.clone()).collect();
        tags.sort();
        Some(Product { id: Some(id), name: row.title.clone(), price: row.price_cents, stock: row.qty, tags })
    }

    fn insert(&mut self, p: &mut Product) {
        self.db.next_id += 1;
        p.id = Some(self.db.next_id);
        self.update(p);
    }

    fn update(&mut self, p: &Product) {
        let id = p.id.expect("update needs a stored product");
        self.db.products.insert(id, ProductRow { title: p.name.clone(), price_cents: p.price, qty: p.stock });
        self.delete_tags(id);
        for tag in &p.tags {
            self.db.product_tags.push(TagRow { product_id: id, tag: tag.clone() });
        }
    }

    fn delete(&mut self, p: &Product) {
        if let Some(id) = p.id {
            self.delete_tags(id);
            self.db.products.remove(&id);
        }
    }

    fn delete_tags(&mut self, id: u32) {
        self.db.product_tags.retain(|t| t.product_id != id);
    }
}

fn main() -> Result<(), String> {
    let mut db = Database::default();
    let mut mapper = ProductMapper { db: &mut db };
    mapper.insert(&mut Product::new("pen", 120, 3, &["office"]));
    let mut ink = Product::new("ink", 450, 40, &["office"]);
    mapper.insert(&mut ink);

    let mut pen = mapper.find(1).ok_or("no pen")?;
    println!("sold: {}", pen.sell(2)?);
    pen.tags.push("sale".into());
    mapper.update(&pen);
    println!("object: {}", mapper.find(1).ok_or("no pen")?);
    let rows: Vec<String> = mapper.db.products.iter().map(|(id, r)| format!("{id} {} {} {}", r.title, r.price_cents, r.qty)).collect();
    println!("products: {}", rows.join(" | "));
    let mut tag_rows: Vec<String> = mapper.db.product_tags.iter().map(|t| format!("{} {}", t.product_id, t.tag)).collect();
    tag_rows.sort();
    println!("product_tags: {}", tag_rows.join(" | "));
    mapper.delete(&ink);
    println!("after delete: {} product, {} tags", mapper.db.products.len(), mapper.db.product_tags.len());
    match mapper.find(2) {
        Some(p) => println!("find(2): {p}"),
        None => println!("find(2): none"),
    }
    Ok(())
}
```

```output
sold: 240
object: 1 pen 120 1 [office, sale]
products: 1 pen 120 1 | 2 ink 450 40
product_tags: 1 office | 1 sale | 2 office
after delete: 1 product, 2 tags
find(2): none
```

- Verdict: idiom. `Product` has no reference to the database, and `ProductMapper` borrows the database mutably for its lifetime.
- `find` uses `?` on the `Option` of the row lookup, so a missing row gives `None`.
