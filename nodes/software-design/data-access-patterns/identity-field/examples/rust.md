```rust run id=insert-then-update
use std::collections::HashMap;

struct Row {
    name: String,
    email: String,
}

// A stand-in for the database: a table keyed by id, and a counter like AUTOINCREMENT.
struct Db {
    person: HashMap<i64, Row>,
    last_row_id: i64,
    // A second table keyed by a meaningful value, the email.
    person_by_email: HashMap<String, String>,
}

struct Person {
    id: Option<i64>, // The Identity Field: None until the row exists.
    name: String,
    email: String,
}

impl Person {
    fn new(name: &str, email: &str) -> Person {
        Person { id: None, name: name.to_string(), email: email.to_string() }
    }

    fn show_id(&self) -> String {
        match self.id {
            None => "none".to_string(),
            Some(id) => id.to_string(),
        }
    }
}

impl Db {
    fn save(&mut self, p: &mut Person) -> usize {
        let row = Row { name: p.name.clone(), email: p.email.clone() };
        match p.id {
            None => {
                self.last_row_id += 1;
                self.person.insert(self.last_row_id, row);
                p.id = Some(self.last_row_id);
                1
            }
            Some(id) => match self.person.get_mut(&id) {
                Some(r) => {
                    *r = row;
                    1
                }
                None => 0,
            },
        }
    }

    fn find(&self, id: i64) -> Person {
        let r = &self.person[&id];
        Person { id: Some(id), name: r.name.clone(), email: r.email.clone() }
    }
}

fn main() {
    let mut db = Db {
        person: HashMap::new(),
        last_row_id: 0,
        person_by_email: HashMap::from([("cy@example.com".to_string(), "Cy".to_string())]),
    };
    let mut ann = Person::new("Ann", "ann@example.com");
    let mut bo = Person::new("Bo", "bo@example.com");
    println!("new: Ann id={} | Bo id={}", ann.show_id(), bo.show_id());
    db.save(&mut ann);
    db.save(&mut bo);
    println!("saved: Ann id={} | Bo id={}", ann.show_id(), bo.show_id());

    ann.email = "ann@new.example".to_string();
    let rows = db.save(&mut ann);
    let id = ann.id.unwrap();
    let r = &db.person[&id];
    println!("update by id: rows {} | {} {} {}", rows, id, r.name, r.email);

    let b = db.find(2);
    println!("find(2): id={} {} {}", b.id.unwrap(), b.name, b.email);

    let first = db.find(1);
    let second = db.find(1);
    let objects = if std::ptr::eq(&first, &second) { "same" } else { "different" };
    let ids = if first.id == second.id { "equal" } else { "different" };
    println!("two loads of row 1: {} objects, {} ids", objects, ids);

    // The same change with a meaningful key: the email is the key.
    let mut cy = Person::new("Cy", "cy@example.com");
    cy.email = "cy@new.example".to_string();
    let mut updated = 0;
    if let Some(name) = db.person_by_email.get_mut(&cy.email) {
        *name = cy.name.clone();
        updated = 1;
    }
    println!("update by email key: rows {}", updated);
}
```

```output
new: Ann id=none | Bo id=none
saved: Ann id=1 | Bo id=2
update by id: rows 1 | 1 Ann ann@new.example
find(2): id=2 Bo bo@example.com
two loads of row 1: different objects, equal ids
update by email key: rows 0
```

- Verdict: idiom. The key is an `Option<i64>`, so the type shows that a new object has no key. `std::ptr::eq` compares addresses, and `==` on the two `Option` values compares the keys.
- The `Db` struct owns the tables, and `save` takes `&mut Person` so that it can set the key after the insert.
