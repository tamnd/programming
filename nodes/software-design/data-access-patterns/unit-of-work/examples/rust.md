```rust run id=transfer-and-close
use std::cell::RefCell;
use std::collections::BTreeMap;
use std::rc::Rc;

struct Account {
    id: u32,
    owner: String,
    balance: i64,
}

struct Entry {
    account_id: u32,
    amount: i64,
}

// The unit of work holds shared pointers, so it can compare identity with Rc::ptr_eq.
#[derive(Clone)]
enum Obj {
    Account(Rc<RefCell<Account>>),
    Entry(Rc<Entry>),
}

impl Obj {
    fn same(&self, other: &Obj) -> bool {
        match (self, other) {
            (Obj::Account(a), Obj::Account(b)) => Rc::ptr_eq(a, b),
            (Obj::Entry(a), Obj::Entry(b)) => Rc::ptr_eq(a, b),
            _ => false,
        }
    }
}

// A stand-in for the database, because the standard library has no driver.
#[derive(Clone)]
struct Database {
    accounts: BTreeMap<u32, (String, i64)>,
    entries: Vec<(u32, i64)>,
}

impl Database {
    // Runs the work on a copy of the tables and keeps the copy only when all of it succeeds.
    fn transaction(&mut self, work: impl FnOnce(&mut Database) -> Result<(), String>) -> Result<(), String> {
        let mut copy = self.clone();
        work(&mut copy)?;
        *self = copy;
        Ok(())
    }
}

fn find_account(db: &Database, id: u32) -> Rc<RefCell<Account>> {
    let (owner, balance) = db.accounts[&id].clone();
    Rc::new(RefCell::new(Account { id, owner, balance }))
}

// The mapper functions write each kind of object.
fn insert(db: &mut Database, obj: &Obj) -> Result<(), String> {
    match obj {
        Obj::Entry(e) => {
            println!("  insert entries ({}, {})", e.account_id, e.amount);
            db.entries.push((e.account_id, e.amount));
            Ok(())
        }
        Obj::Account(_) => Err("no insert for accounts".into()),
    }
}

fn update(db: &mut Database, obj: &Obj) -> Result<(), String> {
    match obj {
        Obj::Account(a) => {
            let a = a.borrow();
            println!("  update accounts {} {} {}", a.id, a.owner, a.balance);
            if a.balance < 0 {
                return Err("CHECK constraint failed: balance >= 0".into());
            }
            db.accounts.insert(a.id, (a.owner.clone(), a.balance));
            Ok(())
        }
        Obj::Entry(_) => Err("no update for entries".into()),
    }
}

fn delete(db: &mut Database, obj: &Obj) -> Result<(), String> {
    match obj {
        Obj::Account(a) => {
            println!("  delete accounts {}", a.borrow().id);
            db.accounts.remove(&a.borrow().id);
            Ok(())
        }
        Obj::Entry(_) => Err("no delete for entries".into()),
    }
}

#[derive(Default)]
struct UnitOfWork {
    new: Vec<Obj>,
    dirty: Vec<Obj>,
    removed: Vec<Obj>,
}

impl UnitOfWork {
    fn register_new(&mut self, obj: Obj) {
        self.new.push(obj);
    }

    fn register_dirty(&mut self, obj: Obj) {
        if !self.new.iter().chain(&self.dirty).any(|o| o.same(&obj)) {
            self.dirty.push(obj);
        }
    }

    fn register_removed(&mut self, obj: Obj) {
        if self.new.iter().any(|o| o.same(&obj)) {
            self.new.retain(|o| !o.same(&obj)); // never written, nothing to delete
            return;
        }
        self.dirty.retain(|o| !o.same(&obj)); // no update before the delete
        self.removed.push(obj);
    }

    fn commit(self, db: &mut Database) {
        let result = db.transaction(|tx| {
            self.new.iter().try_for_each(|o| insert(tx, o))?;
            self.dirty.iter().try_for_each(|o| update(tx, o))?;
            self.removed.iter().try_for_each(|o| delete(tx, o))
        });
        match result {
            Ok(()) => println!("  commit"),
            Err(e) => println!("  rollback: {e}"),
        }
    }
}

fn transfer(uow: &mut UnitOfWork, source: &Rc<RefCell<Account>>, target: &Rc<RefCell<Account>>, amount: i64) {
    source.borrow_mut().balance -= amount;
    target.borrow_mut().balance += amount;
    uow.register_dirty(Obj::Account(source.clone()));
    uow.register_dirty(Obj::Account(target.clone()));
    uow.register_new(Obj::Entry(Rc::new(Entry { account_id: source.borrow().id, amount: -amount })));
    uow.register_new(Obj::Entry(Rc::new(Entry { account_id: target.borrow().id, amount })));
}

fn show(db: &Database) {
    let accounts: Vec<String> = db.accounts.iter().map(|(id, (o, b))| format!("{id} {o} {b}")).collect();
    let entries: Vec<String> = db.entries.iter().map(|(a, m)| format!("{a} {m}")).collect();
    println!("accounts: {}", accounts.join(" | "));
    println!("entries: {}", entries.join(" | "));
}

fn main() {
    let mut db = Database {
        accounts: BTreeMap::from([(1, ("ann".to_string(), 100)), (2, ("bob".to_string(), 0))]),
        entries: Vec::new(),
    };

    let mut uow = UnitOfWork::default();
    transfer(&mut uow, &find_account(&db, 1), &find_account(&db, 2), 30);
    println!("pending: {} new, {} dirty; entries in the table: {}", uow.new.len(), uow.dirty.len(), db.entries.len());
    println!("commit 1");
    uow.commit(&mut db);

    let mut uow = UnitOfWork::default();
    transfer(&mut uow, &find_account(&db, 2), &find_account(&db, 1), 50); // bob has only 30
    println!("commit 2");
    uow.commit(&mut db);
    show(&db);

    let mut uow = UnitOfWork::default();
    let (ann, bob) = (find_account(&db, 1), find_account(&db, 2));
    transfer(&mut uow, &bob, &ann, 30); // close bob's account: move the money, then remove it
    uow.register_removed(Obj::Account(bob.clone()));
    println!("commit 3");
    uow.commit(&mut db);
    show(&db);
}
```

```output
pending: 2 new, 2 dirty; entries in the table: 0
commit 1
  insert entries (1, -30)
  insert entries (2, 30)
  update accounts 1 ann 70
  update accounts 2 bob 30
  commit
commit 2
  insert entries (2, -50)
  insert entries (1, 50)
  update accounts 2 bob -20
  rollback: CHECK constraint failed: balance >= 0
accounts: 1 ann 70 | 2 bob 30
entries: 1 -30 | 2 30
commit 3
  insert entries (2, -30)
  insert entries (1, 30)
  update accounts 1 ann 100
  delete accounts 2
  commit
accounts: 1 ann 100
entries: 1 -30 | 2 30 | 2 -30 | 1 30
```

- Verdict: idiom. The objects are `Rc` values, and `Obj::same` compares them with `Rc::ptr_eq`. `Account` is in a `RefCell`, because the business code changes an account that the unit of work also holds.
- `commit` takes `self`, so the compiler stops a second commit of the same unit of work. `Database::transaction` writes to a copy of the tables and keeps the copy only when every step returns `Ok`.
