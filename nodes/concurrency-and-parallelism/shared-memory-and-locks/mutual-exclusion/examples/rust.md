```rust run id=counter
use std::sync::Mutex;
use std::thread;

fn main() {
    let counter = Mutex::new(0u32);
    thread::scope(|scope| {
        for _ in 0..4 {
            scope.spawn(|| {
                for _ in 0..10_000 {
                    *counter.lock().unwrap() += 1;
                }
            });
        }
    });
    println!("total: {}", counter.into_inner().unwrap());
}
```

```output
total: 40000
```

- Verdict: stdlib. `Mutex<T>` owns the value that it protects. `lock` returns a `MutexGuard`, the only way to reach the value, and the drop of the guard unlocks.
- `thread::scope` lets the threads borrow `counter` and joins them before it returns. `into_inner` then takes the value out with no lock, because no other reference exists.
- `lock` returns a `Result`, because a panic in a thread that holds the guard poisons the mutex.

This program recovers from a poisoned mutex. The error holds the guard, and `into_inner` gives it back, so the program can check the data and go on.

```rust run id=poison
use std::sync::{Arc, Mutex};
use std::thread;

fn main() {
    let balance = Arc::new(Mutex::new(100));
    let worker_balance = Arc::clone(&balance);
    let outcome = thread::spawn(move || {
        let mut guard = worker_balance.lock().unwrap();
        *guard -= 30;
        panic!("the transfer stopped halfway");
    })
    .join();
    println!("worker failed: {}", outcome.is_err());
    println!("poisoned: {}", balance.is_poisoned());
    let guard = match balance.lock() {
        Ok(guard) => guard,
        Err(poisoned) => poisoned.into_inner(),
    };
    println!("balance after the failed transfer: {}", *guard);
}
```

```output
worker failed: true
poisoned: true
balance after the failed transfer: 70
```

