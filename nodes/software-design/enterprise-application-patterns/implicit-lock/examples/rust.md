```rust run id=locking-mapper
use std::cell::RefCell;
use std::collections::HashMap;
use std::rc::Rc;

// Exclusive locks for business transactions, kept until the transaction ends.
#[derive(Default)]
struct LockManager {
    held: HashMap<String, String>,
}

impl LockManager {
    fn acquire(&mut self, resource: &str, owner: &str) -> Result<(), String> {
        let holder = self.held.entry(resource.to_string()).or_insert_with(|| owner.to_string());
        if holder.as_str() != owner {
            return Err(format!("{} is locked by {}", resource, holder));
        }
        Ok(())
    }

    fn release_all(&mut self, owner: &str) {
        self.held.retain(|_, o| o.as_str() != owner);
    }
}

#[derive(Clone)]
struct Customer {
    id: u32,
    name: String,
    credit: i64,
}

trait Mapper {
    fn find(&self, id: u32) -> Result<Customer, String>;
    fn update(&self, c: &Customer);
}

struct CustomerMapper {
    rows: RefCell<HashMap<u32, Customer>>,
}

impl Mapper for CustomerMapper {
    fn find(&self, id: u32) -> Result<Customer, String> {
        Ok(self.rows.borrow()[&id].clone())
    }

    fn update(&self, c: &Customer) {
        self.rows.borrow_mut().insert(c.id, c.clone());
    }
}

// Wraps a mapper. Each find takes the lock first, so the application scripts never call the lock manager.
struct LockingMapper {
    mapper: Rc<CustomerMapper>,
    locks: Rc<RefCell<LockManager>>,
    owner: String,
    taken: Rc<RefCell<u32>>,
}

impl Mapper for LockingMapper {
    fn find(&self, id: u32) -> Result<Customer, String> {
        self.locks.borrow_mut().acquire(&format!("customer {}", id), &self.owner)?;
        *self.taken.borrow_mut() += 1;
        self.mapper.find(id)
    }

    fn update(&self, c: &Customer) {
        self.mapper.update(c);
    }
}

// The framework gives each business transaction a locking mapper and releases its locks at the end.
struct BusinessTransaction {
    owner: String,
    customers: Box<dyn Mapper>,
    locks: Rc<RefCell<LockManager>>,
}

impl BusinessTransaction {
    fn end(&self) {
        self.locks.borrow_mut().release_all(&self.owner);
    }
}

// The application scripts. Not one of them calls the lock manager.
fn open_for_rename(tx: &BusinessTransaction, id: u32) -> Result<Customer, String> {
    tx.customers.find(id)
}

fn save_rename(tx: &BusinessTransaction, mut customer: Customer, name: &str) {
    customer.name = name.to_string();
    tx.customers.update(&customer);
    tx.end();
}

fn raise_credit(tx: &BusinessTransaction, id: u32, amount: i64) -> Result<i64, String> {
    let mut customer = tx.customers.find(id)?;
    customer.credit += amount;
    tx.customers.update(&customer);
    tx.end();
    Ok(customer.credit)
}

fn run_raise_credit(who: &str, tx: &BusinessTransaction) {
    match raise_credit(tx, 1, 100) {
        Ok(credit) => println!("{} raises the credit of customer 1: ok, credit {}", who, credit),
        Err(e) => {
            tx.end();
            println!("{} raises the credit of customer 1: refused: {}", who, e);
        }
    }
}

fn main() {
    let plain = Rc::new(CustomerMapper { rows: RefCell::new(HashMap::new()) });
    plain.update(&Customer { id: 1, name: "Acme Ltd".to_string(), credit: 500 });
    let locks = Rc::new(RefCell::new(LockManager::default()));
    let taken = Rc::new(RefCell::new(0));
    let begin = |owner: &str| BusinessTransaction {
        owner: owner.to_string(),
        customers: Box::new(LockingMapper {
            mapper: Rc::clone(&plain),
            locks: Rc::clone(&locks),
            owner: owner.to_string(),
            taken: Rc::clone(&taken),
        }),
        locks: Rc::clone(&locks),
    };
    let ann = begin("ann");
    let bob = begin("bob");
    let customer = open_for_rename(&ann, 1).expect("ann gets the lock");
    println!("ann opens customer 1 to rename it: {}", customer.name);
    run_raise_credit("bob", &bob);
    save_rename(&ann, customer, "Acme Group");
    println!("ann saves the new name: Acme Group");
    run_raise_credit("bob", &bob);
    let last = plain.find(1).unwrap();
    println!("final: {} with credit {}", last.name, last.credit);
    println!("locks taken by the mapper: {} locks held now: {}", taken.borrow(), locks.borrow().held.len());
}
```

```output
ann opens customer 1 to rename it: Acme Ltd
bob raises the credit of customer 1: refused: customer 1 is locked by ann
ann saves the new name: Acme Group
bob raises the credit of customer 1: ok, credit 600
final: Acme Group with credit 600
locks taken by the mapper: 2 locks held now: 0
```

- Verdict: idiom. `Mapper` is a trait, and the business transaction holds a `Box<dyn Mapper>`, so the scripts can only use the locking mapper. The lock manager and the plain mapper are shared with `Rc<RefCell<...>>`.
- `find` uses the `?` operator, so a refused lock returns from the script before it reads the row.
