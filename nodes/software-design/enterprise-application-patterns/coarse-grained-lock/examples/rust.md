```rust run id=customer-and-addresses
use std::collections::BTreeMap;

#[derive(Clone)]
struct Customer {
    name: String,
    lock: String,
}

#[derive(Clone)]
struct Address {
    id: u32,
    customer: u32,
    street: String,
    primary: bool,
}

// The tables. A customer and its addresses form one group, with one shared version.
struct Db {
    versions: BTreeMap<String, u32>,
    customers: BTreeMap<u32, Customer>,
    addresses: Vec<Address>,
    checks: u32,
}

// A session's copy of the customer and its addresses, with the version of the group.
struct Group {
    customer_id: u32,
    lock: String,
    version: u32,
    customer: Customer,
    addresses: Vec<Address>,
}

impl Group {
    fn load(db: &Db, customer_id: u32) -> Group {
        let customer = db.customers[&customer_id].clone();
        Group {
            customer_id,
            lock: customer.lock.clone(),
            version: db.versions[&customer.lock],
            addresses: db.addresses.iter().filter(|a| a.customer == customer_id).cloned().collect(),
            customer,
        }
    }

    // The rule of the group: at most one primary address for each customer.
    fn make_primary(&mut self, address_id: u32) {
        for a in &mut self.addresses {
            a.primary = a.id == address_id;
        }
    }
}

fn save(db: &mut Db, group: &mut Group) -> Result<(), String> {
    db.checks += 1; // one check for the whole group, however many members changed
    let current = db.versions[&group.lock];
    if current != group.version {
        return Err(format!("the group of customer {} changed (version {})", group.customer_id, current));
    }
    db.versions.insert(group.lock.clone(), current + 1);
    group.version = current + 1;
    db.customers.insert(group.customer_id, group.customer.clone());
    for a in &group.addresses {
        for row in db.addresses.iter_mut().filter(|r| r.id == a.id) {
            *row = a.clone();
        }
    }
    Ok(())
}

fn try_save(db: &mut Db, who: &str, group: &mut Group) {
    match save(db, group) {
        Ok(()) => println!("{} saves: ok, group version {}", who, group.version),
        Err(e) => println!("{} saves: conflict: {}", who, e),
    }
}

fn main() {
    let mut db = Db {
        versions: BTreeMap::from([("customer 1".to_string(), 1)]),
        customers: BTreeMap::from([(1, Customer { name: "Acme Ltd".to_string(), lock: "customer 1".to_string() })]),
        addresses: vec![
            Address { id: 1, customer: 1, street: "1 Elm Street".to_string(), primary: false },
            Address { id: 2, customer: 1, street: "9 Oak Road".to_string(), primary: false },
        ],
        checks: 0,
    };
    let mut ann = Group::load(&db, 1);
    let mut bob = Group::load(&db, 1);
    println!("customer 1: {} with {} addresses, group version {}", ann.customer.name, ann.addresses.len(), ann.version);
    ann.make_primary(1);
    println!("ann makes 1 Elm Street the primary address");
    bob.make_primary(2);
    println!("bob makes 9 Oak Road the primary address");
    try_save(&mut db, "ann", &mut ann);
    try_save(&mut db, "bob", &mut bob);
    let primary: Vec<&str> = db.addresses.iter().filter(|a| a.primary).map(|a| a.street.as_str()).collect();
    println!("primary addresses in the database: {}", primary.join(", "));
    println!("version checks: {}", db.checks);
}
```

```output
customer 1: Acme Ltd with 2 addresses, group version 1
ann makes 1 Elm Street the primary address
bob makes 9 Oak Road the primary address
ann saves: ok, group version 2
bob saves: conflict: the group of customer 1 changed (version 2)
primary addresses in the database: 1 Elm Street
version checks: 2
```

- Verdict: idiom. `Group::load` clones the rows into the group, and `save` takes `&mut Db`, so the compiler allows no other access to the tables between the check and the write.
- `save` returns `Result<(), String>`, and `try_save` prints the conflict with a `match`.
