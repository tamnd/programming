```rust run id=atomic
use std::sync::atomic::{AtomicBool, AtomicU32, AtomicUsize, Ordering};
use std::thread;

fn main() {
    let hits = AtomicUsize::new(0);
    thread::scope(|s| {
        for _ in 0..4 {
            s.spawn(|| {
                for _ in 0..100_000 {
                    hits.fetch_add(1, Ordering::Relaxed);
                }
            });
        }
    });
    println!("{}", hits.load(Ordering::Relaxed));

    let swapped = hits.compare_exchange(400_000, 0, Ordering::AcqRel, Ordering::Acquire);
    println!("{:?} {}", swapped, hits.load(Ordering::Relaxed));

    let data = AtomicU32::new(0);
    let ready = AtomicBool::new(false);
    thread::scope(|s| {
        s.spawn(|| {
            data.store(42, Ordering::Relaxed);
            ready.store(true, Ordering::Release);
        });
        while !ready.load(Ordering::Acquire) {
            std::hint::spin_loop();
        }
        println!("{}", data.load(Ordering::Relaxed));
    });
}
```

```output for=atomic
400000
Ok(400000) 0
42
```

- Verdict: stdlib. `std::sync::atomic` has atomic integers, `AtomicBool` and `AtomicPtr`. Each operation takes an `Ordering`, with the same meanings as in C++: `Relaxed`, `Release`, `Acquire`, `AcqRel` and `SeqCst`.
- Safe Rust has no data races. The compiler does not let two threads use a plain variable unless one of them only reads, so shared counters must be atomic or behind a `Mutex`.
- A relaxed counter is enough when only the final total matters. The flag needs `Release` and `Acquire`, so that the value stored before the flag is visible after the flag.
