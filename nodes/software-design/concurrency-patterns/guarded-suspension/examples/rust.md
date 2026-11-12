```rust run id=guarded-suspension
use std::sync::{Arc, Condvar, Mutex};
use std::thread;

#[derive(Default)]
struct State {
    level: i32,
    lowest: i32, // the lowest level seen after a take
    filled: i32,
    taken: i32,
    served: i32,
}

struct Tank {
    state: Mutex<State>,
    cond: Condvar,
}

impl Tank {
    fn take(&self, amount: i32) {
        let guard = self.state.lock().unwrap();
        // Guarded suspension: wait while the guard is false.
        let mut s = self.cond.wait_while(guard, |s| s.level < amount).unwrap();
        s.level -= amount;
        s.lowest = s.lowest.min(s.level);
        s.taken += amount;
        s.served += 1;
    }

    fn fill(&self, amount: i32) {
        let mut s = self.state.lock().unwrap();
        s.level += amount;
        s.filled += amount;
        self.cond.notify_all(); // each waiting taker checks its guard again
    }
}

fn main() {
    let tank = Arc::new(Tank { state: Mutex::new(State::default()), cond: Condvar::new() });
    let takers: Vec<_> = [5, 3, 8]
        .into_iter()
        .map(|a| {
            let tank = Arc::clone(&tank);
            thread::spawn(move || tank.take(a))
        })
        .collect();
    for _ in 0..10 {
        tank.fill(2);
    }
    for t in takers {
        t.join().unwrap();
    }

    let s = tank.state.lock().unwrap();
    println!("units filled: {}", s.filled);
    println!("units taken: {}", s.taken);
    println!("takers served: {}", s.served);
    println!("level never below 0: {}", if s.lowest >= 0 { "True" } else { "False" });
    println!("units left: {}", s.level);
}
```

```output
units filled: 20
units taken: 16
takers served: 3
level never below 0: True
units left: 4
```

- Verdict: stdlib. `Condvar::wait_while` waits while the guard is false and returns the `MutexGuard`.
