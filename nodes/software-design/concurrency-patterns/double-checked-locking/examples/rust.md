```rust run id=dcl
use std::sync::atomic::{AtomicUsize, Ordering};
use std::sync::{Barrier, Mutex, OnceLock};
use std::thread;

static CREATED: AtomicUsize = AtomicUsize::new(0);
static LOCKED_CALLS: AtomicUsize = AtomicUsize::new(0);

struct Config;

impl Config {
    fn new() -> Config {
        CREATED.fetch_add(1, Ordering::SeqCst);
        Config
    }
}

static INSTANCE: OnceLock<Config> = OnceLock::new();
static LOCK: Mutex<()> = Mutex::new(());

fn instance() -> &'static Config {
    if let Some(config) = INSTANCE.get() {
        return config; // first check, no lock
    }
    let _guard = LOCK.lock().unwrap();
    LOCKED_CALLS.fetch_add(1, Ordering::SeqCst);
    if INSTANCE.get().is_none() {
        // second check, lock held
        let _ = INSTANCE.set(Config::new());
    }
    INSTANCE.get().unwrap()
}

fn main() {
    let start = Barrier::new(8);
    thread::scope(|s| {
        for _ in 0..8 {
            s.spawn(|| {
                start.wait();
                for _ in 0..1000 {
                    instance();
                }
            });
        }
    });
    let locked = LOCKED_CALLS.load(Ordering::SeqCst);
    println!("instances created: {}", CREATED.load(Ordering::SeqCst));
    println!("calls: {}", 8 * 1000);
    if locked <= 8 {
        println!("calls that took the lock: 8 or fewer");
    } else {
        println!("calls that took the lock: {locked}");
    }
}
```

```output
instances created: 1
calls: 8000
calls that took the lock: 8 or fewer
```

- Verdict: idiom. `OnceLock::get` is the first check and needs no lock. `OnceLock::get_or_init` alone is the library form of the same work.
