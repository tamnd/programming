```rust run id=immutable-object
use std::sync::atomic::{AtomicUsize, Ordering};
use std::sync::{Arc, Mutex};
use std::thread;

struct Settings {
    port: u32,
    tag: String,
}

fn main() {
    let first = Arc::new(Settings { port: 8000, tag: "port 8000".to_string() });
    // Arc gives shared references only, so no thread can change a Settings.
    let current = Arc::new(Mutex::new(Arc::clone(&first))); // guards only the pointer
    let reads = Arc::new(AtomicUsize::new(0));
    let mixed = Arc::new(AtomicUsize::new(0));
    let mut threads = Vec::new();
    for _ in 0..4 {
        let (current, reads, mixed) = (Arc::clone(&current), Arc::clone(&reads), Arc::clone(&mixed));
        threads.push(thread::spawn(move || {
            for _ in 0..1000 {
                let s = Arc::clone(&current.lock().unwrap()); // a consistent snapshot
                thread::yield_now();
                if s.tag != format!("port {}", s.port) {
                    mixed.fetch_add(1, Ordering::SeqCst);
                }
                reads.fetch_add(1, Ordering::SeqCst);
            }
        }));
    }
    {
        let current = Arc::clone(&current);
        threads.push(thread::spawn(move || {
            for port in 8001..=8100 {
                let next = Arc::new(Settings { port, tag: format!("port {}", port) });
                *current.lock().unwrap() = next; // a new object, never a change
                thread::yield_now();
            }
        }));
    }
    for t in threads {
        t.join().unwrap();
    }
    let last = Arc::clone(&current.lock().unwrap());
    let unchanged = first.port == 8000 && first.tag == "port 8000";
    println!("reads: {}", reads.load(Ordering::SeqCst));
    println!("reads that saw a mixed pair: {}", mixed.load(Ordering::SeqCst));
    println!("settings published: {}", 1 + last.port - first.port);
    println!("first settings unchanged: {}", if unchanged { "True" } else { "False" });
}
```

```output
reads: 4000
reads that saw a mixed pair: 0
settings published: 101
first settings unchanged: True
```

- Verdict: native. `Arc<Settings>` gives the threads shared references only, so the compiler rejects a write to a field of a shared `Settings`; a mutex guards only the `Arc` pointer.
