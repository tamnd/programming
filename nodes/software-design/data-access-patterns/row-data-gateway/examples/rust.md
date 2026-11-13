```rust run id=product-rows
use std::cell::RefCell;
use std::collections::BTreeMap;
use std::fmt;
use std::rc::Rc;

// A stand-in for one database table: the stored columns by id, and an id counter.
#[derive(Default)]
struct Table {
    rows: BTreeMap<u32, (String, u32, u32)>,
    next_id: u32,
}

type Db = Rc<RefCell<Table>>;

// A gateway to one row: the columns are fields, and the row saves itself.
struct ProductRow {
    db: Db,
    id: u32,
    name: String,
    price: u32,
    stock: u32,
}

impl ProductRow {
    fn new(db: &Db, name: &str, price: u32, stock: u32) -> Self {
        ProductRow { db: Rc::clone(db), id: 0, name: name.to_string(), price, stock }
    }

    fn insert(&mut self) -> u32 {
        let mut t = self.db.borrow_mut();
        t.next_id += 1;
        self.id = t.next_id;
        t.rows.insert(self.id, (self.name.clone(), self.price, self.stock));
        self.id
    }

    fn update(&self) {
        self.db.borrow_mut().rows.insert(self.id, (self.name.clone(), self.price, self.stock));
    }

    fn delete(&self) {
        self.db.borrow_mut().rows.remove(&self.id);
    }
}

impl fmt::Display for ProductRow {
    fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result {
        write!(f, "{} {} {} {}", self.id, self.name, self.price, self.stock)
    }
}

// Reads rows and makes one ProductRow for each of them.
struct ProductFinder {
    db: Db,
}

impl ProductFinder {
    fn load(&self, id: u32, (name, price, stock): &(String, u32, u32)) -> ProductRow {
        ProductRow { db: Rc::clone(&self.db), id, name: name.clone(), price: *price, stock: *stock }
    }

    fn find(&self, id: u32) -> Option<ProductRow> {
        let t = self.db.borrow();
        t.rows.get(&id).map(|r| self.load(id, r))
    }

    fn find_all(&self) -> Vec<ProductRow> {
        let t = self.db.borrow();
        t.rows.iter().map(|(id, r)| self.load(*id, r)).collect()
    }

    fn find_low_stock(&self, limit: u32) -> Vec<ProductRow> {
        self.find_all().into_iter().filter(|p| p.stock < limit).collect()
    }
}

fn restock(finder: &ProductFinder, limit: u32, amount: u32) -> Vec<String> {
    let mut done = Vec::new();
    for mut product in finder.find_low_stock(limit) {
        let before = product.stock;
        product.stock += amount;
        product.update();
        done.push(format!("{} {} -> {}", product.name, before, product.stock));
    }
    done
}

fn main() {
    let db: Db = Rc::new(RefCell::new(Table::default()));
    ProductRow::new(&db, "pen", 120, 3).insert();
    let ink = ProductRow::new(&db, "ink", 450, 40).insert();
    ProductRow::new(&db, "pad", 300, 0).insert();
    let finder = ProductFinder { db: Rc::clone(&db) };
    println!("restocked: {}", restock(&finder, 5, 20).join("; "));
    if let Some(p) = finder.find(ink) {
        p.delete();
    }
    let rows: Vec<String> = finder.find_all().iter().map(|p| p.to_string()).collect();
    println!("rows: {}", rows.join(" | "));
    match finder.find(9) {
        Some(p) => println!("find(9): {p}"),
        None => println!("find(9): none"),
    }
}
```

```output
restocked: pen 3 -> 23; pad 0 -> 20
rows: 1 pen 120 23 | 3 pad 300 20
find(9): none
```

- Verdict: idiom. Each gateway holds an `Rc<RefCell<Table>>`, because many row objects share one table and each must be able to write to it.
- `restock` takes each row by value as `mut product`, changes the field, and calls `update`.
