```rust run id=pipeline-concurrency
use std::sync::atomic::{AtomicBool, Ordering};
use std::sync::mpsc::sync_channel;
use std::sync::{Arc, Mutex};
use std::thread;

const ITEMS: i64 = 20;
const WORKERS: usize = 3;

fn main() {
    // Bounded channels: a send waits while the buffer is full.
    let (numbers_tx, numbers_rx) = sync_channel::<i64>(1); // source -> square workers
    let (squares_tx, squares_rx) = sync_channel::<i64>(1); // square workers -> sink
    let sink_started = Arc::new(AtomicBool::new(false));

    let started = Arc::clone(&sink_started);
    let source = thread::spawn(move || {
        for n in 1..=ITEMS {
            numbers_tx.send(n).unwrap();
        }
        // Each stage can hold only a few values, so the sink must already be running.
        started.load(Ordering::SeqCst)
        // numbers_tx is dropped here, which closes the channel
    });
    // The workers share one receiver (fan-out) and each owns a sender to the sink (fan-in).
    let numbers_rx = Arc::new(Mutex::new(numbers_rx));
    let workers: Vec<_> = (0..WORKERS)
        .map(|_| {
            let (rx, tx) = (Arc::clone(&numbers_rx), squares_tx.clone());
            thread::spawn(move || loop {
                let next = rx.lock().unwrap().recv();
                match next {
                    Ok(n) => tx.send(n * n).unwrap(),
                    Err(_) => break, // the source is done
                }
            })
        })
        .collect();
    drop(squares_tx); // the channel closes when the last worker drops its sender

    let (mut received, mut total) = (0, 0);
    for v in squares_rx {
        sink_started.store(true, Ordering::SeqCst);
        received += 1;
        total += v;
    }
    let overlap = source.join().unwrap();
    for w in workers {
        w.join().unwrap();
    }

    println!("items from the source: {}", ITEMS);
    println!("square workers: {}", WORKERS);
    println!("squares that reached the sink: {}", received);
    println!("sum of squares: {}", total);
    println!("the sink got a value before the source sent its last: {}", if overlap { "True" } else { "False" });
}
```

```output
items from the source: 20
square workers: 3
squares that reached the sink: 20
sum of squares: 2870
the sink got a value before the source sent its last: True
```

- Verdict: stdlib. `std::sync::mpsc::sync_channel(1)` links the stages. The workers share the receiver behind a `Mutex`, and dropping the senders closes each channel.
