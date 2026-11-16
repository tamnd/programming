```rust run id=stop-a-thread
use std::sync::atomic::{AtomicBool, Ordering};
use std::sync::Arc;
use std::thread;
use std::time::{Duration, Instant};

fn main() {
    let stop = Arc::new(AtomicBool::new(false));
    let flag = Arc::clone(&stop);

    let worker = thread::spawn(move || {
        while !flag.load(Ordering::Acquire) {
            // Sleeps for up to 10 s; unpark() wakes it early.
            thread::park_timeout(Duration::from_secs(10));
        }
        println!("worker: stop requested, cleaning up");
    });

    thread::sleep(Duration::from_millis(100));
    let start = Instant::now();
    stop.store(true, Ordering::Release);
    worker.thread().unpark();
    worker.join().unwrap();
    println!("worker stopped in under 1 s: {}", start.elapsed() < Duration::from_secs(1));
}
```

```output for=stop-a-thread
worker: stop requested, cleaning up
worker stopped in under 1 s: true
```

- Verdict: absent. The `std::thread` module has functions to spawn, sleep, park and join threads, but none to stop another thread. The example uses an `AtomicBool` as a stop flag.
- `thread::park_timeout` "Blocks unless or until the current thread’s token is made available or the specified duration has been reached". The main thread sets the flag and then calls `unpark`, so the worker wakes at once.
- The worker checks the flag in a loop, because a parked thread can also wake for other reasons.
- When the main thread of a Rust program ends, other threads are not stopped cleanly: "any destructors on the remaining threads’ stacks may not be executed". The example joins the worker first.
