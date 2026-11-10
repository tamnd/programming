```rust run id=safe-leaks
use std::collections::VecDeque;
use std::mem;
use std::sync::Mutex;
use std::sync::atomic::{AtomicUsize, Ordering};

static DROPS: AtomicUsize = AtomicUsize::new(0);

struct Tracked;

impl Drop for Tracked {
    fn drop(&mut self) {
        DROPS.fetch_add(1, Ordering::SeqCst);
    }
}

// Leaks: a static Vec is reachable until the process ends.
static HISTORY: Mutex<Vec<String>> = Mutex::new(Vec::new());

// Fixed: a static queue that drops the oldest entry above 100.
static RECENT: Mutex<VecDeque<String>> = Mutex::new(VecDeque::new());

fn record(line: String) {
    HISTORY.lock().unwrap().push(line.clone());
    let mut recent = RECENT.lock().unwrap();
    if recent.len() == 100 {
        recent.pop_front();
    }
    recent.push_back(line);
}

fn main() {
    // Box::leak is safe: the String lives for the rest of the program.
    let config: &'static mut String = Box::leak(Box::new(String::from("mode=fast")));
    config.push_str(",retries=3");
    println!("leaked config: {config}");

    // mem::forget is safe too: the destructor never runs.
    drop(Tracked);
    mem::forget(Tracked);
    println!("drops run: {}", DROPS.load(Ordering::SeqCst));

    for i in 0..1000 {
        record(format!("request {i}"));
    }
    println!("history entries: {}", HISTORY.lock().unwrap().len());
    println!("recent entries: {}", RECENT.lock().unwrap().len());
}
```

```output
leaked config: mode=fast,retries=3
drops run: 1
history entries: 1000
recent entries: 100
```

- Verdict: native. Ownership frees each value when its owner goes out of scope. Rust does not count a leak as a memory safety error, so `Box::leak` and `mem::forget` are safe functions.
- Two `Tracked` values exist, but only one destructor runs, because `mem::forget` takes the other one without dropping it.
- A `static` collection has no owner that goes out of scope. Give it a limit, as `RECENT` does, or clear it at a known point.
