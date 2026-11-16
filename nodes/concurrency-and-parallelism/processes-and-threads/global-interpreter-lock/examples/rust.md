```rust run id=threads-and-the-gil
use std::sync::atomic::{AtomicU64, Ordering};
use std::thread;

fn main() {
    // Rust threads are operating system threads with no global lock, so they can run at the same time.
    // The compiler does not allow a plain shared counter here: it must be atomic, or behind a Mutex.
    let total = AtomicU64::new(0);
    thread::scope(|s| {
        for _ in 0..4 {
            s.spawn(|| {
                for _ in 0..100_000 {
                    total.fetch_add(1, Ordering::Relaxed);
                }
            });
        }
    });
    println!("4 threads added 100000 each with AtomicU64");
    println!("total: {}", total.load(Ordering::Relaxed));
}
```

```output for=threads-and-the-gil
4 threads added 100000 each with AtomicU64
total: 400000
```

- Verdict: absent. Rust threads are operating system threads, and the Rust book describes them as independent parts of a program "that run simultaneously".
- Atomic types "provide primitive shared-memory communication between threads", so the example uses `AtomicU64` for the shared total. `Ordering::Relaxed` is enough for a counter that the main thread reads after the joins.
