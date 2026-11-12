```rust run id=lazy
use std::collections::HashMap;
use std::sync::atomic::{AtomicUsize, Ordering};
use std::sync::{Barrier, OnceLock};
use std::thread;

struct Settings {
    config: OnceLock<HashMap<&'static str, u32>>,
    loads: AtomicUsize,
}

impl Settings {
    fn new() -> Settings {
        Settings { config: OnceLock::new(), loads: AtomicUsize::new(0) }
    }

    fn config(&self) -> &HashMap<&'static str, u32> {
        // get_or_init runs the closure once; other threads wait for it.
        self.config.get_or_init(|| {
            println!("loading config"); // stands for a slow read of a file
            self.loads.fetch_add(1, Ordering::SeqCst);
            HashMap::from([("timeout", 30)])
        })
    }

    fn loads(&self) -> usize {
        self.loads.load(Ordering::SeqCst)
    }
}

fn main() {
    let s = Settings::new();
    println!("settings created, loads = {}", s.loads());
    println!("timeout = {}", s.config()["timeout"]);
    println!("timeout = {}", s.config()["timeout"]);
    println!("loads = {}", s.loads());

    let shared = Settings::new();
    let start = Barrier::new(8);
    thread::scope(|scope| {
        for _ in 0..8 {
            scope.spawn(|| {
                start.wait(); // all eight threads ask at the same time
                shared.config();
            });
        }
    });
    println!("8 threads: loads = {}", shared.loads());
}
```

```output
settings created, loads = 0
loading config
timeout = 30
timeout = 30
loads = 1
loading config
8 threads: loads = 1
```

- Verdict: stdlib. `std::sync::OnceLock` is a cell that can be written once, safe for many threads. `std::sync::LazyLock` holds the closure as well, and runs it on the first dereference.
- `std::cell::OnceCell` and `LazyCell` are the single-thread versions, with no synchronization cost.
- The type system marks the difference: a `OnceCell` is not `Sync`, so the compiler rejects code that shares it between threads.
