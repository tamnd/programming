```rust run id=try-lock
use std::sync::{Arc, Barrier, Mutex, TryLockError};
use std::thread;

fn main() {
    let a = Arc::new(Mutex::new(0));
    let b = Arc::new(Mutex::new(0));
    let both_hold = Arc::new(Barrier::new(2));
    let both_tried = Arc::new(Barrier::new(2));

    let spawn = |name: &'static str, first: Arc<Mutex<i32>>, second: Arc<Mutex<i32>>| {
        let (hold, tried) = (Arc::clone(&both_hold), Arc::clone(&both_tried));
        thread::spawn(move || {
            let _guard = first.lock().unwrap();
            hold.wait();
            let busy = matches!(second.try_lock(), Err(TryLockError::WouldBlock));
            tried.wait();
            (name, busy)
        })
    };

    let t1 = spawn("t1", Arc::clone(&a), Arc::clone(&b));
    let t2 = spawn("t2", Arc::clone(&b), Arc::clone(&a));
    for handle in [t1, t2] {
        let (name, busy) = handle.join().unwrap();
        println!("{name}: second lock is held by the other thread: {busy}");
    }
}
```

```output for=try-lock
t1: second lock is held by the other thread: true
t2: second lock is held by the other thread: true
```

- Verdict: stdlib. `std::sync::Mutex` blocks in `lock`. `try_lock` returns `Err(TryLockError::WouldBlock)` at once when another thread holds the lock.
- A deadlock is not undefined behaviour in Rust. The Rust Reference lists deadlocks among the behaviours that are not considered unsafe, so the compiler accepts code that deadlocks.
- A second `lock` in the thread that holds the mutex does not return. The documentation leaves open whether it panics or deadlocks.
- The guard unlocks the mutex when it goes out of scope. Here `_guard` lives to the end of the closure, so each thread holds its first lock until both threads have tried.
