```rust run id=one-object-per-row
use std::cell::{Cell, RefCell};
use std::collections::HashMap;
use std::rc::Rc;

struct Person {
    id: u32,
    name: String,
    city: String,
}

// Rc gives shared ownership, RefCell lets the program change a shared object,
// and Rc::ptr_eq compares identity.
type PersonRef = Rc<RefCell<Person>>;

// A stand-in for the people table, because the standard library has no driver.
// Each call of select() stands for one SELECT statement.
struct Database {
    rows: Vec<(u32, &'static str, &'static str)>,
    selects: Cell<u32>,
}

impl Database {
    fn select(&self, filter: impl Fn(&(u32, &str, &str)) -> bool) -> Vec<(u32, &'static str, &'static str)> {
        self.selects.set(self.selects.get() + 1);
        self.rows.iter().filter(|r| filter(r)).copied().collect()
    }
}

struct PersonMapper<'a> {
    db: &'a Database,
    identity_map: HashMap<u32, PersonRef>, // primary key -> the one object for that row
}

impl<'a> PersonMapper<'a> {
    fn new(db: &'a Database) -> Self {
        PersonMapper { db, identity_map: HashMap::new() }
    }

    fn find(&mut self, id: u32) -> Option<PersonRef> {
        if let Some(p) = self.identity_map.get(&id) {
            return Some(Rc::clone(p)); // no query
        }
        let row = self.db.select(|r| r.0 == id).into_iter().next()?;
        Some(self.load(row))
    }

    fn find_by_city(&mut self, city: &str) -> Vec<PersonRef> {
        // The map is keyed by id, so a query by city must run.
        self.db.select(|r| r.2 == city).into_iter().map(|row| self.load(row)).collect()
    }

    fn load(&mut self, (id, name, city): (u32, &str, &str)) -> PersonRef {
        let person = self
            .identity_map
            .entry(id)
            .or_insert_with(|| Rc::new(RefCell::new(Person { id, name: name.into(), city: city.into() })));
        Rc::clone(person)
    }
}

fn main() {
    let db = Database {
        rows: vec![(1, "Ann", "Oslo"), (2, "Bo", "Oslo"), (3, "Cy", "Bergen")],
        selects: Cell::new(0),
    };
    let mut mapper = PersonMapper::new(&db); // one mapper, and one map, for each session
    let a = mapper.find(1).unwrap();
    let b = mapper.find(1).unwrap();
    println!("Rc::ptr_eq(&a, &b): {} | selects: {}", Rc::ptr_eq(&a, &b), db.selects.get());

    let oslo = mapper.find_by_city("Oslo");
    let names: Vec<String> = oslo.iter().map(|p| p.borrow().name.clone()).collect();
    println!("Oslo: {} | oslo[0] is a: {} | selects: {}", names.join(", "), Rc::ptr_eq(&oslo[0], &a), db.selects.get());

    a.borrow_mut().city = "Rome".into(); // a change in memory, not saved yet
    println!("b.city: {}", b.borrow().city);
    let again: Vec<String> = mapper
        .find_by_city("Oslo")
        .iter()
        .map(|p| format!("{} in {}", p.borrow().name, p.borrow().city))
        .collect();
    println!("Oslo again: {}", again.join(", "));
    let other = PersonMapper::new(&db).find(1).unwrap();
    println!("another session: same object: {} (id {})", Rc::ptr_eq(&other, &a), other.borrow().id);
}
```

```output
Rc::ptr_eq(&a, &b): true | selects: 1
Oslo: Ann, Bo | oslo[0] is a: true | selects: 2
b.city: Rome
Oslo again: Ann in Rome, Bo in Oslo
another session: same object: false (id 1)
```

- Verdict: idiom. The map holds `Rc<RefCell<Person>>`, so all callers share one object and can change it, and `Rc::ptr_eq` compares identity.
- The `entry` API with `or_insert_with` makes the object only when the key is not in the map. A map of `Weak` pointers would let objects go when the program drops them, as the Python example does.
