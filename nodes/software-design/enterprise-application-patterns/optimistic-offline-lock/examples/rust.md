```rust run id=two-sessions-one-customer
use std::collections::HashMap;
use std::fmt;

struct Row {
    name: String,
    credit: i64,
    version: u32,
}

// A session's copy of the record, with the version that it read.
struct Customer {
    id: u32,
    name: String,
    credit: i64,
    version: u32,
}

impl fmt::Display for Customer {
    fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result {
        write!(f, "{}, credit {}, version {}", self.name, self.credit, self.version)
    }
}

struct Conflict(u32);

// The table, kept in memory. `save` takes `&mut self`, so the check and the update
// are one step, like UPDATE ... WHERE version = ? in SQL.
struct Table {
    rows: HashMap<u32, Row>,
}

impl Table {
    fn load(&self, id: u32) -> Customer {
        let r = &self.rows[&id];
        Customer { id, name: r.name.clone(), credit: r.credit, version: r.version }
    }

    fn save(&mut self, c: &mut Customer) -> Result<(), Conflict> {
        let r = self.rows.get_mut(&c.id).ok_or(Conflict(c.id))?;
        if r.version != c.version {
            return Err(Conflict(c.id));
        }
        *r = Row { name: c.name.clone(), credit: c.credit, version: r.version + 1 };
        c.version += 1;
        Ok(())
    }
}

fn try_save(t: &mut Table, who: &str, c: &mut Customer, what: &str) -> bool {
    match t.save(c) {
        Ok(()) => {
            println!("{} saves {}: ok, version {}", who, what, c.version);
            true
        }
        Err(Conflict(id)) => {
            println!("{} saves {}: conflict: customer {} was changed by another session", who, what, id);
            false
        }
    }
}

fn main() {
    let mut t = Table {
        rows: HashMap::from([(1, Row { name: "Acme Ltd".to_string(), credit: 500, version: 1 })]),
    };

    let mut ann = t.load(1);
    println!("ann loads customer 1: {}", ann);
    let mut bob = t.load(1);
    println!("bob loads customer 1: {}", bob);

    bob.credit = 800;
    try_save(&mut t, "bob", &mut bob, "credit 800");

    ann.name = "Acme Group".to_string();
    if !try_save(&mut t, "ann", &mut ann, "name Acme Group") {
        ann = t.load(1); // the session reloads, shows the new data, and the user does the edit again
        println!("ann reloads: {}", ann);
        ann.name = "Acme Group".to_string();
        try_save(&mut t, "ann", &mut ann, "name Acme Group");
    }

    println!("final: {}", t.load(1));
}
```

```output
ann loads customer 1: Acme Ltd, credit 500, version 1
bob loads customer 1: Acme Ltd, credit 500, version 1
bob saves credit 800: ok, version 2
ann saves name Acme Group: conflict: customer 1 was changed by another session
ann reloads: Acme Ltd, credit 800, version 2
ann saves name Acme Group: ok, version 3
final: Acme Group, credit 800, version 3
```

- Verdict: idiom. `save` takes `&mut self`, so no other code can read or write the table between the check of the version and the write. A program with threads would put the table in a `Mutex`.
- `save` returns a `Result`, and the caller must handle the conflict, because the compiler warns on a `Result` that is not used.
