```rust run id=copy-on-write
use std::sync::atomic::{AtomicUsize, Ordering};
use std::sync::{Arc, Mutex};
use std::thread;

struct CopyOnWriteList {
    items: Mutex<Arc<Vec<String>>>, // guards only the pointer
}

impl CopyOnWriteList {
    fn add(&self, item: String) {
        let mut current = self.items.lock().unwrap();
        let mut next = Vec::clone(&current); // copy
        next.push(item); // change the copy
        *current = Arc::new(next); // publish it
    }

    fn snapshot(&self) -> Arc<Vec<String>> {
        Arc::clone(&self.items.lock().unwrap())
    }
}

fn main() {
    let listeners = Arc::new(CopyOnWriteList {
        items: Mutex::new(Arc::new(vec!["listener 0".to_string()])),
    });
    let first = listeners.snapshot();
    let iterations = Arc::new(AtomicUsize::new(0));
    let errors = AtomicUsize::new(0);
    let mut threads = Vec::new();
    for _ in 0..4 {
        let (listeners, iterations) = (Arc::clone(&listeners), Arc::clone(&iterations));
        threads.push(thread::spawn(move || {
            for _ in 0..1000 {
                for _name in listeners.snapshot().iter() {
                    // call each listener; the lock is not held here
                }
                iterations.fetch_add(1, Ordering::SeqCst);
            }
        }));
    }
    {
        let listeners = Arc::clone(&listeners);
        threads.push(thread::spawn(move || {
            for n in 1..=100 {
                listeners.add(format!("listener {}", n));
            }
        }));
    }
    for t in threads {
        t.join().unwrap();
    }
    println!("iterations: {}", iterations.load(Ordering::SeqCst));
    println!("iterations that failed: {}", errors.load(Ordering::SeqCst));
    println!("listeners at the end: {}", listeners.snapshot().len());
    println!("length of the first snapshot: {}", first.len());
}
```

```output
iterations: 4000
iterations that failed: 0
listeners at the end: 101
length of the first snapshot: 1
```

- Verdict: idiom. `CopyOnWriteList` keeps an `Arc<Vec<String>>`; `add` clones the vector and replaces the `Arc`, and a reader holds its own `Arc` to the old vector while it iterates.
