```rust run id=guard
use std::collections::HashMap;
use std::sync::{Arc, Mutex};
use std::thread;

fn increment(table: &Mutex<HashMap<String, i32>>, path: &str) -> i32 {
    let mut guard = table.lock().unwrap(); // the guard unlocks when it is dropped
    if path.is_empty() {
        return -1;
    }
    *guard.entry(path.to_string()).or_insert(0) += 1;
    0
}

fn main() {
    let table = Arc::new(Mutex::new(HashMap::new()));
    let handles: Vec<_> = (0..4)
        .map(|_| {
            let table = Arc::clone(&table);
            thread::spawn(move || {
                for _ in 0..1000 {
                    increment(&table, "/index");
                }
            })
        })
        .collect();
    for h in handles {
        h.join().unwrap();
    }
    println!("hits /index: {}", table.lock().unwrap()["/index"]);
    println!("bad path returns {}", increment(&table, ""));
    let free = table.try_lock().is_ok();
    println!("lock free after the early return: {}", if free { "True" } else { "False" });
    increment(&table, "/index");
    println!("hits /index: {}", table.lock().unwrap()["/index"]);
}
```

```output
hits /index: 4000
bad path returns -1
lock free after the early return: True
hits /index: 4001
```

- Verdict: native. The data is inside the `Mutex`, so code can reach it only through the guard, and the guard unlocks when it goes out of scope.
