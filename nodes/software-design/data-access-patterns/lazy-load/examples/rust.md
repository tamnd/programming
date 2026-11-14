```rust run id=four-kinds-of-lazy-load
use std::cell::{Cell, OnceCell, RefCell};
use std::collections::HashMap;

// A stand-in for the database: the tables and a count of SELECT statements.
struct Db {
    customers: HashMap<u32, (&'static str, &'static str)>,
    orders: Vec<(u32, u32)>,
    selects: Cell<u32>,
}

impl Db {
    fn select_customer(&self, id: u32) -> (&'static str, &'static str) {
        self.selects.set(self.selects.get() + 1);
        self.customers[&id]
    }
    fn selects(&self) -> u32 {
        self.selects.get()
    }
}

trait Customer {
    fn id(&self) -> u32;
    fn name(&self) -> String;
    fn city(&self) -> String;
}

struct RealCustomer {
    id: u32,
    name: String,
    city: String,
}

impl RealCustomer {
    fn load(db: &Db, id: u32) -> RealCustomer {
        let (name, city) = db.select_customer(id);
        RealCustomer { id, name: name.to_string(), city: city.to_string() }
    }
}

impl Customer for RealCustomer {
    fn id(&self) -> u32 {
        self.id
    }
    fn name(&self) -> String {
        self.name.clone()
    }
    fn city(&self) -> String {
        self.city.clone()
    }
}

// 1. Lazy initialization: an empty OnceCell means "not loaded".
struct Order<'a> {
    db: &'a Db,
    customer_id: u32,
    customer: OnceCell<RealCustomer>,
}

impl Order<'_> {
    fn customer(&self) -> &RealCustomer {
        self.customer.get_or_init(|| RealCustomer::load(self.db, self.customer_id))
    }
}

// 2. Virtual proxy: it implements Customer and loads the real one on first use.
struct CustomerProxy<'a> {
    db: &'a Db,
    id: u32,
    real: OnceCell<RealCustomer>,
}

impl CustomerProxy<'_> {
    fn real(&self) -> &RealCustomer {
        self.real.get_or_init(|| RealCustomer::load(self.db, self.id))
    }
}

impl Customer for CustomerProxy<'_> {
    fn id(&self) -> u32 {
        self.id
    }
    fn name(&self) -> String {
        self.real().name()
    }
    fn city(&self) -> String {
        self.real().city()
    }
}

// 3. Value holder: the client asks the holder for the value.
struct ValueHolder<T, F: Fn() -> T> {
    loader: F,
    value: OnceCell<T>,
}

impl<T, F: Fn() -> T> ValueHolder<T, F> {
    fn get_value(&self) -> &T {
        self.value.get_or_init(&self.loader)
    }
}

// 4. Ghost: the real type, with only the key filled in until first use.
struct GhostCustomer<'a> {
    db: &'a Db,
    id: u32,
    data: RefCell<Option<(String, String)>>,
}

impl GhostCustomer<'_> {
    fn load(&self) {
        if self.data.borrow().is_none() {
            let (name, city) = self.db.select_customer(self.id);
            *self.data.borrow_mut() = Some((name.to_string(), city.to_string()));
        }
    }
}

impl Customer for GhostCustomer<'_> {
    fn id(&self) -> u32 {
        self.id
    }
    fn name(&self) -> String {
        self.load();
        self.data.borrow().as_ref().unwrap().0.clone()
    }
    fn city(&self) -> String {
        self.load();
        self.data.borrow().as_ref().unwrap().1.clone()
    }
}

fn main() {
    let db = Db {
        customers: HashMap::from([(1, ("Ann", "Oslo")), (2, ("Bo", "Rome"))]),
        orders: vec![(10, 1), (11, 2), (12, 1)],
        selects: Cell::new(0),
    };

    db.selects.set(0);
    let order = Order { db: &db, customer_id: 1, customer: OnceCell::new() };
    let before = db.selects();
    let name = order.customer().name();
    let after = db.selects();
    let again = order.customer().name();
    println!("lazy initialization: selects {before} | {name} | selects {after} | {again} | selects {}", db.selects());

    db.selects.set(0);
    let proxy: &dyn Customer = &CustomerProxy { db: &db, id: 2, real: OnceCell::new() };
    let (id, before) = (proxy.id(), db.selects());
    let (name, after) = (proxy.name(), db.selects());
    let city = proxy.city();
    println!("virtual proxy: id {id} | selects {before} | {name} | selects {after} | {city} | selects {}", db.selects());

    db.selects.set(0);
    let holder = ValueHolder { loader: || RealCustomer::load(&db, 1), value: OnceCell::new() };
    let before = db.selects();
    let name = holder.get_value().name();
    println!("value holder: selects {before} | {name} | selects {}", db.selects());

    db.selects.set(0);
    let ghost: &dyn Customer = &GhostCustomer { db: &db, id: 1, data: RefCell::new(None) };
    let (id, before) = (ghost.id(), db.selects());
    let (name, city) = (ghost.name(), ghost.city());
    println!("ghost: id {id} | selects {before} | {name} {city} | selects {}", db.selects());

    db.selects.set(1); // One SELECT reads the orders.
    let names: Vec<String> = db
        .orders
        .iter()
        .map(|&(_, customer_id)| Order { db: &db, customer_id, customer: OnceCell::new() }.customer().name())
        .collect();
    println!("lazy: {} | selects {}", names.join(", "), db.selects());

    db.selects.set(1); // One SELECT with a JOIN reads the orders and their customers.
    let names: Vec<&str> = db.orders.iter().map(|&(_, customer_id)| db.customers[&customer_id].0).collect();
    println!("eager: {} | selects {}", names.join(", "), db.selects());
}
```

```output
lazy initialization: selects 0 | Ann | selects 1 | Ann | selects 1
virtual proxy: id 2 | selects 0 | Bo | selects 1 | Rome | selects 1
value holder: selects 0 | Ann | selects 1
ghost: id 1 | selects 0 | Ann Oslo | selects 1
lazy: Ann, Bo, Ann | selects 4
eager: Ann, Bo, Ann | selects 1
```

- Verdict: idiom. `OnceCell` is the marker: it is empty until `get_or_init` runs the loader once. The ghost uses a `RefCell<Option<...>>`, because it fills its own fields through a shared reference.
- The database is a struct with a `Cell` counter, so the loaders can count statements through a shared `&Db`.
