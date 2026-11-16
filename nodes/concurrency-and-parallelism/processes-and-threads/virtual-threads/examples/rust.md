```rust run id=thread-pool
use std::sync::atomic::{AtomicU64, AtomicUsize, Ordering};
use std::thread;
use std::time::Duration;

const TASKS: usize = 1_000;
const WORKERS: usize = 10;

fn main() {
    let next = AtomicUsize::new(0);
    let sum = AtomicU64::new(0);
    // Ten OS threads take tasks from a shared counter until none are left.
    thread::scope(|s| {
        for _ in 0..WORKERS {
            s.spawn(|| loop {
                let n = next.fetch_add(1, Ordering::Relaxed);
                if n >= TASKS {
                    break;
                }
                thread::sleep(Duration::from_millis(10)); // blocks this OS thread
                sum.fetch_add(n as u64, Ordering::Relaxed);
            });
        }
    });
    println!("tasks: {TASKS}, sum: {}", sum.load(Ordering::Relaxed));
    println!("OS threads: {WORKERS}, so never more than {WORKERS} at once");
}
```

```output for=thread-pool
tasks: 1000, sum: 499500
OS threads: 10, so never more than 10 at once
```

- Verdict: absent. A Rust thread is an OS thread, and the standard library has no M:N scheduler. The two alternatives are a fixed set of OS threads, as here, and `async` tasks with an executor from a library.
- Ten scoped threads take task numbers from an atomic counter. The limit of ten comes from the number of threads, so the program needs no semaphore.
- `thread::sleep` blocks the whole OS thread. With ten threads, the 1,000 waits of 10 ms take about one second in total.
- `thread::scope` joins the ten threads before `main` reads the sum.
