```rust run id=thread-confinement
use std::collections::HashMap;
use std::thread;

fn main() {
    let words = ["a", "b", "c", "d", "e"];
    let chunks = 4;
    let handles: Vec<_> = (0..chunks)
        .map(|_| {
            thread::spawn(move || {
                let mut counts: HashMap<&str, usize> = HashMap::new(); // owned by this thread
                for _ in 0..2000 {
                    for w in words {
                        *counts.entry(w).or_insert(0) += 1;
                    }
                }
                counts // ownership moves to the thread that joins
            })
        })
        .collect();

    let mut total: HashMap<&str, usize> = HashMap::new();
    for h in handles {
        for (w, n) in h.join().unwrap() {
            *total.entry(w).or_insert(0) += n;
        }
    }
    println!("chunks: {}", chunks);
    println!("words counted: {}", total.values().sum::<usize>());
    println!("distinct words: {}", total.len());
    println!("count of 'a': {}", total["a"]);
}
```

```output
chunks: 4
words counted: 40000
distinct words: 5
count of 'a': 8000
```

- Verdict: native. The `move` closure gives each thread ownership of its `HashMap`, and `join` moves the map to the main thread.
