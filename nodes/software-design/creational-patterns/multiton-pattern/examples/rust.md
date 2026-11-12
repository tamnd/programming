```rust run id=multiton
use std::collections::HashMap;
use std::collections::HashSet;
use std::sync::{Arc, LazyLock, Mutex};
use std::thread;

#[derive(Debug)]
pub struct Currency {
    pub code: &'static str,
    pub decimals: u32,
    _private: (), // no code outside this module can build one
}

const VALID: [(&str, u32); 3] = [("USD", 2), ("EUR", 2), ("JPY", 0)];

static INSTANCES: LazyLock<Mutex<HashMap<&'static str, Arc<Currency>>>> =
    LazyLock::new(|| Mutex::new(HashMap::new()));

impl Currency {
    pub fn of(code: &str) -> Result<Arc<Currency>, String> {
        let &(code, decimals) = VALID
            .iter()
            .find(|(c, _)| *c == code)
            .ok_or_else(|| format!("{code}: unknown currency"))?;
        let mut map = INSTANCES.lock().unwrap();
        let c = map
            .entry(code)
            .or_insert_with(|| Arc::new(Currency { code, decimals, _private: () }));
        Ok(Arc::clone(c)) // one instance per key
    }

    pub fn count() -> usize {
        INSTANCES.lock().unwrap().len()
    }
}

fn main() {
    println!("USD has {} decimals", Currency::of("USD").unwrap().decimals);
    println!("JPY has {} decimals", Currency::of("JPY").unwrap().decimals);
    let same = Arc::ptr_eq(&Currency::of("USD").unwrap(), &Currency::of("USD").unwrap());
    println!("same USD object: {same}");
    println!("instances: {}", Currency::count());
    if let Err(e) = Currency::of("XYZ") {
        println!("{e}");
    }

    let handles: Vec<_> = (0..8)
        .map(|_| thread::spawn(|| Arc::as_ptr(&Currency::of("EUR").unwrap()) as usize))
        .collect();
    let seen: HashSet<usize> = handles.into_iter().map(|h| h.join().unwrap()).collect();
    println!("8 threads ask for EUR: {} object", seen.len());
    println!("instances: {}", Currency::count());
}
```

```output
USD has 2 decimals
JPY has 0 decimals
same USD object: true
instances: 2
XYZ: unknown currency
8 threads ask for EUR: 1 object
instances: 3
```

- Verdict: stdlib. A `LazyLock<Mutex<HashMap<_, Arc<_>>>>` is the registry. `Arc` shares one object among the callers, and `Arc::ptr_eq` tests identity.
- The `entry` API makes the check and the insert one call, and the `Mutex` makes that call safe across threads.
- A private field stops other modules from building a `Currency`, so `of` is the only way to get one.
