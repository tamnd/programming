```rust run id=producer-consumer
use std::sync::mpsc;
use std::sync::{Arc, Mutex};
use std::thread;

fn main() {
    let (sender, receiver) = mpsc::sync_channel::<u64>(4); // send waits when 4 items wait
    let receiver = Arc::new(Mutex::new(receiver));
    let consumed = Arc::new(Mutex::new(Vec::new()));

    let producers: Vec<_> = [0u64, 500]
        .into_iter()
        .map(|start| {
            let sender = sender.clone();
            thread::spawn(move || {
                for item in start..start + 500 {
                    sender.send(item).unwrap();
                }
            })
        })
        .collect();
    drop(sender); // the stream ends when every sender is dropped

    let consumers: Vec<_> = (0..2)
        .map(|_| {
            let receiver = Arc::clone(&receiver);
            let consumed = Arc::clone(&consumed);
            thread::spawn(move || loop {
                let message = receiver.lock().unwrap().recv();
                match message {
                    Ok(item) => consumed.lock().unwrap().push(item),
                    Err(_) => break,
                }
            })
        })
        .collect();
    for t in producers.into_iter().chain(consumers) {
        t.join().unwrap();
    }

    let mut items = consumed.lock().unwrap().clone();
    items.sort();
    let once = items == (0..1000).collect::<Vec<u64>>();
    println!("items consumed: {}", items.len());
    println!("sum of items: {}", items.iter().sum::<u64>());
    println!("each item consumed once: {}", if once { "True" } else { "False" });
}
```

```output
items consumed: 1000
sum of items: 499500
each item consumed once: True
```

- Verdict: stdlib. `mpsc::sync_channel(4)` is the bounded buffer. The receiver has one owner, so the consumers share it behind `Arc<Mutex<_>>`.
