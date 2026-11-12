```rust run id=future-pattern
use std::collections::HashMap;
use std::sync::{Arc, Barrier};
use std::thread;

fn main() {
    let items = ["apple", "bread", "cheese"];
    let prices: Arc<HashMap<&str, i32>> = Arc::new(HashMap::from([("apple", 12), ("bread", 7), ("cheese", 30)]));
    // The lookups wait here with the caller, so the output does not depend on timing.
    let meeting = Arc::new(Barrier::new(4));

    // Each call returns a handle at once. The lookup runs on its own thread.
    let futures: Vec<thread::JoinHandle<i32>> = items
        .iter()
        .map(|&item| {
            let (prices, meeting) = (Arc::clone(&prices), Arc::clone(&meeting));
            thread::spawn(move || {
                meeting.wait(); // returns when all 3 lookups and the caller are here
                prices[item]
            })
        })
        .collect();
    println!("lookups started: {}", futures.len());
    let any_done = futures.iter().any(|f| f.is_finished());
    println!("done right after the calls: {}", if any_done { "True" } else { "False" });
    meeting.wait(); // the caller is free to do other work here
    println!("all three lookups ran at once: True");
    let results: Vec<i32> = futures.into_iter().map(|f| f.join().unwrap()).collect(); // wait for each result

    println!("prices: {:?}", results);
    println!("total: {}", results.iter().sum::<i32>());
}
```

```output
lookups started: 3
done right after the calls: False
all three lookups ran at once: True
prices: [12, 7, 30]
total: 49
```

- Verdict: stdlib. `thread::spawn` returns a `JoinHandle<i32>` at once; `is_finished` checks it and `join` waits for the value.
