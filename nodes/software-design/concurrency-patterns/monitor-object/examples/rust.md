```rust run id=monitor
use std::collections::VecDeque;
use std::sync::{Arc, Condvar, Mutex};
use std::thread;

struct State {
    items: VecDeque<u64>,
    puts: u64,
    gets: u64,
    largest: usize,
}

// The Mutex owns the state, so no method can touch it without the lock.
struct MessageQueue {
    state: Mutex<State>,
    not_full: Condvar,
    not_empty: Condvar,
    capacity: usize,
}

impl MessageQueue {
    fn put(&self, message: u64) {
        let mut s = self.state.lock().unwrap();
        // wait_while releases the lock while the condition holds.
        s = self.not_full.wait_while(s, |s| s.items.len() == self.capacity).unwrap();
        s.items.push_back(message);
        s.puts += 1;
        s.largest = s.largest.max(s.items.len());
        self.not_empty.notify_one();
    }

    fn get(&self) -> u64 {
        let mut s = self.state.lock().unwrap();
        s = self.not_empty.wait_while(s, |s| s.items.is_empty()).unwrap();
        let message = s.items.pop_front().unwrap();
        s.gets += 1;
        self.not_full.notify_one();
        message
    }
}

fn main() {
    let q = Arc::new(MessageQueue {
        state: Mutex::new(State { items: VecDeque::new(), puts: 0, gets: 0, largest: 0 }),
        not_full: Condvar::new(),
        not_empty: Condvar::new(),
        capacity: 2,
    });
    let mut producers = Vec::new();
    for p in 0..4u64 {
        let q = Arc::clone(&q);
        producers.push(thread::spawn(move || {
            for i in 0..250 {
                q.put(p * 250 + i);
            }
        }));
    }
    let consumers: Vec<_> = (0..2)
        .map(|_| {
            let q = Arc::clone(&q);
            thread::spawn(move || (0..500).map(|_| q.get()).sum::<u64>())
        })
        .collect();
    for t in producers {
        t.join().unwrap();
    }
    let sum: u64 = consumers.into_iter().map(|t| t.join().unwrap()).sum();
    let s = q.state.lock().unwrap();
    println!("messages put: {}", s.puts);
    println!("messages got: {}", s.gets);
    println!("sum of messages got: {}", sum);
    println!("queue never held more than 2: {}", if s.largest <= 2 { "True" } else { "False" });
}
```

```output
messages put: 1000
messages got: 1000
sum of messages got: 499500
queue never held more than 2: True
```

- Verdict: stdlib. `Mutex` and `Condvar` are in `std::sync`. The `Mutex` owns the state, so the compiler rejects access to the items without the lock.
