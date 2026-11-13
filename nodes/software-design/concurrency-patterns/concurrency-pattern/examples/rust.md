```rust run id=three-ways
use std::sync::mpsc;
use std::sync::Mutex;
use std::thread;

const THREADS: usize = 4;
const STEPS: usize = 1000;

fn main() {
    // 1. Synchronization: a mutex owns the shared counter; the type system requires the lock.
    let count = Mutex::new(0);
    thread::scope(|s| {
        for _ in 0..THREADS {
            s.spawn(|| {
                for _ in 0..STEPS {
                    *count.lock().unwrap() += 1;
                }
            });
        }
    });
    println!("lock around a shared counter: {}", count.into_inner().unwrap());

    // 2. Confinement: one owner thread keeps the counter; the others send it messages.
    let (tx, rx) = mpsc::channel();
    let owner = thread::spawn(move || rx.iter().sum::<usize>()); // ends when all senders are gone
    thread::scope(|s| {
        for _ in 0..THREADS {
            let tx = tx.clone();
            s.spawn(move || {
                for _ in 0..STEPS {
                    tx.send(1).unwrap();
                }
            });
        }
    });
    drop(tx);
    println!("one owner thread, the others send messages: {}", owner.join().unwrap());

    // 3. No sharing: each thread gets its own slot, and the results are combined after the join.
    let mut parts = vec![0usize; THREADS];
    thread::scope(|s| {
        for part in parts.iter_mut() {
            s.spawn(move || {
                for _ in 0..STEPS {
                    *part += 1; // this thread has the only mutable reference to its slot
                }
            });
        }
    });
    println!("own result for each thread, combined after the join: {}", parts.iter().sum::<usize>());
}
```

```output
lock around a shared counter: 4000
one owner thread, the others send messages: 4000
own result for each thread, combined after the join: 4000
```

- Verdict: native. The counter lives inside a `Mutex`, so it cannot be changed without the lock. An `mpsc` channel moves the messages to the owner thread, and `iter_mut` gives each scoped thread the only mutable reference to its slot.
