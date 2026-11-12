```rust run id=leader-followers
use std::collections::VecDeque;
use std::sync::{Arc, Condvar, Mutex};
use std::thread;
use std::time::Duration;

#[derive(Default)]
struct Roles {
    leader_present: bool,
    waiting: u32,
    most_waiting: u32,
    processed: u32,
    sum: i64,
}

struct ThreadPool {
    roles: Mutex<Roles>,
    synchronizer: Condvar,
    handle_set: Mutex<VecDeque<i64>>, // stands for the handle set
}

impl ThreadPool {
    fn join(&self) {
        loop {
            {
                let r = self.roles.lock().unwrap(); // follower role
                let mut r = self.synchronizer.wait_while(r, |r| r.leader_present).unwrap();
                r.leader_present = true;
                r.waiting += 1;
                r.most_waiting = r.most_waiting.max(r.waiting);
            }
            let event = self.handle_set.lock().unwrap().pop_front().unwrap(); // leader role
            self.promote_new_leader();
            if event < 0 {
                return;
            }
            thread::sleep(Duration::from_millis(1)); // processing role
            let mut r = self.roles.lock().unwrap();
            r.processed += 1;
            r.sum += event;
        }
    }

    fn promote_new_leader(&self) {
        let mut r = self.roles.lock().unwrap();
        r.waiting -= 1;
        r.leader_present = false;
        self.synchronizer.notify_one();
    }
}

fn main() {
    let mut events: VecDeque<i64> = (0..100).collect();
    events.extend([-1; 4]); // one stop event per thread
    let pool = Arc::new(ThreadPool {
        roles: Mutex::new(Roles::default()),
        synchronizer: Condvar::new(),
        handle_set: Mutex::new(events),
    });
    let threads: Vec<_> = (0..4)
        .map(|_| {
            let pool = Arc::clone(&pool);
            thread::spawn(move || pool.join())
        })
        .collect();
    for t in threads {
        t.join().unwrap();
    }
    let r = pool.roles.lock().unwrap();
    println!("events processed: {}", r.processed);
    println!("sum of event ids: {}", r.sum);
    println!("most threads waiting on the handle set at once: {}", r.most_waiting);
}
```

```output
events processed: 100
sum of event ids: 4950
most threads waiting on the handle set at once: 1
```

- Verdict: stdlib. A `Condvar` is the synchronizer, and `wait_while` keeps a follower asleep while a leader is present.
