```rust run id=lock-table-with-timeout
use std::collections::BTreeMap;

struct Lock {
    owner: String,
    taken: u32,
}

// The lock manager keeps the locks in a table, so that they last across requests.
// In a real application, the table is in the database and the resource is its primary key.
struct LockManager {
    table: BTreeMap<String, Lock>,
}

const TIMEOUT: u32 = 30; // minutes: a lock that is older than this has been abandoned

impl LockManager {
    fn acquire(&mut self, resource: &str, owner: &str, now: u32) -> Result<(), String> {
        if let Some(l) = self.table.get(resource) {
            if l.taken + TIMEOUT <= now {
                self.table.remove(resource);
            }
        }
        match self.table.get(resource) {
            None => {
                self.table.insert(resource.to_string(), Lock { owner: owner.to_string(), taken: now });
                Ok(())
            }
            Some(l) if l.owner != owner => Err(format!("{} is locked by {}", resource, l.owner)),
            Some(_) => Ok(()),
        }
    }

    fn release_all(&mut self, owner: &str) {
        self.table.retain(|_, l| l.owner != owner);
    }

    fn held(&self) -> String {
        let v: Vec<String> = self.table.iter().map(|(r, l)| format!("{} by {}", r, l.owner)).collect();
        v.join(", ")
    }
}

fn clock(minute: u32) -> String {
    format!("10:{:02}", minute)
}

fn open_for_edit(locks: &mut LockManager, minute: u32, who: &str, resource: &str) {
    match locks.acquire(resource, who, minute) {
        Ok(()) => println!("{} {} opens {} for edit: lock granted", clock(minute), who, resource),
        Err(e) => println!("{} {} opens {} for edit: refused: {}", clock(minute), who, resource, e),
    }
}

fn close(locks: &mut LockManager, minute: u32, who: &str) {
    locks.release_all(who);
    println!("{} {} saves and closes: locks released", clock(minute), who);
}

fn main() {
    let mut locks = LockManager { table: BTreeMap::new() };
    open_for_edit(&mut locks, 0, "ann", "customer 1");
    open_for_edit(&mut locks, 5, "bob", "customer 1");
    close(&mut locks, 12, "ann");
    open_for_edit(&mut locks, 15, "bob", "customer 1");
    open_for_edit(&mut locks, 20, "bob", "order 9");
    // Bob closes the browser and never comes back.
    open_for_edit(&mut locks, 40, "carol", "customer 1");
    open_for_edit(&mut locks, 50, "carol", "customer 1");
    println!("locks held: {}", locks.held());
}
```

```output
10:00 ann opens customer 1 for edit: lock granted
10:05 bob opens customer 1 for edit: refused: customer 1 is locked by ann
10:12 ann saves and closes: locks released
10:15 bob opens customer 1 for edit: lock granted
10:20 bob opens order 9 for edit: lock granted
10:40 carol opens customer 1 for edit: refused: customer 1 is locked by bob
10:50 carol opens customer 1 for edit: lock granted
locks held: customer 1 by carol, order 9 by bob
```

- Verdict: idiom. `acquire` and `release_all` take `&mut self`, so the compiler allows no other access to the table while they run. A server with threads would put the lock manager in a `Mutex`.
- `release_all` uses `retain` to keep only the locks of the other owners.
