```rust run id=person-registry
use std::collections::HashMap;
use std::sync::{Arc, RwLock};

trait PersonFinder: Send + Sync {
    fn find(&self, person_id: u32) -> String;
}

struct TableFinder(HashMap<u32, String>);

impl PersonFinder for TableFinder {
    fn find(&self, id: u32) -> String {
        self.0.get(&id).cloned().unwrap_or_default()
    }
}

struct StubFinder;

impl PersonFinder for StubFinder {
    fn find(&self, id: u32) -> String {
        format!("test person {}", id)
    }
}

// A well-known object with one explicit method for each service.
#[derive(Default)]
struct Registry {
    person_finder: Option<Arc<dyn PersonFinder>>,
}

static SOLE: RwLock<Registry> = RwLock::new(Registry { person_finder: None });

// The start of the program, or a test, sets the registry.
fn install(registry: Registry) {
    *SOLE.write().unwrap() = registry;
}

fn person_finder() -> Result<Arc<dyn PersonFinder>, &'static str> {
    SOLE.read().unwrap().person_finder.clone().ok_or("no person finder is registered")
}

// Code deep in the program, with no reference to a finder.
fn greet(id: u32) -> Result<String, &'static str> {
    Ok(format!("hello, {}", person_finder()?.find(id)))
}

fn main() {
    let rows = HashMap::from([(1, "ann".to_string()), (2, "bob".to_string())]);
    install(Registry { person_finder: Some(Arc::new(TableFinder(rows))) });
    println!("{}", greet(1).unwrap());
    println!("{}", greet(2).unwrap());

    install(Registry { person_finder: Some(Arc::new(StubFinder)) }); // a test installs a stub
    println!("in a test: {}", greet(1).unwrap());

    install(Registry::default());
    if let Err(e) = greet(1) {
        println!("with an empty registry: {}", e);
    }
}
```

```output
hello, ann
hello, bob
in a test: hello, test person 1
with an empty registry: no person finder is registered
```

- Verdict: idiom. A `static` `RwLock` holds the registry. Rust does not allow a mutable global without a lock or `unsafe`, so the shared state is explicit in the type.
- `Arc<dyn PersonFinder>` lets a caller keep the finder after the read lock is released.
