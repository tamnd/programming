```rust run id=hsha
use std::sync::atomic::{AtomicU64, Ordering};
use std::sync::mpsc;
use std::sync::{Arc, Barrier, Mutex};
use std::thread;
use std::time::Duration;

fn main() {
    let (tx, rx) = mpsc::channel::<u64>(); // the queueing layer
    let rx = Arc::new(Mutex::new(rx)); // shared by the sync workers
    let gate = Arc::new(Barrier::new(5)); // lets the demo hold the sync layer
    let done = Arc::new(AtomicU64::new(0));
    let sum = Arc::new(AtomicU64::new(0));
    let pending = Arc::new(AtomicU64::new(0)); // requests waiting in the channel

    let workers: Vec<_> = (0..4)
        .map(|_| {
            let (rx, gate, done, sum, pending) =
                (rx.clone(), gate.clone(), done.clone(), sum.clone(), pending.clone());
            thread::spawn(move || {
                gate.wait();
                loop {
                    let next = rx.lock().unwrap().recv(); // the sync layer blocks here
                    let Ok(request) = next else { return };
                    pending.fetch_sub(1, Ordering::Relaxed);
                    thread::sleep(Duration::from_millis(1)); // a blocking call
                    done.fetch_add(1, Ordering::Relaxed);
                    sum.fetch_add(request, Ordering::Relaxed);
                }
            })
        })
        .collect();

    // Async layer: a short handler that never blocks.
    let source_pending = pending.clone();
    let source = thread::spawn(move || {
        let mut received = 0;
        let on_event = |id: u64, received: &mut u64| {
            *received += 1;
            source_pending.fetch_add(1, Ordering::Relaxed);
            tx.send(id).unwrap();
        };
        for id in 0..100 {
            on_event(id, &mut received);
        }
        (received, tx)
    });
    let (received, tx) = source.join().unwrap();
    let queued = pending.load(Ordering::Relaxed);
    println!("events received by the async layer: {}", received);
    println!("requests in the queueing layer before the sync layer starts: {}", queued);
    drop(tx);
    gate.wait();
    for w in workers {
        w.join().unwrap();
    }
    println!("requests done by the sync layer: {}", done.load(Ordering::Relaxed));
    println!("sum of request ids: {}", sum.load(Ordering::Relaxed));
}
```

```output
events received by the async layer: 100
requests in the queueing layer before the sync layer starts: 100
requests done by the sync layer: 100
sum of request ids: 4950
```

- Verdict: stdlib. `mpsc::channel` is unbounded, so `send` never blocks. It has one receiver, so the workers share it behind a `Mutex`.
