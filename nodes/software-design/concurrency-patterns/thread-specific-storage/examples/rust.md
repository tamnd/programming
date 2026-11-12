```rust run id=tss
use std::cell::Cell;
use std::sync::atomic::{AtomicU32, Ordering};
use std::sync::Arc;
use std::thread;

thread_local! {
    static LAST_ERROR: Cell<i32> = Cell::new(0); // one name, one value per thread
}

fn fake_call(code: i32) -> i32 {
    LAST_ERROR.with(|e| e.set(code)); // the call sets its thread's error
    thread::yield_now(); // let another thread run between the set and the test
    -1
}

fn main() {
    let wrong_reads = Arc::new(AtomicU32::new(0));
    let threads: Vec<_> = (0..4)
        .map(|index| {
            let wrong_reads = Arc::clone(&wrong_reads);
            thread::spawn(move || {
                let code = index + 1;
                for _ in 0..1000 {
                    if fake_call(code) == -1 && LAST_ERROR.with(|e| e.get()) != code {
                        wrong_reads.fetch_add(1, Ordering::Relaxed);
                    }
                }
                LAST_ERROR.with(|e| e.get())
            })
        })
        .collect();
    let seen: Vec<i32> = threads.into_iter().map(|t| t.join().unwrap()).collect();
    println!("threads: {}", seen.len());
    println!("calls per thread: {}", 1000);
    println!("reads that saw another thread's error: {}", wrong_reads.load(Ordering::Relaxed));
    println!("error code seen by each thread: {:?}", seen);
}
```

```output
threads: 4
calls per thread: 1000
reads that saw another thread's error: 0
error code seen by each thread: [1, 2, 3, 4]
```

- Verdict: stdlib. The `thread_local!` macro declares `LAST_ERROR`, and `with` reaches the value of the calling thread.
