```rust run id=read-write-lock
use std::sync::atomic::{AtomicUsize, Ordering};
use std::sync::{Arc, RwLock};
use std::thread;
use std::time::{Duration, Instant};

fn main() {
    // The writer keeps the two values equal.
    let lock = Arc::new(RwLock::new((0u64, 0u64)));

    // Part 1: two readers hold the read lock at the same time.
    let at_meeting = Arc::new(AtomicUsize::new(0));
    let meets: Vec<_> = (0..2)
        .map(|_| {
            let lock = Arc::clone(&lock);
            let at_meeting = Arc::clone(&at_meeting);
            thread::spawn(move || {
                let _guard = lock.read().unwrap();
                at_meeting.fetch_add(1, Ordering::SeqCst);
                let deadline = Instant::now() + Duration::from_secs(5);
                while at_meeting.load(Ordering::SeqCst) < 2 && Instant::now() < deadline {
                    thread::yield_now();
                }
                at_meeting.load(Ordering::SeqCst) == 2
            })
        })
        .collect();
    let met = meets.into_iter().all(|h| h.join().unwrap());

    // Part 2: readers and a writer share the pair.
    let inside = Arc::new(AtomicUsize::new(0));
    let reads = Arc::new(AtomicUsize::new(0));
    let mixed = Arc::new(AtomicUsize::new(0));
    let mut readers = Vec::new();
    for _ in 0..4 {
        let (lock, inside) = (Arc::clone(&lock), Arc::clone(&inside));
        let (reads, mixed) = (Arc::clone(&reads), Arc::clone(&mixed));
        readers.push(thread::spawn(move || {
            for _ in 0..1000 {
                let guard = lock.read().unwrap();
                inside.fetch_add(1, Ordering::SeqCst);
                let x = guard.0;
                thread::yield_now(); // give the writer a chance to run here
                let y = guard.1;
                inside.fetch_sub(1, Ordering::SeqCst);
                reads.fetch_add(1, Ordering::SeqCst);
                if x != y {
                    mixed.fetch_add(1, Ordering::SeqCst);
                }
            }
        }));
    }
    let writer = {
        let (lock, inside) = (Arc::clone(&lock), Arc::clone(&inside));
        thread::spawn(move || {
            let (mut writes, mut overlap) = (0, 0);
            for i in 1..=100u64 {
                let mut guard = lock.write().unwrap();
                if inside.load(Ordering::SeqCst) > 0 {
                    overlap += 1;
                }
                guard.0 = i;
                thread::yield_now();
                guard.1 = i;
                writes += 1;
            }
            (writes, overlap)
        })
    };
    for r in readers {
        r.join().unwrap();
    }
    let (writes, overlap) = writer.join().unwrap();

    println!("two readers inside at once: {}", if met { "True" } else { "False" });
    println!("reads: {}", reads.load(Ordering::SeqCst));
    println!("reads that saw a mixed pair: {}", mixed.load(Ordering::SeqCst));
    println!("writes: {}", writes);
    println!("writes that ran with a reader inside: {}", overlap);
}
```

```output
two readers inside at once: True
reads: 4000
reads that saw a mixed pair: 0
writes: 100
writes that ran with a reader inside: 0
```

- Verdict: stdlib. `std::sync::RwLock` gives `read` and `write`, and each guard releases the lock when it is dropped.
